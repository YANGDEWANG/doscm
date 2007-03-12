#include <global.h>
#include "ir.h"
#include "pointvfddisplay.h"
#include "ir_command.h"
#include "c.h"
#include "sysstring.h"
bool noCallShowState;
bool noInUserEvent;

irc PROGMEM irc_com[IRC_MAX] =
{
	IRC_vcd					,
	IRC_ac_3  				,
	//IRC_tuner				,
	IRC_soundfield  		,
	IRC_aux					,
	IRC_reset  				,
	IRC_track_mode			,
	IRC_denxiang    		,
	IRC_onekeydvd			,
	IRC_ok_delay		 	,	
	IRC_pingpu				,
	IRC_left	 			,
	IRC_right				,
	IRC_mute 				,
	IRC_ok_diyinadd		,	
	IRC_ok_diyinsub		,	
	IRC_ok_gaoyinadd		,	
	IRC_ok_gaoyinsub		,	
	IRC_ok_volumeadd		,	
	IRC_ok_volumesub		,
	IRC_sladd	   			,
	IRC_slsub				,
	IRC_cenadd  			,
	IRC_censub				,
	IRC_sradd   			,
	IRC_srsub				,
	IRC_swadd 				,
	IRC_swsub				,
	IRC_volumeadd  			,
	IRC_volumesub			,
	IRC_diyinadd 			,
	IRC_diyinsub			,
	IRC_gaoyinadd 			,
	IRC_gaoyinsub			,
	IRC_reverberationadd	,
	IRC_reverberationsub	,
	IRC_CHMode2_1,
	IRC_CHMode5_1,
	IRC_null,
};
#if 1//丽人
prog_char irc_table1[] =
{
#define IRC_TABLE_MIN1 0
	IRC_CENSUB	,			//	0x00
	IRC_REVERBERATIONADD,	//	0x01
	IRC_OK_DELAY	,		//	0x02
	IRC_NULL	,			//	0x03
	IRC_MUTE 	,			//	0x04
	IRC_VOLUMESUB	,		//	0x05
	IRC_GAOYINSUB	,		//	0x06
	IRC_NULL	,			//	0x07
	IRC_NULL	,			//	0x08
	IRC_NULL	,			//	0x09
	IRC_NULL	,			//	0x0A
	IRC_NULL	,			//	0x0B
	IRC_REVERBERATIONSUB,	//	0x0c
	IRC_VOLUMEADD  	,		//	0x0d
	IRC_GAOYINADD 	,		//	0x0e
	IRC_NULL	,			//	0x0F
	IRC_OK_GAOYINADD	,	//	0x10
	IRC_OK_GAOYINSUB	,	//	0x11
	IRC_OK_VOLUMESUB	,	//	0x12
	IRC_NULL	,			//	0x13
	IRC_NULL	,			//	0x14
	IRC_NULL	,			//	0x15
	IRC_NULL	,			//	0x16
	IRC_NULL	,			//	0x17
	IRC_ONEKEYDVD	,		//	0x18
	IRC_CENADD  	,		//	0x19
	IRC_SRADD   	,		//	0x1a
	IRC_NULL	,			//	0x1B
	IRC_AUX	,				//	0x1cIRC_TUNER
	IRC_SOUNDFIELD	,		//	0x1d
	IRC_RESET  	,			//	0x1e
};
//	,				
//	,
prog_char irc_table2[] =
{
#define IRC_TABLE_MIN2 0x41
	IRC_SRSUB	,			//	0x41
	IRC_SWADD 	,			//	0x42
	IRC_SLSUB	,			//	0x43
	IRC_NULL	,			//	0x44
	IRC_LEFT	,			//	0x45
	IRC_DIYINSUB	,		//	0x46
	IRC_RIGHT	,			//	0x47
	IRC_NULL	,			//	0x48
	IRC_NULL	,			//	0x49
	IRC_NULL	,			//	0x4A
	IRC_NULL	,			//	0x4B
	IRC_NULL	,			//	0x4C
	IRC_PINGPU	,			//	0x4d
	IRC_DIYINADD 	,		//	0x4e
	IRC_SWSUB	,			//	0x4f
	IRC_NULL	,			//	0x50
	IRC_OK_VOLUMEADD	,	//	0x51
	IRC_OK_DIYINSUB	,		//	0x52
	IRC_OK_DIYINADD	,		//	0x53
	IRC_NULL	,			//	0x54
	IRC_NULL	,			//	0x55
	IRC_NULL	,			//	0x56
	IRC_NULL	,			//	0x57
	IRC_NULL	,			//	0x58
	IRC_DENXIANG    	,	//	0x59
	IRC_SLADD	,			//	0x5a
	IRC_CHMODE5_1	,		//	0x5b
	IRC_NULL	,			//	0x5C
	IRC_AC_3  	,			//	0x5d
	IRC_CHMODE2_1  	,		//	0x5e
	IRC_VCD	,				//	0x5f
};
#else
prog_char irc_table1[] =
{
#define IRC_TABLE_MIN1 0
	IRC_CENSUB	,			//	0x00
	IRC_REVERBERATIONADD,	//	0x01
	IRC_OK_DELAY	,		//	0x02
	IRC_NULL	,			//	0x03
	IRC_MUTE 	,			//	0x04
	IRC_VOLUMESUB	,		//	0x05
	IRC_GAOYINSUB	,		//	0x06
	IRC_NULL	,			//	0x07
	IRC_NULL	,			//	0x08
	IRC_NULL	,			//	0x09
	IRC_NULL	,			//	0x0A
	IRC_NULL	,			//	0x0B
	IRC_REVERBERATIONSUB,	//	0x0c
	IRC_VOLUMEADD  	,		//	0x0d
	IRC_GAOYINADD 	,		//	0x0e
	IRC_NULL	,			//	0x0F
	IRC_OK_GAOYINADD	,	//	0x10
	IRC_OK_GAOYINSUB	,	//	0x11
	IRC_OK_VOLUMESUB	,	//	0x12
	IRC_NULL	,			//	0x13
	IRC_NULL	,			//	0x14
	IRC_NULL	,			//	0x15
	IRC_NULL	,			//	0x16
	IRC_NULL	,			//	0x17
	IRC_ONEKEYDVD	,		//	0x18
	IRC_CENADD  	,		//	0x19
	IRC_SRADD   	,		//	0x1a
	IRC_NULL	,			//	0x1B
	IRC_TUNER	,			//	0x1c
	IRC_AUX	,				//	0x1d
	IRC_RESET  	,			//	0x1e
};
//	,				
//	,
prog_char irc_table2[] =
{
#define IRC_TABLE_MIN2 0x41
	IRC_SRSUB	,			//	0x41
	IRC_SWADD 	,			//	0x42
	IRC_SLSUB	,			//	0x43
	IRC_NULL	,			//	0x44
	IRC_LEFT	,			//	0x45
	IRC_DIYINSUB	,		//	0x46
	IRC_RIGHT	,			//	0x47
	IRC_NULL	,			//	0x48
	IRC_NULL	,			//	0x49
	IRC_NULL	,			//	0x4A
	IRC_NULL	,			//	0x4B
	IRC_NULL	,			//	0x4C
	IRC_PINGPU	,			//	0x4d
	IRC_DIYINADD 	,		//	0x4e
	IRC_SWSUB	,			//	0x4f
	IRC_NULL	,			//	0x50
	IRC_OK_VOLUMEADD	,	//	0x51
	IRC_OK_DIYINSUB	,		//	0x52
	IRC_OK_DIYINADD	,		//	0x53
	IRC_NULL	,			//	0x54
	IRC_NULL	,			//	0x55
	IRC_NULL	,			//	0x56
	IRC_NULL	,			//	0x57
	IRC_NULL	,			//	0x58
	IRC_DENXIANG    	,	//	0x59
	IRC_SLADD	,			//	0x5a
	IRC_TRACK_MODE	,		//	0x5b
	IRC_NULL	,			//	0x5C
	IRC_AC_3  	,			//	0x5d
	IRC_SOUNDFIELD  	,	//	0x5e
	IRC_VCD	,				//	0x5f
};
#endif
void IRC_CHMode2_1()
{
	SetTrackMode(TM_2_1CH);
}
void IRC_CHMode5_1()
{
	SetTrackMode(TM_5_1CH);
}
void IRC_vcd			()
{
	SetIntput(INTPUT_VCD);
	ShowIntput(INTPUT_VCD);
}
void IRC_ac_3  			()
{
	SetIntput(INTPUT_AC3);
	ShowIntput(INTPUT_AC3);
}
//void IRC_tuner			()
//{
//	IRC_aux();
//}
prog_char soundfield[]=
{
	//T,B
	0,0,
	5,0,
	0,5,
	5,5,
	5,-5,
	-5,5,
};
prog_char effect[]="EFFECT";
void IRC_soundfield		()
{
	static uint8 sf = 0;
	uint8 sf2;
	sf++;
	/*if(sf==sizeof(soundfield)/2)
	{
	sf=0;
	}*/
	sf = sf%(sizeof(soundfield)/2);
	sf2= sf*2;
	EepromSaveChar(ESL_PT2314Bass,pgm_read_byte(soundfield+sf2));
	EepromSaveChar(ESL_PT2314Treble,pgm_read_byte(soundfield+sf2+1));
	noCallShowState = true;
	ShowStringAndI8_P2(effect,ssnull,sf);
	PT2314UpdateAll();
}
void IRC_aux			()
{
	SetIntput(INTPUT_AUX);
	ShowIntput(INTPUT_AUX);
}
extern	void IniDev();
void IRC_reset  		()
{
	EepromSetDefEX();
	((irc)0)();
	//IniDev();
	//iniPoll();
}
void IRC_track_mode		()
{
	ControlState = CS_TRACK_MODE;
	CVolume(true);
}
prog_char ssLOUD_[]="LOUD ";
void IRC_denxiang    	()
{
	if(PT2314_LoudnessGet())
	{
		PT2314_LoudnessClear();
		ShowString_P2(ssLOUD_,ssOFF,POINTVFDDISPLAY_X_C);
	}
	else
	{
		PT2314_LoudnessSet();
		ShowString_P2(ssLOUD_,ssON,POINTVFDDISPLAY_X_C);
	}
	EepromSaveChar(ESL_PT2314_2Loudness,PT2314Loudness);
	noCallShowState = true;
	PT2314UpdateAll();
}
void IRC_onekeydvd		()
{
	SetIntput(INTPUT_AC3);
	ShowIntput(INTPUT_DVD);
}


void IRC_ok_delay()
{
	ControlState = CS_OK_DELAY;
	CVolume(true);
}

void IRC_pingpu			()
{
	noCallShowState = true;
	noInUserEvent = true;
	u8 pu = PUType+1;
	pu%=PP_OFF;
	if(ShowPingPu)
		DISClean();
	else
		ShowStringAndI8_P2(ssDISPLAY,ssnull,pu);
	EepromSaveChar(ESL_PingPu,pu);
	//ShowPingPu = true;
}
void IRC_att(bool rorl)
{
	/*ControlState = CS_VOLUME_R;
	CVolume(rorl);
	ControlState = CS_VOLUME_L;
	CVolume(!rorl);*/
	ControlState = CS_VOLUME_ATT;
	CVolume(rorl);

}
void IRC_left	 		()
{
	IRC_att(true);
}
void IRC_right			()
{
	IRC_att(false);
}
void IRC_mute 			()
{
	if(Mute)
	{
		AutoControl.Step=1;
		ShowString_P2(ssMUTE_,ssOFF,POINTVFDDISPLAY_X_C);
	}
	else
	{
		AutoControl.Step=-3;
		ShowString_P2(ssMUTE_,ssON,POINTVFDDISPLAY_X_C);
		//InUserEventONAutoExit();
	}
	noCallShowState = true;
	Mute = !Mute;
	/*if(M62446Mute)
	M62446ToSound();
	else
	M62446ToMute();
	M62446Mute = !M62446Mute;*/
}
//extern uint8 __t1,__t2;
void IRC_ok_diyinadd	()
{
	//noCallShowState = true;
	//	ShowUINT8(EepromBuffer[ESL_addupcheck]);
	//WriteByte24c02(ESL_addupcheck,EepromBuffer[ESL_addupcheck]);
	ControlState = CS_OK_DEYIN;
	CVolume(true);
}
//int8 __t3;
void IRC_ok_diyinsub	()
{	
	//noCallShowState = true;
	//	ReadByte24c02(ESL_addupcheck+EepromBufferBasicAdd,&__t3);
	//ShowUINT8(__t3);
	//ShowUINT8(getCheck());
	ControlState = CS_OK_DEYIN;
	CVolume(false);
}
void IRC_ok_gaoyinadd	()
{
	//noCallShowState = true;
	//ShowUINT8(EepromBufferBasicAdd);
	ControlState = CS_OK_GAOYIN;
	CVolume(true);
}
void IRC_ok_gaoyinsub	()
{
	/*static uint8 i =0;
	noCallShowState = true;
	ReadByte24c02(i++,&__t3);
	ShowUINT8(__t3);*/
	ControlState = CS_OK_GAOYIN;
	CVolume(false);
}
void IRC_ok_volumeadd	()
{
	ControlState = CS_OK_VOLUME;
	CVolume(true);
}
void IRC_ok_volumesub	()
{
	ControlState = CS_OK_VOLUME;
	CVolume(false);
}
void IRC_sladd	   		()
{
	ControlState = CS_VOLUME_SL;
	CVolume(true);
}
void IRC_slsub			()
{
	ControlState = CS_VOLUME_SL;
	CVolume(false);
}
void IRC_cenadd  		()
{
	ControlState = CS_VOLUME_CC;
	CVolume(true);
}
void IRC_censub			()
{
	ControlState = CS_VOLUME_CC;
	CVolume(false);
}
void IRC_sradd   		()
{
	ControlState = CS_VOLUME_SR;
	CVolume(true);
}
void IRC_srsub			()
{
	ControlState = CS_VOLUME_SR;
	CVolume(false);
}
void IRC_swadd 			()
{
	ControlState = CS_VOLUME_SW;
	CVolume(true);
}
void IRC_swsub			()
{
	ControlState = CS_VOLUME_SW;
	CVolume(false);
}
void IRC_volumeadd  	()
{
	ControlState = CS_VOLUME_MAIN;
	CVolume(true);
}
void IRC_volumesub		()
{
	ControlState = CS_VOLUME_MAIN;
	CVolume(false);
}
void IRC_diyinadd 		()
{
	ControlState = CS_BASS;
	CVolume(true);
}
void IRC_diyinsub		()
{
	ControlState = CS_BASS;
	CVolume(false);
}
void IRC_gaoyinadd 		()
{
	ControlState = CS_TREBLE;
	CVolume(true);
}
void IRC_gaoyinsub		()
{	
	ControlState = CS_TREBLE;
	CVolume(false);
}
void IRC_reverberationadd()
{
	ControlState = CS_OK_HUNXIANG;
	CVolume(true);
}
void IRC_reverberationsub()
{
	ControlState = CS_OK_HUNXIANG;
	CVolume(false);
}
void IRC_null(){}
void callKeyP(uint8 key)
{
	noInUserEvent=false;
	noCallShowState=false;
	((irc)pgm_read_word(irc_com+key))();
	if(!noInUserEvent)
		InUserEvent();
	if(!noCallShowState)
		ShowState();
}
void keyDown(uint8 IRkey)
{

	uint8 key = IRkey-IRC_TABLE_MIN1;
	if(key<sizeof(irc_table1))
	{
		key= pgm_read_byte(irc_table1+key);
		callKeyP(key);
	}
	else
	{
		key = IRkey-IRC_TABLE_MIN2;
		if(key<sizeof(irc_table2))
		{
			key=pgm_read_byte(irc_table2+key);
			callKeyP(key);
		}
	}
}
//注意调用时间间隔应大于108ms
void PollingIRKey()
{
	static bool PkeyHold;
	static uint8 firstHoldDely = FirstKeyHoldEventCyc;
	//ShowUINT8(GetKey_IR());
	if(IRKeyDown)
	{
		IRKeyDown = false;
		PkeyHold = true;
		keyDown(IrKey);
		return;
	}
	if(IRKeyHold)
	{
		IRKeyHold =false;
		if(PkeyHold)
		{
			if(firstHoldDely>0)
			{
				firstHoldDely--;
			}
			else
			{
				keyDown(IrKey);
			}

		}
		return;
	}

	PkeyHold = false;
	firstHoldDely = FirstKeyHoldEventCyc;
}
prog_char ssIntputX[]="DVDAUX CDAC3";
void ShowIntput(u8 intput)
{
	ShowString_P(ssINTPUT,0,6);
	ShowString_P(ssIntputX+intput*3,6*CHARIMAGE_W,3);
	noCallShowState=true;
	/*switch(intput)
	{
	case  INTPUT_AUX :
	{
	ShowString_P(ssAUX,6*CHARIMAGE_W,3);
	break;
	}
	case  INTPUT_VCD :
	{
	ShowString_P(ssCD,6*CHARIMAGE_W,3);
	break;
	}
	case  INTPUT_AC3 :
	{
	ShowString_P(ssAC3,6*CHARIMAGE_W,3);
	break;
	}
	case  INTPUT_DVD :
	{
	ShowString_P(ssDVD,6*CHARIMAGE_W,3);
	break;
	}
	}*/
}



