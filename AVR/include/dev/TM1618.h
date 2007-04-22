#ifndef _TM1618_H_
#define _TM1618_H_
/*
* TM1618，TM1618 是一种带键盘扫描接口的LED（发光二极管显示器）驱动控制专用电路
* 内部集成有MCU 数字接口、数据锁存器、LED高电流驱动、键盘扫描等电路
* 处理器类型：avr
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

#ifndef TM1618_CFG
//-------------------TM1618_CFG-----------------------//
#define TM1618_CFG
#define TM1618_SOFT_SPI 1//

#define TM1618DISPLAY_MODE	(0)//定义显示模式0:4位；1:5位；2:6位；3:7位；
//定义数据接口引脚
#define TM1618_POPT	D
#define	TM1618_STB	0
#ifdef TM1618_SOFT_SPI
#define TM1618_DAT	1	
#define TM1618_CLK	2	
#endif//TM1618_SOFT_SPI

//-------------------TM1618_CFG-----------------------//

#endif//TM1618_CFG

#define DDR_TM1618	GDDR(TM1618_POPT)
#define PORT_TM1618	GPORT(TM1618_POPT)
#define PIN_TM1618 	GPIN(TM1618_POPT)
#define TM1618MEMIMAGE_SIZE ((TM1618DISPLAY_MODE+4)*2)

extern uint8 TM1618MemImage[TM1618MEMIMAGE_SIZE];
extern uint8 TM1618KeyImage[3];

extern void TM1618ReadKey();
void IniTM1618();
extern void  TM1618UpdateAll();


#endif//#define _TM1618_H_
