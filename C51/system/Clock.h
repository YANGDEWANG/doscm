#include "configure.h"

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 
#ifndef CLICK_CYCLE_MS
#define CLICK_CYCLE_MS 10//ϵͳClickʱ��(ms)
#endif
#define CLOCK_CYCLE_ADD 5//��������΢��ʱ�ӣ�������ֵ����У׼ʱ��
/*
struct Time
{
	uint8 Click;
	uint8 Sec;
	uint8 Minute;
	uint8 Hour;
};
*/

extern uint8 SysClick;
//extern data struct Time CurrentTime;
void ControlClock(bool);	//��ʼ��Clock: true = start;false = stop

#endif//_Clock_H_
