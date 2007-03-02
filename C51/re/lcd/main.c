#include <stdio.h>
#include "Global.h"
#include "UI\Display.h"
#include "Clock.h"
#include "polling.h"
#include "delay.h"
#include "lcd.h"
void IniDev()
{
	/////////////////初始IO///////////////////////////
	

	/////////////////初始化设备///////////////////////////
	EA = 1;
	//IniDisplay();
	//ShowMesStr(MS_HELLO);
	ControlClock(true);
	lcd_init();

	
}
void main()//                         测试程序
{
	_delay_s(1);//开机延时
	IniDev();//初始化设备
	/////////////////////主循环//////////////////////
	while(1)
	{
		PollingMain();
	}
}
