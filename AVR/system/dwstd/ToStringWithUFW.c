/*
* dwstd ���������std�������䷽�������
* ���������ͣ�ƽ̨�޹�

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
ת����ֵ����ȹ̶����ַ�����ʹ��10���Ƹ�ʽ��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
w��		������
*********************************/
void ToStringWithUFW(char *outstr,dwstdint d,uint8 w)
{
	//char oc;
	char *st = outstr;
	uint8  i = w;
	do
	{
		*outstr++=d%10+'0';
		d=d/10;
		i--;
	}while(d&&i);
	while(i--)
	{
		*outstr++=DWSTD_TOSTRING_ADDCHAR;
	}
#ifdef DWSTD_ADDEND
	*outstr = '\0';//��ӽ�����
#endif//DWSTD_ADDEND
	ReverseString(st,w);
}
