#include "ISPEX.h"
#include <util/delay.h>

#define _IN_ISPEX_C_
bool ISP_Erasure()
{
	if(!ISPEnable())
	{
		return false;
	}
	ISPErasure();
	_delay_ms(TWD_ERASE);
	ISPDisable();
	return true;

}
bool ISP_WriteFlash(prog_char* data,uint16_t size)
{
	if(size>Flash_Size||(!ISPEnable()))
	{
		return false;
	}
	/////////////WriteFlash//////////////
	uint16_t writed = 0,i=0,k=0;
	uint8 td;
	while(writed<size)
	{
		for(k=0;k<Flash_Page_Size;k++)
		{
			td = pgm_read_byte_near(data+writed++);
			//if(td!=0xff)
			//{
				ISPLoadFlashPage(k,td);
			//}
			if(!(writed<size))
				break;
		}
		ISPWriteFlashPage(i++);
		_delay_ms(TWD_FLASH);
	}
	ISPDisable();
	return true;
}
bool ISP_WriteEeporm(prog_char* data,uint16_t size)
{
	if(size>Eeporm_Size||(!ISPEnable()))
	{
		return false;
	}
		uint16_t i;
	uint8	td;
	for(i=0;i<size;i++)
	{
		td = pgm_read_byte_near(data+i);
		//if(data == 0xFF)//ÒýÆð´íÎó
		//continue;
		ISPWriteEEPROM(i,td);
		/*//SatrtWait(tuichu);
		while((ISPReadEEPROM(i)!=data))
		{
		if(Wait10MS(tuichu))
		goto tuc;
		}*/
		_delay_ms(TWD_EEPROM);
	}
	ISPDisable();
	return true;
}
bool ISP_VerifyFlash(prog_char* data,uint16_t size)
{
	if(size>Flash_Size||(!ISPEnable()))
	{
		return false;
	}
	uint16_t i;
	for(i=0;i<size;i++)
	{
		uint8 b1 = pgm_read_byte_near(data+i);
		uint8 b2 = ISPReadFlash(i);
		if(b1!=b2)
		{
			return false;
		}
	}
	ISPDisable();
	return true;
}
bool ISP_VerifyEeeprom(prog_char* data,uint16_t size)
{
	if(size>Eeporm_Size||(!ISPEnable()))
	{
		return false;
	}
	uint16_t i;
	for(i=0;i<size;i++)
	{
		if(pgm_read_byte_near(data+i)!=ISPReadEEPROM(i))
		{
			return false;
		}
	}
	ISPDisable();
	return true;
}bool ISP_WriteLockBit(uint8 data)
{
	if(!ISPEnable())
	{
		return false;
	}
	ISPWriteLockBit(data);
	_delay_ms(TWD_FUSE);
	if(ISPReadLockBit()!=data)
	{
		return false;
	}
	ISPDisable();
	return true;
}
bool ISP_WriteFuse(uint8 LFuseByte,uint8 HFuseByte)
{
	if(!ISPEnable())
	{
		return false;
	}
	ISPWriteFuseBit(0,LFuseByte);
	_delay_ms(TWD_FUSE);
	ISPWriteFuseBit(1,HFuseByte);
	_delay_ms(TWD_FUSE);
	uint8 FuseBit ;
	FuseBit = ISPRead_FuseBit(0);
	if((FuseBit)!=(LFuseByte))
	{
		return false;
	}
	FuseBit = ISPRead_FuseBit(1);
	if((FuseBit)!=(HFuseByte))
	{
		return false;
	}
	ISPDisable();
	return true;
}
