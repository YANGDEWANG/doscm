#include <configure.h>

#ifndef _CLOCK_H_ 
#define	_CLOCK_H_ 

#ifndef CLICK_CFG
//------------------CLICK����------------------//
#define CLICK_CFG
//ע�������CLICK_CYCLE_US��CLOCKCS��Ҫ����cpuʱ�����м���
#define CLICK_CYCLE_US 128//ϵͳClickʱ��(us)
#define CLOCKCS	0b101
//----------------CLICK���ý���----------------//
#endif//CLICK_CFG

#define SysClick TCNT0
void ControlClock(bool S_P);//��ʼ��Clock: true = start;false = stop
#endif//_CLOCK_H_
