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

#define setCLK() (stf16360enPORT|=(1<<STF16360EN_CLKP))
#define clCLK()  (stf16360enPORT&=~(1<<STF16360EN_CLKP))
#define setDAT() (stf16360enPORT|=(1<<STF16360EN_DATP))
#define clDAT()  (stf16360enPORT&=~(1<<STF16360EN_DATP))
void stf16360enInit()
{
	setDAT();
	setCLK();
	stf16360enDesableOut();
	stf16360enDDR|=(1<<STF16360EN_CLKP)|(1<<STF16360EN_DATP)|(1<<STF16360EN_ENP);
}
void stf16360enWrite(uint8* dat,uint8 count)
{
	uint8 d,i;
	stf16360enDesableOut();
	while(count--)
	{
		d = *dat++;
		i=8;
		do
		{
			if(d&1)
			{
				setDAT();
			}
			else
			{
				clDAT();
			}
			clCLK();
			d = d>>1;
			setCLK(); 
		}
		while(--i);
	}
	//这里可能有问题
	clCLK();
	setCLK(); 
	stf16360enEnableOut();
}
