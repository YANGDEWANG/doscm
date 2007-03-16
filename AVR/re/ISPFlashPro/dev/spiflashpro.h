/*
* 串口 FLASH
* 处理器类型：AVR
* 版本：1 日期：2007-1-17
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
/*
#include "spiflashpro.h"
*/
#ifndef _SPIFLASHPRO_H_
#define _SPIFLASHPRO_H_
#include <configure.h>
#include "SPIFLASHPRO.h"
extern uint8 SPIFlashManufacturerIDIndex;
extern uint16 SPIFlashDevNameIndex;
extern SPIFlashInfo SPIFlashInfoData;

void LoadSPIFlashInfoData();
void SPIFlashCopy();
void SPIFlashVerify();
bool SPIFlashQueryDevType();


#endif//_SPIFLASHPRO_H_

