#include <global.h>
#include <string.h>
#include "eeprom_ex.h"
#include "boolean.h"
#include "at24c02.h"

uint8 perchdata EepromBuffer[ESL_end];//数据缓存
uint8 perchdata EepromDataCh[ESL_end/8+((ESL_end%8)?1:0)];
bool  EepromDataChanged;
uint8 EepromBufferBasicAdd;
#define MAX_ADD 256
#ifdef EEPROM_CHECK_DATA
static uint8 getCheck()//效验值为所有数据和的按位取反
{
	uint8 i=0;
	uint8 d=0;
	for(;i<ESL_addupcheck;i++)
	{
		d+=EepromBuffer[i];
	}
	//if(d==0)d++;//避免Check值为0这样如果Check值为0就可以判断为数据实效
	return ~d;
}
#endif//EEPROM_CHECK_DATA
void EepromSaveChar(uint8 loc,uint8 dat)	//保存一个字节，loc=位置，dat=数据
{
	EepromBuffer[loc] = dat;
	SetBit(EepromDataCh,loc,true);
	EepromDataChanged = true;
}
void EepromWriteEX()							//写缓存数据到到设备
{
	uint8 i=0;
	if(EepromDataChanged)
	{	
#ifdef EEPROM_CHECK_DATA
		EepromSaveChar(ESL_addupcheck,getCheck());
#endif//EEPROM_CHECK_DATA
		for(;i<ESL_end;i++)
		{
			if(TestBit(i,EepromDataCh))
			{
#ifdef __C51__
				WriteByte24c02(i+EepromBufferBasicAdd,EepromBuffer[i]);
#endif//__C51__
#ifdef __AVR__
				eeprom_busy_wait();
				_EEPUT(i,EepromBuffer[i]);
#endif//__AVR__
			}
		}
		//WriteByte24c02(ESL_addupcheck,EepromBuffer[ESL_addupcheck]);
		EepromDataChanged = false;
	}
}
void EepromSetDefEX()
{
	//memcpy(EepromBuffer,EepromDV,sizeof(EepromDV));//使用了260byte flash
	uint8 i;
	for(i=0;i<sizeof(EepromDV);i++)
	{
		EepromBuffer[i] = EepromDV[i];
	}
	memset(EepromDataCh,0xff,sizeof(EepromDataCh));
	EepromDataChanged = true;
	EepromWriteEX();	
}
static void read()
{
	Read24c02(EepromBufferBasicAdd,EepromBuffer,sizeof(EepromBuffer));
}
void IniEeprom_ex()
{	
#ifdef __C51__	
#ifdef EEPROM_CHECK_DATA
	uint16 add = EepromBufferBasicAdd;
	while(add+sizeof(EepromBuffer)<MAX_ADD)
	{
		EepromBufferBasicAdd = add;
		read();
		if(EepromBuffer[ESL_addupcheck]==getCheck())
		{
			return;
		}
		else
		{
			EepromSetDefEX();//数据效验失败加载缺省值
		}
		read();
		if(EepromBuffer[ESL_addupcheck]==getCheck())
		{
			return;
		}
		else
		{
			add+=sizeof(EepromBuffer);//数据效验失败提高区块
		}
	}
	EepromSetDefEX();//所有尝试失败加载缺省值
#else//EEPROM_CHECK_DATA
read();
#endif//EEPROM_CHECK_DATA
	/*
	bool reCheck = 0;
	READ:
	if(Read24c02(EepromBufferBasicAdd,EepromBuffer,sizeof(EepromBuffer))!=sizeof(EepromBuffer))//读入设置失败加载缺省值
	{
	EepromSetDefEX();
	return;
	}
	if(EepromBuffer[ESL_addupcheck]!=getCheck())//数据效验失败加载缺省值
	{
	if(!reCheck)
	{
	EepromSetDefEX();
	reCheck=true;
	goto READ;
	}
	else
	if(((uint16)EepromBufferBasicAdd+sizeof(EepromBuffer))<MAX_ADD)
	{
	EepromBufferBasicAdd+=sizeof(EepromBuffer);
	reCheck = false;
	goto READ;
	}
	else
	{
	EepromSetDefEX();
	}
	}*/
#endif//__C51__
#ifdef __AVR__
	uint8 i=0;
	for(;i<ESL_end;i++)
	{
		eeprom_busy_wait();
		_EEGET(EepromBuffer[i],i);

	}
#endif//__AVR__
}
