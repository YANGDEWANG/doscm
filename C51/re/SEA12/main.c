#include <stdio.h>
#include "Global.h"
#include "UI\Display.h"
#include "UI\c.h"
#include "ir.h"
#include "iic.h"
#include "m62446.h"
#include "mc14094b.h"
#include "spi.h"
#include "Clock.h"
#include "polling.h"
#include "c51_key.h"
#include "eeprom_ex.h"
#include "PT2314.h"
#include "delay.h"
void IniDev()
{
	/////////////////初始IO///////////////////////////
	

	/////////////////初始化设备///////////////////////////
	EA = 1;
	IniEeprom_ex();
	IniC51Key();
	VSPI_MasterInit();
	IniIR();
	
	IniPT2314();
	IniMC14094B();
	IniDisplay();
	//ShowMesStr(MS_HELLO);
	ControlClock(true);
	
	M62446ToSound();
	//IniM62446();
	//ONJJ();//开继电器
}
/*
typedef void (*ir_cmdf)();
void ir_cmdv()
{
long int e=566666;
long int c=566666;
while(e--)
{ while(c--);
}
}
ir_cmdf	code irrr[] =   {ir_cmdv,ir_cmdv}; */
void main()//                         测试程序
{
	_delay_s(2);//开机延时
	IniDev();//初始化设备
	/////////////////////主循环//////////////////////
	while(1)
	{
		PollingMain();
	}
}
