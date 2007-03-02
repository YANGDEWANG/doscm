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
#include <global.h>
#include "M62446.h"
#include "SPI.h"
#include "..\system\AutoControl.h"
#if !defined(SAVE_M62446_SETTING)
uint8 M62446VolumeL;
uint8 M62446VolumeR;
uint8 M62446VolumeCC;
uint8 M62446VolumeSW;
uint8 M62446VolumeSL;
uint8 M62446VolumeSR;
int8  M62446Bass;
int8  M62446Treble;
uint8 M62446OutputPort;
#ifdef M62446_WORK_MODE_U
uint8 M62446WorkMode;//其中对应M62446_WORK_CHUNNEL的位为1时声道禁音
#endif//M62446_WORK_MODE_U
#endif//SAVE_M62446_SETTING	
//bool  M62446

//错乱定义索引可以map声道
#ifndef M62446_CH_MAP
#define _M62446MainVolume	M62446MainVolume	
#define _M62446VolumeR		M62446VolumeR		
#define _M62446VolumeL		M62446VolumeL		
#define _M62446VolumeSW		M62446VolumeSW	 
#define _M62446VolumeCC		M62446VolumeCC		 
#define _M62446VolumeSL		M62446VolumeSL		
#define _M62446VolumeSR		M62446VolumeSR	
#define _M62446_WC_L		M62446_WC_L					  
#define _M62446_WC_R		M62446_WC_R					  
#define _M62446_WC_SL		M62446_WC_SL					  
#define _M62446_WC_SR		M62446_WC_SR					  
#define _M62446_WC_CC		M62446_WC_CC					  
#define _M62446_WC_SW		M62446_WC_SW
#endif

int8  M62446MuteUtility;				
bool  M62446ByPass;
bool  M62446Mute;
static prog_char ToneTable[]={14,12,11,10,9,0,1,2,3,4,6};
#define setLATCH()	(LATCH_M62446 = true)
#define clLATCH()	(LATCH_M62446 = false)
union _M6244COM
{
	_uint8 dat[2];
	struct
	{
		_uint8 flag	:2;
		_uint8 R_L	:6;	//右声道的低6位
		//_uint8 R_H	:1;
		_uint8 L_R_H	:8;//左声道和右声道的最高位
	}COMLR;
	struct
	{
		_uint8 flag	:2;
		_uint8 ByPass:1; 
		_uint8 		:1;
		_uint8 Bass	:4;
		_uint8 OutputPort :4;
		_uint8 Treble:4;
	}COMTONE;
}M6244COM;
static void wm()
{
	clLATCH();//todo:
	//TODO:注意51和AVR不同
	VSPI_MasterTransmitMSB(M6244COM.dat[1]);
	VSPI_MasterTransmitMSB(M6244COM.dat[0]);
	//2us
	setLATCH();//3.2us
}
#ifdef M62446_WORK_MODE_U
void M62446Update(uint8 flag)
{
	int8 l,r;
	bool ml=false;
	bool mr=false;
	M6244COM.COMLR.flag=flag;
	switch(flag)
	{
	case M62446TONE_OUTPUT:
		{
			M6244COM.COMTONE.ByPass	= M62446ByPassGet();
			M6244COM.COMTONE.Bass	= ToneTable[M62446Bass+5];
			M6244COM.COMTONE.Treble	= ToneTable[M62446Treble+5];
			M6244COM.COMTONE.OutputPort	= M62446OutputPort;
			goto END;
		}
	case M62446VOLUMELR:
		{
			if(M62446WorkMode&(1<<_M62446_WC_L))
				ml=true;
			if(M62446WorkMode&(1<<_M62446_WC_R))
				mr=true;
			l = _M62446VolumeL;
			r = _M62446VolumeR;
			break;
		}
	case M62446VOLUMESLSR:
		{
			if(M62446WorkMode&(1<<_M62446_WC_SL))ml=true;
			if(M62446WorkMode&(1<<_M62446_WC_SR))mr=true;
			l = _M62446VolumeSL;
			r = _M62446VolumeSR;
			break;
		}
	case M62446VOLUMECCSW:
		{
			if(M62446WorkMode&(1<<_M62446_WC_CC))ml=true;
			if(M62446WorkMode&(1<<_M62446_WC_SW))mr=true;
			l = _M62446VolumeCC;
			r = _M62446VolumeSW;
			break;
		}
	}
	l = l+M62446MainVolume-M62446MuteUtility;
	r = r+M62446MainVolume-M62446MuteUtility;
	if(l<0||ml)l=0;
	if(r<0||mr)r=0;
	l=M62446MAX_VOLUME-l;
	r=M62446MAX_VOLUME-r;
	M6244COM.COMLR.R_L=r;
	M6244COM.COMLR.L_R_H=(l<<1|r>>6);
END:
	wm();
}
#else//M62446_WORK_MODE_U
void M62446Update(uint8 flag)
{
	int8 l,r;
	M6244COM.COMLR.flag=flag;
	switch(flag)
	{
	case M62446TONE_OUTPUT:
		{
			M6244COM.COMTONE.ByPass	= M62446ByPassGet();
			M6244COM.COMTONE.Bass	= ToneTable[M62446Bass+5];
			M6244COM.COMTONE.Treble	= ToneTable[M62446Treble+5];
			M6244COM.COMTONE.OutputPort	= M62446OutputPort;
			goto END;
		}
	case M62446VOLUMELR:
		{
			l = _M62446VolumeL;
			r = _M62446VolumeR;
			break;
		}
	case M62446VOLUMESLSR:
		{
			l = _M62446VolumeSL;
			r = _M62446VolumeSR;
			break;
		}
	case M62446VOLUMECCSW:
		{
			l = _M62446VolumeCC;
			r = _M62446VolumeSW;
			break;
		}
	}
	l = l+M62446MainVolume-M62446MuteUtility;
	r = r+M62446MainVolume-M62446MuteUtility;
	if(l<0)l=0;
	if(r<0)r=0;
	l=M62446MAX_VOLUME-l;
	r=M62446MAX_VOLUME-r;
	M6244COM.COMLR.R_L=r;
	M6244COM.COMLR.L_R_H=(l<<1|r>>6);
END:
	wm();
}
#endif//M62446_WORK_MODE_U
void M62446UpdateAll()
{
	uint8 i;
	for(i=0;i<M62446CONTROLMAX;i++)
	{
		M62446Update(i);
	}
	if(M62446MuteUtility==M62446MAX_MUTEUTILITY)
		OFFJJ();//开继电器
	else
		ONJJ();//开继电器
}
static void iniAutoControl()
{
	AutoControl.dat = &M62446MuteUtility;
	AutoControl.Max = M62446MAX_MUTEUTILITY;
	AutoControl.Min = M62446MIN_MUTEUTILITY;
	AutoControl.Callback = M62446UpdateAll;
}
void M62446ToMute()
{
	M62446MuteUtility = M62446MIN_MUTEUTILITY;
	AutoControl.Step = 4;
	iniAutoControl();
}
void M62446ToSound()
{
	M62446MuteUtility = M62446MAX_MUTEUTILITY-8;//-8缩短开启时间
	AutoControl.Step = -4;
	iniAutoControl();
}

