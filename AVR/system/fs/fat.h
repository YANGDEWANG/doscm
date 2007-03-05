/************************************************************/
/*		         HEAD FILE  for   FAT					    */
/*				   By   pasyong								*/
/*				   2006-4									*/
/*				   Base ICC6.31A					        */
/************************************************************/
#ifndef _SYSTEM_FS_FAT_H
#define _SYSTEM_FS_FAT_H
#include <configure.h>

//簇定义
#define MSDOSFSROOT     0               
#define CLUST_FREE      0               
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_RSRVD     0xfffffff6      // 系统保留簇
#define CLUST_BAD       0xfffffff7      // 坏簇
#define CLUST_EOFS      0xfffffff8      // 第一个结束簇
#define CLUST_EOFE      0xffffffff      // 最后一个结束簇

#define FAT12_MASK      0x00000fff      // FAT12掩码
#define FAT16_MASK      0x0000ffff      // FAT16掩码
#define FAT32_MASK      0x0fffffff      // FAT32掩码

#define FAT12           0
#define FAT16           1
#define FAT32           2

// 分区类型
#define PART_TYPE_UNKNOWN		0x00
#define PART_TYPE_FAT12			0x01
#define PART_TYPE_XENIX			0x02
#define PART_TYPE_DOSFAT16	0x04
#define PART_TYPE_EXTDOS		0x05
#define PART_TYPE_FAT16			0x06
#define PART_TYPE_NTFS			0x07
#define PART_TYPE_FAT32			0x0B
#define PART_TYPE_FAT32LBA	0x0C
#define PART_TYPE_FAT16LBA	0x0E
#define PART_TYPE_EXTDOSLBA	0x0F
#define PART_TYPE_ONTRACK		0x33
#define PART_TYPE_NOVELL		0x40
#define PART_TYPE_PCIX			0x4B
#define PART_TYPE_PHOENIXSAVE	0xA0
#define PART_TYPE_CPM				0xDB
#define PART_TYPE_DBFS			0xE0
#define PART_TYPE_BBT				0xFF

typedef struct PART_RECORD//  16 字节
{			
	u8	prIsActive;					// 0x80代表该分区为缺省分区
	u8	prStartHead;				// 该分区入口的磁头地址
	u16	prStartCylSect;				// 该分区入口的扇区地址和柱面地址
	u8	prPartType;					// 该分区类型
	u8	prEndHead;					// 该分区结束的扇区地址
	u16	prEndCylSect;				// 该分区结束的柱面地址
	u32	prStartLBA;					// 该分区内第一个扇区地址
	u32	prSize;						// 该分区所包含的扇区总数
}PartRecord;

        
typedef struct PART_SECTOR{
	char	psPartCode[512-64-2];	// 启动代码 (446 bytes)
	u8	psPart[64];						// 四个分区表(64 bytes)
	u8	psBootSectSig0;				// 两个签名(2 bytes)
	u8	psBootSectSig1;
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
}PartSector;

//系统引导扇区(DOS BOOT RECORD)DBR结构

typedef struct BOOT_SECTOR710{
	u8	bsJump[3];						// 跳转指令
	char	bsOEMName[8];					// 厂商标志和OS版本号
	char	bsBPB[53];						// BIOS 参数块
	char	bsExt[26];						// 扩展BPB
	char	bsBootCode[418];			// 引导扇区代码
	u8	bsBootSectSig2;				
	u8	bsBootSectSig3;
	u8	bsBootSectSig0;				// 引导扇区签名0x55
	u8	bsBootSectSig1;				// 引导扇区签名0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
#define BOOTSIG2        0
#define BOOTSIG3        0
}BootSector710;


// BIOS 参数块
typedef struct BPB710_{
		u16	bpbBytesPerSec;	// 每扇区字节数
		u8	bpbSecPerClust;	// 每簇扇区数
		u16	bpbResSectors;	// 保留区域中的保留扇区数
		u8	bpbFATs;		// FAT表的份数
		u16	bpbRootDirEnts;	// 根目录项数
		u16	bpbSectors;		// 此域为存储卷上的扇区总数
		u8	bpbMedia;		// 固定存储介质描述
		u16	bpbFATsecs;		// FAT表所占的扇区数
		u16	bpbSecPerTrack;	// 每道扇区数
		u16	bpbHeads;		// 磁头数
		u32	bpbHiddenSecs;	// 隐藏扇区数
		u32	bpbHugeSectors;	// 总扇区数
		u32 bpbBigFATsecs;	// 每个FAT区所占扇区数
		u16 bpbExtFlags;	// 扩展标志
#define FATNUM    0xf			
#define FATMIRROR 0x80			
		u16 bpbFSVers;		// 文件系统版本
#define FSVERS    0				
		u32 bpbRootClust;	// 根目录簇号
		u16 bpbFSInfo;		// 文件系统信息扇区号
		u16 bpbBackup;		// 备份引导扇区
} BPB710;

// 目录或文件入口地址结构
typedef struct DIR_ENTRY{
		u8		deName[8];          // 文件名
#define SLOT_EMPTY      0x00            // 目录项为空
#define SLOT_E5         0x05            
#define SLOT_DELETED    0xe5            // 文件已被删除
		u8		deExtension[3];     // 扩展名
		u8		deAttributes;       // 文件属性
#define ATTR_NORMAL     0x00            // 读写
#define ATTR_READONLY   0x01            // 只读
#define ATTR_HIDDEN     0x02            // 隐藏
#define ATTR_SYSTEM     0x04            // 系统文件
#define ATTR_VOLUME     0x08            // 卷标文件
#define ATTR_LONG_FILENAME	0x0f		// 长文件名文件			    
#define ATTR_DIRECTORY  0x10            // 子目录文件
#define ATTR_ARCHIVE    0x20            // 归档文件
		u8        deLowerCase;        // 系统保留
#define LCASE_BASE      0x08            
#define LCASE_EXT       0x10            
		u8        deCHundredth;       // 文件创建时间的10MS
		u8        deCTime[2];         // 文件创建时间
		u8        deCDate[2];         // 文件创建日期
		u8        deADate[2];         // 文件最近访问日期
		u16        deHighClust; 	    // 文件起始簇号的高16位
		u8        deMTime[2];         // 文件最近修改时间
		u8        deMDate[2];         // 文件最近修改日期
		u16       deStartCluster;     // 文件起始簇号的低16位
		u32       deFileSize;  	    // 文件长度
} DIREntry;

// 一个扇区中的目录项数
#define DIRENTRIES_PER_SECTOR	0x10

// 长文件名入口地址结构
typedef struct LONG_FILE_NAME{
		u8		weCnt;                //序号
#define WIN_LAST        0x40            
#define WIN_CNT         0x3f
		u8		wePart1[10];          //长文件名UNICODE码（第一部分）
		u8		weAttributes;         //长文件名目录项标志
#define ATTR_WIN95      0x0f
		u8		weReserved1;          //系统保留              
		u8		weChksum;             //校验值
		u8		wePart2[12];          //长文件名UNICODE码（第二部分）
		u16     weReserved2;          //文件开始簇号
		u8		wePart3[4];           //长文件名UNICODE码（第三部分）
} LongFileName;

#define WIN_CHARS	13      //长文件名入口地址结构可包含13个字符 

//在FAT32中，文件名最长可到255个字符
#define FAT_MAX_FILE_NAME_LEN      255

// 目录或文件入口地址结构中的时间结构

#define DT_2SECONDS_MASK        0x1F    // 秒
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // 分
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // 时
#define DT_HOURS_SHIFT          11

// 目录或文件入口地址结构中的日期结构

#define DD_DAY_MASK             0x1F    // 日
#define DD_DAY_SHIFT            0
#define DD_MONTH_MASK           0x1E0   // 月
#define DD_MONTH_SHIFT          5
#define DD_YEAR_MASK            0xFE00  // 年 - 1980
#define DD_YEAR_SHIFT           9

typedef struct{
	unsigned long Clust; //当前目录的簇号
	unsigned int  Nums;   //仅FAT12—FAT16中的根目录区有效
	unsigned char Index;  //当前的BLOCK中目录项索引
	unsigned long Sector; //当前目录所在的扇区号
	unsigned char NumSector;

}FIND_FILE_INFO;

typedef union{
	 ulong   Sector;    //FAT12&FAT16有效
	 ulong   Clust;     //FAT32时有效
}ROOTDIR_INF;

// 可调函数   
void ReadBlock(unsigned long LBA);
unsigned long fatClustToSect(unsigned long clust);
unsigned long  fatGetRootSector(void);
unsigned long fatGetRootClust(void);
unsigned char fatInit( void);
unsigned int fatClusterSize(void);
unsigned long fatGetFilesize(void);
char* fatGetFilename(void);
char* fatGetDirname(void);
void fatLoadCluster(unsigned long cluster, unsigned char *buffer);
unsigned long fatNextCluster(unsigned long cluster);
void GetSysFileClust(unsigned long *hzk,unsigned long *unit);
void FindLrcFile(unsigned long dircluster,unsigned char *strName);
unsigned char IsMusicFile(unsigned char *strName);
void InitSetPath(FIND_FILE_INFO *fp,unsigned long dircluster);
void ReadBlockToBuff(unsigned long LBA,unsigned char *pbuff); 
unsigned char ReadNextDirEntry(FIND_FILE_INFO *fp); 
void UniToGB(unsigned char *pbuf);
unsigned char IsUnitogbFile(unsigned char *pbuf);
unsigned char IsHzk12File(unsigned char *pbuf);
unsigned char IsCurDir(unsigned char *strName);
unsigned char IsFatherDir(unsigned char *strName);
unsigned int FindDirIndex(unsigned long fatherdir,unsigned long dircluster);
unsigned int SerarchFile(unsigned long dircluster,unsigned int index);
#endif//_SYSTEM_FS_FAT_H
