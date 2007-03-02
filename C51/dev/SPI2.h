#ifndef _SPI2_H_
#define _SPI2_H_
/*
 * SPI 协议 只有你的项目需要两个SPI口才使用此文件
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
#include "configure.h"

/*===========SPI2===========*/
//#define SPI2_U			1	//使用SPI2口
//#define SPI2_Master_U	1	//使用SPI2口主机模式
//#define SPI2_MasterMultibyte_U	1	//使用SPI2口主机多字节传输模式
//#define SPI2_SR_U		1	//使用SPI2口从机模式


/*===========VSPI2===========*/
#define VSPI2			1	//使用虚拟SPI2口
#ifdef VSPI2
//#define VSPI2_MSB_U		1	//使用MSB
//#define VSPI2_LSB_U		1	//使用LSB
//#define VSPI2_SCK_NORMAL_L	1	//定义使SCK信号线常态为低
/*如果要同步使用虚拟SPI2口需定义它，这将导致一个传输过程结束前其他传输请求必须等待(实现进程同步)*/
//#define VSPI2_SYNCHRO	1	

//SPI2模式,四选一
//#define VSPI2_MODE_0 //起始沿采样 ( 上升沿) 结束沿设置 ( 下降沿) SCK常态0
//#define VSPI2_MODE_1 //起始沿设置 ( 上升沿) 结束沿采样 ( 下降沿) SCK常态0
//#define VSPI2_MODE_2 //起始沿采样 ( 下降沿) 结束沿设置 ( 上升沿) SCK常态1
//#define VSPI2_MODE_3 //起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) SCK常态1

#if defined(VSPI2_MODE_0)||defined(VSPI2_MODE_1)
#define  VSPI2_START_RISE//起始沿上升沿
#endif
#if defined(VSPI2_MODE_1)||defined(VSPI2_MODE_3)
#define  VSPI2_START_SET//起始沿设置
#endif
#if 0//下面代码已移到configure.h
#ifndef SDA_VSPI2_PIN
#define SDA_VSPI2_PIN P1^0
#endif
#ifndef SCK_VSPI2_PIN
#define SCK_VSPI2_PIN P1^1
#endif

//定义数据接口引脚
sbit SDA_VSPI2 = SDA_VSPI2_PIN;
sbit SCK_VSPI2 = SCK_VSPI2_PIN;
#endif
#ifdef VSPI2_SYNCHRO
extern volatile bool VSPI2_Transmiting;//传输过程中为1，不要写他
extern void InVSPI2(void);			//开始使用时调用
static void __inline__ OutVSPI2(void)//结束使用时调用
{
	VSPI2_Transmiting = false;
}
#endif//VSPI2_SYNCHRO
extern void VSPI2_MasterInit(void);

#ifdef VSPI2_LSB_U
extern uint8 VSPI2_MasterTransmitLSB(uint8 cData);//传送
#endif//VSPI2_LSB_U
#ifdef VSPI2_MSB_U
extern uint8 VSPI2_MasterTransmitMSB(uint8 cData);
#endif//VSPI2_MSB_U
#endif	//VSPI2
#endif	//_SPI2_H_
