#include <stdio.h>
#include "Global.h"
#include "UI\Display.h"
#include "Clock.h"
#include "polling.h"
#include "delay.h"
#include "lcd.h"
void IniDev()
{
	/////////////////��ʼIO///////////////////////////
	

	/////////////////��ʼ���豸///////////////////////////
	EA = 1;
	//IniDisplay();
	//ShowMesStr(MS_HELLO);
	ControlClock(true);
	lcd_init();

	
}
void main()//                         ���Գ���
{
	_delay_s(1);//������ʱ
	IniDev();//��ʼ���豸
	/////////////////////��ѭ��//////////////////////
	while(1)
	{
		PollingMain();
	}
}
