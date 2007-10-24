#ifndef _IIC_H_
#define _IIC_H_
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

#include <configure.h>

#ifndef I2C_CGF

//-------------I2C_CGF----------------//
#define I2C_CGF
#define I2C_V2
//#define IIC_MULTI_MATER
//#define I2C_WRITEBYTE_U
//定义数据接口引脚
#define SDA_I2C_PIN (P0^0)
#define SCL_I2C_PIN (P0^1)
#define IIC_WAITCYCLE 1 //定义等待慢速设备的时间 

//-------------I2C_CGF----------------//

#endif//I2C_CGF

sbit SDA_I2C=SDA_I2C_PIN;
sbit SCL_I2C=SCL_I2C_PIN;
extern bool I2CStart(uint8,bool);			//寻址从机1:地址2:读/写:true=read re:true=成功


#ifdef I2C_V2
/***************************************************
写一个字节串到指定设备 
add:	设备地址 
dat:	数据首字节指针 
count:	串长度 
return:	成功写入字节数
*****************************************************/
uint8 I2CWriteStream(uint8 add,uint8* dat,uint8 count);
/***************************************************
写一个字节串到指定设备 
add:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
autoStop:=true将在成功结束后产生停止信号
return:	成功写入字节数
*****************************************************/
uint8 I2CWriteBunch(uint8 add,uint8 datAdd,uint8* dat,uint8 count);

/***************************************************
读一个字节串从设备
此方法针对不用提供数据地址的设备或连续读取
devAdd:	设备地址 
dat:	数据首字节指针 
count:	串长度 
return:	成功读入字节数
*****************************************************/
uint8 I2CReadStream(uint8 devAdd,uint8* dat,uint8 count);

/***************************************************
读一个字节串从设备 
devAdd:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
return:	成功读入字节数
*****************************************************/
uint8 I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count);


#else//I2C_V2
#ifdef I2C_WRITEBYTE_U
extern bool 		I2CWriteByte(uint8,uint8);			//写一个字节到指定设备 1:设备地址2:数据 re:true=成功
#endif//I2C_WRITEBYTE_U
//写一个字节串到指定设备 1:设备地址 2:数据首字节指针 3：串长度 autoStop=true:将在成功结束后产生停止信号 re：成功写入字节数
extern uint8	I2CWriteBunch(uint8 add,uint8* dat,uint8 count,bool autoStop);
//extern bool 		ReadByte(uint8 add,uint8* dat);	//读一个字节从设备 1:设备地址2:数据 re:true=成功
//读一个字节串从设备divAdd:设备地址 datAdd:数据首字节地址 dat:数据首字节指针 count：串长度 re：成功读入字节数
extern uint8	I2CReadBunch(uint8 devAdd,uint8 datAdd,uint8* dat,uint8 count);
#endif//I2C_V2

#endif//_IIC_H_
