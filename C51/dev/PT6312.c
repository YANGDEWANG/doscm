#include <util/delay.h>
#include "boolean.h"
#include "display.h"
#include "kernel.h"
#include <avr/io.h>
#include "SPI.h"
#define co1da DisplayMode&0b11	//��ʾģʽ
#if	lum>7||lum<0
#error "PY6312����ֻ����0~7��ֵ��Խ��Խ��"
#endif
#define cmdd 0
#define datt 1
//bool DisplayChanged;

static void setSTB()
{
	//DDR_Display		&=~(1<<DisplaySTB);
	PORT_Display	|= (1<<DisplaySTB);
#ifdef VSPI_SYNCHRO_U
	OutVSPI();
#endif //VSPI_SYNCHRO_U
}

static void clSTB()
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
uint8_t da;
struct
{
uint8_t dms: 3; //ѡ����ʾ��Ӳ����դ�Ͷε�λ��
uint8_t dnc: 3; //���ݿ�λ
uint8_t ad1: 2; //������ĵ�ֵַ�̶�Ϊ0X00
}bitS;
};
union comd2_6312
{
uint8_t da;
struct
{
uint8_t fru: 2; //����ѡ��00��WRITE DATA TO DISRAM 01��WRITE DATA TO LED 02��READ DATA FROM KEY 03��READ DATA FROM RM_SW
uint8_t mad: 1; //0���� 1ָ����ַ
uint8_t mst: 1; //0���� 1����ģʽ
uint8_t dnc: 2; //���ݿ�λ
uint8_t ad2: 2; //������ĵ�ֵַ�̶�Ϊ0X01
}bitS;
};

union comd3_6312
{
uint8_t da;
struct
{
uint8_t add: 5; //16312�ڲ�DISRAM��ַѡ��
uint8_t dnc: 1; //���ݿ�λ
uint8_t ad3: 2; //������ĵ�ֵַ�̶�Ϊ0X03
}bitS;
};*/

#ifdef LumControl
union comd4_6312
{
	uint8_t da;
	struct
	{
		uint8_t pwm: 3; //�������ʱ��ѡ�񣨿��Ե������ȣ�
		uint8_t dis: 1; //0��ʾ�ر�
		uint8_t dnc: 2; //���ݿ�λ
		uint8_t ad4: 2; //������ĵ�ֵַ�̶�Ϊ0X02
	}bitS;
}com4;
#define co4da com4.da
/*
*������ʾ����
*lu������ֵ������[0,7]
*/
void SetDisplayLum(uint8_t lu)
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
#else //LumControl
#define co4da 0x88|lum
#endif //LumControl
//дһ���ֽ�����
void wrd(uint8_t);
//дһ���ֽڵ�����
void wrc(uint8_t);

//����ʾ����disд���Դ�
void RefurbishDisplayMemory()
{  	
	uint8_t i;
	wrc(0x40);//����ѡ��WRITE DATA TO DISRAM
	wrc(0b11000000);//��ַ00
#ifdef ErrorCodeShow
	if(ShowErrorCodeing)
		dis = DeviceMemImage;
#endif
	//todo
	i=0;
	for(;i<sizeof(DeviceMemImage);i++)
	{
		wrd(DeviceMemImage[i]);
	}
	wrc(co1da);
	wrc(co4da);
	setSTB();
} 
//��ʼ��6312
void IniDisplayDevice()
{
#ifdef LumControl
	com4.bitS.pwm=lum;//����
	com4.bitS.dis=0x01;//����ʾ
	com4.bitS.ad4=0x02;
#endif//LumControl
	DDR_Display	|= (1<<DisplaySTB);
	setSTB();
	ErasureMemory(DeviceMemImage,DisplayMemorySize);
	RefurbishDisplayMemory();
}
//дһ���ֽ�����
void wrd(uint8_t disno)  
{
	VSPI_LSB_FIRST();
	VSPI_MasterTransmit(disno);
}
//дһ���ֽڵ�����
void wrc(uint8_t disno)  
{
	VSPI_LSB_FIRST();
	setSTB();
	_delay_loop_1(1);//cano
	clSTB();
	VSPI_MasterTransmit(disno);
}
#ifdef AutoRefurbish
//��VFDָ����ַд����
void wrvfd(uint8_t addr,uint8_t da) 
{
	wrd(0x44,cmdd);
	//cm3.add=addr;
	wrc(0b11000000|(addr&0b11111));
	wrd(da);
	setSTB();
}
#endif//AutoRefurbish

#ifdef 	wrled_U
//����16312���ĸ���������ݴ����LEDDATA�ĵ���λ
void wrled(uint8_t leddata) 
{
	//cm2.fru=0x01;
	wrc(0x41);
	wrd(leddata);
	setSTB();
}
#endif 	//wrled_U
#ifdef 	SWData_U
//�����ݴ�RM_SW�ڣ���4bit��Ч
uint8_t ReadSWData()
{
	wrc(67);
	//_delay_loop_1(1);//cano
	VSPI_LSB_FIRST();
	uint8_t tmp=VSPI_MasterRead();
	setSTB();
	return tmp;
}
#endif 	//SWData_U

uint8_t ReadKey_key[KeyMemorySize];
//������
void ReadKey()			
{
	uint8_t k;
	wrc(0x42);
	//_delay_loop_1(1);//cano
	VSPI_MSB_FIRST();
	for(k=0;k<KeyMemorySize;k++)
	{
		ReadKey_key[k] = VSPI_MasterRead();
	}
	setSTB();
}




uint8_t akey[KeyScanAmount];
//ɨ�谴�����ص�һ�����µļ�����ֵ return null ��ʾ�ް�������
uint8_t * ScanKey()
{
#if KeyScanAmount == 1
	uint8_t i;
	ReadKey();
	//	uint8_t akey[1];
	for(i=0;i<KeyMemorySize*8;i++)
	{
		if(TestBit(i,ReadKey_key))
		{
			*akey = i+1;
			return akey;
		}
	}
	return 0;
#else
	uint8_t i,akeyCount=0;
	ReadKey();
	for(i=0;i<KeyMemorySize*8;i++)
	{
		if(TestBit(i,ReadKey_key))
		{
			akey[akeyCount] = i+1;
			akeyCount++;
			if(akeyCount==KeyScanAmount)
				return akey;
		}
	}
	if(akeyCount==0)
	{
		return null;
	}
	for(;akeyCount<KeyScanAmount;akeyCount++)
	{
		akey[akeyCount] = 0;
	}
	return akey;
#endif
}

