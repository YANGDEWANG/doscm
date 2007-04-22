#ifndef _SPI_H_
#define _SPI_H_
#include <configure.h>

#ifndef SPI_CFG
//--------------------------SPI口配置-------------------------------//
#define SPI_CFG
#define VSPI			1	//使用虚拟SPI口
/*如果要同步使用虚拟SPI口需定义它，这将导致一个传输过程结束前其他传输请求必须等待(实现进程同步)*/
#define VSPI_SYNCHRO_U	1	
#define SPI_MASTER_U	1	//使用SPI口主机机模式
#define SPI_SR_U		1	//使用SPI口从机模式

//pin def
#define SPI_POPT	B
#define	MISO_SPI	6
#define	MOSI_SPI	5
#define	SS_SPI		4
#define	SCK_SPI		7
//--------------------------SPI口配置_END----------------------------//
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
//SPI速度等级
#define SPI_FOSC_4		((0<<SPR01)|(0<<SPR00))//震荡频率除4
#define SPI_FOSC_16		((0<<SPR01)|(1<<SPR00))//震荡频率除16
#define SPI_FOSC_64		((1<<SPR01)|(0<<SPR00))//震荡频率除64
#define SPI_FOSC_128	((1<<SPR01)|(1<<SPR00))//震荡频率除128

//SPI模式
#define SPI_Mode_0		((0<<CPOL0)|(0<<CPHA0))//CPOL = 0, CPHA = 0 起始沿采样 ( 上升沿) 结束沿设置 ( 下降沿) 0
#define SPI_Mode_1		((0<<CPOL0)|(1<<CPHA0))//CPOL = 0, CPHA = 1 起始沿设置 ( 上升沿) 结束沿采样 ( 下降沿) 1
#define SPI_Mode_2		((1<<CPOL0)|(0<<CPHA0))//CPOL = 1, CPHA = 0 起始沿采样 ( 下降沿) 结束沿设置 ( 上升沿) 2
#define SPI_Mode_3		((1<<CPOL0)|(1<<CPHA0))//CPOL = 1, CPHA = 1 起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) 3

//位顺序
#define SPI_LSB			(1<<DORD0)	//LSB首发
#define SPI_MSB			(0)			//MSB首发

//方式
#define SPI_MSTR		(1<<MSTR0)	//主机方式
#define SPI_SR			(0)			//从机方式

//中断开关
#define SPI_IE			(1<<SPIE0)//开
#define SPI_IDIS		(0)//关
/************************************
获取SPI设置
fosc：SPI速度等级
mode：SPI模式
mstr：方式
lsb ：位顺序
ie  ：中断开关
*************************************/
#define GET_SPI_SET(fosc,mode,lsb,mstr,ie)	(fosc|mode|lsb|mstr|ie|(1<<SPE0))

#else
//SPI速度等级
#define SPI_FOSC_4		((0<<SPR1)|(0<<SPR0))//震荡频率除4
#define SPI_FOSC_16		((0<<SPR1)|(1<<SPR0))//震荡频率除16
#define SPI_FOSC_64		((1<<SPR1)|(0<<SPR0))//震荡频率除64
#define SPI_FOSC_128	((1<<SPR1)|(1<<SPR0))//震荡频率除128

//SPI模式
#define SPI_Mode_0		((0<<CPOL)|(0<<CPHA))//CPOL = 0, CPHA = 0 起始沿采样 ( 上升沿) 结束沿设置 ( 下降沿) 0
#define SPI_Mode_1		((0<<CPOL)|(1<<CPHA))//CPOL = 0, CPHA = 1 起始沿设置 ( 上升沿) 结束沿采样 ( 下降沿) 1
#define SPI_Mode_2		((1<<CPOL)|(0<<CPHA))//CPOL = 1, CPHA = 0 起始沿采样 ( 下降沿) 结束沿设置 ( 上升沿) 2
#define SPI_Mode_3		((1<<CPOL)|(1<<CPHA))//CPOL = 1, CPHA = 1 起始沿设置 ( 下降沿) 结束沿采样 ( 上升沿) 3

//位顺序
#define SPI_LSB			(1<<DORD)	//LSB首发
#define SPI_MSB			(0)			//MSB首发

//方式
#define SPI_MSTR		(1<<MSTR)	//主机方式
#define SPI_SR			(0)			//从机方式

//中断开关
#define SPI_IE			(1<<SPIE)//开
#define SPI_IDIS		(0)//关
/************************************
获取SPI设置
fosc：SPI速度等级
mode：SPI模式
mstr：方式
lsb ：位顺序
ie  ：中断开关
*************************************/
#define GET_SPI_SET(fosc,mode,lsb,mstr,ie)	(fosc|mode|lsb|mstr|ie|(1<<SPE))

#endif
/*************************************
初始化SPI口
cfg：SPI口的配置，使用GET_SPI_SET获取
**************************************/
void SPI_Init(uint8 cfg);

//定义SPI速度
//#define SPISpeed	SPI_FOSC_4
//#define SPIDoubleSpeed 1//定义启动SPI倍速功能
//定义SPI模式
//#ifndef SPI_Mode
//#define SPI_Mode SPI_Mode_0
//#endif//SPI_Mode





//pin
#define PIN_VSPI_SCK PIN_VSPI&(1<<SCK_VSPI)
#define PIN_VSPI_SDA PIN_VSPI&(1<<SDA_VSPI)




#ifdef SPI_SR_U
extern void SPI_SRInit(void);//LSB首发
#endif //SPI_SR_U
uint8 SPI_MasterTransmit(uint8 cData);
void SPIRelease();

#ifdef VSPI
#ifdef VSPI_SYNCHRO_U
extern volatile bool VSPI_Transmiting;//传输过程中为1，不要写他
extern void InVSPI(void);			//开始使用时调用
static void __inline__ OutVSPI(void)//结束使用时调用
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
#define VSPI_LSB_FIRST	VSPI_LSB_First = true //LSB首发
#define VSPI_MSB_FIRST	VSPI_LSB_First = false//MSB首发
extern bool VSPI_LSB_First;	//true = LSB首发
extern void VSPI_MasterInit(void);
extern void VSPI_MasterTransmit(uint8 cData);
extern uint8 VSPI_MasterRead();
#endif	//VSPI
#endif	//_SPI_H_
