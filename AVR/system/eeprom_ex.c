#include <global.h>
#include <string.h>
#include "eeprom_ex.h"
#include "boolean.h"
#include <avr/eeprom.h> 

uint8 perchdata EepromBuffer[ESL_end];//���ݻ���
uint8 perchdata EepromDataCh[ESL_end/8+((ESL_end%8)?1:0)];
bool  EepromDataChanged;

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
#else
#define EepromBufferBasicAdd 0
#endif//EEPROM_CHECK_DATA

void EepromSaveChar(uint8 loc,uint8 dat)	//����һ���ֽڣ�loc=λ�ã�dat=����
{
	EepromBuffer[loc] = dat;
	SetBit(EepromDataCh,loc,true);
	EepromDataChanged = true;
}
void EepromWriteEX()							//д�������ݵ����豸
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
				eeprom_busy_wait();
				eeprom_write_byte((uint8 *)(i+EepromBufferBasicAdd),EepromBuffer[i]);
			}
		}
		EepromDataChanged = false;
	}
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
	memset(EepromDataCh,0xff,sizeof(EepromDataCh));
	EepromDataChanged = true;
	EepromWriteEX();	
}
static void read()
{
	eeprom_busy_wait();
	eeprom_read_block(EepromBuffer,(uint8 *)(uint16)EepromBufferBasicAdd,sizeof(EepromBuffer));
}
void IniEeprom_ex()
{	
#ifdef EEPROM_CHECK_DATA
	uint16 add = EepromBufferBasicAdd;
	while(add+sizeof(EepromBuffer)<EEPROM_EX_SIZE)
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
		}
		//���²��Ըո�д�������
		read();
		if(EepromBuffer[ESL_addupcheck]==getCheck())
		{
			return;
		}
		else
		{
			add+=sizeof(EepromBuffer);//����Ч��ʧ���������
		}
	}
	EepromSetDefEX();//���г���ʧ�ܼ���ȱʡֵ
#else//EEPROM_CHECK_DATA
read();
#endif//EEPROM_CHECK_DATA
}
