#include "configure.h"

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 
#ifndef CLOCK_CFG
//--------------CLOCK_CFG-----------------//
#define CLOCK_CFG

#define CLICK_CYCLE_MS 10//ϵͳClickʱ��(ms)
#define CLOCK_CYCLE_ADD 5//��������΢��ʱ�ӣ�������ֵ����У׼ʱ��

#define CLOCK_SET_TMOD() (TMOD	= (0xf0&TMOD)|1)//ģʽ1
#define CLOCK_TL TL0
#define CLOCK_TH TH0
#define CLOCK_ET ET0
#define CLOCK_TR TR0
//--------------CLOCK_CFG-----------------//

#endif
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
