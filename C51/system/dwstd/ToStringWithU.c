/*
* dwstd 包含精简的std方法或其方法的替代
* 处理器类型：C51

* 版本：1 日期：2006-12-28

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
#include "dwstd.h"

/*********************************
转换数值到字符串（使用无符号10进制格式）
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
return：输出字符串长度
*********************************/
uint8 ToStringWithU(char *outstr,dwstdint d)
{
	char *st = outstr;
	uint8 i=0;
	do
	{
		*outstr=d%10+'0';
		d=d/10;
		outstr++;
		i++;
	}while(d);
#ifdef DWSTD_ADDEND
	*outstr = '\0';//添加结束符
#endif//DWSTD_ADDEND
	ReverseString(st,i);
	return i;
}