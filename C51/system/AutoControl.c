#include <global.h>
#include "AutoControl.h"
AutoControlData data  AutoControl;
void PollingAutoControl()
{
	int8 dat;
	if(AutoControl.Step)
	{
		dat = *AutoControl.dat+AutoControl.Step;
		if(dat>=AutoControl.Min
			&&dat<=AutoControl.Max)
		{
			*AutoControl.dat = dat;
			if(AutoControl.Callback)
			{
				AutoControl.Callback();
			}
		}
		else
		{
			AutoControl.Step = 0;
		}
	}
}