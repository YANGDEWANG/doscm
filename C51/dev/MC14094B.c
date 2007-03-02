#include <global.h>
#include "SPI.h"
#include "MC14094B.h"


/*
* MC14094B，8-Stage Shift/Store  Register with Three-State Outputs
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

void IniMC14094B()
{
	STROBE_MC14094B = false;
}
void MC14094BWrite(uint8 *dat)
{
	uint8 i;
	STROBE_MC14094B = true;
	for(i=0;i<MC14094B_MAX_BYTE;i++)
	{
#ifdef MC14094B_MSB
		VSPI_MasterTransmitMSB(*dat++);
#else//MC14094B_MSB
		VSPI_MasterTransmitLSB(*dat++);
#endif//MC14094B_MSB
	}
	STROBE_MC14094B = false;
	//100ns//todo:Delay

}