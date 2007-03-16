#ifndef _IR_COMMAND_H_
#define _IR_COMMAND_H_
#include "configure.h"

#define FirstKeyHoldEventCyc	5
#define SecondKeyHoldEventCyc	1

/*
#define IRC_VCD					0x5f
#define IRC_AC_3  				0x5d
#define IRC_TUNER				0x1c
#define IRC_SOUNDFIELD  		0x5e
#define IRC_AUX					0x1d
#define IRC_RESET  				0x1e
#define IRC_TRACK_MODE			0x5b
#define IRC_DENXIANG    		0x59
#define IRC_ONEKEYDVD			0x18
#define IRC_OK_DELAY		 	0x02	
#define IRC_PINGPU				0x4d
#define IRC_LEFT	 			0x45
#define IRC_RIGHT				0x47
#define IRC_MUTE 				0x04
#define IRC_OK_DIYINADD		0x53	
#define IRC_OK_DIYINSUB		0x52	
#define IRC_OK_GAOYINADD		0x10	
#define IRC_OK_GAOYINSUB		0x11	
#define IRC_OK_VOLUMEADD		0x51	
#define IRC_OK_VOLUMESUB		0x12
#define IRC_SLADD	   			0x5a
#define IRC_SLSUB				0x43
#define IRC_CENADD  			0x19
#define IRC_CENSUB				0x00
#define IRC_SRADD   			0x1a
#define IRC_SRSUB				0x41
#define IRC_SWADD 				0x41
#define IRC_SWSUB				0x4f
#define IRC_VOLUMEADD  			0x0d
#define IRC_VOLUMESUB			0x05
#define IRC_DIYINADD 			0x4e
#define IRC_DIYINSUB			0x46
#define IRC_GAOYINADD 			0x0e
#define IRC_GAOYINSUB			0x06
#define IRC_REVERBERATIONADD	0x01
#define IRC_REVERBERATIONSUB	0x0c
*/
enum IRC//irc
{
IRC_VCD					,
IRC_AC_3  				,
//IRC_TUNER				,
IRC_SOUNDFIELD  		,
IRC_AUX					,
IRC_RESET  				,
IRC_TRACK_MODE			,
IRC_DENXIANG    		,
IRC_ONEKEYDVD			,
IRC_OK_DELAY		 	,	
IRC_PINGPU				,
IRC_LEFT	 			,
IRC_RIGHT				,
IRC_MUTE 				,
IRC_OK_DIYINADD		,	
IRC_OK_DIYINSUB		,	
IRC_OK_GAOYINADD		,	
IRC_OK_GAOYINSUB		,	
IRC_OK_VOLUMEADD		,	
IRC_OK_VOLUMESUB		,
IRC_SLADD	   			,
IRC_SLSUB				,
IRC_CENADD  			,
IRC_CENSUB				,
IRC_SRADD   			,
IRC_SRSUB				,
IRC_SWADD 				,
IRC_SWSUB				,
IRC_VOLUMEADD  			,
IRC_VOLUMESUB			,
IRC_DIYINADD 			,
IRC_DIYINSUB			,
IRC_GAOYINADD 			,
IRC_GAOYINSUB			,
IRC_REVERBERATIONADD	,
IRC_REVERBERATIONSUB	,
IRC_CHMODE2_1,
IRC_CHMODE5_1,
IRC_OK_PREESTABLISH,
IRC_NULL,
IRC_MAX,
};
extern void IRC_vcd				();
extern void IRC_ac_3  			();	
//extern void IRC_tuner			();	
extern void IRC_soundfield		();	
extern void IRC_aux				();	
extern void IRC_reset  			();
extern void IRC_track_mode		();
extern void IRC_denxiang    	();
extern void IRC_onekeydvd		();
extern void IRC_ok_delay		();	
extern void IRC_pingpu			();
extern void IRC_left	 		();
extern void IRC_right			();
extern void IRC_mute 			();
extern void IRC_ok_diyinadd		();	
extern void IRC_ok_diyinsub		();	
extern void IRC_ok_gaoyinadd	();	
extern void IRC_ok_gaoyinsub	();	
extern void IRC_ok_volumeadd	();	
extern void IRC_ok_volumesub	();
extern void IRC_sladd	   		();
extern void IRC_slsub			();
extern void IRC_cenadd  		();
extern void IRC_censub			();
extern void IRC_sradd   		();
extern void IRC_srsub			();
extern void IRC_swadd 			();
extern void IRC_swsub			();
extern void IRC_volumeadd  		();
extern void IRC_volumesub		();
extern void IRC_diyinadd 		();
extern void IRC_diyinsub		();
extern void IRC_gaoyinadd 		();
extern void IRC_gaoyinsub		();
extern void IRC_reverberationadd();
extern void IRC_reverberationsub();
void 	IRC_CHMode2_1();
void 	IRC_CHMode5_1();
void 	IRC_ok_preestablish();
extern void IRC_null();
typedef void (*irc)();
extern irc irc_com[IRC_MAX] PROGMEM;
extern void PollingIRKey();
void ShowIntput(u8 intput);
extern bool noCallShowState;
extern bool noInUserEvent;




#endif//_IR_COMMAND_H_





