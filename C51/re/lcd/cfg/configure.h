#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
//#include <REG2051.H>
//#include <REGX52.H>
#include <REG2051.H>

#include <intrins.h>
#include "TypeDefing.h"



#define CLOCK_M(c) (c*1000000)

#define _CPU_CLOCK_ 11059200
//#define _CPU_CLOCK_ CLOCK_M(66)
#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_/12)
#define F_CPU _CPU_INSTRUCTION_CLOCK_
//ÿ����΢����
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//ÿ΢��������
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)


#define DIS_LINEBUF_SIZE		4//�ַ����������С
#define	MaxUserEventCountDown 5//�û��¼����������󵹼�ʱ����λʱ��s��ָ��ʱ�����򽫽��볣������
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//PIN PIN
#define HD44780_IN_8BIT
#define HD44780_PIN
#define HD44780_PORT 3
#define HD44780_DATA_PORT 1
#define HD44780_RS 4
#define HD44780_RW 5
#define HD44780_E  3

#define HD44780_MODE_SELECT

#define HD44780_TWOLINE		1
#define HD44780_FONT5X10	0

#endif//_CONFIGURE_H_
