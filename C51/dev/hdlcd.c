//-------------------------------------------------------------------//
//标　题: 16X2字符液晶8Bit接口程序（MCS51模拟IO方式）                        //
//版　本: V1.00                                                                            //
//文件名: xwj_lcd16.C                                                                    //
//创建人: 徐文军                         E-mail:xuwenjun@21cn.com              //
//日　期: 02-11-5                                                                         //
//描　述: 16X2字符液晶8Bit接口程序（MCS51模拟IO方式）                        //
//-------------------------------------------------------------------//
//HD44780指令一览表:
//RS=0;R/W=0----写入指令:
//              16进制    D7   D6   D5   D4   D3   D2   D1   D0   运行时间(250KHZ)
//清屏          01        0    0    0    0    0    0    0    1    1.64MS
//归Home        02        0    0    0    0    0    0    1    0    1.64MS
//设置输入方式  04~07     0    0    0    0    0    1    I/D  S    40US
//                        I/D=0:AC减1:AC加;S=0:禁止滚动1:允许滚动
//设置显示状态  08~0F     0    0    0    0    1    D    C    B    40US
//                        D=0:关显示1:开显示;C=0:光标消失1:光标显示;B=0:闪烁禁止1:闪烁启用
//光标或画面滚动10~1F     0    0    0    1    S/C  R/L  *    *    40US    (*为任意值)
//                        S/C=0:画面滚动1:光标滚动;R/L=0:向左滚动1:向右滚动
//设置工作方式  20~3F     0    0    1    DL   N   F     *    *    40US
//                        DL=0:4位总线1:8位总线;N=0:一行字符1:二行字符;
//                        F=0:5X7点阵字体1:5X10点阵字体
//设置CGRAM地址 40~7F     0    1    A5   A4   A3   A2   A1   A0   40US
//设置DDRAM地址 80~FF     1    A6   A5   A4   A3   A2   A1   A0   40US
//RS=0;R/W=1----读状态    BF   AC6  AC5  AC4  AC3  AC2  AC1  AC0  0US    (BF=0:空闲1:忙)
//RS=1;R/W=0----写数据
//RS=1;R/W=1----读数据
//--------------------------------------------------------------------------//

//  P2口16X2字符液晶8Bit -- 存储器定义  //
unsigned char code lcd16_tab_hex[]={"0123456789ABCDEF"}; 
unsigned char code lcd16_tab_cg[]={
	0x0e,0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,    //"元"代码
	0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,    //"年"代码
	0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,    //"月"代码
	0x1f,0x11,0x11,0x1f,0x11,0x11,0x1f,0x00,    //"日"代码
	0x00,0x1F,0x15,0x15,0x1B,0x11,0x1F,0x00,    //"四"代码
	0x1F,0x04,0x04,0x1E,0x0A,0x12,0x1F,0x00,    //"五"代码
	0x08,0x04,0x1F,0x00,0x0A,0x11,0x00,0x00,    //"六"代码
	0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00,    //"\"代码

	0x01,0x02,0x05,0x02,0x06,0x02,0x02,0x02,    //自定义字符
	0x04,0x0A,0x1F,0x04,0x1F,0x0E,0x15,0x0C,    //自定义字符
	0x01,0x0F,0x04,0x02,0x01,0x02,0x04,0x08,    //自定义字符
	0x00,0x1E,0x08,0x10,0x00,0x10,0x08,0x06,    //自定义字符
	0x0F,0x12,0x0F,0x04,0x0F,0x00,0x0F,0x00,    //自定义字符
	0x1F,0x12,0x1C,0x10,0x1E,0x10,0x1F,0x10,    //自定义字符
};
//--------------------------------------------------------------------------//

//  P2口16X2字符液晶8Bit -- IO定义  //
#ifndef P2
#include <reg51.h>               //
#include <intrins.h>             //使用了_nop_()
#endif

#define LCD16_VD P2                  //LCD16_VD------------P2高4位//
sbit LCD16_RS = P1^2;                //LCD16_RS------------P1.2   //
sbit LCD16_RW = P1^3;                //LCD16_RW------------P1.3   //
sbit LCD16_E  = P1^0;                //LCD16_E-------------P1.0   //
//--------------------------------------------------------------------------//

//  P2口16X2字符液晶8Bit -- 函数原型  //
void lcd16_ctrl(unsigned char c);    //指令写入函数(  检测忙信号)//
void lcd16_ctrln(unsigned char c);   //指令写入函数(不检测忙信号)//
void lcd16_wd(unsigned char d);      //数据和指令参数写入函数    //
unsigned char lcd16_readc(void);     //状态和地址读出函数        //
//unsigned char lcd16_readd(void);   //数据读出函数              //
void lcd16_reset(void);              //LCD复位                   //
void lcd16_initcg(void);             //初始化自定义字符          //
void lcd16_writecg(unsigned char x,unsigned char y,unsigned char n);//显示自定义字符 //
void lcd16_wdh(unsigned char d);     //1位16进制数字写入函数     //
void lcd16_wdhh(unsigned char d);    //2位16进制数字写入函数        //
void lcd16_pos(unsigned char xpos,unsigned char ypos);     // 计算并写入地址，参数：列位置，第几行  //
void lcd16_write(unsigned char,unsigned char,unsigned char);  // 带地址写一个字符，参数：列位置，行数，字符 //
void lcd16_writen(unsigned char,unsigned char,unsigned char); // 带地址写一个数字，参数：列位置，行数，数字 //
void lcd16_fill(unsigned char cf);   // 整屏写 cf 中字符 //
void lcd16_display(unsigned char sx,unsigned char sy,unsigned char  *sc);    //带地址写一个字符串，参数：列位置，行数，字符串//
void lcd16_delay(unsigned int x);    //延时 x 毫秒
//--------------------------------------------------------------------------//

//  P2口16X2字符液晶8Bit -- 函数  //
//--------------------------------------------------------------------------//
void lcd16_ctrl(unsigned char c)     //指令写入函数//
{
	unsigned char i;
	while ((lcd16_readc()) > 0x7f) {if (--i == 0) break;}    //检测忙信号//
	LCD16_RS = 0;                    //CS = 1 RS = 0 RW = 0代表写入指令//
	LCD16_RW = 0;
	LCD16_VD = c;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
void lcd16_ctrln(unsigned char c)    //指令写入函数,不检测忙信号//
{
	LCD16_RS = 0;                   //CS = 1 RS = 0 RW = 0代表写入指令//
	LCD16_RW = 0;
	LCD16_VD = c;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
void lcd16_wd(unsigned char d)       //数据和指令参数写入函数//
{
	unsigned char i;
	while ((lcd16_readc()) > 0x7f) {if (--i == 0) break;}    //检测忙信号//
	LCD16_RS = 1;                    //CS = 1 RS = 1 RW = 0代表写入数据或指令参数//
	LCD16_RW = 0;
	LCD16_VD = d;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
unsigned char lcd16_readc(void)      //状态和地址读出函数//
{unsigned char rc;

#ifdef DEBUG
rc=0;                            //防止出现rc未调用的警告
return(rc);
#endif
LCD16_RS = 0;                    //RS为1代表读数据,RS为0代表读状态标志和光标地址//
LCD16_RW = 1;
LCD16_VD = 0xff;                 //把P2置为高电平，只有置为高电平才能正确读入数据//
LCD16_E = 1;rc = LCD16_VD;LCD16_E = 0;
return(rc);
}

/*
//--------------------------------------------------------------------------//
unsigned char lcd16_readd(void)      //数据读出函数//
{unsigned char rd;
LCD16_RS = 1;                    //RS为1代表读数据,RS为0代表读状态标志和光标地址//
LCD16_RW = 1;    
LCD16_VD = 0xff;                 //把P2置为高电平，只有置为高电平才能正确读入数据//
LCD16_E = 1;rd = LCD16_VD;LCD16_E = 0;
return(rd);
}
*/

//--------------------------------------------------------------------------//
void lcd16_reset(void)               //  LCD复位                 //
{
	lcd16_delay(15);
	lcd16_ctrln(0x38);               //显示模式设置(不检测忙信号)//
	lcd16_delay(5);                  //共三次，确保可*复位      //
	lcd16_ctrln(0x38);
	lcd16_delay(5);
	lcd16_ctrln(0x38);
	lcd16_delay(5);
	lcd16_ctrl(0x38);                //显示模式设置(以后均检测忙信号)//
	lcd16_ctrl(0x08);                //   显示关闭               //
	lcd16_ctrl(0x01);                //   显示清屏               //
	lcd16_ctrl(0x06);                //   显示光标移动设置       //
	lcd16_ctrl(0x0c);                //   显示开及光标设置       //
	//    lcd16_initcg();
}

//--------------------------------------------------------------------------//
void lcd16_wdh(unsigned char d)      //1位16进制数字写入函数//
{
	lcd16_wd(lcd16_tab_hex[(d & 0x0f)]);
}

//--------------------------------------------------------------------------//
void lcd16_wdhh(unsigned char d)     //2位16进制数字写入函数//
{
	lcd16_wd(lcd16_tab_hex[(d >> 4)]);
	lcd16_wd(lcd16_tab_hex[(d & 0x0f)]);
}

//--------------------------------------------------------------------------//
void lcd16_initcg(void)              //初始化自定义字符          //
{
	unsigned char i;
	lcd16_ctrl(0x40);
	for (i=0;i<64;i++)
	{
		lcd16_wd(lcd16_tab_cg);
	}
}

//--------------------------------------------------------------------------//
void lcd16_writecg(unsigned char x,unsigned char y,unsigned char n)    //显示自定义字符 //
{
	unsigned char i;
	lcd16_ctrl(0x40 + x * 8);        //写CGRAM的0~7单元(第 1 个字符)
	for (i=0;i<7;i++)
	{
		lcd16_wd(lcd16_tab_cg[n * 8 + i]);
	}
	lcd16_pos(x,y);
	lcd16_wd(x);
}

//--------------------------------------------------------------------------//
void lcd16_pos(unsigned char xpos,unsigned char ypos)
{                                // 计算并写入地址，参数：列位置，第几行  //
	unsigned char lcd16_add;
	xpos &=  0x0f;
	ypos &=  0x01;
	if
		(ypos !=  0) lcd16_add = ((xpos+0x40)|0x80);
	else
		lcd16_add = xpos|0x80;
	lcd16_ctrl(lcd16_add);
}

//--------------------------------------------------------------------------//
void lcd16_write(unsigned char x,unsigned char y,unsigned char c) 
{                                // 带地址写一个字符，参数：列位置，行数，字符//
	lcd16_pos(x,y);
	lcd16_wd(c);
}  

//--------------------------------------------------------------------------//
void lcd16_writen(unsigned char x,unsigned char y,unsigned char c) 
{                                // 带地址写一个字符，参数：列位置，行数，字符//
	lcd16_pos(x,y);
	if ((c & 0xf0) != 0) lcd16_wdh(c >> 4);
	lcd16_wdh(c & 0x0f);
}  

//--------------------------------------------------------------------------//
void lcd16_fill(unsigned char cf)    // 整屏写 cf 中字符 //
{
	unsigned char x = 0;
	unsigned char y = 0;
	for (y = 0;y < 2;y++)
	{
		lcd16_pos(0,y);
		for (x = 0;x < 16;x++) lcd16_wd(cf);
	}
}

//--------------------------------------------------------------------------//
void lcd16_display(unsigned char sx,unsigned char sy,unsigned char *sc)    
{                                //带地址写一个字符串，参数：列位置，行数，字符串//
	lcd16_pos(sx,sy);
	while((*sc !=  0x00) && (sx < 16))
	{
		lcd16_wd(*sc++);
		sx++;
	}
}

//--------------------------------------------------------------------------//
void lcd16_delay(unsigned int x)     //延时 x 毫秒
{
	//    延时机器周期数:    j=10时:6+50*x    j=100时:6+410*x    即6+(10+4*j)*x
	unsigned char j;
	do {
		for (j=0; j<230; j++) _nop_(); //j=(11159KHz/12-10)/4
	}while(--x);
}