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
��ʱѭ��

__delay_C��
��ʱѭ������������ʹ���޷�������
*�����ṩ���������Ͳ�ͬÿ��ѭ����ָ�����ڲ�ͬ
*���Ϊuint8 data ÿ��ѭ��ʹ��2��ָ������
*��__delay_CΪ0ʱΪ����ӳ٣�__delay_C�������ֵ*ָ������ʱ�䣩
cc��
Ϊ�˿�����ͬһ�������ж��ʹ�ô˶��������ÿ��ʹ��ʱ�ṩ��ͬ��ccֵ�������ظ�
ע�⣺��ѭ���ڲ�ÿ��ѭ����ָ�����ڲ���ȷ��
*/
#define __delay_loop_(__delay_C,cc)	\
{											\
	_delay_loopF##cc:						\
	__delay_C--;							\
	if(__delay_C!=0)goto  _delay_loopF##cc;\  
}	

/*
�ӳ�us ΢��(���԰�)
us��
΢����
*/
void _delay_us(uint8 us)
{
#if(_CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>0)
	uint8 data de = _CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1;
re:
	__delay_loop_(de,cc);//1us
	us--;
	if(us!=0)goto re;
#else
re:
	us--;
	if(us!=0)goto re;
#endif
#if (_CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error ����CPUƵ�ʵ�ԭ��_delay_us�ļ�ʱ�������������_delay_ms��
#endif
							 
}

/*
�ӳ�ms ����
ms��
������,��msΪ0ʱΪ�����ʱ��256ms��
*/
void _delay_ms(uint8 ms)
{
#if _CPU_INSTRUCTION_CLOCK_>=5000000
	uint8 data us10,de;
st:
	us10 = 100;
re:
	_delay_loop(de,_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1,cc);//10us
	//��Ϊÿ��ѭ����if(us10!=0)goto re;��ʹ������������˵һ��������_delay_loopѭ��������-1
	us10--;
	if(us10!=0)goto re;
	ms--;
	if(ms!=0)goto st;
#if (_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1<1)||(_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error ����CPUƵ�ʵ�ԭ��_delay_ms�ļ�ʱ�������������_delay_ms��
#endif
#else
	uint8 data us100,de;
st:
	us100 = 10;
re:
	_delay_loop(de,_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1,cc);//_delay_100us(de,1);//100us
	//��Ϊÿ��ѭ����if(us100!=0)goto re;��ʹ������������˵һ��������_delay_loopѭ��������-1
	us100--;
	if(us100!=0)goto re;
	ms--;
	if(ms!=0)goto st;
#if (_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1<1)||(_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error ����CPUƵ�ʵ�ԭ��_delay_ms�ļ�ʱ�������������_delay_ms��
#endif
#endif
}
