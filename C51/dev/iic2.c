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
bool waitSCL(uint8 c)//�ȴ������豸,���11ָ������ time=(WaitCycle*8+5)*ָ������ʱ��
{
Wait:			//�ȴ������豸
	if(SCL_I2C == 1)
	{
		return true;	
	}
	else
	{
		if(c == 0)
		{
			return false;//�ȴ���ʱ
		}
		c--;
		goto Wait;
	}
	return false;//�ȴ���ʱ
}
bool waitAnswer()//�ȴ���Ӧ//���غ�SCL_I2C=1,SDA_I2C=1
{
	clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	//Delay(0);
	setSDA();
	setSCL();
	if(waitSCL(WaitCycle))
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
	if(!waitSCL(WaitCycle))
		return false;//�ȴ���ʱ
	clSCL();
	setSDA();
	return true;
}
/*����Ϊ���
bool sendNOAnswer()
{
setSDA();
setSCL();
if(!waitSCL(WaitCycle))
return false;//�ȴ���ʱ
clSCL();
setSDA();
return true;
}*/
static bool write(uint8 add)//дһ���ֽڵ�����//ʧ�ܺ�SCL_I2C=1,SDA_I2C=1//�ɹ�SCL_I2C=1,SDA_I2Cδ��
{				    //���ʧ���뷢��ֹͣ�����¿�ʼ�źţ���Ҳ�뿼��ʧȥ�ٲõ����
	bool sdav;
	uint8 i;
	for(i=0;i<8;i++)
	{
		clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
		ACC = add;
		_RLC_();
		add = ACC;
		//_nop_();
		//_nop_();
		SDA_I2C = CY;//T.su,dat�ӳ�250ns//���ݽ���ʱ��
		sdav= CY;
		setSCL();//T.hgih�ӳ�4us SCL_I2Cʱ�ӵĸߵ�ƽ����
		if(!waitSCL(WaitCycle))
			goto er;//�ȴ���ʱ	
		if(SDA_I2C != sdav)
		{
			goto er;//ʧȥ�ٲ�
		}
	}
	//SCL_I2C is 1

	return true;
er:	
	setSDA();
	return false;
}
bool read(uint8* dat)//��һ���ֽڴ�����//ʧ�ܺ�SCL_I2C=1,SDA_I2C=1//�ɹ�SCL_I2C=0,SDA_I2C=1
{				   
	uint8 datm = 0;
	uint8 i;
	clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	for(i=0;i<8;i++)
	{
		setSCL();
		if(!waitSCL(WaitCycle))
			goto er;//�ȴ���ʱ	
		CY  = SDA_I2C;//T.hgih�ӳ�4us SCL_I2Cʱ�ӵĸߵ�ƽ����
		ACC = datm;
		_RLC_();
		datm= ACC;
		clSCL();//T.hd,dat�ӳ�5us//���ݱ���ʱ��//��׼IICΪ0us
	}
	*dat = datm;
	return true;
er:	
	return false;
}
void IICStop() //����ֹͣ�ź�
{
#if (_CPU_INSTRUCTION_CLOCK_)>(1000000/4)
	uint8 del;
#endif
	clSCL();
	clSDA();
	setSCL();
	waitSCL(WaitCycle);//ֹͣ�����Ľ���ʱ���ӳ�4us
#if (_CPU_INSTRUCTION_CLOCK_)>(1000000/4)
	_delay_loop(del,(_CPU_INSTRUCTION_CLOCK_)/(1000000/4)/2+1,2);
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
	setSCL();
	clSDA();//�ӳ�4us
	CY 	= r_w;
	ACC = add;
	_RLC_();
	add = ACC;
	//	Delay(0);//�ӳ�
	if(!write(add))
	{  
		setSDA();
		setSCL();
		return false;
	}
	if(waitAnswer())
		return true;
	IICStop();
	return false;
}
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
	waitSCL(WaitCycle);
		//Delay(0);
	IICStop(); //����ֹͣ�ź�
	return i;
}

