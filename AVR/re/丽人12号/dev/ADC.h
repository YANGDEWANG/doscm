#include <configure.h>
#ifndef _ADC_H_
#define _ADC_H_

#ifndef ADC_CFG
//---------------------ADC����--------------------------//
#define ADC_CFG
#define ADC_MUXCFG			(ADC_REFS_AVCC|_BV(ADLAR))
#define ADC_SAMPLE_COUNT	64
#define ADC_PS				ADC_PS_64
//--------------------ADC������-------------------------//
#endif//ADC_CFG

/*
--------------------------------------------------------------
ADC �๤ѡ��Ĵ����� ADMUX
REFS1	REFS0	�ο���ѹѡ��
0		0		AREF�� �ڲ�Vref �ر�
0		1		AVCC�� AREF ��������˲�����
1		0		����
1		1		2.56V ��Ƭ�ڻ�׼��ѹԴ�� AREF ��������˲�����
ADLAR: ADC ת����������
Bits 3:0 �C MUX3:0: ģ��ͨ��ѡ��λ
--------------------------------------------------------------
*/
#define ADC_REFS_AREF (0)
#define ADC_REFS_AVCC (_BV(REFS0))
#define ADC_REFS_256  (_BV(REFS0)|_BV(REFS1))
/*
ADC ���ƺ�״̬�Ĵ���A ��ADCSRA
--------------------------------------------------------------
Bit 7 �C ADEN: ADC ʹ��
Bit 6 �C ADSC: ADC ��ʼת��
Bit 5 �C ADFR: ADC ����ת��ѡ��
Bit 4 �C ADIF: ADC �жϱ�־
Bit 3 �C ADIE: ADC �ж�ʹ��
Bits 2:0 �C ADPS2:0: ADC Ԥ��Ƶ��ѡ��λ
--------------------------------------------------------------
*/
//ADC Ԥ��Ƶ��ѡ��λ
#define ADC_PS_2	1
#define ADC_PS_4	2
#define ADC_PS_8	3
#define ADC_PS_16	4
#define ADC_PS_32	5
#define ADC_PS_64	6
#define ADC_PS_128	7



extern int16 ADCSample[ADC_SAMPLE_COUNT];
extern volatile bool	ADCEnd;//ADC����

void StADC(uint8 Channel);//��ʼADC
#endif	//_ADC_H_
