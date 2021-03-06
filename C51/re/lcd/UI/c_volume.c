#include <global.h>
#include "c.h"
enum CONTROL_STATE ControlState;
ControlData data VolumeControl;
void CVolumeExe()
{
	int8 dat = EepromBuffer[VolumeControl.datESL];
	//测试dat是否超出允许范围。是则置中间值
	if(dat>VolumeControl.Max
		&&dat<VolumeControl.Min)
	{
		dat=VolumeControl.Min+(VolumeControl.Max-VolumeControl.Min)/2;
		EepromSaveChar(VolumeControl.datESL,dat);
	}
	dat+=VolumeControl.Step;
	if(dat<=VolumeControl.Max
		&&dat>=VolumeControl.Min)
	{
		EepromSaveChar(VolumeControl.datESL,dat);
	}
}
void c_M62446(bool aors)
{	
	if(CS_INTPUT_SELECT==ControlState)
	{
		switch(M62446OutputPort&M62446OUTPUTPORT_MASK)
		{
		case INTPUT_5_1:
			{
				M62446OutputportSet(INTPUT_CD);
				break;
			}
		case INTPUT_CD:
			{
				M62446OutputportSet(INTPUT_AUX);
				break;
			}
		case INTPUT_AUX:
			{
				M62446OutputportSet(INTPUT_5_1);
				break;
			}
		}
		//M62446Update(M62446TONE_OUTPUT);
		M62446ToSound();
		return;
	}
	VolumeControl.Min =M62446MIN_ADJUST_VOLUME;
	VolumeControl.Max =M62446MAX_ADJUST_VOLUME;
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:
		{
			VolumeControl.datESL = ESL_M62446MainVolume;
			VolumeControl.Min =M62446MIN_MAIN_VOLUME;
			VolumeControl.Max =M62446MAX_MAIN_VOLUME;
			break;
		}
	case CS_VOLUME_CC:VolumeControl.datESL = ESL_M62446VolumeCC;break;
	case CS_VOLUME_SW:VolumeControl.datESL = ESL_M62446VolumeSW;break;
	case CS_VOLUME_SL:VolumeControl.datESL = ESL_M62446VolumeSL;break;
	case CS_VOLUME_SR:VolumeControl.datESL = ESL_M62446VolumeSR;break;
	case CS_VOLUME_L: VolumeControl.datESL = ESL_M62446VolumeL;break;
	case CS_VOLUME_R: VolumeControl.datESL = ESL_M62446VolumeR;break;
	case CS_TRACK_MODE:
		{
			switch (M62446WorkMode)
			{
			case M62446_WM_6CH:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_3CH);break;
			case M62446_WM_3CH:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_2CH);break;
			case M62446_WM_2CH:
			default:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			}
			goto end;
		}
	}

	if(ControlState == CS_TREBLE||ControlState == CS_BASS)
	{
		VolumeControl.Min =M62446MIN_TONE;
		VolumeControl.Max =M62446MAX_TONE;
		if(ControlState == CS_TREBLE)
		{
			VolumeControl.datESL = ESL_M62446Treble;
		}
		else
		{
			VolumeControl.datESL = ESL_M62446Bass;
		}
	}
	if(aors)
		VolumeControl.Step=1;
	else
		VolumeControl.Step=-1;
	CVolumeExe();

	//停Autoc
	AutoControl.Step = 0;
	//取消禁音
	M62446MuteUtility = M62446MIN_MUTEUTILITY;
	M62446Mute = false;
end:
	M62446UpdateAll();
}
void c_PT2314(bool aors)
{
	int8 step = 1;	
	VolumeControl.Max=PT2314_MAXTONE;
	VolumeControl.Min=PT2314_MINTONE;
	switch(ControlState)
	{

	case CS_OK_VOLUME:
		{
			VolumeControl.Max=PT2314_MAXVOLUME;
			VolumeControl.Min=PT2314_MINVOLUME;
			step = 2;
			VolumeControl.datESL=ESL_PT2314Volume;
			break;
		}
	case CS_OK_HUNXIANG:
		{
			VolumeControl.Max=PT2314_MAXSPEAKERATTENUATORS;
			VolumeControl.Min=PT2314_MINSPEAKERATTENUATORS;
			VolumeControl.datESL=ESL_PT2314SpeakerATT;
			step = 2;
			break;
		}
	case CS_OK_DEYIN:VolumeControl.datESL=ESL_PT2314Bass;break;
	case CS_OK_GAOYIN:VolumeControl.datESL=ESL_PT2314Treble;break;
	}
	if(aors)
		VolumeControl.Step=step;
	else
		VolumeControl.Step=-step;
	CVolumeExe();
	PT2314UpdateAll();
}
void CVolume(bool aors)
{
	//M62446
	if(ControlState<CS_TREBLE+1)
	{
		c_M62446(aors);
	}
	else if(ControlState<CS_OK_YANSHI+1)//PT2314
	{
		if(ok_mode)
		{
			c_PT2314(aors);
		}
		else
		{
			ControlState = 0;
		}
	}
}

