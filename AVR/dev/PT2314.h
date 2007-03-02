#ifndef _PT2314_H_
#define	_PT2314_H_ 
/*
* PT2314/2315，2通道音调控制器
* 处理器类型：C51,avr
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
#include "..\system\eeprom_ex.h"
#ifndef PT2314_CFG
//-------------------PT2314配置---------------------//
#define PT2314_CFG
//选择一种IC
//#define IC_PT2314
#define IC_PT2315
#define PT2314_EXCHANGE_LR//定义交换左右声道
//定义得以保存PT2314设置到EEPROM
#define SAVE_PT2314_SETTING
//-----------------PT2314配置结束-------------------//

#endif//PT2314_CFG
#ifdef IC_PT2314
#define PT2314ADDRESS 	0x44
#elif defined(IC_PT2315)
#define PT2314ADDRESS 	0x40
#endif

#define PT2314_MAXVOLUME				63
#define PT2314_MINVOLUME				0

#define PT2314_MAXSPEAKERATTENUATORS	15
#define PT2314_MINSPEAKERATTENUATORS	(-15)

#define PT2314_MAXINPUTGAIN				3
#define PT2314_MININPUTGAIN				0

#define PT2314_MAXAUDIOCHUNNEL			4
#define PT2314_MINAUDIOCHUNNEL			0

#define PT2314_MAXTONE			7//中间值7为直通
#define PT2314_MINTONE			(-7)
/*
#define PT2314_AudioChannelsCount		4
#define PT2314_AM_FMChunnel		0
#define PT2314_CDChunnel		1
#define PT2314_TapeChunnel		2
#define PT2314_AuxChunnel		3

#define PT2314_DefaultVolume			30
#define PT2314_DefaultChunnel			AuxChunnel //first    0
#define PT2314_DefaultLoudness			1
#define PT2314_DefaultInputGain			3//0=+11.25dB;1= +7.5dB;2= +3.75dB;3= 0dB


enum PT2314_CONTROL
{
	PT2314_VOLUME		=	0x00,
	PT2314_SPEAKERATT	=	0xc0,
	PT2314_LOUDNESS		=	0x40,
	PT2314_TONE			=	0x60,
};
*/

#if !defined(SAVE_PT2314_SETTING)
extern uint8 PT2314Volume;
extern uint8 PT2314Bass;
extern uint8 PT2314Treble;
extern int8  PT2314SpeakerATT;
extern uint8 PT2314Loudness;
#else//SAVE_PT2314_SETTING
#define PT2314Volume		(uint8)EepromBuffer[ESL_PT2314Volume]
#define PT2314Bass			(uint8)EepromBuffer[ESL_PT2314Bass]
#define PT2314Treble		(uint8)EepromBuffer[ESL_PT2314Treble]
#define PT2314SpeakerATT	(int8) EepromBuffer[ESL_PT2314SpeakerATT]
#define PT2314Loudness		(uint8)EepromBuffer[ESL_PT2314Loudness]
#endif//SAVE_PT2314_SETTING

extern bool  PT2314Mute;	
//注意为了使用方便我把InputGain，PT2314TSwitch选项储蓄到了(uint8)EepromBuffer[PT2314Loudness]里
#define PT2314_SWITCH_MASK		(3)
#define PT2314_INPUTGAIN_MASK	(3<<3)
#define PT2314_LOUDNESS_MASK	(1<<2)
#define PT2314_LoudnessSet()	(PT2314Loudness|=PT2314_LOUDNESS_MASK)
#define PT2314_LoudnessClear()	(PT2314Loudness&=~PT2314_LOUDNESS_MASK)
#define PT2314_LoudnessGet()	(PT2314Loudness&PT2314_LOUDNESS_MASK)


extern void PT2314UpdateAll();
//extern void PT2314Update(uint8 flag);
#define IniPT2314() PT2314UpdateAll()

#endif//_PT2314_H_ 
