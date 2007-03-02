#include <global.h>
#include "boolean.h"

/*uint16 DivideChar(uint8 s)
{

asm volatile("mov r25,r24" "\n\t" 
"andi r24,0x07" "\n\t"
"lsr r25" "\n\t"
"lsr r25" "\n\t"
"lsr r25" "\n\t"
::);
return ((s/8)<<8)|s%8;
}*/
bool SetBit(uint8* target,uint8 location,bool ver)//将target位置开始的location位置ver
{
	uint8 BAdrr = location/8,bAdrr = location%8;
	//uint8 BAdrr =0,bAdrr;"mov bAdrr,location" "\n\t" "andi bAdrr,0x07" "\n\t"
	/*asm volatile("mov BAdrr,r24" "\n\t"
	"lsr BAdrr" "\n\t"
	"lsr BAdrr" "\n\t"
	"lsr BAdrr" "\n\t"
	:"=d"(BAdrr):);*/
	//__asm__ volatile("lsr BAdrr"::);
	//uint16 tmp = DivideChar(location);
	//uint8 BAdrr = (uint8)(tmp>>8),bAdrr = (uint8)tmp;
	uint8 daOld = target[BAdrr];
	uint8 daNew = daOld;
	if(ver)
	{
		daNew |=	_crol_(1,bAdrr);
	}
	else
	{
		daNew &=	~(_crol_(1,bAdrr));
	}
	if(daNew != daOld)
	{
		target[BAdrr] = daNew;
		return true;
	}
	return false;
}

bool TestBit(uint8 location,uint8* source)//测试source位置开始的location位的真假并返回
{
	uint8 da = source[location/8];
	if((uint8)(da&_crol_(1,location%8)))
	{
		return true;
	}
	return false;
}
