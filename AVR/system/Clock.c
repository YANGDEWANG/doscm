#include "Global.h"

void ControlClock(bool S_P)//��ʼ��Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR0 = 0b111&CLOCKCS;//���÷�Ƶ
	}
	else
	{
		TCCR0 = 0;//���÷�Ƶ
	}

}
