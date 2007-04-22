#include <global.h>
#include <string.h>
#include <system/eeprom_ex.h>
#include "boolean.h"
#include <avr/eeprom.h> 

uint8 perchdata EepromBuffer[ESL_end];//数据缓存
#ifndef EEPROM_EX_FORCE_WRITE
uint8 perchdata EepromDataCh[ESL_end/8+((ESL_end%8)?1:0)];
bool  EepromDataChanged;
#endif//EEPROM_EX_FORCE_WRITE
#ifdef EEPROM_CHECK_DATA
uint8 EepromBufferBasicAdd;
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
void EepromSetDefEX()
{
	//memcpy(EepromBuffer,EepromDV,sizeof(EepromDV));//使用了260byte flash
	//uint8 i;
	//for(i=0;i<sizeof(EepromDV);i++)
	//{
	//	EepromBuffer[i] = EepromDV[i];
	//}
	memcpy_P(EepromBuffer,EepromDV,sizeof(EepromDV));
	dwmemset(EepromDataCh,0xff,sizeof(EepromDataCh));
	/*u8 i;
	for(i=0;i<sizeof(EepromDataCh);i++)
	{
	EepromDataCh[i]=0xff;
	}*/
	EepromDataChanged = true;
	EepromWriteEX();	
}
#else
#define EepromBufferBasicAdd 0
#endif//EEPROM_CHECK_DATA

void EepromSaveChar(uint8 loc,uint8 dat)	//保存一个字节，loc=位置，dat=数据
{
	EepromBuffer[loc] = dat;
#ifndef EEPROM_EX_FORCE_WRITE
	SetBit(EepromDataCh,loc);
	EepromDataChanged = true;
#endif//EEPROM_EX_FORCE_WRITE
}
void EepromWriteEX()							//写缓存数据到到设备
{
#ifdef EEPROM_EX_FORCE_WRITE
#ifdef EEPROM_CHECK_DATA
		EepromSaveChar(ESL_addupcheck,getCheck());
#endif//EEPROM_CHECK_DATA
		eeprom_busy_wait();
		eeprom_write_block(EepromBuffer,0,ESL_end);

#else//EEPROM_EX_FORCE_WRITE
	
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
				eeprom_busy_wait();
				eeprom_write_byte((uint8 *)(i+EepromBufferBasicAdd),EepromBuffer[i]);
			}
		}
		EepromDataChanged = false;
	}
#endif//EEPROM_EX_FORCE_WRITE
}


static void read()
{
	eeprom_busy_wait();
	eeprom_read_block(EepromBuffer,(uint8 *)(uint16)EepromBufferBasicAdd,sizeof(EepromBuffer));
}
void IniEeprom_ex()
{	
#ifdef EEPROM_CHECK_DATA
#if 0
	read();
	if(EepromBuffer[ESL_addupcheck]==getCheck())
	{
		return;
	}
	else
	{
		EepromSetDefEX();//数据效验失败加载缺省值
	}

#else
	uint16 add = EepromBufferBasicAdd;
	bool retry = false;
	while((add+sizeof(EepromBuffer))<EEPROM_EX_SIZE)
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
			if(retry)
			{
				retry=false;
				add+=sizeof(EepromBuffer);//数据效验失败提高区块
			}
			else
			{
				retry=true;
			}
		}
	}
	EepromSetDefEX();//所有尝试失败加载缺省值
#endif
#else//EEPROM_CHECK_DATA
	read();
#endif//EEPROM_CHECK_DATA
}
