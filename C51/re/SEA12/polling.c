#include <stdio.h>
#include <Global.h>
#include "UI\ir_command.h"
#include "UI\key_command.h"
#include "UI\c.h"
#include "ir.h"
#include "m62446.h"
#include "mc14094b.h"
#include "spi.h"
#include "Clock.h"
#include "polling.h"
uint8 UserEventExitCount;


void ExitUserEvent()
{
	//SetBitDisplay(DisplayGlintPoint,DisplayGlintExitState);
	//DisplayGlintPoint=DISP_NULL;
	ControlState=CS_VOLUME_MAIN;
	ShowState();
	EepromWriteEX();
}
void polling60ms()
{
	//PollingDisplay();
	PollingKey60ms();
}
void polling10ms()
{
	PollingDisplay();
}
void polling200ms()
{
	PollingIRKey();
	PollingAutoControl();
}
void polling500ms()
{
#if defined(DISPLAY_TEST)||defined(DISPLAY_GLINT_POINT)
	PollingDisplay500ms();
#endif//DISPLAY_TEST
}
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
static void iniPoll()
{
	UserEventExitCount =  MaxUserEventCountDown;
	ShowState();
}
void PollingMain()
{

	uint8 sc;
	uint8 oldc60ms		= SysClick;
	uint8 oldc10ms		= oldc60ms;
	uint8 oldc200ms		= oldc60ms;
	uint8 oldc500ms		= oldc60ms;
	uint8 oldc1000ms	= oldc60ms;
	iniPoll();
	while(true)
	{	 
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
		if(sc-oldc200ms>200/CLICK_CYCLE_MS)
		{
			polling200ms();
			oldc200ms+=200/CLICK_CYCLE_MS;
		}
		if(sc-oldc500ms>500/CLICK_CYCLE_MS)
		{
			polling500ms();
			oldc500ms+=500/CLICK_CYCLE_MS;
		}
		if(sc-oldc1000ms>1000/CLICK_CYCLE_MS)
		{
			polling1000ms();
			oldc1000ms+=1000/CLICK_CYCLE_MS;
		}
	}
}

void ShowState()
{
#ifdef DEBUG_ShowState
	static uint8 test;
#endif//DEBUG_ShowState
	uint8 sd;
	uint8 ssd = DISP_NULL;
	bool eState = false;
	IniDisplay();
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:sd=M62446MainVolume-10;break;
	case CS_VOLUME_L:	sd=M62446VolumeL;break;
	case CS_VOLUME_R:	sd=M62446VolumeR;break;
	case CS_VOLUME_CC:	sd=M62446VolumeCC;ssd=DISP_CEN;break;
	case CS_VOLUME_SW:	sd=M62446VolumeSW;ssd=DISP_SW;break;
	case CS_VOLUME_SL:	sd=M62446VolumeSL;ssd=DISP_SL;break;
	case CS_VOLUME_SR:	sd=M62446VolumeSR;ssd=DISP_SR;break;
	case CS_BASS:		sd=M62446Bass;break;
	case CS_TREBLE:		sd=M62446Treble;break;
	case CS_OK_VOLUME:	sd=PT2314Volume/2;break;
	case CS_OK_HUNXIANG:sd=(PT2314SpeakerATT+15)/2;break;
	case CS_OK_DEYIN:	sd=PT2314Bass;break;
	case CS_OK_GAOYIN:	sd=PT2314Treble;break;
	case CS_TRACK_MODE://goto NO_CS_VOLUME
		{
			switch (M62446WorkMode)
			{
			case M62446_WM_6CH:ShowString("6C");break;
			case M62446_WM_3CH:ShowString("3C");break;
			case M62446_WM_2CH:ShowString("2C");break;
			}
		}
	default:goto NO_CS_VOLUME;
	}
	ShowINT8(sd);
NO_CS_VOLUME:
	
	switch(M62446OutputPort&M62446OUTPUTPORT_MASK)
	{
	case INTPUT_5_1:sd = DISP_AC_3;break;
	case INTPUT_CD:sd = DISP_CD;break;
	case INTPUT_AUX:sd = DISP_AUX;break;
	}
	SetBitDisplay(sd,true);
	if(ControlState==CS_INTPUT_SELECT)
	{
		DisplayGlintExitState = false;//will bug
		ssd = sd;
		ShowString("IN");
		eState = true;
	}
	//DisplayGlintPoint
	SetBitDisplay(DisplayGlintPoint,DisplayGlintState);
	if(ssd!=DisplayGlintPoint)
	{
		SetDisplayGlintPoint(ssd,eState);
	}
	//other
	SetBitDisplay(DISP_OK_MOD,ok_mode);
#ifdef DEBUG_ShowState
	ShowUINT8(test++);
#endif//DEBUG_ShowState
}
