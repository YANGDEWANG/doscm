#include <configure.h>
#ifndef _ADC_H_
#define _ADC_H_
#define ADCChannels 2

extern uint8	ADC_H[ADCChannels];
extern volatile bool	ADCEnd;//ADC½áÊø

void ADCInit();
void StADC(uint8 Channel);//¿ªÊ¼ADC
#endif	//_ADC_H_
