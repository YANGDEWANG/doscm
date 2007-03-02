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
#include "global.h"
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
uint8 sectorBuffer[16]; //16 bytes buffer
uint16	readPos=0;
uint8	sectorPos=0;
uint8   LBA_Opened=0; //Set to 1 when a sector is opened.
uint8 mmc_cmd[6];
VOLUME_INFO_TYPE mmc_info;
//uint8   Init_Flag;    //Set it to 1 when Init is processing.
//---------------------------------------------------------------
//加载块地址到mmc_cmd
void IniBlockAdd(uint32 add)
{
	b32 baddr;
	baddr.b32_1 = add << BLOCK_SIZE_EX2; //addr = addr * 512 
	mmc_cmd[1] = baddr.b8_4.b8_4;
	mmc_cmd[2] = baddr.b8_4.b8_3;
	mmc_cmd[3] = baddr.b8_4.b8_2;
}
void inline clr_mmc_cmd()
{
	memset(mmc_cmd,0,sizeof(mmc_cmd));
}
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
uint8 MMCInit(void)
//****************************************************************************
{  
	uint8 retry,temp;
	uint8 i;
	//uint8 CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
	sbi(DDR_MMC,MMC_CS); //Set Pin MMC_Chip_Select as Output
	sbi(PORT_MMC,MMC_CS);                        //Set MMC_Chip_Select to High,MMC/SD Invalid.

	//MMC_Port_Init(); //Init SPI port  

	//for(i=0;i<200;i++) //Wait MMC/SD ready...
	//{
	//	asm("nop");
	//}

	//Init_Flag=1; //Set the init flag

	for (i=0;i<0x0f;i++) 
	{
		Write_Byte_MMC(0xff); //send 74 clock at least!!!
	}

	//Send Command CMD0 to MMC/SD Card
	clr_mmc_cmd();
	mmc_cmd[0]=0x40;
	mmc_cmd[5]=0x95;
	retry=0;
	do
	{ //retry 200 times to send CMD0 command 
		temp=Write_Command_MMC();
		retry++;
		if(retry==10) 
		{ //time out
			return(INIT_CMD0_ERROR);//CMD0 Error!
		}
	} 
	while(temp!=1);

	//Send Command CMD1 to MMC/SD-Card
	mmc_cmd[0] = 0x41; //Command 1
	mmc_cmd[5] = 0xFF;
	retry=0;
	do
	{ //retry 100 times to send CMD1 command 
		temp=Write_Command_MMC();
		retry++;
		if(retry==10) 
		{ //time out
			return(INIT_CMD1_ERROR);//CMD1 Error!
		}
	} 
	while(temp!=0);

	//Init_Flag=0; //Init is completed,clear the flag 

	MMC_Disable();  //set MMC_Chip_Select to high 
	return(0); //All commands have been taken.
} 

//****************************************************************************
//returns the :
// 	size of the card in MB ( ret * 1024^2) == bytes
// 	sector count and multiplier MB are in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
// 	name of the media 
#include <lcd.h>
char stringbuff[16];
uint8 MMCGetVolumeInfo(void)
//****************************************************************************
{   
	//uint8 i;
	uint8 ret;
	union mmc_csd re;
	// read the CSD register
	ret = Read_CSD_MMC(re.dat);
	if(ret!=0)return ret;
	mmc_info.BlockLengthPower2=re.mmc_csd_file.READ_BL_LEN;
	mmc_info.Size = ((uint32)(
		((uint16)re.mmc_csd_file.C_SIZE_H<<10)
		+((uint16)re.mmc_csd_file.C_SIZE_M<2)
		+re.mmc_csd_file.C_SIZE_L))
		<<
		(mmc_info.BlockLengthPower2
		+(re.mmc_csd_file.C_SIZE_MULT_H<<1|re.mmc_csd_file.C_SIZE_MULT_L)
		+2);
	mmc_info.size_MB=mmc_info.Size>>20;
	ToStringWithU(stringbuff,mmc_info.size_MB);
	LCDShowStringAt(16,stringbuff);
	//// get the C_SIZE value. bits [73:62] of dat
	//// [73:72] == sectorBuffer[6] && 0x03
	//// [71:64] == sectorBuffer[7]
	//// [63:62] == sectorBuffer[8] && 0xc0
	//mmc_info.sector_count = sectorBuffer[6] & 0x03;
	//mmc_info.sector_count <<= 8;
	//mmc_info.sector_count += sectorBuffer[7];
	//mmc_info.sector_count <<= 2;
	//mmc_info.sector_count += (sectorBuffer[8] & 0xc0) >> 6;

	//// get the val for C_SIZE_MULT. bits [49:47] of sectorBuffer
	//// [49:48] == sectorBuffer[5] && 0x03
	//// [47]    == sectorBuffer[4] && 0x80
	//mmc_info.sector_multiply = sectorBuffer[9] & 0x03;
	//mmc_info.sector_multiply <<= 1;
	//mmc_info.sector_multiply += (sectorBuffer[10] & 0x80) >> 7;

	//// work out the MBs
	//// mega bytes in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
	//mmc_info.size_MB = mmc_info.sector_count >> (9-mmc_info.sector_multiply);
	//// get the name of the card
	ret = Read_CID_MMC(sectorBuffer);
	if(ret!=0)return ret;
	mmc_info.name[0] = sectorBuffer[3];
	mmc_info.name[1] = sectorBuffer[4];
	mmc_info.name[2] = sectorBuffer[5];
	mmc_info.name[3] = sectorBuffer[6];
	mmc_info.name[4] = sectorBuffer[7];
	mmc_info.name[5] = 0x00; //end flag
	return 0;
	/*----------------------------------------------------------
	LCDclrscr();
	//Print Product name on lcd
	i=0;
	writestring("Product:");
	while((mmc_info.name[i]!=0x00)&&(i<16)) writechar(mmc_info.name[i++]);
	//Print Card Size(eg:128MB)
	gotoxy(1,0);
	writestring("Tot:"); 
	writeNumber(mmc_info.size_MB); writestring("MB ");
	//gotoxy(2,0);
	//writestring("sector_mult:"); writeNumber(mmc_info.sector_multiply);
	//gotoxy(3,0);
	//writestring("sect_cnt:"); writeNumber(mmc_info.sector_count);*/

}

//****************************************************************************
//Send a Command to MMC/SD-Card
//Return: the second uint8 of response register of MMC/SD-Card
uint8 Write_Command_MMC()
//****************************************************************************
{
	uint8 tmp;
	uint8 retry=0;
	uint8 i;

	//set MMC_Chip_Select to high (MMC/SD-Card disable) 
	MMC_Disable();
	//send 8 Clock Impulse
	Write_Byte_MMC(0xFF);
	//set MMC_Chip_Select to low (MMC/SD-Card active)
	MMC_Enable();

	//send 6 Byte Command to MMC/SD-Card
	for (i=0;i<0x06;i++) 
	{ 
		Write_Byte_MMC(mmc_cmd[i]);
	}

	//get 16 bit response
	Read_Byte_MMC(); //read the first uint8,ignore it. 
	do 
	{  //Only last 8 bit is used here.Read it out. 
		tmp = Read_Byte_MMC();
		retry++;
	}
	while((tmp==0xff)&&(retry<100)); 
	return(tmp);
}
#if 0
//****************************************************************************
//Routine for reading a uint8 from MMC/SD-Card
uint8 Read_Byte_MMC(void)
//****************************************************************************
{ 
	uint8 temp=0;
	uint8 i;

	cbi(MMC_Write,SPI_BUSY); //MMC_BUSY_LED=0;
	//Software SPI
	for (i=0; i<8; i++) //MSB First
	{
		cbi(MMC_Write,SPI_Clock); //Clock Impuls (Low)
		if(Init_Flag) delay_us_8_;
		//read mmc dat out pin
		if((MMC_Read&(1<<SPI_DI))!=0)
			temp = (temp << 1) + 1; 
		else
			temp = (temp << 1) + 0;
		sbi(MMC_Write,SPI_Clock); //set Clock Impuls High
		if(Init_Flag) delay_us_8_;	
	}
	sbi(MMC_Write,SPI_BUSY); //MMC_BUSY_LED=1;
	return (temp);
}

//****************************************************************************
//Routine for sending a uint8 to MMC/SD-Card
void Write_Byte_MMC(uint8 value)
//****************************************************************************
{ 
	uint8 i; 

	cbi(MMC_Write,SPI_BUSY); //MMC_BUSY_LED=0; 
	//Software SPI
	for (i=0; i<8; i++) 
	{  //write a uint8
		if (((value >> (7-i)) & 0x01)==0x01)
			sbi(MMC_Write,SPI_DO); //Send bit by bit(MSB First)
		else
			cbi(MMC_Write,SPI_DO);
		cbi(MMC_Write,SPI_Clock); //set Clock Impuls low
		if(Init_Flag) delay_us_8_; 
		sbi(MMC_Write,SPI_Clock); //set Clock Impuls High
		if(Init_Flag) delay_us_8_;     
	}//write a uint8
	sbi(MMC_Write,SPI_DO);	//set Output High 
	sbi(MMC_Write,SPI_BUSY); //MMC_BUSY_LED=1;
}
#endif
//****************************************************************************
//Routine for writing a Block(512Byte) to MMC/SD-Card
//Return 0 if sector writing is completed.
uint8 MMC_write_sector(uint32 addr,uint8 *Buffer)
//****************************************************************************
{  
	uint8 tmp,retry;
	uint16 i;
	//Command 24 is a writing blocks command for MMC/SD-Card.
#if 0
	uint8 CMD[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 

	asm("cli"); //clear all interrupt.

	addr = addr << 9; //addr = addr * 512 
	CMD[1] = ((addr & 0xFF000000) >>24 );
	CMD[2] = ((addr & 0x00FF0000) >>16 );
	CMD[3] = ((addr & 0x0000FF00) >>8 );
#else
	mmc_cmd[0]=0x58;
	mmc_cmd[4]=0;
	mmc_cmd[5]=0xFF;
	IniBlockAdd(addr);
#endif

	//Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)
	retry=0;
	do
	{  //Retry 100 times to send command.
		tmp=Write_Command_MMC();
		retry++;
		if(retry==10) 
		{ 
			return(tmp); //send commamd Error!
		}
	}
	while(tmp!=0); 

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
		return(WRITE_BLOCK_ERROR); //Error!
	}
	//Wait till MMC/SD-Card is not busy
	while (Read_Byte_MMC()!=0xff){};

	//set MMC_Chip_Select to high (MMC/SD-Card Invalid)
	MMC_Disable();
	return(0);
} 

//****************************************************************************
//Routine for reading dat Registers of MMC/SD-Card
//Return 0 if no Error.
//uint8 MMC_Read_Block(uint8 *CMD,uint8 *Buffer,uint16 Bytes)
uint8 MMC_Read_Block(uint8 *Buffer,uint16 Bytes)
//****************************************************************************
{  
	uint16 i; 
	uint8 retry,temp;

	//Send Command CMD to MMC/SD-Card
	retry=0;
	do
	{  //Retry 100 times to send command.
		temp=Write_Command_MMC();
		retry++;
		if(retry==10) 
		{
			return(READ_BLOCK_ERROR); //block write Error!
		}
	}
	while(temp!=0); 

	//Read Start Byte form MMC/SD-Card (FEh/Start Byte)
	retry=0;
	while (Read_Byte_MMC() != 0xfe)
	{
		retry++;
		if(retry==100) 
		{
			return(READ_BLOCK_ERROR); //block write Error!
		}
	};

	//Write blocks(normal 512Bytes) to MMC/SD-Card
	for (i=0;i<Bytes;i++)
	{
		*Buffer++ = Read_Byte_MMC();
	}

	//CRC-Byte
	Read_Byte_MMC();//CRC - Byte 
	Read_Byte_MMC();//CRC - Byte

	//set MMC_Chip_Select to high (MMC/SD-Card invalid)
	MMC_Disable();
	return(0);
}

//****************************************************************************
//Routine for reading Blocks(512Byte) from MMC/SD-Card
//Return 0 if no Error.
uint8 MMC_read_sector(uint32 addr,uint8 *Buffer)
//****************************************************************************
{	
	//Command 16 is reading Blocks from MMC/SD-Card
	//uint8 CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
	uint8 temp;
	asm("cli"); //clear all interrupt.
	//Address conversation(logic block address-->uint8 address) 

#if 0
	addr = addr << 9; //addr = addr * 512 
	CMD[1] = ((addr & 0xFF000000) >>24 );
	CMD[2] = ((addr & 0x00FF0000) >>16 );
	CMD[3] = ((addr & 0x0000FF00) >>8 );
#else
	mmc_cmd[0]=0x51;
	mmc_cmd[4]=0;
	mmc_cmd[5]=0xFF;
	IniBlockAdd(addr);
#endif

	temp=MMC_Read_Block(Buffer,512);

	return(temp);
}

//***************************************************************************
//Routine for reading CID Registers from MMC/SD-Card (16Bytes) 
//Return 0 if no Error.
uint8 Read_CID_MMC(uint8 *Buffer)
//***************************************************************************
{
	//Command for reading CID Registers
	//uint8 CMD[] = {0x4A,0x00,0x00,0x00,0x00,0xFF}; 
	uint8 temp;
	clr_mmc_cmd();
	mmc_cmd[0]=0x51;
	//mmc_cmd[1]=0;
	//mmc_cmd[2]=0;
	//mmc_cmd[3]=0;
	//mmc_cmd[4]=0;
	mmc_cmd[5]=0xFF;
	temp=MMC_Read_Block(Buffer,16); //read 16 bytes

	return(temp);
}

//***************************************************************************
//Routine for reading CSD Registers from MMC/SD-Card (16Bytes)
//Return 0 if no Error.
uint8 Read_CSD_MMC(uint8 *Buffer)
//***************************************************************************
{	
	//Command for reading CSD Registers
	//uint8 CMD[] = {0x49,0x00,0x00,0x00,0x00,0xFF};
	uint8 temp;
	clr_mmc_cmd();
	mmc_cmd[0]=0x49;
	mmc_cmd[5]=0xFF;
	temp=MMC_Read_Block(Buffer,16); //read 16 bytes

	return(temp);
}

//***************************************************************************
//Return: [0]-success or something error!
uint8 MMC_Start_Read_Sector(uint32 sector)
//***************************************************************************
{  
	uint8 retry;
	//Command 16 is reading Blocks from MMC/SD-Card
	//uint8 CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
	uint8 temp;

	asm("cli"); //clear all interrupt.
	//Address conversation(logic block address-->uint8 address)  
	//sector = sector << 9; //sector = sector * 512

	//CMD[1] = ((sector & 0xFF000000) >>24 );
	//CMD[2] = ((sector & 0x00FF0000) >>16 );
	//CMD[3] = ((sector & 0x0000FF00) >>8 );
	mmc_cmd[0]=0x51;
	mmc_cmd[4]=0;
	mmc_cmd[5]=0xFF;
	IniBlockAdd(sector);
	//Send Command CMD to MMC/SD-Card
	retry=0;
	do
	{  //Retry 100 times to send command.
		temp=Write_Command_MMC();
		retry++;
		if(retry==10) 
		{
			return(READ_BLOCK_ERROR); //block write Error!
		}
	}
	while(temp!=0); 

	//Read Start Byte form MMC/SD-Card (FEh/Start Byte)
	//Now dat is ready,you can read it out.
	while (Read_Byte_MMC() != 0xfe){};
	return 0; //Open a sector successfully!
}

//***************************************************************************
void MMC_get_data(uint16 Bytes,uint8 *buffer) 
//***************************************************************************
{
	uint16 j;

	asm("cli"); //clear all interrupt.
	for (j=0;((j<Bytes) && (readPos<512));j++)
	{	
		*buffer++ = Read_Byte_MMC();
		readPos++; //read a uint8,increase read position
	}
	if (readPos==512)  
	{  //CRC-Bytes
		Read_Byte_MMC();//CRC - Byte 
		Read_Byte_MMC();//CRC - Byte
		readPos=0;      //reset sector read offset 
		sectorPos++;    //Need to read next sector
		LBA_Opened=0;   //Set to 1 when a sector is opened.
		//set MMC_Chip_Select to high (MMC/SD-Card invalid)
		MMC_Disable();  //MMC disable
	}
}

//***************************************************************************
void MMC_get_data_LBA(uint32 lba, uint16 Bytes,uint8 *buffer)
//***************************************************************************
{ //get dat from lba address of MMC/SD-Card
	//If a new sector has to be read then move head
	if (readPos==0) MMC_Start_Read_Sector(lba); 
	MMC_get_data(Bytes,buffer);
}

//***************************************************************************
void MMC_GotoSectorOffset(uint32 LBA,uint16 offset)
//***************************************************************************
{  
	//Find the offset in the sector
	uint8 temp[1];
	MMC_LBA_Close(); //close MMC when read a new sector(readPos=0)
	while (readPos<offset) MMC_get_data_LBA(LBA,1,temp); //go to offset  
}

//***************************************************************************
void MMC_LBA_Close()
//***************************************************************************
{  
	uint8 temp[1];
	while((readPos!=0x00)|(LBA_Opened==1))
	{ //read MMC till readPos==0x00
		MMC_get_data(1, temp); //dummy read,temp is a valid dat.
	}  
}

//---------------------------------------------------------------------------- 


