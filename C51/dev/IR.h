#ifndef _IR_H_
#define _IR_H_
/*
 * IR，红外遥控接收
 * 处理器类型：C51
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
#include "configure.h"

//#define SHOWCUSTOMCODE//
//#define IR_DEBUG//
//#define IRKEY_TEST//
#define InceptCustomCode_IR     8			//DVD/VCD:00
#define FirstKeyHoldEventCyc	8
#define SecondKeyHoldEventCyc	1


#define IR_EX	EX0
#define IR_IT	IT0
#define IR_INT	INT0

#define GetKey_IR() DATA_IR[2]
extern void IniIR();			//初始化IR
//extern uint8 GetKey_IR();		//获取按键码
extern bool KeyDown_IR;			//分别表示按键的按下状态
extern bool KeyHoldEvent;		//当按键按住“KeyHoldEventCyc”个周期（110ms）后被置一，须手动清零
extern bool FirstKeyHoldEvent;	//KeyHoldEvent是表示当按键按住过程第一次发生，须手动清零
extern bool KeyHold_IR;			////当FirstKeyHoldEvent置一时置一，一直到下一次检测的新的按键码输入时自动清零
extern uint8 perchdata DATA_IR[4];//接收数据储蓄区
#endif//_IR_H_
