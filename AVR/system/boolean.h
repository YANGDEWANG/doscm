#ifndef _BOOLEAN_H 
#define _BOOLEAN_H
#include "configure.h"

//#define BOOLEAN_USE_RETURN

#ifdef BOOLEAN_USE_RETURN
/*
*��targetλ�ÿ�ʼ��locationλ��ver,�����������target�ĸı䷵��true
*/
bool SetBit(uint8 *target,uint8 location);
#else//BOOLEAN_USE_RETURN
void SetBit(uint8 *target,uint8 location);
#endif//BOOLEAN_USE_RETURN
/*
*����sourceλ�ÿ�ʼ��locationλ����ٲ�����
*/
bool TestBit(uint8 location,uint8 *source);


#endif //_BOOLEAN_H		  
