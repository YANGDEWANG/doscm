/*
* dwstd ���������std�������䷽�������
* ���������ͣ�C51

* �汾��1 ���ڣ�2006-12-28

* ���ߣ������<yangdewang@gmail.com>

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
ת����ֵ���ַ�����ʹ��16���Ƹ�ʽ��
���ҿ�ȹ̶�����8λ�����ַ���16λ�ĸ��ַ�����λ���㲹��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
return������ַ�������
*********************************/
uint8 ToStringWithXFW(char *outstr,dwstdint d)
{
	char oc;
	char *st = outstr;
	uint8 i=0;
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
	while(i<sizeof(dwstdint)*2)
	{
		*outstr=DWSTD_TOSTRING_ADDCHAR;
		outstr++;
		i++;
	}
#ifdef DWSTD_ADDEND
	*outstr = '\0';//��ӽ�����
#endif//DWSTD_ADDEND
	ReverseString(st,i);
	return i;
}

