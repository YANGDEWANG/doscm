/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
* ----------------------------------------------------------------------------
*
* Stdio demo, upper layer of LCD driver.
*
* $Id: lcd.c,v 1.1.2.1 2005/12/28 22:35:08 joerg_wunsch Exp $
*/
#include <global.h>

#include "hd44780.h"
#include "lcd.h"

/*
* Setup the LCD controller.  First, call the hardware initialization
* function, then adjust the display attributes we want.
*/
void
lcd_init(void)
{

	hd44780_init();

	/*
	* Clear the display.
	*/
	hd44780_outcmd(HD44780_CLR);
	

	/*
	* Entry mode: auto-increment address counter, no display shift in
	* effect.
	*/
	hd44780_outcmd(HD44780_ENTMODE(1, 0));
	
	/*
	* Enable display, activate non-blinking cursor.
	*/
	hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));
	
//hd44780_outcmd(HD44780_DDADDR(16));	//
}

/*
* Send character c to the LCD display.  After a '\n' has been seen,
* the next character will first clear the display.
*/
void
lcd_putchar(char c)
{
	static bool nl_seen;
#if (HD44780_TWOLINE==1)
	static bool in_line2;
	if (nl_seen && c != '\n')
	{ 
		if(in_line2)
		{
			/*
			* First character after newline, clear display and home cursor.
			*/
		
			hd44780_outcmd(HD44780_CLR);
			hd44780_outcmd(HD44780_HOME);
			hd44780_outcmd(HD44780_DDADDR(0));
		}
		else
		{
			hd44780_outcmd(HD44780_DDADDR(0x40));
		}
		 in_line2=!in_line2;
		nl_seen = false;
	}
#else
	if (nl_seen && c != '\n')
	{
		/*
		* First character after newline, clear display and home cursor.
		*/
		hd44780_outcmd(HD44780_CLR);
		hd44780_outcmd(HD44780_HOME);
		hd44780_outcmd(HD44780_DDADDR(0));

		nl_seen = false;
	}
#endif//HD44780_TWOLINE
	if (c == '\n')
	{
		nl_seen = true;
	}
	else
	{
		hd44780_outdata(c);
	}

}
void
lcd_ShowString(char *s)
{
	while(*s)
	{
		lcd_putchar(*s);
		s++;
	}
}

