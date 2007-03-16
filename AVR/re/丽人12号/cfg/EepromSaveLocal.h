#ifndef _EEPROMSAVELOCAL_H_
#define _EEPROMSAVELOCAL_H_
enum ESL//eepromSaveLocal
{
	ESL_MainVolume,
	ESL_PT2314Bass,
	ESL_PT2314Treble,
	ESL_PT2314SpeakerATT,
	ESL_VolumeSL,//sl//����˳����ͬʱ����void c_M62446(bool aors)
	ESL_VolumeSR,//sr
	ESL_VolumeSW,//sw
	ESL_VolumeCC,//cc
	ESL_PT2314_2Volume,
	ESL_PT2314_2Bass,
	ESL_PT2314_2Treble,
	ESL_PT2314_2SpeakerATT,
	ESL_OKDelay,
	ESL_TrackMode,
	ESL_PT2314_2Loudness,
	ESL_PT2314Volume,
	ESL_PT2314Loudness,
	ESL_PingPu,
#ifdef EEPROM_CHECK_DATA
	ESL_addupcheck,//����λ��Ҫ���µ�ֵ����ӵ�ǰ��
#endif//EEPROM_CHECK_DATA
	ESL_end,
};//__attribute__ ((packed)),//��ʹ�û����eeprom����,��ʹ��Ƶ�ȸߵ�

#define  ESL_OK_HUNXIANG		ESL_PT2314_2SpeakerATT
//#define  ESL_OK_HUNXIANG_MAX	PT2314_2_MAXSPEAKERATTENUATORS

#endif//_EEPROMSAVELOCAL_H_	
