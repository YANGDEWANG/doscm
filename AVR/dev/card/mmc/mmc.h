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

struct mmc_cid {
	uint16		manfid;
	char			prod_name[8];
	uint16		serial;
	unsigned short		oemid;
	unsigned short		year;
	uint8		hwrev;
	uint8		fwrev;
	uint8		month;
};
union mmc_csd
{
	struct 
	{
uint8	:1;//not used, always °Ø1°Ø [0:0]
		uint8	CRC:7;// CRC 7 R/W/E [7:1]
		uint8	ECC :2;//ECC code  R/W/E [9:8]
		uint8	FILE_FORMAT :2;//File format  R/W [11:10]
		uint8	TMP_WRITE_PROTECT :1;//temporary write protection  R/W/E [12:12]
		uint8	PERM_WRITE_PROTECT :1;//permanent write protection  R/W [13:13]
		uint8	COPY :1;//copy flag (OTP) R/W [14:14]
		uint8	FILE_FORMAT_GRP :1;//File format group  R/W [15:15]
		uint8	CONTENT_PROT_APP :1;//Content protection application  R [16:16]
uint8	:4;//Reserved -  R [20:17]
		uint8	WRITE_BL_PARTIAL :1;//partial blocks for write allowed  R [21:21]
		//uint8	WRITE_BL_LEN :4;//max. write data block length  R [25:22]
		uint8	WRITE_BL_PARTIAL_L :2;
		uint8	WRITE_BL_PARTIAL_H :2;
		uint8	R2W_FACTOR :3;//write speed factor  R [28:26]
		uint8	DEFAULT_ECC :2;//manufacturer default ECC  R [30:29]
		uint8	WP_GRP_ENABLE :1;//write protect group enable  R [31:31]
		uint8	WP_GRP_SIZE :5;//write protect group size  R [36:32]
		//uint8	ERASE_GRP_MULT :5;//erase group size multiplier R [41:37]
		uint8	ERASE_GRP_MULT_L :3;
		uint8	ERASE_GRP_MULT_H :2;
		uint8	ERASE_GRP_SIZE :5;//erase group size  R [46:42]
		//uint8	C_SIZE_MULT :3;//device size multiplier  R [49:47]
		uint8	C_SIZE_MULT_L :1;
		uint8	C_SIZE_MULT_H :2;
		uint8	VDD_W_CURR_MAX :3;//max. write current @VDD max  R [52:50]
		uint8	VDD_W_CURR_MIN :3;//max. write current @VDD min  R [55:53]
		uint8	VDD_R_CURR_MAX :3;//max. read current @VDD max  R [58:56]
		uint8	VDD_R_CURR_MIN :3;//max. read current @VDD min  R [61:59]
		//uint16	C_SIZE :12;//device size  R [73:62]
		uint8	C_SIZE_L :2;
		uint8	C_SIZE_M;
		uint8	C_SIZE_H :2;
uint8	:2;//reserved -  R [75:74]
		uint8	DSR_IMP :1;//DSR implemented  R [76:76]
		uint8	READ_BLK_MISALIGN :1;//read block misalignment  R [77:77]
		uint8	WRITE_BLK_MISALIGN :1;//write block misalignment  R [78:78]
		uint8	READ_BL_PARTIAL :1;//partial blocks for read allowed  R [79:79]
		uint8	READ_BL_LEN :4;//max. read data block length  R [83:80]
		//uint16	CCC :12;//card command classes  R [95:84]
		uint8	CCC_L :4;
		uint8	CCC_H;
		uint8	TRAN_SPEED :8;//max. data transfer rate  R [103:96]
		uint8	NSAC :8;// R [111:104]
		uint8	TAAC :8;//data read access-time-1  R [119:112]
uint8	:2;//reserved -  R [121:120]
		uint8	SPEC_VERS :4;//System specification version  R [125:122]
		uint8	CSD_STRUCTURE :2;//CSD structure  R [127:126]
	}mmc_csd_file;
	uint8 dat[16];
};

//extern	BUFFER_TYPE sectorBuffer; //512 bytes for sector buffer

//--------------------------------------------------------------
extern VOLUME_INFO_TYPE mmc_info;
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

uint8 MMC_Read_Block(uint8 *Buffer,uint16 Bytes);
uint8 MMCInit(void);
uint8 MMC_write_sector(uint32 addr,uint8 *Buffer);
//uint8 MMC_read_sector(uint32 addr,uint8 *Buffer);
uint8 Write_Command_MMC();
uint8 Read_CSD_MMC(uint8 *Buffer);
uint8 Read_CID_MMC(uint8 *Buffer);
uint8 MMC_Start_Read_Sector(uint32 sector);
void MMCGetVolumeInfo(void);
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


