#include <global.h>
#include "arith.h"
#ifdef ArithUchar
bool IncludePointerUchar(uint8 min,uint8 max,uint8 pointer)//min����,max����,pointer��ǰָ�룬���min<=pointer<=max����true
{
	if(pointer<=max&&pointer>=min)
		return true;
	else
		return false;
}
//���޶��Ľ����ڣ��������ޣ���תָ��
uint8 MovePointerUchar(uint8 min,uint8 max,uint8 pointer,bool direction,bool Eddy)//min����,max����,pointer��ǰָ��,direction����true:���false:��С��,Eddy:���Ϊtrue��������ת����
{
	bool tmp = IncludePointerUchar(min,max,pointer);
	if(direction)
	{
		if(pointer<max&&tmp)
		{
			return ++pointer;
		}
		else
		{
			if(Eddy)
				return min;
			else
				return max;
		}
	}
	else
	{
		if(pointer>min&&tmp)
		{
			return --pointer;
		}
		else
		{
			if(Eddy)
				return max;
			else
				return min;
		}
	}
}
#endif
#ifdef ArithUint
bool IncludePointer(uint min,uint max,uint pointer)//min����,max����,pointer��ǰָ�룬���min<=pointer<=max����true
{
	if(pointer<=max&&pointer>=min)
		return true;
	else
		return false;
}
//���޶��Ľ����ڣ��������ޣ���תָ��
uint MovePointer(uint min,uint max,uint pointer,bool direction,bool Eddy)//min����,max����,pointer��ǰָ��,direction����true:���false:��С��,Eddy:���Ϊtrue��������ת����
{
	bool tmp = IncludePointer(min,max,pointer);
	if(direction)
	{
		if(pointer<max&&tmp)
		{
			return ++pointer;
		}
		else
		{
			if(Eddy)
				return min;
			else
				return max;
		}
	}
	else
	{
		if(pointer>min&&tmp)
		{
			return --pointer;
		}
		else
		{
			if(Eddy)
				return max;
			else
				return min;
		}
	}
}
#else
/*bool IncludePointer(uint8 min,uint8 max,uint8 pointer)//min����,max����,pointer��ǰָ�룬���min<=pointer<=max����true
{
if(pointer<=max&&pointer>=min)
return true;
else
return false;
}*/
//���޶��Ľ����ڣ��������ޣ���תָ��
ArithType MoveOREddyPointer(ArithType min,ArithType max,ArithType pointer,bool direction,bool Eddy)//min����,max����,pointer��ǰָ��,direction����true:���false:��С��,Eddy:���Ϊtrue��������ת����
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
//���޶��Ľ����ڣ��������ޣ���תָ��
uint8 EddyMovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)//min����,max����,pointer��ǰָ��,direction����true:���false:��С��
{
	return MoveOREddyPointer(min,max,pointer,direction,true);
}
//���޶��Ľ����ڣ��������ޣ��ƶ�ָ��
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)//min����,max����,pointer��ǰָ��,direction����true:���false:��С��
{
	return MoveOREddyPointer(min,max,pointer,direction,false);
}

#endif
