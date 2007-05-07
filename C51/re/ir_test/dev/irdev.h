#ifndef _IRDEV_H_
#define _IRDEV_H_
#include "configure.h"

typedef struct _IRData
{
	_uint8 CustomCode;
	_uint8 CustomCodeReverse;
	_uint8 KeyCode;
	_uint8 KeyCodeReverse;
}IRData;
enum IC_NAME
{
	IC_NULL,
	IC_LC7461,
	IC_LC7464,
	IC_PT2222,
	IC_PT2268,
	IC_HS6222,
	IC_TC9012,
	IC_SAA3010,
	IC_8521,
	IC_HS3004,
	IC_HS9148,
	IC_UPD6124,//24+1
	IC_MN6014C5D6,//46+1
	IC_MN6014C6D6,
	IC_6014C8D7,
	IC_SHARP,
	IC_SC50462,
	IC_SC50560,
	IC_M50119,
	IC_LX5104,
	IC_MAX,
};
enum IR_DEV_STATE
{
	IR_DEV_IDLE,		//空闲
	IR_DEV_INCEPT,		//数据接收中
	IR_DEV_DATAANALYSE,	//数据分析中
};
#define IR_CHANGE_TIME_SIZE (99)//86
extern uint8 ICName;
extern IRData IrInformation;
extern bool  IRKeyDown,IRKeyHold;
extern prog_char * code ICNameS[];
extern uint8 IRChangec;//输入电平变化计数
extern uint8 perchdata IRChangeTime[IR_CHANGE_TIME_SIZE];
extern uint8 IRDevState;
void ShowIrInformation();
void IniIrdev();
void IrDevDiv();
void PollIR10ms();


#endif//_IRDEV_H_
