#ifndef _CONFIGURE_ALL_H_
#define _CONFIGURE_ALL_H_


//#define ISPFLASHPRO


#if defined(ISPFLASHPRO)
#include "..\re\ISPFlashPro\cfg\configure.h"
#elif defined(CH_DIS1_PROJ)
#include "..\re\����12��\cfg\configure.h"
#endif
//����m8��������������Ӱ
//m8	m16
//PB2	PB4->SPI
//PB3	PB5->SPI
//PB4	PB6->SPI
//PB5	PB7->SPI
//PC5	PC0->TWI_SCL
//PC4	PC1->TWI_SDA
//PC3	PA3->ADC
//PC2	PA2->ADC
//PC1	PA1->ADC
//PC0	PA0->ADC
#endif//_CONFIGURE_ALL_H_
