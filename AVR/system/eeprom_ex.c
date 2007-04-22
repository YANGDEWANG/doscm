#include <global.h>
#include <string.h>
#include <system/eeprom_ex.h>
#include "boolean.h"
#include <avr/eeprom.h> 

uint8 perchdata EepromBuffer[ESL_end];//���ݻ���
#ifndef EEPROM_EX_FORCE_WRITE
uint8 perchdata EepromDataCh[ESL_end/8+((ESL_end%8)?1:0)];
bool  EepromDataChanged;
#endif//EEPROM_EX_FORCE_WRITE
#ifdef EEPROM_CHECK_DATA
uint8 EepromBufferBasicAdd;
static uint8 getCheck()//Ч��ֵΪ�������ݺ͵İ�λȡ��
{
	uint8 i=0;
	uint8 d=0;
	for(;i<ESL_addupcheck;i++)
	{
		d+=EepromBuffer[i];
	}
	//if(d==0)d++;//����CheckֵΪ0�������CheckֵΪ0�Ϳ����ж�Ϊ����ʵЧ
	return ~d;
}
void EepromSetDefEX()
{
	//memcpy(EepromBuffer,EepromDV,sizeof(EepromDV));//ʹ����260byte flash
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

void EepromSaveChar(uint8 loc,uint8 dat)	//����һ���ֽڣ�loc=λ�ã�dat=����
{
	EepromBuffer[loc] = dat;
#ifndef EEPROM_EX_FORCE_WRITE
	SetBit(EepromDataCh,loc);
	EepromDataChanged = true;
#endif//EEPROM_EX_FORCE_WRITE
}
void EepromWriteEX()							//д�������ݵ����豸
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
		EepromSetDefEX();//����Ч��ʧ�ܼ���ȱʡֵ
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
			EepromSetDefEX();//����Ч��ʧ�ܼ���ȱʡֵ
			if(retry)
			{
				retry=false;
				add+=sizeof(EepromBuffer);//����Ч��ʧ���������
			}
			else
			{
				retry=true;
			}
		}
	}
	EepromSetDefEX();//���г���ʧ�ܼ���ȱʡֵ
#endif
#else//EEPROM_CHECK_DATA
	read();
#endif//EEPROM_CHECK_DATA
}
