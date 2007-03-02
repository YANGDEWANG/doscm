#ifndef _DISPLAYDEFINE_H
#define _DISPLAYDEFINE_H

#define TOTAL_SEG 			2	//����������ʾλ
#define DisplayMode			0x02				//����Ϊ6�� 16��
#define DisplayMemorySize	(DisplayMode+4)*2	//��ʹ�õĴ���ռ��С
#define KeyMemorySize		3					//��������ռ��С
#define KeyScanAmount		1					//��ͬʱ����İ���������Ŀ
//����ѡ��
//#define ShowPinPuDF	1//������ʾ����

#define DisplaySecLoc		0	//���塰�롱�ĵ�λ��ʾλ��
#define DisplayMinuteLoc	2	//���塰���ӡ��ĵ�λ��ʾλ��
#define DisplayHourLoc		4	//���塰Сʱ���ĵ�λ��ʾλ��
//��ʾ��
#define StringLoc 4//�����ַ���ʾ�ĸ�λ��ʼλ��
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
//����7��������ʾ��������λ��ֵ(�������⣬˳�򣺴Ӷ��ο�ʼ˳ʱ���ƣ�����м�)
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

