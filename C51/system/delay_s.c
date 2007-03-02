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
延迟s 秒
s：
毫秒数,当s为0时为最大延时（256s）
*/
void _delay_s(uint8 s)
{
	uint8 ms250;
st:
	ms250 = 4;
re:
	_delay_ms(250);
	ms250--;
	if(ms250!=0)goto re;
	s--;
	if(s!=0)goto st;
}