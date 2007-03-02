#include "ADC.h"
#include <global.h>
#include <avr/interrupt.h>

uint8	ADC_H[ADCChannels];

volatile bool	ADCEnd = true;//ADC结束
static 	volatile uint8	CurrentChannel = 0;
void ADCInit()
{
	//2.56v,ADC0,左对齐
	//ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR);
	//AVCC,ADC0,左对齐
	ADMUX = (1<<REFS0)|(1<<ADLAR);
	//64分频
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN)|(1<<ADIE);//
}
void StADC(uint8 Channel)//开始ADC/Channel=将使用的引脚
{
	CurrentChannel = Channel;
	uint8 tmp = ADMUX;
	tmp 	&= ~7;
	ADMUX 	 = tmp|Channel;
	ADCSRA |= (1<<ADSC);
	ADCEnd  = false;
}
ISR(SIG_ADC)
{
	sei();
	ADC_H[CurrentChannel] = ADCH;
	uint8 tmp = CurrentChannel+1;
	if(tmp == ADCChannels)
	{
		ADCEnd = true;
		return;
	}
	StADC(tmp);
}

