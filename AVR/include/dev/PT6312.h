#ifndef _PT6312_H_
#define _PT6312_H_
#include <configure.h>
#ifndef PT6312_CFG
//------------------------------PT6312配置-------------------------//

#define PT6312_CFG
#define PT6312_DISPLAY_MODE	0x02						//设置为6区 16段
#define PT6312_MEMORY_SIZE	(PT6312_DISPLAY_MODE+4)*2	//被使用的储蓄空间大小
#define PT6312_LUM_CONTROL								//定义使用亮度控制
#define	PT6312_WRLED									//定义使用LED口控制
#define	PT6312_SWDATA									//定义使用读取SW数据
#define PT6312_LUM					6					//默认亮度
#define PT6312_KEY_MEMORY_SIZE		3					//按键储蓄空间大小
#define PT6312_KEY_SCAN_AMOUNT		1					//需同时处理的按键按下数目
#define PT6312_SPI_FOSC				SPI_FOSC_16			//选择SPI速度
//定义数据接口引脚
#define PT6312_POPT	B
#define	PT6312_STB	4

//------------------------------PT6312配置-END----------------------//
#endif//PT6312_CFG

//------------------------------------------------------------------//
//扫描按键返
//回按下的键的码值指针最多返回的按键数由"PT6312_KEY_SCAN_AMOUNT"决定
//return 0 表示无按键按下
//------------------------------------------------------------------//
uint8* ScanKey();
//初始化6312；
void PT6312Init();				
//将显示数据dat写入显存
void PT6312UpdateAll(uint8* dat);

//------------------------PT6312_WRLED------------------------------//
#ifdef 	PT6312_WRLED
//写led口数据
void PT6312WriteLed(uint8);
#endif//PT6312_WRLED
//------------------------PT6312_SWDATA-----------------------------//
#ifdef 	PT6312_SWDATA
//读数据从SW口，低4bit有效
uint8 PT6312ReadSWData();
#endif//PT6312_SWDATA
#endif

