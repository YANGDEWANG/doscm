#include "Global.h"

void ControlClock(bool S_P)//初始化Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR0 = 0b111&CLOCKCS;//设置分频
	}
	else
	{
		TCCR0 = 0;//设置分频
	}

}
