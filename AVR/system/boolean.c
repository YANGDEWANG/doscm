#include <global.h>
#include "boolean.h"
bool SetBit(uint8* target,uint8 location,bool ver)//��targetλ�ÿ�ʼ��locationλ��ver
{
	uint8 BAdrr = location/8;
	uint8 newb = 1<<location%8;
	uint8 daOld = target[BAdrr];
	uint8 daNew = daOld;
	if(ver)
	{
		daNew |=newb;
	}
	else
	{
		daNew &=~newb;
	}
	if(daNew != daOld)
	{
		target[BAdrr] = daNew;
		return true;
	}
	return false;
}

bool TestBit(uint8 location,uint8* source)//����sourceλ�ÿ�ʼ��locationλ����ٲ�����
{
	uint8 da = source[location/8];
	uint8 b  = 1<<location%8;
	if(da&b)
	{
		return true;
	}
	return false;
}
