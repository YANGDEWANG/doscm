#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "configure.h"
#include "system/boolean.h"
#define NOTIFY_DIS_CHANGE
#define DISPLAY_SHINE_BIT	true	//����Ҫ������ʾӦ��������ʾ�����ֵΪ1����0
#define TOTAL_SEG 			16	//����������ʾλ
//#define DISPLAY_TEST//������ʾ
//#define DISPLAY_GLINT_POINT //ʹ�õ���˸
extern uint8 DisplayDeviceMemImage[PT6312_MEMORY_SIZE];		//��ʾ����
#ifdef NOTIFY_DIS_CHANGE
extern bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE


void IniDisplay();							//��ʼ������ջ���
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
extern void DisplayTestNext(bool v);//v:����
#endif//DISPLAY_TEST
 
#endif //_DISPLAY_H
