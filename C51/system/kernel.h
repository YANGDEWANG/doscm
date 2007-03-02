#ifndef _KERNEL_H
#define _KERNEL_H
#include "configure.h"
extern void _RRC_();//5指令周期
extern void	_RLC_();//5指令周期
extern void	Delay(uint8);//延时10+t*2个指令周期
//extern uint8 BCDtoUchar(uint8 in);//
#endif//_Kernel_H
