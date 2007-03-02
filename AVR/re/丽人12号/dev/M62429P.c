/*
* M62429P，2通道音量控制器
* 处理器类型：avr
* 版本：1
* 日期：2006-12-3
* 作者：杨德王<yangdewang@gmail.com>
***********************************************************
* Symbol	Parameter							Min		  *
* tcr		Cycle time of clock					4μs	  *
* tWHC		Pulse width of clock ("H" level)	1.6μs	  *
* tWLC		Pulse width of clock ("L" level)	1.6μs	  *
* tSD		Data setup time						0.8μs	  *
* tHD		Data hold time						0.8μs	  *
***********************************************************
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
#include <util/delay.h>
#include <math.h>
#include "M62429P.h"
#include "iic.h"
static uint8 m62429p;
#define clSDA()  (m62429pPORT	&=~(1<<M62429P_DATP))
#define setSDA() (m62429pPORT	|=(1<<M62429P_DATP))

static void clCLK() 
{
	if(m62429p==0)
	{
		m62429pSCK1PORT	&=~(1<<M62429P_SCK1P);
	}
	else
	{
		m62429pSCK2PORT	&=~(1<<M62429P_SCK2P);
	}
}
static void setCLK() 
{
	if(m62429p==0)
	{
		m62429pSCK1PORT	|=(1<<M62429P_SCK1P);
	}
	else
	{
		m62429pSCK2PORT	|=(1<<M62429P_SCK2P);
	}
}

#if !defined(SAVE_M62429P_SETTING)
uint8 M62429PVolumeA[M62429P_IC_COUNT*2];
#endif//SAVE_M62429P_SETTING
void M62429PUpdateAll()
{
	uint8 i;
	uint8 bc;
	uint8 vol;
	//天哪居然要和I2C公用SDA
	IICDisable();
	//dat out 0
	m62429pPORT	&=~(1<<M62429P_DATP);
	m62429pDDR	|=(1<<M62429P_DATP);
	//sck out 0
	m62429pSCK1DDR|=(1<<M62429P_SCK1P);
	m62429pSCK2DDR|=(1<<M62429P_SCK2P);
	m62429p = 0;
	clCLK();
	m62429p = 1;
	clCLK();

	//clk下降沿SDA必须为底否则将锁存数据

	for(i=0;i<2*2;i++)
	{
		m62429p=i/2;
		if(i%2)//如果是R声道
		{
			setSDA();
		}
		_delay_us(2);
		setCLK();
		_delay_us(2);
		clSDA();
		clCLK();
		setSDA();
		_delay_us(2);
		setCLK();
		//该数据了
		vol = M62429PVolumeA[i];
		if(vol>M62429P_MAXVOLUME)
		{
			vol = 0;
		}
		else
		{
			vol += 4;
		}
		vol = vol>>2|vol<<5;
		vol|=1<<7;
		bc = 8;
		do
		{
			clSDA();
			clCLK();
			if(vol&1)
			{
				setSDA();
			}
			_delay_us(2);
			vol=vol>>1;
			setCLK();
			_delay_us(2);
		}
		while(--bc);
		//还发一个1
		clSDA();
		clCLK();
		setSDA();
		_delay_us(2);
		setCLK();
		//锁存数据
		_delay_us(2);
		clCLK();
		_delay_us(2);
		clSDA();
	}

}
