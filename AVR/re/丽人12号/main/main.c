#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <global.h>
#include "polling.h"
#include "ADC.h"
#include "../dev/STF16360EN.h"
#include "PT2314_2.h"
#include "PT2314.h"
#include "iic.h"
#include "..\ui\c.h"
#include "math.h"
#include "M62429P.h"
#include "AutoControl.h"
//#include "SPI.h"
//#include "displayAudio.h"
//#include "../ui/Display.h"
#include "Clock.h"
//#include "UserEventManage.h"
#include "eeprom_ex.h"
#include <avr/wdt.h> 
#include <fft.h> 
#include <ir.h> 
#include "pointvfddisplay.h"
//#include "ISP.h"
void IniDev()
{
	/////////////////��ʼIO///////////////////////////
	DDRB|=(1<<0);//4053 ABC
	DDRD|=(1<<4);//����
	DDRD|=(1<<7);//�̵���
	DDRD|=(1<<0);//ok��ʱ
	DDRD|=(1<<1);//ok��ʱ
#ifdef __AVR_ATmega16__
	PORTA|=(1<<3);//ok�������
#elif defined __AVR_ATmega8__
	PORTC|=(1<<3);//ok�������
#endif
	onSound();

	/////////////////��ʼ���豸///////////////////////////


	//EA = 1;
	sei();
	IniKEYInput();
	//VSPI2_MasterInit();
	//IniDisplay();
	////ShowMesStr(MS_HELLO);
	IniEeprom_ex();
	//IniIIC();
	stf16360enInit();
	AutoControl.Max = MainVolume;
	AutoControl.Min = 0;
	//AutoControl.Step = 2;
	AutoControl.dat = &MainVolume;
	AutoControl.Callback = UpdateAllVolume;
//	OldMainVolume = MainVolume;
	MainVolume = 0;
	UpdateAllVolume();
	//IniPT2314();
	ControlClock(true);
	InitDisplay();
	IniIR();
	//	ADCInit();


}

int main()//                         ���Գ���
{ 
	uint8 delt = 100;
	while(delt--)
	{
		_delay_ms(5);//������ʱ
	}
	//while(1)
	//{
	//	int i =0;
	//	for(;i<sizeof(ADCSample)/sizeof(int16);i++)
	//	{
	//	switch(i%4)
	//	{
	//	case 2:
	//	case 0:ADCSample[i] = (0);break;
	//	case 1:ADCSample[i] = (0x7fff);break;
	//	case 3:ADCSample[i] = (-0x7fff);break;
	//	}
	//		
	//	//	ff+=0.1;
	//	}
	//	WindowCalc(ADCSample, 0); // Window Real Data, and convert to
	//	// differential if it is single-ended
	//	Bit_Reverse(ADCSample); // Sort Real (Input) Data in bit-reverse
	//	// order
	//	Int_FFT(ADCSample, Imag); // Perform FFT on data

	//}
	IniDev();//��ʼ���豸
	/////////////////////��ѭ��//////////////////////

	while(1)
	{
		PollingMain();
	}
	return 0;
}
