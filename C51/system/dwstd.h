#ifndef _DWSTD_H_
#define _DWSTD_H_
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
#include "configure.h"

//#define DWSTD_ADDEND//����Ϊ���ɵ��ַ�����ӽ�����
#ifndef DWSTD_TOSTRING_ADDCHAR//����ʹ�ù̶����tostringʱ����λ������ַ�Ĭ����'0'
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
	STRINGFORMAT_D,//����ʮ����
	STRINGFORMAT_U,//�޷���ʮ����
	STRINGFORMAT_X,//ʮ������
};
/*********************************
ת����ֵ���ַ���
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
format��_STRINGFORMATָ����ʽ
return������ַ�������
*********************************/
uint8 ToString(char *outstr,dwstdint d,uint8 format);


/*********************************
ת����ֵ���ַ�����ʹ��16���Ƹ�ʽ��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
return������ַ�������
*********************************/
uint8 ToStringWithX(char *outstr,dwstdint d);

/*********************************
ת����ֵ���ַ�����ʹ��16���Ƹ�ʽ��
���ҿ�ȹ̶�����8λ�����ַ���16λ�ĸ��ַ�����λ���㲹��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
return������ַ�������
*********************************/
uint8 ToStringWithXFW(char *outstr,dwstdint d);

/*********************************
ת����ֵ���ַ�����ʹ���޷���10���Ƹ�ʽ��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
return������ַ�������
*********************************/
uint8 ToStringWithU(char *outstr,dwstdint d);


/*********************************
ת����ֵ���ַ�����ʹ�÷���10���Ƹ�ʽ��
ע�⣺	����ַ���Ĭ��û�н�����
outstr������ַ�
d��		������ֵ
return������ַ�������
*********************************/
uint8 ToStringWithD(char *outstr,dwstdsint d);


/*********************************
��ת�ַ���
st��	�ַ���
len��	�ַ�������
*********************************/
void ReverseString(char *st,uint8 len);


//���Ա�outstr�ĳ���
#endif//_DWSTD_H_

