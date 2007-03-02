#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "configure.h"

//显示屏选择
#include "se_a13.h"
//#define DISPLAY_TEST//测试显示
#define DISPLAY_GLINT_POINT //使用点闪烁
extern uint8 DisplayDeviceMemImage[DISPLAY_MEMORY_SIZE];		//显示缓存
#ifdef NOTIFY_DIS_CHANGE
extern bool DisplayChanged;
#endif//NOTIFY_DIS_CHANGE


extern void IniDisplay();							//初始化将清空缓存
extern void SetBitDisplay(uint8,bool);				//设置指定位的值(位置,值)
extern void ShowString(char const*s);				//s：字符串
extern void ShowMesStr(uint8 id);					//id：系统消息ID
extern void ShowUINT8(uint8 d);
extern void ShowINT8(int8 d);
extern void PollingDisplay();						//处理显示

#ifdef DISPLAY_GLINT_POINT
extern bool DisplayGlintState;
extern uint8 DisplayGlintPoint;
extern bool DisplayGlintExitState;
extern void ExitDisplayGlintPoint();
extern void SetDisplayGlintPoint(uint8 Point,bool ExitState);
extern void PollingDisplay500ms();
#endif//DISPLAY_GLINT_POINT

#ifdef DISPLAY_TEST
extern void DisplayTestNext(bool v);//v:方向
#endif//DISPLAY_TEST


//extern void RefurbishDisplay();						//写显示缓存到显示设备	

/*
** Definition for 7-seg display
**
** we assume 7-seg are defined as traditional 7-segment LED.
**
**	 AAAA			
**	C    B				 
**	C    B				 
**	 DDDD				 
**	F    E				 
**	F    E				 
**	 GGGG				 
**					 
**					 

*/



#define	VFD_7SEG_SA	(0x01)
#define	VFD_7SEG_SB	(0x02)
#define	VFD_7SEG_SC	(0x04)
#define	VFD_7SEG_SD	(0x08)
#define	VFD_7SEG_SE	(0x10)
#define	VFD_7SEG_SF	(0x20)
#define	VFD_7SEG_SG	(0x40)


#define	VFD_7SEG_0	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SG|VFD_7SEG_SC|VFD_7SEG_SE|VFD_7SEG_SF)
#define	VFD_7SEG_1	(VFD_7SEG_SB|VFD_7SEG_SE)
#define	VFD_7SEG_2	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_3	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_4	(VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SB|VFD_7SEG_SE)
#define	VFD_7SEG_5	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_6	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_7	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SE)
#define	VFD_7SEG_8	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_9	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_DIG_MASK	(VFD_7SEG_8)

#define	VFD_7SEG_A	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SE|VFD_7SEG_SF|VFD_7SEG_SD)
#define	VFD_7SEG_b	(VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_B	VFD_7SEG_b
#define	VFD_7SEG_C	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_d	(VFD_7SEG_SB|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_D	VFD_7SEG_d
#define	VFD_7SEG_E	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_F	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF)
#define	VFD_7SEG_G	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_H	(VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SE)
#define	VFD_7SEG_I	(VFD_7SEG_SB|VFD_7SEG_SE)
#define	VFD_7SEG_J	(VFD_7SEG_SB|VFD_7SEG_SE|VFD_7SEG_SG|VFD_7SEG_SF)
#define	VFD_7SEG_K	(VFD_7SEG_SB|VFD_7SEG_SE|VFD_7SEG_SG|VFD_7SEG_SF)
#define	VFD_7SEG_L	(VFD_7SEG_SC|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_M	(VFD_7SEG_SF|VFD_7SEG_SD|VFD_7SEG_SE)
#define	VFD_7SEG_N	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SE|VFD_7SEG_SF)
//#define	VFD_7SEG_N	(VFD_7SEG_SF|VFD_7SEG_SD|VFD_7SEG_SE)
#define	VFD_7SEG_O	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SG|VFD_7SEG_SC|VFD_7SEG_SE|VFD_7SEG_SF)
#define	VFD_7SEG_P	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF)
#define	VFD_7SEG_Q	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF)
#define	VFD_7SEG_r	(VFD_7SEG_SD|VFD_7SEG_SF)
#define	VFD_7SEG_R	VFD_7SEG_r
#define	VFD_7SEG_S	(VFD_7SEG_SA|VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_t	(VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SG)
#define	VFD_7SEG_T	VFD_7SEG_t
#define	VFD_7SEG_U	(VFD_7SEG_SC|VFD_7SEG_SB|VFD_7SEG_SF|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_V	(VFD_7SEG_SC|VFD_7SEG_SB|VFD_7SEG_SF|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_W	(VFD_7SEG_SC|VFD_7SEG_SB|VFD_7SEG_SF|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_X	(VFD_7SEG_SC|VFD_7SEG_SB|VFD_7SEG_SF|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_Y	(VFD_7SEG_SC|VFD_7SEG_SD|VFD_7SEG_SB|VFD_7SEG_SE|VFD_7SEG_SG)
#define	VFD_7SEG_Z	(VFD_7SEG_SA|VFD_7SEG_SB|VFD_7SEG_SD|VFD_7SEG_SF|VFD_7SEG_SG)


#define	VFD_7SEG__  (VFD_7SEG_SG)//_
#define	VFD_7SEG___  (VFD_7SEG_SD)//-
#define VFD_7SEG_MASK  0xff

#endif //_DISPLAY_H		  
