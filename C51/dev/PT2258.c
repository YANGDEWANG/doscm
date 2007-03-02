/*
* PT2258，6通道音量
* 处理器类型：C51,avr
* 版本：1
* 日期：2007-1-11
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
#include <math.h>
#include "PT2258.h"
#include "iic.h"

#if !defined(SAVE_PT2258_SETTING)
extern uint8 PT2258Volume [7];
#else//SAVE_PT2258_SETTING
#define PT2258Volume (&PT2258Volume1)
#endif////SAVE_PT2258_SETTING
typedef union _PT2258DAT
{
	_uint8 dat;
	struct
	{
		_uint8 b1	:4;
		_uint8 b10	:3;
	}data;
}PT2258DAT;

typedef union _PT2258COM
{
	_uint8 dat;
	struct
	{
		_uint8 vol	:4;
		_uint8 b1	:1; 
		_uint8 ch	:3; 
	}com;
}PT2258COM;

static void PT2258Write(uint8 b10,uint8 b1)
{
	I2CWriteBunch(PT2258ADDRESS,b10,&b1,1);
}
bool  PT2258Mute;
void PT2258UpdateAll()
{
	uint8 i = 0;
	uint8 dat;
	PT2258COM b10,b1;
	b10.dat=0;
	b1.dat=0;
	b1.com.b1  = 1;
	//声道1-6
	for(;i<6;i++)
	{
		dat = PT2258_MAXVOLUME - PT2258Volume[i];
		b10.com.ch = i;
		b10.com.vol = ((PT2258DAT)dat).b10;
		b1.com.ch  = i;
		b1.com.vol = ((PT2258DAT)dat).b1;
		PT2258Write(b10.dat,b1.dat);
	}
	//主音量
	dat = PT2258_MAXVOLUME - PT2258VolumeAll;
	b10.dat = 0xd0;
	b10.com.vol = ((PT2258DAT)dat).b10;
	b1.dat = 0xd0;
	b1.com.vol = ((PT2258DAT)dat).b1;
	PT2258Write(b10.dat,b1.dat);
	//禁音
	if(PT2258Mute)
		I2CWriteBunch(PT2258ADDRESS,0xf9,0,0);
	else
		I2CWriteBunch(PT2258ADDRESS,0xf9-1,0,0);
}
void IniPT2258()
{
	I2CWriteBunch(PT2258ADDRESS,0xc0,0,0);
	PT2258UpdateAll();
}