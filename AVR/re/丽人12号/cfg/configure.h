#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <avr/io.h>

#include "TypeDefing.h"
//自制m8调试器的引脚射影
//m8	m16
//PB2	PB4->SPI
//PB3	PB5->SPI
//PB4	PB6->SPI
//PB5	PB7->SPI
//PC5	PC0->TWI_SCL
//PC4	PC1->TWI_SDA
//PC3	PA3->ADC
//PC2	PA2->ADC
//PC1	PA1->ADC
//PC0	PA0->ADC


#define STRING_BUFF_SIZE 16

//------------------CLICK配置------------------//
#define CLICK_CFG
//注意这里的CLICK_CYCLE_US，CLOCKCS需要根据cpu时钟自行计算
#define CLICK_CYCLE_US 128//系统Click时间(us)
#define CLOCKCS	0b101
//----------------CLICK配置结束----------------//

#define _CPU_INSTRUCTION_CLOCK_ (F_CPU)

//每周期微秒数
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//每微秒周期数
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)

//------------dwstd 配置----------------//
#define DWSTD_CFG
//定义为生成的字符串添加结束符
#define DWSTD_ADDEND
//定义使用固定宽度tostring时不足位的填充字符默认填'0'
#define DWSTD_TOSTRING_ADDCHAR '0'
//定义DWSTD使用的类型
#define dwstdint uint8
#define dwstdsint int8
//------------dwstd 配置结束----------------//
#define	MaxUserEventCountDown 5//用户事件发生后的最大倒计时（单位时间s）指定时间后程序将进入常规流程
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//-------------------------I2C配置-------------------------------//
#define I2C_CFG
#define I2C_SHARE_SDA_PIN	//定义使SDA引脚可以共享
#define I2C_ENABLE_PULL		//定义使能内部上拉电阻
#define F_I2CSCL	100000	//TWI时钟为Hz
//pin def
#ifdef __AVR_ATmega16__
#define I2C_POPT	C
#define	I2C_SDA		1
#define	I2C_SCK		0
#elif defined __AVR_ATmega8__
#define I2C_POPT	C
#define	I2C_SDA		4
#define	I2C_SCK		5
#endif
//-----------------------I2C配置结束-----------------------------//

//--------------------------SPI口配置-------------------------------//
#define SPI_CFG
#define SPI_MASTER_U_U	1	//使用SPI口主机机模式
//pin def
#ifdef __AVR_ATmega16__
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
#elif defined __AVR_ATmega8__
#define SPI_POPT	B
#define	MISO_SPI	4
#define	MOSI_SPI	3
#define	SS_SPI		2
#define	SCK_SPI		5
#endif

//--------------------------SPI口配置_END----------------------------//
#define MainVolume (EepromBuffer[ESL_MainVolume])
#define MAINVOLUME_U		//定义使用主音量控制
#define MAINVOLUME_MAX 62	//主音量的最大值他应该小于各个声道控制的最大音量值

//-------------------PT2314配置---------------------//
#define PT2314_CFG
//选择一种IC
#define IC_PT2314
//#define IC_PT2315
#define PT2314_EXCHANGE_LR//定义交换左右声道
//定义得以保存PT2314设置到EEPROM
#define SAVE_PT2314_SETTING
//-----------------PT2314配置结束-------------------//
//-------------------PT2314_2配置---------------------//
#define PT2314_2_CFG
//选择一种IC
//#define IC_PT2314_2
#define IC_PT2315
#define PT2314_2_EXCHANGE_LR	//定义交换左右声道
//#define PT2314_2_DONT_ATTL		//定义控制平衡时不控制L声道
//定义得以保存PT2314_2设置到EEPROM
#define SAVE_PT2314_2_SETTING
//-----------------PT2314_2配置结束-------------------//
////-------------------M62429P配置---------------------//
//#define M62429P_CFG
////要禁音吧音量最高位设1
////注意ESL_VolumeSL在EepromBuffer中必须为M62429P数据的开始
//#define M62429P_PORT	D
//#define M62429P_DATP	3
//#define M62429P_SCKPA	{4,5}//定义多个M62429P的sck引脚用数组初始化语法
////定义得以保存M62429P设置到EEPROM
//#define SAVE_M62429P_SETTING
////-----------------M62429P配置结束-------------------//
////---------------------------STF16360E-CFG-------------------------------//
//#define STF16360EN_CFG
////pin
//#define STF16360EN_PORT B
//#define STF16360EN_CLKP 5
//#define STF16360EN_DATP 3
//#define STF16360EN_ENP  2
////-------------------------STF16360E-CFG-ENN-----------------------------//
#if 0
//-----------------POINTVFDDISPLAY配置---------------------//
#define POINTVFDDISPLAY_CFG
#define POINTVFDDISPLAY_DISMEM_SIZE	63
#define POINTVFDDISPLAY_X_P	20
//#define POINTVFDDISPLAY_X_BYTE	3
#define POINTVFDDISPLAY_Y_P	7
#define POINTVFDDISPLAY_X_C	(POINTVFDDISPLAY_X_P/8)	//水平方向可用数字显示位
#ifdef __AVR_ATmega16__
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	A
#define POINTVFDDISPLAY_KEY1_P		6
#define POINTVFDDISPLAY_KEY2_P		0
#else
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	C
#define POINTVFDDISPLAY_KEY1_P		4
#define POINTVFDDISPLAY_KEY2_P		0
#endif
typedef int8 IndexScreenLine;
//--------------POINTVFDDISPLAY配置结束--------------------//
#else
//-----------------POINTVFDDISPLAY配置---------------------//
#define POINTVFDDISPLAY_CFG
#define CHARIMAGE_W 8//字符宽度
#define POINTVFDDISPLAY_DISMEM_SIZE	63
#define POINTVFDDISPLAY_X_P	72
//#define POINTVFDDISPLAY_X_BYTE	3
#define POINTVFDDISPLAY_Y_P	7
#define POINTVFDDISPLAY_X_C	(POINTVFDDISPLAY_X_P/8)	//水平方向可用数字显示位
#ifdef __AVR_ATmega16__
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	A
#define POINTVFDDISPLAY_KEY1_P		6
#define POINTVFDDISPLAY_KEY2_P		0
#else
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	C
#define POINTVFDDISPLAY_KEY1_P		4
#define POINTVFDDISPLAY_KEY2_P		0
#endif
typedef int8 IndexScreenLine;
//--------------POINTVFDDISPLAY配置结束--------------------//

#endif
//-------------------M62429P配置---------------------//
#define M62429P_CFG
//要禁音吧音量最高位设1
//注意ESL_VolumeSL在EepromBuffer中必须为M62429P数据的开始

#ifdef __AVR_ATmega16__
#define M62429P_PORT		C
#define M62429P_DATP		1
#define M62429P_SCK1PORT	A
#else
#define M62429P_PORT		C
#define M62429P_DATP		4
#define M62429P_SCK1PORT	C
#endif
#define M62429P_SCK1P		2
#define M62429P_SCK2PORT	D
#define M62429P_SCK2P		3
//定义得以保存M62429P设置到EEPROM
#define SAVE_M62429P_SETTING
#if !defined(SAVE_M62429P_SETTING)
extern uint8 M62429PVolumeA[];
#else//SAVE_M62429P_SETTING
#define M62429PVolumeA		(EepromBuffer+ESL_VolumeSL)
#endif//SAVE_M62429P_SETTING
//-----------------M62429P配置结束-------------------//
//---------------------FFT 配置----------------------//
#define FFT_CFG
#define NUM_FFT 64 // Length of FFT to process
// Must be 2^N, where N is an integer >= 2
#define WINDOW_TYPE 0 // WINDOW_TYPE specifies the window to use on the data
// The available window functions are:
// 0 = No Window
// 1 = Triangle Window
// 2 = Hanning Window
// 3 = Hamming Window
// 4 = Blackman Window
//-------------------FFT 配置结束--------------------//
//---------------------ADC配置--------------------------//
#define ADC_CFG
#define ADC_MUXCFG			(ADC_REFS_AVCC|_BV(ADLAR))
#define ADC_SAMPLE_COUNT	64
#define ADC_PS				ADC_PS_32
//--------------------ADC配置完-------------------------//
#define INTPUT_DVD 0
#define INTPUT_AUX 1
#define INTPUT_VCD 2
#define INTPUT_AC3 3
#define INTPUT_MIN 1
#define INTPUT_MAX 3
//-----------------IO配置结束------------------------//
#define off5_1()	(PORTB&=~(1<<0))
#define on5_1()		(PORTB|=(1<<0))
#define offfan()	(PORTB&=~(1<<1))
#define onfan()		(PORTB|=(1<<1))
#define offSound()	(PORTD|=(1<<4))
#define onSound()	(PORTD&=~(1<<4))
#define checkSound() (!(PORTD&(1<<4)))
#define offjj()	(PORTD|=(1<<7))
#define onjj()	(PORTD&=~(1<<7))
#ifdef __AVR_ATmega16__
#define INOK	((PINA&(1<<3)))
#elif defined __AVR_ATmega8__
#define INOK	((PINC&(1<<3)))
#endif
#endif//_CONFIGURE_H_
