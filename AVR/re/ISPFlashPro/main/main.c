#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Global.h"
#include "lcd.h"
#include "polling.h"
#include "USART.h"
//#include "SPI.h"
//#include "displayAudio.h"
#include "../ui/Display.h"
#include "Clock.h"
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
	InitUSART(12);//38.4
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
