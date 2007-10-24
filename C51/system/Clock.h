#include "configure.h"

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 
#ifndef CLOCK_CFG
//--------------CLOCK_CFG-----------------//
#define CLOCK_CFG

#define CLICK_CYCLE_MS 10//系统Click时间(ms)
#define CLOCK_CYCLE_ADD 5//定义用于微调时钟，更改其值可以校准时间

#define CLOCK_SET_TMOD() (TMOD	= (0xf0&TMOD)|1)//模式1
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
void ControlClock(bool);	//初始化Clock: true = start;false = stop

#endif//_Clock_H_
