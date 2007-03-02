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
 * $Id: lcd.h,v 1.1.2.1 2005/12/28 22:35:08 joerg_wunsch Exp $
 */
#ifndef _LCD_H_
#define _LCD_H_
#include "hd44780.h"
#ifndef LCD_ONE_LINE_CHAR
#define LCD_ONE_LINE_CHAR	16
#endif//LCD_ONE_LINE_CHAR

#ifndef LCD_LINE_COUNT
#define LCD_LINE_COUNT	2
#endif//LCD_LINE_COUNT

/*
 * Initialize LCD controller.  Performs a software reset.
 */
void	lcd_init(void);

/*
 * Send one character to the LCD.
 */
void	lcd_putchar(char c);

void lcd_ShowString(char *s);

/***************************
移动插入点到指定位置，如果指定位置不能显示，插入点将归0
lp：行
cp：列
****************************/
void LcdMovePointer(uint8 lp,uint8 cp);
void LCDShowStringAt(uint8 lc,char *s);
/*
* Clear the display.
*/
#define lcd_Clear()	hd44780_outcmd(HD44780_CLR)

#endif//_LCD_H_
