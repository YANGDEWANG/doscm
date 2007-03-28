#include "Global.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "display.h"
#include "system/boolean.h"
#include "dev/pt6312.h"
#include "../dev/spiflash.h"

#ifdef NOTIFY_DIS_CHANGE
bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE
uint8 DisplayDeviceMemImage[PT6312_MEMORY_SIZE];	//显示设备内存映像
void DisplaySetBit(uint8 bitAdd)
{
	SetBit(DisplayDeviceMemImage,bitAdd);
}
void UpdateDisplay()
{
	WaitSPIIdle();
	PT6312UpdateAll(DisplayDeviceMemImage);
	DisplayChanged = false; 
}
void CleanDisplayMemImage()
{
	memset(DisplayDeviceMemImage,0,sizeof(DisplayDeviceMemImage));
}
void IniDisplay()//初始化清空缓存
{
#if defined(DISPLAY_TEST)
	memset(DisplayDeviceMemImage,0xff,sizeof(DisplayDeviceMemImage));
#else
	CleanDisplayMemImage();
	UpdateDisplay();
#endif
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

#endif//DISPLAY_GLINT_POINT
