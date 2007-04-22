/**************************************************************************************
//------------------ MMC/SD-Card Reading and Writing implementation -------------------
//FileName     : mmc.c
//Function     : Connect AVR to MMC/SD 
//Created by   : ZhengYanbo
//Created date : 15/08/2005
//Version      : V1.2
//Last Modified: 19/08/2005
//Filesystem   : Read or Write MMC without any filesystem

//CopyRight (c) 2005 ZhengYanbo
//Email: Datazyb_007@163.com
****************************************************************************************/
#include <global.h>
#include <util\delay.h>
#include "dev\mmc.h"
#include "protocol.h"
#include "dev\spi.h"
#include "string.h"
#define MMC_WAITE_RETRY_COUNT 200
#define MMC_ERROR_RETRY_COUN  5
#define MMC_ININIT_CLOCK_DELAY_US 50
#define BLOCK_SIZE_EX2 9//BLOCK_SIZE等于2的BLOCK_SIZE_EX2次方
#define SINGLE_BLOCK_WRITE_TOKEN 0xfe
static bool inInit; 
#ifdef MMC_SOFT_SPI
#ifdef MMC_SOFT_EXTPULLUP
#define clSCL() sbi(DDR_MMC,MMC_CLK)
#define setSCL() cbi(DDR_MMC,MMC_CLK)

#define clDO() sbi(DDR_MMC,MMC_DO)
#define setDO() cbi(DDR_MMC,MMC_DO)

#define getDI() (PIN_MMC&(1<<MMC_DI))
//set MMC_Chip_Select to high (MMC/SD-Card Invalid)
#define MMC_Disable() cbi(DDR_MMC,MMC_CS)
//set MMC_Chip_Select to low (MMC/SD-Card Active)
#define MMC_Enable() sbi(DDR_MMC,MMC_CS)

#else
#define clSCL() cbi(PORT_MMC,MMC_CLK)
#define setSCL() sbi(PORT_MMC,MMC_CLK)

#define clDO() cbi(PORT_MMC,MMC_DO)
#define setDO() sbi(PORT_MMC,MMC_DO)

#define getDI() (PIN_MMC&(1<<MMC_DI))
//set MMC_Chip_Select to high (MMC/SD-Card Invalid)
#define MMC_Disable() sbi(PORT_MMC,MMC_CS)
//set MMC_Chip_Select to low (MMC/SD-Card Active)
#define MMC_Enable() cbi(PORT_MMC,MMC_CS)

#endif
//inline
void Write_Byte_MMC(u8 cData)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		clSCL();
		if(cData&0x80)
		{
			setDO();
		}
		else
		{
			clDO();
		}
		cData = cData<<1;
		setSCL();
	}
	setDO();
}
//u8 inline Read_Byte_MMC()
u8 Read_Byte_MMC()
{
	uint8 i,ReadData=0;
	setDO();
	for(i=0;i<8;i++)
	{
		clSCL();
		ReadData = ReadData<<1;
		setSCL();
		if(getDI())
			ReadData|=1;
	}
	return ReadData;
}
//inline
u8 Write_Byte_MMC_InInit(u8 cData)
{
	uint8 i,ReadData=0;
	for(i=0;i<8;i++)
	{
		clSCL();
		if(cData&0x80)
		{
			setDO();
		}
		else
		{
			clDO();
		}
		cData = cData<<1;
		_delay_us(MMC_ININIT_CLOCK_DELAY_US);
		setSCL();
		ReadData = ReadData<<1;
		_delay_us(MMC_ININIT_CLOCK_DELAY_US);
		if(getDI())
			ReadData|=1;
	}
	setDO();
	return ReadData;
}
//u8 inline Read_Byte_MMC()
#define Read_Byte_MMC_InInit() Write_Byte_MMC_InInit(0xff)

//#define Write_Byte_MMC(dat)	MMC_MasterTransmit(dat)
//#define Read_Byte_MMC()		MMC_MasterTransmit(0xff)
#else//MMC_SOFT_SPI
#define Write_Byte_MMC(dat)	SPI_MasterTransmit(dat)
#define Read_Byte_MMC()		SPI_MasterTransmit(0xff)
#endif//MMC_SOFT_SPI

//--------------------------------------------------------------

//uint8   Init_Flag;    //Set it to 1 when Init is processing.
//---------------------------------------------------------------
//加载块地址到mmc_cmd
//void IniBlockAdd(uint32 add)
//{
//	b32 baddr;
//	baddr.b32_1 = add << BLOCK_SIZE_EX2; //addr = addr * 512 
//	mmc_cmd[1] = baddr.b8_3.b8_3;
//	mmc_cmd[2] = baddr.b8_3.b8_2;
//	mmc_cmd[3] = baddr.b8_3.b8_1;
//}

//****************************************************************************
// Port Init
#ifdef MMC_SOFT_EXTPULLUP
#define MMC_Port_Init();
#else
void static inline MMC_Port_Init(void)
//****************************************************************************
{
	
	sbi(DDR_MMC,MMC_CS); 
	sbi(PORT_MMC,MMC_CS);	//Config ports 
#ifdef MMC_SOFT_SPI
	cbi(DDR_MMC,MMC_DI);          //Set Pin MMC_DI as Input
	sbi(DDR_MMC,MMC_CLK);       //Set Pin MMC_Clock as Output
	sbi(DDR_MMC,MMC_DO);          //Set Pin MMC_DO as Output
#endif//MMC_SOFT_SPI
}
#endif
//****************************************************************************
//Routine for Init MMC/SD card(SPI-MODE)
bool MMCInit(void)
//****************************************************************************
{  
	uint8 i;
	//uint8 CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
	inInit = true;
	MMC_Port_Init();
	//_delay_us(250);  //Wait MMC/SD ready...
	for (i=0;i<0x0f;i++) 
	{
		Write_Byte_MMC_InInit(0xff); //send 74 clock at least!!!
	}
	//Send Command CMD0 to MMC/SD Card
	//retry 200 times to send CMD0 command 
	if(MMCWriteCommand(MMC_GO_IDLE_STATE,0,1)!=1)
	{
		MMC_Disable();  //set MMC_Chip_Select to high
		return(false);//CMD0 Error!
	}

	//Send Command CMD1 to MMC/SD-Card
	if(MMCWriteCommand(MMC_SEND_OP_COND,0,0)!=0)
	{
		MMC_Disable();  //set MMC_Chip_Select to high
		return(false);//CMD0 Error!
	}
	MMC_Disable();  //set MMC_Chip_Select to high 
	inInit = false;
	return(true); //All commands have been taken.
} 
#ifdef MMC_INFO_READ_U
VOLUME_INFO_TYPE mmc_info;

//***************************************************************************
//Routine for reading CID Registers from MMC/SD-Card (16Bytes) 
//Return 0 if no Error.
bool Read_CID_MMC(uint8 *Buffer)
{
	if(MMCWriteCommand(MMC_SEND_CID,0,0)!=0)
	{
		MMC_Disable();
		return(false); //block write Error!
	}
	return MMCReadBlock(Buffer,16); //read 16 bytes

}

//***************************************************************************
//Routine for reading CSD Registers from MMC/SD-Card (16Bytes)
//Return 0 if no Error.
bool Read_CSD_MMC(uint8 *Buffer)
//***************************************************************************
{	
	//Command for reading CSD Registers
	//uint8 CMD[] = {0x49,0x00,0x00,0x00,0x00,0xFF};
	if(MMCWriteCommand(MMC_SEND_CSD,0,0)!=0)
	{
		MMC_Disable();
		return(false); //block write Error!
	}
	return MMCReadBlock(Buffer,16); //read 16 bytes

}

//****************************************************************************
//returns the :
// 	size of the card in MB ( ret * 1024^2) == bytes
// 	sector count and multiplier MB are in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
// 	name of the media 
bool MMCGetVolumeInfo(void)
{   
	union mmc_csd re;
	// read the CSD register
	if(!Read_CSD_MMC(re.dat))
		return false;
	uint8 prw2 = re.mmc_csd_file.READ_BL_LEN
		+(re.mmc_csd_file.C_SIZE_MULT_H<<1|re.mmc_csd_file.C_SIZE_MULT_L)
		+2;
	mmc_info.Size = ((uint16)re.mmc_csd_file.C_SIZE_H<<10)
		+((uint16)re.mmc_csd_file.C_SIZE_M<<2)
		+re.mmc_csd_file.C_SIZE_L
		+1;
	mmc_info.Size <<= prw2;
	mmc_info.SizeMB=mmc_info.Size>>20;
	mmc_info.SectorCount = mmc_info.Size>>9;
	//mmc_info.Size = cSize;
	//mmc_info.Size*=(2<<);
	//ToStringWithU(stringbuff,mmc_info.SizeMB);
	//LCDShowStringAt(16,stringbuff);
	// get the name of the card
	if(!Read_CID_MMC(re.dat))
	{
		return false;
	}
	mmc_info.Name[0] = re.dat[3];
	mmc_info.Name[1] = re.dat[4];
	mmc_info.Name[2] = re.dat[5];
	mmc_info.Name[3] = re.dat[6];
	mmc_info.Name[4] = re.dat[7];
	mmc_info.Name[5] = 0x00; //end flag
	return true;
}

#endif//MMC_INFO_READ_U
//****************************************************************************
//Send a Command to MMC/SD-Card
//Return: the second uint8 of response register of MMC/SD-Card
uint8 MMCWriteCommand(uint8 cmd,uint32 arg,uint8 succeed)
//****************************************************************************
{
	uint8 tmp;
	uint8 retry=0,retrySendC = MMC_WAITE_RETRY_COUNT;
	do{
		if(inInit)
		{
			//send 8 Clock Impulse
			Write_Byte_MMC_InInit(0xFF);
			//set MMC_Chip_Select to low (MMC/SD-Card active)
			MMC_Enable();       //SD卡使能
			Write_Byte_MMC_InInit(cmd|0x40);   //送头命令
			Write_Byte_MMC_InInit(arg>>24);
			Write_Byte_MMC_InInit(arg>>16);     //send 6 Byte Command to MMC/SD-Card
			Write_Byte_MMC_InInit(arg>>8);
			Write_Byte_MMC_InInit(arg&0xff);
			Write_Byte_MMC_InInit(0x95);       //仅仅对RESET有效的CRC效验码
			//get 8 bit response
			//Read_Byte_MMC(); //read the first byte,ignore it. 
			do 
			{  //Only last 8 bit is used here.Read it out. 
				tmp = Read_Byte_MMC_InInit();
				retry++;
			}
			while((tmp==0xff)&&(retry<MMC_WAITE_RETRY_COUNT));  //当没有收到有效的命令的时候
		}
		else
		{
			//send 8 Clock Impulse
			Write_Byte_MMC(0xFF);
			//set MMC_Chip_Select to low (MMC/SD-Card active)
			MMC_Enable();       //SD卡使能
			Write_Byte_MMC(cmd|0x40);   //送头命令
			Write_Byte_MMC(arg>>24);
			Write_Byte_MMC(arg>>16);     //send 6 Byte Command to MMC/SD-Card
			Write_Byte_MMC(arg>>8);
			Write_Byte_MMC(arg&0xff);
			Write_Byte_MMC(0x95);       //仅仅对RESET有效的CRC效验码
			//get 8 bit response
			//Read_Byte_MMC(); //read the first byte,ignore it. 
			do 
			{  //Only last 8 bit is used here.Read it out. 
				tmp = Read_Byte_MMC();
				retry++;
			}
			while((tmp==0xff)&&(retry<MMC_WAITE_RETRY_COUNT));  //当没有收到有效的命令的时候
		}
	}
	while(tmp!=succeed&&(--retrySendC));
	return(tmp);
}
/****************************************************************************
写扇区（512字节）
addr  ：扇区编号
Buffer：数据储蓄区
****************************************************************************/
bool MMCWriteSector(uint32 addr,uint8 *Buffer)
{  
	uint8 tmp;
	uint16 i;
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	//Command 24 is a writing blocks command for MMC/SD-Card.
	//Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)
	if(MMCWriteCommand(MMC_WRITE_BLOCK,addr << 9,0)==0)
	{
		Read_Byte_MMC();
		Write_Byte_MMC(SINGLE_BLOCK_WRITE_TOKEN);	
		for (i=0;i<512;i++)
		{
			Write_Byte_MMC(*Buffer++); //send 512 bytes to Card
		}
		//CRC-Byte 
		Write_Byte_MMC(0xFF); //Dummy CRC
		Write_Byte_MMC(0xFF); //CRC Code
		tmp=Read_Byte_MMC();   // read response
		if((tmp & 0x1F)==0x05) // dat block accepted ?
		{
			//Wait till MMC/SD-Card is not busy
			while (Read_Byte_MMC()!=0xff){};
			MMC_Disable();
			return(true); //Error!
		}
		else
		{
			retuy--;
			if(retuy)goto re;
		}
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	MMC_Disable();
	return(false); //send commamd Error!
} 
/****************************************************************************
读取块中数据区
Buffer：数据储蓄区
size  ：数据块大小
offset：块内偏移
Bytes ：读取字节数
****************************************************************************/
bool MMCReadBlockAt(uint8 *Buffer,uint16 size,uint16 offset,uint16 Bytes)
{  
	uint16 i=0; 
	uint8 retry;
	//Read Start Byte form MMC/SD-Card (FEh/Start Byte)
	retry=0;

	while (Read_Byte_MMC() != MMC_STARTBLOCK_READ)
	{
		retry++;
		if(retry==MMC_WAITE_RETRY_COUNT) 
		{
			MMC_Disable();
			return(false); //block write Error!
		}
	}
	for (i=0;i<offset;i++)
	{
		Read_Byte_MMC();
	}
	for (;i<offset+Bytes;i++)
	{
		*Buffer++ = Read_Byte_MMC();
	}
	for (;i<size;i++)
	{
		Read_Byte_MMC();
	}

	//CRC-Byte
	Read_Byte_MMC();//CRC - Byte 
	Read_Byte_MMC();//CRC - Byte

	//set MMC_Chip_Select to high (MMC/SD-Card invalid)
	MMC_Disable();
	Read_Byte_MMC();//end read
	return(true);
}
/****************************************************************************
读取块
Buffer：数据储蓄区
Bytes ：块字节数
****************************************************************************/
bool MMCReadBlock(uint8 *Buffer,uint16 Bytes)
{  
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	if(MMCReadBlockAt(Buffer,Bytes,0,Bytes))
	{
		return true;
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	return false;
}
/****************************************************************************
读取扇区（512字节）
addr  ：扇区编号
Buffer：数据储蓄区
****************************************************************************/
uint8 MMCReadSector(uint32 addr,uint8 *Buffer)
{	
	//Command 16 is reading Blocks from MMC/SD-Card
	//uint8 CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
	//Send Command CMD to MMC/SD-Card
	//Retry 100 times to send command.
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	if((MMCWriteCommand(MMC_READ_SINGLE_BLOCK,addr<<9,0)==0)
		&&MMCReadBlockAt(Buffer,512,0,512))
	{
		MMC_Disable();
		return true;
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	MMC_Disable();
	return false; //block write Error!
}
/****************************************************************************
读取一个32bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u32 MMCReadu32(u32 sector,u16 offset)
{	
	u8 buf[4];
	b32 b;
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	//	b.b32_1=0;
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0)==0
		&&MMCReadBlockAt(buf,512,offset,4))
	{
		b.b8_4.b8_0=	buf[0];
		b.b8_4.b8_1=	buf[1];
		b.b8_4.b8_2=	buf[2];
		b.b8_4.b8_3=	buf[3];
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	MMC_Disable();
	return b.b32_1;
}
/****************************************************************************
读取一个16bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u16 MMCReadu16(u32 sector,u16 offset)
{	
	u8 buf[2];
	b16 b;
	//	b.b16_0=0;
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0)==0
		&&MMCReadBlockAt(buf,512,offset,2))
	{
		b.b8_2.b8_0=	buf[0];
		b.b8_2.b8_1=	buf[1];
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	MMC_Disable();
	return b.b16_1;
}
/****************************************************************************
读取一个8bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u8 MMCReadu8(u32 sector,u16 offset)
{	
	u8 buf[1]={0};
	u8 retuy = MMC_ERROR_RETRY_COUN;
re:
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0)
		&&MMCReadBlockAt(buf,512,offset,1))
	{
	}
	else
	{
		retuy--;
		if(retuy)goto re;
	}
	MMC_Disable();
	return buf[0];
}


