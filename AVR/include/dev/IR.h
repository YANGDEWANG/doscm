#ifndef _IR_H_
#define _IR_H_
/*
 * IR，红外遥控接收
 * 处理器类型：AVR
 * 版本：1
 * 日期：2007-1-24
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
#include "configure.h"

//#define SHOWCUSTOMCODE//
//#define IR_DEBUG//
//#define IRKEY_TEST//
#ifndef IR_CFG
//--------------IR配置-----------------//
#define IR_CFG
#define IR_CUSTOMCODE     8			//DVD/VCD:00
#define IR_INTERRUPT 	INT0_vect
#define IR_PORT	D
#define IR_P	2
//------------IR配置结束---------------//
#endif//IR_CFG

static inline void IniIR(void)	 //初始化接收
{
	//DDRD  &=~(1<<DDD2);	//使能上拉电阻
	//PORTD |= (1<<PD2);	//使能上拉电阻
	//配置为下降沿中断
	MCUCR |= (1<<ISC01);
//	MCUCR &=~(1<<ISC00);
	//GIFR   = (1<<INTF0);
	GICR  |= (1<<INT0);	//开外部中断0
}
extern bool IRKeyDown;					//接收完数据时置位
extern bool IRKeyHold;					//检测到重复信号置位
extern u8 IrKey;//按键码
//接收数据储蓄区
//0:用户码
//1:用户反码
//2:键码
//3:键反码
extern uint8 perchdata IRData[4];		
#define GetKey_IR() IRData[2]

#endif//_IR_H_
