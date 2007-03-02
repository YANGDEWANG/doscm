#ifndef _VAD_H_
#define _VAD_H_
/*
 * VAD，一个低分辨率的用IO口做的AD
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
#include <configure.h>

#define VAD_MAX	(70) 


#ifndef VAD_IN_PIN
#define VAD_IN_PIN  (P1^0)
#endif
#ifndef VAD_OUT_PIN
#define VAD_OUT_PIN (P1^1)
#endif

sbit VAD_IN		= VAD_IN_PIN;
sbit VAD_OUT	= VAD_OUT_PIN;
extern uint8 VADGet();
#define	IniVAD() (VAD_IN=true)
#endif//_VAD_H_
