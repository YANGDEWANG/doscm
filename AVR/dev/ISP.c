#include "ISP.h"
#include "SPI.h"
#include <util/delay.h>

#define _IN_ISP_C_

#define setISPResetPIN() ISPReset_POPT|= (1<<ISPResetPIN)
#define clISPResetPIN()  ISPReset_POPT&= ~(1<<ISPResetPIN)
bool ISPEnable()
{
	uint8 tmp;
	SPI_MasterInit(SPI_FOSC_64);
	SPI_MSB_FIRST;
	clISPResetPIN();
	setISPResetPIN();
	_delay_ms(1);
	clISPResetPIN();
	SPI_MasterTransmit(ISP_Enable_0);
	SPI_MasterTransmit(ISP_Enable_1);
	tmp = SPI_MasterTransmit(ISP_Enable_2);
	SPI_MasterTransmit(ISP_Enable_3);
	if(tmp == ISP_Enable_1)
		return true;
	else
		return false;
}
void ISPDisable()
{
	SPIRelease();
	setISPResetPIN();
}
void ISPErasure()
{
	SPI_MasterTransmit(ISP_Erasure_0);
	SPI_MasterTransmit(ISP_Erasure_1);
	SPI_MasterTransmit(ISP_Erasure_2);
	SPI_MasterTransmit(ISP_Erasure_3);
}
void ISPWriteLockBit(uint8 data)
{
	SPI_MasterTransmit(ISP_Write_LockBit_0);
	SPI_MasterTransmit(ISP_Write_LockBit_1);
	SPI_MasterTransmit(ISP_Write_LockBit_2);
	SPI_MasterTransmit(ISP_Write_LockBit_3|data);
}
void ISPWriteFlashPage(uint8 PageAdd)
{
	SPI_MasterTransmit(ISP_Write_Flash_Page_0);
	SPI_MasterTransmit(ISP_Write_Flash_Page_1&PageAdd>>3);
	SPI_MasterTransmit(PageAdd<<5);
	SPI_MasterTransmit(ISP_Write_Flash_Page_3);
}
void ISPWriteEEPROM(uint16_t add,uint8 data)
{
	SPI_MasterTransmit(ISP_Write_EEPROM_0);
	SPI_MasterTransmit(ISP_Write_EEPROM_1&add>>8);
	SPI_MasterTransmit((uint8)add);
	SPI_MasterTransmit(data);
}
void ISPWriteFuseBit(uint8 add,uint8 data)
{
	switch(add)
	{
	case 0:
		{
			SPI_MasterTransmit(ISP_Write_FuseBit_0);
			SPI_MasterTransmit(ISP_Write_FuseBit_1);
			break;
		}	
	case 1:
		{	
			SPI_MasterTransmit(ISP_Write_HFuseBit_0);
			SPI_MasterTransmit(ISP_Write_HFuseBit_1);
			break;
		}
	}
	SPI_MasterTransmit(ISP_Write_FuseBit_2);
	SPI_MasterTransmit(data);

}

void ISPLoadFlashPage(uint8 add,uint8 data)
{
	if(add%2)
	{
		SPI_MasterTransmit(ISP_Load_FlashH_Page_0);
	}
	else
	{		
		SPI_MasterTransmit(ISP_Load_Flash_Page_0);

	}
	SPI_MasterTransmit(ISP_Load_Flash_Page_1);
	SPI_MasterTransmit(ISP_Load_Flash_Page_2&(add/2));
	SPI_MasterTransmit(data);

}
uint8 ISPReadLockBit()
{
	SPI_MasterTransmit(ISP_Read_LockBit_0);
	SPI_MasterTransmit(ISP_Read_LockBit_1);
	SPI_MasterTransmit(ISP_Read_LockBit_2);
	return SPI_MasterTransmit(ISP_Read_LockBit_3);
}
uint8 ISPReadFlash(uint16_t add)
{
	uint16_t tmp = add/2;
	if(add%2)
	{
		SPI_MasterTransmit(ISP_Read_FlashH_0);
	}
	else
	{		
		SPI_MasterTransmit(ISP_Read_Flash_0);
	}
	SPI_MasterTransmit(ISP_Read_Flash_1&tmp>>8);
	SPI_MasterTransmit((uint8)tmp);
	return SPI_MasterTransmit(ISP_Read_Flash_3);
}
uint8 ISPReadEEPROM(uint16_t add)
{
	SPI_MasterTransmit(ISP_Read_EEPROM_0);
	SPI_MasterTransmit(ISP_Read_EEPROM_1&(add>>8));
	SPI_MasterTransmit((uint8)add);
	return SPI_MasterTransmit(ISP_Read_EEPROM_3);
}
uint8 ISPRead_FuseBit(uint8 add)
{
	switch(add)
	{
	case 0:
		{
			SPI_MasterTransmit(ISP_Read_FuseBit_0);
			SPI_MasterTransmit(ISP_Read_FuseBit_1);
			break;
		}	
	case 1:
		{	
			SPI_MasterTransmit(ISP_Read_HFuseBit_0);
			SPI_MasterTransmit(ISP_Read_HFuseBit_1);
			break;
		}
	}
	SPI_MasterTransmit(ISP_Read_FuseBit_2);
	return SPI_MasterTransmit(ISP_Read_FuseBit_3);
}
uint8 ISPReadAdjustByte(uint8 add)
{
	SPI_MasterTransmit(ISP_Read_AdjustByte_0);
	SPI_MasterTransmit(ISP_Read_AdjustByte_1);
	SPI_MasterTransmit(ISP_Read_AdjustByte_2&add);
	return SPI_MasterTransmit(ISP_Read_AdjustByte_3);
}
void ISPIni()
{
	ISPReset_DDR |= (1<<ISPResetPIN);
	ISPReset_POPT|= (1<<ISPResetPIN);
}
