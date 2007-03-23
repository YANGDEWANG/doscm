/*
* I2C 协议
* 处理器类型：C51
* 版本：1 日期：2006-12-13
* 
* 版本：2 日期：2006-12-28
* 使用版本2需定义I2C_V2
* 更改：“I2CWriteBunch”方法分开“dat”参数为数据地址“datAdd”和“dat”，去除了参数autoStop
* 
* 作者：杨德王<yangdewang@gmail.com>

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
bool waitSCL(uint8 c)//等待慢速设备,c为0等待时间最长
{
	do
	{	
		if(SCL_I2C == 1)return true;	
	}
	while(--c);
	return false;//等待超时
}
#else
#define waitSCL(x) SCL_I2C
#endif
bool waitAnswer()//等待响应//返回后SCL_I2C=1,SDA_I2C=1
{
	clSCL();//T.hd,dat延迟5us//数据保持时间//标准IIC为0us
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
		return false;//等待超时
	clSCL();
	setSDA();
	return true;
}
static bool write(uint8 add)//写一个字节到总线//失败后SCL_I2C=1,SDA_I2C=1//成功SCL_I2C=1,SDA_I2C未定
{				    //如果失败请发送停止或重新开始信号，但也请考虑失去仲裁的情况
#ifdef IIC_MULTI_MATER
	bool sdav;
#endif
	uint8 i=8;
	do
	{
		clSCL();//T.hd,dat延迟5us//数据保持时间//标准IIC为0us
#if 0
		ACC = add;
#pragma asm
		//_RLC_();
		RLC A
#pragma endasm
			add = ACC;
#else
		add=(add<<1);//注意：使用了C替换上面的代码请确保可以替换
#endif
		SDA_I2C = CY;//T.su,dat延迟250ns//数据建立时间
#ifdef IIC_MULTI_MATER
		sdav= CY;
#endif
		setSCL();//T.hgih延迟4us SCL_I2C时钟的高电平周期
		if(!waitSCL(IIC_WAITCYCLE))
			goto er;//等待超时	
#ifdef IIC_MULTI_MATER
		if(SDA_I2C != sdav)
		{
			goto er;//失去仲裁
		}
#endif
	}while(--i);
	//SCL_I2C is 1

	return true;
er:	
	//setSDA();
	return false;
}
bool read(uint8* dat)//读一个字节从总线//失败后SCL_I2C=1,SDA_I2C=1//成功SCL_I2C=0,SDA_I2C=1
{				   
	uint8 datm = 0;
	uint8 i=8;
	clSCL();//T.hd,dat延迟5us//数据保持时间//标准IIC为0us
	do
	{
		setSCL();
		if(!waitSCL(IIC_WAITCYCLE))
			goto er;//等待超时
#if 0
		CY  = SDA_I2C;//T.hgih延迟4us SCL_I2C时钟的高电平周期
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
		clSCL();//T.hd,dat延迟5us//数据保持时间//标准IIC为0us
	}while(--i);
	*dat = datm;
	return true;
er:	
	return false;
}
void IICStop() //发送停止信号
{
	clSCL();
	clSDA();
	setSCL();
	waitSCL(IIC_WAITCYCLE);//停止条件的建立时间延迟 4.7us
#if (_CPU_INSTRUCTION_CLOCK_)>(1000000/5)
	_delay_us(5);
#endif
	setSDA();
}
//寻址从机 add 地址/r_w读/写:true=read//返回后SCL_I2C=1,SDA_I2C=1
bool I2CStart(uint8 add,bool r_w)
{
#ifdef IIC_MULTI_MATER
	//todo:查询总线空闲
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
	//	Delay(0);//延迟
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
写一个字节串到指定设备 
add:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
autoStop:=true将在成功结束后产生停止信号
return:	成功写入字节数
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
读一个字节串从设备 
devAdd:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
return:	成功读入字节数
*****************************************************/
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count)
{
	if(I2CStart(devAdd,0))
	{
		write(datAdd);
		if(!waitAnswer())return 0;
		clSCL();//结束从机的响应信号
		return I2CReadStream(devAdd,dat,count);
	}
	return 0;
#if 0
	uint8 i = 0;
	if(I2CStart(devAdd,0))
	{
		write(datAdd);
		if(!waitAnswer())goto re;
		clSCL();//结束从机的响应信号
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

re:	setSCL();//不响应信号
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //发送停止信号
	return i;
#endif
}
/***************************************************
读一个字节串从设备
此方法针对不用提供数据地址的设备或连续读取
devAdd:	设备地址 
dat:	数据首字节指针 
count:	串长度 
return:	成功读入字节数
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
re:	setSCL();//不响应信号
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //发送停止信号
	return count-i;
}
#else//I2C_V2
uint8 I2CWriteBunch(uint8 add,uint8* dat,uint8 count,bool autoStop)//写一个字节串到指定设备 1:设备地址 2:数据首字节指针 3：串长度 autoStop=true:将在成功结束后产生停止信号 re：成功写入字节数
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
bool I2CWriteByte(uint8 add,uint8 dat)//写一个字节到指定设备
{
	return I2CWriteBunch(add,&dat,1,true); //todo
} 
#endif//I2C_WRITEBYTE_U
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count)//读一个字节串从设备divAdd:设备地址 datAdd:数据首字节地址 dat:数据首字节指针 count：串长度 re：成功读入字节数
{
	uint8 i = 0;
	if(I2CWriteBunch(devAdd,&datAdd,1,false))
	{
		clSCL();//结束从机的响应信号
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
re:	setSCL();//不响应信号
	waitSCL(IIC_WAITCYCLE);
	//Delay(0);
	IICStop(); //发送停止信号
	return i;
}
#endif//I2C_V2
