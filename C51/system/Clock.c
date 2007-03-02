#include <global.h>
#include "Clock.h"

#ifndef CLOCK_SET_TMOD
#define CLOCK_SET_TMOD() (TMOD	= (T1_MASK_&TMOD)|T0_M0_)//模式1
#endif

#ifndef CLOCK_TL
#define CLOCK_TL TL0
#endif

#ifndef CLOCK_TH
#define CLOCK_TH TH0
#endif

#ifndef CLOCK_ET
#define CLOCK_ET ET0
#endif

#ifndef CLOCK_TR
#define CLOCK_TR TR0
#endif

uint8 SysClick;
void ControlClock(bool S_P)//初始化Clock: true = start;false = stop
{
	if(S_P)
	{
		CLOCK_SET_TMOD();

		CLOCK_TL = (0xffff-(CLICK_CYCLE_MS*_CPU_INSTRUCTION_CLOCK_/1000))+CLOCK_CYCLE_ADD;//+8平衡负载
		CLOCK_TH = (0xffff-(CLICK_CYCLE_MS*_CPU_INSTRUCTION_CLOCK_/1000)+CLOCK_CYCLE_ADD)>>8;

		CLOCK_TR = 1;
		CLOCK_ET = 1;
	}
	else
	{
		CLOCK_TR = 0;
	}

}
void Time0 (void) interrupt 1  using 1
{  	
	CLOCK_TL = (0xffff-(CLICK_CYCLE_MS*_CPU_INSTRUCTION_CLOCK_/1000))+CLOCK_CYCLE_ADD;//+8平衡负载
	CLOCK_TH = (0xffff-(CLICK_CYCLE_MS*_CPU_INSTRUCTION_CLOCK_/1000)+CLOCK_CYCLE_ADD)>>8;

	SysClick++;
}