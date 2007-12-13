/************************************************************/
/*		         	FAT 			 					    */
/*				  											*/
/************************************************************/
#include <global.h>
#include <Clock.h>
#include <string.h>
#include <dev/mmc.h>
#include <avr/pgmspace.h>
#include <fs/fat.h>
#include <fs/path.h>
u8 FatBuffer[512];
extern u8 WorkFlag;
u8	FatType;  //0:FAT12  1:FAT16   2:FAT32
u32 FAT_MASK;
u16	RootDirEnts;       //��Ŀ¼�ܵ�����(FAT12&FAT16)
u32 FirstDataSector;   //������
u16	BytesPerSector;
u16	SectorsPerCluster;
u32 FirstFATSector;
u32 FatSectorCount;
u32 BytesPerClust;
ROOTDIR_INF	RootDir;  //Ŀ¼��
//FindFileInfo	FindInfo;
//********************************************************************************************
u32 FatBufferLBA;
#ifdef FAT_USE_FILE_BUFFER
u32 TempFileStartSector;
#endif//FAT_USE_FILE_BUFFER
//��һ������
//#define DEV_ERROR_RETRY  3
bool ReadBlock(u32 lba) 
{
	FatBufferLBA = lba;
	return MMCReadSector(lba,FatBuffer);
	//u8 i = DEV_ERROR_RETRY;
	//while(i--&&(!));
}
bool WriteBlock(u32 lba) 
{
	//u8 i = DEV_ERROR_RETRY;
	//while(i--)
	//{
	//	if()
	//		return true;
	//}
	return MMCWriteSector(lba,FatBuffer);
}
#define ReadBlockToBuff(LBA,pbuff) MMCReadSector(LBA,pbuff)
#define WriteBlockFormBuff(LBA,pbuff) MMCWriteSector(LBA,pbuff)

//bool ReadBlockToBuff(lba,pbuff)
//{
//	if(lba==FatBufferLBA&&pbuff==FatBuffer)
//	{
//		return true;
//	}
//	FatBufferLBA = lba;
//	return	MMCReadSector(lba,pbuff);
//}
//bool WriteBlockFormBuff(lba,pbuff)
//{
//	if(pbuff==FatBuffer)
//		FatBufferLBA = lba;
//	MMCWriteSector(lba,pbuff);
//}
//bool ReadBlock(u32 lba) 
//{
//	ReadBlockToBuff(lba,FatBuffer);
//}
//bool WriteBlock(u32 lba) 
//{
//	return WriteBlockFormBuff(lba,FatBuffer);
//}


/*-----------------------------------------------------------------------
��ѯ�صĿ�ʼ������
-----------------------------------------------------------------------*/
u32 fatClustToSect(u32 clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;//�ر�Ŵ�2��ʼ
}
/*-----------------------------------------------------------------------
���������ڵĴ�
���Sectδָ��������������0
-----------------------------------------------------------------------*/
u32 FatSectToClust(u32 Sect)
{
	if(Sect<FirstDataSector)
		return 0;
	else
		return (Sect-FirstDataSector)/SectorsPerCluster+2;//�ر�Ŵ�2��ʼ
}
static u32 getFatByteOffsetInFatTable(Cluster cluster)//����FAT���ֽ�ƫ��
{
	cluster = cluster << FatType;
#if FAT12_EN>0
	if(FatType==FAT12)cluster+=cluster>>1;
#endif
	return cluster;
}
static u32 getFatSectorOffset(Cluster cluster)//����FAT������
{
	return FirstFATSector + (getFatByteOffsetInFatTable(cluster) / BytesPerSector);	
}
static u16 getFatBytesOffsetInSector(Cluster cluster)//����FAT�������б����ƫ�Ƶ�ַ
{
	return getFatByteOffsetInFatTable(cluster)% BytesPerSector; 
}
void static fatSetFAT(u32 fatIndex,u32 fatData)
{
	u32 fatOffset;
	u32 sector;
	u16 offset;
	//if(fatIndex<2)
	//	return;
	fatOffset = fatIndex << FatType;
#if FAT12_EN>0
	b16 fat12Mask;
	b16 fat12;
	if(FatType==FAT12)fatOffset+=fatIndex>>1;
#endif
	sector = FirstFATSector + (fatOffset / BytesPerSector);	//����FAT������
	offset = fatOffset % BytesPerSector; //����FAT�������б����ƫ�Ƶ�ַ
	ReadBlock(sector);
	switch(FatType)
	{
	case FAT32:
		{
			(*(u32*)(FatBuffer+offset)) = fatData&FAT32_MASK;
			break;
		}
	case FAT16:
		{
			(*(u16*)(FatBuffer+offset)) = fatData&FAT16_MASK;
			break;
		}
#if FAT12_EN>0
	case FAT12:
		{

			fat12.b16_1 = fatData&FAT12_MASK;
			fat12Mask.b16_1 = 0xf00;
			if(fatIndex&0X00000001)
			{   //ȡ��12λ��ַ
				fat12.b16_1<<=4;
				fat12Mask.b16_1 = 0xf;
			}
			// һ������Ϊ1.5bytes(12 bits)
			if(offset==(BytesPerSector-1))
			{
				FatBuffer[offset]&=fat12Mask.b8_2.b8_0;
				FatBuffer[offset]|=fat12.b8_2.b8_0;
				WriteBlock(FatBufferLBA);
				WriteBlock(FatBufferLBA+FatSectorCount);
				ReadBlock(sector+1);
				FatBuffer[0]&=fat12Mask.b8_2.b8_1;
				FatBuffer[0]|=fat12.b8_2.b8_1;
			}
			else
			{
				(*(u16*)(FatBuffer+offset))&=fat12Mask.b16_1;
				(*(u16*)(FatBuffer+offset))|=fat12.b16_1;

			}
			break;
		}
#endif

	}
	WriteBlock(FatBufferLBA);
	WriteBlock(FatBufferLBA+FatSectorCount);
}


/*-----------------------------------------------------------------------
��FAT���в�ѯ��һ��δʹ�ôغ�
ע�⣺�˷���ʹ��FatBuffer
cluster���Ӵ˴ؿ�ʼ����
return �����ҵ��Ĵغţ��������CLUST_EOFE��ʾcluster��û�пմؿ���
-----------------------------------------------------------------------*/
u32 fatNextEmptyCluster(u32 cluster)
{
	//cluster++;
	u32 sector = getFatSectorOffset(cluster);
	u16 offset = getFatBytesOffsetInSector(cluster);
#if FAT12_EN>0
	b16 fat12Cluater;
	bool fat12NextIsHbyte=false;
#endif//#if FAT12_EN>0
	do
	{
		if(sector - FirstFATSector>=FatSectorCount)//����Fat������
			return CLUST_EOFE;
		ReadBlock(sector++);
		while(offset<BytesPerSector)
		{
			switch(FatType)
			{
			case FAT32:
				{
					if((*(u32*)(FatBuffer+offset))==0)
					{
						//(*(u32*)(FatBuffer+offset)) = CLUST_EOFS&FAT32_MASK;
						//WriteBlock(FatBufferLBA);
						//WriteBlock(FatBufferLBA+FatSectorCount);
						return cluster;
					}
					else
						offset+=4;
					break;
				}
			case FAT16:
				{
					if((*(u16*)(FatBuffer+offset))==0)
					{
						//(*(u16*)(FatBuffer+offset)) = CLUST_EOFS&FAT16_MASK;
						//WriteBlock(FatBufferLBA);
						//WriteBlock(FatBufferLBA+FatSectorCount);
						return cluster;
					}
					else
						offset+=2;
					break;
				}
#if FAT12_EN>0
			case FAT12:
				{
					if(fat12NextIsHbyte)
					{
						fat12NextIsHbyte = false;
						fat12Cluater.b8_2.b8_1 = FatBuffer[offset];
					}
					else
					{
						fat12Cluater.b8_2.b8_0 = FatBuffer[offset];
						if(offset!=(BytesPerSector-1))
						{						
							fat12Cluater.b8_2.b8_1 = FatBuffer[offset+1];
						}
						else
						{
							fat12NextIsHbyte = true;
							break;
						}
						offset++;
					}

					if(cluster&1)
					{   //ȡ��12λ��ַ
						fat12Cluater.b16_1>>=4;
						offset++;
					}
					else
					{  //ȡ��12λ��ַ
						fat12Cluater.b16_1&=0xfff;
					}
					if(fat12Cluater.b16_1==0)
					{

						return cluster;
					}
					break;
				}
#endif//#if FAT12_EN>0
			}
			cluster++;
		}
		offset = 0;
	}while(1);
} 

/*-----------------------------------------------------------------------
��FAT���в�ѯ��һ���غ�
����鵽������cluster���򱣳�cluster���䷵��false
-----------------------------------------------------------------------*/
bool fatNextCluster(Cluster *cluster)
{
	u32 nextCluster=*cluster;
	if(nextCluster<2)
		return false;
	u32 sector = getFatSectorOffset(nextCluster);
	u16 offset = getFatBytesOffsetInSector(nextCluster);
	if(FatType==FAT32 )	// һ������Ϊ4bytes(32 bits)
	{
		nextCluster=MMCReadu32(sector,offset);
	}
	else if(FatType==FAT16)
	{    // һ������Ϊ2bytes(16 bits)
		nextCluster=MMCReadu16(sector,offset);
	}
#if FAT12_EN>0
	else if(FatType==FAT12)
	{	// һ������Ϊ1.5bytes(12 bits)
		if(offset==(BytesPerSector-1))
		{
			nextCluster=MMCReadu8(sector,offset);
			nextCluster|=((u16)MMCReadu8(sector+1,0)<<8);
		}
		else
		{
			nextCluster=MMCReadu16(sector,offset);
		}
		if(*(u8*)cluster&0X00000001)
		{   //ȡ��12λ��ַ
			nextCluster>>=4;
		}
		else
		{  //ȡ��12λ��ַ
			nextCluster&=0x00000fff;
		}
	}
#endif//#if FAT12_EN>0
	// �Ƿ��ļ��Ľ�����
	if (nextCluster < (CLUST_EOFS & FAT_MASK))
	{
		*cluster = nextCluster;
		return true;
	}
	return false;
} 
static bool FATAssignNewClust(File* file)
{
	u32 nextE = file->CurrentCluster;
	if(!fatNextCluster(&nextE))
	{
		nextE = fatNextEmptyCluster(file->CurrentCluster);
		if(nextE==CLUST_EOFE)
			nextE = fatNextEmptyCluster(2);
		if(nextE==CLUST_EOFE)
			return false;
		if(file->CurrentCluster==0)
		{
			file->CurrentCluster = nextE;
			file->StartCluster = nextE;
		}
		else
		{
			fatSetFAT(file->CurrentCluster,nextE);
		}
		fatSetFAT(nextE,CLUST_EOFE);
	}
	return true;
}
//�ļ�ϵͳ��ʼ��
//ע�⣺�˷���ʹ��FatBuffer
u8 FATInit()
{ 
#ifdef FAT_USE_FILE_BUFFER
	File TempFile;
#endif//FAT_USE_FILE_BUFFER
	//bool onfenqu = false;
	PartRecord PartInfo;
	BPB710 *bpb;
//re:
	ReadBlock(0);      // ��ȡ��������Ϣ  
	//if(!onfenqu)
	{
		PartInfo = *((PartRecord *) ((PartSector *)FatBuffer)->psPart);
		// ������������PartInfo.prStartLBA��
		ReadBlock(PartInfo.prStartLBA);  //ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	}
	bpb = (BPB710 *) &((BootSector710 *) FatBuffer)->bsBPB;
	FirstDataSector	= PartInfo.prStartLBA;
	if(bpb->bpbFATsecs)
	{
		// bpbFATsecs��0,ΪFAT16,FAT����ռ����������bpbFATsecs��
		FatSectorCount = bpb->bpbFATsecs;
	}
	else
	{
		// bpbFATsecs��0,ΪFAT32,FAT����ռ����������bpbBigFATsecs��
		FatSectorCount = bpb->bpbBigFATsecs;
	}
	FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * FatSectorCount; 
	SectorsPerCluster	= bpb->bpbSecPerClust;
	BytesPerSector		= bpb->bpbBytesPerSec;
	FirstFATSector		= bpb->bpbResSectors + PartInfo.prStartLBA;
	switch (PartInfo.prPartType)
	{
#if FAT12_EN>0
	case PART_TYPE_FAT12:  
		RootDir.Sector=    FirstDataSector;
		FirstDataSector += ((bpb->bpbRootDirEnts*32)/BytesPerSector);
		RootDirEnts = bpb->bpbRootDirEnts;
		FatType = FAT12;  
		FAT_MASK=FAT12_MASK;
		break;
#endif//#if FAT12_EN>0
	case PART_TYPE_DOSFAT16:
	case PART_TYPE_FAT16: 
	case PART_TYPE_FAT16LBA:
		RootDir.Sector =   FirstDataSector;
		RootDirEnts = bpb->bpbRootDirEnts;
		FirstDataSector += (RootDirEnts*32)/BytesPerSector;
		FatType = FAT16; 
		FAT_MASK=FAT16_MASK;	
		break;
	case PART_TYPE_FAT32LBA:
	case PART_TYPE_FAT32:
		RootDir.Clust = bpb->bpbRootClust;
		//RootDir.Sector= fatClustToSect(bpb->bpbRootClust);
		FAT_MASK=FAT32_MASK;
		FatType = FAT32; 
		break;
	default:
		{
			/*if(!onfenqu)
			{
				onfenqu = true;
				goto re;
			}*/
			return false;
		}
	}  
	BytesPerClust = SectorsPerCluster*BytesPerSector;
#ifdef FAT_USE_FILE_BUFFER
	memcpy_P(TempFile.Name.FullName,PSTR(FILE_BUFFER_NAME),11);
	FileNew(0,TempFile.Name.FullName,FILE_ATTR_HIDDEN);
	FileOpenWithFullName(0,&TempFile);
	FATAssignNewClust(&TempFile);
	TempFile.Size = BytesPerClust;
	TempFileStartSector = fatClustToSect(TempFile.StartCluster);
	return FileSave(&TempFile);
#endif//FAT_USE_FILE_BUFFER

	return true;	
}

//bool static FatFileNameComp(char const *a,char const *b)
//{
//	char aa[9],ba[9];
//	aa[8]=0;
//	ba[8]=0;
//	memcpy(aa,a,8);
//	memcpy(ba,b,8);
//	strcasecmp(aa,ba);
//}
//bool static FatFileNameExtComp(char const *a,char const *b)
//{}

//ע�⣺�˷���ʹ��FatBuffer
enum FIND_FILE_WITH//��Index�����㿪ʼ��������
{
	FFW_Index,			//���ҵ�Index���ļ�
	FFW_FullName,		//�����ļ�������չ��ΪFullName���ļ�
	FFW_IsEmpty,		//���ҿ�Ŀ¼��
	FFW_IndexAndIsDir,	//���ҵ�Index��Ŀ¼
	FFW_StartCluster,	//����Ŀ¼��ʼ���ΪStartCluster���ļ�
#if FFW_INDEXANDEXTNAME_EN>0
	FFW_IndexAndExtName,//���ҵ�Index����չ��ΪExt���ļ�
#endif//FFW_INDEXANDEXTNAME_EN
};
struct find_file_parameter
{
	u8 FindWith;						//���ҷ�ʽ
	Cluster DirClust;					//�ļ�����Ŀ¼�����
	struct
	{
		union 
		{
			struct
			{
				u8	Name[8];			// �ļ���
				u8	Ext[3];				// ��չ��
			}Name;
			u8 FullName[11];
			u32 StartCluster;			//��ʼ���
		}Parameter;						//�����ļ�������
		u16 Index;						//�����ļ������������ڱ���Ŀ¼
	}Parameter;
	u8 FileIndexInFatBuffer;			//���ҽ�������Ƿ��ϲ����������ļ���FatBuffer�е�����
	u32 FileDirEntrySector;				//���ҽ�������Ƿ��ϲ����������ļ�Ŀ¼�����ڵ�����
}FindFileParameter;
//ע�⣺�˷���ʹ��FatBuffer
bool static FileFind()//���ò������FindFileParameter�г�����Ϊ[���ҽ��]������ֶ�
{
	u32 DirSector;
	DIREntry *dir;
	u8 i;
	u8 SectorC = SectorsPerCluster;//Ŀ¼��������
	u8 fname;
	u16 fileIndex = FindFileParameter.Parameter.Index;
	Cluster DirClust=FindFileParameter.DirClust;
	if(DirClust==0)
	{//�ڸ�Ŀ¼
		if(FatType==FAT32)
		{
			DirClust = RootDir.Clust;
			DirSector = fatClustToSect(DirClust);
		}
		else
		{
			DirSector = RootDir.Sector;
			SectorC = RootDirEnts*sizeof(DIREntry)/BytesPerSector;
		}
	}
	else
	{
		DirSector = fatClustToSect(DirClust);
	}
	do
	{
		ReadBlock(DirSector);
		dir = (DIREntry *)FatBuffer;
		for(i=0;i<16;i++)
		{
			fname = *dir->FullName;
			if((fname==SLOT_EMPTY)
				||(fname==SLOT_DELETED))
			{//�ջ�ɾ��
				if(FindFileParameter.FindWith==FFW_IsEmpty)
				{
					goto su;
				}
			}
			else if((dir->Attributes.Attribute&FILE_ATTR_LONG_FILENAME)!=FILE_ATTR_LONG_FILENAME)
			{
				switch(FindFileParameter.FindWith)
				{
				case FFW_FullName:
					{
						if(strncasecmp((char*)dir->FullName,(char*)FindFileParameter.Parameter.Parameter.FullName,11)==0)
						{
							goto su;
						}
						break;
					}
				case FFW_Index:
					{
						if(fileIndex==0)
						{
							goto su;
						}
						fileIndex--;
						break;
					}
				case FFW_IndexAndIsDir:
					{
						if(dir->Attributes.Attributes.IsDirectory)
						{
							if(fileIndex==0)
							{
								goto su;
							}
							fileIndex--;
						}
						break;
					}
				case FFW_StartCluster:
					{
						//if(dir->StartCluster==FindFileParameter.Parameter.Parameter.StartCluster)
						if((((u32)(dir->HighClust)<<16)|dir->StartCluster)==FindFileParameter.Parameter.Parameter.StartCluster)
						{
							goto su;
						}
						break;
					}
#if FFW_INDEXANDEXTNAME_EN>0
				case FFW_IndexAndExtName:
					{
						if(strncasecmp((char*)dir->Ext,(char*)FindFileParameter.Parameter.Parameter.Name.Ext,3)==0)
						{
							if(fileIndex==0)
							{
								goto su;
							}
							fileIndex--;
						}
						break;
					}
				}
#endif//FFW_INDEXANDEXTNAME_EN
			}
			dir++;
		}
		SectorC--;
		if(SectorC==0)
		{
			if(DirClust==0&&FatType!=FAT32)
			{
				return false;
			}
			else
			{
				if(fatNextCluster(&DirClust))
				{
					DirSector = fatClustToSect(DirClust);
					SectorC = SectorsPerCluster;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			DirSector++;
		}
	}
	while(true);
su:
	FindFileParameter.FileIndexInFatBuffer = i;
	FindFileParameter.FileDirEntrySector = DirSector;
	return true;
}
static void FileInit(File* file)
{
	DIREntry *dir=((DIREntry *)(FatBuffer))+FindFileParameter.FileIndexInFatBuffer;
	memcpy(file->FullName,dir->FullName,11);
	file->Attributes.Attribute=dir->Attributes.Attribute;
	file->StartCluster  = ((u32)(dir->HighClust)<<16)|dir->StartCluster;
	file->Size = dir->Size;
	file->CurrentCluster=file->StartCluster;
	file->Position=0;
	file->DirEntrySector = FindFileParameter.FileDirEntrySector;
	file->DirEntryIndex = FindFileParameter.FileIndexInFatBuffer;
}
/******************************************************
�ж�Ŀ¼���Ƿ�����ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileName��Ҫ���ҵ��ļ���
return	����淵��True
******************************************************/
bool FileExist(Cluster DirClust,char fileName[11])
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_FullName;
	memcpy(FindFileParameter.Parameter.Parameter.FullName,fileName,11);
	return FileFind();
}
/******************************************************
��Ŀ¼�е�index���ļ���Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
index	��Ҫ�򿪵��ļ���Ŀ¼����
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool FileOpenWithIndex(Cluster DirClust,File* file,u16 index)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Index;
	FindFileParameter.Parameter.Index = index;
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}
/******************************************************
��Ŀ¼����ʼ���ΪStartCluster���ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file->StartCluster����ʼ���
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool FileOpenWithStartCluster(Cluster DirClust,File* file)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_StartCluster;
	FindFileParameter.Parameter.Parameter.StartCluster = file->StartCluster;
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}
/******************************************************
��Ŀ¼�е�index��Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
index	��Ҫ�򿪵�Ŀ¼����
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool DirOpenWithIndex(Cluster DirClust,File* file,u16 index)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_IndexAndIsDir;
	FindFileParameter.Parameter.Index = index;
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}

/******************************************************
�����ļ�������չ�����ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	��Ҫ�򿪵��ļ����ļ�������չ��
return	���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FileOpenWithFullName(Cluster DirClust,File *file)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_FullName;
	memcpy(FindFileParameter.Parameter.Parameter.FullName,file->FullName,11);
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}
#if FFW_INDEXANDEXTNAME_EN>0
/******************************************************
�򿪵�index����չ��ΪExt���ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	��Ҫ�򿪵��ļ�����չ��
index	��Ҫ�򿪵��ļ�����������0��ʼ��
return	���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FileOpenWithIndexAndExtName(Cluster DirClust,File *file,u16 index)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_IndexAndExtName;
	FindFileParameter.Parameter.Index = index;
	memcpy(FindFileParameter.Parameter.Parameter.Name.Ext,file->Ext,3);
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}
#endif//FFW_INDEXANDEXTNAME_EN
/******************************************************
�������·��path���ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
path	��Ҫ�򿪵��ļ������·��
return	���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FileOpenWithPath(Cluster DirClust,File *file,char* path)
{
	char name[12];
	do
	{
		path = PathGetDirName(name,path);
		memcpy(file->FullName,name,11);
		if(!FileOpenWithFullName(DirClust,file))
		{
			return false;
		}
		if(file->Attributes.Attributes.IsDirectory)
		{
			DirClust = file->StartCluster;
		}
		else
		{
			return true;
		}
		if(path==null)
			return true;
	}while(1);
}

///��ȡ�ļ�Position��CurrentCluster�е�Sectorƫ��
static u16 getFileSectorClustoffset(const File* file)
{
	return (file->Position%BytesPerClust)/BytesPerSector;
}
/******************************************************
��ȡ�ļ���Position��ָ������512�ֽڣ���������Position
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
return���ɹ�����True�������ļ���ĩβ����false
******************************************************/
bool FatPeekSector(File* file,u8* buffer)
{
	if(file->Size<=file->Position)
	{
		return false;
	}
	u32 sector = fatClustToSect(file->CurrentCluster);
	return ReadBlockToBuff(sector+getFileSectorClustoffset(file),buffer);
}
/******************************************************
��ȡ�ļ���Position��ָ������512�ֽڣ�����Position����512�ֽ�
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
return�����سɹ���ȡ�ֽ����������ļ���ĩβ����0
******************************************************/
u16 FileReadSector(File* file,u8 buffer[512])
{
	u16 readc = 0;
	if(FatPeekSector(file,buffer))
	{
		if(getFileSectorClustoffset(file)>=(SectorsPerCluster-1))
		{
			fatNextCluster(&(file->CurrentCluster));
			/*if(!))
			{
			return 0;
			}*/
		}
		file->Position+=512;
		if(file->Position>file->Size)
		{
			readc = 512-(file->Position-file->Size);
			file->Position=file->Size;
		}
		else
		{
			readc = 512;
		}
	}
	return readc;
}
static u32 getFileClustCount(u32 fileSize)
{
	u32 cls = fileSize/BytesPerClust;
	if(fileSize%BytesPerClust!=0)cls++;
	return cls;
}

/******************************************************
д�ļ���Position��ָ������512�ֽڣ�����Position����512�ֽ�
�����ļ���ĩβ�������ļ������������û�п��õĿ��彫����false
ע�⣺�˷���ʹ��FatBuffer��������buffer��FatBuffer���ã�����
������FAT_USE_FILE_BUFFER
file  ��Ҫд����ļ�
buffer������
return���ɹ�����True
******************************************************/
bool FileWriteSector(File* file,u8 buffer[512])
{

	if(getFileClustCount(file->Size)*BytesPerClust<(file->Position+512))
	{

#ifdef FAT_USE_FILE_BUFFER
		WriteBlock(TempFileStartSector);
#endif//FAT_USE_FILE_BUFFER
		if(!FATAssignNewClust(file))
		{
			return false;
		}
		//	file->Size += 512-file->Size%512;
#ifdef FAT_USE_FILE_BUFFER
		ReadBlock(TempFileStartSector);
#endif//FAT_USE_FILE_BUFFER
	}
	u32 sector = fatClustToSect(file->CurrentCluster);
	u16 Sectoroffset = (file->Position%BytesPerClust)/BytesPerSector;
	WriteBlockFormBuff(sector+Sectoroffset,buffer);
	file->Position+=512;
	if(file->Position>file->Size)
	{
		file->Size = file->Position;
	}
	if(Sectoroffset>=SectorsPerCluster-1)
	{
		if(!fatNextCluster(&file->CurrentCluster))
		{
			FATAssignNewClust(file);
			return fatNextCluster(&file->CurrentCluster);
		}
	}
	return true;
}
#ifdef DATE_TIME_U
static u16 getFileDate()
{
	return (Now.Year-1980)*512+Now.Month*32+Now.Day;
}
static u16 getFileTime()
{
	return  Now.Hour*2048+Now.Minute*32+Now.Second/2;
}
#else//DATE_TIME_U
#define getFileTime() Now_Hour*2048+Now_Minute*32+Now_Second/2
#define getFileDate() (Now_Year-1980)*512+Now_Month*32+Now_Day
#endif//DATE_TIME_U
/******************************************************
�½��ļ���Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileName���ļ���������ȫΪ��д��
Attributes���ļ�����
return	���ɹ�����True
******************************************************/
bool FileNew(Cluster DirClust,char fileName[11],u8 Attributes)
{
	DIREntry *dir;

	if(!FileExist(DirClust,fileName))
	{
		//	FindFileParameter.DirClust = DirClust;//FileExist�Ѿ�������
		FindFileParameter.FindWith=FFW_IsEmpty;
		if(FileFind())
		{

			dir=(DIREntry *)(FatBuffer+sizeof(DIREntry)*FindFileParameter.FileIndexInFatBuffer);
			memset(dir,0,sizeof(DIREntry));
			dir->Attributes.Attribute=Attributes;
			dir->CTime = getFileTime();
			dir->CDate = getFileDate();
			dir->MTime = getFileTime();
			dir->MDate = getFileDate();
			dir->ADate = getFileDate();
			memcpy(dir,fileName,11);
			return WriteBlock(FatBufferLBA);
			//WriteBlock(FatBufferLBA+FatSectorCount);remove at 2007-12-13//�ɵ����ļ���
		}
	}
	return false;
}
/******************************************************
�����ļ���Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
file��Ҫ������ļ�
return	���ɹ�����True
******************************************************/
bool FileSave(File* file)
{
	if(ReadBlock(file->DirEntrySector))
	{
		DIREntry *dir=(DIREntry *)(FatBuffer+sizeof(DIREntry)*file->DirEntryIndex);
		memcpy(dir->FullName,file->FullName,11);
		dir->Attributes.Attribute=file->Attributes.Attribute;
		dir->HighClust = file->StartCluster>>16;
		dir->StartCluster = file->StartCluster;
		dir->Size=file->Size;
		dir->MTime = getFileTime();
		dir->MDate = getFileDate();
		return WriteBlock(file->DirEntrySector);
	}
	return false;
}
bool FileDelete(Cluster DirClust,char fileName[11])
{
	File file;
	memcpy(file.FullName,fileName,sizeof(file.FullName));
	if(FileOpenWithFullName(DirClust,&file))
	{
		if(file.StartCluster!=0)
		{
			while(fatNextCluster(&file.CurrentCluster))
			{
				fatSetFAT(file.StartCluster,0);
				file.StartCluster = file.CurrentCluster;
			}
			fatSetFAT(file.StartCluster,0);
		}
		file.Name[0]=SLOT_DELETED;
		FileSave(&file);
		return true;
	}
	return false;
}

bool FileSetPosition(File* file,u32 Position)
{//todo:err
	if(file->Size>Position)
	{
		file->CurrentCluster = file->StartCluster;
		u32 p =BytesPerClust;
		while(Position>p)
		{

			if(!fatNextCluster(&file->CurrentCluster))
				return false;
			p+=BytesPerClust;
		}
		file->Position = Position;
		return true;
	}
	return false;

	//if(file->Size>Position)
	//{
	//	file->CurrentCluster = file->StartCluster;
	//	while(Position>BytesPerClust)
	//	{
	//		fatNextCluster(&file->CurrentCluster);
	//	}
	//	file->Position = Position;
	//	return true;
	//}
	//return false;
}


/******************************************************
��ȡ�ļ�
ע�⣺�˷���ʹ��FatBuffer�����Ҫ����buffer��FatBuffer
�����
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
count ��������ȡ���ֽ���
return���ɹ���ȡ���ֽ���
******************************************************/
u16 FileRead(File* file,u8* buffer,u16 count)
{

	u16 offset = file->Position%512;
	u16 cpycount = 512-offset;
	u16 readedCount = 0;
	if(cpycount>count)
	{
		cpycount=count;
	}
	while(1)
	{
		if(FatPeekSector(file,FatBuffer))
		{
			memcpy(buffer,FatBuffer+offset,cpycount);
			offset =0;
			buffer+=cpycount;
			file->Position+=cpycount;
			readedCount+=cpycount;
			if(file->Position>=file->Size)
			{
				readedCount -= file->Position-file->Size;
				file->Position = file->Size;
				break;
			}
			if(readedCount>=count)
			{
				return count;
			}
			cpycount = count-readedCount;
			if(cpycount>512)
			{
				cpycount = 512;
			}
		}
		else
		{
			break;
		}
	}
	return readedCount;
}
