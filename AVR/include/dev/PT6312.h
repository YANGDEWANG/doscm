#ifndef _PT6312_H_
#define _PT6312_H_
#include <configure.h>
#ifndef PT6312_CFG
//------------------------------PT6312����-------------------------//

#define PT6312_CFG
#define PT6312_DISPLAY_MODE	0x02						//����Ϊ6�� 16��
#define PT6312_MEMORY_SIZE	(PT6312_DISPLAY_MODE+4)*2	//��ʹ�õĴ���ռ��С
#define PT6312_LUM_CONTROL								//����ʹ�����ȿ���
#define	PT6312_WRLED									//����ʹ��LED�ڿ���
#define	PT6312_SWDATA									//����ʹ�ö�ȡSW����
#define PT6312_LUM					6					//Ĭ������
#define PT6312_KEY_MEMORY_SIZE		3					//��������ռ��С
#define PT6312_KEY_SCAN_AMOUNT		1					//��ͬʱ����İ���������Ŀ
#define PT6312_SPI_FOSC				SPI_FOSC_16			//ѡ��SPI�ٶ�
//�������ݽӿ�����
#define PT6312_POPT	B
#define	PT6312_STB	4

//------------------------------PT6312����-END----------------------//
#endif//PT6312_CFG

//------------------------------------------------------------------//
//ɨ�谴����
//�ذ��µļ�����ֵָ����෵�صİ�������"PT6312_KEY_SCAN_AMOUNT"����
//return 0 ��ʾ�ް�������
//------------------------------------------------------------------//
uint8* ScanKey();
//��ʼ��6312��
void PT6312Init();				
//����ʾ����datд���Դ�
void PT6312UpdateAll(uint8* dat);

//------------------------PT6312_WRLED------------------------------//
#ifdef 	PT6312_WRLED
//дled������
void PT6312WriteLed(uint8);
#endif//PT6312_WRLED
//------------------------PT6312_SWDATA-----------------------------//
#ifdef 	PT6312_SWDATA
//�����ݴ�SW�ڣ���4bit��Ч
uint8 PT6312ReadSWData();
#endif//PT6312_SWDATA
#endif

