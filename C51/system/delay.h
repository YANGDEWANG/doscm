#ifndef _DELAY_H_
#define _DELAY_H_
#include "configure.h"

#ifdef __C51__
/*
��ʱѭ��

__delay_C��
��ʱѭ������������ʹ���޷�������
*�����ṩ���������Ͳ�ͬÿ��ѭ����ָ�����ڲ�ͬ
*���Ϊuint8 data ÿ��ѭ��ʹ��2��ָ������

__count��
��ʱѭ������
*��__countΪ0ʱΪ����ӳ٣�__delay_C�������ֵ*ָ������ʱ�䣩����Ȼ��Ҫ���ϳ�ʼ��__delay_C��ʱ��

cc��
Ϊ�˿�����ͬһ�������ж��ʹ�ô˶��������ÿ��ʹ��ʱ�ṩ��ͬ��ccֵ�������ظ�
ע�⣺��ѭ���ڲ�ÿ��ѭ����ָ�����ڲ���ȷ��
ע�⣺ʹ��ʱ���__delay_CΪuint8��ʹ������Ĵ��������ṩ��__count�Ŀ�����
#if (__count<1)||(us>255)
#error ʹ�õ�__count�����������ֵ��
#endif
*/
#define _delay_loop(__delay_C,__count,cc)	\
{											\
	__delay_C=__count;						\
	_delay_loopF##cc:						\
	__delay_C--;							\
	if(__delay_C!=0)goto  _delay_loopF##cc; \
}		


#define _DELAY_LOOP_IN_UINT8_IC_ 2//_delay_loop,__delay_CΪuint8 dataʱ��ÿѭ��ָ������

/*
�ӳ�ms ����
ms��
������,��msΪ0ʱΪ�����ʱ��256ms��
*/
extern void _delay_ms(uint8 ms);
/*
�ӳ�s ��
s��
������,��sΪ0ʱΪ�����ʱ��256s��
*/
extern void _delay_s(uint8 s);
/*
�ӳ�us ΢��
us��
΢����
*/
extern void _delay_us(uint8 us);
#endif//__C51__
#endif//_DELAY_H_

