#ifndef _M62446_H_
#define _M62446_H_
/*
 * M62446，6通道音调控制器
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

#include <configure.h>
//#include "system\eeprom_ex.h"

//定义得以保存M62446设置到EEPROM
//#define SAVE_M62446_SETTING
//定义使用单独声道禁音
//#define M62446_WORK_MODE_U

#define M62446MAX_VOLUME			80
#define M62446MAX_MAIN_VOLUME		60
#define M62446MIN_MAIN_VOLUME		10
#define M62446MAX_ADJUST_VOLUME		(9)
#define M62446MIN_ADJUST_VOLUME		(-9)
#define M62446MAX_TONE				5
#define M62446MIN_TONE				(-5)  

#define M62446MAX_MUTEUTILITY		M62446MAX_VOLUME
#define M62446MIN_MUTEUTILITY		0


enum M62446_CONTROL
{
	M62446TONE_OUTPUT = 0,
	M62446VOLUMELR,
	M62446VOLUMECCSW,
	M62446VOLUMESLSR,

	M62446CONTROLMAX,
};
#ifdef M62446_WORK_MODE_U
enum M62446_WORK_CHUNNEL
{
	M62446_WC_L,
	M62446_WC_R,
	M62446_WC_SL,
	M62446_WC_SR,
	M62446_WC_CC,
	M62446_WC_SW,
};
enum M62446_WORK_MODE
{
	M62446_WM_6CH = (1<<M62446_WC_L)|(1<<M62446_WC_R)|(1<<M62446_WC_SL)|(1<<M62446_WC_SR)|(1<<M62446_WC_CC)|(1<<M62446_WC_SW),
	M62446_WM_3CH = (1<<M62446_WC_L)|(1<<M62446_WC_R)|(1<<M62446_WC_SW),
	M62446_WM_2CH = (1<<M62446_WC_L)|(1<<M62446_WC_R),
};
#endif//M62446_WORK_MODE_U
//PIN
#ifndef LATCH_M62446_PIN
#define LATCH_M62446_PIN	(P1^2)
#endif

//定义数据接口引脚
sbit LATCH_M62446 = LATCH_M62446_PIN;
#if !defined(SAVE_M62446_SETTING)
extern uint8 M62446MainVolume;
extern uint8 M62446VolumeL;
extern uint8 M62446VolumeR;
extern uint8 M62446VolumeCC;
extern uint8 M62446VolumeSW;
extern uint8 M62446VolumeSL;
extern uint8 M62446VolumeSR;
extern int8  M62446Bass;
extern int8  M62446Treble;
extern uint8 M62446OutputPort;
#ifdef M62446_WORK_MODE_U
extern uint8 M62446WorkMode;//其中对应M62446_WORK_CHUNNEL的位为1时声道禁音
#endif// M62446_WORK_MODE_U
#else
#define M62446MainVolume	((uint8)EepromBuffer[ESL_M62446MainVolume])
#define M62446VolumeL		((int8)EepromBuffer[ESL_M62446VolumeL])	
#define M62446VolumeR		((int8)EepromBuffer[ESL_M62446VolumeR])	 
#define M62446VolumeCC		((int8)EepromBuffer[ESL_M62446VolumeCC])	 
#define M62446VolumeSW		((int8)EepromBuffer[ESL_M62446VolumeSW])	 
#define M62446VolumeSL		((int8)EepromBuffer[ESL_M62446VolumeSL]) 
#define M62446VolumeSR		((int8)EepromBuffer[ESL_M62446VolumeSR]) 
#define M62446Bass			((int8)EepromBuffer[ESL_M62446Bass] )
#define M62446Treble		((int8)EepromBuffer[ESL_M62446Treble])
#define M62446OutputPort	((uint8)EepromBuffer[ESL_M62446OutputPort]) 
#ifdef M62446_WORK_MODE_U
#define M62446WorkMode		((uint8)EepromBuffer[ESL_M62446WorkMode])//其中对应M62446_WORK_CHUNNEL的位为1时声道禁音
#endif// M62446_WORK_MODE_U
#endif											
extern int8  M62446MuteUtility;//增大其值到M62446MAX_VOLUME可以禁音用来实现声音的渐变和禁音
extern bool  M62446Mute;//表示当前M62446的禁音状态更改它并不能禁音
//注意为了使用方便我把BYPASS选项储蓄到了M62446OutputPort里的第4位
#define M62446_BYPASS_MASK	(1<<4)
#define M62446ByPassSet()	(M62446OutputPort|=M62446_BYPASS_MASK)
#define M62446ByPassClear() (M62446OutputPort&=~M62446_BYPASS_MASK)
#define M62446ByPassGet()	((M62446OutputPort&M62446_BYPASS_MASK)>>4)

#define M62446OUTPUTPORT_MASK 0x0f
#define M62446OUTPUTPORT_1	(1<<3)
#define M62446OUTPUTPORT_2	(1<<2)
#define M62446OUTPUTPORT_3	(1<<1)
#define M62446OUTPUTPORT_4	(1<<0)
#define M62446OutputportSet(op) (EepromSaveChar(ESL_M62446OutputPort,M62446OutputPort&~M62446OUTPUTPORT_MASK|op))
#define M62446OutputportGet()	(M62446OutputPort&M62446OUTPUTPORT_MASK)

#define IniM62446 M62446UpdateAll
extern void M62446Update(uint8 flag);
extern void M62446UpdateAll();
extern void M62446ToMute();
extern void M62446ToSound();
//extern void M62446SetOutputPort(uint8 op);


#endif//_M62446_H_
