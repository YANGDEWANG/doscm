#ifndef _DISPLAYDEFINE_H
#define _DISPLAYDEFINE_H

#define TOTAL_SEG 			2	//可用数字显示位
#define DisplayMode			0x02				//设置为6区 16段
#define DisplayMemorySize	(DisplayMode+4)*2	//被使用的储蓄空间大小
#define KeyMemorySize		3					//按键储蓄空间大小
#define KeyScanAmount		1					//需同时处理的按键按下数目
//功能选择
//#define ShowPinPuDF	1//定义显示屏普

#define DisplaySecLoc		0	//定义“秒”的低位显示位置
#define DisplayMinuteLoc	2	//定义“分钟”的低位显示位置
#define DisplayHourLoc		4	//定义“小时”的低位显示位置
//显示码
#define StringLoc 4//定义字符显示的高位起始位置
#define NullDisplayCode			//未用显示码
#define SecDot1DisplayCode		39
#define SecDot2DisplayCode		47
#define MinuteDotDisplayCode	79

#define FMDisplayCode 		74
#define AMDisplayCode 		75
#define DVDDisplayCode 		86
#define Mp3DisplayCode 		88

//#define KhzDisplayCode 		1
//#define MhzDisplayCode 		17
//#define STDisplayCode		98
#define RightChDisplayCode 	55
#define LeftChDisplayCode 	63
#define DishwareDisplayCode {80,81,82,83,84,85}


/*
** Definition for 7-seg display
**
** we assume 7-seg are defined as traditional 7-segment LED.
**
**	 1111			
**	3    2				 
**	3    2				 
**	 4444				 
**	6    5				 
**	6    5				 
**	 7777				 
**					 
**					 

*/

//定义7段数码显示码的顶段码值(从左到右)
#define ND11 105
#define ND12 104
#define ND13 103
#define ND14 99
#define ND15 100
#if TOTAL_SEG > 5
#define ND16 101
#endif
#if TOTAL_SEG > 6
#define ND17 2
#endif
//#define NDF8
//定义7段数码显示器的最左位码值(除顶段外，顺序：从顶段开始顺时针绕，最后到中间)
#define ND21 89
#define ND31 25
#define ND41 9
#define ND51 73
#define ND61 41
#define ND71 57

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

#endif//_DISPLAYDEFINE_H

