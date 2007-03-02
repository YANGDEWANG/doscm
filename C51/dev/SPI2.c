/*
* SPI2 协议
* 处理器类型：C51 只有你的项目需要两个SPI2口才使用此文件
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
#include "SPI2.h"

#ifdef VSPI2
#define setSCL()	(SCK_VSPI2 = true)
#define clSCL()		(SCK_VSPI2 = false)
#define setSDA()	(SDA_VSPI2 = true)
#define clSDA()		(SDA_VSPI2 = false)


#ifdef VSPI2_SYNCHRO
volatile bool VSPI2_Transmiting = false;
void InVSPI2(void)	//开始使用时调用
{
	while(VSPI2_Transmiting);
	VSPI2_Transmiting = true;
}
/*void OutVSPI2(void)	//结束使用时调用
{
VSPI2_Transmiting = false;
}*/
#endif//VSPI2_SYNCHRO

void VSPI2_MasterInit(void)
{
	setSDA();
#ifdef VSPI2_START_RISE
	clSCL();
#else//VSPI2_START_RISE
	setSCL();
#endif//VSPI2_START_RISE
#ifdef VSPI2_SYNCHRO
	VSPI2_Transmiting = false;
#endif//VSPI2_SYNCHRO
}
#ifdef VSPI2_LSB_U
uint8 VSPI2_MasterTransmitLSB(uint8 cData)
{
	uint8 i=8,ReadData;
	do
	{
#ifdef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		setSCL();
#else//VSPI2_START_RISE
		clSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET
		if(cData&1)
		{
			setSDA();
		}
		else
		{
			clSDA();
		}
		cData = cData>>1;
#ifdef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		clSCL();
#else//VSPI2_START_RISE
		setSCL();
#endif//VSPI2_START_RISE
#else//VSPI2_START_SET
#ifdef VSPI2_START_RISE
		setSCL();
#else//VSPI2_START_RISE
		clSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET
		ReadData = ReadData>>1;
#ifdef SDA_VSPI2_GET
		if(SDA_VSPI2_GET)
#else//SDA_VSPI2_GET
		if(SDA_VSPI2)
#endif//SDA_VSPI2_GET
			ReadData|=0x80;
#ifndef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		clSCL();
#else//VSPI2_START_RISE
		setSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET
	}while(--i);
	return ReadData;
}
#endif//VSPI2_LSB_U
#ifdef VSPI2_MSB_U
uint8 VSPI2_MasterTransmitMSB(uint8 cData)
{
	uint8 i=8,ReadData;
	do
	{
#ifdef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		setSCL();
#else//VSPI2_START_RISE
		clSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET
		if(cData&0x80)
		{
			setSDA();
		}
		else
		{
			clSDA();
		}
		cData = cData<<1;
#ifdef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		clSCL();
#else//VSPI2_START_RISE
		setSCL();
#endif//VSPI2_START_RISE
#else//VSPI2_START_SET
#ifdef VSPI2_START_RISE
		setSCL();
#else//VSPI2_START_RISE
		clSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET

		ReadData = ReadData<<1;
#ifdef SDA_VSPI2_GET
		if(SDA_VSPI2_GET)
#else//SDA_VSPI2_GET
		if(SDA_VSPI2)
#endif//SDA_VSPI2_GET
			ReadData|=1;
#ifndef VSPI2_START_SET
#ifdef VSPI2_START_RISE
		clSCL();
#else//VSPI2_START_RISE
		setSCL();
#endif//VSPI2_START_RISE
#endif//VSPI2_START_SET
	}while(--i)
	//setSDA();//bug
	return ReadData;
}
#endif//VSPI2_MSB_U
#endif//VSPI2
