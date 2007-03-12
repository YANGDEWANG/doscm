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
#include <global.h>
#include <math.h>
#include "PT2314.h"
#include "iic.h"

#if !defined(SAVE_PT2314_SETTING)
uint8 PT2314Volume;
uint8 PT2314Bass;
uint8 PT2314Treble;
int8  PT2314SpeakerATT;
uint8 PT2314Loudness;
#endif//SAVE_PT2314_SETTING
bool  PT2314Mute;
static uint8 dat[6];
void PT2314UpdateAll()
{
	
	//VOLUME
#ifdef MAINVOLUME_U
	dat[0]=PT2314_MAXVOLUME + MAINVOLUME_MAX - PT2314Volume-MainVolume;
#else//MAINVOLUME_U
	dat[0]=PT2314_MAXVOLUME - PT2314Volume;
#endif//MAINVOLUME_U
	//ATTL
#ifdef IC_PT2314
	if(PT2314Mute)
	{
		dat[1]=0xdf;//l
		dat[2]=0xff;//r
	}
	else
	{
		#ifdef PT2314_EXCHANGE_LR
		dat[2]=0xdf-0x1f;//l
		dat[1]=0xff-0x1f;//r
#else
		dat[1]=0xdf-0x1f;//l
		dat[2]=0xff-0x1f;//r
#endif//PT2314_EXCHANGE_LR

		if(PT2314SpeakerATT != 0)
		{
			if(PT2314SpeakerATT>0)//衰减l
			{
				dat[1]|= PT2314SpeakerATT;
			}
			else//衰减r
			{
				dat[2]|=-PT2314SpeakerATT;
			}
		}
	}
#elif defined(IC_PT2315)
	if(PT2314Mute)
	{
		dat[1]=0xdf-0x40;//l
		dat[2]=0xff-0x40;//r
	}
	else
	{
#ifdef PT2314_EXCHANGE_LR
		dat[2]=0xdf-0x1f-0x40;//l
		dat[1]=0xff-0x1f-0x40;//r
#else
		dat[1]=0xdf-0x1f-0x40;//l
		dat[2]=0xff-0x1f-0x40;//r
#endif//PT2314_EXCHANGE_LR
		if(PT2314SpeakerATT != 0)
		{
			if(PT2314SpeakerATT>0)//衰减l
			{
				dat[1]|= PT2314SpeakerATT;
//#pragma warning TODO:由于项目需要上面代码被临时注解
			}
			else//衰减r
			{
				dat[2]|=-PT2314SpeakerATT;
			}
		}
	}
#else
#error "未选择IC"
#endif
	//LOUDNESS
	dat[3]=PT2314Loudness|0x40;
	//Bass Control
	if(PT2314Bass>0)
		dat[4] = (15-PT2314Bass);
	else
		dat[4] = (PT2314Bass+7);
	//Treble Control
	if(PT2314Treble>0)
		dat[5] = 15-PT2314Treble;
	else
		dat[5] = PT2314Treble+7;
	dat[4]|=0x60;
	dat[5]|=0x70;
	I2CWriteStream(PT2314ADDRESS,dat,sizeof(dat));

}
//void IniPT2314()
//{
//	PT2314UpdateAll();
//}

