#include <global.h>
#include "STF16360EN.h"
#include "pointvfddisplay.h"
#include "boolean.h"
#include <avr/interrupt.h>
#include <dwstd.h>
#include <string.h>
uint8 dismem[POINTVFDDISPLAY_DISMEM_SIZE];
uint8 disG;
uint8 keyP;
uint8 KeyCode;
void DISClean()
{
	uint8 i=0;
	for(;i<sizeof(dismem);i++)
	{
		dismem[i]=0;
	}
}
#if 0
uint8 disdevmem[4];

#define MAX_DISB (sizeof(disdevmem)*8)
prog_char disGTable[]=
{
	6,5,4,3,2,1,0

};
prog_char disRTable[]={31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11};
prog_char KeyRTable[]={10,9,8,7,6};
#else
uint8 disdevmem[7];

#define MAX_DISB (sizeof(disdevmem)*8)
prog_char disGTable[]={
	MAX_DISB-16,
	MAX_DISB-15,
	MAX_DISB-14,
	MAX_DISB-13,
	MAX_DISB-12,
	MAX_DISB-11,
	MAX_DISB-10,
	MAX_DISB-9,
	MAX_DISB-8,
	MAX_DISB-7,
	MAX_DISB-6,
	MAX_DISB-5,
	MAX_DISB-4,
	MAX_DISB-3,
	MAX_DISB-2,
	MAX_DISB-1,
	5,
	4,
};
prog_char disRTable[]=
//{12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};
{
	39,38,37,36,
	35,34,33,32,
	31,30,29,28,
	27,26,25,24,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23,
};
prog_char KeyRTable[]={8,9,10,11};
#endif
// w:8,h:7
#define CHARIMAGE_BYTECOUNT 7

prog_char chartable[]=
{
	//0
	0x0C,0x12,0x12,0x12,0x12,0x12,0x0C,
	//1
	0x04,0x06,0x04,0x04,0x04,0x04,0x0E,
	//2
	0x0C,0x12,0x10,0x08,0x04,0x02,0x1E,
	//3
	0x0C,0x12,0x10,0x0C,0x10,0x12,0x0C,
	//4
	0x08,0x0C,0x0A,0x09,0x1F,0x08,0x08,
	//5
	0x1E,0x02,0x0E,0x10,0x10,0x12,0x0C,
	//6
	0x0E,0x01,0x01,0x0F,0x11,0x11,0x0E,
	//7
	0x1E,0x08,0x08,0x04,0x04,0x04,0x04,
	//8
	0x0C,0x12,0x12,0x0C,0x12,0x12,0x0C,
	//9
	0x0C,0x12,0x12,0x1C,0x10,0x10,0x0C,
	//A
	0x04,0x04,0x0A,0x0A,0x0E,0x0A,0x1B,
	//B
	0x0F,0x12,0x0E,0x12,0x12,0x12,0x0F,
	//C
	0x1E,0x11,0x01,0x01,0x01,0x11,0x0E,
	//D
	0x0F,0x12,0x12,0x12,0x12,0x12,0x0F,
	//E
	0x1F,0x12,0x02,0x0E,0x02,0x12,0x1F,
	//F
	0x1F,0x12,0x0A,0x0E,0x0A,0x02,0x07,
	//G
	0x0E,0x01,0x01,0x01,0x1D,0x09,0x06,
	//H
	0x37,0x12,0x12,0x1E,0x12,0x12,0x37,
	//I
	0x0E,0x04,0x04,0x04,0x04,0x04,0x0E,
	//J
	0x1C,0x08,0x08,0x08,0x08,0x08,0x0E,
	//K
	0x17,0x0A,0x06,0x06,0x0A,0x0A,0x1B,
	//L
	0x07,0x02,0x02,0x02,0x02,0x12,0x1F,
	//M
	0x33,0x1E,0x1E,0x1E,0x16,0x12,0x33,
	//N
	0x3B,0x12,0x16,0x16,0x1A,0x12,0x17,
	//O
	0x0E,0x11,0x11,0x11,0x11,0x11,0x0E,
	//P
	0x0F,0x12,0x12,0x0E,0x02,0x02,0x07,
	//Q
	0x0E,0x11,0x11,0x11,0x17,0x19,0x0E,
	//R
	0x0F,0x12,0x0E,0x0A,0x12,0x12,0x37,
	//S
	0x1C,0x12,0x02,0x0C,0x10,0x12,0x0E,
	//T
	0x1F,0x15,0x04,0x04,0x04,0x04,0x0E,
	//U
	0x33,0x12,0x12,0x12,0x12,0x12,0x0C,
	//V
	0x1B,0x0A,0x0A,0x0A,0x0A,0x04,0x04,
	//W
	0x3F,0x25,0x25,0x1A,0x1A,0x12,0x12,
	//X
	0x1B,0x0A,0x0A,0x04,0x0A,0x0A,0x1B,
	//Y
	0x1B,0x0A,0x0A,0x04,0x04,0x04,0x0E,
	//Z
	0x1F,0x09,0x04,0x04,0x02,0x12,0x1F,
	//欢
	0x20,0x30,0x6C,0x7C,0x18,0x7E,0x88,
	//迎
	0x10,0x52,0xAE,0x76,0x6A,0x24,0xF8,
	//使
	0x30,0x34,0x6C,0x36,0x1A,0x1E,0x76,
	//用
	0x20,0x5C,0x74,0x5C,0x7C,0x54,0x70,
	//中
	0x10,0x70,0x58,0x34,0x1C,0x10,0x10,
	//文
	0x00,0x10,0x38,0x18,0x1E,0x32,0xCC,
	//音
	0x10,0x30,0x38,0xF8,0x36,0x38,0x28,
	//响
	0x00,0x10,0xF6,0xAE,0xB6,0xB8,0x40,
	//-
	0x00,0x00,0x00,0x7E,0x00,0x00,0x00,
	// 
	0,0,0,0,0,0,0,
};
static void disSetBit(uint8 lc)
{
	disdevmem[lc/8]|=(1<<lc%8);
}
#if 0
static void disClrBit(uint8 lc)
{
	disdevmem[lc/8]&=~(1<<lc%8);
}
#endif
//static void disclall()
//{
//	uint8 i=0;
//	for(;i<sizeof(disdevmem);i++)
//	{
//		disdevmem[i]=0;
//	}
//}
#ifdef POINTVFDDISPLAY_KEY1_P
//void IniKEYInput()
//{
//	//按键
////	pointvfddisplayKEY1PORT|=_BV(POINTVFDDISPLAY_KEY1_P);
////#ifdef POINTVFDDISPLAY_KEY2_P
////	pointvfddisplayKEY2PORT|=_BV(POINTVFDDISPLAY_KEY2_P);
////#endif
//}
void getKey()
{
	//按键
	u8 kc = 0;
	if(!(pointvfddisplayKEY1PIN&_BV(POINTVFDDISPLAY_KEY1_P)))
	{
		kc =keyP+1;
	}
#ifdef POINTVFDDISPLAY_KEY2_P
	else if(!(pointvfddisplayKEY2PIN&_BV(POINTVFDDISPLAY_KEY2_P)))
	{
		kc =sizeof(KeyRTable)+keyP+1;
	}
#endif
	//KeyCode =0;

	keyP++;
	if(kc==0)
	{
		if(keyP==KeyCode||(keyP+sizeof(KeyRTable))==KeyCode)KeyCode = 0;
	}
	else
	{
		KeyCode = kc;
	}
	if(keyP>=sizeof(KeyRTable))
		keyP = 0;
	disSetBit(pgm_read_byte(KeyRTable+keyP));
}
#endif
void nextdisG(uint8 topline)
{
	uint8 rp = 0;
	u8 dismemP;
	uint8 dat;
	uint8 bc;
	uint8 lc;

	//栅极
	disG++;
	if(disG>=sizeof(disGTable))
		disG = 0;
	//开栅极

	//
#if 0
	//disdevmem[0]=0xff;
	//disClrBit(pgm_read_byte(disGTable+disG));
	dismemP = disG*POINTVFDDISPLAY_X_P;
	for(lc=0;lc<POINTVFDDISPLAY_X_P;lc++)
	{
		if(TestBit(dismemP,dismem))
		{
			disSetBit(pgm_read_byte(disRTable+lc));
		}
		//rp++;
		dismemP+=1;
	}
#else
	disSetBit(pgm_read_byte(disGTable+disG));
	dismemP = disG/2+topline*sizeof(disGTable)/2;
	for(lc=0;lc<POINTVFDDISPLAY_Y_P;lc++)
	{
		dat = dismem[dismemP];
		dismemP+=sizeof(disGTable)/2;
		if(disG%2)//第二
		{
			dat=dat>>4;
		}
		for(bc=0;bc<4;bc++)
		{
			if(dat&1)
			{
				disSetBit(pgm_read_byte(disRTable+rp));
			}
			rp++;
			dat=dat>>1;
		}
	}
#endif
}
void Pollingpointvfddisplay()//1ms
{
	if(stf16360enIdle)
	{
		//disclall();
		dwmemset(disdevmem,0,sizeof(disdevmem));
		getKey();
		nextdisG(0);
		//stf16360enWrite(disdevmem,sizeof(disdevmem));
		stf16360enWrite();
	}
}
uint8 charimg[CHARIMAGE_BYTECOUNT];
void showChar(uint8 lcX)
{
#define showCharmak (uint8)( ((uint8)(0xff<<(8-CHARIMAGE_W))) >> (8-CHARIMAGE_W) )
	uint8 byteinx = lcX/8;
	uint8 bitinx  = lcX%8;
	uint8 i = 0;
	uint8 dat,datS;
	for(;i<CHARIMAGE_BYTECOUNT;i++)
	{
		dat = dismem[byteinx];
		datS= charimg[i];
		//dat&=~(0b111111<<bitinx);
		dat&=~(showCharmak<<bitinx);
		dat|=datS<<bitinx;
		dismem[byteinx]=dat;
		dat = dismem[byteinx+1];
		//dat&=~(0b111111>>(8-bitinx));
		dat&=~(showCharmak>>(8-bitinx));
		dat|=datS>>(8-bitinx);
		dismem[byteinx+1]=dat;
		byteinx+=(POINTVFDDISPLAY_X_P+bitinx)/8;
		bitinx = (POINTVFDDISPLAY_X_P+bitinx)%8;
	}
}
//只支持大写字母
void ShowString(char const*s,uint8 lcX,uint8 charCount)
{
	//	uint8 i;
	char c;
	bool endl = false;
	//prog_char* font;
	//	DisplayChanged = true;
	if(charCount)
	{
		do
		{
			if(endl)
			{
				c=WC_kongge;
			}
			else
			{
				c = *s++;
			}
			if( (c>='A')&&(c<WC_kongge) )
				c=c-'A'+10;
			/*	else if( (c>='a')&&(c<='z'))
			c=vfd_char[c-0x20-'A'];*/
			else if( (c>='0')&&(c<='9') )
				c=c-'0';
			else if(c=='-')
				c=WC_jianhao-'A'+10;
			/*else if(c=='_')
			c=vfdc[27];*/
			else if(c=='\0')
			{
				endl=true;
				c=WC_kongge-'A'+10;
			}
			else
				c=WC_kongge-'A'+10;
			memcpy_P(charimg,chartable+c*CHARIMAGE_BYTECOUNT,CHARIMAGE_BYTECOUNT);
			showChar(lcX);
			lcX+=CHARIMAGE_W;
		}while(--charCount);

		//if(len)
		//{
		//	do
		//	{
		//		*out++=0;
		//		//stp++;
		//	}while(--len);
		//}
	}
	//SetdislinebufTo(' ');//注意清除了dislinebuf
	/*for loop*/
	//setOffsideToNull(i);
}
//只支持大写字母
void ShowString_P(const prog_char *s,uint8 lcX,uint8 charCount)
{
	memcpy_P(stringbuff,s,charCount);
	ShowString(stringbuff,lcX,charCount);
}
//只支持大写字母
void ShowStringALL_P(const prog_char *s)
{
	ShowString_P(s,0,POINTVFDDISPLAY_X_C);
}

bool DrawColor=true;
#if 0
bool inline cheackDotInDis(IndexScreenLine x,IndexScreenLine y)
{
	return x<POINTVFDDISPLAY_X_P
		&&y<POINTVFDDISPLAY_Y_P
		&&x>=0
		&&y>=0;
}
void DrawDot(IndexScreenLine x,IndexScreenLine y)
{
	uint16 dot ;
	u8 dat,dat2;
	u8 add;
	if(cheackDotInDis(x,y))
	{
		dot = y*POINTVFDDISPLAY_X_P+x;
		add = dot/8;
		dat = dismem[add];
		dat2=_BV(dot%8);
		if(DrawColor)
			dismem[add] = dat|dat2;
		else
			dismem[add] =dat&(~dat2);
	}
}

void DrawLine(IndexScreenLine xa, IndexScreenLine ya, IndexScreenLine xb, IndexScreenLine yb)
{
#if 1
	IndexScreenLine e,i,drw,drs,tmp;
	IndexScreenLine	Wstep = 1,Sstep = 1;
	bool chxy = false;
	drw=xb-xa;
	drs=yb-ya;

	if(drw<0)
	{
		drw = -drw;
		Wstep = -1;
	}
	if(drs<0)
	{
		drs = -drs;
		Sstep = -1;
	}
	if(drw<drs)
	{
		tmp = drw;
		drw=drs;
		drs = tmp;
		tmp = xa;
		xa = ya;
		ya = tmp;
		tmp = Wstep;
		Wstep = Sstep;
		Sstep = tmp;
		chxy = true;
	}
	e = drw;
	if(chxy)
	{
		for(i=0;i<drw;i++)
		{
			DrawDot(ya,xa);
			xa+=Wstep;
			e-=drs;
			if(e<0)
			{
				ya+=Sstep;
				e+=drw;
			}
		}
	}
	else
	{
		for(i=0;i<drw;i++)
		{
			DrawDot(xa,ya);
			xa+=Wstep;
			e-=drs;
			if(e<0)
			{
				ya+=Sstep;
				e+=drw;
			}
		}
	}
#else
	IndexScreenLine dx,dy,e,i,drw,drs;
	IndexScreenLine	Wstep = 1,Sstep = 1;
	IndexScreenLine *dwP,*dsP;
	dx=xb-xa;
	dy=yb-ya;
	if(abs(dx)>abs(dy))
	{
		dwP = &xa;dsP = &ya;
		drw = dx;
		drs = dy;
	}
	else
	{
		dwP = &ya;dsP = &xa;
		drw = dy;
		drs = dx;
	}
	if(drw<0)
	{
		drw = -drw;
		Wstep = -1;
	}
	if(drs<0)
	{
		drs = -drs;
		Sstep = -1;
	}
	e = drw;
	for(i=0;i<drw;i++)
	{
		DrawDot(xa,ya);
		*dwP+=Wstep;
		e-=drs;
		if(e<0)
		{
			*dsP+=Sstep;
			e+=drw;
		}
	}
#endif
}

#define exch(a,b) {tmp = a;a = b; b = tmp;}

void DrawRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2)
{

	DrawLine(x1,y1,x2,y1);
	DrawLine(x2,y1,x2,y2);
	DrawLine(x2,y2,x1,y2);
	DrawLine(x1,y2,x1,y1);
}
void FillRectangle(IndexScreenLine x1,IndexScreenLine y1,IndexScreenLine x2,IndexScreenLine y2)
{
	/*IndexScreenLine tmp;
	if(y1>y2)
	{
	tmp = 	y1;
	y1 = y2;
	y2 = tmp;
	}
	while(y1<=y2)
	{
	DrawLine(x1,y1,x2,y1);
	y1++;
	}*/
	uint16 lc;
	uint8 byteinx,bitinx,byteinx_tmp;
	IndexScreenLine dy,dx,tmp;
	if(x1<0)
	{
		x1=0;
	}
	if(y1<0)
	{
		y1=0;
	}
	if(x2>POINTVFDDISPLAY_X_P)
	{
		x2=POINTVFDDISPLAY_X_P;
	}
	if(y2>POINTVFDDISPLAY_Y_P)
	{
		y2=POINTVFDDISPLAY_Y_P;
	}
	dy = y2 - y1;
	dx = x2 - x1;
	if(dy==0||dx==0)
		return;
	if(dy<0)
	{
		y1=y2;
		dy = -dy;
	}
	if(dx<0)
	{
		x1=x2;
		dx = -dx;
	}
	lc = y1*POINTVFDDISPLAY_X_P+x1;
	byteinx = lc/8;
	bitinx  = lc%8;
	do
	{
		tmp = dx;
		byteinx_tmp =byteinx;
		uint8 stratBc = 8-bitinx;
		if(stratBc>dx)
		{
			stratBc = dx;
		}
		tmp-=stratBc;
		dismem[byteinx_tmp++]|=(((uint8)(0xff<<(8-stratBc)))>>(8-stratBc))<<bitinx;
		while(tmp>7)
		{
			dismem[byteinx_tmp++]|=0xff;
			tmp-=8;
		}
		if(tmp>0)
			dismem[byteinx_tmp]|=((uint8)(0xff<<(8-tmp)))>>(8-tmp);
		byteinx+=(POINTVFDDISPLAY_X_P+bitinx)/8;
		bitinx = (POINTVFDDISPLAY_X_P+bitinx)%8;
	}
	while(--dy);

}
#endif

ISR(TIMER2_COMP_vect)
{
	sei();
	Pollingpointvfddisplay();
}
#define SHOW_INT_XP 48
//void ShowINT8(i8 sd)
//{
//	u8 l=ToStringWithD(stringbuff,sd);
//	ShowString(stringbuff,POINTVFDDISPLAY_X_P-l*CHARIMAGE_W,l);
//}
void ShowStringAndI8_P2(const prog_char* s1,const prog_char* s2, i8 d)
{
	u8 l=ToStringWithD(stringbuff,d);
	ShowString(stringbuff,POINTVFDDISPLAY_X_P-l*CHARIMAGE_W,l);
	ShowString_P2(s1,s2,POINTVFDDISPLAY_X_C-l);
}
void ShowString_P2(const prog_char* s1,const prog_char* s2,u8 sCount)
{
	strcpy_P(stringbuff,s1);
	strcat_P(stringbuff,s2);
	ShowString(stringbuff,0,sCount);
}

