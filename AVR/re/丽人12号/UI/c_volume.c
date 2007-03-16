#include <global.h>
#include "c.h"
#include "ir_command.h"
#include "pointvfddisplay.h"
u8 ControlState;
bool Mute;
//u8 OldMainVolume;
ControlData data VolumeControl;
void CVolumeExe(bool aors)
{
	int8 dat = EepromBuffer[VolumeControl.datESL];
	i8 step = VolumeControl.Step;
	if(!aors)
		step=-step;
	//测试dat是否超出允许范围。是则置中间值
	if(dat>VolumeControl.Max
		&&dat<VolumeControl.Min)
	{
		dat=VolumeControl.Min+(VolumeControl.Max-VolumeControl.Min)/2;
		EepromSaveChar(VolumeControl.datESL,dat);
	}
	dat+=step;
	if(dat<=VolumeControl.Max
		&&dat>=VolumeControl.Min)
	{
		EepromSaveChar(VolumeControl.datESL,dat);
	}
}
void ClearMute(u8 loc)
{
	EepromSaveChar(loc,EepromBuffer[loc]&(~(1<<7)));
	//EepromSaveChar(loc,10);
}
void  SetMute(u8 loc)
{
	EepromSaveChar(loc,EepromBuffer[loc]|(1<<7));
}
static bool inline CheckMute(u8 loc)
{
	return EepromBuffer[loc]&(1<<7);
}
//void c_PT2314(bool aors)
//{
//	////int8 step = 1;	
//	////VolumeControl.Max=PT2314_MAXTONE;
//	////VolumeControl.Min=PT2314_MINTONE;
//	////switch(ControlState)
//	////{
//
//	//
//	////case CS_OK_DEYIN:VolumeControl.datESL=ESL_PT2314Bass;break;
//	////case CS_OK_GAOYIN:VolumeControl.datESL=ESL_PT2314Treble;break;
//	////}
//	////if(aors)
//	////	VolumeControl.Step=step;
//	////else
//	////	VolumeControl.Step=-step;
//	////CVolumeExe();
//	////PT2314UpdateAll();
//}
void SetTrackMode(u8 trackMode)
{
	ControlState = CS_TRACK_MODE;
	switch (trackMode)
	{
	default:
	case TM_5_1CH:
		{
			ClearMute(ESL_VolumeCC);
			ClearMute(ESL_VolumeSL);
			ClearMute(ESL_VolumeSR);
			trackMode = TM_5_1CH;
			break;
		}
	case TM_2_1CH:
		{
			SetMute(ESL_VolumeCC);
			SetMute(ESL_VolumeSL);
			SetMute(ESL_VolumeSR);
			break;
		}
	}
	EepromSaveChar(ESL_TrackMode,trackMode);
	UpdateAllVolume();
}
prog_char ok_delay[]=
{
	3,
	1,
	2,
	0,
};
#define ok_P_MASK (3)
void SetOKDelay(u8 okdelay)
{
	if(okdelay>=sizeof(ok_delay))
	{
		okdelay = 0;
	}
	PORTD = (PORTD&(~ok_P_MASK))|pgm_read_byte(ok_delay+okdelay);
	EepromSaveChar(ESL_OKDelay,okdelay);
}
void CVolume(bool aors)
{
	static u8 oldControlState;

	//if(AutoControl.Step!=0)
	//{
	//	MainVolume = AutoControl.Max;
	//	/*if(AutoControl.Step>0)
	//	{
	//		MainVolume = AutoControl.Max;
	//	}
	//	else
	//	{
	//		MainVolume = 0;
	//	}*/
	//	AutoControl.Step =0;
	//}
	VolumeControl.Min =0;
	VolumeControl.Max =M62429P_MAXVOLUME;
	VolumeControl.Step=1;
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:
		{
			if(Mute)
			{
				IRC_mute();
				return;
			}
			AutoControl.Max = MainVolume;
			VolumeControl.Max =MAINVOLUME_MAX;
			//VolumeControl.datESL = ESL_MainVolume;
			break;
		}
	case CS_VOLUME_CC:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_VolumeCC);
			if(CheckMute(ESL_VolumeCC))
				goto INVALID;
			//VolumeControl.datESL = ESL_VolumeCC;
			break;
		}
	case CS_VOLUME_SW:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446WorkMode|M62446_WM_3CH);
			ClearMute(ESL_VolumeSW);
			//VolumeControl.Min =-1;
			//VolumeControl.datESL = ESL_VolumeSW;
			break;
		}
	case CS_VOLUME_SL:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_VolumeSL);
			if(CheckMute(ESL_VolumeSL))
				goto INVALID;
			//VolumeControl.datESL = ESL_VolumeSL;
			break;
		}
	case CS_VOLUME_SR:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_VolumeSR);
			if(CheckMute(ESL_VolumeSR))
				goto INVALID;
			//VolumeControl.datESL = ESL_VolumeSR;
			break;
		}
	case CS_TREBLE:
	case CS_BASS:
		{
			VolumeControl.Min =PT2314_MINTONE;
			VolumeControl.Max =PT2314_MAXTONE;
			//VolumeControl.datESL = ESL_PT2314Bass;
			break;
		}
	case CS_VOLUME_ATT: 
		{
			VolumeControl.Min=PT2314_MINSPEAKERATTENUATORS;
			VolumeControl.Max=PT2314_MAXSPEAKERATTENUATORS;
			//VolumeControl.Step=1;
			//VolumeControl.datESL = ESL_PT2314SpeakerATT;
			break;
		}
	case CS_TRACK_MODE:
		{
			SetTrackMode(TrackMode+1);
			goto end;
		}
	default:
		{
			if(HaveMin)
			{
				VolumeControl.Min =PT2314_MINTONE;
				VolumeControl.Max =PT2314_MAXTONE;
				switch (ControlState)
				{
					//case CS_OK_GAOYIN:
					//	//VolumeControl.datESL = ESL_PT2314_2Treble;
					//	break;
					//case CS_OK_DEYIN:
					//	//VolumeControl.datESL = ESL_PT2314_2Bass;
					//	break;
				case CS_OK_DELAY:
					{
						SetOKDelay(OKDelay+1);
						goto end;
					}
				case CS_OK_HUNXIANG:
					{
						VolumeControl.Max=0;
						VolumeControl.Min=-15;
						//VolumeControl.datESL=ESL_PT2314_2SpeakerATT;
						break;
					}
				case CS_OK_VOLUME:
					{
						VolumeControl.Max =PT2314_2_MAXVOLUME;
						VolumeControl.Min =0;
						//VolumeControl.datESL=ESL_PT2314_2Volume;
						VolumeControl.Step=2;
						break;
					}
					/*default:
					{
					goto INVALID;
					}*/
				}
			}
			else
			{
				goto INVALID;
			}
		}


	}
	if(oldControlState!=ControlState)
	{
		oldControlState=ControlState;
		return;
	}
	
	VolumeControl.datESL=ControlState;
	CVolumeExe(aors);
end:
	UpdateAllVolume();
	return;
INVALID:
	noCallShowState= true;
	ShowStringALL_P(ssINVALID);
}
void UpdateAllVolume()
{
	if(MainVolume==0)
	{
		offjj();
		SetMute(ESL_VolumeSW);
	}
	else
	{
		ClearMute(ESL_VolumeSW);
		onjj();
	}
	M62429PUpdateAll();
	PT2314UpdateAll();
	PT2314_2UpdateAll();

}
void SetIntput(u8 intput)
{
	u8 pl = PT2314Loudness;
	EepromSaveChar(ESL_PT2314Loudness,(pl&(u8)(~PT2314_SWITCH_MASK))+intput);
	if(intput==INTPUT_AC3)
		on5_1();
	else
		off5_1();
	MainVolume = 1;
	UpdateAllVolume();
	AutoControl.Step = 4;
}
//bool Muteing;
//bool Talling;
//u8 oldVolume;
//void ToMute()
//{
//	Muteing = true;
//	Talling = false;
//	oldVolume = MainVolume;
//}
//void ToTall()
//{
//	Muteing = false;
//	Talling = true;
//}
//void PollingVolume()
//{
//	if(Muteing&&MainVolume)
//	{
//		ControlState = CS_VOLUME_MAIN;
//		CVolume(false);
//	}
//	else 
//	{
//		Muteing = false;
//		if(Talling&&MainVolume<oldVolume)
//		{
//			ControlState = CS_VOLUME_MAIN;
//			CVolume(true);
//		}
//		else
//		{
//			Talling = false;
//		}
//	}
//}
//
