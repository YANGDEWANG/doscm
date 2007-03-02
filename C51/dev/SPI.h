#ifndef _SPI_H_
#define _SPI_H_
/*
 * SPI Э��
 * ���������ͣ�C51��AVR
 * �汾��1
 * ���ڣ�2006-12-3
 * ���ߣ������<yangdewang@gmail.com>

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
#include "configure.h"

/*===========SPI===========*/
//#define SPI_U			1	//ʹ��SPI��
//#define SPI_MASTER_U	1	//ʹ��SPI������ģʽ
//#define SPI_MasterMultibyte_U	1	//ʹ��SPI���������ֽڴ���ģʽ
//#define SPI_SR_U		1	//ʹ��SPI�ڴӻ�ģʽ

#ifdef SPI_U
//SPI�ٶȵȼ�
#define SPI_FOSC_4		(0<<SPR1)|(0<<SPR0)//��Ƶ�ʳ�4
#define SPI_FOSC_16		(0<<SPR1)|(1<<SPR0)//��Ƶ�ʳ�16
#define SPI_FOSC_64		(1<<SPR1)|(0<<SPR0)//��Ƶ�ʳ�64
#define SPI_FOSC_128	(1<<SPR1)|(1<<SPR0)//��Ƶ�ʳ�128

//SPIģʽ
#define SPI_Mode_0 (0<<CPOL)|(0<<CPHA)//CPOL = 0, CPHA = 0 ��ʼ�ز��� ( ������) ���������� ( �½���) 0
#define SPI_Mode_1 (0<<CPOL)|(1<<CPHA)//CPOL = 0, CPHA = 1 ��ʼ������ ( ������) �����ز��� ( �½���) 1
#define SPI_Mode_2 (1<<CPOL)|(0<<CPHA)//CPOL = 1, CPHA = 0 ��ʼ�ز��� ( �½���) ���������� ( ������) 2
#define SPI_Mode_3 (1<<CPOL)|(1<<CPHA)//CPOL = 1, CPHA = 1 ��ʼ������ ( �½���) �����ز��� ( ������) 3


//����SPI�ٶ�
//#define SPISpeed	SPI_FOSC_4
//#define SPIDoubleSpeed 1//��������SPI���ٹ���
//����SPIģʽ
#define SPI_Mode SPI_Mode_0

//pin count
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
//port
#define DDR_SPI		DDRB
#define POPT_SPI	PORTB
#define PIN_SPI 	PINB

//pin
#define PIN_SPI_SS 		PIN_SPI&(1<<SS_SPI)
#define PIN_SPI_SCK 	PIN_SPI&(1<<SCK_SPI)
#define PIN_SPI_MISO 	PIN_SPI&(1<<MISO_SPI)
#define PIN_SPI_MOSI 	PIN_SPI&(1<<MOSI_SPI)

#define SPI_LSB_FIRST	SPCR |= (1<<DORD)//LSB�׷�
#define SPI_MSB_FIRST	SPCR &=~(1<<DORD)//MSB�׷�


#ifdef SPI_SR_U
extern void SPI_SRInit(void);//LSB�׷�
#endif //SPI_SR_U

#ifdef SPI_MasterMultibyte_U///////////////////////////////////////////

extern volatile uint8 SPI_OutboxDataCount;	//����SPI����ǰ��ȷ����Ϊ0�����򽫶�ʧΪ��ɴ��������
extern void SPI_MasterMultibyteInit(uint8 SPISpeed);//LSB�׷�SPISpeedָ���ٶ�
extern void SPI_MasterMultibyteTransmit(uint8* cData,uint8 dataCount);//��ͬ��ģʽ����һ���ֽ�return����ȡ���ֽ�

#endif//SPI_MasterMultibyte_U///////////////////////////////////////////
#ifdef SPI_MASTER_U
extern void SPI_MasterInit(uint8 SPISpeed);//LSB�׷�SPISpeedָ���ٶ�
extern uint8 SPI_MasterTransmit(uint8 cData);//��ͬ��ģʽ����һ���ֽ�return����ȡ���ֽ�
#endif //SPI_MASTER_U
extern void SPIRelease();//�ͷ� SPI��
#endif //SPI_U
/*===========VSPI===========*/
#define VSPI			1	//ʹ������SPI��
#ifdef VSPI
#define VSPI_MSB_U		1	//ʹ��MSB
//#define VSPI_LSB_U		1	//ʹ��LSB
//#define VSPI_SCK_NORMAL_L	1	//����ʹSCK�ź��߳�̬Ϊ��
/*���Ҫͬ��ʹ������SPI���趨�������⽫����һ��������̽���ǰ���������������ȴ�(ʵ�ֽ���ͬ��)*/
//#define VSPI_SYNCHRO_U	1	

//SPIģʽ,��ѡһ
#define VSPI_MODE_0 //��ʼ�ز��� ( ������) ���������� ( �½���) SCK��̬0
//#define VSPI_MODE_1 //��ʼ������ ( ������) �����ز��� ( �½���) SCK��̬0
//#define VSPI_MODE_2 //��ʼ�ز��� ( �½���) ���������� ( ������) SCK��̬1
//#define VSPI_MODE_3 //��ʼ������ ( �½���) �����ز��� ( ������) SCK��̬1

#if defined(VSPI_MODE_0)||defined(VSPI_MODE_1)
#define  VSPI_START_RISE//��ʼ��������
#endif
#if defined(VSPI_MODE_1)||defined(VSPI_MODE_3)
#define  VSPI_START_SET//��ʼ������
#endif

#ifndef SDA_VSPI_PIN
#define SDA_VSPI_PIN P1^0
#endif
#ifndef SCK_VSPI_PIN
#define SCK_VSPI_PIN P1^1
#endif

//�������ݽӿ�����
sbit SDA_VSPI = SDA_VSPI_PIN;
sbit SCK_VSPI = SCK_VSPI_PIN;

#ifdef VSPI_SYNCHRO_U
extern volatile bool VSPI_Transmiting;//���������Ϊ1����Ҫд��
extern void InVSPI(void);			//��ʼʹ��ʱ����
static void __inline__ OutVSPI(void)//����ʹ��ʱ����
{
	VSPI_Transmiting = false;
}
#endif//VSPI_SYNCHRO_U
extern void VSPI_MasterInit(void);

#ifdef VSPI_LSB_U
extern uint8 VSPI_MasterTransmitLSB(uint8 cData);//����
#endif//VSPI_LSB_U
#ifdef VSPI_MSB_U
extern uint8 VSPI_MasterTransmitMSB(uint8 cData);
#endif//VSPI_MSB_U
#endif	//VSPI
#endif	//_SPI_H_
