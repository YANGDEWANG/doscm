#ifndef _AUTOCONTROL_H_
#define _AUTOCONTROL_H_
#include <configure.h>
typedef void (*AutoControlDataCallback)();
typedef struct _AutoControlData
{
	_int8 	Max;
	_int8 	Min;
	_int8 	Step;
	_int8 	*dat;
	AutoControlDataCallback Callback;

}AutoControlData;
extern AutoControlData AutoControl;
extern void PollingAutoControl();

#endif// _AUTOCONTROL_H_

