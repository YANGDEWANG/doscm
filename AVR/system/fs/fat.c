/************************************************************/
/*		         	FAT 			 					    */
/*				  											*/
/************************************************************/
#include <global.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <card/mmc/mmc.h>
#include "fat.h"
uchar BUFFER[512];
extern uchar WorkFlag;
unsigned char FatType;  //0:FAT12  1:FAT16   2:FAT32
unsigned long FAT_MASK;
unsigned int  RootDirEnts;       //��Ŀ¼�ܵ�����(FAT12&FAT16)
unsigned long FirstDataSector;   //������
unsigned int  BytesPerSector;
unsigned int  SectorsPerCluster;
unsigned long FirstFATSector;
ROOTDIR_INF   RootDir;  //Ŀ¼��
FIND_FILE_INFO	FindInfo;
//********************************************************************************************
//��һ������
#define ReadBlock(LBA) MMCReadSector(LBA,BUFFER)
#define ReadBlockToBuff(LBA,pbuff) MMCReadSector(LBA,pbuff)

unsigned long  fatGetRootSector(void)
{
	if(FatType == FAT32)
		return ((RootDir.Clust-2)* SectorsPerCluster) +FirstDataSector;
	 return  RootDir.Sector;
}
unsigned long fatGetRootClust(void)
{
	  if(FatType==FAT32)  return RootDir.Clust;
	  	                  return 0x00000000;
	
}

/*-----------------------------------------------------------------------
 ��ѯ������һ���ؿ�ʼ������
-----------------------------------------------------------------------*/
unsigned long fatClustToSect(unsigned long clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
}
/*-----------------------------------------------------------------------
 ��ѯһ������ռ������
-----------------------------------------------------------------------*/
unsigned int fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster;
}

//�ļ�ϵͳ��ʼ��
unsigned char fatInit()
{ 
	PartRecord PartInfo;
	BPB710 *bpb=0;
 	ReadBlock(0);      // ��ȡ��������Ϣ  
	if(0)
	{
	PartInfo = *((PartRecord *) ((PartSector *)BUFFER)->psPart);
	// ������������PartInfo.prStartLBA��
 	ReadBlock(PartInfo.prStartLBA);  //ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	}else
	{
	}
	bpb = (BPB710 *) &((BootSector710 *) BUFFER)->bsBPB;
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
			FAT_MASK=FAT32_MASK;
			FatType = FAT32; 
			break;
		default:
			return 0;
	}  
	return 1;	
}

#define MP3_TYPE   0
#define WMA_TYPE   1
#define MID_TYPE   2
#define LRC_TYPE   3
unsigned char filetype[][3] PROGMEM ={ {"MP3"},{"WMA"},{"MID"},{"LRC"}};
unsigned char hzk12[][11] PROGMEM =
{{"HZK12      "},
 {"UNITOGB BIN"},
 {".          "},
 {"..         "}
};
unsigned char IsHzk12File(unsigned char *pbuf)
{ 
	unsigned char i;
	for(i=0;i<11;i++){
		if(pgm_read_byte(hzk12[0]+i)!=pbuf[i]) break;
	}
   if(i==11)    return 1;  //find ok    
   return 0;
}
unsigned char IsUnitogbFile(unsigned char *pbuf)
{ 
	unsigned char i;
	for(i=0;i<11;i++){
		if(pgm_read_byte(hzk12[1]+i)!=pbuf[i]) break;
	}
   if(i==11)    return 1;  //find ok    
   return 0;
}
unsigned char IsFatherDir(unsigned char *strName)
{
	unsigned char i;
	for(i=0;i<3;i++){
		if(pgm_read_byte(hzk12[3]+i)!=strName[i]) break;
	}
	if(i==3)    return 1;  //���ϲ�Ŀ¼
	return 0;	
	
}

unsigned char IsCurDir(unsigned char *strName)
{
	unsigned char i;
	for(i=0;i<3;i++){
		if(pgm_read_byte(hzk12[2]+i)!=strName[i]) break;
	}
	if(i==3)    return 1;  //�ǵ�ǰĿ¼
	return 0;	
	
}
unsigned char IsMusicFile(unsigned char *strName)
{
	unsigned char i;
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[MP3_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 1;  //��MP3�ļ�
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[WMA_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 2; //��WMA�ļ�
/*
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[MID_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 3; //��MIDI�ļ�
*/
	return 0; 
}		
/*-----------------------------------------------------------------------
 ��FAT���в�ѯ��һ���غ�
-----------------------------------------------------------------------*/
unsigned long fatNextCluster(unsigned long cluster)
{
	unsigned long nextCluster=0;
	unsigned long fatOffset;
	unsigned long sector;
	unsigned int offset;
	if(FatType==FAT32 )	// һ������Ϊ4bytes(32 bits)
	{
		fatOffset = cluster << 2;
		sector = FirstFATSector + (fatOffset / BytesPerSector);	//����FAT������
		offset = fatOffset % BytesPerSector; //����FAT�������б����ƫ�Ƶ�ַ
		nextCluster=MMCReadu32(sector,offset);
		//ReadBlock(sector);  // ��ȡ��һ���غ�	
		//nextCluster = (*((unsigned long*) &((char*)BUFFER)[offset])) ;
	}else if(FatType==FAT16){    // һ������Ϊ2bytes(16 bits)
		fatOffset = cluster << 1; 	//����FAT������
	  	sector = FirstFATSector + (fatOffset / BytesPerSector);
	  	offset = fatOffset % BytesPerSector;//����FAT�������б����ƫ�Ƶ�ַ
	  	nextCluster=MMCReadu16(sector,offset);
//  	ReadBlock(sector);	
//		nextCluster = (*((unsigned int*) &((char*)BUFFER)[offset])) ;
	}else if(FatType==FAT12){	// һ������Ϊ1.5bytes(12 bits)
		fatOffset = cluster+(cluster>>1);
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;
//	ReadBlock(sector);
		if(offset==(BytesPerSector-1)){
			nextCluster=MMCReadu16(sector,offset-1)>>8;
	//	nextCluster=(unsigned long)BUFFER[offset];	//��λ
	//		ReadBlock(sector+1);
			nextCluster|=((MMCReadu16(sector+1,0)<<8)&0x0000ffff);
//		nextCluster|=(BUFFER[0]<<8)&0x0000ffff;  //��λ,������,�������ִ���
		}else{
		nextCluster=MMCReadu16(sector,offset);
//	  	nextCluster =(*((unsigned int*) &((char*)BUFFER)[offset])) ;
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
 
//�ڸ�Ŀ¼�в��Һ��ֿ��ļ����״غż�UNITCODE��ת�����ļ����״غ�---
// 
void GetSysFileClust(unsigned long *hzk,unsigned long *unit)
{
	DIREntry *de=0;	   
	FIND_FILE_INFO	fp;  

	InitSetPath(&fp,fatGetRootClust());
	ReadBlock(fp.Sector);     //���¶�ȡĿ¼��
	(*hzk)=(*unit)=0;
	do{
		if(!ReadNextDirEntry(&fp))  break;
		de = (DIREntry *)BUFFER;
		de+=fp.Index;
		if(*de->deName != 0xe5){
			if((de->deAttributes&ATTR_LONG_FILENAME)!=ATTR_LONG_FILENAME){
			if((de->deAttributes&ATTR_DIRECTORY)!= ATTR_DIRECTORY)// is it a directory ?
			if(*hzk==0){
				if(IsHzk12File(de->deName))
					*hzk= (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;
			}
			if(*unit==0){
				if(IsUnitogbFile(de->deName))
				   *unit= (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;	
		 	} 
		 	if((*hzk!=0)&&(*unit!=0))   break;
		  }
		}
		de++;
	    fp.Index++;
	 if(de->deName[0]==0)   break;
	}while (1);
}  
void InitSetPath(FIND_FILE_INFO *fp,unsigned long dircluster)
{
	fp->Nums=0x8000;      //��λΪ1��ʾ����FAT12&fat16�ĸ�Ŀ¼
	fp->Clust=dircluster;
		if(dircluster==0x00000000){  //��Ŀ¼��
			if(FatType==FAT32){
				    fp->Sector= fatClustToSect(RootDir.Clust);
					fp->Clust = RootDir.Clust;
				}else{
					fp->Sector = RootDir.Sector;   //�õ�Ŀ¼����������
					fp->Nums = RootDirEnts;
				}                                  //��ʱ��Ŀ¼������ȷ����С��	
		}else{
				 fp->Sector=fatClustToSect(dircluster);
		}		
    fp->Index = 0;
	fp->NumSector=0;
}

//�ڵ�ǰ��Ŀ¼�в��Ҹ���ļ�,�ҵ������������lyric��
//���: 1.dircluster    ��ǰĿ¼���״غ�,����FAT12��FAT16�ĸ�Ŀ¼��Ϊ0
//void FindLrcFile(unsigned long dircluster,unsigned char *strName)
//{
//	FIND_FILE_INFO	fp;
//	DIREntry *de=0;	
//	unsigned char  name[8];
//	unsigned char i=0;    
//	WorkFlag&=~bLrc;   
//	for(i=0;i<8;i++)  name[i]=strName[i];  
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //���¶�ȡĿ¼��
//	do 
//	{  
//		if(!ReadNextDirEntry(&fp)) {lyric.Clust=0;  return;} 
//		de=(DIREntry *) BUFFER;
//		de += fp.Index; 
//		if(*de->deName != 0xe5){
//			if((de->deAttributes&ATTR_LONG_FILENAME)!=ATTR_LONG_FILENAME){
//			if((de->deAttributes& ATTR_DIRECTORY)!= ATTR_DIRECTORY){// is it a directory ?
//			   for(i=0;i<8;i++) { 
//			   	 if(name[i]!=de->deName[i])  break;
//			   }                                   
//			   if(i==8){   //���ļ����Ƚ���ȷ 
//			   	   for(i=0;i<3;i++){
//			   	       if(pgm_read_byte(filetype[LRC_TYPE]+i)!=de->deExtension[i]) break; 
//			   	    }
//			   	    if(i==3)  break;
//			   }
//			   
//       	 } 	} }
//		de++;
//	    fp.Index++; 
//	    if(de->deName[0]==0) { lyric.Clust=0;  return;} //�ļ������һ��
//	}while(1);  
//	lyric.first = 1;  
//	lyric.t_sec=lyric.t_ms=0; 
//	WorkFlag|=bLrc;   
//	lyric.so=0;
//	lyric.sl=0;
//	lyric.wl=0;  //��ʼλ��
//	lyric.filelen=(unsigned int)de->deFileSize; //�������ļ���С
//	lyric.Clust = (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//}
unsigned int FindDirIndex(unsigned long fatherdir,unsigned long dircluster)
{
	FIND_FILE_INFO	fp;  
	DIREntry *de=0;	
	uint fileNo=0;     
	unsigned long tempclust;
	InitSetPath(&fp,fatherdir);
	ReadBlock(fp.Sector);     //��ȡĿ¼��
	do 
	{  
		if(!ReadNextDirEntry(&fp))  break;
		de = (DIREntry *)BUFFER;
		de+=fp.Index;
		if(*de->deName != 0xe5){       //�ҵ�Ŀ¼���ļ�
			if((de->deAttributes&ATTR_LONG_FILENAME) != ATTR_LONG_FILENAME){
					if((de->deAttributes&ATTR_DIRECTORY) == ATTR_DIRECTORY){// is it a directory ?
							if(!IsCurDir(de->deName)){
							   fileNo++;
							   tempclust=((unsigned long)de->deHighClust << 16) + de->deStartCluster;
							   if(tempclust==dircluster)	return fileNo;							   	
						}
					}
		    }   
    	}
		de++;
	  fp.Index++; 
	  if(de->deName[0]==0)  break;  
	}while(1);  
	 return 0;
}
			
//-----------------------------------------------------
// ��¼��INDEX�ĺ������ļ���Ŀ¼����: �����״غţ��ļ������ƣ����ļ���)
//
//unsigned int SerarchFile(unsigned long dircluster,unsigned int index)
//{
//	FIND_FILE_INFO	fp;  
//	LongFileName *we;
//	unsigned char  LONGNAME_BUFFER_ADDR[60]; //����25���ַ�
//	unsigned char *LongNameBuffer =	(unsigned char *) LONGNAME_BUFFER_ADDR;
//	unsigned char hasBuffer=0;
//	unsigned int b;
//	unsigned char line=0;
//	char *p;
//	DIREntry *de=0;	
//	unsigned char i=0;  
//	uint fileNo=0;     
////================�Ȳ���Ŀ¼=====================================	
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //��ȡĿ¼��
//	do 
//	{  
//		if(!ReadNextDirEntry(&fp))  break;
//		de = (DIREntry *)BUFFER;
//		de+=fp.Index;
//		if(*de->deName != 0xe5){       //�ҵ�Ŀ¼���ļ�
//			if((de->deAttributes&ATTR_LONG_FILENAME) == ATTR_LONG_FILENAME){
//				we = (LongFileName *) de;
//				b = 13 *( (we->weCnt-1) & 0x0f);				// index into string
//		//		if(b<50-13){
//				p = &LongNameBuffer[b];
//				for (i=0;i<10;i++)	*p++ = we->wePart1[i];	// copy first part			
//				for (i=0;i<12;i++)	*p++ = we->wePart2[i];	// second part
//				for (i=0;i<4;i++)	*p++ = we->wePart3[i];	// and third part
//		//	 }
//				if (we->weCnt & 0x40) *p = 0;					// in case dirnamelength is multiple of 13
//				if ((we->weCnt & 0x0f) == 1) hasBuffer = 1;		// mark that we have a long entry
//			}else{
//				if(hasBuffer){   
//					 hasBuffer = 0;	// clear buffer	
//					if((de->deAttributes&ATTR_DIRECTORY) == ATTR_DIRECTORY){// is it a directory ?
//						fileNo++;
//						if((fileNo>=index)&&(line<4)){  
//							
//							if(WorkFlag&bUnicode){
//						 		UniToGB(LongNameBuffer);  //ת�ɹ�����
//						 		for(i=0;i<40;i++)
//					      		m_c[line].LongName[i]=LongNameBuffer[i]; 
//						    	}else{
//						 		for(i=0;i<11;i++)
//					      		m_c[line].LongName[i]=de->deName[i];
//					    	}
//					      	m_c[line].LongName[i]='\0';
//					      	for(i=0;i<11;i++)
//					      		m_c[line].ShortName[i]=de->deName[i];
//					      	m_c[line].ShortName[i]='\0';
//					      	m_c[line].Type=0;
//					      	m_c[line].Clust=((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//					      	m_c[line++].FileLen=0;
//						}
//					}
//				}else{
//					if((de->deAttributes&ATTR_DIRECTORY) == ATTR_DIRECTORY){
//						if(!IsCurDir(de->deName)){
//							fileNo++;
//						 if((fileNo>=index)&&(line<4)){
//						 	for(i=0;i<11;i++){
//					      	m_c[line].ShortName[i]=de->deName[i];
//					      	m_c[line].LongName[i]=de->deName[i];
//					     }
//					      	m_c[line].ShortName[i]='\0';
//					      	m_c[line].LongName[i]='\0';
//					      	m_c[line].Type=0;
//					      	m_c[line].Clust=((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//					      	m_c[line++].FileLen=0;
//						}
//					 }
//				 }
//  		  }
//		    }   
//       	}
//			de++;
//	    fp.Index++; 
//	    if(de->deName[0]==0)  break;  //�ļ������һ��
//	}while(1);  
////-------------------------�ٲ��������ļ�-------------------------------------------
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //��ȡĿ¼��
//	do 
//	{  
//		if(!ReadNextDirEntry(&fp))  break;
//		de = (DIREntry *)BUFFER;
//		de+=fp.Index;
//		if(*de->deName != 0xe5){       //�ҵ�Ŀ¼���ļ�
//			if((de->deAttributes&ATTR_LONG_FILENAME) == ATTR_LONG_FILENAME)
//			{
//				we = (LongFileName *) de;
//				b = 13 *( (we->weCnt-1) & 0x0f);				// index into string
//		//		if(b<50-13){
//					p = &LongNameBuffer[b];
//					for (i=0;i<10;i++)	*p++ = we->wePart1[i];	// copy first part			
//					for (i=0;i<12;i++)	*p++ = we->wePart2[i];	// second part
//					for (i=0;i<4;i++)	*p++ = we->wePart3[i];	// and third part
//					if (we->weCnt & 0x40) *p = 0;					// in case dirnamelength is multiple of 13
//			//	}
//				if ((we->weCnt & 0x0f) == 1) hasBuffer = 1;		// mark that we have a long entry
//			}else{
//				if(hasBuffer)
//				{   
//					 hasBuffer = 0;	
//					if((de->deAttributes&ATTR_DIRECTORY) != ATTR_DIRECTORY)// is it a directory ?
//					{     
//						if(IsMusicFile(&de->deExtension[0])!=0){
//							fileNo++;
//						 if((fileNo>=index)&&(line<4)){  
//						 	if(WorkFlag&bUnicode){
//						 		UniToGB(LongNameBuffer);  //ת��GB��
//						 		for(i=0;i<40;i++)
//					      		m_c[line].LongName[i]=LongNameBuffer[i]; 
//					     	}else{
//						 		for(i=0;i<11;i++)
//					      		m_c[line].LongName[i]=de->deName[i];
//					    	}
//					    	m_c[line].LongName[i]='\0';
//					    	for(i=0;i<11;i++)
//					      		m_c[line].ShortName[i]=de->deName[i];
//					      	m_c[line].ShortName[i]='\0';
//					      	m_c[line].Type=2;
//					      	m_c[line].Clust=((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//					      	m_c[line++].FileLen=de->deFileSize;
//						  }	 
//						}
//					}
//				}else{
//					if((de->deAttributes&ATTR_DIRECTORY) != ATTR_DIRECTORY){
//						if(IsMusicFile(&de->deExtension[0])!=0){
//						   fileNo++;
//						 if((fileNo>=index)&&(line<4)){  
//						   	for(i=0;i<11;i++)
//					      		m_c[line].ShortName[i]=de->deName[i];
//					      	m_c[line].ShortName[i]='\0';
//						   	for(i=0;i<11;i++)
//					      		m_c[line].LongName[i]=de->deName[i];
//					      	m_c[line].LongName[i]='\0';
//					      	m_c[line].Type=2;
//					      	m_c[line].Clust=((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//					      	m_c[line++].FileLen=de->deFileSize;
//						  }	 
//						}  
//					}
//			  } 
//			}
//		}
//		de++;
//	    fp.Index++; 
//	    if(de->deName[0]==0)  break;  //�ļ������һ��
//	}while(1);  
//     for(i=line;i<4;i++){
//     	m_c[i].LongName[0]='\0';
//		m_c[i].ShortName[0]='\0';
//   		m_c[i].Type=0xff;
//    }
////----------------------------------------------------------------------	
//   return fileNo;
//	
//}
//ȡ��һ��Ŀ¼��
unsigned char ReadNextDirEntry(FIND_FILE_INFO *fp)  
{
	if(fp->Index == 16)	
	{   
		if(!(fp->Nums&0x8000)){		
			if((fp->Nums&0x7FFF)==0) 	return 0;
				fp->Nums--;			
			}else{
				if(fp->NumSector>=SectorsPerCluster){
					fp->Clust=fatNextCluster(fp->Clust); //ȡ��һ��Ŀ¼��
					if(fp->Clust==CLUST_EOFE)  return 0;  // Ŀ¼����ȫ������
					fp->Sector=fatClustToSect(fp->Clust);
					fp->NumSector=0;
			    }
			    fp->NumSector++;
			}
			  fp->Sector++;
				ReadBlock(fp->Sector); // ��ȡ��һ��������Ŀ¼����  
				fp->Index = 0;
		} 
	return 1;		
}
		
//����ת��unit code-> gb2312
//void UniToGB(unsigned char *pbuf)
//{       
//	unsigned char wh;
//	unsigned int  sector,clusor,secoff;
//	unsigned long foffset;
//	unsigned int  code;
//	unsigned char i,m=0;
//	for(i=0;i<25;i++){
//		code= pbuf[i*2+1]*256+pbuf[i*2];
//		if((code==0)||(code==0xffff))   break;
//		if((code&0xff00)==0){
//			if((code>=0x20)&&(code<=0x7e)){
//			 pbuf[m++]=(unsigned char)code;              
//			}else{
//			 pbuf[m++]='?';
//			}
//			 continue;
//		}
//		if(code>=0x4e00){    //�Ǻ���
//			code=(code-0x4e00)*2;
//			sector=code/BytesPerSector;           //�õ��ܵ�������������
//	 		secoff= (unsigned int) code%BytesPerSector; //�����ڵ��ֽ���ƫ��
//	 		wh=(unsigned char) sector%SectorsPerCluster; 
//	 		clusor=(unsigned int) sector/SectorsPerCluster;//�õ��ܵĴ���   
//	     	eeprom_read_block(&foffset,(void*)(clusor*4+0x200),4); //���õ�������
//	     	code=SD_Read_Word(foffset+wh,secoff);
//			pbuf[m++]=(uchar)code;
//	 		pbuf[m++]=code>>8; 
//	 }else{
//	     pbuf[m++]='?';
//	 }	
//	
//	}  
//	pbuf[m]='\0';    
//}
