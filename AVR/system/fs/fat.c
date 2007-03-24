/************************************************************/
/*		         	FAT 			 					    */
/*				  											*/
/************************************************************/
#include <global.h>
#include <string.h>
#include <card/mmc/mmc.h>
#include "fat.h"
u8 FatBuffer[512];
extern u8 WorkFlag;
u8	FatType;  //0:FAT12  1:FAT16   2:FAT32
u32 FAT_MASK;
u16	RootDirEnts;       //根目录总的项数(FAT12&FAT16)
u32 FirstDataSector;   //数据区
u16	BytesPerSector;
u16	SectorsPerCluster;
u32 FirstFATSector;
u32 FatSectorCount;
ROOTDIR_INF	RootDir;  //目录区
//FindFileInfo	FindInfo;
//********************************************************************************************
u32 FatBufferLBA;
//读一个扇区
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

/*-----------------------------------------------------------------------
查询数据区一个簇开始扇区号
-----------------------------------------------------------------------*/
u32 fatClustToSect(u32 clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;//簇编号从2开始
}
/*-----------------------------------------------------------------------
查询一个簇所占字节数
-----------------------------------------------------------------------*/
u32 fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster*BytesPerSector;
}

//文件系统初始化
u8 fatInit()
{ 
	PartRecord PartInfo;
	BPB710 *bpb=0;
	ReadBlock(0);      // 读取分区表信息  
	PartInfo = *((PartRecord *) ((PartSector *)FatBuffer)->psPart);
	// 引导扇区号在PartInfo.prStartLBA中
	ReadBlock(PartInfo.prStartLBA);  //ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	bpb = (BPB710 *) &((BootSector710 *) FatBuffer)->bsBPB;
	FirstDataSector	= PartInfo.prStartLBA;
	if(bpb->bpbFATsecs)
	{
		// bpbFATsecs非0,为FAT16,FAT表所占的扇区数在bpbFATsecs里
		FatSectorCount = bpb->bpbFATsecs;
	}
	else
	{
		// bpbFATsecs是0,为FAT32,FAT表所占的扇区数在bpbBigFATsecs里
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
	return 1;	
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
void static fatSetFAT(u32 fatIndex,u32 fatData)
{
	u32 fatOffset;
	u32 sector;
	u16 offset;
	b16 fat12Mask;
	b16 fat12;
	fatOffset = fatIndex << FatType;
	if(FatType==FAT12)fatOffset+=fatIndex>>1;
	sector = FirstFATSector + (fatOffset / BytesPerSector);	//计算FAT扇区号
	offset = fatOffset % BytesPerSector; //计算FAT扇区号中表项的偏移地址
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
			{   //取低12位地址
				fat12.b16_1<<=4;
				fat12Mask.b16_1 = 0xf;
			}
			// 一个表项为1.5bytes(12 bits)
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
在FAT表中查询下一个未使用簇号
cluster：从此簇开始查找
return ：查找到的簇号，如果返回CLUST_EOFE表示cluster后没有空簇可用
-----------------------------------------------------------------------*/
u32 fatNextEmptyCluster(u32 cluster)
{
	//cluster++;
	u32 fatOffset;
	u32 sector;
	u16 offset;
	b16 fat12Cluater;
	bool fat12NextIsHbyte=false;
	fatOffset = cluster << FatType;
	if(FatType==FAT12)fatOffset+=cluster>>1;
	sector = FirstFATSector + (fatOffset / BytesPerSector);	//计算FAT扇区号
	offset = fatOffset % BytesPerSector; //计算FAT扇区号中表项的偏移地址
	do
	{
		if(sector - FirstFATSector>=FatSectorCount)//最后的Fat扇区了
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
					{   //取高12位地址
						fat12Cluater.b16_1>>=4;
						offset++;
					}
					else
					{  //取低12位地址
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
在FAT表中查询下一个簇号
-----------------------------------------------------------------------*/
u32 fatNextCluster(u32 cluster)
{
	u32 nextCluster=CLUST_EOFS;
	u32 fatOffset;
	u32 sector;
	u16 offset;
	fatOffset = cluster << FatType;
	if(FatType==FAT12)fatOffset+=cluster>>1;
	sector = FirstFATSector + (fatOffset / BytesPerSector);	//计算FAT扇区号
	offset = fatOffset % BytesPerSector; //计算FAT扇区号中表项的偏移地址
	if(FatType==FAT32 )	// 一个表项为4bytes(32 bits)
	{
		nextCluster=MMCReadu32(sector,offset);
	}
	else if(FatType==FAT16)
	{    // 一个表项为2bytes(16 bits)
		nextCluster=MMCReadu16(sector,offset);
	}
	else if(FatType==FAT12)
	{	// 一个表项为1.5bytes(12 bits)
		if(offset==(BytesPerSector-1))
		{
			nextCluster=MMCReadu8(sector,offset);
			nextCluster|=((u16)MMCReadu8(sector+1,0)<<8);
		}
		else
		{
			nextCluster=MMCReadu16(sector,offset);
		}
		if(cluster&0X00000001)
		{   //取低12位地址
			nextCluster>>=4;
		}
		else
		{  //取高12位地址
			nextCluster&=0x00000fff;
		}
	}
	// 是否文件的结束簇
	if (nextCluster >= (CLUST_EOFS & FAT_MASK))
		nextCluster = CLUST_EOFE;
	return nextCluster;
} 
enum FIND_FILE_WITH
{
	FFW_Index,
	FFW_Name,
	FFW_IsEmpty,
};
struct FIND_FILE_PARAMETER
{
	u8 FindWith;//FIND_FILE_WITH
	Cluster DirClust;//文件所在目录的族号
	union
	{
		union 
		{
			struct
			{
				u8	Name[8];          // 文件名
				u8	Ext[3]; // 扩展名
			}Name;
			u8 FullName[11];
		}Name;//基于文件名查找
		u16 Index;//基于文件索引查找用于遍历目录
	}Parameter;
	u8 FileIndexInFatBuffer;//查找结果，它是符合查找条件的文件在FatBuffer中的索引
}FindFileParameter;
bool static FindFile()//调用不会更改FindFileParameter中除FileIndexInFatBuffer以外的字段
{
	u32 DirSector;
	DIREntry *dir;
	u8 i;
	u8 SectorC = SectorsPerCluster;
	u8 fname;
	u16 fileIndex = FindFileParameter.Parameter.Index;
	Cluster DirClust=FindFileParameter.DirClust;
	if(DirClust==0)
	{//在根目录
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
			{//空或被删出
				if(FindFileParameter.FindWith==FFW_IsEmpty)
				{
					goto su;
				}
			}
			else if((dir->deAttributes&ATTR_LONG_FILENAME)!=ATTR_LONG_FILENAME)
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
				DirClust = fatNextCluster(DirClust);
				if(DirClust!=CLUST_EOFE)
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
	return true;
}
///******************************************************
//从文件夹中查找文件或目录
//DirClust：目录所在的族（0为根目录）
//file	：要查找的文件的文件名和扩展名
//fileIndex：要查找的文件的目录索引
//withName：为true通过文件名查找否则通过索引
//return	：查到文件返回True，并填写file中的其他字段
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
//	file->FileSize = dir->deFileSize;
//	file->CurrentCluster=file->StartCluster;
//	file->Position=0;
//	return true;
//
//}

static void FileInit(File* file)
{
	DIREntry *dir=(DIREntry *)(FatBuffer+sizeof(DIREntry)*FindFileParameter.FileIndexInFatBuffer);
	memcpy(file->Name.FullName,dir->Name.FullName,11);
	file->Attributes.Attribute=dir->deAttributes;
	file->StartCluster  = ((u32)(dir->deHighClust)<<16)|dir->deStartCluster;
	file->FileSize = dir->deFileSize;
	file->CurrentCluster=file->StartCluster;
	file->Position=0;
}
/******************************************************
从文件夹中查找文件或目录
DirClust：目录所在的族（0为根目录）
fileIndex：要查找的文件的目录索引
return	：查到文件返回True，并填写file中的字段
******************************************************/
bool OpenFileWithIndex(Cluster DirClust,File* file,u16 fileIndex)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Index;
	FindFileParameter.Parameter.Index = fileIndex;
	if(FindFile())
	{
		FileInit(file);
		return true;
	}
	return false;
}
bool FileExist(Cluster DirClust,char fileName[11])
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Name;
	memcpy(FindFileParameter.Parameter.Name.FullName,fileName,11);
	return FindFile();
}
/******************************************************
从文件夹中查找文件或目录
dirCluster：目录所在的族（0为根目录）
file	  ：要查找的文件的文件名和扩展名
return	  ：查到文件返回True，并填写file中的其他字段
******************************************************/
bool OpenFileWithName(Cluster DirClust,File *file)
{
	FindFileParameter.DirClust = DirClust;
	FindFileParameter.FindWith=FFW_Name;
	memcpy(FindFileParameter.Parameter.Name.FullName,file->Name.FullName,11);
	if(FindFile())
	{
		FileInit(file);
		return true;
	}
	return false;
}
/******************************************************
读取文件的下一个扇区（512字节）
file  ：要读取的文件
buffer：读取数据的储蓄buffer
return：成功返回True，到了文件的末尾返回false
******************************************************/
bool FatReadSector(File* file,u8* buffer)
{
	if(file->CurrentCluster==CLUST_EOFE
		||file->FileSize<file->Position)
	{
		return false;
	}
	u32 sector = fatClustToSect(file->CurrentCluster);
	u32 Clustoffset = (file->Position%fatClusterSize())/BytesPerSector;
	ReadBlockToBuff(sector+Clustoffset,buffer);
	file->Position+=512;
	if(Clustoffset+512>fatClusterSize())
	{
		file->CurrentCluster=fatNextCluster(file->CurrentCluster);
	}
	return true;
}


/******************************************************
新建文件或目录
DirClust：目录所在的族（0为根目录）
fileName：文件名（必须全为大写）
Attributes：文件属性
return	：成功返回True
******************************************************/
bool FatNewFile(Cluster DirClust,char fileName[11],u8 Attributes)
{
	DIREntry *dir;
	if(!FileExist(DirClust,fileName))
	{
		//	FindFileParameter.DirClust = DirClust;//FileExist已经设置了
		FindFileParameter.FindWith=FFW_IsEmpty;
		if(FindFile())
		{

			dir=(DIREntry *)(FatBuffer+sizeof(DIREntry)*FindFileParameter.FileIndexInFatBuffer);
			memset(dir,0,sizeof(DIREntry));
			dir->deAttributes=Attributes;
			memcpy(dir,fileName,11);
			WriteBlock(FatBufferLBA);
			return WriteBlock(FatBufferLBA+FatSectorCount);
		}
	}
	return false;
}

