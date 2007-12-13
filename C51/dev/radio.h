#ifndef _READIO_H_
#define _READIO_H_
#include "configure.h"


//#define	UseSW 

#define FMStationStorageCount 10
#define AMStationStorageCount 10

#ifdef SAVE_RADIO_SETTING
#define	FMStation ((uint8)EepromBuffer[ESL_FMStation])//当前FM电台
#define	AMStation ((uint8)EepromBuffer[ESL_AMStation])//当前AM电台
#else
extern uint8 FMStation;//当前FM电台
extern uint8 AMStation;//当前AM电台
#endif//SAVE_RADIO_SETTING

extern bool RadioAutoSeekFlag;	//启动电台自动扫描被置一，搜索结束时被自动置零


void	IniRadio(void);
void	SetRadioStation(uint8 st);	//选择电台
//extern void		NextRadioStation(char offset);	//SetRadioStation(当前电台+offset)
void	SaveRadioStation(void);				//保存频率到当前电台
void	RadioAutoSeekStart(void);			//开始自动搜台并储蓄可用电台
void	LC72131SeekStart(bool SeekWay);//开始搜台 SeekWay == true —> 向高频段搜
void	RadioSeekStop(void);				//停止搜台
void	DriveRadio(void);					//驱动Radio，需要重复调用以搜台

/*
*设置频率
*调用方法前请先设置“LC72131CurrentFrequency”。			   isrs.NLine
*seek:	为true 将进行中频计数，等待40ms后调用“IsLock“判断是否有台。
*/
//extern void		SeekFrequency(bool seek);		
//extern ulong GetCTR();
extern void 	SetRadioMode(uint8 mode);		//设置收音机波段
//UINTextern void 	LC72131Mute(bool of);		//禁音 of=false->请求中频输出加禁音
#endif//_READIO_H_

