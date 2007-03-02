#ifndef _C51_KEY_H_
#define _C51_KEY_H_
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
#include <configure.h>
#ifndef C51_KEY_LEVEL
#define C51_KEY_LEVEL 0//
#endif//C51_KEY_LEVEL

//#define C51_KEY_PIN_1	(P0^0)
//#define C51_KEY_PIN_2	(P0^1)
//#define C51_KEY_PIN_3	(P0^2)
//#define C51_KEY_PIN_4	(P0^3)
//#define C51_KEY_PIN_5	(P1^0)
//#define C51_KEY_PIN_6	(P1^0)
//#define C51_KEY_PIN_7	(P1^0)
//#define C51_KEY_PIN_8	(P1^0)

#ifdef C51_KEY_PIN_1
sbit C51_KEY_1 = C51_KEY_PIN_1;
#endif
#ifdef C51_KEY_PIN_2
sbit C51_KEY_2 = C51_KEY_PIN_2;
#endif
#ifdef C51_KEY_PIN_3
sbit C51_KEY_3 = C51_KEY_PIN_3;
#endif
#ifdef C51_KEY_PIN_4
sbit C51_KEY_4 = C51_KEY_PIN_4;
#endif
#ifdef C51_KEY_PIN_5
sbit C51_KEY_5 = C51_KEY_PIN_5;
#endif
#ifdef C51_KEY_PIN_6
sbit C51_KEY_6 = C51_KEY_PIN_6;
#endif
#ifdef C51_KEY_PIN_7
sbit C51_KEY_7 = C51_KEY_PIN_7;
#endif
#ifdef C51_KEY_PIN_8
sbit C51_KEY_8 = C51_KEY_PIN_8;
#endif

extern void IniC51Key();
extern uint8 C51KeyGetKey();

#endif//_C51_KEY_H_
