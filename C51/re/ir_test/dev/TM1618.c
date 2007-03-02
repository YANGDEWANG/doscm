/*
* TM1618，此版本将clSTB，setSTB定义为函数指针以便于在同一个项目中使用多个tm1618使用
* TM1618 是一种带键盘扫描接口的LED（发光二极管显示器）驱动控制专用电路
* 内部集成有MCU 数字接口、数据锁存器、LED高电流驱动、键盘扫描等电路
* 处理器类型：C51,avr
* 版本：1.1
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
#include <string.h>
#include <spi2.h>
#include "TM1618.h"
#include "delay.h"
tm1628_stbf tm1628_setSTB;
tm1628_stbf tm1628_clSTB;
#define setSTB	tm1628_setSTB
#define clSTB	tm1628_clSTB


//#define setSTB() (TM1618_STB = true)
//#define clSTB()  (TM1618_STB = false)
//command

#define DATA_SETTING	(1<<6)
#define WRITE_DISPLAY	DATA_SETTING
#define READ_KEY		(DATA_SETTING|(1<<1))
#define ADD_SET			(3<<6)
#define DISPLAY_CONTROL (2<<6)
#define DISPLAY_SWITCH  (1<<3)
#define DISPLAY_LUM		(7)


/*
void IniTM1618()		
{
setSTB();
memset(TM1618MemImage,0,sizeof(TM1618MemImage));
TM1618UpdateAll();
}*/
static bool checkSTB()
{
	return (setSTB==null||clSTB==null);
}
void TM1618UpdateAll(uint8 *dat)
{
	uint8 dt,i;
	if(checkSTB())
		return;
#if defined(__C51__)&&_CPU_INSTRUCTION_CLOCK_>1000000
	uint8 del;
#endif
	clSTB();
	VSPI2_MasterTransmitLSB(TM1618DISPLAY_MODE);
	setSTB();//PWSTB:1us
#if _CPU_INSTRUCTION_CLOCK_>1000000
	_delay_loop(del,_CPU_PER_US_INSTRUCTION_/2+1,1);
#endif
	clSTB();
	VSPI2_MasterTransmitLSB(WRITE_DISPLAY);
	setSTB();//PWSTB:1us
#if _CPU_INSTRUCTION_CLOCK_>1000000
	_delay_loop(del,_CPU_PER_US_INSTRUCTION_/2+1,2);
#endif
	clSTB();
	VSPI2_MasterTransmitLSB(ADD_SET);
	i=4;
	do
	//for(i=0;i<4;i++,dat++)
	{
		dt = *dat++;
		VSPI2_MasterTransmitLSB(dt&0x1F);
		VSPI2_MasterTransmitLSB((dt&~0x1F)>>2);
	}while(--i);
	setSTB();//PWSTB:1us
#if _CPU_INSTRUCTION_CLOCK_>1000000
	_delay_loop(del,_CPU_PER_US_INSTRUCTION_/2+1,3);
#endif
	clSTB();
	VSPI2_MasterTransmitLSB(DISPLAY_CONTROL|DISPLAY_SWITCH|DISPLAY_LUM);
	setSTB();
}
uint8 TM1618ReadKey()		
{
	uint8 dat=0,dat2,i;
	if(checkSTB())
		return 0;
	clSTB();
	VSPI2_MasterTransmitLSB(READ_KEY);//TW:1us
	i=3;
	do
	{
		dat2 = VSPI2_MasterTransmitLSB(0xff);
		dat=dat<<2;
		dat|=dat2&(1<<1);
		if(dat2&1<<4)
			dat++;
	}while(--i);
	setSTB();
	return dat;
}
