#ifndef _IIC2_H_
#define _IIC2_H_
/*
* I2C Э�� ֻ�������Ŀ��Ҫ����I2C�ڲ�ʹ�ô��ļ�
* ���������ͣ�C51
* �汾��1
* ���ڣ�2006-12-13
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

//#define IIC_MULTI_MATER
//#define I2C_WRITEBYTE_U
//�������ݽӿ�����
#ifndef SDA_I2C_PIN
#define SDA_I2C_PIN (P1^0)
#endif//SDA_I2C_PIN
#ifndef SCL_I2C_PIN
#define SCL_I2C_PIN (P1^1)
#endif//SDA_I2C_PIN

sbit SDA_I2C=SDA_I2C_PIN;
sbit SCL_I2C=SCL_I2C_PIN;
#define WaitCycle 0 //����ȴ������豸��ʱ�� time=(WaitCycle*8+5)*ָ������ʱ��


extern bool 		I2CStart(uint8,bool);			//Ѱַ�ӻ�1:��ַ2:��/д:true=read re:true=�ɹ�
#ifdef I2C_WRITEBYTE_U
extern bool 		I2CWriteByte(uint8,uint8);			//дһ���ֽڵ�ָ���豸 1:�豸��ַ2:���� re:true=�ɹ�
#endif//I2C_WRITEBYTE_U
//дһ���ֽڴ���ָ���豸 1:�豸��ַ 2:�������ֽ�ָ�� 3�������� autoStop=true:���ڳɹ����������ֹͣ�ź� re���ɹ�д���ֽ���
extern uint8	I2CWriteBunch(uint8 add,uint8* dat,uint8 count,bool autoStop);
//extern bool 		ReadByte(uint8 add,uint8* dat);	//��һ���ֽڴ��豸 1:�豸��ַ2:���� re:true=�ɹ�
//��һ���ֽڴ����豸divAdd:�豸��ַ datAdd:�������ֽڵ�ַ dat:�������ֽ�ָ�� count�������� re���ɹ������ֽ���
extern uint8	I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count);

#endif//_IIC2_H_
