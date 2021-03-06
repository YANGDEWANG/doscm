#ifndef _BOOLEAN_H 
#define _BOOLEAN_H
#include "configure.h"

//#define BOOLEAN_USE_RETURN

#ifdef BOOLEAN_USE_RETURN
/*
*将target位置开始的location位置ver,如果方法引起target的改变返回true
*/
bool SetBit(uint8 *target,uint8 location);
#else//BOOLEAN_USE_RETURN
void SetBit(uint8 *target,uint8 location);
#endif//BOOLEAN_USE_RETURN
/*
*测试source位置开始的location位的真假并返回
*/
bool TestBit(uint8 location,uint8 *source);


#endif //_BOOLEAN_H		  
