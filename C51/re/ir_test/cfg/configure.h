#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
//#include <REG2051.H>
#include <REGX52.H>
//#include <REG2051.H>

#include <intrins.h>
#include "TypeDefing.h"



#define CLOCK_M(c) (c*1000000)

//#define _CPU_CLOCK_ 11059200
#define _CPU_CLOCK_ CLOCK_M(12)
#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_/12)
#define F_CPU _CPU_INSTRUCTION_CLOCK_
//ÿ����΢����
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//ÿ΢��������
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)
#define I2C_V2//ʹ�ð汾2
#define MULTI_AT24C02			//���AT24C02����ʹ��24C04����

#define VSPI2_LSB_U		1	//ʹ��LSB
#define VSPI2_MODE_3 //��ʼ������ ( �½���) �����ز��� ( ������) SCK��̬1

#define DIS_LINEBUF_SIZE		10//�ַ����������С
#define	MaxUserEventCountDown 15//�û��¼����������󵹼�ʱ����λʱ��s��ָ��ʱ�����򽫽��볣������
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//PIN PIN
//�������ݽӿ�����tm1628
#define SDA_VSPI2 P0_2=P2_5=P3_5=P1_2
#define SCK_VSPI2 P0_3=P2_4=P3_4=P1_1
#define SDA_VSPI2_GET (P1_2&&P3_5)//P0_2&P2_5&

//�������ݽӿ�����
#define SDA_I2C_PIN (P2^1)
#define SCL_I2C_PIN (P2^2)

#define RecordL		P1_5
#define StandardL	P1_4
#define CompL		P1_3


#endif//_CONFIGURE_H_
