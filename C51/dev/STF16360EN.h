/*
* STF16360EN 一种负电源型VFD显示驱动的CMOS集成电路
* 处理器类型：C51
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
#ifndef _STF16360EN_H_
#define _STF16360EN_H_
#include <configure.h>

#ifndef STF16360EN_CFG
//---------------------------STF16360E-CFG-------------------------------//
#define STF16360EN_CFG
//pin
#define STF16360EN_CLKP P1_0
#define STF16360EN_DATP P1_1
#define STF16360EN_ENP  P1_2
//-------------------------STF16360E-CFG-ENN-----------------------------//
#endif//STF16360EN_CFG

void stf16360enInit();
void stf16360enWrite(uint8* dat,uint8 count);
#define stf16360enEnableOut()	(STF16360EN_ENP=false)
#define stf16360enDesableOut()	(STF16360EN_ENP=true)
#endif//_STF16360EN_H_
