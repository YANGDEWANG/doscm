#ifndef _DWSTD_H_
#define _DWSTD_H_
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
#include "configure.h"

//#define DWSTD_ADDEND//定义为生成的字符串添加结束符
#ifndef DWSTD_TOSTRING_ADDCHAR//定义使用固定宽度tostring时不足位的填充字符默认填'0'
#define DWSTD_TOSTRING_ADDCHAR '0'
#endif//DWSTD_TOSTRING_ADDCHAR

#ifndef dwstdint
#define dwstdint uint8
#endif
#ifndef dwstdsint
#define dwstdsint int8
#endif

enum _STRINGFORMAT
{
	STRINGFORMAT_D,//符号十进制
	STRINGFORMAT_U,//无符号十进制
	STRINGFORMAT_X,//十六进制
};
/*********************************
转换数值到字符串
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
format：_STRINGFORMAT指定格式
return：输出字符串长度
*********************************/
uint8 ToString(char *outstr,dwstdint d,uint8 format);


/*********************************
转换数值到字符串（使用16进制格式）
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
return：输出字符串长度
*********************************/
uint8 ToStringWithX(char *outstr,dwstdint d);

/*********************************
转换数值到字符串（使用16进制格式）
并且宽度固定例如8位两个字符，16位四个字符，高位不足补零
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
return：输出字符串长度
*********************************/
uint8 ToStringWithXFW(char *outstr,dwstdint d);

/*********************************
转换数值到字符串（使用无符号10进制格式）
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
return：输出字符串长度
*********************************/
uint8 ToStringWithU(char *outstr,dwstdint d);


/*********************************
转换数值到字符串（使用符号10进制格式）
注意：	输出字符串默认没有结束符
outstr：输出字符
d：		输入数值
return：输出字符串长度
*********************************/
uint8 ToStringWithD(char *outstr,dwstdsint d);


/*********************************
反转字符串
st：	字符串
len：	字符串长度
*********************************/
void ReverseString(char *st,uint8 len);


//，以便outstr的长度
#endif//_DWSTD_H_

