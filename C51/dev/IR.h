#ifndef _IR_H_
#define _IR_H_
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
#include "configure.h"

//#define SHOWCUSTOMCODE//
//#define IR_DEBUG//
//#define IRKEY_TEST//
#define InceptCustomCode_IR     8			//DVD/VCD:00
#define FirstKeyHoldEventCyc	8
#define SecondKeyHoldEventCyc	1


#define IR_EX	EX0
#define IR_IT	IT0
#define IR_INT	INT0

#define GetKey_IR() DATA_IR[2]
extern void IniIR();			//��ʼ��IR
//extern uint8 GetKey_IR();		//��ȡ������
extern bool KeyDown_IR;			//�ֱ��ʾ�����İ���״̬
extern bool KeyHoldEvent;		//��������ס��KeyHoldEventCyc�������ڣ�110ms������һ�����ֶ�����
extern bool FirstKeyHoldEvent;	//KeyHoldEvent�Ǳ�ʾ��������ס���̵�һ�η��������ֶ�����
extern bool KeyHold_IR;			////��FirstKeyHoldEvent��һʱ��һ��һֱ����һ�μ����µİ���������ʱ�Զ�����
extern uint8 perchdata DATA_IR[4];//�������ݴ�����
#endif//_IR_H_
