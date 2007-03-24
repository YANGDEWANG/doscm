#include <configure.h>

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 

#ifndef CLICK_CFG
//------------------CLICK配置------------------//
#define CLICK_CFG
//#define DATE_TIME_U //使用日期和时间
//注意这里的CLICK_CYCLE_US，CLOCKCS需要根据cpu时钟自行计算
#define CLICK_CYCLE_US 128//系统Click时间(us)
#define CLOCKCS	0b101
//----------------CLICK配置结束----------------//
#endif//CLICK_CFG

#define SysClick TCNT0
static inline void ControlClock(bool S_P)//初始化Clock: true = start;false = stop
{
	if(S_P)
	{
		//TIMSK |= (1<<TOIE0);
		TCCR0 = 0b111&CLOCKCS;//设置分频
	}
	else
	{
		TCCR0 = 0;//设置分频
	}

}
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
