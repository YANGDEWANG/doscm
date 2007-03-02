#include <global.h>
#include "radio.h"
#include "LC72131.h"
#include "Clock.h"
#ifndef SAVE_RADIO_SETTING
uint8  FMStation,AMStation;
#endif//SAVE_RADIO_SETTING

bool RadioAutoSeekFlag;	//启动电台自动扫描被置一，搜索结束时被自动置零

void RadioSeekStop()
{
	RadioAutoSeekFlag = false;
	LC72131SeekFlag = false;
	LC72131Mute(true);
//	ConMute(false);//禁音 of=true->禁音
}
void SaveRadioStation()//保存频率到当前电台
{
	
//	eeprom_busy_wait();
	if(LC72131RadioMode==RM_FM)
	{
		//_EEPUT(FMStation+FMStationStorageLocal,LC72131CurrentFrequency);
	}
	else
	{
	//	_EEPUT(AMStation+AMStationStorageLocal,LC72131CurrentFrequency);
	}
}
void SetRadioStation(uint8 st)//选择电台
{
uint8 sttmp;
#ifdef VolumeGather
	GatherVolume(Volume,0);
#endif
	RadioSeekStop();

	//ClearNumberAll();
//	ControlDisWin.Clear();
	if(LC72131RadioMode==RM_FM)
	{
		//sttmp = st%FMStationStorageCount;
		//EepromSaveChar(FMStationStorage,sttmp);
		//_EEGET(LC72131CurrentFrequency,sttmp+FMStationStorageLocal);
		//ControlDisWin.ShowNumberEX2(0,FMStation+1);
	}
	else
	{
		//sttmp = st%AMStationStorageCount;
		//EepromSaveChar(AMStationStorage,sttmp);
		//_EEGET(LC72131CurrentFrequency,sttmp+AMStationStorageLocal);
		//ControlDisWin.ShowNumberEX2(0,AMStation+1);
	} 
	//ShowRadio(false);
	//ShowControlDisWin();
	LC72131SeekFrequency(false);
}/*
void NextRadioStation(int8_t offset)
{
if(RadioMode==FM)
SetRadioStation(FMStation+offset);
else
SetRadioStation(AMStation+offset);
}*/

static void RadioAutoSeek()
{
	if(!LC72131SeekFlag)
	{
		LC72131SeekStart(true);
		SaveRadioStation();
		if(LC72131RadioMode==RM_FM)
		{
			if(FMStation<FMStationStorageCount-1)
			{
				FMStation++;
			}
			else
			{
				RadioAutoSeekFlag = false;
			}
		}
		else
		{
			if(AMStation<AMStationStorageCount-1)
			{
				AMStation++;
			}
			else
			{
				RadioAutoSeekFlag = false;
			}
		}

	}
}

void RadioAutoSeekStart() //开始自动搜台并储蓄可用电台
{
	//ConMute(true);//禁音 true->禁音
	if(LC72131RadioMode==RM_FM)
	{
		LC72131CurrentFrequency = MinFMFrequency;
		FMStation = 0;
	}
	else
	{
		AMStation = 0;
		LC72131CurrentFrequency = MinAMFrequency;
	}
	LC72131SeekStart(true);
	RadioAutoSeekFlag =	true;
}
void DriveRadio()
{
	if(LC72131SeekWait&&(LC72131SeekSatrtTime-SysClick)>CLICK_CYCLE_MS*4)//如果LC72131等待读中频并时间以过了40ms
	{
		LC72131Seek();
	}
	if(RadioAutoSeekFlag)
	{
		RadioAutoSeek();
	}
}
