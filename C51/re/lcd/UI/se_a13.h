#ifndef _DISPLAYDEFINE_H
#define _DISPLAYDEFINE_H

#define TOTAL_SEG 			2	//����������ʾλ
#define DISPLAY_MEMORY_SIZE	4	//��ʾ�����ֽ���
#define DISPLAY_SHINE_BIT	false	//����Ҫ������ʾӦ��������ʾ�����ֵΪ1����0


//��ʾ��
#define StringLoc 1//�����ַ���ʾ�ĸ�λ��ʼλ��
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


	DISP_NULL=18,//��Ϊδ����ʾ��
};
#define NullDisplayCode			//δ����ʾ��
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

//����7��������ʾ��Ķ�����ֵ(������)
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
/*����7��������ʾ��������λ��ֵ(�������⣬˳������ͼʾ

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

