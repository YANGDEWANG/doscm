#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Global.h"
#include "lcd.h"
#include "polling.h"
#include "USART.h"
#include "SPI.h"
#include "card/mmc/MMC.h"
//#include "displayAudio.h"
#include "../ui/Display.h"
#include "Clock.h"
//#include "UserEventManage.h"
//#include "eeprom_ex.h"
#include <avr/wdt.h> 
//#include "ISP.h"
void IniDev()
{
	/////////////////初始IO///////////////////////////
	

	/////////////////初始化设备///////////////////////////

	
	//EA = 1;
	sei();
	//VSPI2_MasterInit();
	IniDisplay();
	////ShowMesStr(MS_HELLO);
	//IniIrdev();
	//InitUSART(3);
	lcd_init();
	//InitUSART(12);//38.4
	ControlClock(true);
	//lcd_init();
	//while(1)
	/*uint8 dat[512];
	uint16 i;
	for(i = 0;i<sizeof(dat);i++)dat[i]=i+1;
	MMC_write_sector(1024,dat);*/

	
}

int main()//                         测试程序
{ 
	//_delay_ms(40);//开机延时
	//_delay_ms(40);//开机延时
	_delay_ms(40);//开机延时
	_delay_ms(40);//开机延时
	IniDev();//初始化设备
	/////////////////////主循环//////////////////////
	while(1)
	{

		PollingMain();
	}
	return 0;
}
