#ifndef _IR_H_
#define _IR_H_
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
#include "configure.h"

//#define SHOWCUSTOMCODE//
//#define IR_DEBUG//
//#define IRKEY_TEST//
#ifndef IR_CFG
//--------------IR����-----------------//
#define IR_CFG
#define IR_CUSTOMCODE     8			//DVD/VCD:00
#define IR_INTERRUPT 	INT0_vect
#define IR_PORT	D
#define IR_P	2
//------------IR���ý���---------------//
#endif//IR_CFG

static inline void IniIR(void)	 //��ʼ������
{
	//DDRD  &=~(1<<DDD2);	//ʹ����������
	//PORTD |= (1<<PD2);	//ʹ����������
	//����Ϊ�½����ж�
	MCUCR |= (1<<ISC01);
//	MCUCR &=~(1<<ISC00);
	//GIFR   = (1<<INTF0);
	GICR  |= (1<<INT0);	//���ⲿ�ж�0
}
extern bool IRKeyDown;					//����������ʱ��λ
extern bool IRKeyHold;					//��⵽�ظ��ź���λ
extern u8 IrKey;//������
//�������ݴ�����
//0:�û���
//1:�û�����
//2:����
//3:������
extern uint8 perchdata IRData[4];		
#define GetKey_IR() IRData[2]

#endif//_IR_H_
