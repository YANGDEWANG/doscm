/*
* 更新时间：2006-12-21
*/

#include <delay.h>
#include <Clock.h>
#include <global.h>

#include "hd44780.h"

#define GLUE(a, b)     a##b

#define PORT(x)        GLUE(P, x)
#define HD44780_PORT_DATAOUT	PORT(HD44780_DATA_PORT)
#define HD44780_PORTOUT	PORT(HD44780_PORT)
#define HD44780_PORTIN	PORT(HD44780_PORT)
#define HD44780_DATABITS \
	(_BV(HD44780_D4)|_BV(HD44780_D5)|_BV(HD44780_D6)|_BV(HD44780_D7))

sbit hd44780RS = HD44780_PORTOUT^HD44780_RS;
sbit hd44780RW = HD44780_PORTOUT^HD44780_RW;
sbit hd44780E  = HD44780_PORTOUT^HD44780_E; 

#define set_E()		(hd44780E = true)
#define cl_E()		(hd44780E = false)
#define set_RW()	(hd44780RW = true)
#define cl_RW()		(hd44780RW = false)
#define set_RS()	(hd44780RS = true)
#define cl_RS()		(hd44780RS = false)

#define HD44780_BUSYFLAG 0x80

uint8 hd44780_inbyte(void)
{
	uint8 x;
	HD44780_PORT_DATAOUT = 0xff;
	set_RW();
	///Tas:40ns
	set_E();
	///Tddr:160ns
	///PWeh:230us

#if F_CPU > 10000000UL
	_delay_us(1);		/* guarantee 500 ns high */
#elif F_CPU > 5000000UL
	_nop_();
	_nop_();
#endif
	x = HD44780_PORT_DATAOUT;
	cl_E();
	return x;
}

/*
* Wait until the busy flag is cleared.
*/
void hd44780_wait_ready(void)
{
	//最多等待5ms
	//uint8 oldt = SysClick;
	while (hd44780_inbyte() & HD44780_BUSYFLAG)
	{
		//if(SysClick-oldt>(5/CLICK_CYCLE_MS))return;
	}
}

/*
* Send one byte to the LCD controller.  As we are in 4-bit mode, we
* have to send two nibbles.
*/
void hd44780_outbyte(uint8 b)
{


	cl_RW();
	///Tas:40ns
	HD44780_PORT_DATAOUT = b;
	set_E();
	///PWeh:230ns

#if F_CPU > 4000000UL
	_delay_us(1);		/* guarantee 500 ns high */
#elif F_CPU > 2000000UL
	_nop_();
#endif
	cl_E();

}



void hd44780_init(void)
{
	cl_RS();
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	_delay_ms(5);
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	_delay_ms(5);
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));

	hd44780_outcmd(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	hd44780_outcmd(HD44780_DISPCTL(0, 0, 0));
}

/* Send a command to the LCD controller. */
void hd44780_outcmd(uint8 b)
{
	cl_RS();
	hd44780_wait_ready();
	hd44780_outbyte(b);
}

/* Send a data byte to the LCD controller. */
void hd44780_outdata(uint8 b)
{
	cl_RS();
	hd44780_wait_ready();
	set_RS();
	hd44780_outbyte(b);
}
