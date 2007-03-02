#ifndef _EEPROM_EX_H_
#define _EEPROM_EX_H_

//#define EEPROM_CHECK_DATA 1 //�����������������Ч��

#include <configure.h>

#ifdef EEPROM_CHECK_DATA
#define EEPROMDV_DATASIZE (ESL_end-1)
#else
#define EEPROMDV_DATASIZE (ESL_end)
#endif

#define EepromReadChar(loc)	EepromBuffer[(uint8)loc]//��ȡһ���ֽڣ�loc=λ�ã�return=����

extern prog_char EepromDV[EEPROMDV_DATASIZE];//EepromDefaultValue

extern uint8 perchdata EepromBuffer[ESL_end];			//���ݻ���
extern uint8 EepromBufferBasicAdd;
extern void IniEeprom_ex();
extern void EepromSaveChar(uint8 loc,uint8 dat);	//����һ���ֽڣ�loc=λ�ã�dat=����
extern void EepromWriteEX();							//д�������ݵ����豸
extern void EepromSetDefEX();

#endif //_EEPROM_EX_H_
