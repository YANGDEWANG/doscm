/*
* 串口 FLASH
* 处理器类型：AVR
* 版本：1 日期：2007-1-17
* 作者：杨德王<yangdewang@gmail.com>

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.  Or, point your browser to
* http://www.gnu.org/copyleft/gpl.html
*/
#include <global.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <spi.h>
//#include <intrins.h>
#include <avr/interrupt.h>
#include <fs/fat.h>
#include "spiFlash.h"
uint8 SPIFlashManufacturerIDIndex;
uint16 SPIFlashDevNameIndex;
u32 SPIFlashSize;
SPIFlashInfo SPIFlashInfoData;
u8 SPIFlashBuffer[512];
File *SpiFlashFile;
static u8 workMode;
enum SPIFlashProWorkMode
{
	SFPWM_idle,
	SFPWM_writing,
	SFPWM_reading,
	SFPWM_Verifying,
};
void SPIFlashStartPro()
{
	if(workMode==SFPWM_idle)
	{
		if(SpiFlashFile->Size<=SPIFlashSize)
		{
			FileSetPosition(SpiFlashFile,0);
			SPIFlashAddress = 0;
			SPIFlashWorkMak = 0xff;
			workMode = SFPWM_writing;
		}
		else
		{
			//todo:showerr
		}
	}
}
void SPIFlashStartRead()
{
	if(workMode==SFPWM_idle)
	{
		FileSetPosition(SpiFlashFile,0);
		SPIFlashAddress = 0;
		SPIFlashWorkMak = 1;
		workMode = SFPWM_reading;
	}
}
void SPIFlashStartVerify()
{
	if(workMode==SFPWM_idle)
	{
		FileSetPosition(SpiFlashFile,0);
		SPIFlashAddress = 0;
		SPIFlashWorkMak = 0xff;
		workMode = SFPWM_Verifying;
	}
}
void LoadSPIFlashInfoData()
{
	memcpy_P(&SPIFlashInfoData,&SPIFlashInfoTabel[SPIFlashDevNameIndex],sizeof(SPIFlashInfo));
	SPIFlashSize = SPIFlashInfoData.PageCount*SPIFlashInfoData.PageSize;
}
void SPIFlashCopy()
{
	uint8 buffer[256];
	uint16 i = 0;
	for(;i<SPIFlashInfoData.PageCount;i++)
	{
		SPIFlashAddress = SPIFlashInfoData.PageSize*i;
		SPIFlashReadBegin(buffer,SPIFlashInfoData.PageSize,0);
		SPIFlashPageProgramBegin(buffer,SPIFlashInfoData.PageSize);
	}
}
void SPIFlashVerify()
{
	uint8 buffers[256];
	uint8 buffer2[256];
	uint16 i = 0;
	uint8 devC;
	uint8 bc;
	uint8 errMak = 0;
	for(;i<SPIFlashInfoData.PageCount;i++)
	{
		SPIFlashAddress = SPIFlashInfoData.PageSize*i;
		SPIFlashReadBegin(buffers,SPIFlashInfoData.PageSize,0);
		devC = 1;
		for(;devC<9;devC++)
		{
			if(errMak&(1<<(devC-1)))
			{
				//is err dev
				break;
			}
			SPIFlashReadBegin(buffer2,SPIFlashInfoData.PageSize,devC);
			for(bc=0;bc<SPIFlashInfoData.PageSize;bc++)
			{
				if(buffers[bc]!=buffer2[bc])
				{
					//have err
					errMak|=(1<<(devC-1));
					break;
				}
			}
		}
	}
}
bool SPIFlashQueryDevType()
{
	uint8 i = 0;
	uint16 j = 0;
	SPIFlashID id;

	for(;i<sizeof(SPIFlashCommandInfo);i++)
	{
		SPIFlashSetCommand(i);
		id = SPIFlashReadID(0);
		if(id.ManufacturerID==pgm_read_byte(SPIFlashManufacturerID+i))
		{
			break;
		}
	}
	SPIFlashManufacturerIDIndex = i;
	if(i<sizeof(SPIFlashCommandInfo))
	{
		for(;j<sizeof(SPIFlashInfoTabel);j++)
		{
			LoadSPIFlashInfoData(j);
			if(id.ManufacturerID==SPIFlashInfoData.id.ManufacturerID
				&&id.DeviceID1	==SPIFlashInfoData.id.DeviceID1
				&&id.DeviceID2	==SPIFlashInfoData.id.DeviceID2)
			{
				SPIFlashDevNameIndex = j;
				return true;
			}
		}
	}
	SPIFlashDevNameIndex = sizeof(SPIFlashInfoTabel);
	return false;
}
void pollingSpiFlash()
{
	switch(workMode)
	{
	case SFPWM_writing:
		{
			if(FatReadSector(SpiFlashFile,FatBuffer))
			{
				memcpy(SPIFlashBuffer,FatBuffer,512);
				//WaitSPIFlashIdle();
				SPIFlashPageProgramBegin(SPIFlashBuffer,256);
				//WaitSPIFlashIdle();
				SPIFlashPageProgramBegin(SPIFlashBuffer+256,256);
			}
			else
			{
				workMode = SFPWM_idle;
			}
			break;
		}
	case SFPWM_reading:
		{
			if(SPIFlashAddress<SPIFlashSize)
			{
				SPIFlashReadBegin(SPIFlashBuffer,512,1);
				WaitSPIFlashIdle();
				FatWriteSector(SpiFlashFile,SPIFlashBuffer);
			}
			else
			{
				workMode = SFPWM_idle;
			}
			break;
		}
	case SFPWM_Verifying:
		{
			if(SPIFlashAddress<SPIFlashSize)
			{
				FatReadSector(SpiFlashFile,FatBuffer);
				u8 i;
				for(i=0;i<8;i++)
				{
					if(SPIFlashWorkMak&(1<<i))
					{
						SPIFlashReadBegin(SPIFlashBuffer,512,i);
						WaitSPIFlashIdle();
						SPIFlashAddress-=512;
						if(memcmp(SPIFlashBuffer,FatBuffer,512)!=0)
						{
							SPIFlashWorkMak&=~(1<<i);
						}
					}
				}
				SPIFlashAddress+=512;
			}
			else
			{
				workMode = SFPWM_idle;
			}
			break;
		}
	}
}
