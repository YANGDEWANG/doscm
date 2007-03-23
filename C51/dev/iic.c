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
#include <intrins.h>
#include <global.h>
#include "iic.h"
#include "delay.h"

//#define DEBUG_IIC

#define clSCL()		(SCL_I2C = false)
#define setSCL()	(SCL_I2C = true)
#define clSDA()		(SDA_I2C = false)
#define setSDA()	(SDA_I2C = true)
/*
void clSCL()		{SCL_I2C = false;}
void setSCL()	{SCL_I2C = true;}
void clSDA()		{SDA_I2C = false;}
void setSDA()	{SDA_I2C = true;}*/
#if (IIC_WAITCYCLE>0)
bool waitSCL(uint8 c)//�ȴ������豸,cΪ0�ȴ�ʱ���
{
	do
	{	
		if(SCL_I2C == 1)return true;	
	}
	while(--c);
	return false;//�ȴ���ʱ
}
#else
#define waitSCL(x) SCL_I2C
#endif
bool waitAnswer()//�ȴ���Ӧ//���غ�SCL_I2C=1,SDA_I2C=1
{
	clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	//Delay(0);
	setSDA();
	setSCL();
	if(waitSCL(IIC_WAITCYCLE))
	{
		if(SDA_I2C == 0)
			return true;
	}
	return false;
}
bool sendAnswer()
{
	clSDA();
	setSCL();
	if(!waitSCL(IIC_WAITCYCLE))
		return false;//�ȴ���ʱ
	clSCL();
	setSDA();
	return true;
}
static bool write(uint8 add)//дһ���ֽڵ�����//ʧ�ܺ�SCL_I2C=1,SDA_I2C=1//�ɹ�SCL_I2C=1,SDA_I2Cδ��
{				    //���ʧ���뷢��ֹͣ�����¿�ʼ�źţ���Ҳ�뿼��ʧȥ�ٲõ����
#ifdef IIC_MULTI_MATER
	bool sdav;
#endif
	uint8 i=8;
	do
	{
		clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
#if 0
		ACC = add;
#pragma asm
		//_RLC_();
		RLC A
#pragma endasm
			add = ACC;
#else
		add=(add<<1);//ע�⣺ʹ����C�滻����Ĵ�����ȷ�������滻
#endif
		SDA_I2C = CY;//T.su,dat�ӳ�250ns//���ݽ���ʱ��
#ifdef IIC_MULTI_MATER
		sdav= CY;
#endif
		setSCL();//T.hgih�ӳ�4us SCL_I2Cʱ�ӵĸߵ�ƽ����
		if(!waitSCL(IIC_WAITCYCLE))
			goto er;//�ȴ���ʱ	
#ifdef IIC_MULTI_MATER
		if(SDA_I2C != sdav)
		{
			goto er;//ʧȥ�ٲ�
		}
#endif
	}while(--i);
	//SCL_I2C is 1

	return true;
er:	
	//setSDA();
	return false;
}
bool read(uint8* dat)//��һ���ֽڴ�����//ʧ�ܺ�SCL_I2C=1,SDA_I2C=1//�ɹ�SCL_I2C=0,SDA_I2C=1
{				   
	uint8 datm = 0;
	uint8 i=8;
	clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	do
	{
		setSCL();
		if(!waitSCL(IIC_WAITCYCLE))
			goto er;//�ȴ���ʱ
#if 0
		CY  = SDA_I2C;//T.hgih�ӳ�4us SCL_I2Cʱ�ӵĸߵ�ƽ����
		ACC = datm;
#pragma asm
		//_RLC_();
		RLC A
#pragma endasm
			datm= ACC;
#else
		datm=datm<<1;
		if(SDA_I2C)datm++;
#endif
		clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	}while(--i);
	*dat = datm;
	return true;
er:	
	return false;
}
void IICStop() //����ֹͣ�ź�
{
	clSCL();
	clSDA();
	setSCL();
	waitSCL(IIC_WAITCYCLE);//ֹͣ�����Ľ���ʱ���ӳ� 4.7us
#if (_CPU_INSTRUCTION_CLOCK_)>(1000000/5)
	_delay_us(5);
#endif
	setSDA();
}
//Ѱַ�ӻ� add ��ַ/r_w��/д:true=read//���غ�SCL_I2C=1,SDA_I2C=1
bool I2CStart(uint8 add,bool r_w)
{
#ifdef IIC_MULTI_MATER
	//todo:��ѯ���߿���
#endif
	setSDA();
	setSCL();//Start Setup Time 4.7us
#if (_CPU_INSTRUCTION_CLOCK_)>(1000000/5)
	_delay_us(5);
#endif
	clSDA();
#if 0
	CY 	= r_w;
	ACC = add;
#pragma asm
	//_RLC_();
	RLC A
#pragma endasm
		add = ACC;
#else
	add=add<<1;
	if(r_w)add++;
#endif
	//	Delay(0);//�ӳ�
	/*if(!write(add))
	{  
	setSDA();
	setSCL();
	return false;
	}*/

	return write(add)&&waitAnswer();
}
#ifdef I2C_V2
/***************************************************
дһ���ֽڴ���ָ���豸 
add:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
autoStop:=true���ڳɹ����������ֹͣ�ź�
return:	�ɹ�д���ֽ���
*****************************************************/
uint8 I2CWriteBunch(uint8 add,uint8 datAdd,uint8* dat,uint8 count)
{
	uint8 i=0;
	if(I2CStart(add,0))
	{
		write(datAdd);
		if(!waitAnswer())goto end;
		for(;i<count;i++)
			if(!write(dat[i])||!waitAnswer())
				goto end;
	}
end:
	IICStop();	
	return i;
}
/***************************************************
��һ���ֽڴ����豸 
devAdd:	�豸��ַ 
datAdd:	���ݵ�ַ
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count)
{
	if(I2CStart(devAdd,0))
	{
		write(datAdd);
		if(!waitAnswer())return 0;
		clSCL();//�����ӻ�����Ӧ�ź�
		return I2CReadStream(devAdd,dat,count);
	}
	return 0;
#if 0
	uint8 i = 0;
	if(I2CStart(devAdd,0))
	{
		write(datAdd);
		if(!waitAnswer())goto re;
		clSCL();//�����ӻ�����Ӧ�ź�
		if(I2CStart(devAdd,1))
		{
			while(i<count-1)
			{
				if(!read(dat)||!sendAnswer())
				{
					goto re;
				}
				dat++;
				i++;
			}
			if(read(dat))i++;
		}
	}

re:	setSCL();//����Ӧ�ź�
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //����ֹͣ�ź�
	return i;
#endif
}
/***************************************************
��һ���ֽڴ����豸
�˷�����Բ����ṩ���ݵ�ַ���豸��������ȡ
devAdd:	�豸��ַ 
dat:	�������ֽ�ָ�� 
count:	������ 
return:	�ɹ������ֽ���
*****************************************************/
uint8 I2CReadStream(uint8 devAdd,uint8* dat,uint8 count)
{
	uint8 i;
	if(count==0)
		return 0;
	i= count-1;
	if(I2CStart(devAdd,1))
	{
		while(i)
		{
			if(!read(dat)||!sendAnswer())
			{
				goto re;
			}
			dat++;
			i--;
		}
		if(!read(dat))i++;
	}
re:	setSCL();//����Ӧ�ź�
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //����ֹͣ�ź�
	return count-i;
}
#else//I2C_V2
uint8 I2CWriteBunch(uint8 add,uint8* dat,uint8 count,bool autoStop)//дһ���ֽڴ���ָ���豸 1:�豸��ַ 2:�������ֽ�ָ�� 3�������� autoStop=true:���ڳɹ����������ֹͣ�ź� re���ɹ�д���ֽ���
{
	uint8 i=0;
	if(I2CStart(add,0))
	{
		for(;i<count;i++)
		{
			if(!write(dat[i]))
				goto end;
			if(!waitAnswer())
				goto end;
		} 
	}
	else
	{ 
		goto rt;
	}					
end:
	if(autoStop)
		IICStop();	
rt:
	return i;
}

#ifdef I2C_WRITEBYTE_U
bool I2CWriteByte(uint8 add,uint8 dat)//дһ���ֽڵ�ָ���豸
{
	return I2CWriteBunch(add,&dat,1,true); //todo
} 
#endif//I2C_WRITEBYTE_U
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count)//��һ���ֽڴ����豸divAdd:�豸��ַ datAdd:�������ֽڵ�ַ dat:�������ֽ�ָ�� count�������� re���ɹ������ֽ���
{
	uint8 i = 0;
	if(I2CWriteBunch(devAdd,&datAdd,1,false))
	{
		clSCL();//�����ӻ�����Ӧ�ź�
		if(I2CStart(devAdd,1))
		{
			while(i<count-1)
			{
				if(!read(dat)||!sendAnswer())
				{
					goto re;
				}
				dat++;
				i++;
			}
			if(read(dat))i++;
		}
	}
re:	setSCL();//����Ӧ�ź�
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //����ֹͣ�ź�
	return i;
}
#endif//I2C_V2
