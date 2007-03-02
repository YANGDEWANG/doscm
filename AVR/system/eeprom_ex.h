#ifndef _EEPROM_EX_H_
#define _EEPROM_EX_H_

#include <configure.h>

#ifndef EEPROM_EX_CFG

//-----------------EEPROM_EX配置----------------------//
#define EEPROM_EX_CFG
#define EEPROM_CHECK_DATA 1 //定义进行数据完整性效验
#define EEPROM_EX_SIZE 256
//---------------EEPROM_EX配置结束--------------------//

#endif//EEPROM_EX_CFG

#include <EepromSaveLocal.h>

#ifdef EEPROM_CHECK_DATA
#define EEPROMDV_DATASIZE (ESL_end-1)
#else
#define EEPROMDV_DATASIZE (ESL_end)
#endif

#define EepromReadChar(loc)	EepromBuffer[(uint8)loc]//读取一个字节，loc=位置，return=数据

extern prog_char EepromDV[EEPROMDV_DATASIZE];//EepromDefaultValue

extern uint8 perchdata EepromBuffer[ESL_end];			//数据缓存
extern uint8 EepromBufferBasicAdd;
extern void IniEeprom_ex();
extern void EepromSaveChar(uint8 loc,uint8 dat);	//保存一个字节，loc=位置，dat=数据
extern void EepromWriteEX();							//写缓存数据到到设备
extern void EepromSetDefEX();

#endif //_EEPROM_EX_H_
