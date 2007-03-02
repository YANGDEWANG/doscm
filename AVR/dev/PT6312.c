#include "global.h"
#include <util/delay.h>
#include "boolean.h"
#include "pt6312.h"
#include "SPI.h"

#define co1da PT6312_DISPLAY_MODE&0b11	//显示模式
#if	PT6312_LUM>7||PT6312_LUM<0
#error "PY6312亮度只允许0~7的值，越大越亮"
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
uint8 dms: 3; //选择显示屏硬件的栅和段的位数
uint8 dnc: 3; //数据空位
uint8 ad1: 2; //此命令的地址值固定为0X00
}bitS;
};
union comd2_6312
{
uint8 da;
struct
{
uint8 fru: 2; //功能选择：00：WRITE DATA TO DISRAM 01：WRITE DATA TO LED 02：READ DATA FROM KEY 03：READ DATA FROM RM_SW
uint8 mad: 1; //0递增 1指定地址
uint8 mst: 1; //0正常 1测试模式
uint8 dnc: 2; //数据空位
uint8 ad2: 2; //此命令的地址值固定为0X01
}bitS;
};

union comd3_6312
{
uint8 da;
struct
{
uint8 add: 5; //16312内部DISRAM地址选择
uint8 dnc: 1; //数据空位
uint8 ad3: 2; //此命令的地址值固定为0X03
}bitS;
};*/

#ifdef PT6312_LUM_CONTROL
union comd4_6312
{
	uint8 da;
	struct
	{
		uint8 pwm: 3; //输出点亮时间选择（可以调节亮度）
		uint8 dis: 1; //0显示关闭
		uint8 dnc: 2; //数据空位
		uint8 ad4: 2; //此命令的地址值固定为0X02
	}bitS;
}com4;
#define co4da com4.da
/*
*设置显示亮度
*lu：亮度值，允许[0,7]
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
//写一个字节数据
static void wrd(uint8);
//写一个字节的命令
static void wrc(uint8);

//将显示数据dat写入显存
void PT6312UpdateAll(uint8* dat)
{  	
	uint8 i;
	SPI_Init(GET_SPI_SET(PT6312_SPI_FOSC,SPI_Mode_3,SPI_LSB,SPI_MSTR,SPI_IDIS));
	wrc(0x40);//功能选择：WRITE DATA TO DISRAM
	wrc(0b11000000);//地址00
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
//初始化6312
void PT6312Init()
{
#ifdef PT6312_LUM_CONTROL
	com4.bitS.pwm=PT6312_LUM;//亮度
	com4.bitS.dis=0x01;//打开显示
	com4.bitS.ad4=0x02;
#endif//PT6312_LUM_CONTROL
	DDR_Display	|= (1<<DisplaySTB);
	setSTB();
	//ErasureMemory(DeviceMemImage,DisplayMemorySize);
	//PT6312UpdateAll();
}
//写一个字节数据
static void wrd(uint8 disno)  
{
	
	SPI_MasterTransmit(disno);
}
//写一个字节的命令
static void wrc(uint8 disno)  
{
	setSTB();
	_delay_us(1);
	clSTB();
	SPI_MasterTransmit(disno);
}

#ifdef 	PT6312_WRLED
//控制16312的四个输出口数据存放在LEDDATA的低四位
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
//读数据从RM_SW口，第4bit有效
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
//读键盘
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
//扫描按键返回第一个按下的键的码值 return null 表示无按键按下
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

