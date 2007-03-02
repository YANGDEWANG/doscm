/*
* C51 key,用51IO口的按键接口当前版本直接判断IO口电平来检测按键
* 处理器类型：C51
* 版本：1
* 日期：2006-12-5
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
#include "c51_key.h"


void IniC51Key()
{
#ifdef C51_KEY_PIN_1
	C51_KEY_1 = true;
#endif
#ifdef C51_KEY_PIN_2
	C51_KEY_2 = true;
#endif
#ifdef C51_KEY_PIN_3
	C51_KEY_3 = true;
#endif
#ifdef C51_KEY_PIN_4
	C51_KEY_4 = true;
#endif
#ifdef C51_KEY_PIN_5
	C51_KEY_5 = true;
#endif
#ifdef C51_KEY_PIN_6
	C51_KEY_6 = true;
#endif
#ifdef C51_KEY_PIN_7
	C51_KEY_7 = true;
#endif
#ifdef C51_KEY_PIN_8
	C51_KEY_8 = true;
#endif
}
uint8 C51KeyGetKey()
{
	#ifdef C51_KEY_PIN_1
	if(C51_KEY_1 == C51_KEY_LEVEL)
		return 1;
#endif
#ifdef C51_KEY_PIN_2
	if(C51_KEY_2 == C51_KEY_LEVEL)
		return 2;
#endif
#ifdef C51_KEY_PIN_3
	if(C51_KEY_3 == C51_KEY_LEVEL)
		return 3;
#endif
#ifdef C51_KEY_PIN_4
	if(C51_KEY_4 == C51_KEY_LEVEL)
		return 4;
#endif
#ifdef C51_KEY_PIN_5
	if(C51_KEY_5 == C51_KEY_LEVEL)
		return 5;
#endif
#ifdef C51_KEY_PIN_6
	if(C51_KEY_6 == C51_KEY_LEVEL)
		return 6;
#endif
#ifdef C51_KEY_PIN_7
	if(C51_KEY_7 == C51_KEY_LEVEL)
		return 7;
#endif
#ifdef C51_KEY_PIN_8
	if(C51_KEY_8 == C51_KEY_LEVEL)
		return 8;
#endif
	return 0;
}


