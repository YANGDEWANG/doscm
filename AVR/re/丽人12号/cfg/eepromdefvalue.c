#include <global.h>
#include "eeprom_ex.h"


prog_char EepromDV[EEPROMDV_DATASIZE]=//EepromDefaultValue
{
	15,//ESL_MainVolume
	20,	//ESL_PT2314Volume,
	0,	//ESL_PT2314Bass,
	0,	//ESL_PT2314Treble,
	0,	//ESL_PT2314SpeakerATT,
	25,	//ESL_PT2314Loudness,
	20,	//ESL_PT2314_2Volume,
	0,	//ESL_PT2314_2Bass,
	0,	//ESL_PT2314_2Treble,
	0,	//ESL_PT2314_2SpeakerATT,
	0,	//ESL_PT2314_2Loudness,
	20,	//ESL_M62429P1VolumeL,
	20,	//ESL_M62429P1VolumeR,
	20,	//ESL_M62429P2VolumeL,
	20,	//ESL_M62429P2VolumeR,
	0,//ESL_PingPu
	0,//ESL_OKDelay,
	0,//ESL_TrackMode
};
