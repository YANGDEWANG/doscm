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
	/////////////////初始IO///////////////////////////
	

	/////////////////初始化设备///////////////////////////

	
	EA = 1;
	VSPI2_MasterInit();
	IniDisplay();
	//ShowMesStr(MS_HELLO);
	IniIrdev();
	ControlClock(true);
	//lcd_init();

	
}

void main()//                         测试程序
{ 
	P2_0 = false;
	_delay_ms(200);//开机延时
	IniDev();//初始化设备
	/////////////////////主循环//////////////////////
	while(1)
	{
		PollingMain();
	}
}
