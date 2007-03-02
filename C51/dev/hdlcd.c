//-------------------------------------------------------------------//
//�ꡡ��: 16X2�ַ�Һ��8Bit�ӿڳ���MCS51ģ��IO��ʽ��                        //
//�桡��: V1.00                                                                            //
//�ļ���: xwj_lcd16.C                                                                    //
//������: ���ľ�                         E-mail:xuwenjun@21cn.com              //
//�ա���: 02-11-5                                                                         //
//�衡��: 16X2�ַ�Һ��8Bit�ӿڳ���MCS51ģ��IO��ʽ��                        //
//-------------------------------------------------------------------//
//HD44780ָ��һ����:
//RS=0;R/W=0----д��ָ��:
//              16����    D7   D6   D5   D4   D3   D2   D1   D0   ����ʱ��(250KHZ)
//����          01        0    0    0    0    0    0    0    1    1.64MS
//��Home        02        0    0    0    0    0    0    1    0    1.64MS
//�������뷽ʽ  04~07     0    0    0    0    0    1    I/D  S    40US
//                        I/D=0:AC��1:AC��;S=0:��ֹ����1:�������
//������ʾ״̬  08~0F     0    0    0    0    1    D    C    B    40US
//                        D=0:����ʾ1:����ʾ;C=0:�����ʧ1:�����ʾ;B=0:��˸��ֹ1:��˸����
//���������10~1F     0    0    0    1    S/C  R/L  *    *    40US    (*Ϊ����ֵ)
//                        S/C=0:�������1:������;R/L=0:�������1:���ҹ���
//���ù�����ʽ  20~3F     0    0    1    DL   N   F     *    *    40US
//                        DL=0:4λ����1:8λ����;N=0:һ���ַ�1:�����ַ�;
//                        F=0:5X7��������1:5X10��������
//����CGRAM��ַ 40~7F     0    1    A5   A4   A3   A2   A1   A0   40US
//����DDRAM��ַ 80~FF     1    A6   A5   A4   A3   A2   A1   A0   40US
//RS=0;R/W=1----��״̬    BF   AC6  AC5  AC4  AC3  AC2  AC1  AC0  0US    (BF=0:����1:æ)
//RS=1;R/W=0----д����
//RS=1;R/W=1----������
//--------------------------------------------------------------------------//

//  P2��16X2�ַ�Һ��8Bit -- �洢������  //
unsigned char code lcd16_tab_hex[]={"0123456789ABCDEF"}; 
unsigned char code lcd16_tab_cg[]={
	0x0e,0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,    //"Ԫ"����
	0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,    //"��"����
	0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,    //"��"����
	0x1f,0x11,0x11,0x1f,0x11,0x11,0x1f,0x00,    //"��"����
	0x00,0x1F,0x15,0x15,0x1B,0x11,0x1F,0x00,    //"��"����
	0x1F,0x04,0x04,0x1E,0x0A,0x12,0x1F,0x00,    //"��"����
	0x08,0x04,0x1F,0x00,0x0A,0x11,0x00,0x00,    //"��"����
	0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00,    //"\"����

	0x01,0x02,0x05,0x02,0x06,0x02,0x02,0x02,    //�Զ����ַ�
	0x04,0x0A,0x1F,0x04,0x1F,0x0E,0x15,0x0C,    //�Զ����ַ�
	0x01,0x0F,0x04,0x02,0x01,0x02,0x04,0x08,    //�Զ����ַ�
	0x00,0x1E,0x08,0x10,0x00,0x10,0x08,0x06,    //�Զ����ַ�
	0x0F,0x12,0x0F,0x04,0x0F,0x00,0x0F,0x00,    //�Զ����ַ�
	0x1F,0x12,0x1C,0x10,0x1E,0x10,0x1F,0x10,    //�Զ����ַ�
};
//--------------------------------------------------------------------------//

//  P2��16X2�ַ�Һ��8Bit -- IO����  //
#ifndef P2
#include <reg51.h>               //
#include <intrins.h>             //ʹ����_nop_()
#endif

#define LCD16_VD P2                  //LCD16_VD------------P2��4λ//
sbit LCD16_RS = P1^2;                //LCD16_RS------------P1.2   //
sbit LCD16_RW = P1^3;                //LCD16_RW------------P1.3   //
sbit LCD16_E  = P1^0;                //LCD16_E-------------P1.0   //
//--------------------------------------------------------------------------//

//  P2��16X2�ַ�Һ��8Bit -- ����ԭ��  //
void lcd16_ctrl(unsigned char c);    //ָ��д�뺯��(  ���æ�ź�)//
void lcd16_ctrln(unsigned char c);   //ָ��д�뺯��(�����æ�ź�)//
void lcd16_wd(unsigned char d);      //���ݺ�ָ�����д�뺯��    //
unsigned char lcd16_readc(void);     //״̬�͵�ַ��������        //
//unsigned char lcd16_readd(void);   //���ݶ�������              //
void lcd16_reset(void);              //LCD��λ                   //
void lcd16_initcg(void);             //��ʼ���Զ����ַ�          //
void lcd16_writecg(unsigned char x,unsigned char y,unsigned char n);//��ʾ�Զ����ַ� //
void lcd16_wdh(unsigned char d);     //1λ16��������д�뺯��     //
void lcd16_wdhh(unsigned char d);    //2λ16��������д�뺯��        //
void lcd16_pos(unsigned char xpos,unsigned char ypos);     // ���㲢д���ַ����������λ�ã��ڼ���  //
void lcd16_write(unsigned char,unsigned char,unsigned char);  // ����ַдһ���ַ�����������λ�ã��������ַ� //
void lcd16_writen(unsigned char,unsigned char,unsigned char); // ����ַдһ�����֣���������λ�ã����������� //
void lcd16_fill(unsigned char cf);   // ����д cf ���ַ� //
void lcd16_display(unsigned char sx,unsigned char sy,unsigned char  *sc);    //����ַдһ���ַ�������������λ�ã��������ַ���//
void lcd16_delay(unsigned int x);    //��ʱ x ����
//--------------------------------------------------------------------------//

//  P2��16X2�ַ�Һ��8Bit -- ����  //
//--------------------------------------------------------------------------//
void lcd16_ctrl(unsigned char c)     //ָ��д�뺯��//
{
	unsigned char i;
	while ((lcd16_readc()) > 0x7f) {if (--i == 0) break;}    //���æ�ź�//
	LCD16_RS = 0;                    //CS = 1 RS = 0 RW = 0����д��ָ��//
	LCD16_RW = 0;
	LCD16_VD = c;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
void lcd16_ctrln(unsigned char c)    //ָ��д�뺯��,�����æ�ź�//
{
	LCD16_RS = 0;                   //CS = 1 RS = 0 RW = 0����д��ָ��//
	LCD16_RW = 0;
	LCD16_VD = c;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
void lcd16_wd(unsigned char d)       //���ݺ�ָ�����д�뺯��//
{
	unsigned char i;
	while ((lcd16_readc()) > 0x7f) {if (--i == 0) break;}    //���æ�ź�//
	LCD16_RS = 1;                    //CS = 1 RS = 1 RW = 0����д�����ݻ�ָ�����//
	LCD16_RW = 0;
	LCD16_VD = d;LCD16_E = 1;LCD16_E = 0;
	LCD16_VD = 0xff;
}

//--------------------------------------------------------------------------//
unsigned char lcd16_readc(void)      //״̬�͵�ַ��������//
{unsigned char rc;

#ifdef DEBUG
rc=0;                            //��ֹ����rcδ���õľ���
return(rc);
#endif
LCD16_RS = 0;                    //RSΪ1���������,RSΪ0�����״̬��־�͹���ַ//
LCD16_RW = 1;
LCD16_VD = 0xff;                 //��P2��Ϊ�ߵ�ƽ��ֻ����Ϊ�ߵ�ƽ������ȷ��������//
LCD16_E = 1;rc = LCD16_VD;LCD16_E = 0;
return(rc);
}

/*
//--------------------------------------------------------------------------//
unsigned char lcd16_readd(void)      //���ݶ�������//
{unsigned char rd;
LCD16_RS = 1;                    //RSΪ1���������,RSΪ0�����״̬��־�͹���ַ//
LCD16_RW = 1;    
LCD16_VD = 0xff;                 //��P2��Ϊ�ߵ�ƽ��ֻ����Ϊ�ߵ�ƽ������ȷ��������//
LCD16_E = 1;rd = LCD16_VD;LCD16_E = 0;
return(rd);
}
*/

//--------------------------------------------------------------------------//
void lcd16_reset(void)               //  LCD��λ                 //
{
	lcd16_delay(15);
	lcd16_ctrln(0x38);               //��ʾģʽ����(�����æ�ź�)//
	lcd16_delay(5);                  //�����Σ�ȷ����*��λ      //
	lcd16_ctrln(0x38);
	lcd16_delay(5);
	lcd16_ctrln(0x38);
	lcd16_delay(5);
	lcd16_ctrl(0x38);                //��ʾģʽ����(�Ժ�����æ�ź�)//
	lcd16_ctrl(0x08);                //   ��ʾ�ر�               //
	lcd16_ctrl(0x01);                //   ��ʾ����               //
	lcd16_ctrl(0x06);                //   ��ʾ����ƶ�����       //
	lcd16_ctrl(0x0c);                //   ��ʾ�����������       //
	//    lcd16_initcg();
}

//--------------------------------------------------------------------------//
void lcd16_wdh(unsigned char d)      //1λ16��������д�뺯��//
{
	lcd16_wd(lcd16_tab_hex[(d & 0x0f)]);
}

//--------------------------------------------------------------------------//
void lcd16_wdhh(unsigned char d)     //2λ16��������д�뺯��//
{
	lcd16_wd(lcd16_tab_hex[(d >> 4)]);
	lcd16_wd(lcd16_tab_hex[(d & 0x0f)]);
}

//--------------------------------------------------------------------------//
void lcd16_initcg(void)              //��ʼ���Զ����ַ�          //
{
	unsigned char i;
	lcd16_ctrl(0x40);
	for (i=0;i<64;i++)
	{
		lcd16_wd(lcd16_tab_cg);
	}
}

//--------------------------------------------------------------------------//
void lcd16_writecg(unsigned char x,unsigned char y,unsigned char n)    //��ʾ�Զ����ַ� //
{
	unsigned char i;
	lcd16_ctrl(0x40 + x * 8);        //дCGRAM��0~7��Ԫ(�� 1 ���ַ�)
	for (i=0;i<7;i++)
	{
		lcd16_wd(lcd16_tab_cg[n * 8 + i]);
	}
	lcd16_pos(x,y);
	lcd16_wd(x);
}

//--------------------------------------------------------------------------//
void lcd16_pos(unsigned char xpos,unsigned char ypos)
{                                // ���㲢д���ַ����������λ�ã��ڼ���  //
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
{                                // ����ַдһ���ַ�����������λ�ã��������ַ�//
	lcd16_pos(x,y);
	lcd16_wd(c);
}  

//--------------------------------------------------------------------------//
void lcd16_writen(unsigned char x,unsigned char y,unsigned char c) 
{                                // ����ַдһ���ַ�����������λ�ã��������ַ�//
	lcd16_pos(x,y);
	if ((c & 0xf0) != 0) lcd16_wdh(c >> 4);
	lcd16_wdh(c & 0x0f);
}  

//--------------------------------------------------------------------------//
void lcd16_fill(unsigned char cf)    // ����д cf ���ַ� //
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
{                                //����ַдһ���ַ�������������λ�ã��������ַ���//
	lcd16_pos(sx,sy);
	while((*sc !=  0x00) && (sx < 16))
	{
		lcd16_wd(*sc++);
		sx++;
	}
}

//--------------------------------------------------------------------------//
void lcd16_delay(unsigned int x)     //��ʱ x ����
{
	//    ��ʱ����������:    j=10ʱ:6+50*x    j=100ʱ:6+410*x    ��6+(10+4*j)*x
	unsigned char j;
	do {
		for (j=0; j<230; j++) _nop_(); //j=(11159KHz/12-10)/4
	}while(--x);
}