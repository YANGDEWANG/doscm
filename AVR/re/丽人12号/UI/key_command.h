#ifndef _KEY_COMMAND_H_
#define _KEY_COMMAND_H_
#include "configure.h"

#define KEY_REPEAT_TIME 5//�����ظ�ʱ��=KEY_REPEAT_TIME*60ms
//#define KEY_TEST//���԰���
extern void PollingKey60ms();
enum KEYC//key
{
	KEYC_MENU=1,//�ߵ�����ƽ�⣬����������
	KEYC_CH_DOME,//2��1    3��1    5��1
	KEYC_INPUT,
	KEYC_SUB,
	KEYC_M_VOL_M_TONE,//��ĸߵ���������
	KEYC_ADD,
	KEYC_DEL_ECHO,//��4����ʱ״̬//��ʱ/���죬���õ��� 
	KEYC_DISPLAY,//Ƶ��ģ
};
#endif//_KEY_COMMAND_H_
