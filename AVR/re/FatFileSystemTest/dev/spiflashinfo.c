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
#include <global.h>
#include <avr/pgmspace.h>
#include "spiflash.h"

prog_SPIFlashCommand SPIFlashCommandInfo[]=
{
	//eon
	{6,4,5,1,3,0x0b,2,0xd8,0xc7,0x9f},
};
prog_char SPIFlashManufacturerID[]=
{
	
	1,//eon
	0x1F,//Atmel
};
prog_SPIFlashInfo SPIFlashInfoTabel[]=
{
	//eon
	{{1,0,0},SFC_Eon,"EN25T80",256,4096},
	//{{0x1c,0x31,0x14},"11111111",256,4096u}
};
