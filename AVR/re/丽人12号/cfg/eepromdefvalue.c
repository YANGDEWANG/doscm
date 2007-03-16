#include <global.h>
#include "eeprom_ex.h"


prog_char EepromDV[EEPROMDV_DATASIZE]=//EepromDefaultValue
{
	20,//ESL_MainVolume
	0,	//ESL_PT2314Bass,
	0,	//ESL_PT2314Treble,
	0,	//ESL_PT2314SpeakerATT,
	10,	//ESL_VolumeSL,
	10,	//ESL_VolumeSR,
	10,	//ESL_VolumeSW,
	10,	//ESL_VolumeCC,
	40,	//ESL_PT2314_2Volume,
	0,	//ESL_PT2314_2Bass,
	0,	//ESL_PT2314_2Treble,
	-7,//ESL_PT2314_2SpeakerATT,
	0,//ESL_OKDelay
	0,//ESL_TrackMode
	0,	//ESL_PT2314_2Loudness,
	1,	//ESL_PT2314Volume,
	25,	//ESL_PT2314Loudness,
	0,//ESL_PingPu
};
