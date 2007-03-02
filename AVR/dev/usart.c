/*
* 串口 协议
* 处理器类型：AVR
* 版本：1 日期：2007-1-16
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
#include "usart.h"
#include <avr/interrupt.h>

uint8 UsartBuffer[256];
uint16 UsartRXP;
void InitUSART(uint16 baud)
{
	//设置波特率
	UBRRH = (uint8)(baud>>8);
	UBRRL = (uint8)baud;
	// 接收器与发送器使能//开接收器与发送器中断
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);//|(1<<UDRE)|(1<<TXCIE);
	

	// 设置帧格式: 8 个数据位, 2 个停止位,偶效验
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0)|(1<<UPM1);
}
ISR(USART_RXC_vect)
{
	uint8 dat = UDR;
	if(UsartRXP!=sizeof(UsartBuffer))
	{
		UsartBuffer[UsartRXP] = dat;
		UDR = dat;
		UsartRXP++;
	}
	
}
ISR(USART_TXC_vect)
{
//	 UDR = data;
}
ISR(USART_UDRE_vect)
{
	//UDR = data;
}

