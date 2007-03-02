#include <stdio.h>
#include <Global.h>
#include "Clock.h"
#include "polling.h"
#include "TM1618.h"
#include "irdev.h"
#include "ui\key_command.h"
//#define POLLING200MS
//#define POLLING1000MS
//uint8 UserEventExitCount;
uint8 WorkState;
#define bbP P2_0
uint8 bbtime;
uint8 ErrorCode;
uint8 bbEtime;
bool ebb;


void setSTB0();
void clSTB0();
void setSTB1();
void clSTB1();
void setSTB2();
void clSTB2();
void setSTB3();
void clSTB3();


//void ExitUserEvent()
//{
	//WorkState= WS_STANDARD;	
	//ShowState();//导致按相同遥控按键无显示更改
//}
void polling60ms()
{
	PollingKey60ms();
}
void polling10ms()
{
	PollIR10ms();
	if(bbtime==0&&ebb)
	{
		//bbtime = 20;
		bbEtime = 3;
		ebb = false;
	}
	//蜂鸣器
	if(bbtime)
	{
		bbP = true;
		bbtime--;
	}
	else
	{
		bbP = false;
	}
	if(bbEtime)
	{
		bbEtime--;
		if(bbEtime==0)
		{
			bbtime = 16;
		}
	}
	
}
#ifdef POLLING200MS
void polling200ms()
{

}
#endif//POLLING200MS
void polling500ms()
{
	PollingDisplay500ms();

}
#ifdef POLLING1000MS
void polling1000ms()
{
	if(UserEventExitCount<MaxUserEventCountDown)
	{
		UserEventExitCount++; 
		if(UserEventExitCount==MaxUserEventCountDown)
		{
			ExitUserEvent();
		}
	}
}
#endif//POLLING1000MS
static void iniPoll()
{
	//UserEventExitCount =  MaxUserEventCountDown;
	//ToStringWithXFW(dislinebuf,18);
	//ShowString(dislinebuf,2,4);
	ShowString("HELLO",9,7);
	//UpdateDisplay();
	StandardL	=0;

}
#ifdef DWDEBUGF
uint8 debugC;
void debugkeyc();
void debug()
{
debugkeyc();
}
#endif
void PollingMain()
{

	uint8 sc;
	uint8 oldc60ms		= SysClick;
	uint8 oldc10ms		= oldc60ms;
#ifdef POLLING200MS
	uint8 oldc200ms		= oldc60ms;
#endif//POLLING200MS
	uint8 oldc500ms		= oldc60ms;
#ifdef POLLING1000MS
	uint8 oldc1000ms	= oldc60ms;
#endif//POLLING1000MS
	iniPoll();
	while(true)
	{
#ifdef DWDEBUGF
		debug();	
#endif//DWDEBUGF
		//IniEeprom_ex();
		sc = SysClick;
		if(sc-oldc10ms>10/CLICK_CYCLE_MS)
		{
			polling10ms();
			oldc10ms+=10/CLICK_CYCLE_MS;
		}
		if(sc-oldc60ms>60/CLICK_CYCLE_MS)
		{
			polling60ms();
			oldc60ms+=60/CLICK_CYCLE_MS;
		}
#ifdef POLLING200MS
		if(sc-oldc200ms>200/CLICK_CYCLE_MS)
		{
			//IrDevDiv();
			polling200ms();
			oldc200ms+=200/CLICK_CYCLE_MS;
		}
#endif//POLLING200MS
		if(sc-oldc500ms>500/CLICK_CYCLE_MS)
		{
			polling500ms();
			oldc500ms+=500/CLICK_CYCLE_MS;
		}
#ifdef POLLING1000MS
		if(sc-oldc1000ms>1000/CLICK_CYCLE_MS)
		{	
			polling1000ms();
			oldc1000ms+=1000/CLICK_CYCLE_MS;
		}
#endif//POLLING1000MS
		IrDevDiv();
		//---------------------ir--------------------//
		PollingIRkey();
		if(DisplayChanged)
		{
			UpdateDisplay();
		}
	}
}


