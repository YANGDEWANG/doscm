#include <global.h>
#include "AutoControl.h"
AutoControlData AutoControl;
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
		}
		else
		{
			if(AutoControl.Step>0)
			{
				*AutoControl.dat = AutoControl.Max;
			}
			else
			{
				*AutoControl.dat = AutoControl.Min;
			}
			AutoControl.Step = 0;
		}
		if(AutoControl.Callback)
		{
			AutoControl.Callback();
		}
	}
}
