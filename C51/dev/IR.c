/*
 * IR������ң�ؽ���
 * ���������ͣ�C51
 * �汾��1
 * ���ڣ�2006-12-3
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
#include <global.h>
#include "ir.h"


enum IR_STATE
{
	Idle_IR,		//���� 		�ߵ�ƽ
	LeaderL_IR	,	//�����ź� 	�͵�ƽ
	LeaderH_IR,	//�����ź� 	�ߵ�ƽ
	CodeH_IR,		//���ݸߵ�ƽ�ڼ�//����PT2222.pdf
	CodeL_IR,		//���ݵ͵�ƽ�ڼ�
	Incept_IR,		//���ݽ�����
	KeyDowning_IR,	//��⵽������ס�ź�
};
enum IR_DELAY_TIME
{
	IR_DELAY1_5MS,//��ʱ1.5ms
	IR_DELAY4_5MS,//��ʱ4.5ms
	IR_DELAY7_5MS,//��ʱ7.5ms
};
//51,4M ÿ����3us
uint8 State_IR=Idle_IR;			//��ǰ����״̬
static uint8 InceptBitCount=0;	//�Ѿ��յ��ı�����
//static uint8 hold_tmp;			//��ʱ�������ڽ��timer�޷���ʱ����110ms����
uint8 perchdata DATA_IR[4];				//�������ݴ�����
uint8 KeyHoldEventCyc	= FirstKeyHoldEventCyc;
uint8 keyHoldCount		= 0;
static bool DAT_bit_IR;			//�����жϵ�ǰ���ص�0/1״̬?
bool KeyDown_IR;					//��ʾ�����İ���״̬
bool KeyHold_IR;					//��FirstKeyHoldEvent��һʱ��һ��һֱ����һ�μ����µİ���������ʱ�Զ�����
bool KeyHoldEvent;				//��������ס��KeyHoldEventCyc�������ڣ�110ms������һ�����ֶ�����
bool FirstKeyHoldEvent;			//KeyHoldEvent�Ǳ�ʾ��������ס���̵�һ�η��������ֶ�����

#ifdef IR_DEBUG
static void showIR()  using 0
{
	if(InceptBitCount==33)
		ShowUINT8(GetKey_IR());
	//else
	//ShowUINT8(InceptBitCount);
}
#endif//IR_DEBUG
void IniIR()	 //��ʼ������
{

	IR_IT = 1;  //�½�����Ч
	IR_EX = 1;//���ⲿ�ж�1
}


static void SetBitIR() using 1
{
	//SetBit(InceptBitCount-1,DAT_bit_IR,DATA_IR);
	uint8 BAdrr = (InceptBitCount-1)/8,bAdrr = (InceptBitCount-1)%8;
	uint8 DATA = DATA_IR[BAdrr];
	if(DAT_bit_IR)
	{
		DATA_IR[BAdrr] = _crol_(1,bAdrr)|DATA;
	}
	else
	{
		DATA_IR[BAdrr] = _crol_(~1,bAdrr)&DATA;
	}
}
static void IniTimer1(uint8 flg) using 1//��ʼ��timer
{
	TMOD	= (T0_MASK_&TMOD)|T1_M0_;//ģʽ1
	switch(flg)
	{
	case IR_DELAY1_5MS:
		{

			TH1 = (0xffff-(15*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(15*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	case IR_DELAY4_5MS:
		{
			TH1 = (0xffff-(45*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(45*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	case IR_DELAY7_5MS:
		{
			TH1 = (0xffff-(75*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(75*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	}
	TR1		 = 1;
	ET1		 = 1;
}
void Time1 (void) interrupt 3  using 1
{
	
	TR1	= 0;//ͣ����
	switch (State_IR)
	{
	case LeaderL_IR:
		{
			if(IR_INT)
			{
				State_IR = 	Idle_IR;
			}
			else
			{
				State_IR = 	LeaderH_IR;
				goto NEXT4_5;
				//IniTimer1(IR_DELAY4_5MS);//��ʱ4.5ms//osc 16.9344M/E732
			}
			break;
		}
	case LeaderH_IR:
		{
			if(IR_INT)
			{
				InceptBitCount	=	0;
				DAT_bit_IR	=	true;
				State_IR = Incept_IR;
				goto NEXT4_5;
				//IniTimer1(IR_DELAY4_5MS);//��ʱ4.5ms//osc 16.9344M/E732
			}
			else
			{
				State_IR = 	Idle_IR;
			}
			break;
		}
	case Incept_IR:
		{
			if(DAT_bit_IR)//����ȴ�4.5ms��IR_EX��δ��'DAT_bit_IR',���˳�����״̬
			{
				State_IR = 	Idle_IR;
				break;
			}
			DAT_bit_IR = 1;
			goto NEXT4_5;
			//IniTimer1(IR_DELAY4_5MS);//��ʱ4.5ms//osc 16.9344M/E732
			
			break;
		}
	}
	return;
NEXT4_5:
	IniTimer1(IR_DELAY4_5MS);//��ʱ4.5ms
} 
void IRinterrupt(void) interrupt 0  using 1
{
//DAT_bit_IR = 1;
//InceptBitCount = 0;
//SetBitIR();
	switch (State_IR)
	{
	case Idle_IR:
		{
			State_IR = LeaderL_IR;
			IniTimer1(IR_DELAY7_5MS);//��ʱ7.5ms//osc 16.9344M/D3E7
			break;
		}
	case LeaderH_IR://��ס�ź�(9ms���������4.5ms���ٴη����ж�)
		{
			State_IR	=	Idle_IR;
			keyHoldCount++;
			if(keyHoldCount>KeyHoldEventCyc)
			{
				keyHoldCount = 0;
				KeyHoldEvent = 1;
				if(KeyHoldEventCyc ==  FirstKeyHoldEventCyc)
				{
					//KeyHold_IR	=	1;
					FirstKeyHoldEvent = 1;
					KeyHoldEventCyc = SecondKeyHoldEventCyc;
				}
				/*else
				{
				FirstKeyHoldEvent = 0;
				}*/
			}
			/*State_IR	=	KeyDowning_IR;
			hold_tmp	=	0;
			IniTimer1(0,0);//��ʱ110ms���'KeyDown_IR'//osc 16.9344M/D3E7
			*/
			break;
		}
		/*case CustomCode_IR:
		{
		DAT_bit_IR = 0;
		InceptBitCount	=	0;
		State_IR = Incept_IR;
		IniTimer1(0xF7,0xC0);//��ʱ1.5ms//osc 16.9344M/F7C0
		break;
		}*/
	case Incept_IR:
		{
			IniTimer1(IR_DELAY1_5MS);//��ʱ1.5ms//osc 16.9344M/F7C0
			if(InceptBitCount>0)
			{
				//SetBit(InceptBitCount-1,DAT_bit_IR,DATA_IR);
				SetBitIR();
			}
			DAT_bit_IR = 0;					//����ȴ������'1'timer1���Ὣ����λ
			if(InceptBitCount==32)
			{	
				State_IR = Idle_IR;
				keyHoldCount	= 0;
				KeyHoldEventCyc = FirstKeyHoldEventCyc;
				//uint8 a =  DATA_IR[0];
				//uint8 b = ~DATA_IR[1];
				//uint8 c =  DATA_IR[2];
				//uint8 d = ~DATA_IR[3];
				if((DATA_IR[0]==~DATA_IR[1])
					&&DATA_IR[2]==~DATA_IR[3]
#ifndef SHOWCUSTOMCODE
				&&(DATA_IR[0]==InceptCustomCode_IR)
#endif//SHOWCUSTOMCODE
					)
				{
					KeyDown_IR = true;
				}
				//KeyDown_IR = true;
			}
			InceptBitCount++;
			break;

			/*bytec = InceptBitCount/8; 
			tmp	  = DATA_IR[bytec]>>1;
			if(DAT_bit_IR)
			tmp  |= 0x80;
			DATA_IR[bytec]	=	tmp;
			if(InceptBitCount==31)
			{
			State_IR = KeyDowning_IR;
			if((DATA_IR[2] == (~DATA_IR[3]))&&(DATA_IR[0] == (~DATA_IR[1]))&&DATA_IR[0]==SystemCode_IR)
			KeyDown_IR = true;
			keyHoldCount = 0;
			hold_tmp	=	0;
			KeyHoldEventCyc = FirstKeyHoldEventCyc;
			IniTimer1(0,0);//��ʱ110ms���'KeyDown_IR'//osc 16.9344M/D3E7
			}
			InceptBitCount++;
			DAT_bit_IR = 0;//����ȴ������'1'timer1���Ὣ����λ
			break;
			*/

		}
	}
	#ifdef IR_DEBUG
showIR();
#endif// IR_DEBUG
}
