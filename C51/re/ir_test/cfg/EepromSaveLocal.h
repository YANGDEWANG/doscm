#ifndef _EEPROMSAVELOCAL_H_
#define _EEPROMSAVELOCAL_H_
//#include "system\eeprom_ex.h"
enum ESL//eepromSaveLocal
{
	ESL_M62446MainVolume,
	ESL_M62446VolumeL,
	ESL_M62446VolumeR,
	ESL_M62446VolumeCC,
	ESL_M62446VolumeSW,
	ESL_M62446VolumeSL,
	ESL_M62446VolumeSR,
	ESL_M62446Bass,
	ESL_M62446Treble,
	ESL_M62446OutputPort,
	ESL_M62446WorkMode,
	ESL_PT2314Volume,
	ESL_PT2314Bass,
	ESL_PT2314Treble,
	ESL_PT2314SpeakerATT,
	ESL_PT2314Loudness,


#ifdef EEPROM_CHECK_DATA
	ESL_addupcheck,//����λ��Ҫ���µ�ֵ����ӵ�ǰ��
#endif//EEPROM_CHECK_DATA
	ESL_end,
};//__attribute__ ((packed)),//��ʹ�û����eeprom����,��ʹ��Ƶ�ȸߵ�


#endif//_EEPROMSAVELOCAL_H_	
