#include <stdio.h>
#include "Global.h"
#include "UI\Display.h"
#include "Clock.h"
#include "polling.h"
#include "delay.h"
#include "irdev.h"
#include <spi2.h>
void IniDev()
{
	/////////////////��ʼIO///////////////////////////
	

	/////////////////��ʼ���豸///////////////////////////

	
	EA = 1;
	VSPI2_MasterInit();
	IniDisplay();
	//ShowMesStr(MS_HELLO);
	IniIrdev();
	ControlClock(true);
	//lcd_init();

	
}

void main()//                         ���Գ���
{ 
	P2_0 = false;
	_delay_ms(200);//������ʱ
	IniDev();//��ʼ���豸
	/////////////////////��ѭ��//////////////////////
	while(1)
	{
		PollingMain();
	}
}
