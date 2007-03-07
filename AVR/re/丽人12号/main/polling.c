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
////#define POLLING200MS
#define POLLING1000MS
////uint8 UserEventExitCount;
//uint8 WorkState;

//uint8 ErrorCode;
char stringbuff[STRING_BUFF_SIZE];


//void ExitUserEvent()
//{
//WorkState= WS_STANDARD;	
//ShowState();//导致按相同遥控按键无显示更改
//}
void polling60ms()
{
	PollingKey60ms();
	//DISClean();
	if(0)//(ADCEnd)
	{
		//显屏普
		DISClean();//70612
		WindowCalc(ADCSample, 1); // Window Real Data, and convert to
		// differential if it is single-ended
		Bit_Reverse(ADCSample); // Sort Real (Input) Data in bit-reverse
		// order
		Int_FFT(ADCSample, Imag); // Perform FFT on data
		uint8 x;
		for(x=0;x<POINTVFDDISPLAY_X_P;x++)
		{
			//DrawLine(x,POINTVFDDISPLAY_Y_P,x,POINTVFDDISPLAY_Y_P-abs(Imag[x*3])/(0xffff/POINTVFDDISPLAY_Y_P/2));
			DrawLine(x,0,x,abs(Imag[x*3]));
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
		//开始新的ADC
		StADC(1);
	}
}
void polling10ms()
{
	M62429PUpdateAll();
}
#ifdef POLLING200MS
void polling200ms()
{

}
#endif//POLLING200MS
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
#ifdef POLLING1000MS
void polling1000ms()
{
}
#endif//POLLING1000MS
prog_char huanying[] ={'Z'+1, 'Z'+2,'Z'+3,'Z'+4,'Z'+5,'Z'+6,'Z'+7,'Z'+8};
static void iniPoll()
{
	////UserEventExitCount =  MaxUserEventCountDown;
	////ToStringWithXFW(dislinebuf,18);
	////ShowString(dislinebuf,2,4);
	//ShowString("HELLO",9,7);
	////UpdateDisplay();
	//StandardL	=0;
	//ShowString("123",0,3);
	memcpy_P(stringbuff,huanying,sizeof(huanying));
	ShowString(stringbuff,4,sizeof(huanying));
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
	uint8 oldc1ms	= SysClick;
	uint8 oldc60ms		= 0;
	uint8 oldc10ms		= 0;
	uint8 oldc150ms		= 0;
#ifdef POLLING200MS
	uint8 oldc200ms		= 0;
#endif//POLLING200MS
	uint8 oldc500ms		= 0;
#ifdef POLLING1000MS
	uint8 oldc1000ms	= 0;
#endif//POLLING1000MS
	iniPoll();
	while(true)
	{
		/////////


		/////////
		//poll 1ms
		if((uint8)(SysClick-oldc1ms)>1000/CLICK_CYCLE_US)
		{
			//Pollingpointvfddisplay();
			oldc1ms+=1000/CLICK_CYCLE_US;
		}
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
			polling10ms();
			oldc10ms+=10/CLICK_CYCLE_MS;
		}
		if((uint8)(SysClickMS-oldc60ms)>60/CLICK_CYCLE_MS)
		{
			polling60ms();
			oldc60ms+=60/CLICK_CYCLE_MS;
		}
		if((uint8)(SysClickMS-oldc150ms)>150/CLICK_CYCLE_MS)
		{
			PollingIRKey();
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
		if((uint8)(SysClickMS-oldc500ms)>500/CLICK_CYCLE_MS)
		{
			polling500ms();
			oldc500ms+=500/CLICK_CYCLE_MS;
		}
#ifdef POLLING1000MS
		if((uint8)(SysClickMS-oldc1000ms)>1000/CLICK_CYCLE_MS)
		{	
			polling1000ms();
			oldc1000ms+=1000/CLICK_CYCLE_MS;
		}
#endif//POLLING1000MS
		//IrDevDiv();
		////---------------------ir--------------------//
		//PollingIRkey();
		//if(DisplayChanged)
		{
			//	UpdateDisplay();
		}
	}
}


