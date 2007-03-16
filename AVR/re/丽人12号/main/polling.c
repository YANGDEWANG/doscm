#include <stdio.h>
#include <Global.h>
#include <lcd.h>
#include <avr/wdt.h> 
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
#define SOUND_MIN_LEVEL_MUTE	20
#define SOUND_MIN_LEVEL_SEARCH	100
u8 SoundMinLevel = SOUND_MIN_LEVEL_SEARCH;
uint8 UserEventExitCount;
bool ShowPingPu;
bool HaveSound;
bool SearchSounding;
bool HaveMin;//咪头
//uint8 WorkState;

//uint8 ErrorCode;
char stringbuff[STRING_BUFF_SIZE];

void ShowBankgrund()
{

	if(Mute)
	{
		ShowString_P2(ssMUTE_,ssON,POINTVFDDISPLAY_X_C);
		offfan();
	}
	else
	{
		if(checkSound())
		{
			if(!ShowPingPu)
			{
				DISClean();
				ShowPingPu = true;
				if(MainVolume>25)
				{
					onfan();
				}
				else
				{
					offfan();
				}
			}
		}
		else
		{
			ShowPingPu = false;
			ShowString("NO SIGNAL",0,POINTVFDDISPLAY_X_C);
			offfan();
		}
	}
}
void ExitUserEvent()
{
	ShowBankgrund();
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
		/*if(PUType==PP_BOXING)
		{
		for(x=0;x<72;x++)
		{
		dat = ADCSample[x];
		pudata[x]=dat/(0xffff/6);
		}
		}*/
		WindowCalc(ADCSample, 1); // Window Real Data, and convert to
		Bit_Reverse(ADCSample); // Sort Real (Input) Data in bit-reverse
		Int_FFT(ADCSample, Imag); // Perform FFT on data
		i16 *ImaeP = Imag+1;
		u8* pudataP = pudata;
		oldD = pudata[3];
		for(x=0;x<24;x++)
		{

			dat = abs(ImaeP[x]);
			if(dat>SoundMinLevel)
			{
				HaveSound = true;
			}
			//显屏普

			if(PUType<PP_PP_W)
			{
				d=dat/10;
				for(i=0;i<7;i++)
				{
					if(d==0)
					{
						break;
					}
					d>>=1;
				}
				if(PP_PP3==PUType||PP_PP_T==PUType)
				{
					*pudataP++=i;
					*pudataP++=i;
					*pudataP++=i;
				}
				else
				{
					tmp = (u8)(i+oldD)>>1;
					*pudataP++=(u8)(tmp+oldD)>>1;
					*pudataP++=(u8)(tmp+i)>>1;
					*pudataP++=i;
					oldD = i;
				}
			}
		}
		
		//开始新的ADC
		StADC(1);
		if(ShowPingPu&&(!SearchSounding))
		{
			if(PUType==PP_PP_W)
			{
				pudataP=pudata+4;
				for(x=0;x<64;x++)
				{
					dat = abs(Imag[x]);
					d=dat/10;
					for(i=0;i<7;i++)
					{
						if(d==0)
						{
							break;
						}
						d>>=1;
					}
					pudataP[x]=i;
				}
				for(;x<64+4;x++)
				{
					pudataP[x]=i;
				}
				for(x=0;x<6;x++)
				{
					pudata[x]=pudata[5];
				}
			}
			for(x=0;x<72;x++)
			{
				oldD = oldPingpuData[x];
				//tmp = oldPpTop[x];
				d=pudata[x];
				if(d<oldD)
				{
					d = oldD-1;
				}
				/*if(PUType==PP_PP||PUType==PP_PP_T)
				{

				}*/

				if(PUType==PP_PP_T)
				{
					if(d!=0)
					{
					DrawColor = true;
					drawDot(x,POINTVFDDISPLAY_Y_P-d);
					}
					if(d!=oldD&&oldD!=0)
					{
						DrawColor = false;
						drawDot(x,POINTVFDDISPLAY_Y_P-oldD);
					}

				}
				else if(PUType==PP_PP_F)
				{
					u8 i;
					DrawColor = true;
					for(i=0;i<d;i++)
					{
						drawDot(x,i);
					}
					DrawColor = false;
					for(;i<POINTVFDDISPLAY_Y_P;i++)
					{
						drawDot(x,i);
					}
				}
				else
				{
					u8 i;
					DrawColor = true;
					for(i=1;i<=d;i++)
					{
						drawDot(x,POINTVFDDISPLAY_Y_P-i);
					}
					DrawColor = false;
					for(;i<=POINTVFDDISPLAY_Y_P;i++)
					{
						drawDot(x,POINTVFDDISPLAY_Y_P-i);
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
//static void polling10s()
//{
//	if(HaveSound)
//	{
//		HaveSound = false;
//	}
//	else
//	{
//
//		SearchSounding = true;
//	}
//}
static u8 intputC = INTPUT_MIN;
//prog_char ssSearching[]="SEARCH";
void NextIntput(bool show)
{
	//ShowStringALL_P(ssSearching);
	intputC++;
	if(intputC>INTPUT_MAX)
	{
		intputC=INTPUT_MIN;
	}
	SetIntput(intputC);
	if(show)
	{
		ShowIntput(intputC);
	}
}

//void IntoSearchSound()
//{
//	NextIntput();
//	HaveSound = false;
//	SearchSounding = true;
//}

void polling1000ms()
{
	//不晓得需要不
	M62429PUpdateAll();
	//static u8 polling10s_dat;
	if(SearchSounding)
	{
		if(HaveSound)
		{
			InUserEvent();
			//ShowIntput(intputC);
			SearchSounding=false;
			SoundMinLevel = SOUND_MIN_LEVEL_MUTE;
			AutoControl.Step=1;
		}
		else
		{
			NextIntput(false);
		}
	}
	if(UserEventExitCount<MaxUserEventCountDown)
	{
		UserEventExitCount++;
		if(UserEventExitCount==MaxUserEventCountDown)
			ExitUserEvent();
	}
	else
	{
		ShowBankgrund();
	}
	/*polling10s_dat++;
	if(polling10s_dat>9)
	{
		polling10s_dat = 0;
		polling10s();
	}*/
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
	/////////////////开看门狗///////////////////
	wdt_enable(WDTO_1S);
	while(true)
	{
		///////////喂狗///////////
		wdt_reset();  
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
		if((uint8)(SysClickMS-oldc10ms)>50/CLICK_CYCLE_MS)
		{
			//polling10ms();
			DoADC();
			oldc10ms+=50/CLICK_CYCLE_MS;

		}
		if((uint8)(SysClickMS-oldc60ms)>60/CLICK_CYCLE_MS)
		{
			//polling60ms();
			
			PollingKey60ms();
			oldc60ms+=60/CLICK_CYCLE_MS;
		}
		if((uint8)(SysClickMS-oldc150ms)>150/CLICK_CYCLE_MS)
		{
#define HSDL_TIME 100
			static u8  hsdl= 0;
			if(HaveSound)
			{
				onSound();
				hsdl = 0;
			}
			else
			{
				
				if(hsdl<HSDL_TIME)
					hsdl++;
				else
					offSound();
			}
			
			if(!SearchSounding)
			{
				HaveSound = false;
			}
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

		
	}

}
prog_char ss5_1ch[]="5-1 CH";
prog_char ss2_1ch[]="2-1 CH";
void ShowState()
{
	uint8 sd = 0;
	//bool eState = false;

	const prog_char* s2=ssnull,*s1=ssVOL_;
	bool esl = true;
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:
		{
			//sd=MainVolume;
			//esl = ESL_MainVolume;
			break;
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
			//sd=M62429PVolumeA[3];
			//esl = ESL_VolumeCC;
			break;
		}
	case CS_VOLUME_SW:
		{
			s2=ssSW;
			//esl = ESL_VolumeSW;
			//sd=M62429PVolumeA[2];
			break;
		}
	case CS_VOLUME_SL:	
		{
			s2=ssSL;
			//sd=M62429PVolumeA[0];
			//esl = ESL_VolumeSL;
			break;
		}
	case CS_VOLUME_SR:
		{
			s2=ssSR;
			//sd=M62429PVolumeA[1];
			//esl = ESL_VolumeSR;
			break;
		}

	case CS_OK_VOLUME:
		{
			s2=ssOK_;
			sd=PT2314_2Volume>>1;
			esl = false;
			break;
		}
	case CS_OK_HUNXIANG:
		{
			s2=ssECHO;
			s1=ssnull;
			sd=PT2314_2SpeakerATT+15;
			esl = false;
			break;
		}
	case CS_OK_DELAY:
		{
			s2=ssDELAY;
			s1=ssOK_;
			//sd=OKDelay;
			//esl = ESL_OKDelay;
			goto derctshow;
		}
	case CS_OK_DEYIN:
		{
			s1=ssOK_;
			s2=ssBASS;
			//sd=PT2314_2Bass;
			//esl = ESL_PT2314_2Bass;
			goto derctshow;
		}
	case CS_OK_GAOYIN:
		{
			s1=ssOK_;
			s2=ssTREBLE;
			//sd=PT2314_2Treble;
			//esl = ESL_PT2314_2Treble;
			goto derctshow;
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
					//sd=PT2314Bass;
					//esl = ESL_PT2314Bass;
					goto derctshow;
				}
			case CS_TREBLE:	
				{
					s2=ssTREBLE;
					//sd=PT2314Treble;
					//esl = ESL_PT2314Treble;
					goto derctshow;
				}
			case CS_VOLUME_ATT:
				{
					s2=ssATT;
					//sd=PT2314SpeakerATT;
					//esl = ESL_PT2314SpeakerATT;
					break;
					//break;
				}
			default:
				{
					return;
				}
			}
		}
	}

	if(esl)
	{
		sd = EepromBuffer[ControlState];
	}
	if(sd==0)
	{
		ShowString_P2(s1,s2,POINTVFDDISPLAY_X_C-3);
		ShowString_P(ssOFF,(POINTVFDDISPLAY_X_C-3)*CHARIMAGE_W,3);
		return;
	}
derctshow:	
	if(esl)
	{
		sd = EepromBuffer[ControlState];
	}
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


