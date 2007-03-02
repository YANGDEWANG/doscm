#ifndef _LC72131_H_
#define _LC72131_H_
/*
* LC72131 PLL RM_AM/RM_FM收音控制器
* 处理器类型：C51,AVR
* 版本：1
* 日期：2006-12-14
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

//#define	UseSW
enum RADIO_MODE
{
	RM_AM,
	RM_SW,
	RM_FM,
};
typedef uint8 RadioMode;
#define DefaultRadioMode		RM_FM	//默认波段
#define MinFMFrequency			0		//实际频率 = MinFMFrequency*100k+FMFrequencyExcursion
#define MaxFMFrequency			210		//实际频率 = MinFMFrequency*100k+FMFrequencyExcursion
#define FMFrequencyExcursion	87		//RM_FM的频率偏移（MHZ）
#define MinAMFrequency			58		//实际频率 = MinAMFrequency*9k
#define MaxAMFrequency			190		//实际频率 = MaxAMFrequency*9k
#define MaxSWFrequency			0		//实际频率 = MaxSWFrequency*5k
#define SeekPointCount			8		//自动搜台时判断锁定后继续向后探测最佳频点的次数
#define FMLockFrequencyPatient	50	//允许中频误差RM_FM（Hz）//最大 255
#define AMLockFrequencyPatient	80	//允许中频误差RM_AM（Hz）//最大 255
//#define	LC72131RadioMode EepromBuffer[RadioModeStorage]
union LC72131IntputCom1
{
	_uint8 dat[3];
	struct
	{
		_uint16 P_CTR_L:8;
		_uint16 P_CTR_H:8;
		_uint8	AM_FM:	2;	//0=MW;1=RM_SW;2=RM_FM
		_uint8	CET: 	1; 	//IF counter measurement start data 1=Counter start,0=Counter reset
		_uint8	XS: 	1;	//Crystal resonator selection 0=4.5 MHz,1=7.2 MHz restart=1
		_uint8	R_CRT: 	4; 	//Reference frequency
	}LC72131IM1;
};

union LC72131IntputCom2
{
	_uint8 dat[3];
	struct
	{
		_uint8 IO_C1: 	1; 	//I/O port specification data
		_uint8 IO_C2: 	1;	//0 = input mode, 1 = output mode
		//Output port data:0 = open, 1 = low
		_uint8 IO1:	1;
		_uint8 IO2:	1;
		_uint8 BO1:	1;
		_uint8 BO2:	1;
		_uint8 BO3:	1;
		_uint8 BO4:	1;
		_uint8 DNC: 	1; 	//Don’t care. This data must be set to 0.
		_uint8 DO_C: 	3; 	//DO pin control data
		_uint8 UNLOCK:	2;	//Unlock detection data
		_uint8 DZ_C: 	2;	//Dead zone widths: DZA < DZB < DZC < DZD (0--3)
		_uint8 IF_CAT:	2;	//Determines the IF counter measurement period.
		_uint8 TBC: 	1;	//Clock time base
		_uint8 DLC: 	1;	//Charge pump control data
		_uint8 IFS: 	1;	//This data must be set 1 in normal mode.
		_uint8 TEST: 3;	//These values must all be set to 0.
	}LC72131IM2;
};

union LC72131OutputCom
{
	_uint8 dat[3];
	struct
	{	
		_uint16	IF_CTRL:	16;	//IF counter binary data
		_uint8	IF_CTRH:	4;
		_uint8	UL:			1;		//PLL unlock Latched from the state of the unlock detection circuit.
		_uint8	NC:			1;		//Mast be 0. 
		_uint8	IO1:		1;
		_uint8	IO2:		1;
	}LC72131OM;
};


#ifdef SAVE_RADIO_SETTING
#define LC72131RadioMode ((RadioMode)EepromBuffer[ESL_LC72131RadioMode])
#else
extern RadioMode	LC72131RadioMode; 
#endif//SAVE_RADIO_SETTING

extern uint8	LC72131CurrentFrequency;  	//当前LC72131的工作频率，实际频率因波段不同有不同的计算方法（如上）
extern uint8	LC72131SeekSatrtTime;//搜索开始时间
extern uint8	CurrentSeekPoint_Radio;			//当前缓存“中频偏差”frequencyTmp 的偏移量
extern uint8	LC72131CurrentFrequency;  				//当前LC72131的工作频率，实际频率因波段不同有不同的计算方法（如上）
extern bool LC72131SeekFlag;//启动自动搜台时被置一，搜索结束时被自动置零
extern bool LC72131SeekWait;//为一表示LC72131.C处于等的读取中频计数时间段


/*
*设置频率
*调用方法前请先设置“LC72131CurrentFrequency”。
*seek:	为true 将进行中频计数，等待40ms后调用“LC72131IsLock“判断是否有台。
*/
extern void		LC72131SeekFrequency(bool seek);
/*
*设置当前频率为下一点 
*注意：如果当前频率当前频率已到界限调用方法将无效果
*/
extern void LC72131NextFrequemcy();
/*
*仅在自动搜台时使用，并且应在LC72131SeekWait置true后40ms调用
*在LC72131SeekWait置true的时候我们保存了系统时间LC72131SeekSatrtTime便于你计时
*/
extern void LC72131Seek();
extern void SetRadioMode(RadioMode mode);//设置收音机波段
extern void LC72131Mute(bool of);		//禁音 of=false->请求中频输出加禁音
extern void	SetST_MO(bool ST_MO);		//打开与关闭立体声输出 ST_MO=true时立体声关闭
extern bool	GetST_MO();					//获取当前声音输出状态 return:false =立体声
extern void IniLC72131();				//初始化
extern uint8 LC72131IsLock();			//调用后测试 RadioLocked 判断是否有台 return：中频偏差绝对值（仅当调用后 RadioLocked == true 时有意义）

//extern ulong	GetCTR();
#ifdef __AVR__
//定义数据接口引脚
#define PIN_Readio 		PIND
#define DDR_Readio 		DDRD
#define PORT_Readio 	PORTD
#define ReadioSTB		7
#elif defined(__C51__)//end __AVR__
//PIN
#ifndef LC72131_STB_PIN
#define LC72131_STB_PIN P1^1
#endif
sbit LC72131_STB = LC72131_STB_PIN;
#endif//__C51__


#endif//_LC72131_H_
