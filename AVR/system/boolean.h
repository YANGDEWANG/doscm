#ifndef _BOOLEAN_H 
#define _BOOLEAN_H
#include "configure.h"


/*
*��targetλ�ÿ�ʼ��locationλ��ver,�����������target�ĸı䷵��true
*/
bool SetBit(uint8 *target,uint8 location,bool ver);
/*
*����sourceλ�ÿ�ʼ��locationλ����ٲ�����
*/
bool TestBit(uint8 location,uint8 *source);


#endif //_BOOLEAN_H		  
