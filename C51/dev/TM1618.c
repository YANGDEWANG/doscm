/*
* TM1618��TM1618 ��һ�ִ�����ɨ��ӿڵ�LED�������������ʾ������������ר�õ�·
* �ڲ�������MCU ���ֽӿڡ�������������LED�ߵ�������������ɨ��ȵ�·
* ���������ͣ�C51,avr
* �汾��1
* ���ڣ�2006-12-3
* ���ߣ������<yangdewang@gmail.com>

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
#include "spi2.h"
#include "TM1618.h"
#include "delay.h"

#define setSTB() (TM1618_STB = true)
#define clSTB()  (TM1618_STB = false)
//command

#define DATA_SETTING	(1<<6)
#define WRITE_DISPLAY	DATA_SETTING
#define READ_KEY		(DATA_SETTING|(1<<1))
#define ADD_SET			(3<<6)
#define DISPLAY_CONTROL (2<<6)
#define DISPLAY_SWITCH  (1<<3)
#define DISPLAY_LUM		(7)


uint8 TM1618MemImage[TM1618MEMIMAGE_SIZE];
/*
void IniTM1618()		
{
	setSTB();
	memset(TM1618MemImage,0,sizeof(TM1618MemImage));
	TM1618UpdateAll();
}*/
void TM1618UpdateAll()
{
#if defined(__C51__)&&_CPU_INSTRUCTION_CLOCK_>1000000
	uint8 del;
#endif
	uint8 i;
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
	for(i=0;i<sizeof(TM1618MemImage);i++)
	{
		VSPI2_MasterTransmitLSB(TM1618MemImage[i]);
	}
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
