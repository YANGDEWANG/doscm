#ifndef _KEY_COMMAND_H_
#define _KEY_COMMAND_H_
#include "configure.h"

#define KEY_REPEAT_TIME 5//按键重复时间=KEY_REPEAT_TIME*60ms
//#define KEY_TEST//测试按键
void PollingKey60ms();
void PollingIRkey();
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

typedef union _IR_RECORD_HEAD
{
	_uint8 dat[5];
	struct
	{
		_uint8 KeyCount:7;
		_uint8 MultiCustomCode:1;
		_uint8 ICName;
		_uint8 CustomCode;
		_uint8 CustomCodeReverse;
		_uint8 Check;
		//char CustomName[3];

	}RecordHead;
}IRRecordHead;

extern IRRecordHead RecordHead;
extern uint8 CurrentRecord;


#endif//_KEY_COMMAND_H_
