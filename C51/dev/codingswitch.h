#ifndef _CODINGSWITCH_H_
#define _CODINGSWITCH_H_
/*
 * codingswitch,编码开关
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
//引脚定义
#ifndef CODINGSWITCH_A_PIN
#define CODINGSWITCH_A_PIN (P1^0)
#endif//CODINGSWITCH_A_PIN
#ifndef CODINGSWITCH_B_PIN
#define CODINGSWITCH_B_PIN (P1^1)
#endif//CODINGSWITCH_B_PIN
sbit CodingsWitch_A = CODINGSWITCH_A_PIN;
sbit CodingsWitch_B = CODINGSWITCH_B_PIN;



#endif//_CODINGSWITCH_KEY_
