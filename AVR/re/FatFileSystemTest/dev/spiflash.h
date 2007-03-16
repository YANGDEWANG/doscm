#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_
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
#include <configure.h>
#include <avr/pgmspace.h>
enum SPIFlashManufacturer
{
	SFM_Eon,
	SFM_Atmel,
};
typedef uint8 SFM;
enum SPI_FLASH
{
	en,
};
typedef struct SPI_FLASH_COMMAND
{
	_uint8 WriteEnable;
	_uint8 WriteDisable;
	_uint8 SPIFlashReadStatusRegister;
	_uint8 SPIFlashWriteStatusRegister;
	_uint8 ReadData;
	_uint8 FastRead;
	_uint8 PageProgram;
	_uint8 SectorErase;
	_uint8 EraseAll;
	_uint8 ReadIdentification;
}SPIFlashCommand;
typedef SPIFlashCommand prog_SPIFlashCommand PROGMEM;
typedef struct SPI_FLASHID
{
	_uint8  ManufacturerID;
	_uint8 DeviceID1;
	_uint8 DeviceID2;
}SPIFlashID;
typedef struct SPI_FLASH_INFO
{
	SPIFlashID id;
	_uint8	PageSize;
	_uint16	PageCount;
}SPIFlashInfo;
typedef SPIFlashInfo prog_SPIFlashInfo PROGMEM;

extern prog_SPIFlashCommand SPIFlashCommandInfo[1];
extern prog_char SPIFlashManufacturerID[2];
extern prog_SPIFlashInfo SPIFlashInfoTabel[1];
extern SPIFlashCommand SPIFlashCom;
extern uint32 SPIFlashAddress;
extern volatile bool spiIdle;

void SPIFlashInit();
void SPIFlashSetManufacturer(SFM m);
//void inline WaitSPIIdle();
void static inline WaitSPIIdle()
{
	while(!spiIdle);
}
void SPIFlashWriteStatusRegister(uint8 srdata);
uint8 SPIFlashReadStatusRegister(uint8 devC);
bool SPIFlashPageProgram(uint8* dat,uint8 count);
bool SPIFlashRead(uint8* dat,uint8 count,uint8 devC);
SPIFlashID SPIFlashReadID(uint8 devC);
void SPIFlashErasureAll();
#define SPIFlashCleanWriteProtect() SPIFlashWriteStatusRegister(0)

#endif//_SPI_FLASH_H_
