/*
 */
#ifndef _HD44780_H_
#define _HD44780_H_
#include <configure.h>

//mode
#ifndef HD44780_MODE_SELECT

#define HD44780_TWOLINE		1
#define HD44780_FONT5X10	1
#endif//HD44780_MODE_SELECT

#ifndef HD44780_PIN
#define HD44780_PORT 1
#define HD44780_DATA_PORT 2
#define HD44780_RS 6
#define HD44780_RW 4
#define HD44780_E  5
#endif

#ifndef HD44780_PIN
#define HD44780_PORT 1
#define HD44780_RS 6
#define HD44780_RW 4
#define HD44780_E  5
#define HD44780_D4 0
#define HD44780_D5 1
#define HD44780_D6 2
#define HD44780_D7 3
#endif

//void	hd44780_outbyte(uint8 b);
//uint8	hd44780_inbyte(void);
void	hd44780_init(void);
void	hd44780_outcmd(uint8 n);	
void	hd44780_outdata(uint8 n);
#define hd44780_shift_display(right)	hd44780_outcmd(HD44780_SHIFT(1, right))
#define hd44780_cursor_display(right)	hd44780_outcmd(HD44780_SHIFT(0, right))


/* Clear LCD display command. */
#define HD44780_CLR \
	0x01

/* Home cursor command. */
#define HD44780_HOME \
	0x02

/*
 * Select the entry mode.  inc determines whether the address counter
 * auto-increments, shift selects an automatic display shift.
 */
#define HD44780_ENTMODE(inc, shift) \
	(0x04 | ((inc)? 0x02: 0) | ((shift)? 1: 0))

/*
 * Selects disp[lay] on/off, cursor on/off, cursor blink[ing]
 * on/off.
 */
#define HD44780_DISPCTL(disp, cursor, blink) \
	(0x08 | ((disp)? 0x04: 0) | ((cursor)? 0x02: 0) | ((blink)? 1: 0))

/*
 * With shift = 1, shift display right or left.
 * With shift = 0, move cursor right or left.
 */
#define HD44780_SHIFT(shift, right) \
	(0x10 | ((shift)? 0x08: 0) | ((right)? 0x04: 0))

/*
 * Function set.  if8bit selects an 8-bit data path, twoline arranges
 * for a two-line display, font5x10 selects the 5x10 dot font (5x8
 * dots if clear).
 */
#define HD44780_FNSET(if8bit, twoline, font5x10) \
	(0x20 | ((if8bit)? 0x10: 0) | ((twoline)? 0x08: 0) | \
		((font5x10)? 0x04: 0))

/*
 * Set the next character generator address to addr.
 */
#define HD44780_CGADDR(addr) \
	(0x40 | ((addr) & 0x3f))

/*
 * Set the next display address to addr.
 */
#define HD44780_DDADDR(addr) \
	(0x80 | ((addr) & 0x7f))
#endif//_HD44780_H_


