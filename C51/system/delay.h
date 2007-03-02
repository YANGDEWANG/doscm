#ifndef _DELAY_H_
#define _DELAY_H_
#include "configure.h"

#ifdef __C51__
/*
延时循环

__delay_C：
延时循环计数器，需使用无符号类型
*根据提供变量的类型不同每个循环的指令周期不同
*如果为uint8 data 每次循环使用2个指令周期

__count：
延时循环计数
*当__count为0时为最大延迟（__delay_C类型最大值*指令周期时间），当然还要加上初始化__delay_C的时间

cc：
为了可以在同一个方法中多次使用此定义你必须每次使用时提供不同的cc值避免标号重复
注意：在循环内部每个循环的指令周期不能确定
注意：使用时如果__delay_C为uint8请使用下面的代码检查你提供的__count的可用性
#if (__count<1)||(us>255)
#error 使用的__count超过了允许的值。
#endif
*/
#define _delay_loop(__delay_C,__count,cc)	\
{											\
	__delay_C=__count;						\
	_delay_loopF##cc:						\
	__delay_C--;							\
	if(__delay_C!=0)goto  _delay_loopF##cc; \
}		


#define _DELAY_LOOP_IN_UINT8_IC_ 2//_delay_loop,__delay_C为uint8 data时的每循环指令周期

/*
延迟ms 毫秒
ms：
毫秒数,当ms为0时为最大延时（256ms）
*/
extern void _delay_ms(uint8 ms);
/*
延迟s 秒
s：
毫秒数,当s为0时为最大延时（256s）
*/
extern void _delay_s(uint8 s);
/*
延迟us 微秒
us：
微秒数
*/
extern void _delay_us(uint8 us);
#endif//__C51__
#endif//_DELAY_H_

