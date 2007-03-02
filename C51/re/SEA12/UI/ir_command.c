#include <global.h>
#include "ir.h"
#include "at24c02.h"
#include "ir_command.h"
#include "c.h"
bool noUpdateDis;
irc code irc_com[IRC_MAX]=
{
	IRC_vcd					,
	IRC_ac_3  				,
	IRC_tuner				,
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
	IRC_null,
};
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
void IRC_vcd			()
{
	ControlState = CS_INTPUT_SELECT;
	M62446OutputportSet(INTPUT_CD);
	//M62446Update(M62446TONE_OUTPUT);
	M62446ToSound();
}
void IRC_ac_3  			()
{
	ControlState = CS_INTPUT_SELECT;
	M62446OutputportSet(INTPUT_5_1);
	//M62446Update(M62446TONE_OUTPUT);
	M62446ToSound();
}
void IRC_tuner			()
{
	IRC_aux();
}
prog_char soundfield[]=
{
	//T,B
	5,0,
	0,5,
	5,5,
	5,-5,
	-5,5,
	0,0,
};
void IRC_soundfield		()
{
	static uint8 sf = 0;
	uint8 sf2;
	sf++;
	if(sf==sizeof(soundfield)/2)
	{
		sf=0;
	}
	sf2= sf*2;
	EepromSaveChar(ESL_M62446Treble,soundfield[sf2]);

	EepromSaveChar(ESL_M62446Treble,soundfield[sf2+1]);
	noUpdateDis = true;
	ShowUINT8(sf);
	M62446UpdateAll();
}
void IRC_aux			()
{
	ControlState = CS_INTPUT_SELECT;
	M62446OutputportSet(INTPUT_AUX);
	//M62446Update(M62446TONE_OUTPUT);
	M62446ToSound();
}
extern	void IniDev();
void IRC_reset  		()
{
	EepromSetDefEX();
	IniDev();
}
void IRC_track_mode		()
{
	ControlState = CS_TRACK_MODE;
	CVolume(true);
}
void IRC_denxiang    	(){}
void IRC_onekeydvd		()
{
	IRC_ac_3();
}
prog_char ok_delay[]=
{
	3<<6,
	1<<6,
	2<<6,
	0<<6,
};
#define ok_P_MASK (3<<6)
#define SetOKDelay(okd) (P3&=~ok_P_MASK,P3|=okd)
void IRC_ok_delay()
{
	static uint8 okd = 0;
	noUpdateDis = true;
	if(!ok_mode)
	return;
	okd++;
	if(okd==sizeof(ok_delay))
		okd=0;
	SetOKDelay(ok_delay[okd]);
	
	ShowUINT8(okd);
}
void IRC_pingpu			(){}
void IRC_att(bool rorl)
{
	ControlState = CS_VOLUME_R;
	CVolume(rorl);
	ControlState = CS_VOLUME_L;
	CVolume(!rorl);
}
void IRC_left	 		()
{
	IRC_att(false);
}
void IRC_right			()
{
	IRC_att(true);
}
void IRC_mute 			()
{
	if(M62446Mute)
		M62446ToSound();
	else
		M62446ToMute();
	M62446Mute = !M62446Mute;
}
//extern uint8 __t1,__t2;
void IRC_ok_diyinadd	()
{
	//noUpdateDis = true;
	//	ShowUINT8(EepromBuffer[ESL_addupcheck]);
	//WriteByte24c02(ESL_addupcheck,EepromBuffer[ESL_addupcheck]);
	ControlState = CS_OK_DEYIN;
	CVolume(true);
}
//int8 __t3;
void IRC_ok_diyinsub	()
{	
	//noUpdateDis = true;
	//	ReadByte24c02(ESL_addupcheck+EepromBufferBasicAdd,&__t3);
	//ShowUINT8(__t3);
	//ShowUINT8(getCheck());
	ControlState = CS_OK_DEYIN;
	CVolume(false);
}
void IRC_ok_gaoyinadd	()
{
	//noUpdateDis = true;
	//ShowUINT8(EepromBufferBasicAdd);
	ControlState = CS_OK_GAOYIN;
	CVolume(true);
}
void IRC_ok_gaoyinsub	()
{
	/*static uint8 i =0;
	noUpdateDis = true;
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

void keyDown()
{

	uint8 key = GetKey_IR()-IRC_TABLE_MIN1;	 
	InUserEvent();
	if(key<sizeof(irc_table1))
	{
		key=irc_table1[key];
		irc_com[key]();
	}else
	{
		key = GetKey_IR()-IRC_TABLE_MIN2;
		if(key<sizeof(irc_table2))
		{
			key=irc_table2[key];
			irc_com[key]();
		}
	}
	if(!noUpdateDis)
	{
		ShowState();
	}
	noUpdateDis = false;
}
void PollingIRKey()
{
	//ShowUINT8(GetKey_IR());
	if(KeyDown_IR)
	{
		KeyDown_IR =false;
		keyDown();
#ifdef SHOWCUSTOMCODE
		ShowUINT8(DATA_IR[0]);
#endif//SHOWCUSTOMCODE
#ifdef IRKEY_TEST
		ShowUINT8(GetKey_IR());
#endif//SHOWCUSTOMCODE
	}
	if(KeyHoldEvent)
	{
		KeyHoldEvent=false;
		keyDown();
	}
}

