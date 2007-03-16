#ifndef _IIC_H_
#define _IIC_H_
/*
* I2C 协议
* 处理器类型：AVR
* 版本：1 日期：2007-1-23
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
#ifndef I2C_CFG
//-------------------------I2C配置-------------------------------//
#define I2C_CFG
#define I2C_SHARE_SDA_PIN	//定义使SDA引脚可以共享
#define I2C_ENABLE_PULL		//定义使能内部上拉电阻
#define F_I2CSCL	100000	//TWI时钟为Hz
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
//-----------------------I2C配置结束-----------------------------//
#endif//I2C_CFG
/***************************************************
执行初始化
*****************************************************/
void IniIIC();
#ifdef I2C_SHARE_SDA_PIN
/***************************************************
首先等待IIC传输数据完成然后关闭IIC释放DAT引脚
在需要公用DAT脚是使用此方法
调用方法后SCK脚被置为输出低电平避免I2C上产生错误信号
要重新使用IIC需调用IniIIC
注：如果使用I2CWriteStream，方法将自动调用IniIIC
*****************************************************/
void IICDisable();
#endif//I2C_SHARE_SDA_PIN
/***************************************************
写一个字节串到指定设备 
add:	设备地址 
dat:	数据首字节指针 
count:	串长度 
*****************************************************/
void I2CWriteStream(uint8 add,uint8* dat,uint8 count);

#if 0
/***************************************************
写一个字节串到指定设备 
add:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
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
#endif//0

#endif//_IIC_H_
