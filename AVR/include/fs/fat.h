/************************************************************/
/*		         HEAD FILE  for   FAT					    */
/*				   By   pasyong								*/
/*				   2006-4									*/
/*				   Base ICC6.31A					        */
/************************************************************/
#ifndef _SYSTEM_FS_FAT_H
#define _SYSTEM_FS_FAT_H
#include <configure.h>
#ifndef _SYSTEM_FS_FAT_CFG
//----------------------FAT配置-----------------------//
#define _SYSTEM_FS_FAT_CFG
//#define FAT_MAX_FILE_NAME_LEN 16
#define FAT_USE_FILE_BUFFER//定义在文件系统上建立一个临时文件，这样可以避免出现对FATBuffer的争用
#ifdef FAT_USE_FILE_BUFFER
#define FILE_BUFFER_NAME "FATTMPFLTMP"
#endif
//======================FAT配置=======================//
#endif//_SYSTEM_FS_FAT_CFG
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

//不要更改下面的数值因为它不仅参与比较运算
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

typedef u32 Cluster;
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
	u8	bsJump[3];				// 跳转指令
	char	bsOEMName[8];		// 厂商标志和OS版本号
	char	bsBPB[53];			// BIOS 参数块
	char	bsExt[26];			// 扩展BPB
	char	bsBootCode[418];	// 引导扇区代码
	u8	bsBootSectSig2;				
	u8	bsBootSectSig3;
	u8	bsBootSectSig0;			// 引导扇区签名0x55
	u8	bsBootSectSig1;			// 引导扇区签名0xAA
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
		//u8		deName[8];     		// 文件名
		//u8		deExtension[3];		// 扩展名
	union{
		struct{
				u8	Name[8];        // 文件名
#define SLOT_EMPTY      0x00   		// 目录项为空
#define SLOT_E5         0x05   		 
#define SLOT_DELETED    0xe5   		// 文件已被删除
				u8	Ext[3];			// 扩展名
			}Name;
			u8 FullName[11];
		}Name;//文件名
		u8		Attributes;  		// 文件属性
#define FILE_ATTR_NORMAL     0x00   		// 读写
#define FILE_ATTR_READONLY   0x01   		// 只读
#define FILE_ATTR_HIDDEN     0x02   		// 隐藏
#define FILE_ATTR_SYSTEM     0x04   		// 系统文件
#define FILE_ATTR_VOLUME     0x08   		// 卷标文件
#define FILE_ATTR_LONG_FILENAME	0x0f	// 长文件名文件			    
#define FILE_ATTR_DIRECTORY  0x10    	// 子目录文件
#define FILE_ATTR_ARCHIVE    0x20    	// 归档文件
		u8        LowerCase;  	// 系统保留
#define LCASE_BASE      0x08    	  
#define LCASE_EXT       0x10    	  
		u8        CHundredth; 	// 文件创建时间的10MS
		u16       CTime;   	// 文件创建时间
		u16       CDate;   	// 文件创建日期
		u16       ADate;   	// 文件最近访问日期
		u16       HighClust; 		// 文件起始簇号的高16位
		u16       MTime;   	// 文件最近修改时间
		u16       MDate;   	// 文件最近修改日期
		u16       StartCluster;   // 文件起始簇号的低16位
		u32       Size;  	    // 文件长度
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

typedef struct FIND_FILE_INFO{
	u32	Clust; //当前目录的簇号
	u16	Nums;   //仅FAT12—FAT16中的根目录区有效
	u8	Index;  //当前的BLOCK中目录项索引
	u32	Sector; //当前目录所在的扇区号
	u8	NumSector;

}FindFileInfo;

typedef union{
	 u32   Sector;    //FAT12&FAT16有效
	 u32   Clust;     //FAT32时有效
}ROOTDIR_INF;

typedef struct _FILE{
	union 
	{
		struct
		{
			u8	Name[8];         // 文件名
			u8	Ext[3];			// 扩展名
		}Name;
		u8 FullName[11];
	}Name;
	union	
	{
		u8 Attribute;
		struct
		{
			u8 IsReadOnly	:1;
			u8 IsHide		:1;
			u8 IsSystem		:1;
			u8 IsVolume		:1;
			u8 IsDirectory	:1;
			u8 IsArchive	:1;
		}Attributes;
	}Attributes;		// 文件属性
	u32 StartCluster;	// 文件起始簇号
	u32	Size;  			// 文件长度
	u32	Position;
	u32 CurrentCluster;
	u32 DirEntrySector;
	u8  DirEntryIndex;
}File;
extern u8 FatBuffer[512];
// 可调函数   
u8 FATInit( void);
/******************************************************
从文件夹中查找文件或目录
DirClust：目录所在的族（0为根目录）
file	：要查找的文件的文件名和扩展名
fileIndex：要查找的文件的目录索引
withName：为true通过文件名查找否则通过索引
return	：查到文件返回True，并填写file中的其他字段
******************************************************/
bool FatFindFile(Cluster DirClust,File* file,u16 fileIndex,bool withName);
/******************************************************
从文件夹中查找文件或目录
DirClust：目录所在的族（0为根目录）
fileIndex：要查找的文件的目录索引
return	：查到文件返回True，并填写file中的字段
******************************************************/
bool FileOpenWithIndex(Cluster DirClust,File* file,u16 fileIndex);
/******************************************************
从文件夹中查找文件或目录
dirCluster：目录所在的族（0为根目录）
file	  ：要查找的文件的文件名和扩展名
return	  ：查到文件返回True，并填写file中的其他字段
******************************************************/
bool FileOpenWithName(Cluster dirCluster,File *file);
/******************************************************
读取文件的下一个扇区（512字节）
file  ：要读取的文件
buffer：读取数据的储蓄buffer
return：成功返回True，到了文件的末尾返回false
******************************************************/
bool FatReadSector(File* file,u8 buffer[512]);
/******************************************************
新建文件或目录
DirClust：目录所在的族（0为根目录）
fileName：文件名
Attributes：文件属性
return	：成功返回True
******************************************************/
bool FileNew(Cluster DirClust,char fileName[11],u8 Attributes);

/******************************************************
写文件的Position所指扇区（512字节）并将Position后移512字节
到了文件的末尾将扩充文件的容量，如果没有可用的空族将返回false
注意：此方法使用FatBuffer，不能让buffer与FatBuffer重用，除非
定义了FAT_USE_FILE_BUFFER
file  ：要写入的文件
buffer：数据
return：成功返回True
******************************************************/
bool FatWriteSector(File* file,u8 buffer[512]);
/******************************************************
保存文件或目录
注意：此方法使用FatBuffer
file：要保存的文件
return	：成功返回True
******************************************************/
bool FileSave(File* file);
/******************************************************
查找目录中是否存在文件
注意：此方法使用FatBuffer
DirClust：目录所在的族（0为根目录）
fileName：要查找的文件名
return	：否存返回True
******************************************************/
bool FileExist(Cluster DirClust,char fileName[11]);
bool FileDelete(Cluster DirClust,char fileName[11]);
bool FileSetPosition(File* file,u32 Position);
#endif//_SYSTEM_FS_FAT_H
