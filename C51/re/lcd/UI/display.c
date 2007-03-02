#include "Global.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "boolean.h"

/*
7段数码显示码定义
字符定义顺序：从左到右
字符段
*/
const prog_char VFD_7SEG_TAB [TOTAL_SEG][7]  =
{
	{ND11,ND21,ND31,ND41,ND51,ND61,ND71},//字符1
#if TOTAL_SEG > 1
	{ND12,ND12-ND11+ND21,ND12-ND11+ND31,ND12-ND11+ND41,ND12-ND11+ND51,ND12-ND11+ND61,ND12-ND11+ND71},//字符2
#endif
#if TOTAL_SEG > 2
	{ND13,ND13-ND11+ND21,ND13-ND11+ND31,ND13-ND11+ND41,ND13-ND11+ND51,ND13-ND11+ND61,ND13-ND11+ND71},//字符3
#endif
#if TOTAL_SEG > 3
	{ND14,ND14-ND11+ND21,ND14-ND11+ND31,ND14-ND11+ND41,ND14-ND11+ND51,ND14-ND11+ND61,ND14-ND11+ND71},//字符4
#endif
#if TOTAL_SEG > 4
	{ND15,ND15-ND11+ND21,ND15-ND11+ND31,ND15-ND11+ND41,ND15-ND11+ND51,ND15-ND11+ND61,ND15-ND11+ND71},//字符5
#endif
#if TOTAL_SEG > 5
	{ND16,ND16-ND11+ND21,ND16-ND11+ND31,ND16-ND11+ND41,ND16-ND11+ND51,ND16-ND11+ND61,ND16-ND11+ND71},//字符6
#endif
#if TOTAL_SEG > 6
	{ND17,ND17-ND11+ND21,ND17-ND11+ND31,ND17-ND11+ND41,ND17-ND11+ND51,ND17-ND11+ND61,ND17-ND11+ND71} //字符7
#endif
};

const prog_char vfd_char[26]=
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
	VFD_7SEG_Z //z
};



const  prog_char  vfd_digit[] =
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
#ifdef NOTIFY_DIS_CHANGE
bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE
uint8 DisplayDeviceMemImage[DISPLAY_MEMORY_SIZE];	//显示设备内存映像
uint8 perchdata dislinebuf[DIS_LINEBUF_SIZE];//字符操作缓存
static void setOffsideToNull(uint8 n)
{
	while(n<TOTAL_SEG)
	{ 
		dislinebuf[n++]=0;
	}
}
static void setString(char const*s)
{
	uint8 i;
	char c;
	for(i=0;i<TOTAL_SEG;i++)
	{
		c = s[i];
		if( (c>='A')&&(c<='Z') )
			c=vfd_char[c-'A'];
		/*else if( (c>='a')&&(c<='z'))
			c=vfd_char[c-0x20-'A'];*/
		else if( (c>='0')&&(c<='9') )
			c=vfd_digit[c-'0'];
		else if(c=='-')
			c=VFD_7SEG___;
		else if(c=='_')
			c=VFD_7SEG__;
		else if(c=='\0')
			break;
		else
			c=0;
		dislinebuf[i]=c;
	}/*for loop*/
	setOffsideToNull(i);
}
static void Show()
{
	BYTE i,j;
	for(j=0;j<TOTAL_SEG;j++)
	{
		uint8 dat	= dislinebuf[j];
		for(i=0;i<7;i++)
		{
			SetBitDisplay(VFD_7SEG_TAB[j][i],dat&1); 
			dat = dat>>1;
		}
	}
}
void  ShowString(char const*s)
{
	setString(s);
	Show();
}
/*
void ShowINT(int8 snu,bool s)
{
uint8 lc = 0;
uint8 nu;
//printf("");
if(s)
{
nu= cabs(snu);
if(snu<0)
{
dislinebuf[0]=VFD_7SEG___;
}
}
else
{
nu=(uint8)snu;
}
if(nu<10&&lc==0)
{
dislinebuf[0]=VFD_7SEG_0;
}
for(;lc<TOTAL_SEG;lc++)
{
if(nu==0)break;
dislinebuf[TOTAL_SEG-1-lc]=vfd_digit[nu%10];
nu = nu/10; 
}
//setOffsideToNull(lc);
Show();
}*/
void ShowUINT8(uint8 nu)
{
	dislinebuf[1]=vfd_digit[nu%10];
	nu = nu/10;
	dislinebuf[0]=vfd_digit[nu%10];
	Show();
}
void ShowINT8(int8 snu)
{
	ShowUINT8(cabs(snu));
	if(snu<0)
	{
		dislinebuf[0]=VFD_7SEG___;
	}
	Show();
}
/*void  ShowMesStr(uint8 id)
{
	ShowString(MessageString[id]);
}


void RefurbishDisplay()
{

RefurbishDisplayMemory();
DisplayChanged = false;
}
*/
//设置指定位的值true为点亮
void SetBitDisplay(uint8 lc,bool ver)
{	
#ifdef NOTIFY_DIS_CHANGE
#if (DISPLAY_SHINE_BIT==true)
	if(SetBit(DisplayDeviceMemImage,lc,ver))
#else
	if(SetBit(DisplayDeviceMemImage,lc,!ver))
#endif//DISPLAY_SHINE_BIT
	{
		DisplayChanged = true;
	}
#else//NOTIFY_DIS_CHANGE
#if (DISPLAY_SHINE_BIT==true)
	SetBit(DisplayDeviceMemImage,lc,ver);
#else
	SetBit(DisplayDeviceMemImage,lc,!ver);
#endif//DISPLAY_SHINE_BIT
#endif//NOTIFY_DIS_CHANGE
}
/*
void ClearNumberAll()//清除字符区域显示。
{
	ShowString("");
}
*/
void PollingDisplay()
{

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
#undef _DDATA_
}
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
