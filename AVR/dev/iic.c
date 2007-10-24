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
#include <global.h>
#include "dev\iic.h"
#include <util/twi.h>
#include <avr/interrupt.h>
//#define IIC_DEBUG
//struct IIC_STATE IICStatebits;

#define S_start()	{TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWSTA);IICState.Error = false;}
#define S_stop()	{TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);IICState.cStop = false;IICState.IICBusy = false;}
#define S_ack()		(TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA))
#define S_nack()	(TWCR =(1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define WaitIICIdle() {while(IICState.IICBusy);}

//TWCR只能IN/OUT,直接赋值比逻辑运算(|= &=)更节省空间


//预分频系数=1(TWPS=0)
#if ((F_CPU/F_I2CSCL-16)/8)<10
#define TWBR_SET 			(10)					//TWBR必须大于等于10
#else
#define TWBR_SET  			((F_CPU/F_I2CSCL-16)/8)//计算TWBR值
#endif

#define TW_START				0x08	//已发送START,加载SLA+W
#define TW_RESTART				0x10	//重复START 已发送
#define TW_MT_ARB_LOST			0x38	//SLA+W或SLA+R 或数据的仲裁失败

//主机发送机
#define TW_MT_SLA_W_ACK			0x18	//已发送SLA_W,接收到ACK；加载数据( 字节)
#define TW_MT_SLA_W_NACK		0x20	//已发送SLA_W,接收到NACK
#define TW_MT_DATA_ACK			0x28	//数据已发送,接收到ACK
#define TW_MT_DATA_NACK			0x30	//数据已发送,接收到NACK
//主机接收机
#define TW_MR_SLA_R_ACK			0x40	//已发送SLA_R,接收到ACK；加载数据( 字节)
#define TW_MR_SLA_R_NACK		0x48	//已发送SLA_R,接收到NACK
#define TW_MR_DATA_ACK			0x50	//接收数据,ACK 已返回
#define TW_MR_DATA_NACK			0x58	//接收数据,NACK 已返回
//在从机接收模式	
#define TW_SR_SLA_ACK           0x60	//自己的SLA+W 已经被接收ACK 已返回
#define TW_SR_ARB_LOST_SLA_ACK  0x68	//SLA+R/W 作为主机的仲裁失败；自己的SLA+W 已经被接收ACK 已返回
#define TW_SR_GCALL_ACK         0x70	//接收到广播地址 ACK 已返回
#define TW_SR_ARB_LOST_GCALL_ACK 0x78	//SLA+R/W 作为主机的仲裁失败；接收到广播地址ACK 已返回
#define TW_SR_DATA_ACK          0x80	//以前以自己的 SLA+W 被寻址；数据已经被接收ACK 已返回
#define TW_SR_DATA_NACK         0x88	//以前以自己的 SLA+W 被寻址数据已经被接收NOT ACK 已返回
#define TW_SR_GCALL_DATA_ACK    0x90	//以前以广播方式被寻址；数据已经被接收ACK 已返回
#define TW_SR_GCALL_DATA_NACK   0x98	//以前以广播方式被寻址；数据已经被接收NOT ACK 已返回
#define TW_SR_STOP              0xA0	//在以从机工作时接收到STOP或重复START
//从机发送模式
#define TW_ST_SLA_ACK           0xA8	//自己的SLA+R 已经被接收ACK 已返回
#define TW_ST_ARB_LOST_SLA_ACK  0xB0	//SLA+R/W 作为主机的仲裁失败；自己的SLA+R 已经被接收ACK 已返回
#define TW_ST_DATA_ACK          0xB8	//TWDR 里数据已经发送接收到ACK
#define TW_ST_DATA_NACK         0xC0	//TWDR 里数据已经发送接收到NOT ACK
#define TW_ST_LAST_DATA         0xC8	//TWDR 的一字节数据已经发送(TWAE = “0”);接收到ACK
/* Misc */
#define TW_NO_INFO              0xF8	//没有相关的状态信息；TWINT = “0”
#define TW_BUS_ERROR            0x00	//由于非法的START 或STOP 引起的总线错误

#define TW_MT_TWSR_STATUS_MASK	0xF8
#define TW_MT_TWCR_CMD_MASK		0x0F

#define IICPORT GPORT(I2C_POPT)
#define IICPIN  GPIN(I2C_POPT)
#define IICDDR  GDDR(I2C_POPT)

volatile IICStatebits IICState;
//bool volatile IICBusy;
static uint8* dataPtr;
static uint8  srAdd;
#define dataCount IICState.CountWaitToSend
//volatile static uint8  dataCount;
void IniIIC()
{
	IICDDR  &= ~(_BV(I2C_SCK)|_BV(I2C_SDA));
#ifdef I2C_ENABLE_PULL
	IICPORT |= _BV(I2C_SCK)|_BV(I2C_SDA);
#else
	IICPORT &= ~(_BV(I2C_SCK)|_BV(I2C_SDA));
#endif

	TWSR=0;									//预分频=0^4=1
	TWAR=0x00;									//主机模式，该地址无效
	TWCR=0x00;									//关闭TWI模块
	IICState.IICBusy = false;
	TWBR=TWBR_SET;
}
#ifdef I2C_SHARE_SDA_PIN
/***************************************************
首先等待IIC传输数据完成然后关闭IIC释放DAT引脚
在需要公用DAT脚是使用此方法
调用方法后SCK脚被置为输出低电平避免I2C上产生错误信号
要重新使用IIC需调用IniIIC
注：如果使用I2CWriteStream，方法将自动调用IniIIC
*****************************************************/
void IICDisable()
{
#ifndef AVRSIMULATOR
	while(IICBusy);
#endif//AVRSIMULATOR
	IICPORT &= ~(_BV(I2C_SCK));
	IICDDR  |= _BV(I2C_SCK);
	TWCR=0x00;		//关闭TWI模块
}
#endif//I2C_SHARE_SDA_PIN
/***************************************************
写一个字节串到指定设备 
add:	设备地址 
dat:	数据首字节指针 
count:	串长度 
*****************************************************/
void I2CWriteStream(uint8 devAdd,uint8* dat,uint8 count)
{
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.IICBusy = true;
	srAdd	= devAdd<<1;
	dataPtr = dat;
	dataCount = count;
	//if(TWCR&(1<<TWEN))
	//{
	//	while(!(TWCR&(1<<TWINT)));//等待stop发送完成
	//}
	S_start();
}
#if 0

/***************************************************
写一个字节串到指定设备 
add:	设备地址 
datAdd:	数据地址
dat:	数据首字节指针 
count:	串长度 
return:	成功写入字节数
*****************************************************/
uint8 I2CWriteBunch(uint8 add,uint8 datAdd,uint8* dat,uint8 count)
{
	return 0;
}




#endif//0
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
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.IICBusy = true;
	srAdd	= (devAdd<<1)+1;
	dataPtr = dat;
	dataCount = count;
	S_start();
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	return count-dataCount;
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
#ifdef I2C_SHARE_SDA_PIN
	IniIIC();
#endif//I2C_SHARE_SDA_PIN
	IICState.cStop=true;
	I2CWriteStream(devAdd,&datAdd,1);

#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	if(IICState.Error)
		return 0;
	IICState.IICBusy = true;
	srAdd	= (devAdd<<1)+1;
	dataPtr = dat;
	dataCount = count;
	S_start();
#ifndef AVRSIMULATOR
	WaitIICIdle();
#endif//AVRSIMULATOR
	return count-dataCount;
}
ISR(SIG_2WIRE_SERIAL)
{

	switch(TWSR & TW_MT_TWSR_STATUS_MASK)
	{
	case TW_MT_ARB_LOST://SLA+W或SLA+R 或数据的仲裁失败
		{
			S_start();
			break;
		}
	case TW_RESTART:	//重复START 已发送		
	case TW_START:		//已发送START,加载SLA+W
		{
			TWDR = srAdd;
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
			break;
		}
#ifdef IIC_DEBUG
	case TW_MT_DATA_NACK:	//数据已发送,接收到NACK		
	case TW_MT_SLA_W_NACK:	//已发送SLA_W,接收到NACK
#endif//IIC_DEBUG
	case TW_MT_DATA_ACK:	//数据已发送,接收到ACK		
	case TW_MT_SLA_W_ACK:	//已发送SLA_W,接收到ACK；加载数据( 字节)
		{
			if(dataCount)
			{
				dataCount--;
				TWDR = *dataPtr++;
				TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
			}
			else
			{
				if(!IICState.cStop)
				{
					S_stop();
				}
				else
				{
					//S_nack();
				}
				IICState.cStop = false;
				IICState.IICBusy = false;//TODO:

			}
			break;
		}
	case TW_MR_SLA_R_ACK://已发送SLA_R,接收到ACK；加载数据( 字节)
		{
			if(dataCount>1)
			{
				S_ack();
			}
			else
			{
				S_nack();
			}
			break;
		}
	case TW_MR_DATA_ACK://接收数据,ACK 已返回
		{
			dataCount--;
			*dataPtr++=TWDR;
			if(dataCount>1)
			{
				S_ack();
			}
			else
			{
				S_nack();
			}
			break;
		}
	case TW_MR_DATA_NACK://接收数据,NACK 已返回
		{
			dataCount--;
			*dataPtr++=TWDR;
			S_stop();
			//IICState.IICBusy = false;//TODO:
			break;
		}
#ifndef IIC_DEBUG

	case TW_MR_SLA_R_NACK://已发送SLA_R,接收到NACK
	case TW_MT_DATA_NACK:	//数据已发送,接收到NACK		
	case TW_MT_SLA_W_NACK:	//已发送SLA_W,接收到NACK	
		{
			S_stop();
			IICState.Error = true;
			//IICState.IICBusy = false;
#ifdef ErrorCodeShow
			ShowErrorCode(ErrorCode_TW_MT_NACK);
#endif
			break;
		}
#endif//IIC_DEBUG
	case TW_BUS_ERROR:	//由于非法的START 或STOP 引起的总线错误
		{
			S_stop();
			IICState.Error = true;
			//IICState.IICBusy = false;
			break;
		}
		/*default:
		{
		S_stop();
		IICBusy = false;
		break;
		}*/
	}
}
