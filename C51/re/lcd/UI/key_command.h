#ifndef _KEY_COMMAND_H_
#define _KEY_COMMAND_H_
#include "configure.h"

#define KEY_REPEAT_TIME 5//�����ظ�ʱ��=KEY_REPEAT_TIME*60ms
//#define KEY_TEST//���԰���
extern void PollingKey60ms();
enum KEYC//key
{
	KEYC_MENU=1,
	KEYC_OK_MODE,
	KEYC_ADD,
	KEYC_SUB,
	
};




#endif//_KEY_COMMAND_H_
