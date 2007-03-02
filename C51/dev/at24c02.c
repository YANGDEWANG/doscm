/*
* AT24C02 Two-wire Serial EEPROM
* 处理器类型：C51
* 版本：1 日期：2006-12-31

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
#include "iic.h"
#include "at24c02.h"


#define Add24c 0x50

#ifdef MULTI_AT24C02
uint8 At24c02Choice;
#else
#define At24c02Choice (0)
#endif//MULTI_AT24C02

/*****************************************
写数据到24c02
注意：	关于页写请参照24c02文档
datAdd:	数据地址
Dat:	数据指针
Count:	数据字节数
return:	成功写入字节数
******************************************/
uint8 Write24c02(uint8 datAdd,uint8* Dat,uint8 Count)
{
	return I2CWriteBunch(Add24c|At24c02Choice,datAdd,Dat,Count);
}
/*****************************************
读数据从24c02
注意：	联续的读取同写入一样有时间限制
		因为在读前执行了写命令以设置地址
datAdd:	数据地址
dat:	数据指针
count:	数据字节数
return:	成功读入字节数
******************************************/
uint8 Read24c02(uint8 datAdd,uint8* dat,uint8 count)
{
	return I2CReadBunch(Add24c|At24c02Choice,datAdd,dat,count);
}
/*****************************************
连续读数据从24c02
dat:	数据指针
count:	数据字节数
return:	成功读入字节数
******************************************/
uint8 Read24c02Stream(uint8* dat,uint8 count)
{
	return I2CReadStream(Add24c|At24c02Choice,dat,count);
}
#ifdef AT24C02_WRITEBYTE_U 
bool WriteByte24c02(uint8 Add,uint8 dat)		//写一字节到24c02//Add:地址；dat:数据；re=true 写入成功
{
	return Write24c02(Add,&dat,1);
}
#endif AT24C02_WRITEBYTE_U
#ifdef AT24C02_READBYTE_U
bool ReadByte24c02(uint8 Add,uint8 *dat)		//读一字节从24c02//Add:地址；dat:数据指针；re=true 读入成功
{
	if(Read24c02(Add,dat,1)==1)
		return true;
	return false;
}
#endif//AT24C02_READBYTE_U
