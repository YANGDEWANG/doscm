
#include "global.h"
#include <avr/interrupt.h>
#include "SPI.h"




#ifdef SPI_SR_U
void SPI_SRInit(void)//LSB�׷�
{
	//SPDR = 0xff;
	// ʹ��SPI �ӻ�ģʽ
	SPCR |= (1<<SPIE)|(1<<SPE)|(1<<DORD)|(1<<CPOL)|(1<<CPHA);
	SPCR &=~(1<<MSTR);
}
#endif //SPI_SR_U
#ifdef __AVR_ATmega324P__
/************************************
��ʼ��SPI��
cfg��SPI�ڵ����ã�ʹ��GET_SPI_SET��ȡ
************************************/
void SPI_Init(uint8 cfg)
{	
	/* ����MOSI ��SCKΪ���,SS����Ϊ���ӻ�Ϊ�룬����Ϊ���� */
	DDR_SPI  &= ~((1<<MOSI_SPI)|(1<<MISO_SPI)|(1<<SCK_SPI)|(1<<SS_SPI));
	if(cfg&SPI_MSTR)
	{
		DDR_SPI  |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);
	}
	else
	{
		DDR_SPI  |= (1<<MOSI_SPI);
	}
	SPCR0 = cfg;
#ifdef SPIDoubleSpeed
	SPSR0 |= (1<<SPI2X);
#endif//SPIDoubleSpeed
}
void SPIRelease()
{
	SPCR0 = 0;
	DDR_SPI  &= ~((1<<MOSI_SPI)|(1<<MISO_SPI)|(1<<SCK_SPI)|(1<<SS_SPI));
}
#else
/************************************
��ʼ��SPI��
cfg��SPI�ڵ����ã�ʹ��GET_SPI_SET��ȡ
************************************/
void SPI_Init(uint8 cfg)
{	
	/* ����MOSI ��SCKΪ���,SS����Ϊ���ӻ�Ϊ�룬����Ϊ���� */
	DDR_SPI  &= ~((1<<MOSI_SPI)|(1<<MISO_SPI)|(1<<SCK_SPI)|(1<<SS_SPI));
	if(cfg&SPI_MSTR)
	{
		DDR_SPI  |= (1<<MOSI_SPI)|(1<<SCK_SPI)|(1<<SS_SPI);
	}
	else
	{
		DDR_SPI  |= (1<<MOSI_SPI);
	}
	SPCR = cfg;
#ifdef SPIDoubleSpeed
	SPSR |= (1<<SPI2X);
#endif//SPIDoubleSpeed
}
void SPIRelease()
{
	SPCR = 0;
	DDR_SPI  &= ~((1<<MOSI_SPI)|(1<<MISO_SPI)|(1<<SCK_SPI)|(1<<SS_SPI));
}
#endif
#ifdef SPI_MASTER_U
uint8 SPI_MasterTransmit(uint8 cData)
{
	/* �������ݴ��� */
	SPDR = cData;
	/* �ȴ�������� */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
#endif //SPI_MASTER_U

#ifdef VSPI



static void __inline__ setSCL()
{
	DDR_VSPI  &=~(1<<SCK_VSPI);
	//PORT_VSPI	|= (1<<SCK_VSPI);
}
static void __inline__ clSCL()
{
	//PORT_VSPI	&=~(1<<SCK_VSPI);
	DDR_VSPI  |= (1<<SCK_VSPI);
}
static void __inline__ setSDA()
{
	DDR_VSPI  &=~(1<<SDA_VSPI);
	//PORT_VSPI	|= (1<<SDA_VSPI);
}
static void __inline__ clSDA()
{
	//PORT_VSPI	&=~(1<<SDA_VSPI);
	DDR_VSPI  |= (1<<SDA_VSPI);
}

bool VSPI_LSB_First;	//true = LSB�׷�
static uint8 ReadData;

#ifdef VSPI_SYNCHRO_U
volatile bool VSPI_Transmiting = false;
void InVSPI(void)	//��ʼʹ��ʱ����
{
	while(VSPI_Transmiting){}
	VSPI_Transmiting = true;
}
/*void OutVSPI(void)	//����ʹ��ʱ����
{
VSPI_Transmiting = false;
}*/
#endif//VSPI_SYNCHRO_U
void VSPI_MasterInit(void)//LSB�׷�
{
	setSDA();
	setSCL();
	PORT_VSPI	&=~(1<<SCK_VSPI);
	PORT_VSPI	&=~(1<<SDA_VSPI);
	VSPI_LSB_First = true;	
#ifdef VSPI_SYNCHRO_U
	VSPI_Transmiting = false;
#endif//VSPI_SYNCHRO_U
}
void VSPI_MasterTransmit(uint8 cData)
{
	uint8 i;
	if(VSPI_LSB_First)
	{
		for(i=0;i<8;i++)
		{
			clSCL();
			if(cData&1)
			{
				setSDA();
			}
			else
			{
				clSDA();
			}
			cData = cData>>1;
			setSCL();
			ReadData = ReadData>>1;
			if(PIN_VSPI_SDA)
				ReadData|=0x80;
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			clSCL();
			if(cData&0x80)
			{
				setSDA();
			}
			else
			{
				clSDA();
			}
			cData = cData<<1;
			setSCL();
			ReadData = ReadData<<1;
			if(PIN_VSPI_SDA)
				ReadData|=1;
		}
	}
	setSDA();
}
uint8 VSPI_MasterRead()
{
	VSPI_MasterTransmit(0xff);
	return ReadData;
}
#endif
