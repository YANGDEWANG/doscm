/*
* TM1618，TM1618 是一种带键盘扫描接口的LED（发光二极管显示器）驱动控制专用电路
* 内部集成有MCU 数字接口、数据锁存器、LED高电流驱动、键盘扫描等电路
* 处理器类型：C51,avr
* 版本：1
* 日期：2006-12-3
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
#include <system/delay.h>
#include <string.h>
//#include "spi2.h"
#include "dev/TM1618.h"



//#define setSTB() sbi(PORT_TM1618,TM1618_STB)
static void setSTB()
{
	sbi(PORT_TM1618,TM1618_STB);
	_delay_us(1);
}
#define clSTB()  cbi(PORT_TM1618,TM1618_STB)
#ifdef TM1618_SOFT_SPI
#define clSCL() cbi(PORT_TM1618,TM1618_CLK)
#define setSCL() sbi(PORT_TM1618,TM1618_CLK)

#define clDO() cbi(PORT_TM1618,TM1618_DAT)
#define setDO() sbi(PORT_TM1618,TM1618_DAT)

#define getDI() (PIN_TM1618&(1<<TM1618_DAT))

static u8 TM1618Transmit(u8 cData)//TM1618_MasterTransmitLSB
{
	uint8 i,ReadData=0;
	for(i=0;i<8;i++)
	{
		clSCL();
		if(cData&1)
		{
			setDO();
		}
		else
		{
			clDO();
		}
		cData = cData>>1;
		setSCL();
		ReadData = ReadData>>1;
		if(getDI())
			ReadData|=0x80;
	}
	setDO();
	return ReadData;
}
#else//TM1618_SOFT_SPI
#endif//TM1618_SOFT_SPI
//command

#define DATA_SETTING	(1<<6)
#define WRITE_DISPLAY	DATA_SETTING
#define READ_KEY		(DATA_SETTING|(1<<1))
#define ADD_SET			(3<<6)
#define DISPLAY_CONTROL (2<<6)
#define DISPLAY_SWITCH  (1<<3)

#if TM1618_LUM>7
#error TM1618_LUM 不能大于7
#endif


uint8 TM1618MemImage[TM1618MEMIMAGE_SIZE];
uint8 TM1618KeyImage[3];

void IniTM1618()		
{
#ifdef TM1618_SOFT_SPI
	sbi(DDR_TM1618,TM1618_DAT);   
	sbi(DDR_TM1618,TM1618_CLK);      
#endif//MMC_SOFT_SPI
	sbi(DDR_TM1618,TM1618_STB);      
	setSTB();
	memset(TM1618MemImage,0,sizeof(TM1618MemImage));
	TM1618UpdateAll();
}
void TM1618UpdateAll()
{
	uint8 i;
	clSTB();
	TM1618Transmit(TM1618DISPLAY_MODE);
	setSTB();//PWSTB:1us
	clSTB();
	TM1618Transmit(WRITE_DISPLAY);
	setSTB();//PWSTB:1us
	clSTB();
	TM1618Transmit(ADD_SET);
	for(i=0;i<sizeof(TM1618MemImage);i++)
	{
		TM1618Transmit(TM1618MemImage[i]);
	}
	setSTB();//PWSTB:1us
	clSTB();
	TM1618Transmit(DISPLAY_CONTROL|DISPLAY_SWITCH|TM1618_LUM);
	setSTB();
}
void TM1618ReadKey()		
{
	u8 i;
	clSTB();
	TM1618Transmit(READ_KEY);//TW:1us
#ifdef TM1618_SOFT_SPI
	cbi(DDR_TM1618,TM1618_DAT);   
#endif//MMC_SOFT_SPI
	for(i=0;i<sizeof(TM1618KeyImage);i++)
	{
		TM1618KeyImage[i] = TM1618Transmit(0xff);
	}
	setSTB();
#ifdef TM1618_SOFT_SPI
	sbi(DDR_TM1618,TM1618_DAT);   
#endif//MMC_SOFT_SPI
}
