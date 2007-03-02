#ifndef _PT2314_2_H_
#define	_PT2314_2_H_ 
/*
* PT2314/2315，2通道音调控制器;只有你在一个系统使用了两个2314/2315时使用
* 处理器类型：C51,avr
* 版本：1
* 日期：2007-1-14
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
#include "..\system\eeprom_ex.h"
#ifndef PT2314_2_CFG
//-------------------PT2314_2配置---------------------//
#define PT2314_2_CFG
//选择一种IC
//#define IC_PT2314_2
#define IC_PT2315
#define PT2314_2_EXCHANGE_LR//定义交换左右声道
//定义得以保存PT2314_2设置到EEPROM
#define SAVE_PT2314_2_SETTING
//-----------------PT2314_2配置结束-------------------//

#endif//PT2314_2_CFG
#ifdef IC_PT2314_2
#define PT2314_2ADDRESS 	0x44
#elif defined(IC_PT2315)
#define PT2314_2ADDRESS 	0x40
#endif

#define PT2314_2_MAXVOLUME				63
#define PT2314_2_MINVOLUME				0

#define PT2314_2_MAXSPEAKERATTENUATORS	15
#define PT2314_2_MINSPEAKERATTENUATORS	(-15)

#define PT2314_2_MAXINPUTGAIN				3
#define PT2314_2_MININPUTGAIN				0

#define PT2314_2_MAXAUDIOCHUNNEL			4
#define PT2314_2_MINAUDIOCHUNNEL			0

#define PT2314_2_MAXTONE			7//中间值7为直通
#define PT2314_2_MINTONE			(-7)
/*
#define PT2314_2_AudioChannelsCount		4
#define PT2314_2_AM_FMChunnel		0
#define PT2314_2_CDChunnel		1
#define PT2314_2_TapeChunnel		2
#define PT2314_2_AuxChunnel		3

#define PT2314_2_DefaultVolume			30
#define PT2314_2_DefaultChunnel			AuxChunnel //first    0
#define PT2314_2_DefaultLoudness			1
#define PT2314_2_DefaultInputGain			3//0=+11.25dB;1= +7.5dB;2= +3.75dB;3= 0dB


enum PT2314_2_CONTROL
{
	PT2314_2_VOLUME		=	0x00,
	PT2314_2_SPEAKERATT	=	0xc0,
	PT2314_2_LOUDNESS		=	0x40,
	PT2314_2_TONE			=	0x60,
};
*/

#if !defined(SAVE_PT2314_2_SETTING)
extern uint8 PT2314_2Volume;
extern uint8 PT2314_2Bass;
extern uint8 PT2314_2Treble;
extern int8  PT2314_2SpeakerATT;
extern uint8 PT2314_2Loudness;
#else//SAVE_PT2314_2_SETTING
#define PT2314_2Volume		(uint8)EepromBuffer[ESL_PT2314_2Volume]
#define PT2314_2Bass		(uint8)EepromBuffer[ESL_PT2314_2Bass]
#define PT2314_2Treble		(uint8)EepromBuffer[ESL_PT2314_2Treble]
#define PT2314_2SpeakerATT	(int8) EepromBuffer[ESL_PT2314_2SpeakerATT]
#define PT2314_2Loudness	(uint8)EepromBuffer[ESL_PT2314_2Loudness]
#endif//SAVE_PT2314_2_SETTING

extern bool  PT2314_2Mute;	
//注意为了使用方便我把InputGain，PT2314_2TSwitch选项储蓄到了(uint8)EepromBuffer[PT2314_2Loudness]里
#define PT2314_2_SWITCH_MASK		(3)
#define PT2314_2_INPUTGAIN_MASK	(3<<3)
#define PT2314_2_LOUDNESS_MASK	(1<<2)
#define PT2314_2_LoudnessSet()	(PT2314_2Loudness|=PT2314_2_LOUDNESS_MASK)
#define PT2314_2_LoudnessClear()	(PT2314_2Loudness&=~PT2314_2_LOUDNESS_MASK)
#define PT2314_2_LoudnessGet()	(PT2314_2Loudness&PT2314_2_LOUDNESS_MASK)


extern void PT2314_2UpdateAll();
//extern void PT2314_2Update(uint8 flag);
#define IniPT2314_2() PT2314_2UpdateAll()

#endif//_PT2314_2_H_ 
