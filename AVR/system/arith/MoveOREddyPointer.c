#include <global.h>
#include "arith.h"

/*****************************************
���޶��Ľ����ڣ��������ޣ���תָ����ƶ�
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
Eddy��		���Ϊtrue��������ת����
return��	���
*****************************************/
ArithType MoveOREddyPointer(ArithType min,ArithType max,ArithType pointer,bool direction,bool Eddy)
{
	if(pointer<=min&&!direction)
	{
		if(Eddy)
		{
			return max;
		}
		else
		{
			return min;
		}
	}
	if(pointer>=max&&direction)
	{
		if(Eddy)
		{
			return min;
		}
		else
		{
			return max;
		}
	}
	if(direction)
	{
		return ++pointer;
	}
	else
	{
		return --pointer;
	}
}
