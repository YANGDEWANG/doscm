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
#include <avr/pgmspace.h>
#include "hd44780.h"
#ifndef LCD_CFG
//---------------LCD配置------------------//
#define LCD_CFG

#define LCD_ONE_LINE_CHAR	16
#define LCD_LINE_COUNT	2
//--------------LCD配置完-----------------//
#endif//LCD_CFG
/*
 * Initialize LCD controller.  Performs a software reset.
 */
void	lcd_init(void);

/*
 * Send one character to the LCD.
 */
void lcd_putchar(char c);

void LCDShowString(char const *s);
void LCDShowString_P(prog_char const *s);
/******************************************************
移动插入点到指定位置，如果指定位置不能显示，插入点将归0
lp：行
cp：列
******************************************************/
void LcdMovePointer(uint8 lp,uint8 cp);
/******************************************************
从LCD指定位置开始显示字符串
lc：起始位置
s ：要显示字符串
******************************************************/
void LCDShowStringAt(uint8 lc,char *s);
/******************************************************
从LCD指定位置开始显示字符串
lc：起始位置
s ：要显示字符串
******************************************************/
void LCDShowString_PAt(uint8 lc,prog_char const *s);

void LCDShowStringRTL_At(uint8 lc,char const *s);
/*
* Clear the display.
*/
#define LCDClear()	hd44780_outcmd(HD44780_CLR)

void LCDClearAt(u8 lc,u8 len);

#endif//_LCD_H_
