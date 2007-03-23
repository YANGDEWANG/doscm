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
#include "mmc.h"
#include "protocol.h"
#include "spi.h"
#include "string.h"
#define BLOCK_SIZE_EX2 9//BLOCK_SIZE等于2的BLOCK_SIZE_EX2次方
#define SINGLE_BLOCK_WRITE_TOKEN 0xfe
#define cbi(p,b) (p&=~(1<<b))
#define sbi(p,b) (p|=(1<<b))
#define Write_Byte_MMC(dat)	SPI_MasterTransmit(dat)
#define Read_Byte_MMC()		SPI_MasterTransmit(0xff)


//--------------------------------------------------------------
//uint8 sectorBuffer[16]; //16 bytes buffer
uint16	readPos=0;
uint8	sectorPos=0;
uint8   LBA_Opened=0; //Set to 1 when a sector is opened.
VOLUME_INFO_TYPE mmc_info;
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
#if 0
//****************************************************************************
// Port Init
//void MMC_Port_Init(void)
//****************************************************************************
{
	//Config ports 
	cbi(MMC_Direction_REG,SPI_DI);          //Set Pin MMC_DI as Input
	sbi(MMC_Direction_REG,SPI_Clock);       //Set Pin MMC_Clock as Output
	sbi(MMC_Direction_REG,SPI_DO);          //Set Pin MMC_DO as Output
	sbi(MMC_Direction_REG,MMC_Chip_Select); //Set Pin MMC_Chip_Select as Output
	//busy led port init
	sbi(MMC_Direction_REG,SPI_BUSY);        //Set spi busy led port output
	sbi(MMC_Write,SPI_BUSY);                      //busy led off

	sbi(MMC_Write,MMC_Chip_Select);                        //Set MMC_Chip_Select to High,MMC/SD Invalid.
}
#endif
//****************************************************************************
//Routine for Init MMC/SD card(SPI-MODE)
bool MMCInit(void)
//****************************************************************************
{  
	uint8 i;
	//uint8 CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
	sbi(DDR_MMC,MMC_CS); //Set Pin MMC_Chip_Select as Output
	sbi(PORT_MMC,MMC_CS);                        //Set MMC_Chip_Select to High,MMC/SD Invalid.
	_delay_us(250);  //Wait MMC/SD ready...
	for (i=0;i<0x0f;i++) 
	{
		Write_Byte_MMC(0xff); //send 74 clock at least!!!
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
	return(true); //All commands have been taken.
} 

//****************************************************************************
//returns the :
// 	size of the card in MB ( ret * 1024^2) == bytes
// 	sector count and multiplier MB are in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
// 	name of the media 
//#include <lcd.h>
//char stringbuff[16];
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

//****************************************************************************
//Send a Command to MMC/SD-Card
//Return: the second uint8 of response register of MMC/SD-Card
uint8 MMCWriteCommand(uint8 cmd,uint32 arg,uint8 succeed)
//****************************************************************************
{
	uint8 tmp;
	uint8 retry=0,retrySendC = 100;
	do{
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
		while((tmp==0xff)&&(retry<100));  //当没有收到有效的命令的时候
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
	//Command 24 is a writing blocks command for MMC/SD-Card.
	//Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)
	if(MMCWriteCommand(MMC_WRITE_BLOCK,addr << 9,0)!=0)
	{
		MMC_Disable();
		return(false); //send commamd Error!
	}
	////Before writing,send 100 clock to MMC/SD-Card
	//for (i=0;i<100;i++)
	//{
	//	Read_Byte_MMC();
	//}
	//Before writing,send 8 clock to MMC/SD-Card
	Read_Byte_MMC();
	//Send Start Byte to MMC/SD-Card//
	Write_Byte_MMC(SINGLE_BLOCK_WRITE_TOKEN);	

	//Now send real dat Bolck (512Bytes) to MMC/SD-Card
	for (i=0;i<512;i++)
	{
		Write_Byte_MMC(*Buffer++); //send 512 bytes to Card
	}

	//CRC-Byte 
	Write_Byte_MMC(0xFF); //Dummy CRC
	Write_Byte_MMC(0xFF); //CRC Code


	tmp=Read_Byte_MMC();   // read response
	if((tmp & 0x1F)!=0x05) // dat block accepted ?
	{
		MMC_Disable();
		return(false); //Error!
	}
	//Wait till MMC/SD-Card is not busy
	while (Read_Byte_MMC()!=0xff){};

	//set MMC_Chip_Select to high (MMC/SD-Card Invalid)
	MMC_Disable();
	return(0);
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
		if(retry==100) 
		{
			MMC_Disable();
			return(false); //block write Error!
		}
	}
	for (i=0;i<offset;i++)
	{
		Read_Byte_MMC();
	}
	for (;i<Bytes;i++)
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
	return MMCReadBlockAt(Buffer,Bytes,0,Bytes);
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
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,addr<<9,0)!=0)
	{
		MMC_Disable();
		return false; //block write Error!
	}
	return MMCReadBlockAt(Buffer,512,0,512);
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
//	b.b32_1=0;
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0)==0
		&&MMCReadBlockAt(buf,512,offset,4))
	{
		b.b8_4.b8_0=	buf[0];
		b.b8_4.b8_1=	buf[1];
		b.b8_4.b8_2=	buf[2];
		b.b8_4.b8_3=	buf[3];
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
	if(MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0)==0
		&&MMCReadBlockAt(buf,512,offset,2))
	{
		b.b8_2.b8_0=	buf[0];
		b.b8_2.b8_1=	buf[1];
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
	MMCWriteCommand(MMC_READ_SINGLE_BLOCK,sector<<9,0);
	MMCReadBlockAt(buf,512,offset,1);
	MMC_Disable();
	return buf[0];
}
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

//***************************************************************************
//Return: [0]-success or something error!
//uint8 MMC_Start_Read_Sector(uint32 sector)
////***************************************************************************
//{  
//	uint8 retry;
//	//Command 16 is reading Blocks from MMC/SD-Card
//	//uint8 CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
//	uint8 temp;
//
//	asm("cli"); //clear all interrupt.
//	//Address conversation(logic block address-->uint8 address)  
//	//sector = sector << 9; //sector = sector * 512
//
//	//CMD[1] = ((sector & 0xFF000000) >>24 );
//	//CMD[2] = ((sector & 0x00FF0000) >>16 );
//	//CMD[3] = ((sector & 0x0000FF00) >>8 );
//	mmc_cmd[0]=0x51;
//	mmc_cmd[4]=0;
//	mmc_cmd[5]=0xFF;
//	IniBlockAdd(sector);
//	//Send Command CMD to MMC/SD-Card
//	retry=0;
//	do
//	{  //Retry 100 times to send command.
//		temp=MMCWriteCommand();
//		retry++;
//		if(retry==10) 
//		{
//			return(READ_BLOCK_ERROR); //block write Error!
//		}
//	}
//	while(temp!=0); 
//
//	//Read Start Byte form MMC/SD-Card (FEh/Start Byte)
//	//Now dat is ready,you can read it out.
//	while (Read_Byte_MMC() != 0xfe){};
//	return 0; //Open a sector successfully!
//}

////***************************************************************************
//void MMC_get_data(uint16 Bytes,uint8 *buffer) 
////***************************************************************************
//{
//	uint16 j;
//
//	asm("cli"); //clear all interrupt.
//	for (j=0;((j<Bytes) && (readPos<512));j++)
//	{	
//		*buffer++ = Read_Byte_MMC();
//		readPos++; //read a uint8,increase read position
//	}
//	if (readPos==512)  
//	{  //CRC-Bytes
//		Read_Byte_MMC();//CRC - Byte 
//		Read_Byte_MMC();//CRC - Byte
//		readPos=0;      //reset sector read offset 
//		sectorPos++;    //Need to read next sector
//		LBA_Opened=0;   //Set to 1 when a sector is opened.
//		//set MMC_Chip_Select to high (MMC/SD-Card invalid)
//		MMC_Disable();  //MMC disable
//	}
//}
//
////***************************************************************************
//void MMC_get_data_LBA(uint32 lba, uint16 Bytes,uint8 *buffer)
////***************************************************************************
//{ //get dat from lba address of MMC/SD-Card
//	//If a new sector has to be read then move head
//	//if (readPos==0) MMC_Start_Read_Sector(lba); 
//	//MMC_get_data(Bytes,buffer);
//}
//
////***************************************************************************
//void MMC_GotoSectorOffset(uint32 LBA,uint16 offset)
////***************************************************************************
//{  
//	//Find the offset in the sector
//	uint8 temp[1];
//	MMC_LBA_Close(); //close MMC when read a new sector(readPos=0)
//	while (readPos<offset) MMC_get_data_LBA(LBA,1,temp); //go to offset  
//}

//***************************************************************************
//void MMC_LBA_Close()
////***************************************************************************
//{  
//	uint8 temp[1];
//	while((readPos!=0x00)|(LBA_Opened==1))
//	{ //read MMC till readPos==0x00
//		MMC_get_data(1, temp); //dummy read,temp is a valid dat.
//	}  
//}

//---------------------------------------------------------------------------- 


