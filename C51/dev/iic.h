#ifndef _IIC_H_
#define _IIC_H_
/*
* I2C Э��
* ���������ͣ�C51
* �汾��1 ���ڣ�2006-12-13
* 
* �汾��2 ���ڣ�2006-12-28
* ʹ�ð汾2�趨��I2C_V2
* ���ģ���I2CWriteBunch�������ֿ���dat������Ϊ���ݵ�ַ��datAdd���͡�dat����ȥ���˲���autoStop
* 
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

#ifndef I2C_CGF

//-------------I2C_CGF----------------//
#define I2C_CGF
#define I2C_V2
//#define IIC_MULTI_MATER
//#define I2C_WRITEBYTE_U
//�������ݽӿ�����
#define SDA_I2C_PIN (P0^0)
#define SCL_I2C_PIN (P0^1)
#define IIC_WAITCYCLE 1 //����ȴ������豸��ʱ�� 

//-------------I2C_CGF----------------//

#endif//I2C_CGF

sbit SDA_I2C=SDA_I2C_PIN;
sbit SCL_I2C=SCL_I2C_PIN;
extern bool I2CStart(uint8,bool);			//Ѱַ�ӻ�1:��ַ2:��/д:true=read re:true=�ɹ�


#ifdef I2C_V2
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ�д���ֽ���
*****************************************************/
uint8 I2CWriteStream(uint8 add,uint8* dat,uint8 count);
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
autoStop:=true���ڳɹ����������ֹͣ�ź�
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


#else//I2C_V2
#ifdef I2C_WRITEBYTE_U
extern bool 		I2CWriteByte(uint8,uint8);			//дһ���ֽڵ�ָ���豸 1:�豸��ַ2:���� re:true=�ɹ�
#endif//I2C_WRITEBYTE_U
//дһ���ֽڴ���ָ���豸 1:�豸��ַ 2:�������ֽ�ָ�� 3�������� autoStop=true:���ڳɹ����������ֹͣ�ź� re���ɹ�д���ֽ���
extern uint8	I2CWriteBunch(uint8 add,uint8* dat,uint8 count,bool autoStop);
//extern bool 		ReadByte(uint8 add,uint8* dat);	//��һ���ֽڴ��豸 1:�豸��ַ2:���� re:true=�ɹ�
//��һ���ֽڴ����豸divAdd:�豸��ַ datAdd:�������ֽڵ�ַ dat:�������ֽ�ָ�� count�������� re���ɹ������ֽ���
extern uint8	I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count);
#endif//I2C_V2

#endif//_IIC_H_
