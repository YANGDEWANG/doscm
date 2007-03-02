/*
 * IR，红外遥控接收
 * 处理器类型：C51
 * 版本：1
 * 日期：2006-12-3
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
#include "ir.h"


enum IR_STATE
{
	Idle_IR,		//空闲 		高电平
	LeaderL_IR	,	//导入信号 	低电平
	LeaderH_IR,	//导入信号 	高电平
	CodeH_IR,		//数据高电平期间//参照PT2222.pdf
	CodeL_IR,		//数据低电平期间
	Incept_IR,		//数据接收中
	KeyDowning_IR,	//检测到按键按住信号
};
enum IR_DELAY_TIME
{
	IR_DELAY1_5MS,//延时1.5ms
	IR_DELAY4_5MS,//延时4.5ms
	IR_DELAY7_5MS,//延时7.5ms
};
//51,4M 每周期3us
uint8 State_IR=Idle_IR;			//当前接收状态
static uint8 InceptBitCount=0;	//已经收到的比特数
//static uint8 hold_tmp;			//临时数据用于解决timer无法计时超过110ms问题
uint8 perchdata DATA_IR[4];				//接收数据储蓄区
uint8 KeyHoldEventCyc	= FirstKeyHoldEventCyc;
uint8 keyHoldCount		= 0;
static bool DAT_bit_IR;			//用于判断当前比特的0/1状态?
bool KeyDown_IR;					//表示按键的按下状态
bool KeyHold_IR;					//当FirstKeyHoldEvent置一时置一，一直到下一次检测的新的按键码输入时自动清零
bool KeyHoldEvent;				//当按键按住“KeyHoldEventCyc”个周期（110ms）后被置一，须手动清零
bool FirstKeyHoldEvent;			//KeyHoldEvent是表示当按键按住过程第一次发生，须手动清零

#ifdef IR_DEBUG
static void showIR()  using 0
{
	if(InceptBitCount==33)
		ShowUINT8(GetKey_IR());
	//else
	//ShowUINT8(InceptBitCount);
}
#endif//IR_DEBUG
void IniIR()	 //初始化接收
{

	IR_IT = 1;  //下降沿有效
	IR_EX = 1;//开外部中断1
}


static void SetBitIR() using 1
{
	//SetBit(InceptBitCount-1,DAT_bit_IR,DATA_IR);
	uint8 BAdrr = (InceptBitCount-1)/8,bAdrr = (InceptBitCount-1)%8;
	uint8 DATA = DATA_IR[BAdrr];
	if(DAT_bit_IR)
	{
		DATA_IR[BAdrr] = _crol_(1,bAdrr)|DATA;
	}
	else
	{
		DATA_IR[BAdrr] = _crol_(~1,bAdrr)&DATA;
	}
}
static void IniTimer1(uint8 flg) using 1//初始化timer
{
	TMOD	= (T0_MASK_&TMOD)|T1_M0_;//模式1
	switch(flg)
	{
	case IR_DELAY1_5MS:
		{

			TH1 = (0xffff-(15*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(15*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	case IR_DELAY4_5MS:
		{
			TH1 = (0xffff-(45*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(45*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	case IR_DELAY7_5MS:
		{
			TH1 = (0xffff-(75*_CPU_INSTRUCTION_CLOCK_/10000))>>8;
			TL1 = (0xffff-(75*_CPU_INSTRUCTION_CLOCK_/10000));
			break;
		}
	}
	TR1		 = 1;
	ET1		 = 1;
}
void Time1 (void) interrupt 3  using 1
{
	
	TR1	= 0;//停计数
	switch (State_IR)
	{
	case LeaderL_IR:
		{
			if(IR_INT)
			{
				State_IR = 	Idle_IR;
			}
			else
			{
				State_IR = 	LeaderH_IR;
				goto NEXT4_5;
				//IniTimer1(IR_DELAY4_5MS);//延时4.5ms//osc 16.9344M/E732
			}
			break;
		}
	case LeaderH_IR:
		{
			if(IR_INT)
			{
				InceptBitCount	=	0;
				DAT_bit_IR	=	true;
				State_IR = Incept_IR;
				goto NEXT4_5;
				//IniTimer1(IR_DELAY4_5MS);//延时4.5ms//osc 16.9344M/E732
			}
			else
			{
				State_IR = 	Idle_IR;
			}
			break;
		}
	case Incept_IR:
		{
			if(DAT_bit_IR)//如果等待4.5ms后IR_EX仍未清'DAT_bit_IR',则退出接收状态
			{
				State_IR = 	Idle_IR;
				break;
			}
			DAT_bit_IR = 1;
			goto NEXT4_5;
			//IniTimer1(IR_DELAY4_5MS);//延时4.5ms//osc 16.9344M/E732
			
			break;
		}
	}
	return;
NEXT4_5:
	IniTimer1(IR_DELAY4_5MS);//延时4.5ms
} 
void IRinterrupt(void) interrupt 0  using 1
{
//DAT_bit_IR = 1;
//InceptBitCount = 0;
//SetBitIR();
	switch (State_IR)
	{
	case Idle_IR:
		{
			State_IR = LeaderL_IR;
			IniTimer1(IR_DELAY7_5MS);//延时7.5ms//osc 16.9344M/D3E7
			break;
		}
	case LeaderH_IR://按住信号(9ms导入脉冲后4.5ms内再次发生中断)
		{
			State_IR	=	Idle_IR;
			keyHoldCount++;
			if(keyHoldCount>KeyHoldEventCyc)
			{
				keyHoldCount = 0;
				KeyHoldEvent = 1;
				if(KeyHoldEventCyc ==  FirstKeyHoldEventCyc)
				{
					//KeyHold_IR	=	1;
					FirstKeyHoldEvent = 1;
					KeyHoldEventCyc = SecondKeyHoldEventCyc;
				}
				/*else
				{
				FirstKeyHoldEvent = 0;
				}*/
			}
			/*State_IR	=	KeyDowning_IR;
			hold_tmp	=	0;
			IniTimer1(0,0);//延时110ms清除'KeyDown_IR'//osc 16.9344M/D3E7
			*/
			break;
		}
		/*case CustomCode_IR:
		{
		DAT_bit_IR = 0;
		InceptBitCount	=	0;
		State_IR = Incept_IR;
		IniTimer1(0xF7,0xC0);//延时1.5ms//osc 16.9344M/F7C0
		break;
		}*/
	case Incept_IR:
		{
			IniTimer1(IR_DELAY1_5MS);//延时1.5ms//osc 16.9344M/F7C0
			if(InceptBitCount>0)
			{
				//SetBit(InceptBitCount-1,DAT_bit_IR,DATA_IR);
				SetBitIR();
			}
			DAT_bit_IR = 0;					//清零等待如果是'1'timer1将会将其置位
			if(InceptBitCount==32)
			{	
				State_IR = Idle_IR;
				keyHoldCount	= 0;
				KeyHoldEventCyc = FirstKeyHoldEventCyc;
				//uint8 a =  DATA_IR[0];
				//uint8 b = ~DATA_IR[1];
				//uint8 c =  DATA_IR[2];
				//uint8 d = ~DATA_IR[3];
				if((DATA_IR[0]==~DATA_IR[1])
					&&DATA_IR[2]==~DATA_IR[3]
#ifndef SHOWCUSTOMCODE
				&&(DATA_IR[0]==InceptCustomCode_IR)
#endif//SHOWCUSTOMCODE
					)
				{
					KeyDown_IR = true;
				}
				//KeyDown_IR = true;
			}
			InceptBitCount++;
			break;

			/*bytec = InceptBitCount/8; 
			tmp	  = DATA_IR[bytec]>>1;
			if(DAT_bit_IR)
			tmp  |= 0x80;
			DATA_IR[bytec]	=	tmp;
			if(InceptBitCount==31)
			{
			State_IR = KeyDowning_IR;
			if((DATA_IR[2] == (~DATA_IR[3]))&&(DATA_IR[0] == (~DATA_IR[1]))&&DATA_IR[0]==SystemCode_IR)
			KeyDown_IR = true;
			keyHoldCount = 0;
			hold_tmp	=	0;
			KeyHoldEventCyc = FirstKeyHoldEventCyc;
			IniTimer1(0,0);//延时110ms清除'KeyDown_IR'//osc 16.9344M/D3E7
			}
			InceptBitCount++;
			DAT_bit_IR = 0;//清零等待如果是'1'timer1将会将其置位
			break;
			*/

		}
	}
	#ifdef IR_DEBUG
showIR();
#endif// IR_DEBUG
}
