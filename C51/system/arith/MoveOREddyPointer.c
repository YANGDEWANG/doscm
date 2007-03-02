#include <global.h>
#include "arith.h"

/*****************************************
在限定的界限内（包括界限）旋转指针或移动
min：		下限
max：		上限
pointer：	当前指针
direction：	方向（true:向大，false:向小）
Eddy：		如果为true将进行旋转超作
return：	结果
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
