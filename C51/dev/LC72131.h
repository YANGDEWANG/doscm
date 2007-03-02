#ifndef _LC72131_H_
#define _LC72131_H_
/*
* LC72131 PLL RM_AM/RM_FM����������
* ���������ͣ�C51,AVR
* �汾��1
* ���ڣ�2006-12-14
* ���ߣ������<yangdewang@gmail.com>

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.  Or, point your browser to
* http://www.gnu.org/copyleft/gpl.html
*/
#include "configure.h"

//#define	UseSW
enum RADIO_MODE
{
	RM_AM,
	RM_SW,
	RM_FM,
};
typedef uint8 RadioMode;
#define DefaultRadioMode		RM_FM	//Ĭ�ϲ���
#define MinFMFrequency			0		//ʵ��Ƶ�� = MinFMFrequency*100k+FMFrequencyExcursion
#define MaxFMFrequency			210		//ʵ��Ƶ�� = MinFMFrequency*100k+FMFrequencyExcursion
#define FMFrequencyExcursion	87		//RM_FM��Ƶ��ƫ�ƣ�MHZ��
#define MinAMFrequency			58		//ʵ��Ƶ�� = MinAMFrequency*9k
#define MaxAMFrequency			190		//ʵ��Ƶ�� = MaxAMFrequency*9k
#define MaxSWFrequency			0		//ʵ��Ƶ�� = MaxSWFrequency*5k
#define SeekPointCount			8		//�Զ���̨ʱ�ж�������������̽�����Ƶ��Ĵ���
#define FMLockFrequencyPatient	50	//������Ƶ���RM_FM��Hz��//��� 255
#define AMLockFrequencyPatient	80	//������Ƶ���RM_AM��Hz��//��� 255
//#define	LC72131RadioMode EepromBuffer[RadioModeStorage]
union LC72131IntputCom1
{
	_uint8 dat[3];
	struct
	{
		_uint16 P_CTR_L:8;
		_uint16 P_CTR_H:8;
		_uint8	AM_FM:	2;	//0=MW;1=RM_SW;2=RM_FM
		_uint8	CET: 	1; 	//IF counter measurement start data 1=Counter start,0=Counter reset
		_uint8	XS: 	1;	//Crystal resonator selection 0=4.5 MHz,1=7.2 MHz restart=1
		_uint8	R_CRT: 	4; 	//Reference frequency
	}LC72131IM1;
};

union LC72131IntputCom2
{
	_uint8 dat[3];
	struct
	{
		_uint8 IO_C1: 	1; 	//I/O port specification data
		_uint8 IO_C2: 	1;	//0 = input mode, 1 = output mode
		//Output port data:0 = open, 1 = low
		_uint8 IO1:	1;
		_uint8 IO2:	1;
		_uint8 BO1:	1;
		_uint8 BO2:	1;
		_uint8 BO3:	1;
		_uint8 BO4:	1;
		_uint8 DNC: 	1; 	//Don��t care. This data must be set to 0.
		_uint8 DO_C: 	3; 	//DO pin control data
		_uint8 UNLOCK:	2;	//Unlock detection data
		_uint8 DZ_C: 	2;	//Dead zone widths: DZA < DZB < DZC < DZD (0--3)
		_uint8 IF_CAT:	2;	//Determines the IF counter measurement period.
		_uint8 TBC: 	1;	//Clock time base
		_uint8 DLC: 	1;	//Charge pump control data
		_uint8 IFS: 	1;	//This data must be set 1 in normal mode.
		_uint8 TEST: 3;	//These values must all be set to 0.
	}LC72131IM2;
};

union LC72131OutputCom
{
	_uint8 dat[3];
	struct
	{	
		_uint16	IF_CTRL:	16;	//IF counter binary data
		_uint8	IF_CTRH:	4;
		_uint8	UL:			1;		//PLL unlock Latched from the state of the unlock detection circuit.
		_uint8	NC:			1;		//Mast be 0. 
		_uint8	IO1:		1;
		_uint8	IO2:		1;
	}LC72131OM;
};


#ifdef SAVE_RADIO_SETTING
#define LC72131RadioMode ((RadioMode)EepromBuffer[ESL_LC72131RadioMode])
#else
extern RadioMode	LC72131RadioMode; 
#endif//SAVE_RADIO_SETTING

extern uint8	LC72131CurrentFrequency;  	//��ǰLC72131�Ĺ���Ƶ�ʣ�ʵ��Ƶ���򲨶β�ͬ�в�ͬ�ļ��㷽�������ϣ�
extern uint8	LC72131SeekSatrtTime;//������ʼʱ��
extern uint8	CurrentSeekPoint_Radio;			//��ǰ���桰��Ƶƫ�frequencyTmp ��ƫ����
extern uint8	LC72131CurrentFrequency;  				//��ǰLC72131�Ĺ���Ƶ�ʣ�ʵ��Ƶ���򲨶β�ͬ�в�ͬ�ļ��㷽�������ϣ�
extern bool LC72131SeekFlag;//�����Զ���̨ʱ����һ����������ʱ���Զ�����
extern bool LC72131SeekWait;//Ϊһ��ʾLC72131.C���ڵȵĶ�ȡ��Ƶ����ʱ���


/*
*����Ƶ��
*���÷���ǰ�������á�LC72131CurrentFrequency����
*seek:	Ϊtrue ��������Ƶ�������ȴ�40ms����á�LC72131IsLock���ж��Ƿ���̨��
*/
extern void		LC72131SeekFrequency(bool seek);
/*
*���õ�ǰƵ��Ϊ��һ�� 
*ע�⣺�����ǰƵ�ʵ�ǰƵ���ѵ����޵��÷�������Ч��
*/
extern void LC72131NextFrequemcy();
/*
*�����Զ���̨ʱʹ�ã�����Ӧ��LC72131SeekWait��true��40ms����
*��LC72131SeekWait��true��ʱ�����Ǳ�����ϵͳʱ��LC72131SeekSatrtTime�������ʱ
*/
extern void LC72131Seek();
extern void SetRadioMode(RadioMode mode);//��������������
extern void LC72131Mute(bool of);		//���� of=false->������Ƶ����ӽ���
extern void	SetST_MO(bool ST_MO);		//����ر���������� ST_MO=trueʱ�������ر�
extern bool	GetST_MO();					//��ȡ��ǰ�������״̬ return:false =������
extern void IniLC72131();				//��ʼ��
extern uint8 LC72131IsLock();			//���ú���� RadioLocked �ж��Ƿ���̨ return����Ƶƫ�����ֵ���������ú� RadioLocked == true ʱ�����壩

//extern ulong	GetCTR();
#ifdef __AVR__
//�������ݽӿ�����
#define PIN_Readio 		PIND
#define DDR_Readio 		DDRD
#define PORT_Readio 	PORTD
#define ReadioSTB		7
#elif defined(__C51__)//end __AVR__
//PIN
#ifndef LC72131_STB_PIN
#define LC72131_STB_PIN P1^1
#endif
sbit LC72131_STB = LC72131_STB_PIN;
#endif//__C51__


#endif//_LC72131_H_
