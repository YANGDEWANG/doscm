#ifndef _EEPROMSAVELOCAL_H_
#define _EEPROMSAVELOCAL_H_
enum ESL//eepromSaveLocal
{
	ESL_MainVolume,
	ESL_PT2314Bass,
	ESL_PT2314Treble,
	ESL_PT2314SpeakerATT,
	ESL_VolumeSL,//sl//更改顺序请同时更改void c_M62446(bool aors)
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
	ESL_addupcheck,//这两位不要动新的值需添加到前面
#endif//EEPROM_CHECK_DATA
	ESL_end,
};//__attribute__ ((packed)),//需使用缓存的eeprom变量,即使用频度高的

#define  ESL_OK_HUNXIANG		ESL_PT2314_2SpeakerATT
//#define  ESL_OK_HUNXIANG_MAX	PT2314_2_MAXSPEAKERATTENUATORS

#endif//_EEPROMSAVELOCAL_H_	
