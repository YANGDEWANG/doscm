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
#define LCD_LINE_LENGTH 16
/*
 * Initialize LCD controller.  Performs a software reset.
 */
void	lcd_init(void);

/*
 * Send one character to the LCD.
 */
void	lcd_putchar(char c);

void
lcd_ShowString(char *s);
