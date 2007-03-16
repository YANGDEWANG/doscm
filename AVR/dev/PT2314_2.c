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
#include <global.h>
#include <math.h>
#include "PT2314_2.h"
#include "iic.h"

#if !defined(SAVE_PT2314_2_SETTING)
uint8 PT2314_2Volume;
uint8 PT2314_2Bass;
uint8 PT2314_2Treble;
int8  PT2314_2SpeakerATT;
uint8 PT2314_2Loudness;
#endif//SAVE_PT2314_2_SETTING
bool  PT2314_2Mute;
static uint8 dat[6];
void PT2314_2UpdateAll()
{
	
	//VOLUME
#ifdef PT2314_2_MAINVOLUME_U
	dat[0]=PT2314_2_MAXVOLUME + MAINVOLUME_MAX - PT2314_2Volume-MainVolume;
#else//PT2314_2_MAINVOLUME_U
	dat[0]=PT2314_2_MAXVOLUME - PT2314_2Volume;
#endif//PT2314_2_MAINVOLUME_U
	//ATTL
#ifdef IC_PT2314_2
	if(PT2314_2Mute)
	{
		dat[1]=0xdf;//l
		dat[2]=0xff;//r
	}
	else
	{
		#ifdef PT2314_2_EXCHANGE_LR
		dat[2]=0xdf-0x1f;//l
		dat[1]=0xff-0x1f;//r
#else
		dat[1]=0xdf-0x1f;//l
		dat[2]=0xff-0x1f;//r
#endif//PT2314_2_EXCHANGE_LR

		if(PT2314_2SpeakerATT != 0)
		{
			if(PT2314_2SpeakerATT>0)//衰减l
			{
#ifndef PT2314_2_DONT_ATTL
				dat[1]|= PT2314_2SpeakerATT;
#else//PT2314_2_DONT_ATTL
#pragma warning TODO:由于项目需要上面代码被临时注解
#endif//PT2314_2_DONT_ATTL
			}
			else//衰减r
			{
				dat[2]|=-PT2314_2SpeakerATT;
			}
		}
	}
#elif defined(IC_PT2315)
	if(PT2314_2Mute)
	{
		dat[1]=0xdf-0x40;//l
		dat[2]=0xff-0x40;//r
	}
	else
	{
#ifdef PT2314_2_EXCHANGE_LR
		dat[2]=0xdf-0x1f-0x40;//l
		dat[1]=0xff-0x1f-0x40;//r
#else
		dat[1]=0xdf-0x1f-0x40;//l
		dat[2]=0xff-0x1f-0x40;//r
#endif//PT2314_2_EXCHANGE_LR
		if(PT2314_2SpeakerATT != 0)
		{
			if(PT2314_2SpeakerATT>0)//衰减l
			{
#ifndef PT2314_2_DONT_ATTL
				dat[1]|= PT2314_2SpeakerATT;
#else//PT2314_2_DONT_ATTL
#pragma warning TODO:由于项目需要上面代码被临时注解
#endif//PT2314_2_DONT_ATTL

			}
			else//衰减r
			{
				dat[2]|=-PT2314_2SpeakerATT;
			}
		}
	}
#else
#error "未选择IC"
#endif
	//LOUDNESS
	dat[3]=PT2314_2Loudness|0x40;
	//Bass Control
	if(PT2314_2Bass>0)
		dat[4] = (15-PT2314_2Bass);
	else
		dat[4] = (PT2314_2Bass+7);
	//Treble Control
	if(PT2314_2Treble>0)
		dat[5] = 15-PT2314_2Treble;
	else
		dat[5] = PT2314_2Treble+7;
	dat[4]|=0x60;
	dat[5]|=0x70;
	I2CWriteStream(PT2314_2ADDRESS,dat,sizeof(dat));

}
//void IniPT2314_2()
//{
//	PT2314_2UpdateAll();
//}

