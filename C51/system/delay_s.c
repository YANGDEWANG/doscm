/*
* delay������ʱ����
* ���������ͣ�C51
* �ļ�������ʹ���˴�����goto��Ϊ�˱���ʹ��ѭ����_delay_100us�����ɴ���ĸ���
* ��Ҫ����Щ�������ھ�ȷ��ʱ������ִ��ʱ�����ṩ�Ĳ���ָ����ʱ���Գ�
* ��ָ������Ϊ1usʱ����Լ   1.3%������ָ�����ڵļӴ����Ҳ���Ӵ�
* ��ָ������Ϊ0.5usʱ����Լ 0.6%
* ��ָ������Ϊ12usʱ����Լ 11.6%


* �汾��1
* ���ڣ�2006-12-16
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
#include "delay.h"

/*
�ӳ�s ��
s��
������,��sΪ0ʱΪ�����ʱ��256s��
*/
void _delay_s(uint8 s)
{
	uint8 ms250;
st:
	ms250 = 4;
re:
	_delay_ms(250);
	ms250--;
	if(ms250!=0)goto re;
	s--;
	if(s!=0)goto st;
}