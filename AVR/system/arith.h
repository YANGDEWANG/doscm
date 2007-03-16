#ifndef _ARITH_H_
#define _ARITH_H_
#include <configure.h>

#ifndef ArithType
#define ArithType uint8
#endif

/*****************************************
在限定的界限内（包括界限）旋转指针或移动
min：		下限
max：		上限
pointer：	当前指针
direction：	方向（true:向大，false:向小）
Eddy：		如果为true将进行旋转超作
return：	结果
*****************************************/
ArithType MoveOREddyPointer(ArithType min,ArithType max,ArithType pointer,bool direction,bool Eddy);

/*****************************************
在限定的界限内（包括界限）移动指针
min：		下限
max：		上限
pointer：	当前指针
direction：	方向（true:向大，false:向小）
return：	结果
*****************************************/
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction);

/*****************************************
在限定的界限内（包括界限）旋转指针
min：		下限
max：		上限
pointer：	当前指针
direction：	方向（true:向大，false:向小）
return：	结果
*****************************************/
uint8 EddyMovePointer(uint8 min,uint8 max,uint8 pointer,bool direction);
#endif//_ARITH_H_
