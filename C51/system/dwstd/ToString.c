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
转换数值到字符串
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
format：_STRINGFORMAT指定格式
return：输出字符串长度
*********************************/
uint8 ToString(char *outstr,dwstdint d,uint8 format)
{
	uint8 i=0,oc;
	char *st = outstr;
	//	char ex;
	switch(format)
	{
	case STRINGFORMAT_U:
		{
			do
			{
				*outstr=d%10+'0';
				d=d/10;
				outstr++;
				i++;
			}while(d);
			break;
		}
	case STRINGFORMAT_X:
		{
			do
			{
				oc = d%16;
				if(oc>9)
				{
					oc+='A'-'9'-1;
				}
				*outstr=oc+'0';
				d=d/16;
				outstr++;
				i++;
			}while(d);
			break;
		}
	}


#ifdef DWSTD_ADDEND
	*outstr = '\0';//添加结束符
#endif//DWSTD_ADDEND
	ReverseString(st,i);
	return i;
	/*i=i/2;
	while(i)
	{
	ex=*--outstr;
	*outstr =  *tmp;
	*tmp++=ex;
	i--;
	}*/
}
