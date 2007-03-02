#ifndef _SPI2_H_
#define _SPI2_H_
/*
 * SPI Э�� ֻ�������Ŀ��Ҫ����SPI�ڲ�ʹ�ô��ļ�
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

/*===========SPI2===========*/
//#define SPI2_U			1	//ʹ��SPI2��
//#define SPI2_Master_U	1	//ʹ��SPI2������ģʽ
//#define SPI2_MasterMultibyte_U	1	//ʹ��SPI2���������ֽڴ���ģʽ
//#define SPI2_SR_U		1	//ʹ��SPI2�ڴӻ�ģʽ


/*===========VSPI2===========*/
#define VSPI2			1	//ʹ������SPI2��
#ifdef VSPI2
//#define VSPI2_MSB_U		1	//ʹ��MSB
//#define VSPI2_LSB_U		1	//ʹ��LSB
//#define VSPI2_SCK_NORMAL_L	1	//����ʹSCK�ź��߳�̬Ϊ��
/*���Ҫͬ��ʹ������SPI2���趨�������⽫����һ��������̽���ǰ���������������ȴ�(ʵ�ֽ���ͬ��)*/
//#define VSPI2_SYNCHRO	1	

//SPI2ģʽ,��ѡһ
//#define VSPI2_MODE_0 //��ʼ�ز��� ( ������) ���������� ( �½���) SCK��̬0
//#define VSPI2_MODE_1 //��ʼ������ ( ������) �����ز��� ( �½���) SCK��̬0
//#define VSPI2_MODE_2 //��ʼ�ز��� ( �½���) ���������� ( ������) SCK��̬1
//#define VSPI2_MODE_3 //��ʼ������ ( �½���) �����ز��� ( ������) SCK��̬1

#if defined(VSPI2_MODE_0)||defined(VSPI2_MODE_1)
#define  VSPI2_START_RISE//��ʼ��������
#endif
#if defined(VSPI2_MODE_1)||defined(VSPI2_MODE_3)
#define  VSPI2_START_SET//��ʼ������
#endif
#if 0//����������Ƶ�configure.h
#ifndef SDA_VSPI2_PIN
#define SDA_VSPI2_PIN P1^0
#endif
#ifndef SCK_VSPI2_PIN
#define SCK_VSPI2_PIN P1^1
#endif

//�������ݽӿ�����
sbit SDA_VSPI2 = SDA_VSPI2_PIN;
sbit SCK_VSPI2 = SCK_VSPI2_PIN;
#endif
#ifdef VSPI2_SYNCHRO
extern volatile bool VSPI2_Transmiting;//���������Ϊ1����Ҫд��
extern void InVSPI2(void);			//��ʼʹ��ʱ����
static void __inline__ OutVSPI2(void)//����ʹ��ʱ����
{
	VSPI2_Transmiting = false;
}
#endif//VSPI2_SYNCHRO
extern void VSPI2_MasterInit(void);

#ifdef VSPI2_LSB_U
extern uint8 VSPI2_MasterTransmitLSB(uint8 cData);//����
#endif//VSPI2_LSB_U
#ifdef VSPI2_MSB_U
extern uint8 VSPI2_MasterTransmitMSB(uint8 cData);
#endif//VSPI2_MSB_U
#endif	//VSPI2
#endif	//_SPI2_H_
