#ifndef _KEY_COMMAND_H_
#define _KEY_COMMAND_H_
#include "configure.h"

#define KEY_REPEAT_TIME 5//按键重复时间=KEY_REPEAT_TIME*60ms
//#define KEY_TEST//测试按键
void PollingKey60ms();
void ShowState();

enum KEYC//key
{
	KEYC_STANDARD=0x20,
	KEYC_COMP=0x10,
	KEYC_UP=0x9f,
	KEYC_DOWN=0x8f,
	KEYC_RECORD=0x8,
	KEYC_MENU=0x87,
};


#endif//_KEY_COMMAND_H_
