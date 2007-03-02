#ifndef _DISPLAYDEFINE_H
#define _DISPLAYDEFINE_H

#define TOTAL_SEG 			2	//可用数字显示位
#define DISPLAY_MEMORY_SIZE	4	//显示缓存字节数
#define DISPLAY_SHINE_BIT	false	//定义要点亮显示应该设置显示缓存的值为1还是0


//显示码
#define StringLoc 1//定义字符显示的高位起始位置
enum DIS_POINT//dis_point
{
	DISP_DOT1=6,
	DISP_DOT2=14,
	DISP_SR=19,
	DISP_SW,
	DISP_OK_MOD,
	DISP_CEN,
	DISP_SL,
	DISP_CD = 27,
	DISP_AUX,
	DISP_AC_3,


	DISP_NULL=18,//设为未用显示码
};
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
#define ND11 12
#define ND12 4
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
/*定义7段数码显示器的最左位码值(除顶段外，顺序：如上图示

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
#define ND21 10
#define ND31 13
#define ND41 15
#define ND51 11
#define ND61 8
#define ND71 9


#endif//_DISPLAYDEFINE_H

