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
ROOTDIR_INF	RootDir;  //目录区
//FindFileInfo	FindInfo;
//********************************************************************************************
//读一个扇区
#define ReadBlock(LBA) MMCReadSector(LBA,FatBuffer)
#define ReadBlockToBuff(LBA,pbuff) MMCReadSector(LBA,pbuff)

/*-----------------------------------------------------------------------
查询数据区一个簇开始扇区号
-----------------------------------------------------------------------*/
u32 fatClustToSect(u32 clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
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
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbFATsecs; 
	}
	else
	{
		// bpbFATsecs是0,为FAT32,FAT表所占的扇区数在bpbBigFATsecs里
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbBigFATsecs;
	}
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


/*-----------------------------------------------------------------------
在FAT表中查询下一个簇号
-----------------------------------------------------------------------*/
u32 fatNextCluster(u32 cluster)
{
	u32 nextCluster=0;
	u32 fatOffset;
	u32 sector;
	u16 offset;
	if(FatType==FAT32 )	// 一个表项为4bytes(32 bits)
	{
		fatOffset = cluster << 2;
		sector = FirstFATSector + (fatOffset / BytesPerSector);	//计算FAT扇区号
		offset = fatOffset % BytesPerSector; //计算FAT扇区号中表项的偏移地址
		nextCluster=MMCReadu32(sector,offset);
		//ReadBlock(sector);  // 读取下一个簇号	
		//nextCluster = (*((u32*) &((char*)FatBuffer)[offset])) ;
	}else if(FatType==FAT16){    // 一个表项为2bytes(16 bits)
		fatOffset = cluster << 1; 	//计算FAT扇区号
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;//计算FAT扇区号中表项的偏移地址
		nextCluster=MMCReadu16(sector,offset);
		//  	ReadBlock(sector);	
		//		nextCluster = (*((u16*) &((char*)FatBuffer)[offset])) ;
	}else if(FatType==FAT12){	// 一个表项为1.5bytes(12 bits)
		fatOffset = cluster+(cluster>>1);
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;
		//	ReadBlock(sector);
		if(offset==(BytesPerSector-1)){
			nextCluster=MMCReadu16(sector,offset-1)>>8;
			//	nextCluster=(u32)FatBuffer[offset];	//低位
			//		ReadBlock(sector+1);
			nextCluster|=((MMCReadu16(sector+1,0)<<8)&0x0000ffff);
			//		nextCluster|=(FatBuffer[0]<<8)&0x0000ffff;  //高位,必须与,否则会出现错误
		}else{
			nextCluster=MMCReadu16(sector,offset);
			//	  	nextCluster =(*((u16*) &((char*)FatBuffer)[offset])) ;
		}
		if(cluster&0X00000001){   //取低12位地址
			nextCluster>>=4;
		}else{  //取高12位地址
			nextCluster&=0x00000fff;
		}
	}
	// 是否文件的结束簇
	if (nextCluster >= (CLUST_EOFS & FAT_MASK))
		nextCluster = CLUST_EOFE;
	return nextCluster;
} 
/******************************************************
从文件夹中查找文件或目录
DirClust：目录所在的族（0为根目录）
file	：要查找的文件的文件名和扩展名
fileIndex：要查找的文件的目录索引
withName：为true通过文件名查找否则通过索引
return	：查到文件返回True，并填写file中的其他字段
******************************************************/
bool FatGetFile(Cluster DirClust,File* file,u16 fileIndex,bool withName)
{
	Cluster DirSector;
	DIREntry *dir;
	u8 i;
	u8 SectorC = SectorsPerCluster;
	u8 fname;
	if(DirClust==0)
	{
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
			fname = *dir->deName;
			if((fname!=SLOT_EMPTY)
				&&(fname!=SLOT_DELETED)
				//&&(fname!='.')
				&&((dir->deAttributes&ATTR_LONG_FILENAME)!=ATTR_LONG_FILENAME))
			{
				if(withName)
				{
					if(memcmp(dir->deName,file->Name,8)==0
						&&memcmp(dir->deExtension,file->ExtensionName,3)==0)
					{
						goto su;
					}
				}
				else
				{
				if(fileIndex==0)
				{
					goto su;
				}
				fileIndex--;
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
			}
		}
		else
		{
			DirSector++;
		}
	}
	while(true);
su:
	memcpy(file->Name,dir->deName,8);
	memcpy(file->ExtensionName,dir->deExtension,8);
	file->Attributes.Attribute=dir->deAttributes;
	file->StartCluster  = ((u32)(dir->deHighClust)<<16)|dir->deStartCluster;
	file->FileSize = dir->deFileSize;
	file->CurrentCluster=file->StartCluster;
	file->Position=0;
	return true;

}
/******************************************************
从文件夹中查找文件或目录
DirClust：目录所在的族（0为根目录）
fileIndex：要查找的文件的目录索引
return	：查到文件返回True，并填写file中的字段
******************************************************/
bool FatGetFileWithIndex(Cluster DirClust,File* file,u16 fileIndex)
{
	return FatGetFile(DirClust,file,fileIndex,false);
}

/******************************************************
从文件夹中查找文件或目录
dirCluster：目录所在的族（0为根目录）
file	  ：要查找的文件的文件名和扩展名
return	  ：查到文件返回True，并填写file中的其他字段
******************************************************/
bool FatFindFileInDirWithName(Cluster dirCluster,File *file)
{
	return FatGetFile(dirCluster,file,0,true);
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

