/*
* IR������ң�ؽ���
* ���������ͣ�AVR
* �汾��1
* ���ڣ�2007-1-24
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
#include <avr/interrupt.h>
#include "ir.h"
#include "Clock.h"
#include "boolean.h"

#define irPORT GPORT(IR_PORT)
#define irDDR  GDDR(IR_PORT)
#define irPIN  GPIN(IR_PORT)

enum IR_STATE
{
	IR_IDLE,	//����
	IR_ST,		//��ʼ��
	IR_Incept,//
};
uint8 IRState = IR_IDLE;		//��ǰ����״̬
uint8 IRInceptBitCount;	//�Ѿ��յ��ı�����
uint8 perchdata IRData[4];		//�������ݴ�����

bool IRKeyDown;					//����������ʱ��λ
bool IRKeyHold;					//��⵽�ظ��ź���λ
uint8 IRoldTime;
ISR(IR_INTERRUPT)
{
	uint8 IRTime = SysClick;
	uint8 timespan = IRTime-IRoldTime;
	if(irPIN&(1<<IR_P))//����Ϊ�ߵ�ƽ
	{
		switch(IRState)
		{
		case IR_ST:
			{
				if(timespan<8000u/CLICK_CYCLE_US&&timespan>10000u/CLICK_CYCLE_US)//����9ms��ʼ�˳�
				{	
					IRState = IR_IDLE;
				}
				break;
			}				
		}	
	}
	else
	{
		switch(IRState)
		{
		case IR_IDLE:
			{
				IRState = IR_ST;
				break;
			}
		case IR_ST:
			{
				if((timespan)>4000u/CLICK_CYCLE_US)//4.5ms��ʼ
				{
					IRState = IR_Incept;
					IRInceptBitCount = 0;
				}
				else if((timespan)>2000u/CLICK_CYCLE_US)//2.25ms�ظ�
				{
					IRKeyHold = true;
				}
				break;
			}
		case IR_Incept:
			{
				if((timespan)>1000u/CLICK_CYCLE_US)//1690usΪ1��560usΪ0
				{
					//IRData[IRInceptBitCount/8]|=1<<(IRInceptBitCount%8);
					SetBit(IRData,IRInceptBitCount,true);
				}
				IRInceptBitCount++;
				if(IRInceptBitCount>31)
				{
					IRKeyDown = true;
					IRState = IR_IDLE;
				}
				break;
			}
		}

	}
	IRoldTime = IRTime;
}
void IniIR(void)	 //��ʼ������
{
	//DDRD  &=~(1<<DDD2);	//ʹ����������
	//PORTD |= (1<<PD2);	//ʹ����������
	//����Ϊ�����ƽ�仯�ж�
	MCUCR |= (1<<ISC00);
	MCUCR &=~(1<<ISC01);
	//GIFR   = (1<<INTF0);
	GICR  |= (1<<INT0);	//���ⲿ�ж�0
}