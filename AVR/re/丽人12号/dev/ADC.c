#include "ADC.h"
#include <global.h>
#include <avr/interrupt.h>

volatile bool	ADCEnd = true;//ADC结束
//static 	volatile uint8	CurrentChannel = 0;


int16 ADCSample[ADC_SAMPLE_COUNT];
static uint8  sampleIndex;
void StADC(uint8 Channel)//开始ADC/Channel=将使用的引脚
{
	//CurrentChannel = Channel;
	sampleIndex = 0;
	ADCEnd  = false;
	ADMUX 	 = ADC_MUXCFG|Channel;
#ifdef __AVR_ATmega16__
	ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADATE)|_BV(ADIE)|ADC_PS;
#elif defined __AVR_ATmega8__
	ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADFR)|_BV(ADIE)|ADC_PS;
#endif
}

ISR(SIG_ADC)
{
//	sei();
	b16 tmp;
	
	tmp.b8_2.b8_0 = ADCL;
	tmp.b8_2.b8_1 = ADCH;
	//uint8 l = ADCL;
	//uint8 h = ADCH;
	ADCSample[sampleIndex] =tmp.b16_1;
	sampleIndex++;
	if(sampleIndex>=ADC_SAMPLE_COUNT)
	{
		ADCSRA = 0;
		ADCEnd  = true;
	}
}

