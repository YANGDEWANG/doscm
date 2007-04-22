#include <global.h>
#include <system/boolean.h>

#ifdef BOOLEAN_USE_RETURN
/*
*将target位置开始的location位置ver,如果方法引起target的改变返回true
*/
bool SetBit(uint8* target,uint8 location)//将target位置开始的location位置ver
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
void SetBit(uint8* target,uint8 location)//将target位置开始的location位置ver
{
	target += location/8;
	uint8 newb = 1<<(location%8);
	uint8 daNew = *target;
	daNew |=newb;
	*target = daNew;
}
#endif//BOOLEAN_USE_RETURN


bool TestBit(uint8 location,uint8* source)//测试source位置开始的location位的真假并返回
{
	uint8 da = source[location/8];
	uint8 b  = 1<<location%8;
	if(da&b)
	{
		return true;
	}
	return false;
}
