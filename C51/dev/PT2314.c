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
void PT2314UpdateAll()
{
	uint8 dat[6];
	//VOLUME
	dat[0]=PT2314_MAXVOLUME - PT2314Volume;
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
				dat[2]|=cabs(PT2314SpeakerATT);
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
				//dat[1]|= PT2314SpeakerATT;
#pragma warning TODO:由于项目需要上面代码被临时注解
			}
			else//衰减r
			{
				dat[2]|=cabs(PT2314SpeakerATT);
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
	I2CWriteBunch(PT2314ADDRESS,dat,sizeof(dat),true);

}
void IniPT2314()
{
	PT2314UpdateAll();
}
/*
#ifdef ConSpeakerATT_U
int8 SpeakerAttenuators;
static uint8 tmpConSpeakerATT[2];
ConSpeakerATT(int8 att)//平衡控制正数衰减左声道
{
uint8 tmp[2];
SpeakerAttenuators = att;
if(att == 0)
tmp[0]=tmp[1]=0;
else
{
if(att>0)//衰减l
{
tmp[1] = att&MaxSpeakerAttenuators;
tmp[0] = 0;
}
else//衰减r
{
att = cabs(att);
tmp[0] = att&MaxSpeakerAttenuators;
tmp[1] = 0;
}
}
tmp[0]	=	tmp[0]|224;
tmp[1]	=	tmp[1]|192;
I2CWriteBunch(pt2314address,tmp,2,true);//写一个字节串到指定设备 1:设备地址 2:数据首字节指针 3：串长度 re：成功写入字节数

}
#endif //ConSpeakerATT_U

#ifdef ConLoudness_U
void ConLoudness(bool OFForON)//开关等响
{
Loudness = OFForON;	
AudioSwitch(CurrentChunnel);
}
#endif //ConLoudness_U
#ifdef ConInputGain_U
void ConInputGain(uint8 gain)//输入增益
{
InputGain = gain&3;
AudioSwitch(CurrentChunnel);	
}
#endif //ConInputGain_U

*/
/*void ConTB(uint8 con,uint8 BorT)//控制高底音 1: 直通为7，最高14 2:true=高音，false=底音
{
con &=0x0f;
//if(con>MaxTrebleBass)
//con = MaxTrebleBass;
if(BorT)
{
Treble 	= 	con;
EepromSaveChar(StorageLocalTreble,con);
}
else
{
Bass 	=	con;
EepromSaveChar(StorageLocalBass,con);
}
if(con>7)
{
//con = 22-con;	
con = (~con)|(1<<3);		
}
if(BorT)
{
con|=0x70;
}
else
{
con|=0x60;
}
write(con);
}*/
/*
void ConBass(uint8 con)//控制底音直通为7，最高14
{
//con &=0x0f;
con %=15;
//if(con>MaxTrebleBass)
//con = MaxTrebleBass;

EepromSaveChar(StorageLocalBass,con);
if(con>7)
{
con = 22-con;	
//con = (~con)|(1<<3);		
}

con|=0x60;
write(con);
}
void ConTreble(uint8 con)//控制高音直通为7，最高14 
{
//con &=0x0f;
con %=15;
EepromSaveChar(StorageLocalTreble,con);
if(con>7)
{
con = 22-con;
//con = (~con)|(1<<3);		
}
con|=0x70;
write(con);
}


#ifdef VolumeGather
void GatherVolume(uint8 aim,uint8 root)//调整音量从root 到 aim
{	
//aim&= MaxVolume;
aimVolume = aim&MaxVolume;
currentlyVolume = root&MaxVolume;
write(MaxVolume - currentlyVolume);
}
#endif
void AudioSwitch(uint8 dat) //切换输入通道
{//010000xx//0x40->等响开0x44->等响关
dat = CurrentChunnel = dat&3;
dat |= (InputGain<<3)|64;
//dat|=64;
#ifdef ConLoudness_U
if(!Loudness)
dat|=0x04;
#endif
#ifdef VolumeGather
GatherVolume(Volume,0);
#endif
write(dat);
}
void ConVolume(uint8 vol)//控制音量
{
vol&= MaxVolume;
EepromSaveChar(StorageLocalVolume,vol);
#ifdef VolumeGather
GatherVolume(vol,aimVolume);
#else
vol = MaxVolume - vol;
write(vol);
#endif
}
void ConMute(bool of)//禁音 of=true->禁音/如果定义了VolumeGather禁音过程将需要一段时间需要马上禁音的话请使用GatherVolume(0,1);
{
if(Mute == of)
return;
Mute = of;
if(of)
{
#ifdef VolumeGather
GatherVolume(0,Volume);
#else
write(0xff);
write(0xdf);
#endif
}
else
{
#ifdef VolumeGather
GatherVolume(Volume,0);
#endif
#ifdef ConSpeakerATT_U
ConSpeakerATT(SpeakerAttenuators);//平衡控制正数衰减左声道
#else
write(0xC0);
write(0xE0);
#endif
}
}
void IniPT2314()	 //初始化
{
//	eeprom_read_byte
ConVolume(Volume);//控制音量
ConBass(Bass);//控制底音直通为7，最高14
ConTreble(Treble);//控制高音直通为7，最高14 
#ifdef ConSpeakerATT_U
ConSpeakerATT(SpeakerAttenuators);//平衡控制正数衰减左声道
#else
ConMute(false);
#endif
AudioSwitch(DefaultChunnel);	

}
#ifdef VolumeGather
void DriveAudio()
{
uint8 vol;
if(Click20ms_DriveAudio)
{
Click20ms_DriveAudio = false;
if(aimVolume!=currentlyVolume)
{
if(aimVolume>currentlyVolume)
{
vol = MaxVolume-(++currentlyVolume);
}
else
{
vol = MaxVolume-(--currentlyVolume);
}
if(currentlyVolume == 0)
{
write(0xff);
write(0xdf);
Mute = true;
}
else
{
write(vol);
}
if(aimVolume != 0)
{
ConMute(false);
}
}
}
}
#endif
*/