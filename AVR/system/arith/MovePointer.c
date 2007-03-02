#include <global.h>
#include "arith.h"

/*****************************************
在限定的界限内（包括界限）移动指针
min：		下限
max：		上限
pointer：	当前指针
direction：	方向（true:向大，false:向小）
return：	结果
*****************************************/
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)
{
	return MoveOREddyPointer(min,max,pointer,direction,false);
}
