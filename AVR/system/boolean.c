#include <global.h>
#include <system/boolean.h>

#ifdef BOOLEAN_USE_RETURN
/*
*��targetλ�ÿ�ʼ��locationλ��ver,�����������target�ĸı䷵��true
*/
bool SetBit(uint8* target,uint8 location)//��targetλ�ÿ�ʼ��locationλ��ver
{
	target += location/8;
	uint8 newb = 1<<(location%8);
	uint8 daOld = *target;
	uint8 daNew = daOld;
	daNew |=newb;
	if(daNew != daOld)
	{
		*target = daNew;
		return true;
	}
	return false;
}
#else//BOOLEAN_USE_RETURN
void SetBit(uint8* target,uint8 location)//��targetλ�ÿ�ʼ��locationλ��ver
{
	target += location/8;
	uint8 newb = 1<<(location%8);
	uint8 daNew = *target;
	daNew |=newb;
	*target = daNew;
}
#endif//BOOLEAN_USE_RETURN


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
