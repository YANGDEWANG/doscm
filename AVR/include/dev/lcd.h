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
//---------------LCD����------------------//
#define LCD_CFG

#define LCD_ONE_LINE_CHAR	16
#define LCD_LINE_COUNT	2
//--------------LCD������-----------------//
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
�ƶ�����㵽ָ��λ�ã����ָ��λ�ò�����ʾ������㽫��0
lp����
cp����
******************************************************/
void LcdMovePointer(uint8 lp,uint8 cp);
/******************************************************
��LCDָ��λ�ÿ�ʼ��ʾ�ַ���
lc����ʼλ��
s ��Ҫ��ʾ�ַ���
******************************************************/
void LCDShowStringAt(uint8 lc,char *s);
/******************************************************
��LCDָ��λ�ÿ�ʼ��ʾ�ַ���
lc����ʼλ��
s ��Ҫ��ʾ�ַ���
******************************************************/
void LCDShowString_PAt(uint8 lc,prog_char const *s);

void LCDShowStringRTL_At(uint8 lc,char const *s);
/*
* Clear the display.
*/
#define LCDClear()	hd44780_outcmd(HD44780_CLR)

void LCDClearAt(u8 lc,u8 len);

#endif//_LCD_H_
