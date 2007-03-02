/*
* LC72131 PLL AM/FM/SW收音控制器
* 处理器类型：C51,AVR
* 版本：1
* 日期：2006-12-14
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

#include <math.h>
#include "radio.h"
#include "LC72131.h"
#include "arith.h"
#include "spi2.h"
#include "Clock.h"
#include <global.h>

////定义数据接口引脚
#ifdef __AVR__
static void inline setCE()	
{ 

	PORT_Readio |= (1<<ReadioSTB);

}
static void inline clCE()
{
	PORT_Readio &=~(1<<ReadioSTB);

}
#elif defined (__C51__)
#define setCE()	(LC72131_STB=true)
#define clCE()	(LC72131_STB=false)
#endif
//定义引脚
#define AM_FM_SW_PIN	BO1
#define IF_REQ_PIN		BO2
#define ST_MO_SW_PIN	BO3
#define ST_OUT_BIN		IO2

#define InMod1 0x28	 //add
#define InMod2 0x29	 //add
#define OutMod 0x2a	 //add

#define con1 LC72131IM1.LC72131IM1
#define con2 LC72131IM2.LC72131IM2
#define indat LC72131OM.LC72131OM
#define C1 1		//命令1
#define C2 0		//命令2

#define write(dat) VSPI2_MasterTransmitLSB(dat)

#ifndef SAVE_RADIO_SETTING
RadioMode	LC72131RadioMode		=	DefaultRadioMode;//为0=am
#endif//SAVE_RADIO_SETTING
static uint8 perchdata frequencyTmp[SeekPointCount];
uint8 CurrentSeekPoint_Radio;//当前缓存“中频偏差”frequencyTmp 的偏移量
uint8 LC72131CurrentFrequency;//当前LC72131的工作频率，实际频率因波段不同有不同的计算方法（如上）

union LC72131IntputCom1 LC72131IM1;
union LC72131IntputCom2 LC72131IM2;
union LC72131OutputCom LC72131OM;	
uint8  LC72131SeekSatrtTime;//搜索开始时间
bool RadioLocked;//调用LC72131IsLock()后，用于判断是否有台
bool LC72131SeekWait;//为一表示LC72131.C处于等的读取中频计数时间段
bool LC72131SeekFlag;//启动自动搜台时被置一，搜索结束时被自动置零
static bool LC72131SeekWay; //自动搜台时的频率更改方向



void WriteConLC72131(bool ConOneOrTow)//写命令到lc72131 ConOneOrTow = 1,命令1;=0 命令2//ret: CE=0,CL=1
{
#ifdef VSPI_SYNCHRO_U
	InVSPI();
#endif //VSPI_SYNCHRO_U
	uint8* tmp,com;
	if(ConOneOrTow)
	{
		tmp = LC72131IM1.dat;
		com	= InMod1;
	}
	else
	{
		tmp = LC72131IM2.dat;
		com	= InMod2;
	}
	clCE();
	write(com);
	setCE();
	write(*tmp);
	tmp++;
	write(*tmp);
	tmp++;
	write(*tmp);

	clCE();
#ifdef VSPI_SYNCHRO_U
	OutVSPI();
#endif //VSPI_SYNCHRO_U
}
void ReadConLC72131()//读取
{
#ifdef VSPI_SYNCHRO_U
	InVSPI();
#endif //VSPI_SYNCHRO_U
	clCE();
	write(OutMod);
//	VSPI_MSB_FIRST();
	setCE();
//	_delay_loop_1(1);//cano
	LC72131OM.dat[2] = VSPI2_MasterTransmitMSB(0xff);
#if defined(__C51__)//C51高位低字节
	LC72131OM.dat[0] = VSPI2_MasterTransmitMSB(0xff);
	LC72131OM.dat[1] = VSPI2_MasterTransmitMSB(0xff);
#else
	LC72131OM.dat[1] = VSPI2_MasterTransmitMSB(0xff);
	LC72131OM.dat[0] = VSPI2_MasterTransmitMSB(0xff);
#endif
	clCE();
#ifdef VSPI_SYNCHRO_U
	OutVSPI();
#endif //VSPI_SYNCHRO_U
}
void LC72131Mute(bool of)//禁音 of=false->请求中频输出加禁音
{
	if(of)
		con2.IF_REQ_PIN = 1;//关闭中频输出清禁音
	else
		con2.IF_REQ_PIN = 0;//请求中频输出加禁音

	WriteConLC72131(C2);	//写命令2
}
void LC72131SeekFrequency(bool seek) //设置频率//fm = f*50k ,am = f*9k,sw = f*5k 调用方法前请先设置“LC72131CurrentFrequency”;seek = true 将进行中频计数//等待40ms后调用“LC72131IsLock“判断是否有台
{
	uint16 f;
	switch(LC72131RadioMode)
	{
	case RM_AM:
		{
			f = 800+((uint16)LC72131CurrentFrequency)*16;//450k/9k<<4 + f<<4
			break;
		}
#ifdef UseSW 
	case RM_SW:
		{
			f = 90+LC72131CurrentFrequency;//450k/5k
			break;
		}
#endif
	case RM_FM:
		{
			f = (107+FMFrequencyExcursion*10+(uint16)LC72131CurrentFrequency)/2;//10.7m//100k//最低频率87.00M
			break;
		}
	}
	con1.P_CTR_L = f;
	con1.P_CTR_H = (f>>8);
	//reset IFcounter
	con1.CET = 0;
	WriteConLC72131(C1);		//写命令1
	//ShowRadio(true);////////todo
	if(!seek)
		return;
	//start IFcounter
	LC72131Mute(false);
	con1.CET = 1;
	WriteConLC72131(C1);		//写命令1
	LC72131SeekWait = true;
	LC72131SeekSatrtTime = SysClick;
}


void SetRadioMode(RadioMode mode)//设置收音机波段
{
	con1.AM_FM = LC72131RadioMode = mode;
	if(mode==RM_FM)
	{
		con2.AM_FM_SW_PIN = 0;
		//LC72131CurrentFrequency = FMFrequency;
		//con1.R_CRT	= 8;//步进10khz
		//con1.R_CRT	= 1;//步进50khz
		con1.R_CRT	= 0;//步进100khz
		SetRadioStation(FMStation);
	}
	else
	{
		con2.AM_FM_SW_PIN = 1;
#ifndef UseSW
		//LC72131CurrentFrequency = AMFrequency;
		con1.R_CRT	= 9;//步进9khz
		SetRadioStation(AMStation);
#endif
#ifdef UseSW
		if(mode==RM_AM)
		{
			//LC72131CurrentFrequency = AMFrequency;
			con1.R_CRT	= 9;//步进9khz
			SetRadioStation(AMStation);
		}
		else
		{
			con1.R_CRT	= 10;//步进5khz
			SetRadioStation(SWStation);
		}
#endif
	}

	WriteConLC72131(C2);			//写命令2
}
void IniLC72131()
{

#ifdef __AVR__
	DDR_Readio  |= (1<<ReadioSTB);
#endif
	clCE();
	/*
	SetRadioMode(LC72131RadioMode);
	con1.CET	= 0;
	con1.XS  	= 0;		//4.5Mhz
	con2.IO_C1	= 0;		//inport
	con2.IO_C2	= 0;
	con2.ST_OUT_BIN		= 0;//open
	con2.IF_REQ_PIN		= 1;
	con2.DNC	= 0;
	con2.DO_C	= 0;		//open
	con2.UNLOCK= 0;	
	con2.DZ_C 	= 3;
	con2.IF_CAT= 2;			//32ms
	con2.TBC	= 0;
	con2.DLC	= 0;
	con2.IFS	= 1;
	con2.TEST	= 0;
	*/
	LC72131IM1.dat[0] = 0xd1;
	LC72131IM1.dat[1] = 0x03;
	LC72131IM1.dat[2] = 0x12;
	LC72131IM2.dat[0] = 0x20;
	LC72131IM2.dat[1] = 0xc0;
	LC72131IM2.dat[2] = 0x12; 
	WriteConLC72131(C1);			//写命令1
	WriteConLC72131(C2);			//写命令2
	SetRadioMode(LC72131RadioMode);
}
/*ulong GetCTR()
{
ulong tmp = indat.IF_CTRH;
tmp =	tmp<<16;
return (tmp+indat.IF_CTRL);
}*/
uint8 LC72131IsLock() //调用后测试 RadioLocked 判断是否有台 return：中频偏差绝对值（仅当调用后 RadioLocked == true 时有意义）
{
	uint16 tmp;
	uint8 Patient;
	LC72131SeekWait = false;	
	RadioLocked		 = false;
	ReadConLC72131();//读取
	if(LC72131RadioMode==RM_FM)
	{
		if(0x05 == indat.IF_CTRH)//14720//342400
		{
			tmp = 14720;
			Patient = FMLockFrequencyPatient;
		}
		else
		{
			return 255;
		}
	}
	else//14400
	{
		tmp = 14400;
		Patient = AMLockFrequencyPatient;
	}
	tmp = abs(tmp-((int)(indat.IF_CTRL)));
	if(tmp<Patient)
	{
		//frequencyTmp[CurrentSeekPoint_Radio] = tmp; 
		//CurrentSeekPoint_Radio++;
		RadioLocked		 = true;
		return (uint8)tmp;
	}
	return 255;
}

static void optimization()//挑选最佳频点//调用时要求frequencyTmp[CurrentSeekPoint_Radio]为有效数据//LC72131CurrentFrequency为frequencyTmp[CurrentSeekPoint_Radio-1]处频率
//即CurrentSeekPoint_Radio 指向有效数据 LC72131CurrentFrequency 为锁定时频率
{
	uint8 i,mini=0;
	if(CurrentSeekPoint_Radio)
	{
		for(i=1;i<=CurrentSeekPoint_Radio;i++)
		{
			if(frequencyTmp[i]<frequencyTmp[mini])
			{
				mini = i;
			}
		}
		mini = CurrentSeekPoint_Radio - mini;
		if(LC72131SeekWay)
		{
			LC72131CurrentFrequency -= mini;
		}
		else
		{
			LC72131CurrentFrequency += mini;
		}
		LC72131SeekFrequency(false);
	}
	LC72131SeekFlag	 = false;
	LC72131Mute(true);
}
/*设置当前频率为下一点 
注意：如果当前频率当前频率已到界限调用方法将无效果
*/
void LC72131NextFrequemcy()
{
	//uint8 oldFrequencyORpiancha = LC72131CurrentFrequency;
	//LC72131CurrentFrequency += SeekStep;
	switch(LC72131RadioMode)
	{
	case RM_AM:
		{
			LC72131CurrentFrequency = MovePointer(MinAMFrequency,MaxAMFrequency,LC72131CurrentFrequency,LC72131SeekWay);//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
			break;
		}
#ifdef UseSW 
	case RM_SW:
		{
			LC72131CurrentFrequency = MovePointer(MinSWFrequency,MaxSWFrequency,LC72131CurrentFrequency,LC72131SeekWay);//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
			break;
		}
#endif
	case RM_FM:
		{
			LC72131CurrentFrequency = MovePointer(MinFMFrequency,MaxFMFrequency,LC72131CurrentFrequency,LC72131SeekWay);//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
			break;
		}
	}
	LC72131SeekFrequency(LC72131SeekFlag);
#if 0
	if(oldFrequencyORpiancha == LC72131CurrentFrequency)
	{
		//RadioAutoSeekFlag = false;
		/*if(CurrentSeekPoint_Radio)
		{
		CurrentSeekPoint_Radio--;
		}
		optimization();*/
		SetRadioStation(0);
	}
	else
		LC72131SeekFrequency(true);
	//		optimization();
#endif
}
/*
*仅在自动搜台时使用，并且应在LC72131SeekWait置true后40ms调用
*在LC72131SeekWait置true的时候我们保存了系统时间LC72131SeekSatrtTime便于你计时
*/
void LC72131Seek()
{
	uint8 piancha = LC72131IsLock();
	if(RadioLocked)
	{
		frequencyTmp[CurrentSeekPoint_Radio] = piancha;
		if(CurrentSeekPoint_Radio < (SeekPointCount-1))
		{	
			CurrentSeekPoint_Radio++;
			LC72131NextFrequemcy();
		}
		else
		{
			optimization();
		}
	}
	else
	{
		if(CurrentSeekPoint_Radio)
		{
			CurrentSeekPoint_Radio--;
			//将当前频率退回上次锁定的频率
			if(LC72131SeekWay)
			{
				LC72131CurrentFrequency--;
			}
			else
			{
				LC72131CurrentFrequency++;
			}
			optimization();
		}
		else
		{
			LC72131NextFrequemcy();
		}
	}
}
void SetST_MO(bool ST_MO)	//打开与关闭立体声输出 ST_MO=true时立体声关闭
{
	con2.ST_MO_SW_PIN = ST_MO;
	if(LC72131RadioMode == RM_FM)
		WriteConLC72131(C2);			//写命令2
}
bool GetST_MO() // 获取当前声音输出状态 return:false =立体声
{
	if(!LC72131SeekWait)
	{
		ReadConLC72131();//读取
		//避免ReadConLC72131出错导致跑台
		//WriteConLC72131(C1);		//写命令1
		//WriteConLC72131(C2);		//写命令1
	}
	return indat.ST_OUT_BIN;
}


void LC72131SeekStart(bool SeekWay)//开始搜台 SeekWay == true —> 向高频段搜
{
	CurrentSeekPoint_Radio = 0;
	LC72131SeekWay	=	SeekWay;
	LC72131SeekFlag	=	true;
	LC72131NextFrequemcy();
}
