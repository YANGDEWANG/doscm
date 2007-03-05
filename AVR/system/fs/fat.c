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
unsigned int  RootDirEnts;       //根目录总的项数(FAT12&FAT16)
unsigned long FirstDataSector;   //数据区
unsigned int  BytesPerSector;
unsigned int  SectorsPerCluster;
unsigned long FirstFATSector;
ROOTDIR_INF   RootDir;  //目录区
FIND_FILE_INFO	FindInfo;
//********************************************************************************************
//读一个扇区
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
 查询数据区一个簇开始扇区号
-----------------------------------------------------------------------*/
unsigned long fatClustToSect(unsigned long clust)
{
	return ((clust-2) * SectorsPerCluster) + FirstDataSector;
}
/*-----------------------------------------------------------------------
 查询一个簇所占扇区数
-----------------------------------------------------------------------*/
unsigned int fatClusterSize(void)
{
	// return the number of sectors in a disk cluster
	return SectorsPerCluster;
}

//文件系统初始化
unsigned char fatInit()
{ 
	PartRecord PartInfo;
	BPB710 *bpb=0;
 	ReadBlock(0);      // 读取分区表信息  
	if(0)
	{
	PartInfo = *((PartRecord *) ((PartSector *)BUFFER)->psPart);
	// 引导扇区号在PartInfo.prStartLBA中
 	ReadBlock(PartInfo.prStartLBA);  //ataReadSectors( DRIVE0, PartInfo.prStartLBA, 1, SectorBuffer );
	}else
	{
	}
	bpb = (BPB710 *) &((BootSector710 *) BUFFER)->bsBPB;
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
	if(i==3)    return 1;  //是上层目录
	return 0;	
	
}

unsigned char IsCurDir(unsigned char *strName)
{
	unsigned char i;
	for(i=0;i<3;i++){
		if(pgm_read_byte(hzk12[2]+i)!=strName[i]) break;
	}
	if(i==3)    return 1;  //是当前目录
	return 0;	
	
}
unsigned char IsMusicFile(unsigned char *strName)
{
	unsigned char i;
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[MP3_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 1;  //是MP3文件
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[WMA_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 2; //是WMA文件
/*
	for(i=0;i<3;i++){
		if(pgm_read_byte(filetype[MID_TYPE]+i)!=strName[i]) break;
	}
	if(i==3)    return 3; //是MIDI文件
*/
	return 0; 
}		
/*-----------------------------------------------------------------------
 在FAT表中查询下一个簇号
-----------------------------------------------------------------------*/
unsigned long fatNextCluster(unsigned long cluster)
{
	unsigned long nextCluster=0;
	unsigned long fatOffset;
	unsigned long sector;
	unsigned int offset;
	if(FatType==FAT32 )	// 一个表项为4bytes(32 bits)
	{
		fatOffset = cluster << 2;
		sector = FirstFATSector + (fatOffset / BytesPerSector);	//计算FAT扇区号
		offset = fatOffset % BytesPerSector; //计算FAT扇区号中表项的偏移地址
		nextCluster=MMCReadu32(sector,offset);
		//ReadBlock(sector);  // 读取下一个簇号	
		//nextCluster = (*((unsigned long*) &((char*)BUFFER)[offset])) ;
	}else if(FatType==FAT16){    // 一个表项为2bytes(16 bits)
		fatOffset = cluster << 1; 	//计算FAT扇区号
	  	sector = FirstFATSector + (fatOffset / BytesPerSector);
	  	offset = fatOffset % BytesPerSector;//计算FAT扇区号中表项的偏移地址
	  	nextCluster=MMCReadu16(sector,offset);
//  	ReadBlock(sector);	
//		nextCluster = (*((unsigned int*) &((char*)BUFFER)[offset])) ;
	}else if(FatType==FAT12){	// 一个表项为1.5bytes(12 bits)
		fatOffset = cluster+(cluster>>1);
		sector = FirstFATSector + (fatOffset / BytesPerSector);
		offset = fatOffset % BytesPerSector;
//	ReadBlock(sector);
		if(offset==(BytesPerSector-1)){
			nextCluster=MMCReadu16(sector,offset-1)>>8;
	//	nextCluster=(unsigned long)BUFFER[offset];	//低位
	//		ReadBlock(sector+1);
			nextCluster|=((MMCReadu16(sector+1,0)<<8)&0x0000ffff);
//		nextCluster|=(BUFFER[0]<<8)&0x0000ffff;  //高位,必须与,否则会出现错误
		}else{
		nextCluster=MMCReadu16(sector,offset);
//	  	nextCluster =(*((unsigned int*) &((char*)BUFFER)[offset])) ;
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
 
//在根目录中查找汉字库文件的首簇号及UNITCODE码转换表文件的首簇号---
// 
void GetSysFileClust(unsigned long *hzk,unsigned long *unit)
{
	DIREntry *de=0;	   
	FIND_FILE_INFO	fp;  

	InitSetPath(&fp,fatGetRootClust());
	ReadBlock(fp.Sector);     //重新读取目录表
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
	fp->Nums=0x8000;      //高位为1表示不是FAT12&fat16的根目录
	fp->Clust=dircluster;
		if(dircluster==0x00000000){  //根目录区
			if(FatType==FAT32){
				    fp->Sector= fatClustToSect(RootDir.Clust);
					fp->Clust = RootDir.Clust;
				}else{
					fp->Sector = RootDir.Sector;   //得到目录区的扇区号
					fp->Nums = RootDirEnts;
				}                                  //此时根目录项数是确定大小的	
		}else{
				 fp->Sector=fatClustToSect(dircluster);
		}		
    fp->Index = 0;
	fp->NumSector=0;
}

//在当前的目录中查找歌词文件,找到置相关数据至lyric中
//入口: 1.dircluster    当前目录的首簇号,若是FAT12或FAT16的根目录则为0
//void FindLrcFile(unsigned long dircluster,unsigned char *strName)
//{
//	FIND_FILE_INFO	fp;
//	DIREntry *de=0;	
//	unsigned char  name[8];
//	unsigned char i=0;    
//	WorkFlag&=~bLrc;   
//	for(i=0;i<8;i++)  name[i]=strName[i];  
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //重新读取目录表
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
//			   if(i==8){   //主文件名比较正确 
//			   	   for(i=0;i<3;i++){
//			   	       if(pgm_read_byte(filetype[LRC_TYPE]+i)!=de->deExtension[i]) break; 
//			   	    }
//			   	    if(i==3)  break;
//			   }
//			   
//       	 } 	} }
//		de++;
//	    fp.Index++; 
//	    if(de->deName[0]==0) { lyric.Clust=0;  return;} //文件的最后一项
//	}while(1);  
//	lyric.first = 1;  
//	lyric.t_sec=lyric.t_ms=0; 
//	WorkFlag|=bLrc;   
//	lyric.so=0;
//	lyric.sl=0;
//	lyric.wl=0;  //初始位置
//	lyric.filelen=(unsigned int)de->deFileSize; //保存歌词文件大小
//	lyric.Clust = (unsigned long) ((unsigned long)de->deHighClust << 16) + de->deStartCluster;
//}
unsigned int FindDirIndex(unsigned long fatherdir,unsigned long dircluster)
{
	FIND_FILE_INFO	fp;  
	DIREntry *de=0;	
	uint fileNo=0;     
	unsigned long tempclust;
	InitSetPath(&fp,fatherdir);
	ReadBlock(fp.Sector);     //读取目录表
	do 
	{  
		if(!ReadNextDirEntry(&fp))  break;
		de = (DIREntry *)BUFFER;
		de+=fp.Index;
		if(*de->deName != 0xe5){       //找到目录或文件
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
// 记录第INDEX的后四项文件或目录内容: 内容首簇号，文件的名称（长文件名)
//
//unsigned int SerarchFile(unsigned long dircluster,unsigned int index)
//{
//	FIND_FILE_INFO	fp;  
//	LongFileName *we;
//	unsigned char  LONGNAME_BUFFER_ADDR[60]; //保存25个字符
//	unsigned char *LongNameBuffer =	(unsigned char *) LONGNAME_BUFFER_ADDR;
//	unsigned char hasBuffer=0;
//	unsigned int b;
//	unsigned char line=0;
//	char *p;
//	DIREntry *de=0;	
//	unsigned char i=0;  
//	uint fileNo=0;     
////================先查找目录=====================================	
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //读取目录表
//	do 
//	{  
//		if(!ReadNextDirEntry(&fp))  break;
//		de = (DIREntry *)BUFFER;
//		de+=fp.Index;
//		if(*de->deName != 0xe5){       //找到目录或文件
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
//						 		UniToGB(LongNameBuffer);  //转成国标码
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
//	    if(de->deName[0]==0)  break;  //文件的最后一项
//	}while(1);  
////-------------------------再查找音乐文件-------------------------------------------
//	InitSetPath(&fp,dircluster);
//	ReadBlock(fp.Sector);     //读取目录表
//	do 
//	{  
//		if(!ReadNextDirEntry(&fp))  break;
//		de = (DIREntry *)BUFFER;
//		de+=fp.Index;
//		if(*de->deName != 0xe5){       //找到目录或文件
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
//						 		UniToGB(LongNameBuffer);  //转成GB码
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
//	    if(de->deName[0]==0)  break;  //文件的最后一项
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
//取下一个目录项
unsigned char ReadNextDirEntry(FIND_FILE_INFO *fp)  
{
	if(fp->Index == 16)	
	{   
		if(!(fp->Nums&0x8000)){		
			if((fp->Nums&0x7FFF)==0) 	return 0;
				fp->Nums--;			
			}else{
				if(fp->NumSector>=SectorsPerCluster){
					fp->Clust=fatNextCluster(fp->Clust); //取下一个目录簇
					if(fp->Clust==CLUST_EOFE)  return 0;  // 目录内容全部找完
					fp->Sector=fatClustToSect(fp->Clust);
					fp->NumSector=0;
			    }
			    fp->NumSector++;
			}
			  fp->Sector++;
				ReadBlock(fp->Sector); // 读取下一个扇区的目录数据  
				fp->Index = 0;
		} 
	return 1;		
}
		
//代码转换unit code-> gb2312
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
//		if(code>=0x4e00){    //是汉字
//			code=(code-0x4e00)*2;
//			sector=code/BytesPerSector;           //得到总的完整的扇区数
//	 		secoff= (unsigned int) code%BytesPerSector; //扇区内的字节数偏移
//	 		wh=(unsigned char) sector%SectorsPerCluster; 
//	 		clusor=(unsigned int) sector/SectorsPerCluster;//得到总的簇数   
//	     	eeprom_read_block(&foffset,(void*)(clusor*4+0x200),4); //查表得到首扇区
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
