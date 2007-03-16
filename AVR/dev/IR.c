/*
* IR，红外遥控接收
* 处理器类型：AVR
* 版本：1
* 日期：2007-1-24
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
#include <avr/interrupt.h>
#include "ir.h"
#include "Clock.h"
#include "boolean.h"

#define irPORT GPORT(IR_PORT)
#define irDDR  GDDR(IR_PORT)
#define irPIN  GPIN(IR_PORT)

enum IR_STATE
{
	IR_IDLE,	//空闲
	IR_ST,		//起始高
	IR_Incept,//
};
uint8 IRState = IR_IDLE;		//当前接收状态
uint8 IRInceptBitCount;	//已经收到的比特数
uint8 perchdata IRData[4];		//接收数据储蓄区
u8 IrKey;//按键码

bool IRKeyDown;					//接收完数据时置位
bool IRKeyHold;					//检测到重复信号置位
uint8 IRoldTime;
#if 1//下降沿中断
ISR(IR_INTERRUPT)
{
	uint8 IRTime = SysClick;
	uint8 timespan = IRTime-IRoldTime;
	if(timespan>15000u/CLICK_CYCLE_US)//超时退出13.5
	{	
		IRState = IR_IDLE;
	}
	switch(IRState)
	{
	case IR_IDLE:
		{
			IRState = IR_ST;
			IRInceptBitCount = 0;
			IRData[0]=0;
			IRData[1]=0;
			IRData[2]=0;
			IRData[3]=0;
			break;
		}
	case IR_ST:
		{
			if((timespan)>12375u/CLICK_CYCLE_US)//13.5ms起始
			{
				IRState = IR_Incept;
			}
			else if((timespan)>10000u/CLICK_CYCLE_US)//11.25 ms重复
			{
				IRKeyHold = true;
				IRState = IR_IDLE;
			}
			break;
		}
	case IR_Incept:
		{
			if((timespan)>1687u/CLICK_CYCLE_US)//2.25 ms为1；1.125 ms为0
			{
				//IRData[IRInceptBitCount/8]|=1<<(IRInceptBitCount%8);
				SetBit(IRData,IRInceptBitCount);
			}
			IRInceptBitCount++;
			if(IRInceptBitCount>31)
			{
				if(IRData[0]==IR_CUSTOMCODE
					&&(u8)IRData[0]==(u8)(~IRData[1])
					&&(u8)IRData[2]==(u8)(~IRData[3]))
				{
#ifdef SHOWCUSTOMCODE
					ShowUINT8(DATA_IR[0]);
#endif//SHOWCUSTOMCODE
#ifdef IRKEY_TEST
					ShowUINT8(GetKey_IR());
#endif//SHOWCUSTOMCODE

					IrKey = IRData[2];
					IRKeyDown = true;
				}
				/*else
				{
				IRInceptBitCount++;
				}*/
				IRState = IR_IDLE;
			}
			break;
		}
	}


	IRoldTime = IRTime;
}


#else
ISR(IR_INTERRUPT)
{
	uint8 IRTime = SysClick;
	uint8 timespan = IRTime-IRoldTime;
	if(irPIN&(1<<IR_P))//引脚为高电平
	{
		//switch(IRState)
		//{
		//case IR_ST:
		//	{
		//		if(timespan<8000u/CLICK_CYCLE_US&&timespan>10000u/CLICK_CYCLE_US)//不是9ms起始退出
		//		{	
		//			IRState = IR_IDLE;
		//		}
		//		break;
		//	}				
		//}	
	}
	else
	{
		if(timespan>10000u/CLICK_CYCLE_US)//超时退出
		{	
			IRState = IR_IDLE;
		}
		switch(IRState)
		{
		case IR_IDLE:
			{
				IRState = IR_ST;
				IRInceptBitCount = 0;
				IRData[0]=0;
				IRData[1]=0;
				IRData[2]=0;
				IRData[3]=0;
				break;
			}
		case IR_ST:
			{
				if((timespan)>4000u/CLICK_CYCLE_US)//4.5ms起始
				{
					IRState = IR_Incept;
				}
				else if((timespan)>2000u/CLICK_CYCLE_US)//2.25ms重复
				{
					IRKeyHold = true;
					IRState = IR_IDLE;
				}
				break;
			}
		case IR_Incept:
			{
				if((timespan)>1000u/CLICK_CYCLE_US)//1690us为1；560us为0
				{
					//IRData[IRInceptBitCount/8]|=1<<(IRInceptBitCount%8);
					SetBit(IRData,IRInceptBitCount);
				}
				IRInceptBitCount++;
				if(IRInceptBitCount>31)
				{
					if(IRData[0]==IR_CUSTOMCODE
						&&IRData[0]==(u8)(~IRData[1])
						&&IRData[2]==(u8)(~IRData[3]))
					{
#ifdef SHOWCUSTOMCODE
						ShowUINT8(DATA_IR[0]);
#endif//SHOWCUSTOMCODE
#ifdef IRKEY_TEST
						ShowUINT8(GetKey_IR());
#endif//SHOWCUSTOMCODE

						IrKey = IRData[2];
						IRKeyDown = true;
					}
					/*else
					{
					IRInceptBitCount++;
					}*/
					IRState = IR_IDLE;
				}
				break;
			}
		}

	}
	IRoldTime = IRTime;
}
void IniIR(void)	 //初始化接收
{
	//DDRD  &=~(1<<DDD2);	//使能上拉电阻
	//PORTD |= (1<<PD2);	//使能上拉电阻
	//配置为任意电平变化中断
	MCUCR |= (1<<ISC00);
	MCUCR &=~(1<<ISC01);
	//GIFR   = (1<<INTF0);
	GICR  |= (1<<INT0);	//开外部中断0
}

#endif


