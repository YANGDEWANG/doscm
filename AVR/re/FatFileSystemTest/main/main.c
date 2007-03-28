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
	/////////////////��ʼIO///////////////////////////
	

	/////////////////��ʼ���豸///////////////////////////

	
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

int main()//                         ���Գ���
{ 
	//_delay_ms(40);//������ʱ
	//_delay_ms(40);//������ʱ
	_delay_ms(40);//������ʱ
	_delay_ms(40);//������ʱ
	IniDev();//��ʼ���豸
	/////////////////////��ѭ��//////////////////////
	while(1)
	{

		PollingMain();
	}
	return 0;
}
