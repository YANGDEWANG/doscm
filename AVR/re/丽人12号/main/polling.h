#ifndef _POLLING_H_
#define _POLLING_H_
/*
* polling，查询处理系统事务
* 处理器类型：C51
* 版本：1
* 日期：2006-12-3
* 作者：杨德王<yangdewang@gmail.com>

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.  Or, point your browser to
* http://www.gnu.org/copyleft/gpl.html
*/
#include "configure.h"
#include "../ui/ir_command.h"

#define CLICK_CYCLE_MS 10
extern uint8 SysClickMS;//没CLICK_CYCLE_MS毫秒加一但这样要求你不阻塞
extern uint8 UserEventExitCount;
extern uint8 WorkState;
void InUserEvent();// (UserEventExitCount=0)

void PollingMain();
void ShowState();
extern char stringbuff[STRING_BUFF_SIZE];
extern uint8 bbtime;
extern bool ebb;
#define bbONTime(time) (bbtime = time)//单位：10ms
#define bbstd()  bbONTime(7)
#define bbwarn() bbONTime(60)
#define bberr()  bbtime=8;ebb=true


enum WORK_STATE
{
	WS_STANDARD,
	WS_IR_RecordNameInput,
	WS_IR_RECORD_RECORD_PICK,
	WS_IR_COMP_RECORD_PICK,
	WS_IR_RECORD,
	WS_IR_COMP,
	WS_IR_SHOW_DETAILED,
	
};
enum ERRORCODE
{
	EC_OK,
	EC_RecordCheck,
	//EC_ErrorStringEnd,
	EC_WaitIRPinToHighOvertime,
	EC_CustomCodeComp,
	EC_NOSUPPORTIC,
};
//void IntoSearchSound();
void iniPoll();
void NextIntput(bool show);
extern uint8 ErrorCode;
extern bool HaveMin;//咪头
extern bool ShowPingPu;
enum PING_PU
{
	PP_PP,
	PP_PP3,
	PP_PP_F,
	PP_PP_T,
	PP_PP_W,
	//PP_BOXING,
	PP_OFF,
};
#define PUType ((u8)EepromBuffer[ESL_PingPu])
static void inline InUserEventONAutoExit() 
{
	UserEventExitCount = MaxUserEventCountDown;
	ShowPingPu = false;
	noInUserEvent = true;
}
#endif//_POLLING_H_
