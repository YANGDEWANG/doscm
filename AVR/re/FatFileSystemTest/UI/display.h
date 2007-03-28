#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "configure.h"
#include "system/boolean.h"
#define NOTIFY_DIS_CHANGE
#define DISPLAY_SHINE_BIT	true	//定义要点亮显示应该设置显示缓存的值为1还是0
#define TOTAL_SEG 			16	//可用数字显示位
//#define DISPLAY_TEST//测试显示
//#define DISPLAY_GLINT_POINT //使用点闪烁
extern uint8 DisplayDeviceMemImage[PT6312_MEMORY_SIZE];		//显示缓存
#ifdef NOTIFY_DIS_CHANGE
extern bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE


void IniDisplay();							//初始化将清空缓存
void UpdateDisplay();
void DisplaySetBit(uint8 bitAdd);// SetBit(DisplayDeviceMemImage,bit,true)
void CleanDisplayMemImage();
//extern uint8 GlintLen;
extern bool GlintEn;
extern uint8 GlintLc;
extern uint8 GlintData;
void SetGlint();
#define StopGlint() (GlintEn=0)
//#define StopGlint() (GlintLen=0)
extern void PollingDisplay500ms();
#ifdef DISPLAY_GLINT_POINT
extern bool DisplayGlintState;
extern uint8 DisplayGlintPoint;
extern bool DisplayGlintExitState;
extern void ExitDisplayGlintPoint();
extern void SetDisplayGlintPoint(uint8 Point,bool ExitState);

#endif//DISPLAY_GLINT_POINT

#ifdef DISPLAY_TEST
extern void DisplayTestNext(bool v);//v:方向
#endif//DISPLAY_TEST
 
#endif //_DISPLAY_H
