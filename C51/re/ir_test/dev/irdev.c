#include <Global.h>
#include <stdio.h>
#include <string.h>
#include "ui\display.h"
#include "irdev.h"

#define IR_EX	EX0
#define IR_IT	IT0
#define IR_INT	INT0
#define IR_IE	IE0
#define MAX_WAIT_IRPIN_TO_HIGH_TIME 10000u	//最大低电平时间（单位us）//12M晶振
#define TO_DATAANALYSE_LONG_TIME	10000u	//由数据接收转到分析数据等待的时间（单位us）//12M晶振
#define TO_DATAANALYSE__TIME		4000u	//由数据接收转到分析数据等待的时间（单位us）//12M晶振//LX5104

#define IR_CLICK_TIME				50		//（单位us）

static bool wait; //用于有两节的马，取消的一次
typedef union _IR_TIME
{
	_uint16 time;
	_uint8  dat[2];//注意c51不兼容标准C
}IRTime;

uint16 getCurrentTime()
{
	IRTime time;
	TR1 = false;
	time.dat[1]  =  TL1;
	time.dat[0]  =  TH1;
	time.time+=15;
	TH1=time.dat[0];
	TL1 = time.dat[1];
	TR1 = true;
	return time.time;

}
#define CurrentTime  getCurrentTime()
prog_char * code ICNameS[]=
{
	"UN IC",
	"LC7461",
	//HS7461			
	//引导1，	用户13，	用户反13，	键8，键反8ok

	"LC7464",					
	//引导1，16用户,4parity,4system code，8prodact,8键8键反//未完成丢弃了4parity,4system code，8prodact

	"PT2222",
	//UPD6122	UPD6121 LC7462	
	//引导1，	用户8，		用户反8，	键8，键反8ok

	"PT2268",//					
	//用户8，	键8，		键8，空白重复前面//test

	"HS6222",					
	//引导1，	用户低8，	用户高8，	键8，键反8ok

	"TC9012",//TC9012、TC9243,TC9028	
	//引导1，	用户8，		用户8，		键8，键反8ok

	"3010",//SAA3010,HS3010						
	//起始2.5，	设置0.5，	用户5，		键6//14

	"HS3004",//低电平7.36ms
	//起始1，	奇偶1，		用户3，		键6//11

	"HS9148",//TC9148			
	//用户3，	连发标示3，	键6//12

	"PD6124",//UPD6124			
	//起始1，	键6，		用户6//13

	"6014C5",//MN6014C5D6			
	//47
	"6014C6",//MN6014C6D6			
	//49
	"6014C8",//MN6014C8D7			
	//31

	"SHARP",

	"50462",
	//SC50462
	//8用户,8键

	"50560",
	//SC50560
	//1起始,8用户,1 SYNC,8键

	"M50119",
	"LX5104",
	//3起始,2用户,7键重复3起始,2用户,7键
	//M50119
	//
	//mitc8d8
	//upd6124//upd6124-d7c8//d7c13
	//m50560
	//saa3010
	//m50119
	//sharp
	//m3004
	//jvc
	//custom6bit
};

enum IR_BIT_IDENTIFY
{
	IBI_PPM,
	IBI_PWM,
};


uint8 IRDevState;
uint8 ICName;
bool  IRKeyDown,IRKeyHold;
volatile uint8 IRChangec;//输入电平变化计数
uint8 perchdata IRChangeTime[IR_CHANGE_TIME_SIZE];
IRData IrInformation;
static uint8 IRDisableSysC;
void ShowIrInformation()
{
	ShowString(ICNameS[ICName],0,6);
	memset(dislinebuf,'-',sizeof(dislinebuf));
	switch(ICName)
	{
	
	case IC_TC9028:
	case IC_HS6222:
	case IC_PT2222:
	case IC_LC7461:
	case IC_LC7464:
	case IC_MN6014C5D6:
	case IC_MN6014C6D6:
		{
			ToStringWithXFW(dislinebuf,IrInformation.CustomCodeReverse);
		}
	case IC_SHARP:
	case IC_SAA3010:
	case IC_PT2268:
	case IC_HS3004:
	case IC_HS9148:
		{
			ToStringWithXFW(dislinebuf+2,IrInformation.CustomCode);
			ToStringWithXFW(dislinebuf+4,IrInformation.KeyCodeReverse);
			ToStringWithXFW(dislinebuf+6,IrInformation.KeyCode);
			break;
		}
	case IC_6014C8D7:
	
	case IC_SC50560:
	case IC_UPD6124:
	case IC_SC50462:
	case IC_M50119:
	case IC_LX5104:
		{
			ToStringWithXFW(dislinebuf+2,IrInformation.CustomCode);
			ToStringWithXFW(dislinebuf+6,IrInformation.KeyCode);
			break;
		}
	}
	ShowString(dislinebuf,8,8);
}

void IniIrdev()
{
	//ShowIrInformation();
	TMOD	= (T0_MASK_&TMOD)|T1_M0_;//使用timer1模式1
	TR1 = true;//开始计数
	IR_IT = 1;  //下降沿有效
	IP|=~PX0;
	//IR_IE = 0;//清中断标志
	IR_EX = 1;	//开外部中断0
}
void IRDisable(uint8 sysc)
{
	IRDisableSysC = sysc;
	IR_EX = 0;	//关外部中断
}
#define IRWait(sysc)\
{\
	IRDisableSysC = sysc;\
}
static volatile uint16 oldtime;
//static uint16 t1,t2;
void IRDevinterrupt(void) interrupt 0  using 2
{
	uint16 time = CurrentTime;
	if(!(IRChangec<sizeof(IRChangeTime)-1))
	{
		IRChangec=0;
		//return;
	}
	switch(IRDevState)
	{
	case IR_DEV_IDLE:
		{
			IRChangec = 0;
			//oldtime = CurrentTime;
			IRDevState = IR_DEV_INCEPT;
			break;
		}
	case IR_DEV_INCEPT:
		{
			IRChangeTime[IRChangec]=(time-oldtime)/IR_CLICK_TIME;//每格50us
			//oldtime = CurrentTime;
			IRChangec++;
			break;
		}
	case IR_DEV_DATAANALYSE:
		{
			return;
		}
	}
	while(!IR_INT)//等待变高
	{
		//t1 =   CurrentTime;
		//t2 =   time;
		if((CurrentTime-time)>MAX_WAIT_IRPIN_TO_HIGH_TIME)//超时出错
		{
			ErrorCode=EC_WaitIRPinToHighOvertime;
			IRDevState=IR_DEV_IDLE;
			return;
		}
	}
	oldtime = CurrentTime;
	IRChangeTime[IRChangec]=(oldtime-time)/IR_CLICK_TIME;//每格50us
	IRChangec++;
}
#define CHECK_WARP 5
static bool checkWarp(uint8 st,uint8 d)
{
	return (st+CHECK_WARP>d&&st-CHECK_WARP<d);
}
#define CHECK_WARPH 3
static bool checkWarpH(uint8 st,uint8 d)
{
	return (st+CHECK_WARPH>d&&st-CHECK_WARPH<d);
}
static uint8 getDataPPM(uint8 ic,uint8 time)//lsb
{
	uint8 dat,i=8;
	do
	{
		dat=dat>>1;
		if(IRChangeTime[ic]>time)
		{
			dat|=(1<<7);
		}
		ic+=2;
	}while(--i);
	return dat;
}
/*static uint8 set3010(uint8 ic)
{
bool oldbit = true;
uint8 i = 6;
uint8 dat = 0;
do
{
dat=dat<<1;
if(IRChangeTime[ic]>20)//与上一比特相反
{	
oldbit=!oldbit;
}
else
{
ic++;//与上一比特相同跳过一个电平
}
ic++;
if(oldbit)dat++;
}while(--i);
return ic|(dat<<8);
}*/
void setData()
{
	uint8 i,b,dat,ic;
	bool oldbit;
	if(!wait)
	{
		IrInformation.CustomCode = 0;
		IrInformation.CustomCodeReverse = 0;
		IrInformation.KeyCode = 0;
		IrInformation.KeyCodeReverse = 0;
	}
	switch(ICName)
	{
	case IC_HS9148:
		{
			i=3;
			ic=0;
			dat=0;
			do
			//for(i=0,dat=0;i<3;i++)
			{
				dat=dat<<1;
				if(IRChangeTime[ic]>17)
					dat++;
				ic+=2;
			}while(--i);
			IrInformation.CustomCode = dat;
			if(IRChangeTime[6]>17)//连发标志
				IrInformation.KeyCodeReverse = 1;
			i=8;
			ic=8;
			dat=0;
			do
			//for(i=0,dat=0,ic=8;i<8;i++)
			{
				dat=dat<<1;
				if(IRChangeTime[ic]>17)
					dat++;
				ic+=2;
			}while(--i);
			IrInformation.KeyCode = dat;
			IRDisable(54/CLICK_CYCLE_MS+1);//禁止在54ms内在接收数据//跳过重复码，有点不明智:)等待好的解决方法
			break;
		}
	case IC_HS3004:
		{
			if(IRChangeTime[3]>120)//判断奇偶（奇次就下面）
				IrInformation.KeyCodeReverse = 1;
#if 1
			i=3;
			ic=5;
			dat=0;
			do
			//for(i=0,dat=0;i<3;i++)
			{
				dat=dat<<1;
				if(IRChangeTime[ic]>120)
					dat++;
				ic+=2;
			}while(--i);
			IrInformation.CustomCode = dat;
			i=6;
			dat=0;
			do
			//for(i=0,dat=0;i<6;i++)
			{
				dat=dat<<1;
				if(IRChangeTime[ic]>120)
					dat++;
				ic+=2;
			}while(--i);
			IrInformation.KeyCode = dat;
#else
			IrInformation.CustomCode=getDataPPM(5,120)&7;
			IrInformation.KeyCode=getDataPPM(5+2*3,120)&63;
#endif
			break;
		}
	case IC_SAA3010:
		{
			ic=2;
			oldbit = true;
			for(b=0;b<2;b++)
			{
				dat = 0;
				i = 6;
				do
				{
					dat=dat<<1;
					if(IRChangeTime[ic]>20)//与上一比特相反
					{	
						oldbit=!oldbit;
					}
					else
					{
						ic++;//与上一比特相同跳过一个电平
					}
					ic++;
					if(oldbit)dat++;
				}while(--i);
				if(b==0)
				{
					IrInformation.CustomCode = dat&0x1f;
					IrInformation.KeyCodeReverse = dat>>5;
				}
				else
					IrInformation.KeyCode = dat;
			}
			break;
		}
	case IC_TC9028:
	case IC_HS6222:
	case IC_PT2222:
		{

			IrInformation.CustomCode = getDataPPM(3,20);
			IrInformation.CustomCodeReverse = getDataPPM(3+2*8,20);
			IrInformation.KeyCode = getDataPPM(3+4*8,20);
			IrInformation.KeyCodeReverse = getDataPPM(3+6*8,20);
			if(IrInformation.CustomCode==IrInformation.CustomCodeReverse&&ICName!=IC_TC9028)
			{
				ICName = IC_HS6222;
			}
			break;
		}
	case IC_PT2268:
		{

			IrInformation.CustomCode = getDataPPM(0,25);
			IrInformation.KeyCode = getDataPPM(2*8,25);
			IrInformation.KeyCodeReverse = getDataPPM(4*8,25);
			break;
		}
	case IC_LC7461:
		{
			IrInformation.CustomCode = getDataPPM(3,20);//用户码底八位
			IrInformation.CustomCodeReverse = getDataPPM(3+2*8,20)&0x1f;//用户码高5位
			IrInformation.KeyCode = getDataPPM(3+4*13,20);
			IrInformation.KeyCodeReverse = getDataPPM(3+4*13+2*8,20);
			break;
		}
	case IC_UPD6124:
		{
			IrInformation.KeyCode= getDataPPM(2,15)&0x7f;
			IrInformation.CustomCode= getDataPPM(2+7*2,15)&0x3f;
			break;
		}
	case IC_MN6014C5D6://1起始，5用户正6键正，5用户反6键反////多重用户码
		{
			IrInformation.CustomCode = getDataPPM(3,30)&0x1f;
			IrInformation.KeyCode = getDataPPM(3+2*5,30)&0x3f;
			IrInformation.CustomCodeReverse = getDataPPM(3+2*5+2*6,30)&0x1f;
			IrInformation.KeyCodeReverse = getDataPPM(3+2*5+2*6+2*5,30)&0x3f;
			break;
		}
	case IC_MN6014C6D6://1起始，6用户正6键正，6用户反,6键反//
		{
			IrInformation.CustomCode = getDataPPM(3,30)&0x3f;
			IrInformation.KeyCode = getDataPPM(3+2*6,30)&0x3f;
			IrInformation.CustomCodeReverse = getDataPPM(3+2*6+2*6,30)&0x3f;
			IrInformation.KeyCodeReverse = getDataPPM(3+2*6+2*6+2*6,30)&0x3f;
			break;
		}
	case IC_6014C8D7://
		{
			IrInformation.KeyCode = getDataPPM(2,15)&0x7f;
			IrInformation.CustomCode = getDataPPM(2+2*7,15);
			break;
		}	
	case IC_LC7464://1起始，16用户,4parity,4system code，8prodact,8键8键反//未完成丢弃了4parity,4system code，8prodact
		{
			IrInformation.CustomCodeReverse = getDataPPM(3,20);
			IrInformation.CustomCode = getDataPPM(3+2*8,20);
			IrInformation.KeyCode = getDataPPM(3+2*4*8,20);
			IrInformation.KeyCodeReverse = getDataPPM(3+2*5*8,20);
			break;
		}
	case IC_SHARP://
		{
			IrInformation.CustomCode = getDataPPM(1,27)&31;
			if(wait)
			{
				wait = false;
				IrInformation.KeyCodeReverse = getDataPPM(1+2*5,27);
				//IrInformation.CustomCodeReverse = getDataPPM(3+2*4+2*8,20);

				IRWait(0);
			}
			else
			{
				wait = true;
				IrInformation.KeyCode = getDataPPM(1+2*5,27);
				IRWait(200/CLICK_CYCLE_MS);
			}

			//IrInformation.CustomCodeReverse = getDataPPM(3,20);
			//IrInformation.CustomCode = getDataPPM(3+2*8,20);
			//IrInformation.KeyCode = getDataPPM(3+2*4*8,20);
			//IrInformation.KeyCodeReverse = getDataPPM(3+2*5*8,20);
			//IRDisable(60/CLICK_CYCLE_MS+1);//禁止在54ms内在接收数据//跳过重复码，有点不明智:)等待好的解决方法
			break;
		}
	case IC_SC50462://8用户,8键
		{
			IrInformation.CustomCode = getDataPPM(1,20);
			IrInformation.KeyCode = getDataPPM(1+2*8,20);
			break;
		}
	case IC_SC50560://1起始,8用户,1 SYNC,8键
		{
			IrInformation.CustomCode = getDataPPM(3,20);
			IrInformation.KeyCode = getDataPPM(5+2*8,20);
			break;
		}
	case IC_M50119://3用户,7键
		{
			IrInformation.KeyCodeReverse = getDataPPM(1,20)&7;
			IrInformation.KeyCode = getDataPPM(1+2*3,20)&0x7f;
			break;
		}
	case IC_LX5104://3起始,2用户,7键重复3起始,2用户,7键
		{
			IrInformation.CustomCode = getDataPPM(3*2,16)&3;
			IrInformation.KeyCode = getDataPPM(3*2+2*2,16)&0x7f;
			if(IrInformation.KeyCode==0)
				ICName = IC_NULL;
			break;
		}
	}
	//	firstS = true;
}
void dataAnalyse()
{
	//bool ok = false;
	if(IRChangec<10)//可能是重复码
	{
		switch(ICName)
		{
		case IC_LC7461:
		case IC_HS6222:
			/*case IC_PT2222:
			{
			if(IRChangec == 3
			&&checkWarp(9000/IR_CLICK_TIME,IRChangeTime[0])
			&&(checkWarp(2250/IR_CLICK_TIME,IRChangeTime[1])||checkWarp(4500/IR_CLICK_TIME,IRChangeTime[1]))
			)
			{
			IRKeyHold = true;
			}
			break;
			}*/
		case IC_TC9028:
			{
				if(IRChangec==5
					&&checkWarp(4480/IR_CLICK_TIME,IRChangeTime[0])
					&&checkWarp(4480/IR_CLICK_TIME,IRChangeTime[1])
					)
				{
					IRKeyHold = true;
				}
				break;
			}
		case IC_LC7464:
			{
				if(IRChangec==3
					&&checkWarp(68,IRChangeTime[0])
					&&checkWarp(68,IRChangeTime[1])
					)
				{
					IRKeyHold = true;
				}
				break;
			}
		}
		if(IRKeyHold)
			return;
	}
	ICName = IC_NULL;
	//M50119
	if(IRChangec == 20+1
		&&checkWarpH(10,IRChangeTime[0]))					//21
	{
		ICName = IC_M50119;
	}
	//HS3004
	if(IRChangec == 22+1)									//23
	{
		if(checkWarpH(5,IRChangeTime[0])&&checkWarp(147,IRChangeTime[1]))
		{
			ICName = IC_HS3004;
		}
		if(checkWarp(24,IRChangeTime[0])
			&&checkWarpH(8,IRChangeTime[1])
			&&checkWarp(24,IRChangeTime[2])
			&&checkWarpH(8,IRChangeTime[3])
			&&checkWarpH(8,IRChangeTime[4])
			&&checkWarp(24,IRChangeTime[5]))	
			/*if(checkWarp(24,IRChangeTime[0])
			&&checkWarpH(8,IRChangeTime[1])
			&&checkWarp(24,IRChangeTime[3])
			&&checkWarpH(8,IRChangeTime[2])
			&&checkWarpH(8,IRChangeTime[5])
			&&checkWarp(24,IRChangeTime[4]))*/	
		{
			ICName = IC_LX5104;
		}
	}
	if(IRChangec == 22+1)									//23
	{
		if(checkWarp(34,IRChangeTime[0]+IRChangeTime[1]))
			ICName = IC_HS9148;
	}
	//HS9148//IC_PT2268//UPD6124
	if(IRChangec == 24+1)									//25
	{
		if(checkWarpH(12,IRChangeTime[0])&&
			checkWarp(37,IRChangeTime[1]))
			ICName = IC_PT2268;
		else if(checkWarp(34,IRChangeTime[0]+IRChangeTime[1]))
			ICName = IC_HS9148;
		else if(checkWarp(48,IRChangeTime[0])//sony ir
			||checkWarp(51,IRChangeTime[0]))// 美浩 ir
			ICName = IC_UPD6124;
	}
	//SC50462
	if(IRChangec == 32+1)									//33
	{
		//if(checkWarp(67,IRChangeTime[0])&&checkWarp(67,IRChangeTime[1]))
		ICName =IC_SC50462;
	}
	//SHARP//IC_6014C8D7
	if(IRChangec == 30+1)									//31
	{
		//IC_6014C8D7
		if(checkWarp(48,IRChangeTime[0]))
			ICName = IC_6014C8D7;
		else
			//if(checkWarp(67,IRChangeTime[0])&&checkWarp(67,IRChangeTime[1]))
			ICName = IC_SHARP;
	}
	//SC50560
	if(IRChangec == 36+1)									//37
	{
		if((checkWarp(8400/IR_CLICK_TIME,IRChangeTime[0])&&checkWarp(4200/IR_CLICK_TIME,IRChangeTime[1]))//标准
			||(checkWarp(170,IRChangeTime[0])&&checkWarp(77,IRChangeTime[1])))//实测
			ICName =IC_SC50560;
	}

	if(checkWarp(67,IRChangeTime[0])&&checkWarp(67,IRChangeTime[1]))
	{
		//MN6014C5D6
		if(IRChangec == 46+1)									//47
		{
			ICName = IC_MN6014C5D6;
		}
		//MN6014C6D6
		if(IRChangec == 50+1)									//51
		{
			ICName = IC_MN6014C6D6;
		}
	}


	//UPD6122,PT2222,LC7461 和LC7462
	if(checkWarp(9000/IR_CLICK_TIME,IRChangeTime[0])
		||checkWarp(170,IRChangeTime[0]))//有的差得太远也不知是不是别地
	{
		if(IRChangec == 66+1)//UPD6122,PT2222和LC7462					//67
			ICName = IC_PT2222;
		else if(IRChangec == 86+1)//LC7461								//87
			ICName = IC_LC7461;
	}
	//TC9028,TC9012、TC9243
	if(checkWarp(4480/IR_CLICK_TIME,IRChangeTime[0])
		&&checkWarp(4480/IR_CLICK_TIME,IRChangeTime[1]))
	{
		if(IRChangec == 66+1)											//67
			ICName = IC_TC9028;
	}
	//IC_LC7464
	if(IRChangec == 99)									//99
	{
		if(checkWarp(68,IRChangeTime[0]))
		{
			if(checkWarp(34,IRChangeTime[1]))
				ICName = IC_LC7464;

		}
	}

	//SAA3010
	//if(checkWarp(17,IRChangeTime[0])&&checkWarp(17,IRChangeTime[1]))	//xx
	if(checkWarpH(17,IRChangeTime[0])||checkWarp(34,IRChangeTime[0]))	//不同与datasheet上面的同
	{
		ICName = IC_SAA3010;
	}


	if(ICName != IC_NULL)
	{
		setData();
	}
	/*else
	{
	ErrorCode =EC_NOSUPPORTIC;
	}*/
}
uint16 checkTimeOut()
{
	IRTime time;
	//uint16 oldt;
	TR1 = false;
	time.dat[1]  =  TL1;
	time.dat[0]  =  TH1;
	time.time+=15;
	TH1=time.dat[0];
	TL1 = time.dat[1];
	TR1 = true;

	//oldt = oldtime;
	//EA = true;
	return time.time;

}
#if 1
bool ToDataAnalyseTimeLong;
void IrDevDiv()
{
	uint8 ic = 	 IRChangec;
	uint8 i;
	uint16 timespan;
	EA = false;
	timespan= checkTimeOut()-oldtime;

	if(timespan>TO_DATAANALYSE__TIME)
	{

		if(!ToDataAnalyseTimeLong&&ic<15)
		{
			if(timespan<TO_DATAANALYSE_LONG_TIME)
				ToDataAnalyseTimeLong = true;
			else
				for(i=0;i<ic;i++)
				{
					if(IRChangeTime[i]>TO_DATAANALYSE__TIME/IR_CLICK_TIME)
						ToDataAnalyseTimeLong = true;
				}
		}
		if(ToDataAnalyseTimeLong&&timespan<TO_DATAANALYSE_LONG_TIME)
			goto end;
		EA = true;
		if(IRDevState==IR_DEV_INCEPT&&ic)
		{
			IRDevState = IR_DEV_DATAANALYSE;
			while(ic<sizeof(IRChangeTime))
			{
				IRChangeTime[ic]=0;
				ic++;
			}
			dataAnalyse();
			if(ICName != IC_NULL&&!wait)
			{
				IRKeyDown = true;
			}
			IRDevState = IR_DEV_IDLE;
			ToDataAnalyseTimeLong = false;
		}
	}
end:
	EA = true;
}
#else
//void IrDevDiv()
//{
//	uint8 ic = 	 IRChangec;
//	EA = false;
//	if(checkTimeOut()-oldtime>TO_DATAANALYSE_LONG_TIME)
//	{
//		EA = true;
//		if(IRDevState==IR_DEV_INCEPT&&ic)
//		{
//			IRDevState = IR_DEV_DATAANALYSE;
//			while(ic<sizeof(IRChangeTime))
//			{
//				IRChangeTime[ic]=0;
//				ic++;
//			}
//			dataAnalyse();
//			if(ICName != IC_NULL&&!wait)
//			{
//				IRKeyDown = true;
//			}
//			IRDevState = IR_DEV_IDLE;
//		}
//	}
//	EA = true;
//}
void IrDevDiv()
{
	uint8 ic = 	 IRChangec;
	uint16 timespan;
	EA = false;
	timespan= checkTimeOut()-oldtime;

	if(timespan>TO_DATAANALYSE_LONG_TIME)
	{
		EA = true;
		if(IRDevState==IR_DEV_INCEPT&&ic)
		{
			IRDevState = IR_DEV_DATAANALYSE;
			while(ic<sizeof(IRChangeTime))
			{
				IRChangeTime[ic]=0;
				ic++;
			}
			dataAnalyse();
			if(ICName != IC_NULL&&!wait)
			{
				IRKeyDown = true;
			}
			IRDevState = IR_DEV_IDLE;
		}
	}
	EA = true;
}
#endif
void PollIR10ms()
{
	if(IRDisableSysC)
	{

		IRDisableSysC--;
		if(IRDisableSysC==0)
		{
			wait = false;
			IniIrdev();
		}
	}
}