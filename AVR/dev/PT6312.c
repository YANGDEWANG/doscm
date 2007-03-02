#include "global.h"
#include <util/delay.h>
#include "boolean.h"
#include "pt6312.h"
#include "SPI.h"

#define co1da PT6312_DISPLAY_MODE&0b11	//��ʾģʽ
#if	PT6312_LUM>7||PT6312_LUM<0
#error "PY6312����ֻ����0~7��ֵ��Խ��Խ��"
#endif
#define cmdd 0
#define datt 1
//bool DisplayChanged;

#define GLUE(a, b)     a##b

#define GPORT(x)       GLUE(PORT, x)
#define GDDR(x)        GLUE(DDR, x)
#define GPIN(x)        GLUE(PIN, x)

#define PIN_Display 	GPIN(PT6312_POPT)
#define DDR_Display 	GDDR(PT6312_POPT)
#define PORT_Display 	GPORT(PT6312_POPT)
#define DisplaySTB		PT6312_STB
static inline void setSTB()
{
	//DDR_Display		&=~(1<<DisplaySTB);
	PORT_Display	|= (1<<DisplaySTB);
#ifdef VSPI_SYNCHRO_U
	OutVSPI();
#endif //VSPI_SYNCHRO_U
}

static inline void clSTB()
{
#ifdef VSPI_SYNCHRO_U
	InVSPI();
#endif //VSPI_SYNCHRO_U
	//DDR_Display		|= (1<<DisplaySTB);
	PORT_Display	&=~(1<<DisplaySTB);
}
/*
union comd1_6312
{
uint8 da;
struct
{
uint8 dms: 3; //ѡ����ʾ��Ӳ����դ�Ͷε�λ��
uint8 dnc: 3; //���ݿ�λ
uint8 ad1: 2; //������ĵ�ֵַ�̶�Ϊ0X00
}bitS;
};
union comd2_6312
{
uint8 da;
struct
{
uint8 fru: 2; //����ѡ��00��WRITE DATA TO DISRAM 01��WRITE DATA TO LED 02��READ DATA FROM KEY 03��READ DATA FROM RM_SW
uint8 mad: 1; //0���� 1ָ����ַ
uint8 mst: 1; //0���� 1����ģʽ
uint8 dnc: 2; //���ݿ�λ
uint8 ad2: 2; //������ĵ�ֵַ�̶�Ϊ0X01
}bitS;
};

union comd3_6312
{
uint8 da;
struct
{
uint8 add: 5; //16312�ڲ�DISRAM��ַѡ��
uint8 dnc: 1; //���ݿ�λ
uint8 ad3: 2; //������ĵ�ֵַ�̶�Ϊ0X03
}bitS;
};*/

#ifdef PT6312_LUM_CONTROL
union comd4_6312
{
	uint8 da;
	struct
	{
		uint8 pwm: 3; //�������ʱ��ѡ�񣨿��Ե������ȣ�
		uint8 dis: 1; //0��ʾ�ر�
		uint8 dnc: 2; //���ݿ�λ
		uint8 ad4: 2; //������ĵ�ֵַ�̶�Ϊ0X02
	}bitS;
}com4;
#define co4da com4.da
/*
*������ʾ����
*lu������ֵ������[0,7]
*/
void SetDisplayLum(uint8 lu)
{
	if(0==lu)
	{
		com4.bitS.dis = 0;
	}
	else
	{
		com4.bitS.dis = 1;
	}
	com4.bitS.pwm = lu;
}
#else //PT6312_LUM_CONTROL
#define co4da 0x88|PT6312_LUM
#endif //PT6312_LUM_CONTROL
//дһ���ֽ�����
static void wrd(uint8);
//дһ���ֽڵ�����
static void wrc(uint8);

//����ʾ����datд���Դ�
void PT6312UpdateAll(uint8* dat)
{  	
	uint8 i;
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	wrc(0x40);//����ѡ��WRITE DATA TO DISRAM
	wrc(0b11000000);//��ַ00
#ifdef ErrorCodeShow
	if(ShowErrorCodeing)
		dis = DeviceMemImage;
#endif
	//todo
	i=0;
	for(;i<PT6312_MEMORY_SIZE;i++)
	{
		wrd(*dat++);
	}
	wrc(co1da);
	wrc(co4da);
	setSTB();
} 
//��ʼ��6312
void PT6312Init()
{
#ifdef PT6312_LUM_CONTROL
	com4.bitS.pwm=PT6312_LUM;//����
	com4.bitS.dis=0x01;//����ʾ
	com4.bitS.ad4=0x02;
#endif//PT6312_LUM_CONTROL
	DDR_Display	|= (1<<DisplaySTB);
	setSTB();
	//ErasureMemory(DeviceMemImage,DisplayMemorySize);
	//PT6312UpdateAll();
}
//дһ���ֽ�����
static void wrd(uint8 disno)  
{
	
	SPI_MasterTransmit(disno);
}
//дһ���ֽڵ�����
static void wrc(uint8 disno)  
{
	setSTB();
	_delay_us(1);
	clSTB();
	SPI_MasterTransmit(disno);
}

#ifdef 	PT6312_WRLED
//����16312���ĸ���������ݴ����LEDDATA�ĵ���λ
void PT6312WriteLed(uint8 leddata) 
{
	//cm2.fru=0x01;
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	wrc(0x41);
	wrd(leddata);
	setSTB();
}
#endif 	//PT6312_WRLED
#ifdef 	PT6312_SWDATA
//�����ݴ�RM_SW�ڣ���4bit��Ч
uint8 PT6312ReadSWData()
{
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	wrc(67);
	//_delay_loop_1(1);//cano
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	uint8 tmp=SPI_MasterTransmit(0xff);
	setSTB();
	return tmp;
}
#endif 	//PT6312_SWDATA

static uint8 ReadKey_key[PT6312_KEY_MEMORY_SIZE];
//������
static void ReadKey()			
{
	uint8 k;
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	wrc(0x42);
	//_delay_loop_1(1);//cano
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_MSB,SPI_MSTR,SPI_IDIS));
	for(k=0;k<PT6312_KEY_MEMORY_SIZE;k++)
	{
		ReadKey_key[k] = SPI_MasterTransmit(0xff);
	}
	setSTB();
}

static uint8 akey[PT6312_KEY_SCAN_AMOUNT];
//ɨ�谴�����ص�һ�����µļ�����ֵ return null ��ʾ�ް�������
uint8 * ScanKey()
{
#if PT6312_KEY_SCAN_AMOUNT == 1
	uint8 i;
	ReadKey();
	//	uint8 akey[1];
	for(i=0;i<PT6312_KEY_MEMORY_SIZE*8;i++)
	{
		if(TestBit(i,ReadKey_key))
		{
			*akey = i+1;
			return akey;
		}
	}
	return 0;
#else
	uint8 i,akeyCount=0;
	ReadKey();
	for(i=0;i<PT6312_KEY_MEMORY_SIZE*8;i++)
	{
		if(TestBit(i,ReadKey_key))
		{
			akey[akeyCount] = i+1;
			akeyCount++;
			if(akeyCount==PT6312_KEY_SCAN_AMOUNT)
				return akey;
		}
	}
	if(akeyCount==0)
	{
		return null;
	}
	for(;akeyCount<PT6312_KEY_SCAN_AMOUNT;akeyCount++)
	{
		akey[akeyCount] = 0;
	}
	return akey;
#endif
}

