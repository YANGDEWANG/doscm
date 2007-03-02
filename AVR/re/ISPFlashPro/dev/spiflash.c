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
SPIFlashCommand SPIFlashCom;
uint32 SPIFlashAddress;
static uint8* dataP;
static uint8 dataCount;
#define RxDataP dataP
static bool  DontAutoDisableFlash;
static bool  saveRxData;
static volatile bool spiIdle=true;
static uint8 dat[6];
#define SPIFlashCSDDR	DDRC
#define SPIFlashCSPORT	PORTC
#define SPIFlashMSDDR	DDRD
#define SPIFlashMSPORT	PORTD
#define SPIFlashMSPIN	4

//返回成功的mak位
static uint8 WaitSlFlashIdelMak(uint8 mak);
static void WaitFlashIdel(uint8 devC);
static void inline activeAllSlFlash();
static void inline disableAllSlFlash();
static void inline activeMsFlash();
static void activeFlash(uint8 devC);
static void inline WriteEnable();
static void inline WriteDisable();
static void startSend(uint8 count);
static void startRead(uint8 count);
static void WriteCom(uint8 com);
static void SetPageProgramAdd();
static void SetReadAdd(uint8 devC);

void SPIFlashInit();
void SPIFlashSetManufacturer(SFM m);
void inline WaitSPIIdle();
void SPIFlashWriteStatusRegister(uint8 srdata);
uint8 SPIFlashReadStatusRegister(uint8 devC);
bool SPIFlashPageProgram(uint8* dat,uint8 count);
bool SPIFlashRead(uint8* dat,uint8 count,uint8 devC);
SPIFlashID SPIFlashReadID(uint8 devC);
void SPIFlashErasureAll();

//-------------------------------------------------------------------------------//
static inline void activeAllSlFlash()
{
	//SPIFlashCSDDR = 0xff;
	SPIFlashCSPORT	= 0;
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
static inline void disableMsFlash()
{
	//SPIFlashMSDDR&=~(1<<SPIFlashMSPIN);
	SPIFlashMSPORT|=(1<<SPIFlashMSPIN);
}
static void activeFlash(uint8 devC)
{
	if(devC==0)
	{
		 activeMsFlash();
	}
	else
	{
		//SPIFlashCSDDR |= (1<<(devC-1));
		SPIFlashCSPORT	&= ~(1<<(devC-1));
	}
}

void SPIFlashInit()
{
	//DDRB|=(1<<4);//SS OUTPUT
	SPSR&=~(1<<SPIF);
	SPI_Init(GET_SPI_SET(SPI_FOSC_4,SPI_Mode_3,SPI_MSB,SPI_MSTR,SPI_IE));
	SPSR|=(1<<SPI2X);
	//cs
	SPIFlashCSPORT	= 0xff;
	SPIFlashCSDDR	= 0xff;//xff
	SPIFlashMSPORT|=(1<<SPIFlashMSPIN);
	SPIFlashMSDDR|=(1<<SPIFlashMSPIN);
	//SPIFlashMSPORT&=~(1<<SPIFlashMSPIN);
	
	spiIdle=true;
}
///单位CLICK_CYCLE_US
void inline WaitSPIIdle()
{
	while(!spiIdle);
}
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

static void startSend(uint8 count)
{
	spiIdle = false;
	dataCount = count;
	SPDR = *dataP++;
}
static void startRead(uint8 count)
{
	spiIdle = false;
	saveRxData = true;
	dataCount = count;
	SPDR = *dataP;
}
static void WriteCom(uint8 com)
{
	WaitSlFlashIdelMak(0xff);
	activeAllSlFlash();
	dataP=dat;
	dat[0] = com;
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
	activeAllSlFlash();
	//saveRxData = true;
	dataP=dat;
	dat[0] = SPIFlashCom.SPIFlashWriteStatusRegister;
	dat[1] = srdata;
	startSend(2);
}
uint8 SPIFlashReadStatusRegister(uint8 devC)
{
	WaitSPIIdle();
	activeFlash(devC);
	saveRxData = true;
	dataP=dat;
	dat[0] = SPIFlashCom.SPIFlashReadStatusRegister;
	startSend(2);
	WaitSPIIdle();
	return dat[1];
}
static void WaitFlashIdel(uint8 devC)
{
	//bit_is_clear
	while(SPIFlashReadStatusRegister(devC)&1)
	{
		//todo:chaoshi
	}
}
//返回成功的mak位
static uint8 WaitSlFlashIdelMak(uint8 mak)
{
	uint8 i = 0;
	for(;i<8;i++)
	{
		if(mak&(1<<i))
		{
			 WaitFlashIdel(i+1);
		}
	}
	return mak;
}
void SPIFlashErasureAll()
{
	WriteEnable();
	WriteCom(SPIFlashCom.EraseAll);
}
static void SetPageProgramAdd()
{
	WriteEnable();
	WaitSPIIdle();
	DontAutoDisableFlash = true;
	activeAllSlFlash();
	dat[0]=SPIFlashCom.PageProgram;
	dat[1]=(uint8)(SPIFlashAddress>>16);
	dat[2]=(uint8)(SPIFlashAddress>>8);
	dat[3]=(uint8)(SPIFlashAddress>>0);
	dataP = dat;
	startSend(4);
}
bool SPIFlashPageProgram(uint8* dat,uint8 count)
{
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
	
	dat[0]=SPIFlashCom.ReadData;
	dat[1]=(uint8)(SPIFlashAddress>>16);
	dat[2]=(uint8)(SPIFlashAddress>>8);
	dat[3]=(uint8)(SPIFlashAddress>>0);
	dataP = dat;
	startSend(4);
}
bool SPIFlashRead(uint8* dat,uint8 count,uint8 devC)
{
	SetReadAdd(devC);
	WaitSPIIdle();
	dataP = dat;
	startRead(count);
	return true;
}
SPIFlashID SPIFlashReadID(uint8 devC)
{
	SPIFlashID id;
	WaitFlashIdel(devC);
	dat[0]=SPIFlashCom.ReadIdentification;
	dataP = dat;
	startRead(3);
	id.ManufacturerID = dat[0];
	id.DeviceID1=dat[1];
	id.DeviceID2=dat[2];
	return id;
}
void SPIFlashSetManufacturer(SFM m)
{
	memcpy_P(&SPIFlashCom,SPIFlashCommandInfo+m,sizeof(SPIFlashCommand));
}
ISR(SPI_STC_vect)
{
	dataCount--;
	if(dataCount!=0)
	{
		SPDR = *dataP;
		if(saveRxData)
		{
			*RxDataP = SPDR;
		}
		dataP++;
	}
	else
	{
		spiIdle = true;
		saveRxData = false;
		if(!DontAutoDisableFlash)
		{
			disableAllSlFlash();
			disableMsFlash();
		}
		
		DontAutoDisableFlash = false;
	}
}
