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
u16	RootDirEnts;       //��Ŀ¼�ܵ�����(FAT12&FAT16)
u32 FirstDataSector;   //������
u16	BytesPerSector;
u16	SectorsPerCluster;
u32 FirstFATSector;
ROOTDIR_INF	RootDir;  //Ŀ¼��
//FindFileInfo	FindInfo;
//********************************************************************************************
//��һ������
#define ReadBlock(LBA) MMCReadSector(LBA,FatBuffer)
#define ReadBlockToBuff(LBA,pbuff) MMCReadSector(LBA,pbuff)

/*-----------------------------------------------------------------------
��ѯ������һ���ؿ�ʼ������
-----------------------------------------------------------------------*/
u32 fatClustToSect(u32 clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
}
/*-----------------------------------------------------------------------
��ѯһ������ռ�ֽ���
-----------------------------------------------------------------------*/
u32 fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster*BytesPerSector;
}

//�ļ�ϵͳ��ʼ��
u8 fatInit()
{ 
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
		FirstDataSector	+= bpb->bpbResSectors + bpb->bpbFATs * bpb->bpbFATsecs; 
	}
	else
	{
		// bpbFATsecs��0,ΪFAT32,FAT����ռ����������bpbBigFATsecs��
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
��FAT���в�ѯ��һ���غ�
-----------------------------------------------------------------------*/
u32 fatNextCluster(u32 cluster)
{
	u32 nextCluster=0;
	u32 fatOffset;
	u32 sector;
	u16 offset;
	if(FatType==FAT32 )	// һ������Ϊ4bytes(32 bits)
	{
		fatOffset = cluster << 2;
		sector = FirstFATSector + (fatOffset / BytesPerSector);	//����FAT������
		offset = fatOffset % BytesPerSector; //����FAT�������б����ƫ�Ƶ�ַ
		nextCluster=MMCReadu32(sector,offset);
		//ReadBlock(sector);  // ��ȡ��һ���غ�	
		//nextCluster = (*((u32*) &((char*)FatBuffer)[offset])) ;
	}else if(FatType==FAT16){    // һ������Ϊ2bytes(16 bits)
		fatOffset = cluster << 1; 	//����FAT������
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;//����FAT�������б����ƫ�Ƶ�ַ
		nextCluster=MMCReadu16(sector,offset);
		//  	ReadBlock(sector);	
		//		nextCluster = (*((u16*) &((char*)FatBuffer)[offset])) ;
	}else if(FatType==FAT12){	// һ������Ϊ1.5bytes(12 bits)
		fatOffset = cluster+(cluster>>1);
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;
		//	ReadBlock(sector);
		if(offset==(BytesPerSector-1)){
			nextCluster=MMCReadu16(sector,offset-1)>>8;
			//	nextCluster=(u32)FatBuffer[offset];	//��λ
			//		ReadBlock(sector+1);
			nextCluster|=((MMCReadu16(sector+1,0)<<8)&0x0000ffff);
			//		nextCluster|=(FatBuffer[0]<<8)&0x0000ffff;  //��λ,������,�������ִ���
		}else{
			nextCluster=MMCReadu16(sector,offset);
			//	  	nextCluster =(*((u16*) &((char*)FatBuffer)[offset])) ;
		}
		if(cluster&0X00000001){   //ȡ��12λ��ַ
			nextCluster>>=4;
		}else{  //ȡ��12λ��ַ
			nextCluster&=0x00000fff;
		}
	}
	// �Ƿ��ļ��Ľ�����
	if (nextCluster >= (CLUST_EOFS & FAT_MASK))
		nextCluster = CLUST_EOFE;
	return nextCluster;
} 
/******************************************************
���ļ����в����ļ���Ŀ¼
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	��Ҫ���ҵ��ļ����ļ�������չ��
fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
withName��Ϊtrueͨ���ļ������ҷ���ͨ������
return	���鵽�ļ�����True������дfile�е������ֶ�
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
���ļ����в����ļ���Ŀ¼
DirClust��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
fileIndex��Ҫ���ҵ��ļ���Ŀ¼����
return	���鵽�ļ�����True������дfile�е��ֶ�
******************************************************/
bool FatGetFileWithIndex(Cluster DirClust,File* file,u16 fileIndex)
{
	return FatGetFile(DirClust,file,fileIndex,false);
}

/******************************************************
���ļ����в����ļ���Ŀ¼
dirCluster��Ŀ¼���ڵ��壨0Ϊ��Ŀ¼��
file	  ��Ҫ���ҵ��ļ����ļ�������չ��
return	  ���鵽�ļ�����True������дfile�е������ֶ�
******************************************************/
bool FatFindFileInDirWithName(Cluster dirCluster,File *file)
{
	return FatGetFile(dirCluster,file,0,true);
}
/******************************************************
��ȡ�ļ�����һ��������512�ֽڣ�
file  ��Ҫ��ȡ���ļ�
buffer����ȡ���ݵĴ���buffer
return���ɹ�����True�������ļ���ĩβ����false
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

