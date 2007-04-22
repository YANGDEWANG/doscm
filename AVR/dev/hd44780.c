/*
* 更新时间：2006-12-21
*/
#include <global.h>
#include <util\delay.h>
#include <Clock.h>


#include <dev/hd44780.h>

//#define GLUE(a, b)     a##b
//
//#define GPORT(x)       GLUE(PORT, x)
//#define GDDR(x)        GLUE(DDR, x)
//#define GPIN(x)        GLUE(PIN, x)

//DATA P
#define HD44780_DATA_DDR	GDDR(HD44780_DATA_P)
#define HD44780_DATA_PIN	GPIN(HD44780_DATA_P)
#define HD44780_DATA_PORT	GPORT(HD44780_DATA_P)

//COM P
#define HD44780_COM_DDR		GDDR(HD44780_COM_P)
#define HD44780_COM_PIN		GPIN(HD44780_COM_P)
#define HD44780_COM_PORT	GPORT(HD44780_COM_P)


#define set_E()		(HD44780_COM_PORT|=(uint8)(1<<HD44780_E))
#define cl_E()		(HD44780_COM_PORT&=(uint8)~(1<<HD44780_E))
#define set_RW()	(HD44780_COM_PORT|=(uint8)(1<<HD44780_RW))
#define cl_RW()		(HD44780_COM_PORT&=(uint8)~(1<<HD44780_RW))
#define set_RS()	(HD44780_COM_PORT|=(uint8)(1<<HD44780_RS))
#define cl_RS()		(HD44780_COM_PORT&=(uint8)~(1<<HD44780_RS))

#define HD44780_BUSYFLAG 0x80

static uint8 hd44780_inbyte(void)
{
	uint8 x;
	HD44780_DATA_DDR = 0;
	set_RW();
	///Tas:40ns
	set_E();
	///Tddr:160ns
	///PWeh:230us

#if F_CPU > 10000000UL
	_delay_us(1);		/* guarantee 500 ns high */
#elif F_CPU > 5000000UL
  __asm__ volatile("nop");
  __asm__ volatile("nop");
  #endif
	x = HD44780_DATA_PIN;
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
	//loop_until_bit_is_clear(hd44780_inbyte(),7);
	while ((uint8)(hd44780_inbyte()&HD44780_BUSYFLAG)!=0)
	//while (HD44780_BUSYFLAG&hd44780_inbyte())
	{
		//if(SysClick-oldt>(5/CLICK_CYCLE_MS))return;
	}
}

/*
* Send one byte to the LCD controller.  As we are in 4-bit mode, we
* have to send two nibbles.
*/
static void hd44780_outbyte(uint8 b)
{


	cl_RW();
	///Tas:40ns
	HD44780_DATA_DDR = ~b;
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
//HD44780_COM_DDR = 0;
//HD44780_COM_PORT = 0;
	HD44780_COM_PORT&=~(1<<HD44780_RS)|(1<<HD44780_RW)|(1<<HD44780_E);
	HD44780_COM_DDR |=(1<<HD44780_RS)|(1<<HD44780_RW)|(1<<HD44780_E);
	
//	cl_RS();
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	_delay_ms(5);
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	//hd44780_outcmd(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	_delay_ms(1);
	hd44780_outbyte(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
	//hd44780_outcmd(HD44780_FNSET(1, HD44780_TWOLINE, HD44780_FONT5X10));
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
