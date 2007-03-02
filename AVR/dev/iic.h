#ifndef _IIC_H_
#define _IIC_H_
/*
* I2C Э��
* ���������ͣ�AVR
* �汾��1 ���ڣ�2007-1-23
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

#include <configure.h>
#ifndef I2C_CFG
//-------------------------I2C����-------------------------------//
#define I2C_CFG
#define I2C_SHARE_SDA_PIN	//����ʹSDA���ſ��Թ���
#define I2C_ENABLE_PULL		//����ʹ���ڲ���������
#define F_I2CSCL	100000	//TWIʱ��ΪHz
//pin def
#ifdef __AVR_ATmega16__
#define I2C_POPT	C
#define	I2C_SDA		1
#define	I2C_SCK		0
#elif defined __AVR_ATmega8__
#define I2C_POPT	C
#define	I2C_SDA		4
#define	I2C_SCK		5
#endif
//-----------------------I2C���ý���-----------------------------//
#endif//I2C_CFG
/***************************************************
ִ�г�ʼ��
*****************************************************/
void IniIIC();
#ifdef I2C_SHARE_SDA_PIN
/***************************************************
���ȵȴ�IIC�����������Ȼ��ر�IIC�ͷ�DAT����
����Ҫ����DAT����ʹ�ô˷���
���÷�����SCK�ű���Ϊ����͵�ƽ����I2C�ϲ��������ź�
Ҫ����ʹ��IIC�����IniIIC
ע�����ʹ��I2CWriteStream���������Զ�����IniIIC
*****************************************************/
void IICDisable();
#endif//I2C_SHARE_SDA_PIN
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
*****************************************************/
void I2CWriteStream(uint8 add,uint8* dat,uint8 count);

#if 0
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ�д���ֽ���
*****************************************************/
uint8 I2CWriteBunch(uint8 add,uint8 datAdd,uint8* dat,uint8 count);

/***************************************************
��һ���ֽڴ����豸
�˷�����Բ����ṩ���ݵ�ַ���豸��������ȡ
devAdd:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadStream(uint8 devAdd,uint8* dat,uint8 count);

/***************************************************
��һ���ֽڴ����豸 
devAdd:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count);
#endif//0

#endif//_IIC_H_
