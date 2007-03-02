#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <avr/io.h>

#include "TypeDefing.h"


#ifndef CLICK_CYCLE_US
#define CLICK_CYCLE_US 128//系统Click时间(us)
#endif
#define CLOCKCS	0b101


#define CLOCK_M(c) (c*1000000ul)

//#define _CPU_CLOCK_ 11059200
#define _CPU_CLOCK_ CLOCK_M(8)
#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_)

//每周期微秒数
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//每微秒周期数
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)

#define	MaxUserEventCountDown 15//用户事件发生后的最大倒计时（单位时间s）指定时间后程序将进入常规流程
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//
//------------dwstd 配置----------------//
#define DWSTD_CFG
//定义为生成的字符串添加结束符
#define DWSTD_ADDEND
//定义使用固定宽度tostring时不足位的填充字符默认填'0'
#define DWSTD_TOSTRING_ADDCHAR '0'
//定义DWSTD使用的类型
#define dwstdint uint16
#define dwstdsint int16
//------------dwstd 配置结束----------------//

//--------------------------SPI口配置-------------------------------//
#define SPI_CFG
#define SPI_MASTER_U	1	//使用SPI口主机机模式
//pin def
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7

//--------------------------SPI口配置_END----------------------------//

//------------------------------PT6312配置-------------------------//
#define PT6312_CFG
#define PT6312_DISPLAY_MODE	0x02						//设置为6区 16段
#define PT6312_MEMORY_SIZE	(PT6312_DISPLAY_MODE+4)*2	//被使用的储蓄空间大小
#define PT6312_LUM_CONTROL								//定义使用亮度控制
//#define	PT6312_WRLED									//定义使用LED口控制
//#define	PT6312_SWDATA									//定义使用读取SW数据
#define PT6312_LUM					6					//默认亮度
#define PT6312_KEY_MEMORY_SIZE		3					//按键储蓄空间大小
#define PT6312_KEY_SCAN_AMOUNT		1					//需同时处理的按键按下数目
#define PT6312_SPI_FOSC				SPI_FOSC_16			//选择SPI速度
//定义数据接口引脚
//pin def
#ifndef PT6312_POPT
#define PT6312_POPT	B
#define	PT6312_STB	4
#endif//SPI_POPT
//------------------------------PT6312配置-END----------------------//

#endif//_CONFIGURE_H_
