#ifndef _READIO_H_
#define _READIO_H_
#include "configure.h"


//#define	UseSW 

#define FMStationStorageCount 10
#define AMStationStorageCount 10

#ifdef SAVE_RADIO_SETTING
#define	FMStation ((uint8)EepromBuffer[ESL_FMStation])//��ǰFM��̨
#define	AMStation ((uint8)EepromBuffer[ESL_AMStation])//��ǰAM��̨
#else
extern uint8 FMStation;//��ǰFM��̨
extern uint8 AMStation;//��ǰAM��̨
#endif//SAVE_RADIO_SETTING

extern bool RadioAutoSeekFlag;	//������̨�Զ�ɨ�豻��һ����������ʱ���Զ�����


void	IniRadio(void);
void	SetRadioStation(uint8 st);	//ѡ���̨
//extern void		NextRadioStation(char offset);	//SetRadioStation(��ǰ��̨+offset)
void	SaveRadioStation(void);				//����Ƶ�ʵ���ǰ��̨
void	RadioAutoSeekStart(void);			//��ʼ�Զ���̨��������õ�̨
void	LC72131SeekStart(bool SeekWay);//��ʼ��̨ SeekWay == true ��> ���Ƶ����
void	RadioSeekStop(void);				//ֹͣ��̨
void	DriveRadio(void);					//����Radio����Ҫ�ظ���������̨

/*
*����Ƶ��
*���÷���ǰ�������á�LC72131CurrentFrequency����			   isrs.NLine
*seek:	Ϊtrue ��������Ƶ�������ȴ�40ms����á�IsLock���ж��Ƿ���̨��
*/
//extern void		SeekFrequency(bool seek);		
//extern ulong GetCTR();
extern void 	SetRadioMode(uint8 mode);		//��������������
//UINTextern void 	LC72131Mute(bool of);		//���� of=false->������Ƶ����ӽ���
#endif//_READIO_H_

