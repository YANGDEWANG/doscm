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
//--------------------------MMC配置----------------------------//
#define MMC_CFG
#define MMC_INFO_READ_U		1//
#define MMC_SOFT_SPI		1//
#define MMC_SOFT_EXTPULLUP	1//定义使用外部上拉电阻

#define MMC_PORT	D
#define MMC_CS		7	//-->MMC_CS_PIN 
#ifdef MMC_SOFT_SPI
#define MMC_DI		5	//-->MMC_DO_PIN 
#define MMC_DO		6	//-->MMC_DI_PIN 
#define MMC_CLK		4	//-->MMC_CLK_PIN
#endif//MMC_SOFT_SPI
//------------------------MMC配置_END---------------------------//

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
	uint16	SizeMB;
	uint32	SectorCount;
	uint32	Size;
	uint8	Name[6];
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
typedef union MMCCSD
{
	struct 
	{
		uint8	n4:2;//reserved -  R [121:120]
		uint8	SPEC_VERS :4;//System specification version  R [125:122]
		uint8	CSD_STRUCTURE :2;//CSD structure  R [127:126]

		//uint8	TAAC :8;//data read access-time-1  R [119:112]
		uint8	TAAC_UNIT :3;
		uint8	TAAC_VALUE :4;
		uint8	TAAC_re :1;
		uint8	NSAC :8;// R [111:104]

		uint8	TRAN_SPEED :8;//max. data transfer rate  R [103:96]

		//uint16	CCC :12;//card command classes  R [95:84]
		uint8	CCC_H;

		uint8	READ_BL_LEN :4;//max. read data block length  R [83:80]
		uint8	CCC_L :4;

		uint8	C_SIZE_H :2;
		uint8	n3:2;//reserved -  R [75:74]
		uint8	DSR_IMP :1;//DSR implemented  R [76:76]
		uint8	READ_BLK_MISALIGN :1;//read block misalignment  R [77:77]
		uint8	WRITE_BLK_MISALIGN :1;//write block misalignment  R [78:78]
		uint8	READ_BL_PARTIAL :1;//partial blocks for read allowed  R [79:79]

		uint8	C_SIZE_M;

		uint8	VDD_R_CURR_MAX :3;//max. read current @VDD max  R [58:56]
		uint8	VDD_R_CURR_MIN :3;//max. read current @VDD min  R [61:59]
		//uint16	C_SIZE :12;//device size  R [73:62]
		uint8	C_SIZE_L :2;

		uint8	C_SIZE_MULT_H :2;
		uint8	VDD_W_CURR_MAX :3;//max. write current @VDD max  R [52:50]
		uint8	VDD_W_CURR_MIN :3;//max. write current @VDD min  R [55:53]

		uint8	ERASE_GRP_MULT_H :2;
		uint8	ERASE_GRP_SIZE :5;//erase group size  R [46:42]
		//uint8	C_SIZE_MULT :3;//device size multiplier  R [49:47]
		uint8	C_SIZE_MULT_L :1;

		uint8	WP_GRP_SIZE :5;//write protect group size  R [36:32]
		//uint8	ERASE_GRP_MULT :5;//erase group size multiplier R [41:37]
		uint8	ERASE_GRP_MULT_L :3;

		uint8	WRITE_BL_LEN_H :2;
		uint8	R2W_FACTOR :3;//write speed factor  R [28:26]
		uint8	DEFAULT_ECC :2;//manufacturer default ECC  R [30:29]
		uint8	WP_GRP_ENABLE :1;//write protect group enable  R [31:31]

		uint8	CONTENT_PROT_APP :1;//Content protection application  R [16:16]
		uint8	n2:4;//Reserved -  R [20:17]
		uint8	WRITE_BL_PARTIAL :1;//partial blocks for write allowed  R [21:21]
		//uint8	WRITE_BL_LEN :4;//max. write data block length  R [25:22]
		uint8	WRITE_BL_LEN_L :2;

		uint8	ECC :2;//ECC code  R/W/E [9:8]
		uint8	FILE_FORMAT :2;//File format  R/W [11:10]
		uint8	TMP_WRITE_PROTECT :1;//temporary write protection  R/W/E [12:12]
		uint8	PERM_WRITE_PROTECT :1;//permanent write protection  R/W [13:13]
		uint8	COPY :1;//copy flag (OTP) R/W [14:14]
		uint8	FILE_FORMAT_GRP :1;//File format group  R/W [15:15]

		uint8	n1:1;//not used, always ’1’ [0:0]
		uint8	CRC:7;// CRC 7 R/W/E [7:1]
	}mmc_csd_file;
	uint8 dat[16];
}MMC_CSD;

//extern	BUFFER_TYPE sectorBuffer; //512 bytes for sector buffer

//--------------------------------------------------------------
extern VOLUME_INFO_TYPE mmc_info;
//---------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------

bool MMC_Read_Block(uint8 *Buffer,uint16 Bytes);
bool MMCInit(void);
/****************************************************************************
写扇区（512字节）
addr  ：扇区编号
Buffer：数据储蓄区
****************************************************************************/
bool MMCWriteSector(uint32 addr,uint8 *Buffer);
/****************************************************************************
读取块
Buffer：数据储蓄区
Bytes ：块字节数
****************************************************************************/
bool MMCReadBlock(uint8 *Buffer,uint16 Bytes);
/****************************************************************************
读取扇区（512字节）
addr  ：扇区编号
Buffer：数据储蓄区
****************************************************************************/
bool MMCReadSector(uint32 addr,uint8 *Buffer);
/****************************************************************************
读取一个32bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u32 MMCReadu32(u32 sector,u16 offset);
/****************************************************************************
读取一个16bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u16 MMCReadu16(u32 sector,u16 offset);
/****************************************************************************
读取一个8bit数
sector：扇区编号
offset：数据储蓄区
****************************************************************************/
u8 MMCReadu8(u32 sector,u16 offset);
bool MMCWriteCommand(uint8 cmd,uint32 arg,uint8 succeed);
#ifdef MMC_INFO_READ_U
bool Read_CSD_MMC(uint8 *Buffer);
bool Read_CID_MMC(uint8 *Buffer);
bool MMCGetVolumeInfo(void);
#endif//MMC_INFO_READ_U


#define nop() asm("nop"); //asm nop defined in CVAVR

#endif //_MMC_H_


