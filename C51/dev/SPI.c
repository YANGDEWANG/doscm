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

#include <global.h>
#include "SPI.h"

#ifdef SPI_U
#ifdef SPI_SR_U
void SPI_SRInit(void)//LSB�׷�
{
	//SPDR = 0xff;
	// ʹ��SPI �ӻ�ģʽ
	SPCR |= (1<<SPIE)|(1<<SPE)|(1<<DORD)|(1<<CPOL)|(1<<CPHA);
	SPCR &=~(1<<MSTR);
}
#endif //SPI_SR_U
#ifdef SPI_MasterMultibyte_U
volatile uint8 SPI_OutboxDataCount;	//����SPI����ǰ��ȷ����Ϊ0�����򽫶�ʧΪ��ɴ��������
static uint8* SPI_OutboxData;		//��ǰ���������

void SPI_MasterMultibyteInit(uint8 SPISpeed)//LSB�׷�SPISpeedָ���ٶ�
{	
	/* ����MOSI ��SCK ,SSΪ���������Ϊ���� */
	DDR_SPI  |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);//
	//POPT_SPI |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);//
	/* ʹ��SPI ����ģʽ��*/
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<MSTR)|(1<<DORD)|SPI_Mode|SPISpeed;
#ifdef SPIDoubleSpeed
	SPSR |= (1<<SPI2X);
#endif//SPIDoubleSpeed
}
void SPI_MasterMultibyteTransmit(uint8* cData,uint8 dataCount)//��ͬ��ģʽ����һ���ֽ�return����ȡ���ֽ�
{
	/* �������ݴ��� */
	SPI_OutboxDataCount = dataCount;
	SPDR = *cData;
	SPI_OutboxData = cData;
}
ISR(SPI_STC_vect)
{
	os_in_Interrupt();
	if(SPI_OutboxDataCount)
	{
		*SPI_OutboxData = SPDR;
		if(--SPI_OutboxDataCount)
		{
			SPDR = *++SPI_OutboxData;
		}
	}
	os_out_Interrupt();
}
#endif//SPI_MasterMultibyte_U
#ifdef SPI_MASTER_U
void SPI_MasterInit(uint8 SPISpeed)//LSB�׷�SPISpeedָ���ٶ�
{	
	/* ����MOSI ��SCK ,SSΪ���������Ϊ���� */
	DDR_SPI  |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);//
	//POPT_SPI |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);//
	/* ʹ��SPI ����ģʽ��*/
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<DORD)|SPI_Mode|SPISpeed;
#ifdef SPIDoubleSpeed
	SPSR |= (1<<SPI2X);
#endif//SPIDoubleSpeed
}
uint8 SPI_MasterTransmit(uint8 cData)//��ͬ��ģʽ����һ���ֽ�return����ȡ���ֽ�
{
	/* �������ݴ��� */
	SPDR = cData;
	/* �ȴ�������� */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
#endif //SPI_MASTER_U
void SPIRelease()//�ͷ� SPI��
{
	SPCR = 0;
	DDR_SPI  &= ~((1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI));//

}
#endif //SPI_U

#ifdef VSPI
//CM
sbit scl2 =  P2^0;
sbit sda2 =  P2^1;
#define setSCL()	(scl2=SCK_VSPI = true)
#define clSCL()		(scl2=SCK_VSPI = false)
#define setSDA()	(sda2=SDA_VSPI = true)
#define clSDA()		(sda2=SDA_VSPI = false)


#ifdef VSPI_SYNCHRO_U
volatile bool VSPI_Transmiting = false;
void InVSPI(void)	//��ʼʹ��ʱ����
{
	while(VSPI_Transmiting);
	VSPI_Transmiting = true;
}
/*void OutVSPI(void)	//����ʹ��ʱ����
{
VSPI_Transmiting = false;
}*/
#endif//VSPI_SYNCHRO_U

void VSPI_MasterInit(void)
{
	setSDA();
#ifdef VSPI_START_RISE
	clSCL();
#else//VSPI_START_RISE
	setSCL();
#endif//VSPI_START_RISE
#ifdef VSPI_SYNCHRO_U
	VSPI_Transmiting = false;
#endif//VSPI_SYNCHRO_U
}
#ifdef VSPI_LSB_U
uint8 VSPI_MasterTransmitLSB(uint8 cData)
{
	uint8 i=8,ReadData;
	do
	{
#ifdef VSPI_START_SET
#ifdef VSPI_START_RISE
		setSCL();
#else//VSPI_START_RISE
		clSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET
		if(cData&1)
		{
			setSDA();
		}
		else
		{
			clSDA();
		}
		cData = cData>>1;
#ifdef VSPI_START_SET
#ifdef VSPI_START_RISE
		clSCL();
#else//VSPI_START_RISE
		setSCL();
#endif//VSPI_START_RISE
#else//VSPI_START_SET
#ifdef VSPI_START_RISE
		setSCL();
#else//VSPI_START_RISE
		clSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET
		ReadData = ReadData>>1;
		if(SDA_VSPI)
			ReadData|=0x80;
#ifndef VSPI_START_SET
#ifdef VSPI_START_RISE
		clSCL();
#else//VSPI_START_RISE
		setSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET
	}while(--i);
	return ReadData;
}
#endif//VSPI_LSB_U
#ifdef VSPI_MSB_U
uint8 VSPI_MasterTransmitMSB(uint8 cData)
{
	uint8 i=8,ReadData;
	do
	{
#ifdef VSPI_START_SET
#ifdef VSPI_START_RISE
		setSCL();
#else//VSPI_START_RISE
		clSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET
		if(cData&0x80)
		{
			setSDA();
		}
		else
		{
			clSDA();
		}
		cData = cData<<1;
#ifdef VSPI_START_SET
#ifdef VSPI_START_RISE
		clSCL();
#else//VSPI_START_RISE
		setSCL();
#endif//VSPI_START_RISE
#else//VSPI_START_SET
#ifdef VSPI_START_RISE
		setSCL();
#else//VSPI_START_RISE
		clSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET

		ReadData = ReadData<<1;
		if(SDA_VSPI)
			ReadData|=1;
#ifndef VSPI_START_SET
#ifdef VSPI_START_RISE
		clSCL();
#else//VSPI_START_RISE
		setSCL();
#endif//VSPI_START_RISE
#endif//VSPI_START_SET
	}while(--i);
	//setSDA();//bug
	return ReadData;
}
#endif//VSPI_MSB_U
#endif//VSPI
