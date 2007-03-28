#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Global.h"
#include "dev/lcd.h"
#include "polling.h"
#include "dev/USART.h"
#include "dev/SPI.h"
#include "dev/MMC.h"
//#include "displayAudio.h"
#include "../ui/Display.h"
#include "system/Clock.h"
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
	SPIFlashInit();
	ControlClock(true);
	//lcd_init();
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
