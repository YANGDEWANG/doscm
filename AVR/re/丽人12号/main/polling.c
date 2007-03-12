#include <stdio.h>
#include <Global.h>
#include <lcd.h>

#include "dwstd.h"
#include "Clock.h"
#include "polling.h"
#include "ADC.h"
#include "M62429P.h"
#include "..\dev\pointvfddisplay.h"
#include "..\dev\STF16360EN.h"
#include <fft.h> 
#include "..\ui\key_command.h"
#include "..\ui\ir_command.h"
#include "..\ui\c.h"
////#define POLLING200MS
#define POLLING1000MS
#define SOUND_MIN_LEVE 100
uint8 UserEventExitCount;
bool ShowPingPu;
bool HaveSound;
bool SearchSounding;
bool HaveMin;//咪头
//uint8 WorkState;

//uint8 ErrorCode;
char stringbuff[STRING_BUFF_SIZE];


void ExitUserEvent()
{
	DISClean();
	if(Mute)
	{
		ShowString_P2(ssMUTE_,ssON,POINTVFDDISPLAY_X_C);
	}
	else
	{
		ShowPingPu = true;
	}
	ControlState = 0;
	EepromWriteEX();
}
void InUserEvent()
{
	ShowPingPu = false;
	UserEventExitCount = 0;
}
void drawDot(IndexScreenLine x,IndexScreenLine y)
{
	uint16 dot ;
	u8 dat,dat2;
	u8 add;
	dot = y*POINTVFDDISPLAY_X_P+x;
	add = dot/8;
#ifdef DEBUG
	ISTRUESTOP(add>=sizeof(dismem));
#endif
	dat = dismem[add];
	dat2=_BV(dot%8);
	if(DrawColor)
		dismem[add] = dat|dat2;
	else
		dismem[add] =dat&(~dat2);
}
u8 oldPingpuData[72];
//u8 oldPpTop[72];
u8 pudata[74];
#define PU_TOP_TIME 15
#define PU_FLL_TIME 2
u8 putime;
u8 puflltime;


void DoADC()
{
	u8 x,d,tmp,oldD=0;
	u16 dat;
	u8 i;
	if(ADCEnd)
	{
		if(PUType==PP_BOXING)
		{
			for(x=0;x<72;x++)
			{
				dat = ADCSample[x];
				pudata[x]=dat/(0xffff/6);
			}
		}
		WindowCalc(ADCSample, 1); // Window Real Data, and convert to
		Bit_Reverse(ADCSample); // Sort Real (Input) Data in bit-reverse
		Int_FFT(ADCSample, Imag); // Perform FFT on data
		i16 *ImaeP = Imag+1;
		u8* pudataP = pudata;
		oldD = pudata[3];
		for(x=0;x<24;x++)
		{

			dat = abs(ImaeP[x]);
			if(dat>SOUND_MIN_LEVE)
			{
				HaveSound = true;
			}
			//显屏普
			
			if(PUType==PP_PP||PUType==PP_PP_T)
			{
				d=dat/10;
				for(i=0;i<8;i++)
				{
					if(d==0)
					{
						break;
					}
					d>>=1;
				}
				tmp = (u8)(i+oldD)>>1;
				*pudataP++=(u8)(tmp+oldD)>>1;
				*pudataP++=(u8)(tmp+i)>>1;
				*pudataP++=i;
				oldD = i;
			}
		}
		//开始新的ADC
		StADC(1);
		if(ShowPingPu&&(!SearchSounding))
		{
			if(PUType==PP_PP_W)
			{
				for(x=0;x<64;x++)
				{
					dat = abs(Imag[x]);
					d=dat/10;
					for(i=0;i<8;i++)
					{
						if(d==0)
						{
							break;
						}
						d>>=1;
					}
					pudata[x]=i;
				}
			}
			for(x=0;x<72;x++)
			{
				oldD = oldPingpuData[x];
				//tmp = oldPpTop[x];
				d=pudata[x];
				if(PUType==PP_PP||PUType==PP_PP_T)
				{
					if(d<oldD)
					{
						d = oldD-1;
					}
				}
				if(PUType==PP_PP||PUType==PP_PP_W)
				{
					u8 i;
					DrawColor = true;
					for(i=1;i<d;i++)
					{
						drawDot(x,POINTVFDDISPLAY_Y_P-i);
					}
					DrawColor = false;
					for(;i<=POINTVFDDISPLAY_Y_P;i++)
					{
						drawDot(x,POINTVFDDISPLAY_Y_P-i);
					}
				}
				else if(PUType==PP_BOXING||PUType==PP_PP_T)
				{
					DrawColor = true;
					drawDot(x,POINTVFDDISPLAY_Y_P-1-d);
					if(d!=oldD)
					{
						DrawColor = false;
						drawDot(x,POINTVFDDISPLAY_Y_P-1-oldD);
					}
					
				}
				oldPingpuData[x]=d;
			}
		}


	}
	//指正//
	//DrawLine(1,7,2,0);
	//DrawLine(10,6,(ADC_H[0]-127)/10+10,-1);
	//DrawLine(10,0,(ADC_H[0]-127)/10+10,7);
	//DrawRectangle(1,7-(ADC_H[0]/(255/7)),5,6);
	//FillRectangle(1,7-(ADC_H[0]/(255/7)),21,6);
	//FillRectangle(0,ADC_H[0]/(255/7),20,7);//74160:74026:73185:73137:72566:70791
	//		FillRectangle(ADC_H[0]/(255/20),0,20,7);//74160:74026:73185:73137:72566:70791
	//FillRectangle(15,0,20,7);//74160:74026:73185:73137:72566:70791


}
//void polling60ms()
//{
//	PollingKey60ms();
//}
//void polling10ms()
//{
//	DoADC();
//	//M62429PUpdateAll();
//}
//void Polling150ms()
//{
//	PollingIRKey();
//	PollingAutoControl();
//}
#ifdef POLLING200MS
void polling200ms()
{

}
#endif//POLLING200MS
#ifdef POLLING500MS
void polling500ms()
{
	//	static uint8 i='0';
	//
	////ToStringWithXFW(stringbuff,i);
	//DISClean();
	//stringbuff[0]=i;
	//ShowString(stringbuff,3,1);
	//i++;


}
#endif//POLLING500MS
#ifdef POLLING1000MS
static void polling10s()
{
	if(HaveSound)
	{
		HaveSound = false;
	}
	else
	{

		SearchSounding = true;
	}
}
static u8 intputC = INTPUT_MIN;
prog_char ssSearching[]="SEARCHING";
static void NextIntput()
{
	ShowStringALL_P(ssSearching);
	intputC++;
	if(intputC>INTPUT_MAX)
	{
		intputC=INTPUT_MIN;
	}
	SetIntput(intputC);
}

void IntoSearchSound()
{
	NextIntput();
	HaveSound = false;
	SearchSounding = true;
}

void polling1000ms()
{
	static u8 polling10s_dat;
	if(SearchSounding)
	{
		if(HaveSound)
		{
			InUserEvent();
			ShowIntput(intputC);
			SearchSounding=false;
			AutoControl.Step=1;
		}
		else
		{
			NextIntput();
		}
	}
	if(UserEventExitCount<MaxUserEventCountDown)
	{
		UserEventExitCount++;
		if(UserEventExitCount==MaxUserEventCountDown)
			ExitUserEvent();
	}
	polling10s_dat++;
	if(polling10s_dat>9)
	{
		polling10s_dat = 0;
		polling10s();
	}
}
#endif//POLLING1000MS
prog_char huanying[] ={'Z'+1, 'Z'+2,'Z'+3,'Z'+4,'Z'+5,'Z'+6,'Z'+7,'Z'+8};
void iniPoll()
{
	//UserEventExitCount =  0;
	
	SearchSounding = true;
	////ToStringWithXFW(dislinebuf,18);
	////ShowString(dislinebuf,2,4);
	//ShowString("HELLO",9,7);
	////UpdateDisplay();
	//StandardL	=0;
	//ShowString("123",0,3);
	ShowString_P(huanying,4,sizeof(huanying));
	//DrawLine(0,0,70,7);

}
#ifdef DWDEBUGF
uint8 debugC;
void debugkeyc();
void debug()
{
	debugkeyc();
}
#endif

uint8 SysClickMS;
void PollingMain()
{

	uint8 oldSysClick	= SysClick;
	//uint8 oldc1ms	= SysClick;
	uint8 oldc60ms		= 0;
	uint8 oldc10ms		= 0;
	uint8 oldc150ms		= 0;
#ifdef POLLING200MS
	uint8 oldc200ms		= 0;
#endif//POLLING200MS
#ifdef POLLING500MS
	uint8 oldc500ms		= 0;
#endif//POLLING500MS
#ifdef POLLING1000MS
	uint8 oldc1000ms	= 0;
#endif//POLLING1000MS
	iniPoll();
	while(true)
	{
		////poll 1ms
		//if((uint8)(SysClick-oldc1ms)>1000/CLICK_CYCLE_US)
		//{
		//	//Pollingpointvfddisplay();
		//	oldc1ms+=1000/CLICK_CYCLE_US;
		//}
		if((uint8)(SysClick-oldSysClick)>CLICK_CYCLE_MS*1000/CLICK_CYCLE_US)
		{
			SysClickMS++;
			oldSysClick+=CLICK_CYCLE_MS*1000/CLICK_CYCLE_US;
		}
#ifdef DWDEBUGF
		debug();	
#endif//DWDEBUGF
		//IniEeprom_ex();
		//SysClickMS = SysClick;
		if((uint8)(SysClickMS-oldc10ms)>10/CLICK_CYCLE_MS)
		{
			//polling10ms();
			DoADC();
			oldc10ms+=10/CLICK_CYCLE_MS;

		}
		if((uint8)(SysClickMS-oldc60ms)>60/CLICK_CYCLE_MS)
		{
			//polling60ms();
			PollingKey60ms();
			oldc60ms+=60/CLICK_CYCLE_MS;
		}
		if((uint8)(SysClickMS-oldc150ms)>150/CLICK_CYCLE_MS)
		{

			//Polling150ms();
			PollingIRKey();
			PollingAutoControl();
			oldc150ms+=150/CLICK_CYCLE_MS;
		}
#ifdef POLLING200MS
		if((uint8)(SysClickMS-oldc200ms)>200/CLICK_CYCLE_MS)
		{
			//IrDevDiv();
			polling200ms();
			oldc200ms+=200/CLICK_CYCLE_MS;
		}
#endif//POLLING200MS
		#ifdef POLLING500MS
		if((uint8)(SysClickMS-oldc500ms)>500/CLICK_CYCLE_MS)
		{
			polling500ms();
			oldc500ms+=500/CLICK_CYCLE_MS;
		}
#endif//POLLING500MS
#ifdef POLLING1000MS
		if((uint8)(SysClickMS-oldc1000ms)>1000/CLICK_CYCLE_MS)
		{	
			polling1000ms();
			oldc1000ms+=1000/CLICK_CYCLE_MS;
		}
#endif//POLLING1000MS

		//OK
		if(HaveMin)
		{
			if(!INOK)
			{
				HaveMin = false;
				InUserEvent();
				ShowString_P2(ssOK_,ssOFF,POINTVFDDISPLAY_X_C);
			}
		}
		else
		{
			if(INOK)
			{
				HaveMin = true;
				InUserEvent();
				ShowString_P2(ssOK_,ssON,POINTVFDDISPLAY_X_C);
			}
		}
	}

}
prog_char ss5_1ch[]="5-1 CH";
prog_char ss2_1ch[]="2-1 CH";
void ShowState()
{
	uint8 sd;
	//bool eState = false;

	const prog_char* s2=ssnull,*s1=ssVOL_;
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:
		{
			sd=MainVolume;
			goto showNODiv;
		}
		/*case CS_VOLUME_L:
		s2=ssVOL;
		sd=PT2314Volume;break;
		case CS_VOLUME_R:	
		s2=ssVOL;
		sd=PT2314Volume;break;*/

	case CS_VOLUME_CC:
		{
			s2=ssCC;
			sd=M62429PVolumeA[3];break;
		}
	case CS_VOLUME_SW:
		{
			s2=ssSW;
			sd=M62429PVolumeA[2];break;
		}
	case CS_VOLUME_SL:	
		{
			s2=ssSL;
			sd=M62429PVolumeA[0];break;
		}
	case CS_VOLUME_SR:
		{
			s2=ssSR;
			sd=M62429PVolumeA[1];break;
		}

	case CS_OK_VOLUME:
		{
			s2=ssOK_;
			sd=PT2314_2Volume;
			break;
		}
	case CS_OK_HUNXIANG:
		{
			s2=ssECHO;
			s1=ssnull;
			sd=PT2314_2SpeakerATT+15;
			break;
		}
	case CS_OK_DELAY:
		{
			s2=ssDELAY;
			s1=ssOK_;
			sd=OKDelay;
			goto showNODiv;
		}
	case CS_OK_DEYIN:
		{
			s2=ssOK_;
			s1=ssBASS;
			sd=PT2314_2Bass;
			goto showNODiv;
		}
	case CS_OK_GAOYIN:
		{
			s2=ssOK_;
			s1=ssTREBLE;
			sd=PT2314_2Treble;
			goto showNODiv;
		}
		case CS_TRACK_MODE://goto NO_CS_VOLUME
			{
				switch (TrackMode)
				{
				case TM_5_1CH:
					{
						ShowStringALL_P(ss5_1ch);
						break;
					}
				case TM_2_1CH:
					{
						ShowStringALL_P(ss2_1ch);
						break;
					}
				}
				return;
			}
	default:
		{
			s1=ssnull;
			switch (ControlState)
			{
			case CS_BASS:	
				{
					s2=ssBASS;
					sd=PT2314Bass;
					goto showNODiv;
				}
			case CS_TREBLE:	
				{
					s2=ssTREBLE;
					sd=PT2314Treble;
					goto showNODiv;
				}
			case CS_VOLUME_ATT:
				{
					s2=ssATT;
					sd=PT2314SpeakerATT;
					goto showNODiv;
					//break;
				}
			default:
				{
					return;
				}
			}
		}
	}
	//DISClean();
	sd = sd/2;
showNODiv:
	/*ShowString_P(s2,0,POINTVFDDISPLAY_X_C);
	ShowINT8(sd);*/
	ShowStringAndI8_P2(s1,s2,sd);


	//NO_CS_VOLUME:

	//switch(M62446OutputPort&M62446OUTPUTPORT_MASK)
	//{
	//case INTPUT_5_1:sd = DISP_AC_3;break;
	//case INTPUT_CD:sd = DISP_CD;break;
	//case INTPUT_AUX:sd = DISP_AUX;break;
	//}
	//if(ControlState==CS_INTPUT_SELECT)
	//{
	//	DisplayGlintExitState = false;//will bug
	//	ssd = sd;
	//	ShowString("IN");
	//	eState = true;
	//}

}


