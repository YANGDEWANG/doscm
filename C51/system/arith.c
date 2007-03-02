#include <global.h>
#include "arith.h"
#ifdef ArithUchar
bool IncludePointerUchar(uint8 min,uint8 max,uint8 pointer)//min下限,max上限,pointer当前指针，如果min<=pointer<=max返回true
{
	if(pointer<=max&&pointer>=min)
		return true;
	else
		return false;
}
//在限定的界限内（包括界限）旋转指针
uint8 MovePointerUchar(uint8 min,uint8 max,uint8 pointer,bool direction,bool Eddy)//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
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
bool IncludePointer(uint min,uint max,uint pointer)//min下限,max上限,pointer当前指针，如果min<=pointer<=max返回true
{
	if(pointer<=max&&pointer>=min)
		return true;
	else
		return false;
}
//在限定的界限内（包括界限）旋转指针
uint MovePointer(uint min,uint max,uint pointer,bool direction,bool Eddy)//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
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
/*bool IncludePointer(uint8 min,uint8 max,uint8 pointer)//min下限,max上限,pointer当前指针，如果min<=pointer<=max返回true
{
if(pointer<=max&&pointer>=min)
return true;
else
return false;
}*/
//在限定的界限内（包括界限）旋转指针
ArithType MoveOREddyPointer(ArithType min,ArithType max,ArithType pointer,bool direction,bool Eddy)//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）,Eddy:如果为true将进行旋转超作
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
//在限定的界限内（包括界限）旋转指针
uint8 EddyMovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）
{
	return MoveOREddyPointer(min,max,pointer,direction,true);
}
//在限定的界限内（包括界限）移动指针
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)//min下限,max上限,pointer当前指针,direction方向（true:向大，false:向小）
{
	return MoveOREddyPointer(min,max,pointer,direction,false);
}

#endif
