#include <global.h>
#include "key_command.h"
#include "pt6312.h"
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
//#define SHOW_DETAILED
#define CUSTO_MNAME_LC (8+5)
#define showErrStr(s) ShowString(s,0,6)
static bool caShowState;
static uint8 oldk;
static uint8 REPkeyC = KEY_REPEAT_TIME;
#ifdef SHOW_DETAILED
static uint8 ShowDetailedC;
#endif//SHOW_DETAILED
//void setAllDisTo(char c)
//{
//
//	SetdislinebufTo(c);
//	ShowString(dislinebuf,0,8);
//	SetdislinebufTo(c);
//	ShowString(dislinebuf,8,8);
//}
//显示错误代码
//void ShowError()
//{
//	ShowString("ERR",8,5);
//	ShowUint8(15,ErrorCode);
//	ErrorCode = EC_OK;
//}


static void updown(bool ud)
{
}

void PollingKey60ms()
{	
	//uint8 oldErrorCode=ErrorCode;
	bool kf = false; 
	uint8 k = *ScanKey();
	caShowState = false;

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
		//InUserEvent();
		//StopGlint();

		//ErrorCode = EC_OK;
		//notifyKeyDown = true;
#ifdef DISPLAY_TEST
		DisplayTestNext(true);
#endif//DISPLAY_TEST
#ifdef KEY_TEST
		//ShowUINT8(k);
		//sprintf(dislinebuf,"%d",k);
		ToStringWithXFW(dislinebuf,k);
		ShowString(dislinebuf,0,2);
		UpdateDisplay();
#endif//KEY_TEST

		switch(k)
		{
//		case KEYC_STANDARD:
//			{
//				if(WorkState==WS_IR_RECORD)
//				{
//					endRecord();
//				}
//				IrCompGroup=irkeyC=0;
//				WorkState = WS_STANDARD;
//				break;
//			}
//		case KEYC_COMP:
//			{
//				if(WorkState==WS_IR_RECORD)
//				{
//					endRecord();
//					_delay_ms(5);
//				}
//				WorkState = WS_IR_COMP;
//				CheckRecordData();
//				break;
//			}
//		case KEYC_UP:
//			{
//				updown(true);
//				break;
//			}
//		case KEYC_DOWN:
//			{
//				updown(false);
//				break;
//			}
//		case KEYC_RECORD:
//			{
//				WorkState = WS_IR_RECORD_RECORD_PICK;
//				loadRecordHead();
//				break;
//			}
//		case KEYC_MENU:
//			{
//				switch(WorkState)
//				{
//				case WS_STANDARD:
//					{
//#ifdef SHOW_DETAILED
//						WorkState = WS_IR_SHOW_DETAILED;
//						ShowDetailedC = 0;
//#endif//SHOW_DETAILED
//						irkeyC=0;
//						break;
//					}
//					/*case WS_IR_COMP_RECORD_PICK:
//					{
//					WorkState = WS_IR_COMP;
//					break;
//					}*/
//					case WS_IR_COMP:
//					{
//						CheckRecordData();
//						break;
//					}
//				case WS_IR_RECORD:
//					{
//						if(oldErrorCode == EC_CustomCodeComp)
//						{
//							MultiCustomCode = true;
//							RecordHead.RecordHead.MultiCustomCode = true;
//							showErrStr("MC");
//							//ErrorCode = EC_OK;
//							//UpdateDisplay();
//							caShowState = true;
//						}
//						else//清除记录重新开始//结束纪录并保存
//						{
//
//							//RecordHead.RecordHead.Check = 0;
//							MultiCustomCode = false;
//							memset(RecordHead.dat,0,sizeof(RecordHead));
//							IrCompGroup=irkeyC=0;
//							WorkState = WS_IR_RECORD;
//							//endRecord();
//							//WorkState = WS_STANDARD;
//						}
//						break;
//					}
//				case WS_IR_RECORD_RECORD_PICK:
//					{
//						//to record
//						//RecordHead.RecordHead.Check = 0;
//						MultiCustomCode = false;
//						memset(RecordHead.dat,0,sizeof(RecordHead));
//						RecordHead.RecordHead.ICName = IC_NULL;
//						IrCompGroup=irkeyC=0;
//						WorkState = WS_IR_RECORD;
//						//WorkState = WS_IR_RecordNameInput;
//						//GlintLc = CUSTO_MNAME_LC;
//						//memset(RecordHead.RecordHead.CustomName,'0',3);
//						break;
//					}
//				}
//				break;
//			}
		}
		if(!caShowState)
			ShowState();
	}
}
void ShowState()
{

}

#ifdef DWDEBUGF
void _delay_ms(uint8);
void debugkeyc()
{
	static i = 0;
	i++;
	CurrentRecord=i%28;
	IrCompGroup =0;
	IrKeyA[0] = CurrentRecord;
	_delay_ms(5);
	//endRecord();
	CheckRecordData();

}
#endif//DWDEBUGF
