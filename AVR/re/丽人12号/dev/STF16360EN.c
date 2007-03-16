/*
* STF16360EN 一种负电源型VFD显示驱动的CMOS集成电路
* 处理器类型：AVR
* 版本：1 日期：2007-1-22

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
#include "STF16360EN.h"
#include "SPI.h"
#include <avr/interrupt.h>
volatile bool stf16360enIdle;
uint8*	dataPtr;
uint8	dataCount;
extern uint8 disdevmem[7];
void stf16360enInit()
{
	DDR_SPI  |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);
	SPCR = GET_SPI_SET(SPI_FOSC_4,SPI_Mode_0,SPI_LSB,SPI_MSTR,SPI_IE);
#ifdef SPIDoubleSpeed
	SPSR |= (1<<SPI2X);
#endif//SPIDoubleSpeed
	stf16360enDesableOut();
	stf16360enIdle=true;
}
//void stf16360enWrite(uint8* dat,uint8 count)
//{
//	WaitSTF16360ENIdle();
//	//todo: 下面的可不要
//	stf16360enInit();
//	stf16360enIdle = false;
//	//stf16360enDesableOut();
//	dataPtr = dat;
//	dataCount = count;
//	SPDR = *dataPtr++;
//
//}
void stf16360enWrite()
{
	WaitSTF16360ENIdle();
	//todo: 下面的可不要
	stf16360enInit();
	stf16360enIdle = false;
	//stf16360enDesableOut();
	dataPtr = disdevmem;
	dataCount = sizeof(disdevmem);
	SPDR = *dataPtr++;

}
ISR(SPI_STC_vect)
{
	dataCount--;
	if(dataCount!=0)
	{
		SPDR = *dataPtr++;
	}
	else
	{
		//这里可能有问题
	//clCLK();
	//setCLK(); 
		stf16360enIdle = true;
		stf16360enEnableOut();
	}
}
