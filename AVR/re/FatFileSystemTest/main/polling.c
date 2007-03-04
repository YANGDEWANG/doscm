#include <stdio.h>
#include <Global.h>
#include <lcd.h>
#include "dwstd.h"
#include "Clock.h"
#include "spi.h"
#include "polling.h"
#include "usart.h"
#include "card/mmc/MMC.h"
#include "../dev/spiflash.h"
#include "../dev/spiflashpro.h"
//#include "TM1618.h"
//#include "irdev.h"
//#include "ui\key_command.h"
////#define POLLING200MS
#define POLLING1000MS
////uint8 UserEventExitCount;
//uint8 WorkState;

//uint8 ErrorCode;
uint16 time = 0;
char stringbuff[STRING_BUFF_SIZE];
uint8 bbEtime;
bool ebb;
bool haveCard=false;
uint8 bbtime;

#define ONBB()  DDRD|=(1<<3)
#define OFFBB() DDRD&=~(1<<3)

//void ExitUserEvent()
//{
//WorkState= WS_STANDARD;	
//ShowState();//导致按相同遥控按键无显示更改
//}
void showTime()
{
	uint8 h = time/60/60;
	uint8 m = time/60;
	uint8 s = time%60;
	//memset(stringbuff,'0',8);
	ToStringWithUFW(stringbuff,h,2);
	stringbuff[2] = ':';
	ToStringWithUFW(stringbuff+3,m,2);
	stringbuff[5] = ':';
	ToStringWithUFW(stringbuff+6,s,2);
	//stringbuff[8] = ':';
	stringbuff[8] =0;
	LCDShowStringAt(0,stringbuff);
}
void polling60ms()
{
	//PollingKey60ms();
}
void polling10ms()
{

	//static uint8 mm;
	//mm++;
	//if(mm>99)
	//	mm=0;
	//ToStringWithUFW(stringbuff,mm,2);
	//stringbuff[2] = 0;
	//LCDShowStringAt(9,stringbuff);
	//-----------------------------------
	if(bbtime==0&&ebb)
	{
		bbEtime = 3;
		ebb = false;
	}
	//蜂鸣器
	if(bbtime)
	{
		ONBB();
		bbtime--;
	}
	else
	{
		OFFBB();
	}
	if(bbEtime)
	{
		bbEtime--;
		if(bbEtime==0)
		{
			bbtime = 16;
		}
	}
	//---------------------------------------	

}
#ifdef POLLING200MS
void polling200ms()
{

}
#endif//POLLING200MS
void polling500ms()
{
	//DDRC = 0xff;
	//PORTC = 0;
	//ASSR = 0;
	//LCDClear();
	//LCDShowStringAt(5,"yangdewang");
	//	PollingDisplay500ms();

}
#ifdef POLLING1000MS
uint8 buf[256];
void polling1000ms()
{
	LCDClear();
	time++;
	showTime();
	bbONTime(8);
	SPI_Init(GET_SPI_SET(SPI_FOSC_4,SPI_Mode_0,SPI_MSB,SPI_MSTR,SPI_IDIS));
	if(haveCard)
	{
		if(!MMCGetVolumeInfo())
		{
			haveCard=false;
		}
		else
		{
			LCDClear();
			uint8 mb=	ToStringWithU(stringbuff,mmc_info.Size);
			//stringbuff[mb++]='M';
			stringbuff[mb++]='B';stringbuff[mb++]=0;
			LCDShowStringAt(16,stringbuff);
			LCDShowStringAt(0,mmc_info.Name);
			ToStringWithU(stringbuff,mmc_info.SizeMB);
			LCDShowStringAt(8,stringbuff);
			uint16 i=0;
			for(;i<512;i++)
			{
				MMCReadSector(i,buf);
			}
			
		}
	}
	else
	{
		if(MMCInit())
		{
			haveCard = true;
		}
		else
		{
			haveCard=false;
			LCDShowStringAt(16,"NO CARD");
		}
	}
	/*if(UserEventExitCount<MaxUserEventCountDown)
	{
	UserEventExitCount++; 
	if(UserEventExitCount==MaxUserEventCountDown)
	{
	ExitUserEvent();
	}
	}*/
}
#endif//POLLING1000MS
static void iniPoll()
{
	SPI_Init(GET_SPI_SET(SPI_FOSC_4,SPI_Mode_0,SPI_MSB,SPI_MSTR,SPI_IDIS));
while(!MMCInit());
	showTime();
	LCDShowStringAt(16,"HELLO");
	SPIFlashSetManufacturer(SFM_Eon);
	SPIFlashInit();

	SPIFlashCleanWriteProtect();
	SPIFlashErasureAll();
	//do
	//{
	SPIFlashRead(UsartBuffer,0,0);

	SPIFlashPageProgram(UsartBuffer,0);
	SPIFlashAddress+=256;
	//}while(1);
	////UserEventExitCount =  MaxUserEventCountDown;
	////ToStringWithXFW(dislinebuf,18);
	////ShowString(dislinebuf,2,4);
	//ShowString("HELLO",9,7);
	////UpdateDisplay();
	//StandardL	=0;


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
	uint8 oldc60ms		= 0;
	uint8 oldc10ms		= 0;
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
		//poll SysClickMS
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


