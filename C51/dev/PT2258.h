#ifndef _PT2258_H_
#define	_PT2258_H_ 
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

#include "configure.h"
#include "..\system\eeprom_ex.h"

//定义得以保存PT2258设置到EEPROM
#define SAVE_PT2258_SETTING

#ifndef PT2258ADDRESS
#define PT2258ADDRESS 0x46//CODE1:1 CODE2:1
#endif

#define PT2258_MAXVOLUME				127
#define PT2258_MINVOLUME				0


#if !defined(SAVE_PT2258_SETTING)

extern uint8 PT2258VolumeAll [7];

#define PT2258Volume1	(uint8)PT2258Volume[0]
#define PT2258Volume2	(uint8)PT2258Volume[1]
#define PT2258Volume3	(uint8)PT2258Volume[2]
#define PT2258Volume4	(uint8)PT2258Volume[3]
#define PT2258Volume5	(uint8)PT2258Volume[4]
#define PT2258Volume6	(uint8)PT2258Volume[5]
#define PT2258VolumeAll	(uint8)PT2258Volume[6]

#else//SAVE_PT2258_SETTING

#define PT2258Volume1	(uint8)EepromBuffer[ESL_PT2258Volume1]
#define PT2258Volume2	(uint8)EepromBuffer[ESL_PT2258Volume2]
#define PT2258Volume3	(uint8)EepromBuffer[ESL_PT2258Volume3]
#define PT2258Volume4	(uint8)EepromBuffer[ESL_PT2258Volume4]
#define PT2258Volume5	(uint8)EepromBuffer[ESL_PT2258Volume5]
#define PT2258Volume6	(uint8)EepromBuffer[ESL_PT2258Volume6]
#define PT2258VolumeAll	(uint8)EepromBuffer[ESL_PT2258VolumeAll]

#endif//SAVE_PT2258_SETTING

extern bool  PT2258Mute;

void IniPT2258();
void PT2258UpdateAll();

#endif//_PT2258_H_ 
