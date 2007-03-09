#ifndef _C_VOLUME_H_
#define _C_VOLUME_H_

enum CONTROL_STATE
{
	CS_VOLUME_MAIN,
	CS_VOLUME_SL,
	CS_VOLUME_CC,
	CS_VOLUME_SW,
	CS_VOLUME_SR,
	CS_INTPUT_SELECT,
	CS_VOLUME_ATT,
	//CS_VOLUME_L,
	//CS_VOLUME_R,

	CS_TRACK_MODE,
	CS_BASS,
	CS_TREBLE,

	CS_OK_VOLUME,
	CS_OK_DEYIN,
	CS_OK_GAOYIN,
	CS_OK_HUNXIANG,
	CS_OK_YANSHI,
	
};

typedef struct _ControlData
{
	_int8 	Max;
	_int8 	Min;
	_int8 	Step;
	_uint8 	datESL;


}ControlData;
extern ControlData data VolumeControl;
extern void CVolume(bool aors);//根据ControlState配置VolumeControl后执行
extern void CVolumeExe(bool aor);//根据VolumeControl执行
void SetIntput(u8 intput);
void SetMute(u8 loc);
extern u8 ControlState;

#endif// _C_VOLUME_H_

