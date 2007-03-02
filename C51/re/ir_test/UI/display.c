#include "Global.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "boolean.h"
#include "tm1618.h"

const prog_char vfd_char[26+2]=
{
	VFD_7SEG_A,
	VFD_7SEG_B,
	VFD_7SEG_C,
	VFD_7SEG_D,
	VFD_7SEG_E,
	VFD_7SEG_F,
	VFD_7SEG_G,
	VFD_7SEG_H,
	VFD_7SEG_I,
	VFD_7SEG_J,
	VFD_7SEG_K,
	VFD_7SEG_L,
	VFD_7SEG_M,
	VFD_7SEG_N,
	VFD_7SEG_O,
	VFD_7SEG_P,
	VFD_7SEG_P,//q
	VFD_7SEG_R,
	VFD_7SEG_S,
	VFD_7SEG_T,
	VFD_7SEG_U,
	VFD_7SEG_U,//v
	VFD_7SEG_W,
	VFD_7SEG_X,
	VFD_7SEG_Y,
	VFD_7SEG_Z, //z
	VFD_7SEG___,//-
	VFD_7SEG__,//_
};



const  prog_char  vfd_digit[10] =
{
	VFD_7SEG_0,
	VFD_7SEG_1,
	VFD_7SEG_2,
	VFD_7SEG_3,
	VFD_7SEG_4,
	VFD_7SEG_5,
	VFD_7SEG_6,
	VFD_7SEG_7,
	VFD_7SEG_8,
	VFD_7SEG_9
};
const prog_char vfd2_char[26+2]=
{
	VFD2_7SEG_A,
	VFD2_7SEG_B,
	VFD2_7SEG_C,
	VFD2_7SEG_D,
	VFD2_7SEG_E,
	VFD2_7SEG_F,
	VFD2_7SEG_G,
	VFD2_7SEG_H,
	VFD2_7SEG_I,
	VFD2_7SEG_J,
	VFD2_7SEG_K,
	VFD2_7SEG_L,
	VFD2_7SEG_M,
	VFD2_7SEG_N,
	VFD2_7SEG_O,
	VFD2_7SEG_P,
	VFD2_7SEG_P,//q
	VFD2_7SEG_R,
	VFD2_7SEG_S,
	VFD2_7SEG_T,
	VFD2_7SEG_U,
	VFD2_7SEG_U,//v
	VFD2_7SEG_W,
	VFD2_7SEG_X,
	VFD2_7SEG_Y,
	VFD2_7SEG_Z, //z
	VFD2_7SEG___,//-
	VFD2_7SEG__,//_
};	   



const  prog_char  vfd2_digit[10] =
{
	VFD2_7SEG_0,
	VFD2_7SEG_1,
	VFD2_7SEG_2,
	VFD2_7SEG_3,
	VFD2_7SEG_4,
	VFD2_7SEG_5,
	VFD2_7SEG_6,
	VFD2_7SEG_7,
	VFD2_7SEG_8,
	VFD2_7SEG_9
};
#ifdef NOTIFY_DIS_CHANGE
bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE
uint8 data DisplayDeviceMemImage[DISPLAY_MEMORY_SIZE];	//显示设备内存映像
uint8 perchdata dislinebuf[DIS_LINEBUF_SIZE];//字符操作缓存
sbit stb0=P1^0;
sbit stb1=P3^3;
sbit stb2=P2^3;
sbit stb3=P0^4;

void setSTB0()	{stb0=true;}
void clSTB0()	{stb0=false;}
void setSTB1()	{stb1=true;}
void clSTB1()	{stb1=false;}
void setSTB2()	{stb2=true;}
void clSTB2()   {stb2=false;}
void setSTB3()	{stb3=true;}
void clSTB3()   {stb3=false;}
void send_dis_todev(uint8 disC)
{
	switch(disC)
	{
	case 0:
		{
			tm1628_clSTB=clSTB0;
			tm1628_setSTB=setSTB0;
			break;
		}
	case 1:
		{
			tm1628_clSTB=clSTB1;
			tm1628_setSTB=setSTB1;
			break;
		}
	case 2:
		{
			tm1628_clSTB=clSTB2;
			tm1628_setSTB=setSTB2;
			break;
		}
	case 3:
		{
			tm1628_clSTB=clSTB3;
			tm1628_setSTB=setSTB3;
			break;
		}
	}
	TM1618UpdateAll(DisplayDeviceMemImage+disC*4);
}
void UpdateDisplay()
{
	uint8 i=0;
	while(i<4)send_dis_todev(i),i++;  //2175--2305
	DisplayChanged = false; 
}
//只支持大写字母
void ShowString(char const*s,uint8 stp,uint8 len)
{
	//	uint8 i;
	char c;
	uint8 data* out =   DisplayDeviceMemImage+stp;
	prog_char *vfdc = vfd_char;
	prog_char *vfdd = vfd_digit;
	DisplayChanged = true;
	if(len)
	{
		do
		//for(i=0;i<len;i++,stp++)
		{
			//if(stp>=DISPLAY_MEMORY_SIZE/2)break;//转到第二个屏
			if(out>=&DisplayDeviceMemImage[DISPLAY_MEMORY_SIZE/2])
			{
				vfdc = vfd2_char;
				vfdd = vfd2_digit;
			}
			c = *s++;
			if( (c>='A')&&(c<='Z') )
				c=vfdc[c-'A'];
			/*else if( (c>='a')&&(c<='z'))
			c=vfd_char[c-0x20-'A'];*/
			else if( (c>='0')&&(c<='9') )
				c=vfdd[c-'0'];
			else if(c=='-')
				c=vfdc[26];
			else if(c=='_')
				c=vfdc[27];
			else if(c=='\0')
				break;
			else
				c=0;
			*out++=c;
		}while(--len);
#if 0
		if(!len)
			return;
		do
		//for(;i<len;i++,stp++)
		{
			c = *s++;
			if((c>='A')&&(c<='Z'))
				c=vfd2_char[c-'A'];
			/*else if( (c>='a')&&(c<='z'))
			c=vfd_char[c-0x20-'A'];*/
			else if( (c>='0')&&(c<='9') )
				c=vfd2_digit[c-'0'];
			else if(c=='-')
				c=VFD2_7SEG___;
			else if(c=='_')
				c=VFD2_7SEG__;
			else if(c=='\0')
				break;
			else
				c=0;
			//DisplayDeviceMemImage[stp++]=c;
			*out++=c;
		}while(--len);
#endif
		if(len)
		{
			do
			{
				//DisplayDeviceMemImage[stp++]=VFD2_7SEG__;
				*out++=0;
				//stp++;
			}while(--len);
		}
	}
	SetdislinebufTo(' ');//注意清除了dislinebuf
	/*for loop*/
	//setOffsideToNull(i);
}

void SetdislinebufTo(char c)
{
	uint8 i =0;
	for(;i<sizeof(dislinebuf);i++)
	{
		dislinebuf[i]=c;
	}
}
//注意显示为右对齐lc为最右边位置
void ShowUint8(uint8 lc,uint8 dat)
{
	uint8 l = ToStringWithU(dislinebuf,dat);
	ShowString(dislinebuf,lc-l+1,l);
}
void IniDisplay()//初始化清空缓存
{
#if defined(DISPLAY_TEST)
#if (DISPLAY_SHINE_BIT==true)
#define _DDATA_ 0xff
#else
#define _DDATA_ 0
#endif
#else
#if (DISPLAY_SHINE_BIT==true)
#define _DDATA_ 0
#else
#define _DDATA_ 0xff
#endif	
#endif//DISPLAY_TEST
	memset(DisplayDeviceMemImage,_DDATA_,sizeof(DisplayDeviceMemImage));
	//ShowString("",0,16);
#undef _DDATA_
	UpdateDisplay();
}
#define MAX_GLINTLEN 1
#if MAX_GLINTLEN>1
static uint8 GlintLc;
uint8 GlintLen;
static uint8 GlintData[MAX_GLINTLEN];
static bool GlintSt;
//bool StopGlint;
void PollingDisplay500ms()
{
	uint8 i;
	if(GlintLen)
	{
		for(i=0,i<GlintLen,i++)
		{
			if(GlintSt)
			{
				DisplayDeviceMemImage[GlintLc+i]=GlintData[i];
			}
			else
			{
				DisplayDeviceMemImage[GlintLc+i]=0;
			}
		}
		GlintSt=!GlintSt;
	}
}
void SetGlint(uint8 lc,uint8 len)
{
	GlintLc =lc;
	GlintLen = len%(MAX_GLINTLEN+1);
	for(i=0,i<GlintLen,i++)
	{
		GlintData[i]=DisplayDeviceMemImage[GlintLc+i];
	}
}
#else
uint8 GlintLc;
uint8 GlintData;
static bool GlintSt;
bool GlintEn;
void PollingDisplay500ms()
{
	if(GlintEn)
	{
		if(GlintSt)
			DisplayDeviceMemImage[GlintLc]=GlintData;
		else
			DisplayDeviceMemImage[GlintLc]=0;
		GlintSt=!GlintSt;
		UpdateDisplay();
	}
}
void SetGlint()
{
	GlintData=DisplayDeviceMemImage[GlintLc];
	GlintEn = true;
}
#endif//MAX_GLINTLEN>1

#ifdef DISPLAY_GLINT_POINT
uint8 DisplayGlintPoint=DISP_NULL;
bool DisplayGlintState;
bool DisplayGlintExitState;
void ExitDisplayGlintPoint()
{
	SetBitDisplay(DisplayGlintPoint,DisplayGlintExitState);
}
void SetDisplayGlintPoint(uint8 Point,bool ExitState)
{
	ExitDisplayGlintPoint();
	DisplayGlintPoint = Point;
	DisplayGlintExitState = ExitState;
}
void PollingDisplay500ms()
{
#ifndef DISPLAY_TEST

	if(DISP_NULL==DisplayGlintPoint)
		return;
	DisplayGlintState=!DisplayGlintState;
	SetBitDisplay(DisplayGlintPoint,DisplayGlintState);
#else//DISPLAY_TEST
	static bool b;
	static uint8 oldDisplayTestPoint;
	b=!b;
	if(oldDisplayTestPoint!=DisplayGlintPoint)
	{
		SetBitDisplay(oldDisplayTestPoint,false);
		oldDisplayTestPoint=DisplayGlintPoint;
	}
	SetBitDisplay(DisplayGlintPoint,b);
#endif//DISPLAY_TEST
}
#endif//DISPLAY_GLINT_POINT
#ifdef DISPLAY_TEST
void DisplayTestNext(bool v)//v:方向
{
	if(v)
	{
		DisplayGlintPoint++;
		if(DisplayGlintPoint>=sizeof(DisplayDeviceMemImage)*8)
		{
			DisplayGlintPoint = 0;
		}
	}
	else
	{
		DisplayGlintPoint--;
		if(DisplayGlintPoint>=sizeof(DisplayDeviceMemImage)*8)
		{
			DisplayGlintPoint = sizeof(DisplayDeviceMemImage)*8-1;
		}
	}
}

#endif//
