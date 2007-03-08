#ifndef _KEY_COMMAND_H_
#define _KEY_COMMAND_H_
#include "configure.h"

#define KEY_REPEAT_TIME 5//按键重复时间=KEY_REPEAT_TIME*60ms
//#define KEY_TEST//测试按键
extern void PollingKey60ms();
enum KEYC//key
{
	KEYC_MENU=1,//高低音，平衡，各声道音量
	KEYC_CH_DOME,//2。1    3。1    5。1
	KEYC_INPUT,
	KEYC_SUB,
	KEYC_M_VOL_M_TONE,//咪的高低音，音量
	KEYC_ADD,
	KEYC_DEL_ECHO,//有4种延时状态//延时/混响，再用调节 
	KEYC_DISPLAY,//频谱模
};
#endif//_KEY_COMMAND_H_
