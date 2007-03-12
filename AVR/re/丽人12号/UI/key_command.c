#include <global.h>
#include "key_command.h"
#include "c.h"
#include "dwstd.h"
#include "../dev/pointvfddisplay.h"
#include "ir_command.h"
static uint8 oldk;
static uint8 REPkeyC = KEY_REPEAT_TIME;
static void keycmd_menu()
{
	if(ControlState<CS_INTPUT_SELECT)
	{
		ControlState++;
	}
	else
	{
		ControlState = 0;
	}
}
//static void keycmd_OKmenu()
//{
//	ControlState++;
//	if(ControlState<CS_OK_VOLUME||ControlState>CS_OK_DEYIN)
//	{
//		ControlState=CS_OK_VOLUME;
//	}
//}
//void PollingOK()
//{
//	//SetBitDisplay(DISP_OK_MOD,ok_mode);
//}
void PollingKey60ms()
{
	bool kf = false;
	uint8 k = KeyCode;
	//PollingOK();
	if(k==0)
	{
		REPkeyC = KEY_REPEAT_TIME;
		oldk=0;
		return;
	}
	if(k==oldk)
	{
		REPkeyC--;
	}
	else
	{
		REPkeyC=KEY_REPEAT_TIME;
		kf = true;
		oldk = k;
	}
	if(REPkeyC==0)
	{
		kf = true;
		REPkeyC=KEY_REPEAT_TIME;
	}

	if(kf)//下面处理按键
	{
		
#ifdef DISPLAY_TEST
		DisplayTestNext(true);
#endif//DISPLAY_TEST
#ifdef KEY_TEST
		ToStringWithU(stringbuff,k);
		DISClean();
		ShowString(stringbuff,1,3);
#endif//KEY_TEST
		noCallShowState= false;
		noInUserEvent=false;
		switch(k)
		{
		case KEYC_MENU:
			{
				keycmd_menu();
				break;
			}
		case KEYC_CH_DOME:
			{
				ControlState = CS_TRACK_MODE;
				CVolume(true);
				break;
			}
		case KEYC_SUB:
			{
				CVolume(false);
				break;
			}
		case KEYC_ADD:
			{
				CVolume(true);
				break;
			}
		case KEYC_INPUT:
			{
				IntoSearchSound();
				return;
			}
		case KEYC_M_VOL_M_TONE:
			{
				ControlState++;
				if(ControlState<CS_OK_VOLUME||ControlState>CS_OK_DEYIN)
				{
					ControlState=CS_OK_VOLUME;
				}
				break;
			}
		case KEYC_DEL_ECHO:
			{
				ControlState++;
				if(ControlState<CS_OK_HUNXIANG||ControlState>CS_OK_DELAY)
				{
					ControlState=CS_OK_HUNXIANG;
				}
				break;
			}
		case KEYC_DISPLAY:
			{
				IRC_pingpu();
				break;
			}
		}
		if(!noInUserEvent)
			InUserEvent();
		if(!noCallShowState)
			ShowState();
	}
}



