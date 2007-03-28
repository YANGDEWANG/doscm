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
#include <system/clock.h>
//#include <intrins.h>
#include <avr/interrupt.h>
#include "spiFlash.h"
SPIFlashCommand SPIFlashCom;
uint32 SPIFlashAddress;
static uint8* dataP;
static u16 dataCount;
#define RxDataP dataP
static bool  DontAutoDisableFlash;
static bool  saveRxData;
u8 SPIFlashWorkMak;
volatile bool spiIdle=true;
static uint8 SpiFlash[6];
#define SPIFlashCSDDR	DDRC
#define SPIFlashCSPORT	PORTC
#define SPIFlashMSDDR	DDRD
#define SPIFlashMSPORT	PORTD
#define SPIFlashMSPIN	4

//返回成功的mak位
static uint8 WaitSlFlashIdelMak(uint8 mak);
static bool WaitFlashIdel(uint8 devC);
static void inline activeSlFlash(u8 mak);
static void inline disableAllSlFlash();
static void inline activeMsFlash();
static void activeFlash(uint8 devC);
static void inline WriteEnable();
static void inline WriteDisable();
static void startSend(u16 count);
static void startRead(u16 count);
static void WriteCom(uint8 com);
static void SetPageProgramAdd();
static void SetReadAdd(uint8 devC);

void SPIFlashInit();
void SPIFlashSetCommand(SFM m);
void inline WaitSPIIdle();
void SPIFlashWriteStatusRegister(uint8 srdata);
uint8 SPIFlashReadStatusRegister(uint8 devC);
bool SPIFlashPageProgram(uint8* SpiFlash,u16 count);
bool SPIFlashRead(uint8* SpiFlash,u16 count,uint8 devC);
SPIFlashID SPIFlashReadID(uint8 devC);
void SPIFlashErasureAll();

//-------------------------------------------------------------------------------//
static inline void activeSlFlash(u8 mak)
{
	//SPIFlashCSDDR = 0xff;
	SPIFlashCSPORT	= (u8)(~mak);
}
static inline void disableAllSlFlash()
{
	//SPIFlashCSDDR = 0;
	SPIFlashCSPORT	= 0xff;
}
static inline void activeMsFlash()
{
	//SPIFlashMSDDR|=(1<<SPIFlashMSPIN);
	SPIFlashMSPORT&=~(1<<SPIFlashMSPIN);
}
static void activeFlash(uint8 devC)
{
	if(devC)
	{
		//SPIFlashCSDDR |= (1<<(devC-1));
		SPIFlashCSPORT	&= ~(1<<(devC-1));
	}
}

void SPIFlashInit()
{
	//DDRB|=(1<<4);//SS OUTPUT
	SPSR&=~(1<<SPIF);
	SPI_Init(GET_SPI_SET(SPI_FOSC_4,SPI_Mode_0,SPI_MSB,SPI_MSTR,SPI_IE));
	//SPSR|=(1<<SPI2X);
	//cs
	SPIFlashCSPORT	= 0xff;
	SPIFlashCSDDR	= 0xff;//xff
	//SPIFlashMSPORT&=~(1<<SPIFlashMSPIN);
	
	spiIdle=true;
}
///单位CLICK_CYCLE_US
//void inline WaitSPIIdle()
//{
//	while(!spiIdle);
//}
/*
bool WaitSPIIdle(uint8 timeout)
{
	uint8 intime = SysClick;
	while(!spiIdle)
	{
		if((uint8)(SysClick-intime)>=timeout)
		{
			return false;
		}
	}
	return true;
}
*/

static void startSend(u16 count)
{
	spiIdle = false;
	dataCount = count;
	SPDR = *dataP;
}
static void startRead(u16 count)
{
	spiIdle = false;
	saveRxData = true;
	dataCount = count;
}
static void WriteCom(uint8 com)
{
	WaitSlFlashIdelMak(SPIFlashWorkMak);
	activeSlFlash(SPIFlashWorkMak);
	dataP=SpiFlash;
	SpiFlash[0] = com;
	startSend(1);
}
static void inline WriteEnable()
{
	WriteCom(SPIFlashCom.WriteEnable);
}
static void inline WriteDisable()
{
	WriteCom(SPIFlashCom.WriteDisable);
}
void SPIFlashWriteStatusRegister(uint8 srdata)
{
	WriteEnable();
	WaitSPIIdle();
	activeSlFlash(SPIFlashWorkMak);
	//saveRxData = true;
	dataP=SpiFlash;
	SpiFlash[0] = SPIFlashCom.SPIFlashWriteStatusRegister;
	SpiFlash[1] = srdata;
	startSend(2);
}
uint8 SPIFlashReadStatusRegister(uint8 devC)
{
	WaitSPIIdle();
	activeFlash(devC);
	dataP=SpiFlash;
	startRead(2);
	SPDR = SPIFlashCom.SPIFlashReadStatusRegister;
	WaitSPIIdle();
	return SpiFlash[1];
}
#define SPIFLASH_WAIT_TIME_MS 30// max:256*CLICK_CYCLE_US/1000
static bool WaitFlashIdel(uint8 devC)
{
	//bit_is_clear
	u8 sttime = SysClick;
	while(SPIFlashReadStatusRegister(devC)&1)
	{
		if((uint8)(SysClick-sttime)>SPIFLASH_WAIT_TIME_MS*1000/CLICK_CYCLE_US)
			return false;
	}
	return true;
}
//返回成功的mak位
static uint8 WaitSlFlashIdelMak(uint8 mak)
{
	uint8 i = 0;
	for(;i<8;i++)
	{
		if(mak&(1<<i))
		{
			 if(!WaitFlashIdel(i+1))
			 {
				 mak&=~(1<<i);//等待失败就把对应位置0
			 }
		}
	}
	return mak;
}
void SPIFlashErasureAll()
{
	SPIFlashWriteStatusRegister(0);
	WriteEnable();
	WriteCom(SPIFlashCom.EraseAll);
}
static void SetPageProgramAdd()
{
	WriteEnable();
	WaitSPIIdle();
	DontAutoDisableFlash = true;
	activeSlFlash(SPIFlashWorkMak);
	SpiFlash[0]=SPIFlashCom.PageProgram;
	SpiFlash[1]=(uint8)(SPIFlashAddress>>16);
	SpiFlash[2]=(uint8)(SPIFlashAddress>>8);
	SpiFlash[3]=(uint8)(SPIFlashAddress>>0);
	dataP = SpiFlash;
	startSend(4);
}
bool SPIFlashPageProgram(uint8* dat,u16 count)
{
	SPIFlashWriteStatusRegister(0);
	SetPageProgramAdd();
	WaitSPIIdle();
	dataP = dat;
	startSend(count);
	return true;
}
static void SetReadAdd(uint8 devC)
{
	WaitFlashIdel(devC);
	DontAutoDisableFlash = true;

	activeFlash(devC);
	
	SpiFlash[0]=SPIFlashCom.ReadData;
	SpiFlash[1]=(uint8)(SPIFlashAddress>>16);
	SpiFlash[2]=(uint8)(SPIFlashAddress>>8);
	SpiFlash[3]=(uint8)(SPIFlashAddress>>0);
	dataP = SpiFlash;
	startSend(4);
}
bool SPIFlashRead(uint8* dat,u16 count,uint8 devC)
{
	SetReadAdd(devC);
	WaitSPIIdle();
	dataP = dat;
	startRead(count);
	SPDR = 0;
	return true;
}
SPIFlashID SPIFlashReadID(uint8 devC)
{
	SPIFlashID id;
	WaitFlashIdel(devC);
	dataP = SpiFlash;
	startRead(3);
	SPDR = SPIFlashCom.ReadIdentification;
	WaitSPIIdle();
	id.ManufacturerID = SpiFlash[0];
	id.DeviceID1=SpiFlash[1];
	id.DeviceID2=SpiFlash[2];
	return id;
}
void SPIFlashSetCommand(SFM m)
{
	memcpy_P(&SPIFlashCom,SPIFlashCommandInfo+m,sizeof(SPIFlashCommand));
}
ISR(SPI_STC_vect)
{
	dataCount--;
	if(dataCount!=0)
	{
		if(saveRxData)
		{
			*dataP++ = SPDR;
			SPDR = 0;
			//*RxDataP = SPDR;
		}
		else
		{
		SPDR = *++dataP;
		}
		
	//	dataP++;
	}
	else
	{
		if(saveRxData)
		{
			*dataP = SPDR;
			//*RxDataP = SPDR;
		}
		
		saveRxData = false;
		if(!DontAutoDisableFlash)
		{
			disableAllSlFlash();
		}
		DontAutoDisableFlash = false;
		spiIdle = true;
	}
}
