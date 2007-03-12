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
	EepromSaveChar(loc,EepromBuffer[loc]&~(1<<7));
}
void SetMute(u8 loc)
{
	EepromSaveChar(loc,EepromBuffer[loc]|(1<<7));
}
bool CheckMute(u8 loc)
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
			ClearMute(ESL_M62429P2VolumeR);
			ClearMute(ESL_M62429P1VolumeL);
			ClearMute(ESL_M62429P1VolumeR);
			trackMode = TM_5_1CH;
			break;
		}
	case TM_2_1CH:
		{
			SetMute(ESL_M62429P2VolumeR);
			SetMute(ESL_M62429P1VolumeL);
			SetMute(ESL_M62429P1VolumeR);
			break;
		}
	}
	EepromSaveChar(ESL_TrackMode,trackMode);
}
prog_char ok_delay[]=
{
	3,
	1,
	2,
	0,
};
#define ok_P_MASK (3)
#define SetOKDelay(okd) (PORTD&=~ok_P_MASK,PORTD|=okd)
void CVolume(bool aors)
{

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
	//if(CS_INTPUT_SELECT==ControlState)
	//{
	//	switch(M62446OutputPort&M62446OUTPUTPORT_MASK)
	//	{
	//	case INTPUT_5_1:
	//		{
	//			M62446OutputportSet(INTPUT_CD);
	//			break;
	//		}
	//	case INTPUT_CD:
	//		{
	//			M62446OutputportSet(INTPUT_AUX);
	//			break;
	//		}
	//	case INTPUT_AUX:
	//		{
	//			M62446OutputportSet(INTPUT_5_1);
	//			break;
	//		}
	//	}
	//	//M62446Update(M62446TONE_OUTPUT);
	//	M62446ToSound();
	//	return;
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
			VolumeControl.datESL = ESL_MainVolume;
			break;
		}
	case CS_VOLUME_CC:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_M62429P2VolumeR);
			if(CheckMute(ESL_M62429P2VolumeR))
				goto INVALID;
			VolumeControl.datESL = ESL_M62429P2VolumeR;
			break;
		}
	case CS_VOLUME_SW:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446WorkMode|M62446_WM_3CH);
			ClearMute(ESL_M62429P2VolumeL);
			VolumeControl.datESL = ESL_M62429P2VolumeL;
			break;
		}
	case CS_VOLUME_SL:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_M62429P1VolumeL);
			if(CheckMute(ESL_M62429P1VolumeL))
				goto INVALID;
			VolumeControl.datESL = ESL_M62429P1VolumeL;
			break;
		}
	case CS_VOLUME_SR:
		{
			//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			//ClearMute(ESL_M62429P1VolumeR);
			if(CheckMute(ESL_M62429P1VolumeR))
				goto INVALID;
			VolumeControl.datESL = ESL_M62429P1VolumeR;
			break;
		}
	case CS_TREBLE:
		{
			VolumeControl.Min =PT2314_MINTONE;
			VolumeControl.Max =PT2314_MAXTONE;
			VolumeControl.datESL = ESL_PT2314Treble;
			break;
		}
	case CS_BASS:
		{
			VolumeControl.Min =PT2314_MINTONE;
			VolumeControl.Max =PT2314_MAXTONE;
			VolumeControl.datESL = ESL_PT2314Bass;
			break;
		}
	case CS_VOLUME_ATT: 
		{
			VolumeControl.Min=PT2314_MINSPEAKERATTENUATORS;
			VolumeControl.Max=PT2314_MAXSPEAKERATTENUATORS;
			//VolumeControl.Step=1;
			VolumeControl.datESL = ESL_PT2314SpeakerATT;
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
				switch (ControlState)
				{
				case CS_OK_GAOYIN:
					VolumeControl.datESL = ESL_PT2314_2Treble;
					break;
				case CS_OK_DEYIN:
					VolumeControl.datESL = ESL_PT2314_2Bass;
					break;
				case CS_OK_DELAY:
					{
						u8 okd = OKDelay+1;
						if(!(okd<sizeof(ok_delay)))
							okd=0;
						SetOKDelay(pgm_read_byte(ok_delay+okd));
						EepromSaveChar(ESL_OKDelay,okd);
						goto end;
					}
				case CS_OK_HUNXIANG:
					{
						VolumeControl.Max=PT2314_MAXSPEAKERATTENUATORS;
						VolumeControl.Min=PT2314_MINSPEAKERATTENUATORS;
						VolumeControl.datESL=ESL_PT2314_2SpeakerATT;
						break;
					}
				case CS_OK_VOLUME:
					{
						VolumeControl.datESL=ESL_PT2314_2Volume;
						break;
					}
				default:
					{
						goto INVALID;
					}
				}
			}
			else
			{
				goto INVALID;
			}
		}


	}

	/*if(ControlState == CS_TREBLE||ControlState == CS_BASS)
	{
	VolumeControl.Min =PT2314_MINTONE;
	VolumeControl.Max =PT2314_MAXTONE;
	if(ControlState == CS_TREBLE)
	{
	VolumeControl.datESL = ESL_PT2314Treble;
	}
	else
	{
	VolumeControl.datESL = ESL_PT2314Bass;
	}
	}*/

	CVolumeExe(aors);

	//停Autoc
	//	AutoControl.Step = 0;
	//取消禁音
	//M62446MuteUtility = M62446MIN_MUTEUTILITY;
	//M62446Mute = false;
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
		SetMute(ESL_M62429P2VolumeL);
	}
	else
	{
		ClearMute(ESL_M62429P2VolumeL);
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
	AutoControl.Step = 2;
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
