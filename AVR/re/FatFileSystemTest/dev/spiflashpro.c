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
#include <avr/pgmspace.h>
#include <spi.h>
//#include <intrins.h>
#include <avr/interrupt.h>
#include "spiFlash.h"
uint8 SPIFlashManufacturerIDIndex;
uint16 SPIFlashDevNameIndex;
SPIFlashInfo SPIFlashInfoData;

void LoadSPIFlashInfoData()
{
	memcpy_P(&SPIFlashInfoData,&SPIFlashInfoTabel[SPIFlashDevNameIndex],sizeof(SPIFlashInfo));
}
void SPIFlashCopy()
{
	uint8 buffer[256];
	uint16 i = 0;
	for(;i<SPIFlashInfoData.PageCount;i++)
	{
		SPIFlashAddress = SPIFlashInfoData.PageSize*i;
		SPIFlashRead(buffer,SPIFlashInfoData.PageSize,0);
		SPIFlashPageProgram(buffer,SPIFlashInfoData.PageSize);
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
		SPIFlashRead(buffers,SPIFlashInfoData.PageSize,0);
		devC = 1;
		for(;devC<9;devC++)
		{
			if(errMak&(1<<(devC-1)))
			{
				//is err dev
				break;
			}
			SPIFlashRead(buffer2,SPIFlashInfoData.PageSize,devC);
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
		SPIFlashSetManufacturer(i);
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
