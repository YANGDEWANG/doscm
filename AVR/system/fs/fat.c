/************************************************************/
/*		         	FAT 			 					    */
/*				  											*/
/************************************************************/
#include <global.h>
#include <Clock.h>
#include <string.h>
#include <dev/mmc.h>
#include <avr/pgmspace.h>
#include "fs/fat.h"
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
void ReadBlock(u32 lba) 
{
	FatBufferLBA = lba;
	MMCReadSector(lba,FatBuffer);
}
bool WriteBlock(u32 lba) 
{
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
��ѯ������һ���ؿ�ʼ������
-----------------------------------------------------------------------*/
u32 fatClustToSect(u32 clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;//�ر�Ŵ�2��ʼ
}
static u32 getFatByteOffsetInFatTable(Cluster cluster)//����FAT���ֽ�ƫ��
{
	cluster = cluster << FatType;
	if(FatType==FAT12)cluster+=cluster>>1;
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
	b16 fat12Mask;
	b16 fat12;
	fatOffset = fatIndex << FatType;
	if(FatType==FAT12)fatOffset+=fatIndex>>1;
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
	b16 fat12Cluater;
	bool fat12NextIsHbyte=false;
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
	PartRecord PartInfo;
	BPB710 *bpb=0;
	ReadBlock(0);      // ��ȡ��������Ϣ  
	PartInfo = *((PartRecord *) ((PartSector *)FatBuffer)->psPart);
	// ������������PartInfo.prStartLBA��
	ReadBlock(PartInfo.prStartLBA);  //ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
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
	case PART_TYPE_FAT12:  
		RootDir.Sector=    FirstDataSector;
		FirstDataSector += ((bpb->bpbRootDirEnts*32)/BytesPerSector);
		RootDirEnts = bpb->bpbRootDirEnts;
		FatType = FAT12;  
		FAT_MASK=FAT12_MASK;
		break;
	case PART_TYPE_DOSFAT16:
	case PART_TYPE_FAT16: 
	case PART_TYPE_FAT16LBA:
		RootDir.Sector =   FirstDataSector;
		FirstDataSector += (bpb->bpbRootDirEnts*32)/BytesPerSector;
		RootDirEnts = bpb->bpbRootDirEnts;
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
		return 0;
	}  
	BytesPerClust = SectorsPerCluster*BytesPerSector;
#ifdef FAT_USE_FILE_BUFFER
	memcpy_P(TempFile.Name.FullName,PSTR(FILE_BUFFER_NAME),11);
	FileNew(0,TempFile.Name.FullName,FILE_ATTR_HIDDEN);
	FileOpenWithName(0,&TempFile);
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
enum FIND_FILE_WITH
{
	FFW_Index,
	FFW_Name,
	FFW_IsEmpty,
};
struct FIND_FILE_PARAMETER
{
	u8 FindWith;//FIND_FILE_WITH
	Cluster DirClust;//�ļ�����Ŀ¼�����
	union
	{
		union 
		{
			struct
			{
				u8	Name[8];          // �ļ���
				u8	Ext[3]; // ��չ��
			}Name;
			u8 FullName[11];
		}Name;//�����ļ�������
		u16 Index;//�����ļ������������ڱ���Ŀ¼
	}Parameter;
	u8 FileIndexInFatBuffer;//���ҽ�������Ƿ��ϲ����������ļ���FatBuffer�е�����
	u32 FileDirEntrySector;//���ҽ�������Ƿ��ϲ����������ļ�Ŀ¼�����ڵ�����
}FindFileParameter;
//ע�⣺�˷���ʹ��FatBuffer
bool static FileFind()//���ò������FindFileParameter�г�����Ϊ[���ҽ��]������ֶ�
{
	u32 DirSector;
	DIREntry *dir;
	u8 i;
	u8 SectorC = SectorsPerCluster;
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
			fname = *dir->Name.FullName;
			if((fname==SLOT_EMPTY)
				||(fname==SLOT_DELETED))
			{//�ջ�ɾ��
				if(FindFileParameter.FindWith==FFW_IsEmpty)
				{
					goto su;
				}
			}
			else if((dir->Attributes&FILE_ATTR_LONG_FILENAME)!=FILE_ATTR_LONG_FILENAME)
			{
				switch(FindFileParameter.FindWith)
				{
				case FFW_Name:
					{
						if(strncasecmp(dir->Name.FullName,FindFileParameter.Parameter.Name.FullName,11)==0)
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
				}
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
///******************************************************
//���ļ����в����ļ���Ŀ¼
//DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
//file	��Ҫ���ҵ��ļ����ļ�������չ��
//fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
//withName��Ϊtrueͨ���ļ������ҷ���ͨ������
//return	���鵽�ļ�����True������дfile�е������ֶ�
//******************************************************/
//bool FatFindFile(Cluster DirClust,File* file,u16 fileIndex,bool withName)
//{
//	Cluster DirSector;
//	DIREntry *dir;
//	u8 i;
//	u8 SectorC = SectorsPerCluster;
//	u8 fname;
//	if(DirClust==0)
//	{
//		if(FatType==FAT32)
//		{
//			DirClust = RootDir.Clust;
//			DirSector = fatClustToSect(DirClust);
//		}
//		else
//		{
//			DirSector = RootDir.Sector;
//			SectorC = RootDirEnts*sizeof(DIREntry)/BytesPerSector;
//		}
//	}
//	else
//	{
//		DirSector = fatClustToSect(DirClust);
//	}
//	do
//	{
//		ReadBlock(DirSector);
//		dir = (DIREntry *)FatBuffer;
//		for(i=0;i<16;i++)
//		{
//			fname = *dir->deName;
//			if((fname!=SLOT_EMPTY)
//				&&(fname!=SLOT_DELETED)
//				//&&(fname!='.')
//				&&((dir->deAttributes&ATTR_LONG_FILENAME)!=ATTR_LONG_FILENAME))
//			{
//				if(withName)
//				{
//					if(strncasecmp(dir->deName,file->Name,8)==0
//						&&strncasecmp(dir->deExtension,file->ExtensionName,3)==0)
//					{
//						goto su;
//					}
//				}
//				else
//				{
//					if(fileIndex==0)
//					{
//						goto su;
//					}
//					fileIndex--;
//				}
//			}
//			dir++;
//		}
//		SectorC--;
//		if(SectorC==0)
//		{
//			if(DirClust==0&&FatType!=FAT32)
//			{
//				return false;
//			}
//			else
//			{
//				DirClust = fatNextCluster(DirClust);
//				if(DirClust!=CLUST_EOFE)
//				{
//					DirSector = fatClustToSect(DirClust);
//					SectorC = SectorsPerCluster;
//				}
//			}
//		}
//		else
//		{
//			DirSector++;
//		}
//	}
//	while(true);
//su:
//	memcpy(file->Name,dir->deName,8);
//	memcpy(file->ExtensionName,dir->deExtension,8);
//	file->Attributes.Attribute=dir->deAttributes;
//	file->StartCluster  = ((u32)(dir->deHighClust)<<16)|dir->deStartCluster;
//	file->Size = dir->deFileSize;
//	file->CurrentCluster=file->StartCluster;
//	file->Position=0;
//	return true;
//
//}

static void FileInit(File* file)
{
	DIREntry *dir=((DIREntry *)(FatBuffer))+FindFileParameter.FileIndexInFatBuffer;
	memcpy(file->Name.FullName,dir->Name.FullName,11);
	file->Attributes.Attribute=dir->Attributes;
	file->StartCluster  = ((u32)(dir->HighClust)<<16)|dir->StartCluster;
	file->Size = dir->Size;
	file->CurrentCluster=file->StartCluster;
	file->Position=0;
	file->DirEntrySector = FindFileParameter.FileDirEntrySector;
	file->DirEntryIndex = FindFileParameter.FileIndexInFatBuffer;
}
/******************************************************
���ļ����в����ļ���Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool FileOpenWithIndex(Cluster DirClust,File* file,u16 fileIndex)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Index;
	FindFileParameter.Parameter.Index = fileIndex;
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
}
/******************************************************
����Ŀ¼���Ƿ�����ļ�
ע�⣺�˷���ʹ��FatBuffer
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileName��Ҫ���ҵ��ļ���
return	����淵��True
******************************************************/
bool FileExist(Cluster DirClust,char fileName[11])
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Name;
	memcpy(FindFileParameter.Parameter.Name.FullName,fileName,11);
	return FileFind();
}
/******************************************************
���ļ����в����ļ���Ŀ¼
ע�⣺�˷���ʹ��FatBuffer
dirCluster��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	  ��Ҫ���ҵ��ļ����ļ�������չ��
return	  ���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FileOpenWithName(Cluster DirClust,File *file)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Name;
	memcpy(FindFileParameter.Parameter.Name.FullName,file->Name.FullName,11);
	if(FileFind())
	{
		FileInit(file);
		return true;
	}
	return false;
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
	ReadBlockToBuff(sector+getFileSectorClustoffset(file),buffer);
	return true;
}
/******************************************************
��ȡ�ļ���Position��ָ������512�ֽڣ�����Position����512�ֽ�
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
return���ɹ�����True�������ļ���ĩβ����false
******************************************************/
bool FatReadSector(File* file,u8 buffer[512])
{
	if(FatPeekSector(file,buffer))
	{

		if(getFileSectorClustoffset(file)>=(SectorsPerCluster-1))
		{
			if(!fatNextCluster(&(file->CurrentCluster)))
			{
				return false;
			}
		}
		file->Position+=512;
		if(file->Position>file->Size)
		{
			file->Position=file->Size;
		}
		return true;
	}
	return false;
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
bool FatWriteSector(File* file,u8 buffer[512])
{

	if(getFileClustCount(file->Size)*BytesPerClust<(file->Position+512))
	{

#ifdef FAT_USE_FILE_BUFFER
		WriteBlock(TempFileStartSector);
#endif//FAT_USE_FILE_BUFFER
		FATAssignNewClust(file);
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
			dir->Attributes=Attributes;
			dir->CTime = getFileTime();
			dir->CDate = getFileDate();
			dir->MTime = getFileTime();
			dir->MDate = getFileDate();
			dir->ADate = getFileDate();
			memcpy(dir,fileName,11);
			WriteBlock(FatBufferLBA);
			return WriteBlock(FatBufferLBA+FatSectorCount);
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
	ReadBlock(file->DirEntrySector);
	DIREntry *dir=(DIREntry *)(FatBuffer+sizeof(DIREntry)*file->DirEntryIndex);
	memcpy(dir->Name.FullName,file->Name.FullName,11);
	dir->Attributes=file->Attributes.Attribute;
	dir->HighClust = file->StartCluster>>16;
	dir->StartCluster = file->StartCluster;
	dir->Size=file->Size;
	dir->MTime = getFileTime();
	dir->MDate = getFileDate();
	return WriteBlock(file->DirEntrySector);
}
bool FileDelete(Cluster DirClust,char fileName[11])
{
	File file;
	memcpy(&file.Name.FullName,fileName,sizeof(fileName));
	if(FileOpenWithName(DirClust,&file))
	{
		if(file.CurrentCluster!=0)
		{
			while(fatNextCluster(&file.CurrentCluster))
			{
				fatSetFAT(file.StartCluster,0);
				file.StartCluster = file.CurrentCluster;
			}
			fatSetFAT(file.StartCluster,0);
		return true;
		}
	}
	return false;
}

bool FileSetPosition(File* file,u32 Position)
{
	if(file->Size>Position)
	{
		file->CurrentCluster = file->StartCluster;
		while(Position>BytesPerClust)
		{
			fatNextCluster(&file->CurrentCluster);
		}
		file->Position = Position;
		return true;
	}
	return false;
}

