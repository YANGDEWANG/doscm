/*
 * codingswitch,编码开关
 * 处理器类型：C51
 * 版本：1
 * 日期：2006-12-5
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
#include "codingswitch.h"

int8 CodingsWitchPolling()//return 0=不变；-1=减；1=加//a先高=+
{
	static bool aold,bold;	//定义了两个变量用来储蓄上一次调用此方法是编码开关两引脚的电平
	static bool st;			//定义了一个变量用来储蓄以前是否出现了两个引脚都为高电平的状态
	int8 tmp = 0;
	if(CodingsWitch_A&&CodingsWitch_A) st = 1;		//如果两个引脚都为高电平则把st置1
	if(st)					//如果st为1执行下面的步骤
	{
		if(CodingsWitch_A==0&&CodingsWitch_A==0)		//如果当前编码开关的两个引脚都为底电平执行下面的步骤
		{
			if(bold)		//如果上一次调用时编码开关b引脚为高说明编码开关在向加大的方向转
			{
				st = 0;		
				tmp++;		//设返回值为1
			}
			if(aold)		//如果上一次调用时编码开关a引脚为高说明编码开关在向减小的方向转
			{
				st = 0;
				tmp--;		//设返回值为-1
			}
		}
	}
	aold = CodingsWitch_A;				//储蓄a到aold
	bold = CodingsWitch_A;				//储蓄b到bold
	return tmp;				//返回 0=不变；-1=减；1=加
}
