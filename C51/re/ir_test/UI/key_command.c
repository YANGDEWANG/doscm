#include <global.h>
#include "key_command.h"
#include "TM1618.h"
#include <stdio.h>
#include <delay.h>
#include <string.h>
#include <arith.h>
#include "dev\irdev.h"
#include <at24c02.h>
//#define SHOW_DETAILED
#define CUSTO_MNAME_LC (8+5)
#define showErrStr(s) ShowString(s,0,6)

void setSTB0();
void clSTB0();
void setSTB1();
void clSTB1();

static bool caShowState;
static uint8 oldk;
static uint8 REPkeyC = KEY_REPEAT_TIME;
#ifdef SHOW_DETAILED
static uint8 ShowDetailedC;
#endif//SHOW_DETAILED
uint8 irkeyC;
uint8 IrKeyA[16];
#define MAX_IRCOMPGROUP 3
uint8 IrCompGroup;//每个Record被分为4个Group
#define MAX_RECORD_COUNT 27
uint8 CurrentRecord;//总共28个Record（0-27）
IRRecordHead RecordHead;
bool MultiCustomCode;//允许多个CustomCode
uint8 OldIRKey;//上一次接受到的遥控麻子
//bool notifyKeyDown;//为PollingIRkey准备的当有面板按键操作时被置1
#define getRecordAt24c02Choice() (CurrentRecord/4+1)//第一个256用于储蓄RecordHead

void setAllDisTo(char c)
{

	SetdislinebufTo(c);
	ShowString(dislinebuf,0,8);
	SetdislinebufTo(c);
	ShowString(dislinebuf,8,8);
}
//显示错误代码
void ShowError()
{
	ShowString("ERR",8,5);
	ShowUint8(15,ErrorCode);
	//ErrorCode = EC_OK;
}
/*
#if sizeof(IRRecordHead)>16
#error IRRecordHead too lager
#elif sizeof(IRRecordHead)>8
#define RECORDHEAD_EEPROM_SIZE 16
#else 
#define RECORDHEAD_EEPROM_SIZE 8
#endif
*/
#define RECORDHEAD_EEPROM_SIZE 8
static void loadRecordHead()
{
	At24c02Choice = 0;
	Read24c02(CurrentRecord*RECORDHEAD_EEPROM_SIZE,RecordHead.dat,sizeof(IRRecordHead));
	MultiCustomCode = RecordHead.RecordHead.MultiCustomCode;
	if(RecordHead.RecordHead.ICName>=IC_MAX)
		RecordHead.RecordHead.ICName=IC_NULL;
}
static void saveRecordHead()
{
	At24c02Choice = 0;
	Write24c02(CurrentRecord*RECORDHEAD_EEPROM_SIZE,RecordHead.dat,sizeof(IRRecordHead));
}

static uint8 getCheck()//效验值为所有数据和的按位取反
{
	uint8 i=0;
	uint8 d=0;
	for(;i<sizeof(IrKeyA);i++)
	{
		d+=IrKeyA[i];
	}
	return ~d;
}
static uint8 loadRecord()
{
	At24c02Choice = getRecordAt24c02Choice();
	Read24c02((CurrentRecord<<6) | (IrCompGroup<<4),IrKeyA,sizeof(IrKeyA));
	return getCheck();
}

static uint8 saveRecord()
{
	At24c02Choice = getRecordAt24c02Choice();
	Write24c02((CurrentRecord<<6) | (IrCompGroup<<4),IrKeyA,sizeof(IrKeyA));
	return getCheck();
}
static void endRecord()
{
	RecordHead.RecordHead.Check+=saveRecord();
	RecordHead.RecordHead.KeyCount = irkeyC+IrCompGroup*sizeof(IrKeyA);
	while(IrCompGroup!=MAX_IRCOMPGROUP)
	{
		memset(IrKeyA,0,sizeof(IrKeyA));
		_delay_ms(5);
		IrCompGroup++;
		RecordHead.RecordHead.Check+=saveRecord();
	}
	_delay_ms(5);
	saveRecordHead();
}
static uint8 getkey()
{
	uint8  out;
	tm1628_clSTB = clSTB0;
	tm1628_setSTB = setSTB0;
	out= TM1618ReadKey();
	if(out)
		return out;
	tm1628_clSTB = clSTB1;
	tm1628_setSTB = setSTB1;
	out= TM1618ReadKey();
	if(out)
		return out+0xff/2;
	else
		return 0;
}
static void CheckRecordData()
{
	uint8 check=0;
	uint8 c=3;
	IrCompGroup = 0;
	irkeyC=0;//开始比较需要的
	loadRecordHead();
	_delay_ms(5);
	At24c02Choice = getRecordAt24c02Choice();
	Read24c02((CurrentRecord<<6),IrKeyA,sizeof(IrKeyA));
	check+=getCheck();
	do
	{
		Read24c02Stream(IrKeyA,sizeof(IrKeyA));
		check+=getCheck();
	}while(--c);
	if(RecordHead.RecordHead.Check!=check)
	{
		ErrorCode = EC_RecordCheck;
	}
	else
	{
		_delay_ms(5);//todu
		loadRecord();
	}
}
static void updown(bool ud)
{

	switch(WorkState)
	{

	case WS_IR_COMP_RECORD_PICK:
	case WS_IR_RECORD_RECORD_PICK:
	case WS_IR_COMP:
		{
			CurrentRecord = EddyMovePointer(0,MAX_RECORD_COUNT,CurrentRecord,ud);

			if(WorkState==WS_IR_COMP)
			{
				CheckRecordData();
			}
			else
			{
				loadRecordHead();
			}
			break;
		}
#ifdef SHOW_DETAILED
	case WS_IR_SHOW_DETAILED:
		{
			if(IRChangec>2)
				ShowDetailedC = EddyMovePointer(0,IRChangec-2,ShowDetailedC,ud);
			break;
		}
#endif//SHOW_DETAILED
	case WS_IR_RECORD:
		{
			irkeyC = EddyMovePointer(0,sizeof(IrKeyA)-1,irkeyC,ud);

			ShowIrInformation();
			//ToStringWithU(dislinebuf,IrCompGroup*sizeof(IrKeyA)+irkeyC);
			//ShowString(dislinebuf,6,2);
			ShowUint8(6,0);
			ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
			//UpdateDisplay();
			caShowState = true;
			break;
		}
	}
}

void PollingKey60ms()
{	
	uint8 oldErrorCode=ErrorCode;
	bool kf = false; 
	uint8 k = getkey();
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
		StopGlint();

		ErrorCode = EC_OK;
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
		case KEYC_STANDARD:
			{
				if(WorkState==WS_IR_RECORD)
				{
					endRecord();
				}
				IrCompGroup=irkeyC=0;
				WorkState = WS_STANDARD;
				break;
			}
		case KEYC_COMP:
			{
				if(WorkState==WS_IR_RECORD)
				{
					endRecord();
					_delay_ms(5);
				}
				WorkState = WS_IR_COMP;
				CheckRecordData();
				break;
			}
		case KEYC_UP:
			{
				updown(true);
				break;
			}
		case KEYC_DOWN:
			{
				updown(false);
				break;
			}
		case KEYC_RECORD:
			{
				WorkState = WS_IR_RECORD_RECORD_PICK;
				loadRecordHead();
				break;
			}
		case KEYC_MENU:
			{
				switch(WorkState)
				{
				case WS_STANDARD:
					{
#ifdef SHOW_DETAILED
						WorkState = WS_IR_SHOW_DETAILED;
						ShowDetailedC = 0;
#endif//SHOW_DETAILED
						irkeyC=0;
						break;
					}
					/*case WS_IR_COMP_RECORD_PICK:
					{
					WorkState = WS_IR_COMP;
					break;
					}*/
					case WS_IR_COMP:
					{
						CheckRecordData();
						break;
					}
				case WS_IR_RECORD:
					{
						if(oldErrorCode == EC_CustomCodeComp)
						{
							MultiCustomCode = true;
							RecordHead.RecordHead.MultiCustomCode = true;
							showErrStr("MC");
							//ErrorCode = EC_OK;
							//UpdateDisplay();
							caShowState = true;
						}
						else//清除记录重新开始
						{

							//RecordHead.RecordHead.Check = 0;
							MultiCustomCode = false;
							memset(RecordHead.dat,0,sizeof(RecordHead));
							IrCompGroup=irkeyC=0;
							//WorkState = WS_IR_RECORD;
							//endRecord();//清除记录
							//WorkState = WS_STANDARD;
						}
						break;
					}
				case WS_IR_RECORD_RECORD_PICK:
					{
						//to record
						//RecordHead.RecordHead.Check = 0;
						MultiCustomCode = false;
						memset(RecordHead.dat,0,sizeof(RecordHead));
						RecordHead.RecordHead.ICName = IC_NULL;
						IrCompGroup=irkeyC=0;
						WorkState = WS_IR_RECORD;
						//WorkState = WS_IR_RecordNameInput;
						//GlintLc = CUSTO_MNAME_LC;
						//memset(RecordHead.RecordHead.CustomName,'0',3);
						break;
					}
				}
				break;
			}
		}
		if(!caShowState)
			ShowState();
	}
}
#ifdef SHOW_DETAILED
static void showEtailed()
{
	//uint8 i = ShowDetailedC;
	//Cldislinebuf();
	ToStringWithU(dislinebuf,ShowDetailedC);
	ToStringWithU(dislinebuf+4,ShowDetailedC+1);
	ShowString(dislinebuf,0,8);
	ToStringWithU(dislinebuf,IRChangeTime[ShowDetailedC]);
	ToStringWithU(dislinebuf+4,IRChangeTime[ShowDetailedC+1]);
	ShowString(dislinebuf,8,8);

}
#endif//SHOW_DETAILED
static void ShowRecordHead()
{
	ShowUint8(7,CurrentRecord);
	ShowString(ICNameS[RecordHead.RecordHead.ICName],0,6);
	ShowUint8(10,RecordHead.RecordHead.KeyCount);
	//ShowString(RecordHead.RecordHead.CustomName,CUSTO_MNAME_LC,3);
}

void ShowState()
{
	StandardL	=1;
	CompL		=1;
	RecordL		=1;
	setAllDisTo('_');//清显示
	switch(WorkState)
	{
	case WS_IR_RECORD_RECORD_PICK:
		{
			RecordL=0;
			ShowRecordHead();
			GlintLc = 7;
			SetGlint();
			break;
		}
	case WS_IR_RECORD:
		{
			RecordL=0;

			break;
		}
	case WS_IR_COMP:
		{
			CompL=0;
			ShowRecordHead();
			break;
		}
	case WS_IR_COMP_RECORD_PICK:
		{
			CompL=0;
			ShowRecordHead();
			GlintLc = 7;
			SetGlint();
			break;
		}
	case WS_IR_RecordNameInput:
		{
			RecordL=0;
			ShowRecordHead();
			SetGlint();
			break;
		}
#ifdef SHOW_DETAILED
	case WS_IR_SHOW_DETAILED:
		{
			StandardL	=0;
			showEtailed();
			break;
		}
#endif//SHOW_DETAILED
	case WS_STANDARD:
		{
			StandardL	=0;
			//setAllDisTo('-');
			break;
		}
	default:
		{
			//if((RecordL||CompL))
			StandardL	=0;
			break;
		}
	}
	if(ErrorCode)
	{
		bberr();
		ShowError();
		//WorkState = WS_STANDARD;
	}
	//UpdateDisplay();
}

static void icE(){
	bberr();
	showErrStr("ERR IC");
}
static void codeE(){
	bberr();
	showErrStr("ERR C");
}
static void keyE(){
	bberr();
	showErrStr("ERR K");
}
void PollingIRkey()
{
	bool samekey = false;
	if(IRKeyDown)
	{
		IRKeyDown = false;
		//InUserEvent();
		if(IRKeyHold)
		{
			IRKeyHold=false;

			bbstd();
			return;
		}
		if(ErrorCode)
		{
			bberr();
			ShowError();
			return;
		}



		switch(WorkState)
		{
#ifdef SHOW_DETAILED
		case WS_IR_SHOW_DETAILED:
			{
				WorkState = WS_STANDARD;//退出WS_IR_SHOW_DETAILED
				break;
			}
#endif
		case WS_IR_COMP_RECORD_PICK:
		case WS_IR_RECORD_RECORD_PICK:
			{
				bbwarn();
				return;
			}
		}

		if(OldIRKey==IrInformation.KeyCode)
		{
			samekey = true;
			if(bbtime==0)
			{
				bbstd();
			}
		}
		else
		{
			bbstd();
		}
		/*
		for(;i<irkeyC;i++)
		{
		if(IrKeyA[i]==IrInformation.KeyCode)
		{
		irkeyC = i;
		break;
		}
		}
		*/

		setAllDisTo(' ');//清显示
		switch(WorkState)
		{
		case WS_STANDARD:
			{
				if(!samekey)
				{
					irkeyC++;
				}
				if(irkeyC>99)
				{
					irkeyC=0;
				}
				ShowUint8(7,irkeyC+IrCompGroup*sizeof(IrKeyA));
				ShowIrInformation();
				break;
			}
		case WS_IR_COMP:
			{
				ShowIrInformation();
				if(IrCompGroup*sizeof(IrKeyA)+irkeyC==RecordHead.RecordHead.KeyCount
					&&!samekey)//end//相同键避免归0以便继续显pass
				{
					//next ir
					IrCompGroup=0;
					irkeyC=0;
					loadRecord();

				}
				if(irkeyC>sizeof(IrKeyA)-1)
				{
					//next grup
					irkeyC=0;
					if(IrCompGroup<MAX_IRCOMPGROUP)
					{
						IrCompGroup++;
					}
					else
					{
						IrCompGroup=0;
						//break;
					}
					loadRecord();
				}
				if(RecordHead.RecordHead.ICName != ICName)
				{
					icE();
				}else if(!MultiCustomCode
					&&(RecordHead.RecordHead.CustomCode != IrInformation.CustomCode
					||RecordHead.RecordHead.CustomCodeReverse != IrInformation.CustomCodeReverse))
				{
					codeE();
				}else if(IrKeyA[irkeyC] != IrInformation.KeyCode&&!samekey)
				{
					keyE();
				} 
				else 
				{
					if(!samekey)irkeyC++;
					if(IrCompGroup*sizeof(IrKeyA)+irkeyC==RecordHead.RecordHead.KeyCount)
					{
						ShowString(" -PASS-",8,8);
						bbtime = 30;
					}
					ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
					break;
				}
				ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
				return;//有错返回
			}
		case WS_IR_RECORD:
			{
				if(irkeyC==0&&IrCompGroup==0)//save CustomCode
				{
					RecordHead.RecordHead.CustomCode=IrInformation.CustomCode;
					RecordHead.RecordHead.CustomCodeReverse= IrInformation.CustomCodeReverse;
					RecordHead.RecordHead.ICName = ICName;
				}
				else//check CustomCode
				{
					if(!MultiCustomCode//允许多个CustomCode
						&&(RecordHead.RecordHead.CustomCode!=IrInformation.CustomCode
						||RecordHead.RecordHead.CustomCodeReverse != IrInformation.CustomCodeReverse))
					{
						ErrorCode = EC_CustomCodeComp;

						ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
						ShowIrInformation();
						codeE();
						return;
					}
					if(RecordHead.RecordHead.ICName != ICName)
					{
						icE();
						return;
					}
				}
				if(irkeyC>sizeof(IrKeyA)-1)
				{
					//nextgrup
					if(IrCompGroup<MAX_IRCOMPGROUP)
					{
						RecordHead.RecordHead.Check += saveRecord();
						irkeyC=0;
						IrCompGroup++;
					}
					else
					{
						ShowString("DATAFULL",8,8);
						bbwarn();
						break;
					}
				}
				IrKeyA[irkeyC] = IrInformation.KeyCode;
				if(!samekey)
				{
					irkeyC++;
				}
				ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
				ShowIrInformation();
				break;
			}
		}
		OldIRKey=IrInformation.KeyCode;
	}

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