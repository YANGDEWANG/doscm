/*
* delay包含延时方法
* 处理器类型：C51
* 文件方法中使用了大量的goto是为了避免使用循环对_delay_100us宏生成代码的干扰
* 不要将这些方法用于精确定时，他的执行时间会比提供的参数指定的时间稍长
* 当指令周期为1us时误差大约   1.3%，随作指令周期的加大误差也将加大
* 当指令周期为0.5us时误差大约 0.6%
* 当指令周期为12us时误差大约 11.6%


* 版本：1
* 日期：2006-12-16
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
#include "delay.h"

/*
延时循环

__delay_C：
延时循环计数器，需使用无符号类型
*根据提供变量的类型不同每个循环的指令周期不同
*如果为uint8 data 每次循环使用2个指令周期
*当__delay_C为0时为最大延迟（__delay_C类型最大值*指令周期时间）
cc：
为了可以在同一个方法中多次使用此定义你必须每次使用时提供不同的cc值避免标号重复
注意：在循环内部每个循环的指令周期不能确定
*/
#define __delay_loop_(__delay_C,cc)	\
{											\
	_delay_loopF##cc:						\
	__delay_C--;							\
	if(__delay_C!=0)goto  _delay_loopF##cc;\  
}	

/*
延迟us 微秒(测试版)
us：
微秒数
*/
void _delay_us(uint8 us)
{
#if(_CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>0)
	uint8 data de = _CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1;
re:
	__delay_loop_(de,cc);//1us
	us--;
	if(us!=0)goto re;
#else
re:
	us--;
	if(us!=0)goto re;
#endif
#if (_CPU_INSTRUCTION_CLOCK_/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error 由于CPU频率的原因_delay_us的计时有误请重新设计_delay_ms。
#endif
							 
}

/*
延迟ms 毫秒
ms：
毫秒数,当ms为0时为最大延时（256ms）
*/
void _delay_ms(uint8 ms)
{
#if _CPU_INSTRUCTION_CLOCK_>=5000000
	uint8 data us10,de;
st:
	us10 = 100;
re:
	_delay_loop(de,_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1,cc);//10us
	//因为每次循环“if(us10!=0)goto re;”使用了两个周期说一上面计算出_delay_loop循环数后又-1
	us10--;
	if(us10!=0)goto re;
	ms--;
	if(ms!=0)goto st;
#if (_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1<1)||(_CPU_INSTRUCTION_CLOCK_*10/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error 由于CPU频率的原因_delay_ms的计时有误请重新设计_delay_ms。
#endif
#else
	uint8 data us100,de;
st:
	us100 = 10;
re:
	_delay_loop(de,_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1,cc);//_delay_100us(de,1);//100us
	//因为每次循环“if(us100!=0)goto re;”使用了两个周期说一上面计算出_delay_loop循环数后又-1
	us100--;
	if(us100!=0)goto re;
	ms--;
	if(ms!=0)goto st;
#if (_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1<1)||(_CPU_INSTRUCTION_CLOCK_*100/(1000000*_DELAY_LOOP_IN_UINT8_IC_)-1>255)
#error 由于CPU频率的原因_delay_ms的计时有误请重新设计_delay_ms。
#endif
#endif
}
