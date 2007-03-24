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
//----------------------FAT����-----------------------//
#define _SYSTEM_FS_FAT_CFG
//#define FAT_MAX_FILE_NAME_LEN 16
//======================FAT����=======================//
#endif//_SYSTEM_FS_FAT_CFG
//�ض���
#define MSDOSFSROOT     0               
#define CLUST_FREE      0               
#define MSDOSFSFREE     CLUST_FREE
#define CLUST_RSRVD     0xfffffff6      // ϵͳ������
#define CLUST_BAD       0xfffffff7      // ����
#define CLUST_EOFS      0xfffffff8      // ��һ��������
#define CLUST_EOFE      0xffffffff      // ���һ��������

#define FAT12_MASK      0x00000fff      // FAT12����
#define FAT16_MASK      0x0000ffff      // FAT16����
#define FAT32_MASK      0x0fffffff      // FAT32����

//��Ҫ�����������ֵ��Ϊ����������Ƚ�����
#define FAT12           0
#define FAT16           1
#define FAT32           2

// ��������
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
typedef struct PART_RECORD//  16 �ֽ�
{			
	u8	prIsActive;					// 0x80����÷���Ϊȱʡ����
	u8	prStartHead;				// �÷�����ڵĴ�ͷ��ַ
	u16	prStartCylSect;				// �÷�����ڵ�������ַ�������ַ
	u8	prPartType;					// �÷�������
	u8	prEndHead;					// �÷���������������ַ
	u16	prEndCylSect;				// �÷��������������ַ
	u32	prStartLBA;					// �÷����ڵ�һ��������ַ
	u32	prSize;						// �÷�������������������
}PartRecord;

        
typedef struct PART_SECTOR{
	char	psPartCode[512-64-2];	// �������� (446 bytes)
	u8	psPart[64];						// �ĸ�������(64 bytes)
	u8	psBootSectSig0;				// ����ǩ��(2 bytes)
	u8	psBootSectSig1;
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
}PartSector;

//ϵͳ��������(DOS BOOT RECORD)DBR�ṹ

typedef struct BOOT_SECTOR710{
	u8	bsJump[3];				// ��תָ��
	char	bsOEMName[8];		// ���̱�־��OS�汾��
	char	bsBPB[53];			// BIOS ������
	char	bsExt[26];			// ��չBPB
	char	bsBootCode[418];	// ������������
	u8	bsBootSectSig2;				
	u8	bsBootSectSig3;
	u8	bsBootSectSig0;			// ��������ǩ��0x55
	u8	bsBootSectSig1;			// ��������ǩ��0xAA
#define BOOTSIG0        0x55
#define BOOTSIG1        0xaa
#define BOOTSIG2        0
#define BOOTSIG3        0
}BootSector710;


// BIOS ������
typedef struct BPB710_{
		u16	bpbBytesPerSec;	// ÿ�����ֽ���
		u8	bpbSecPerClust;	// ÿ��������
		u16	bpbResSectors;	// ���������еı���������
		u8	bpbFATs;		// FAT��ķ���
		u16	bpbRootDirEnts;	// ��Ŀ¼����
		u16	bpbSectors;		// ����Ϊ�洢���ϵ���������
		u8	bpbMedia;		// �̶��洢��������
		u16	bpbFATsecs;		// FAT����ռ��������
		u16	bpbSecPerTrack;	// ÿ��������
		u16	bpbHeads;		// ��ͷ��
		u32	bpbHiddenSecs;	// ����������
		u32	bpbHugeSectors;	// ��������
		u32 bpbBigFATsecs;	// ÿ��FAT����ռ������
		u16 bpbExtFlags;	// ��չ��־
#define FATNUM    0xf			
#define FATMIRROR 0x80			
		u16 bpbFSVers;		// �ļ�ϵͳ�汾
#define FSVERS    0				
		u32 bpbRootClust;	// ��Ŀ¼�غ�
		u16 bpbFSInfo;		// �ļ�ϵͳ��Ϣ������
		u16 bpbBackup;		// ������������
} BPB710;

// Ŀ¼���ļ���ڵ�ַ�ṹ
typedef struct DIR_ENTRY{
		//u8		deName[8];     		// �ļ���
		//u8		deExtension[3];		// ��չ��
	union{
		struct{
				u8	Name[8];        // �ļ���
#define SLOT_EMPTY      0x00   		// Ŀ¼��Ϊ��
#define SLOT_E5         0x05   		 
#define SLOT_DELETED    0xe5   		// �ļ��ѱ�ɾ��
				u8	Ext[3];			// ��չ��
			}Name;
			u8 FullName[11];
		}Name;//�ļ���
		u8		deAttributes;  		// �ļ�����
#define ATTR_NORMAL     0x00   		// ��д
#define ATTR_READONLY   0x01   		// ֻ��
#define ATTR_HIDDEN     0x02   		// ����
#define ATTR_SYSTEM     0x04   		// ϵͳ�ļ�
#define ATTR_VOLUME     0x08   		// ����ļ�
#define ATTR_LONG_FILENAME	0x0f	// ���ļ����ļ�			    
#define ATTR_DIRECTORY  0x10    	// ��Ŀ¼�ļ�
#define ATTR_ARCHIVE    0x20    	// �鵵�ļ�
		u8        deLowerCase;  	// ϵͳ����
#define LCASE_BASE      0x08    	  
#define LCASE_EXT       0x10    	  
		u8        deCHundredth; 	// �ļ�����ʱ���10MS
		u8        deCTime[2];   	// �ļ�����ʱ��
		u8        deCDate[2];   	// �ļ���������
		u8        deADate[2];   	// �ļ������������
		u16       deHighClust; 		// �ļ���ʼ�غŵĸ�16λ
		u8        deMTime[2];   	// �ļ�����޸�ʱ��
		u8        deMDate[2];   	// �ļ�����޸�����
		u16       deStartCluster;   // �ļ���ʼ�غŵĵ�16λ
		u32       deFileSize;  	    // �ļ�����
} DIREntry;

// һ�������е�Ŀ¼����
#define DIRENTRIES_PER_SECTOR	0x10

// ���ļ�����ڵ�ַ�ṹ
typedef struct LONG_FILE_NAME{
		u8		weCnt;                //���
#define WIN_LAST        0x40            
#define WIN_CNT         0x3f
		u8		wePart1[10];          //���ļ���UNICODE�루��һ���֣�
		u8		weAttributes;         //���ļ���Ŀ¼���־
#define ATTR_WIN95      0x0f
		u8		weReserved1;          //ϵͳ����              
		u8		weChksum;             //У��ֵ
		u8		wePart2[12];          //���ļ���UNICODE�루�ڶ����֣�
		u16     weReserved2;          //�ļ���ʼ�غ�
		u8		wePart3[4];           //���ļ���UNICODE�루�������֣�
} LongFileName;

#define WIN_CHARS	13      //���ļ�����ڵ�ַ�ṹ�ɰ���13���ַ� 

//��FAT32�У��ļ�����ɵ�255���ַ�
#define FAT_MAX_FILE_NAME_LEN      255

// Ŀ¼���ļ���ڵ�ַ�ṹ�е�ʱ��ṹ

#define DT_2SECONDS_MASK        0x1F    // ��
#define DT_2SECONDS_SHIFT       0
#define DT_MINUTES_MASK         0x7E0   // ��
#define DT_MINUTES_SHIFT        5
#define DT_HOURS_MASK           0xF800  // ʱ
#define DT_HOURS_SHIFT          11

// Ŀ¼���ļ���ڵ�ַ�ṹ�е����ڽṹ

#define DD_DAY_MASK             0x1F    // ��
#define DD_DAY_SHIFT            0
#define DD_MONTH_MASK           0x1E0   // ��
#define DD_MONTH_SHIFT          5
#define DD_YEAR_MASK            0xFE00  // �� - 1980
#define DD_YEAR_SHIFT           9

typedef struct FIND_FILE_INFO{
	u32	Clust; //��ǰĿ¼�Ĵغ�
	u16	Nums;   //��FAT12��FAT16�еĸ�Ŀ¼����Ч
	u8	Index;  //��ǰ��BLOCK��Ŀ¼������
	u32	Sector; //��ǰĿ¼���ڵ�������
	u8	NumSector;

}FindFileInfo;

typedef union{
	 u32   Sector;    //FAT12&FAT16��Ч
	 u32   Clust;     //FAT32ʱ��Ч
}ROOTDIR_INF;

typedef struct _FILE{
	union 
	{
		struct
		{
			u8	Name[8];          // �ļ���
			u8	Ext[3]; // ��չ��
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
	}Attributes;       // �ļ�����
	u32 StartCluster;     // �ļ���ʼ�غ�
	u32	FileSize;  	    // �ļ�����
	u32	Position;
	u32 CurrentCluster;
}File;
extern u8 FatBuffer[512];
// �ɵ�����   
u8 fatInit( void);
/******************************************************
���ļ����в����ļ���Ŀ¼
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	��Ҫ���ҵ��ļ����ļ�������չ��
fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
withName��Ϊtrueͨ���ļ������ҷ���ͨ������
return	���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FatFindFile(Cluster DirClust,File* file,u16 fileIndex,bool withName);
/******************************************************
���ļ����в����ļ���Ŀ¼
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool OpenFileWithIndex(Cluster DirClust,File* file,u16 fileIndex);
/******************************************************
���ļ����в����ļ���Ŀ¼
dirCluster��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	  ��Ҫ���ҵ��ļ����ļ�������չ��
return	  ���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool OpenFileWithName(Cluster dirCluster,File *file);
/******************************************************
��ȡ�ļ�����һ��������512�ֽڣ�
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
return���ɹ�����True�������ļ���ĩβ����false
******************************************************/
bool FatReadSector(File* file,u8* buffer);
/******************************************************
�½��ļ���Ŀ¼
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileName���ļ���
Attributes���ļ�����
return	���ɹ�����True
******************************************************/
bool FatNewFile(Cluster DirClust,char fileName[11],u8 Attributes);
#endif//_SYSTEM_FS_FAT_H
