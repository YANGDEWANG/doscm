#include <global.h>
#include "c.h"
u8 ControlState;
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
	switch (ControlState)
	{
	case CS_VOLUME_MAIN:
		{
			VolumeControl.datESL = ESL_PT2314Volume;
			//VolumeControl.Min =M62446MIN_MAIN_VOLUME;
			VolumeControl.Max =PT2314_MAXVOLUME;
			break;
		}
	case CS_VOLUME_CC:
		//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
		VolumeControl.datESL = ESL_M62429P2VolumeR;
		break;
	case CS_VOLUME_SW:
		//EepromSaveChar(ESL_M62446WorkMode,M62446WorkMode|M62446_WM_3CH);
		VolumeControl.datESL = ESL_M62429P2VolumeL;
		break;
	case CS_VOLUME_SL:
		//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
		VolumeControl.datESL = ESL_M62429P1VolumeL;
		break;
	case CS_VOLUME_SR:
		//EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
		VolumeControl.datESL = ESL_M62429P1VolumeR;
		break;
	case CS_VOLUME_L: VolumeControl.datESL = ESL_PT2314Volume;break;
	case CS_VOLUME_R: VolumeControl.datESL = ESL_PT2314Volume;break;
	/*case CS_TRACK_MODE:
		{
			switch (M62446WorkMode)
			{
			case M62446_WM_6CH:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_3CH);break;
			case M62446_WM_3CH:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_2CH);break;
			case M62446_WM_2CH:
			default:EepromSaveChar(ESL_M62446WorkMode,M62446_WM_6CH);
			}
			goto end;
		}*/
	}

	if(ControlState == CS_TREBLE||ControlState == CS_BASS)
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
	}
	if(aors)
		VolumeControl.Step=1;
	else
		VolumeControl.Step=-1;
	CVolumeExe();

	//停Autoc
	AutoControl.Step = 0;
	//取消禁音
	//M62446MuteUtility = M62446MIN_MUTEUTILITY;
	//M62446Mute = false;
end:
	PT2314UpdateAll();
	PT2314_2UpdateAll();
	M62429PUpdateAll();

}
void c_PT2314(bool aors)
{
	//int8 step = 1;	
	//VolumeControl.Max=PT2314_MAXTONE;
	//VolumeControl.Min=PT2314_MINTONE;
	//switch(ControlState)
	//{

	//case CS_OK_VOLUME:
	//	{
	//		VolumeControl.Max=PT2314_MAXVOLUME;
	//		VolumeControl.Min=PT2314_MINVOLUME;
	//		step = 2;
	//		VolumeControl.datESL=ESL_PT2314Volume;
	//		break;
	//	}
	//case CS_OK_HUNXIANG:
	//	{
	//		VolumeControl.Max=PT2314_MAXSPEAKERATTENUATORS;
	//		VolumeControl.Min=PT2314_MINSPEAKERATTENUATORS;
	//		VolumeControl.datESL=ESL_PT2314SpeakerATT;
	//		step = 2;
	//		break;
	//	}
	//case CS_OK_DEYIN:VolumeControl.datESL=ESL_PT2314Bass;break;
	//case CS_OK_GAOYIN:VolumeControl.datESL=ESL_PT2314Treble;break;
	//}
	//if(aors)
	//	VolumeControl.Step=step;
	//else
	//	VolumeControl.Step=-step;
	//CVolumeExe();
	//PT2314UpdateAll();
}
void CVolume(bool aors)
{
	////M62446
	if(ControlState<CS_TREBLE+1)
	{
		c_M62446(aors);
	}
	else if(ControlState<CS_OK_YANSHI+1)//PT2314
	{
		//if(ok_mode)
		//{
		//	//c_PT2314(aors);
		//}
		//else
		//{
		//	ControlState = 0;
		//}
	}
}

