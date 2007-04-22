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

#include <dev/hd44780.h>
#include <dev/lcd.h>
#include "string.h"
static uint8 charPointer;
static uint8 linePointer;
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

}
/***************************
移动插入点到指定位置，如果指定位置不能显示，插入点将归0
lp：行
cp：列
****************************/
void LcdMovePointer(uint8 lp,uint8 cp)
{
	linePointer = lp;
	charPointer = cp;
	if(lp>=LCD_LINE_COUNT||cp>=LCD_ONE_LINE_CHAR)
	{
		//hd44780_outcmd(HD44780_CLR);
		hd44780_outcmd(HD44780_HOME);
		//hd44780_outcmd(HD44780_DDADDR(0));
		linePointer = 0;
		charPointer = 0;
	}
	else
	{
		hd44780_outcmd(HD44780_DDADDR(0x40*lp+cp));
		/*if(lp==0)
		{
		hd44780_outcmd(HD44780_HOME);
		}
		else
		{

		}*/
	}
}
/*
* Send character c to the LCD display.  After a '\n' has been seen,
* the next character will first clear the display.
*/
void
lcd_putchar(char c)
{
#if 0
	static bool nl_seen;
#if (HD44780_TWOLINE==1)

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
#endif
	if (c == '\n')
	{
		LcdMovePointer(linePointer+1,0);
	}
	else
	{
		hd44780_outdata(c);
		charPointer++;
	}
	if(charPointer>=LCD_ONE_LINE_CHAR)
	{
		LcdMovePointer(linePointer+1,0);
	}


}
void LCDShowString(char const *s)
{
	while(*s)
	{
		lcd_putchar(*s);
		s++;
	}
}
void LCDShowStringRTL(char const *s,u8 slen)
{
	s+=slen-1;
	while(slen--)
	{
		lcd_putchar(*s--);
	}
}
void LCDShowStringRTL_At(uint8 lc,char const *s)
{
	u8 len = strlen(s);
	lc -=len-1;
	LcdMovePointer(lc/LCD_ONE_LINE_CHAR,lc%LCD_ONE_LINE_CHAR);
	LCDShowString(s);
}
void LCDShowString_P(prog_char const *s)
{
	char c;
	while(1)
	{
		c=pgm_read_byte(s);
		if(c==0)
			return;
		lcd_putchar(c);
		s++;
	}
}
void LCDShowStringAt(uint8 lc,char *s)
{
	LcdMovePointer(lc/LCD_ONE_LINE_CHAR,lc%LCD_ONE_LINE_CHAR);
	LCDShowString(s);
}
/******************************************************
从LCD指定位置开始显示字符串
lc：起始位置
s ：要显示字符串
******************************************************/
void LCDShowString_PAt(uint8 lc,prog_char const *s)
{
	LcdMovePointer(lc/LCD_ONE_LINE_CHAR,lc%LCD_ONE_LINE_CHAR);
	LCDShowString_P(s);
}
void LCDClearAt(u8 lc,u8 len)
{
	LcdMovePointer(lc/LCD_ONE_LINE_CHAR,lc%LCD_ONE_LINE_CHAR);
	while(len--)
	{
		lcd_putchar(' ');
	}
}

