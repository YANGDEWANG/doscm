#include <configure.h>

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 

#ifndef CLICK_CFG
//------------------CLICK配置------------------//
#define CLICK_CFG
//注意这里的CLICK_CYCLE_US，CLOCKCS需要根据cpu时钟自行计算
#define CLICK_CYCLE_US 128//系统Click时间(us)
#define CLOCKCS	0b101
//----------------CLICK配置结束----------------//
#endif//CLICK_CFG

#define SysClick TCNT0
void ControlClock(bool S_P);//初始化Clock: true = start;false = stop
#endif//_CLOCK_H_
