#ifndef _SPI_H_
#define _SPI_H_
#include <configure.h>

#ifndef SPI_CFG
//--------------------------SPI������-------------------------------//
#define SPI_CFG
#define VSPI			1	//ʹ������SPI��
/*���Ҫͬ��ʹ������SPI���趨�������⽫����һ��������̽���ǰ���������������ȴ�(ʵ�ֽ���ͬ��)*/
#define VSPI_SYNCHRO_U	1	
#define SPI_MASTER_U	1	//ʹ��SPI��������ģʽ
#define SPI_SR_U		1	//ʹ��SPI�ڴӻ�ģʽ

//pin def
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
//--------------------------SPI������_END----------------------------//
#endif//SPI_CFG

//port
#define DDR_SPI		GDDR(SPI_POPT)
#define POPT_SPI	GPORT(SPI_POPT)
#define PIN_SPI 	GPIN(SPI_POPT)
//pin
#define PIN_SPI_SS 		PIN_SPI&(1<<SS_SPI)
#define PIN_SPI_SCK 	PIN_SPI&(1<<SCK_SPI)
#define PIN_SPI_MISO 	PIN_SPI&(1<<MISO_SPI)
#define PIN_SPI_MOSI 	PIN_SPI&(1<<MOSI_SPI)

#ifdef __AVR_ATmega324P__
//SPI�ٶȵȼ�
#define SPI_FOSC_4		((0<<SPR01)|(0<<SPR00))//��Ƶ�ʳ�4
#define SPI_FOSC_16		((0<<SPR01)|(1<<SPR00))//��Ƶ�ʳ�16
#define SPI_FOSC_64		((1<<SPR01)|(0<<SPR00))//��Ƶ�ʳ�64
#define SPI_FOSC_128	((1<<SPR01)|(1<<SPR00))//��Ƶ�ʳ�128

//SPIģʽ
#define SPI_Mode_0		((0<<CPOL0)|(0<<CPHA0))//CPOL = 0, CPHA = 0 ��ʼ�ز��� ( ������) ���������� ( �½���) 0
#define SPI_Mode_1		((0<<CPOL0)|(1<<CPHA0))//CPOL = 0, CPHA = 1 ��ʼ������ ( ������) �����ز��� ( �½���) 1
#define SPI_Mode_2		((1<<CPOL0)|(0<<CPHA0))//CPOL = 1, CPHA = 0 ��ʼ�ز��� ( �½���) ���������� ( ������) 2
#define SPI_Mode_3		((1<<CPOL0)|(1<<CPHA0))//CPOL = 1, CPHA = 1 ��ʼ������ ( �½���) �����ز��� ( ������) 3

//λ˳��
#define SPI_LSB			(1<<DORD0)	//LSB�׷�
#define SPI_MSB			(0)			//MSB�׷�

//��ʽ
#define SPI_MSTR		(1<<MSTR0)	//������ʽ
#define SPI_SR			(0)			//�ӻ���ʽ

//�жϿ���
#define SPI_IE			(1<<SPIE0)//��
#define SPI_IDIS		(0)//��
/************************************
��ȡSPI����
fosc��SPI�ٶȵȼ�
mode��SPIģʽ
mstr����ʽ
lsb ��λ˳��
ie  ���жϿ���
*************************************/
#define GET_SPI_SET(fosc,mode,lsb,mstr,ie)	(fosc|mode|lsb|mstr|ie|(1<<SPE0))

#else
//SPI�ٶȵȼ�
#define SPI_FOSC_4		((0<<SPR1)|(0<<SPR0))//��Ƶ�ʳ�4
#define SPI_FOSC_16		((0<<SPR1)|(1<<SPR0))//��Ƶ�ʳ�16
#define SPI_FOSC_64		((1<<SPR1)|(0<<SPR0))//��Ƶ�ʳ�64
#define SPI_FOSC_128	((1<<SPR1)|(1<<SPR0))//��Ƶ�ʳ�128

//SPIģʽ
#define SPI_Mode_0		((0<<CPOL)|(0<<CPHA))//CPOL = 0, CPHA = 0 ��ʼ�ز��� ( ������) ���������� ( �½���) 0
#define SPI_Mode_1		((0<<CPOL)|(1<<CPHA))//CPOL = 0, CPHA = 1 ��ʼ������ ( ������) �����ز��� ( �½���) 1
#define SPI_Mode_2		((1<<CPOL)|(0<<CPHA))//CPOL = 1, CPHA = 0 ��ʼ�ز��� ( �½���) ���������� ( ������) 2
#define SPI_Mode_3		((1<<CPOL)|(1<<CPHA))//CPOL = 1, CPHA = 1 ��ʼ������ ( �½���) �����ز��� ( ������) 3

//λ˳��
#define SPI_LSB			(1<<DORD)	//LSB�׷�
#define SPI_MSB			(0)			//MSB�׷�

//��ʽ
#define SPI_MSTR		(1<<MSTR)	//������ʽ
#define SPI_SR			(0)			//�ӻ���ʽ

//�жϿ���
#define SPI_IE			(1<<SPIE)//��
#define SPI_IDIS		(0)//��
/************************************
��ȡSPI����
fosc��SPI�ٶȵȼ�
mode��SPIģʽ
mstr����ʽ
lsb ��λ˳��
ie  ���жϿ���
*************************************/
#define GET_SPI_SET(fosc,mode,lsb,mstr,ie)	(fosc|mode|lsb|mstr|ie|(1<<SPE))

#endif
/*************************************
��ʼ��SPI��
cfg��SPI�ڵ����ã�ʹ��GET_SPI_SET��ȡ
**************************************/
void SPI_Init(uint8 cfg);

//����SPI�ٶ�
//#define SPISpeed	SPI_FOSC_4
//#define SPIDoubleSpeed 1//��������SPI���ٹ���
//����SPIģʽ
//#ifndef SPI_Mode
//#define SPI_Mode SPI_Mode_0
//#endif//SPI_Mode





//pin
#define PIN_VSPI_SCK PIN_VSPI&(1<<SCK_VSPI)
#define PIN_VSPI_SDA PIN_VSPI&(1<<SDA_VSPI)




#ifdef SPI_SR_U
extern void SPI_SRInit(void);//LSB�׷�
#endif //SPI_SR_U
uint8 SPI_MasterTransmit(uint8 cData);
void SPIRelease();

#ifdef VSPI
#ifdef VSPI_SYNCHRO_U
extern volatile bool VSPI_Transmiting;//���������Ϊ1����Ҫд��
extern void InVSPI(void);			//��ʼʹ��ʱ����
static void __inline__ OutVSPI(void)//����ʹ��ʱ����
{
	VSPI_Transmiting = false;
}
#endif//VSPI_SYNCHRO_U
//pin count
#define SCK_VSPI 3
#define SDA_VSPI 4
//port
#define PIN_VSPI 	PIND
#define DDR_VSPI 	DDRD
#define PORT_VSPI 	PORTD
#define VSPI_LSB_FIRST	VSPI_LSB_First = true //LSB�׷�
#define VSPI_MSB_FIRST	VSPI_LSB_First = false//MSB�׷�
extern bool VSPI_LSB_First;	//true = LSB�׷�
extern void VSPI_MasterInit(void);
extern void VSPI_MasterTransmit(uint8 cData);
extern uint8 VSPI_MasterRead();
#endif	//VSPI
#endif	//_SPI_H_
