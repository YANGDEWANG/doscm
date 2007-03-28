#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
//#include <REG2051.H>
#include <REGX52.H>
//#include <REG2051.H>

#include <intrins.h>
#include "TypeDefing.h"



#define CLOCK_M(c) (c*1000000)

//#define _CPU_CLOCK_ 11059200
#define _CPU_CLOCK_ CLOCK_M(12)
#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_/12)
#define F_CPU _CPU_INSTRUCTION_CLOCK_
//每周期微秒数
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//每微秒周期数
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)
#define I2C_V2//使用版本2
#define MULTI_AT24C02			//多个AT24C02，或使用24C04以上

#define VSPI2_LSB_U		1	//使用LSB
#define VSPI2_MODE_3 //起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) SCK常态1

#define DIS_LINEBUF_SIZE		10//字符操作缓存大小
#define	MaxUserEventCountDown 15//用户事件发生后的最大倒计时（单位时间s）指定时间后程序将进入常规流程
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//PIN PIN
//定义数据接口引脚tm1628
#define SDA_VSPI2 P0_2=P2_5=P3_5=P1_2
#define SCK_VSPI2 P0_3=P2_4=P3_4=P1_1
#define SDA_VSPI2_GET (P1_2&&P3_5)//P0_2&P2_5&

//定义数据接口引脚
#define SDA_I2C_PIN (P2^1)
#define SCL_I2C_PIN (P2^2)

#define RecordL		P1_5
#define StandardL	P1_4
#define CompL		P1_3


#endif//_CONFIGURE_H_
