#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <avr/io.h>

#include "TypeDefing.h"


#ifndef CLICK_CYCLE_US
#define CLICK_CYCLE_US 128//ϵͳClickʱ��(us)
#endif
#define CLOCKCS	0b101


#define CLOCK_M(c) (c*1000000ul)

//#define _CPU_CLOCK_ 11059200
#define _CPU_CLOCK_ CLOCK_M(8)
#define _CPU_INSTRUCTION_CLOCK_ (_CPU_CLOCK_)

//ÿ����΢����
#define _CPU_PER_INSTRUCTION_US_ (1000000/_CPU_INSTRUCTION_CLOCK_) 
//ÿ΢��������
#define _CPU_PER_US_INSTRUCTION_ (_CPU_INSTRUCTION_CLOCK_/1000000)

#define	MaxUserEventCountDown 15//�û��¼����������󵹼�ʱ����λʱ��s��ָ��ʱ�����򽫽��볣������
//#define DEBUG
//#define ASM			  interrupt				interrupt			  interrupt
//#define C//
//------------dwstd ����----------------//
#define DWSTD_CFG
//����Ϊ���ɵ��ַ�����ӽ�����
#define DWSTD_ADDEND
//����ʹ�ù̶����tostringʱ����λ������ַ�Ĭ����'0'
#define DWSTD_TOSTRING_ADDCHAR '0'
//����DWSTDʹ�õ�����
#define dwstdint uint16
#define dwstdsint int16
//------------dwstd ���ý���----------------//

//--------------------------SPI������-------------------------------//
#define SPI_CFG
#define SPI_MASTER_U	1	//ʹ��SPI��������ģʽ
//pin def
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7

//--------------------------SPI������_END----------------------------//

//------------------------------PT6312����-------------------------//
#define PT6312_CFG
#define PT6312_DISPLAY_MODE	0x02						//����Ϊ6�� 16��
#define PT6312_MEMORY_SIZE	(PT6312_DISPLAY_MODE+4)*2	//��ʹ�õĴ���ռ��С
#define PT6312_LUM_CONTROL								//����ʹ�����ȿ���
//#define	PT6312_WRLED									//����ʹ��LED�ڿ���
//#define	PT6312_SWDATA									//����ʹ�ö�ȡSW����
#define PT6312_LUM					6					//Ĭ������
#define PT6312_KEY_MEMORY_SIZE		3					//��������ռ��С
#define PT6312_KEY_SCAN_AMOUNT		1					//��ͬʱ����İ���������Ŀ
#define PT6312_SPI_FOSC				SPI_FOSC_16			//ѡ��SPI�ٶ�
//�������ݽӿ�����
//pin def
#ifndef PT6312_POPT
#define PT6312_POPT	B
#define	PT6312_STB	4
#endif//SPI_POPT
//------------------------------PT6312����-END----------------------//

#endif//_CONFIGURE_H_
