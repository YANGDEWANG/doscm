#include "ProgrammeCommand.h"
#ifndef _ISP_H_
#define _ISP_H_

#define ISPReset_DDR	DDRB
#define ISPReset_POPT	PORTB
#define ISPReset_PIN	PINB
#define ISPResetPIN		3




//�������������˿λ����̶�����
//��˿���ֽ�λ������Ĭ��ֵ
//#define ISP_RSTDISBL	1//					1( δ��̣�PC6ΪRESET����)
//#define ISP_WDTON		1//					1 ( δ��̣�ͨ��WDTCR ʹWDT ʹ��)
#define ISP_SPIEN		1//					0 ( �ѱ�̣� SPI ���ʹ��)
//#define ISP_CKOPT		1//����ѡ��		1 ( δ���)
//#define ISP_EESAVE		1//					1 ( δ��̣� EEPROM ���ݲ�����)
#define ISP_BOOTSZ1		1//ѡ�� Boot ����С 0 ( �ѱ��)
#define ISP_BOOTSZ0		1//ѡ�� Boot ����С 0 ( �ѱ��)
//#define ISP_BOOTRST		1//ѡ��λ����1 ( δ���)
//��˿λ��λ�ֽ�λ������Ĭ��ֵ
#define ISP_BODLEVEL	1//BOD ������ƽ	1 ( δ���)
#define ISP_BODEN		1//BOD ʹ��		1 ( δ��̣� BOD ����)
//#define ISP_SUT1		1//ѡ������ʱ��	1 ( δ���)
#define ISP_SUT0		1//ѡ������ʱ��	0 ( �ѱ��)
#define ISP_CKSEL3		1//ѡ��ʱ��Դ	0 ( �ѱ��)
//#define ISP_CKSEL2		1//ѡ��ʱ��Դ	0 ( �ѱ��)
#define ISP_CKSEL1		1//ѡ��ʱ��Դ	0 ( �ѱ��)
#define ISP_CKSEL0		1//ѡ��ʱ��Դ	1 ( δ���)

//����λ�ֽ�
//����λ�ֽ�λ������Ĭ��ֵ(1)
//#define ISP_BLB12	1//Boot ����λ1 ( δ���)
//#define ISP_BLB11	1//Boot ����λ1 ( δ���)
//#define ISP_BLB02	1//Boot ����λ1 ( δ���)
//#define ISP_BLB01	1//Boot ����λ1 ( δ���)
#define ISP_LB2		1//����λ1 ( δ���)
#define ISP_LB1		1//����λ1 ( δ���)

#ifndef ISP_RSTDISBL 
#define ISP_HF7 _BV(HFuseBit_RSTDISBL)
#else
#define ISP_HF7 0
#endif
#ifndef ISP_WDTON 
#define ISP_HF6 _BV(HFuseBit_WDTON)
#else
#define ISP_HF6 0
#endif
#ifndef ISP_SPIEN 
#define ISP_HF5 _BV(HFuseBit_SPIEN)
#else
#define ISP_HF5 0
#endif
#ifndef ISP_CKOPT 
#define ISP_HF4 _BV(HFuseBit_CKOPT)
#else
#define ISP_HF4 0
#endif
#ifndef ISP_EESAVE 
#define ISP_HF3 _BV(HFuseBit_EESAVE)
#else
#define ISP_HF3 0
#endif
#ifndef ISP_BOOTSZ1 
#define ISP_HF2 _BV(HFuseBit_BOOTSZ1)
#else
#define ISP_HF2 0
#endif
#ifndef ISP_BOOTSZ0 
#define ISP_HF1 _BV(HFuseBit_BOOTSZ0)
#else
#define ISP_HF1 0
#endif
#ifndef ISP_BOOTRST 
#define ISP_HF0 _BV(HFuseBit_BOOTRST)
#else
#define ISP_HF0 0
#endif
#define ISP_HFuseByte (ISP_HF0|ISP_HF1|ISP_HF2|ISP_HF3|ISP_HF4|ISP_HF5|ISP_HF6|ISP_HF7)

#ifndef ISP_BODLEVEL
#define ISP_LF7 _BV(LFuseBit_BODLEVEL)
#else
#define ISP_LF7 0
#endif
#ifndef ISP_BODEN
#define ISP_LF6 _BV(LFuseBit_BODEN)
#else
#define ISP_LF6 0
#endif
#ifndef ISP_SUT1
#define ISP_LF5 _BV(LFuseBit_SUT1)
#else
#define ISP_LF5 0
#endif
#ifndef ISP_SUT0
#define ISP_LF4 _BV(LFuseBit_SUT0)
#else
#define ISP_LF4 0
#endif
#ifndef ISP_CKSEL3
#define ISP_LF3 _BV(LFuseBit_CKSEL3)
#else
#define ISP_LF3 0
#endif
#ifndef ISP_CKSEL2
#define ISP_LF2 _BV(LFuseBit_CKSEL2)
#else
#define ISP_LF2 0
#endif
#ifndef ISP_CKSEL1
#define ISP_LF1 _BV(LFuseBit_CKSEL1)
#else
#define ISP_LF1 0
#endif
#ifndef ISP_CKSEL0
#define ISP_LF0 _BV(LFuseBit_CKSEL0)
#else
#define ISP_LF0 0
#endif
#define ISP_LFuseByte (ISP_LF0|ISP_LF1|ISP_LF2|ISP_LF3|ISP_LF4|ISP_LF5|ISP_LF6|ISP_LF7)



#define ISP_Lock7 _BV(7)
#define ISP_Lock6 _BV(6)
#ifndef ISP_BLB12
#define ISP_Lock5 _BV(LOCKBit_BLB12)
#else
#define ISP_Lock5 0
#endif
#ifndef ISP_BLB11
#define ISP_Lock4 _BV(LOCKBit_BLB11)
#else
#define ISP_Lock4 0
#endif
#ifndef ISP_BLB02
#define ISP_Lock3 _BV(LOCKBit_BLB02)
#else
#define ISP_Lock3 0
#endif
#ifndef ISP_BLB01
#define ISP_Lock2 _BV(LOCKBit_BLB01)
#else
#define ISP_Lock2 0
#endif
#ifndef ISP_LB2
#define ISP_Lock1 _BV(LOCKBit_LB2)
#else
#define ISP_Lock1 0
#endif
#ifndef ISP_LB1
#define ISP_Lock0 _BV(LOCKBit_LB1)
#else
#define ISP_Lock0 0
#endif
#define ISP_LockByte (ISP_Lock0|ISP_Lock1|ISP_Lock2|ISP_Lock3|ISP_Lock4|ISP_Lock5|ISP_Lock6|ISP_Lock7)
#ifndef _IN_ISP_C_

uint32_t ISPReadFlag();//��������ʾ
extern bool ISPEnable();
extern void ISPDisable();
extern void ISPIni();
extern void ISPErasure();
extern void ISPWriteLockBit(uint8 data);
extern void ISPWriteFlashPage(uint8 PageAdd);
extern void ISPWriteEEPROM(uint16_t add,uint8 data);
extern void ISPWriteFuseBit(uint8 add,uint8 data);
extern void ISPLoadFlashPage(uint8 add,uint8 data);
extern uint8 ISPReadLockBit();
extern uint8 ISPReadFlash(uint16_t add);
extern uint8 ISPReadEEPROM(uint16_t add);
extern uint8 ISPRead_FuseBit(uint8 add);
extern uint8 ISPReadAdjustByte(uint8 add);

#endif//_IN_ISP_C_

#endif//_ISP_H_
