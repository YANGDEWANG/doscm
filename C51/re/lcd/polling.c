#include <stdio.h>
#include <Global.h>
#include "Clock.h"
#include "polling.h"
#include "lcd.h"
#include "hd44780.h"
uint8 UserEventExitCount;


void ExitUserEvent()
{
}
void polling60ms()
{
}
void polling10ms()
{

}
prog_char *aa= "0123456789";
void polling200ms()
{
	//hd44780_wait_ready();
	//hd44780_shift_display(1);
}
void polling500ms()
{
 /*static bool t;
 if(t)
	lcd_ShowString(aa);
 else
	 lcd_ShowString(ab);
 t=!t;*/
}
void polling1000ms()
{
	static char a='A';
	lcd_putchar(a);
	a++;
}
static void iniPoll()
{
	UserEventExitCount =  MaxUserEventCountDown;
	
}
void PollingMain()
{

	uint8 sc;
	uint8 oldc60ms		= SysClick;
	uint8 oldc10ms		= oldc60ms;
	uint8 oldc200ms		= oldc60ms;
	uint8 oldc500ms		= oldc60ms;
	uint8 oldc1000ms	= oldc60ms;
	iniPoll();
	while(true)
	{	 
		//IniEeprom_ex();
		sc = SysClick;
		if(sc-oldc10ms>10/CLICK_CYCLE_MS)
		{
			polling10ms();
			oldc10ms+=10/CLICK_CYCLE_MS;
		}
		if(sc-oldc60ms>60/CLICK_CYCLE_MS)
		{
			polling60ms();
			oldc60ms+=60/CLICK_CYCLE_MS;
		}
		if(sc-oldc200ms>200/CLICK_CYCLE_MS)
		{
			polling200ms();
			oldc200ms+=200/CLICK_CYCLE_MS;
		}
		if(sc-oldc500ms>500/CLICK_CYCLE_MS)
		{
			polling500ms();
			oldc500ms+=500/CLICK_CYCLE_MS;
		}
		if(sc-oldc1000ms>1000/CLICK_CYCLE_MS)
		{
			polling1000ms();
			oldc1000ms+=1000/CLICK_CYCLE_MS;
		}
	}
}
