#ifndef _EEPROMSAVELOCAL_H_
#define _EEPROMSAVELOCAL_H_
enum ESL//eepromSaveLocal
{
	ESL_PT2314Volume,
	ESL_PT2314Bass,
	ESL_PT2314Treble,
	ESL_PT2314SpeakerATT,
	ESL_PT2314Loudness,
	ESL_PT2314_2Volume,
	ESL_PT2314_2Bass,
	ESL_PT2314_2Treble,
	ESL_PT2314_2SpeakerATT,
	ESL_PT2314_2Loudness,
	ESL_M62429P1VolumeL,//sl
	ESL_M62429P1VolumeR,//sr
	ESL_M62429P2VolumeL,//sw
	ESL_M62429P2VolumeR,//cc
#ifdef EEPROM_CHECK_DATA
	ESL_addupcheck,//����λ��Ҫ���µ�ֵ����ӵ�ǰ��
#endif//EEPROM_CHECK_DATA
	ESL_end,
};//__attribute__ ((packed)),//��ʹ�û����eeprom����,��ʹ��Ƶ�ȸߵ�


#endif//_EEPROMSAVELOCAL_H_	
