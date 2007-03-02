#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <avr/io.h>

#include "TypeDefing.h"
//����m8��������������Ӱ
//m8	m16
//PB2	PB4->SPI
//PB3	PB5->SPI
//PB4	PB6->SPI
//PB5	PB7->SPI
//PC5	PC0->TWI_SCL
//PC4	PC1->TWI_SDA
//PC3	PA3->ADC
//PC2	PA2->ADC
//PC1	PA1->ADC
//PC0	PA0->ADC

//------------------CLICK����------------------//
#define CLICK_CFG
//ע�������CLICK_CYCLE_US��CLOCKCS��Ҫ����cpuʱ�����м���
#define CLICK_CYCLE_US 128//ϵͳClickʱ��(us)
#define CLOCKCS	0b101
//----------------CLICK���ý���----------------//

#define _CPU_INSTRUCTION_CLOCK_ (F_CPU)

//ÿ����΢����
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//ÿ΢��������
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)

//------------dwstd ����----------------//
#define DWSTD_CFG
//����Ϊ���ɵ��ַ�����ӽ�����
#define DWSTD_ADDEND
//����ʹ�ù̶����tostringʱ����λ������ַ�Ĭ����'0'
#define DWSTD_TOSTRING_ADDCHAR '0'
//����DWSTDʹ�õ�����
#define dwstdint uint8
#define dwstdsint int8
//------------dwstd ���ý���----------------//
#define	MaxUserEventCountDown 15//�û��¼����������󵹼�ʱ����λʱ��s��ָ��ʱ�����򽫽��볣������
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//

//-------------------------I2C����-------------------------------//
#define I2C_CFG
#define I2C_SHARE_SDA_PIN	//����ʹSDA���ſ��Թ���
#define I2C_ENABLE_PULL		//����ʹ���ڲ���������
#define F_I2CSCL	100000	//TWIʱ��ΪHz
//pin def
#ifdef __AVR_ATmega16__
#define I2C_POPT	C
#define	I2C_SDA		1
#define	I2C_SCK		0
#elif defined __AVR_ATmega8__
#define I2C_POPT	C
#define	I2C_SDA		4
#define	I2C_SCK		5
#endif
//-----------------------I2C���ý���-----------------------------//

//--------------------------SPI������-------------------------------//
#define SPI_CFG
#define SPI_MASTER_U_U	1	//ʹ��SPI��������ģʽ
//pin def
#ifdef __AVR_ATmega16__
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
#elif defined __AVR_ATmega8__
#define SPI_POPT	B
#define	MISO_SPI	4
#define	MOSI_SPI	3
#define	SS_SPI		2
#define	SCK_SPI		5
#endif

//--------------------------SPI������_END----------------------------//
//-------------------PT2314����---------------------//
#define PT2314_CFG
//ѡ��һ��IC
#define IC_PT2314
//#define IC_PT2315
//#define PT2314_EXCHANGE_LR//���彻����������
//������Ա���PT2314���õ�EEPROM
#define SAVE_PT2314_SETTING
//-----------------PT2314���ý���-------------------//
//-------------------PT2314_2����---------------------//
#define PT2314_2_CFG
//ѡ��һ��IC
//#define IC_PT2314_2
#define IC_PT2315
//#define PT2314_2_EXCHANGE_LR//���彻����������
//������Ա���PT2314_2���õ�EEPROM
#define SAVE_PT2314_2_SETTING
//-----------------PT2314_2���ý���-------------------//
////-------------------M62429P����---------------------//
//#define M62429P_CFG
////Ҫ�������������λ��1
////ע��ESL_M62429P1VolumeL��EepromBuffer�б���ΪM62429P���ݵĿ�ʼ
//#define M62429P_PORT	D
//#define M62429P_DATP	3
//#define M62429P_SCKPA	{4,5}//������M62429P��sck�����������ʼ���﷨
////������Ա���M62429P���õ�EEPROM
//#define SAVE_M62429P_SETTING
////-----------------M62429P���ý���-------------------//
////---------------------------STF16360E-CFG-------------------------------//
//#define STF16360EN_CFG
////pin
//#define STF16360EN_PORT B
//#define STF16360EN_CLKP 5
//#define STF16360EN_DATP 3
//#define STF16360EN_ENP  2
////-------------------------STF16360E-CFG-ENN-----------------------------//
//-----------------POINTVFDDISPLAY����---------------------//
#define POINTVFDDISPLAY_CFG
#define POINTVFDDISPLAY_DISMEM_SIZE	63
#define POINTVFDDISPLAY_X_P	20
//#define POINTVFDDISPLAY_X_BYTE	3
#define POINTVFDDISPLAY_Y_P	7
#define POINTVFDDISPLAY_X_C	(POINTVFDDISPLAY_X_P/8)	//ˮƽ�������������ʾλ
#ifdef __AVR_ATmega16__
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	A
#define POINTVFDDISPLAY_KEY1_P		6
#define POINTVFDDISPLAY_KEY2_P		0
#else
#define POINTVFDDISPLAY_KEY1_PORT	B
#define POINTVFDDISPLAY_KEY2_PORT	C
#define POINTVFDDISPLAY_KEY1_P		4
#define POINTVFDDISPLAY_KEY2_P		0
#endif
typedef int8 IndexScreenLine;
//--------------POINTVFDDISPLAY���ý���--------------------//
//-------------------M62429P����---------------------//
#define M62429P_CFG
//Ҫ�������������λ��1
//ע��ESL_M62429P1VolumeL��EepromBuffer�б���ΪM62429P���ݵĿ�ʼ

#ifdef __AVR_ATmega16__
#define M62429P_PORT		C
#define M62429P_DATP		1
#define M62429P_SCK1PORT	A
#else
#define M62429P_PORT		C
#define M62429P_DATP		4
#define M62429P_SCK1PORT	C
#endif
#define M62429P_SCK1P		2
#define M62429P_SCK2PORT	D
#define M62429P_SCK2P		3
//������Ա���M62429P���õ�EEPROM
#define SAVE_M62429P_SETTING
//-----------------M62429P���ý���-------------------//

#endif//_CONFIGURE_H_
