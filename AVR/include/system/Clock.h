#include <configure.h>

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 

#ifndef CLICK_CFG
//------------------CLICK����------------------//
#define CLICK_CFG
#define CLICK_16BIT_EN	1//ʹ��16bit��timer
//#define DATE_TIME_U //ʹ�����ں�ʱ��
//ע�������CLICK_CYCLE_US��CLOCKCS��Ҫ����cpuʱ�����м���
#define CLICK_CYCLE_US 128//ϵͳClickʱ��(us)
#define CLOCKCS	0b101
//----------------CLICK���ý���----------------//
#endif//CLICK_CFG

#if CLICK_16BIT_EN>0
#define SysClick GetSysClick()
static inline u16 GetSysClick()
{
	b16 b;
	b.b8_2.b8_0 = TCNT1L;
	b.b8_2.b8_1 = TCNT1H;
	return b.b16_1;
}
static inline void ControlClock(bool S_P)//��ʼ��Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR1B = 0b111&CLOCKCS;//���÷�Ƶ
	}
	else
	{
		TCCR1B = 0;//���÷�Ƶ
	}

}
#else//CLICK_16BIT_EN
#ifdef __AVR_ATmega324P__
#define SysClick TCNT0
static inline void ControlClock(bool S_P)//��ʼ��Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR0B = 0b111&CLOCKCS;//���÷�Ƶ
	}
	else
	{
		TCCR0B = 0;//���÷�Ƶ
	}

}

#else
#define SysClick TCNT0
static inline void ControlClock(bool S_P)//��ʼ��Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR0 = 0b111&CLOCKCS;//���÷�Ƶ
	}
	else
	{
		TCCR0 = 0;//���÷�Ƶ
	}

}
#endif
#endif//CLICK_16BIT_EN
#ifdef DATE_TIME_U
typedef struct DATE_TIME
{
	u16 Year;
	u8 Month;
	u8 Day;
	u8 Hour;
	u8 Minute;
	u8 Second;
}DateTime;

extern DateTime Now;
#else//DATE_TIME_U
#define Now_Year	2000
#define Now_Month	1
#define Now_Day		1
#define Now_Hour	0
#define Now_Minute	0
#define Now_Second	0
#endif//DATE_TIME_U
#endif//_CLOCK_H_
