#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
//#include <SM895xA.H>

#include <REGX52.H>
#include <intrins.h>
#include "TypeDefing.h"


#define CLOCK_M(c) (c*1000000)

#define _CPU_CLOCK_ CLOCK_M(12)

#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_/12)
#define F_CPU _CPU_INSTRUCTION_CLOCK_
//每周期微秒数
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//每微秒周期数
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)

#define EEPROM_CHECK_DATA 1 //定义进行数据完整性效验

#define M62446_WORK_MODE_U//定义使用单独声道禁音
#define SAVE_M62446_SETTING//定义得以保存M62446设置到EEPROM


#define M62446_CH_MAP
#define _M62446MainVolume	M62446MainVolume	
#define _M62446VolumeR		M62446VolumeL		
#define _M62446VolumeL		M62446VolumeR		
#define _M62446VolumeSW		M62446VolumeCC		 
#define _M62446VolumeCC		M62446VolumeSW		 
#define _M62446VolumeSL		M62446VolumeSL		
#define _M62446VolumeSR		M62446VolumeSR
#define _M62446_WC_L		M62446_WC_R				  
#define _M62446_WC_R		M62446_WC_L					  
#define _M62446_WC_SL		M62446_WC_SL					  
#define _M62446_WC_SR		M62446_WC_SR					  
#define _M62446_WC_CC		M62446_WC_SW					  
#define _M62446_WC_SW		M62446_WC_CC


#define DIS_LINEBUF_SIZE		4//字符操作缓存大小
#define	MaxUserEventCountDown 5//用户事件发生后的最大倒计时（单位时间s）指定时间后程序将进入常规流程
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//PIN PIN
#define SDA_VSPI_PIN		(P1^1)
#define SCK_VSPI_PIN		(P1^0)

#define LATCH_M62446_PIN	(P2^2)

#define STROBE_MC14094B_PIN (P1^2)

#define SDA_I2C_PIN			(P0^6)
#define SCL_I2C_PIN			(P0^7)


#define CODINGSWITCH_A_PIN (P3^6)
#define CODINGSWITCH_B_PIN (P3^7)

#define C51_KEY_PIN_1	(P0^0)
#define C51_KEY_PIN_2	(P0^1)
#define C51_KEY_PIN_3	(P0^2)
#define C51_KEY_PIN_4	(P0^3)


#include "EepromSaveLocal.h"
#endif//_CONFIGURE_H_
