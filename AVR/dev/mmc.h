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

#ifndef _MMC_H_
#define _MMC_H_
#include <configure.h>
#ifndef MMC_CFG
//--------------------------MMC≈‰÷√----------------------------//
#define MMC_CFG
#define MMC_PORT	B
#define MMC_CS		2	//-->MMC_CS_PIN 
//------------------------MMC≈‰÷√_END---------------------------//

#endif//MMC_CFG
#define DDR_MMC		GDDR(MMC_PORT)
#define PORT_MMC	GPORT(MMC_PORT)
#define PIN_MMC 	GPIN(MMC_PORT)
//------------------------------------------------------------
// Error define
//-------------------------------------------------------------
#define INIT_CMD0_ERROR     0x01
#define INIT_CMD1_ERROR		0x02
#define WRITE_BLOCK_ERROR	0x03
#define READ_BLOCK_ERROR   	0x04 
//-------------------------------------------------------------
// data type
//-------------------------------------------------------------   
// this structure holds info on the MMC card currently inserted 
typedef struct MMC_VOLUME_INFO
{ //MMC/SD Card info
	uint16   size_MB;
	uint8   sector_multiply;
	uint16   sector_count;
	uint8   name[6];
} VOLUME_INFO_TYPE; 


typedef struct STORE 
{ 
	uint8   dat[256]; 
}BUFFER_TYPE; //256 bytes, 128 words

//extern	BUFFER_TYPE sectorBuffer; //512 bytes for sector buffer

//--------------------------------------------------------------
extern	uint16	readPos;
extern	uint8	sectorPos;
extern	uint8    LBA_Opened; //Set to 1 when a sector is opened.
extern	uint8    Init_Flag;    //Set it to 1 when Init is processing.
//---------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------
void MMC_Port_Init(void);

uint8 Read_Byte_MMC(void);
//uint8 Read_Byte_MMC_Long(void);

void Write_Byte_MMC(uint8 value);
//void Write_Byte_MMC_Long(uint8 value);

uint8 MMC_Read_Block(uint8 *CMD,uint8 *Buffer,uint16 Bytes);
uint8 MMC_Init(void);
uint8 MMC_write_sector(uint32 addr,uint8 *Buffer);
//uint8 MMC_read_sector(uint32 addr,uint8 *Buffer);
uint8 Write_Command_MMC(uint8 *CMD);
uint8 Read_CSD_MMC(uint8 *Buffer);
uint8 Read_CID_MMC(uint8 *Buffer);
uint8 MMC_Start_Read_Sector(uint32 sector);
void MMC_get_volume_info(void);
void MMC_get_data(uint16 Bytes,uint8 *buffer);
void MMC_get_data_LBA(uint32 lba, uint16 Bytes,uint8 *buffer);
void MMC_GotoSectorOffset(uint32 LBA,uint16 offset);
void MMC_LBA_Close(void);


//set MMC_Chip_Select to high (MMC/SD-Card Invalid)
#define MMC_Disable() sbi(PORT_MMC,MMC_CS)
//set MMC_Chip_Select to low (MMC/SD-Card Active)
#define MMC_Enable() cbi(PORT_MMC,MMC_CS)

#define nop() asm("nop"); //asm nop defined in CVAVR

#define delay_us_8_ asm("nop")
#endif //_MMC_H_


