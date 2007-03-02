#ifndef _SPI_H_
#define _SPI_H_
/*
 * SPI 协议
 * 处理器类型：C51，AVR
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

/*===========SPI===========*/
//#define SPI_U			1	//使用SPI口
//#define SPI_MASTER_U	1	//使用SPI口主机模式
//#define SPI_MasterMultibyte_U	1	//使用SPI口主机多字节传输模式
//#define SPI_SR_U		1	//使用SPI口从机模式

#ifdef SPI_U
//SPI速度等级
#define SPI_FOSC_4		(0<<SPR1)|(0<<SPR0)//震荡频率除4
#define SPI_FOSC_16		(0<<SPR1)|(1<<SPR0)//震荡频率除16
#define SPI_FOSC_64		(1<<SPR1)|(0<<SPR0)//震荡频率除64
#define SPI_FOSC_128	(1<<SPR1)|(1<<SPR0)//震荡频率除128

//SPI模式
#define SPI_Mode_0 (0<<CPOL)|(0<<CPHA)//CPOL = 0, CPHA = 0 起始沿采样 ( 上升沿) 结束沿设置 ( 下降沿) 0
#define SPI_Mode_1 (0<<CPOL)|(1<<CPHA)//CPOL = 0, CPHA = 1 起始沿设置 ( 上升沿) 结束沿采样 ( 下降沿) 1
#define SPI_Mode_2 (1<<CPOL)|(0<<CPHA)//CPOL = 1, CPHA = 0 起始沿采样 ( 下降沿) 结束沿设置 ( 上升沿) 2
#define SPI_Mode_3 (1<<CPOL)|(1<<CPHA)//CPOL = 1, CPHA = 1 起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) 3


//定义SPI速度
//#define SPISpeed	SPI_FOSC_4
//#define SPIDoubleSpeed 1//定义启动SPI倍速功能
//定义SPI模式
#define SPI_Mode SPI_Mode_0

//pin count
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
//port
#define DDR_SPI		DDRB
#define POPT_SPI	PORTB
#define PIN_SPI 	PINB

//pin
#define PIN_SPI_SS 		PIN_SPI&(1<<SS_SPI)
#define PIN_SPI_SCK 	PIN_SPI&(1<<SCK_SPI)
#define PIN_SPI_MISO 	PIN_SPI&(1<<MISO_SPI)
#define PIN_SPI_MOSI 	PIN_SPI&(1<<MOSI_SPI)

#define SPI_LSB_FIRST	SPCR |= (1<<DORD)//LSB首发
#define SPI_MSB_FIRST	SPCR &=~(1<<DORD)//MSB首发


#ifdef SPI_SR_U
extern void SPI_SRInit(void);//LSB首发
#endif //SPI_SR_U

#ifdef SPI_MasterMultibyte_U///////////////////////////////////////////

extern volatile uint8 SPI_OutboxDataCount;	//启动SPI传输前需确认其为0，否则将丢失为完成传输的数据
extern void SPI_MasterMultibyteInit(uint8 SPISpeed);//LSB首发SPISpeed指定速度
extern void SPI_MasterMultibyteTransmit(uint8* cData,uint8 dataCount);//以同步模式传输一个字节return：读取的字节

#endif//SPI_MasterMultibyte_U///////////////////////////////////////////
#ifdef SPI_MASTER_U
extern void SPI_MasterInit(uint8 SPISpeed);//LSB首发SPISpeed指定速度
extern uint8 SPI_MasterTransmit(uint8 cData);//以同步模式传输一个字节return：读取的字节
#endif //SPI_MASTER_U
extern void SPIRelease();//释放 SPI口
#endif //SPI_U
/*===========VSPI===========*/
#define VSPI			1	//使用虚拟SPI口
#ifdef VSPI
#define VSPI_MSB_U		1	//使用MSB
//#define VSPI_LSB_U		1	//使用LSB
//#define VSPI_SCK_NORMAL_L	1	//定义使SCK信号线常态为低
/*如果要同步使用虚拟SPI口需定义它，这将导致一个传输过程结束前其他传输请求必须等待(实现进程同步)*/
//#define VSPI_SYNCHRO_U	1	

//SPI模式,四选一
#define VSPI_MODE_0 //起始沿采样 ( 上升沿) 结束沿设置 ( 下降沿) SCK常态0
//#define VSPI_MODE_1 //起始沿设置 ( 上升沿) 结束沿采样 ( 下降沿) SCK常态0
//#define VSPI_MODE_2 //起始沿采样 ( 下降沿) 结束沿设置 ( 上升沿) SCK常态1
//#define VSPI_MODE_3 //起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) SCK常态1

#if defined(VSPI_MODE_0)||defined(VSPI_MODE_1)
#define  VSPI_START_RISE//起始沿上升沿
#endif
#if defined(VSPI_MODE_1)||defined(VSPI_MODE_3)
#define  VSPI_START_SET//起始沿设置
#endif

#ifndef SDA_VSPI_PIN
#define SDA_VSPI_PIN P1^0
#endif
#ifndef SCK_VSPI_PIN
#define SCK_VSPI_PIN P1^1
#endif

//定义数据接口引脚
sbit SDA_VSPI = SDA_VSPI_PIN;
sbit SCK_VSPI = SCK_VSPI_PIN;

#ifdef VSPI_SYNCHRO_U
extern volatile bool VSPI_Transmiting;//传输过程中为1，不要写他
extern void InVSPI(void);			//开始使用时调用
static void __inline__ OutVSPI(void)//结束使用时调用
{
	VSPI_Transmiting = false;
}
#endif//VSPI_SYNCHRO_U
extern void VSPI_MasterInit(void);

#ifdef VSPI_LSB_U
extern uint8 VSPI_MasterTransmitLSB(uint8 cData);//传送
#endif//VSPI_LSB_U
#ifdef VSPI_MSB_U
extern uint8 VSPI_MasterTransmitMSB(uint8 cData);
#endif//VSPI_MSB_U
#endif	//VSPI
#endif	//_SPI_H_
