#include <global.h>
#include "key_command.h"
#include "TM1618.h"
#include <stdio.h>
#include <delay.h>
#include <string.h>
#include <arith.h>
#include "dev\irdev.h"
#include <at24c02.h>
//#include "c.h"

#define CUSTO_MNAME_LC (8+5)
#define showErrStr(s) ShowString(s,8,8)

void setSTB0();
void clSTB0();
void setSTB1();
void clSTB1();


static uint8 oldk;
static uint8 REPkeyC = KEY_REPEAT_TIME;
static uint8 ShowDetailedC;
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
//显示并清除错误代码
void ShowError()
{
	ShowString("ERR",8,5);
	ShowUint8(15,ErrorCode);
	ErrorCode = EC_OK;
}
static void loadRecordHead()
{
	At24c02Choice = 0;
	Read24c02(CurrentRecord*sizeof(IRRecordHead),RecordHead.dat,sizeof(IRRecordHead));
	MultiCustomCode = RecordHead.RecordHead.MultiCustomCode;
	if(RecordHead.RecordHead.ICName>IC_NULL)
		RecordHead.RecordHead.ICName=IC_NULL;
}
static void saveRecordHead()
{
	At24c02Choice = 0;
	Write24c02(CurrentRecord*sizeof(IRRecordHead),RecordHead.dat,sizeof(IRRecordHead));
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
	//ShowRecordHead();
	//WorkState = WS_STANDARD;
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
static void NameInput(bool ud)
{
	uint8 lc = GlintLc - CUSTO_MNAME_LC;
	char c = EddyMovePointer('0','Z',RecordHead.RecordHead.CustomName[lc],ud);
	if(c>'9'&&c<'A')
	{
		if(ud)
			c ='A';
		else
			c='9';
	}
	RecordHead.RecordHead.CustomName[lc] = c;
}
static void updown(bool ud)
{

	switch(WorkState)
	{

	case WS_IR_COMP_RECORD_PICK:
	case WS_IR_RECORD_RECORD_PICK:
		{
			CurrentRecord = EddyMovePointer(0,MAX_RECORD_COUNT,CurrentRecord,ud);

			if(WorkState==WS_IR_COMP_RECORD_PICK)
			{
				CheckRecordData();
			}
			else
			{
				loadRecordHead();
			}
			break;
		}
	case WS_IR_RecordNameInput:
		{
			NameInput(ud);
			break;
		}
	case WS_IR_SHOW_DETAILED:
		{
			if(IRChangec>2)
				ShowDetailedC = EddyMovePointer(0,IRChangec-2,ShowDetailedC,ud);
			break;
		}
	case WS_IR_RECORD:
		{
			irkeyC = EddyMovePointer(0,sizeof(IrKeyA)-1,irkeyC,ud);
			ToStringWithU(dislinebuf,IrCompGroup*sizeof(IrKeyA)+irkeyC);
			ShowString(dislinebuf,6,2);
			break;
		}
	}
}

void PollingKey60ms()
{
	bool kf = false;
	bool showstate = true;
	uint8 k = getkey();
	//	PollingOK();
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
		bbONTime(2);
		InUserEvent();
		StopGlint();
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
				WorkState = WS_STANDARD;
				break;
			}
		case KEYC_COMP:
			{
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
						WorkState = WS_IR_SHOW_DETAILED;
						ShowDetailedC = 0;
						break;
					}
				case WS_IR_COMP_RECORD_PICK:
					{
						WorkState = WS_IR_COMP;
						break;
					}
				case WS_IR_COMP:
					{
						WorkState = WS_IR_COMP_RECORD_PICK;
						break;
					}
				case WS_IR_RECORD:
					{
						if(ErrorCode == EC_CustomCodeComp)
						{
							MultiCustomCode = true;
							showErrStr("MCUSTOM");
							ErrorCode = EC_OK;
							showstate = false;
						}
						else//结束纪录并保存
						{
							endRecord();
							WorkState = WS_STANDARD;
						}
						break;
					}
				case WS_IR_RECORD_RECORD_PICK:
					{
						WorkState = WS_IR_RecordNameInput;
						GlintLc = CUSTO_MNAME_LC;
						memset(RecordHead.RecordHead.CustomName,'0',3);
						break;
					}
				case WS_IR_RecordNameInput:
					{

						if(GlintLc<CUSTO_MNAME_LC+2)
						{
							GlintLc++;
						}
						else
						{							
							WorkState = WS_IR_RECORD;
							RecordHead.RecordHead.Check = 0;
							IrCompGroup=irkeyC=0;
						}
						break;
					}
				}
				break;
			}
		}
		if(showstate)
			ShowState();
	}
}
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
static void ShowRecordHead()
{
	ShowUint8(7,CurrentRecord);
	ShowString(ICNameS[RecordHead.RecordHead.ICName],0,6);
	ShowUint8(10,RecordHead.RecordHead.KeyCount);
	ShowString(RecordHead.RecordHead.CustomName,CUSTO_MNAME_LC,3);
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
	case WS_IR_SHOW_DETAILED:
		{
			StandardL	=0;
			showEtailed();
			break;
		}
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
	showErrStr("ERR CODE");
}
static void keyE(){
	bberr();
	showErrStr("ERR KEY");
}
void PollingIRkey()
{
	bool samekey = false;
	if(IRKeyDown)
	{
		IRKeyDown = false;
		InUserEvent();
		bbstd();
		if(IRKeyHold)
		{
			IRKeyHold=false;
		}
		if(WorkState == WS_IR_SHOW_DETAILED)
			WorkState = WS_STANDARD;//退出WS_IR_SHOW_DETAILED
		if(OldIRKey==IrInformation.KeyCode)
		{
			samekey = true;
			//goto end;//相同按键并无按键退出
		}
		//notifyKeyDown = false;

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
				//if(irkeyC>=sizeof(IrKeyA))
				//irkeyC = 0;
				//IrKeyA[irkeyC] = IrInformation.KeyCode;
				//irkeyC++;
				//ShowUint8(7,irkeyC);
				ShowIrInformation();
				break;
			}
		case WS_IR_COMP:
			{
				ShowString(ICNameS[ICName],0,6);
				ToStringWithXFW(dislinebuf,IrInformation.CustomCode);
				ShowString(dislinebuf,6,2);
				if(irkeyC==sizeof(IrKeyA))
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
						break;
					}
					loadRecord();
				}

				if(IrKeyA[irkeyC] != IrInformation.KeyCode&&!samekey)
				{
					keyE();
				}else if(!MultiCustomCode
					&&(RecordHead.RecordHead.CustomCode != IrInformation.CustomCode
					||RecordHead.RecordHead.CustomCodeReverse != IrInformation.CustomCodeReverse))
				{
					codeE();
				}else if(RecordHead.RecordHead.ICName != ICName)
				{
					icE();
				}
				else 
				{
					if(!samekey)
						irkeyC++;
					if(IrCompGroup*sizeof(IrKeyA)+irkeyC==RecordHead.RecordHead.KeyCount)//end
					{
						//next ir
						IrCompGroup=0;
						irkeyC=0;
						loadRecord();
						ShowString("-END-",8,8);
					}
					else
					{
						ToStringWithU(dislinebuf,IrCompGroup*sizeof(IrKeyA)+irkeyC);
						ShowString(dislinebuf,8,8);
					}
					break;
				}
				goto noUpdateOldIRKey;
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
						codeE();
						goto noUpdateOldIRKey;
					}
					if(RecordHead.RecordHead.ICName != ICName)
					{
						icE();
						break;
					}
				}
				if(irkeyC==sizeof(IrKeyA))
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
						goto end;
						break;
					}
				}
				IrKeyA[irkeyC] = IrInformation.KeyCode;
				if(!samekey)
					irkeyC++;
				ShowUint8(7,IrCompGroup*sizeof(IrKeyA)+irkeyC);
				ShowIrInformation();
				break;
			}
		}
end:
		OldIRKey=IrInformation.KeyCode;
noUpdateOldIRKey:
		//UpdateDisplay();
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