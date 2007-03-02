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
#include "math.h"
#include "M62429P.h"
//#include "SPI.h"
//#include "displayAudio.h"
//#include "../ui/Display.h"
#include "Clock.h"
//#include "UserEventManage.h"
#include "eeprom_ex.h"
#include <avr/wdt.h> 
#include <fft.h> 
#include "pointvfddisplay.h"
//#include "ISP.h"
void IniDev()
{
	/////////////////初始IO///////////////////////////


	/////////////////初始化设备///////////////////////////


	//EA = 1;
	sei();
	IniKEYInput();
	//VSPI2_MasterInit();
	//IniDisplay();
	////ShowMesStr(MS_HELLO);
	IniEeprom_ex();
	//IniIIC();
	stf16360enInit();
	IniM62429P();
	//IniPT2314_2();
	//IniPT2314();
	//IniPT2314();
	ControlClock(true);
	InitDisplay();
//	ADCInit();


}

int main()//                         测试程序
{ 
	uint8 delt = 0;
	while(delt--)
	{
		_delay_ms(10);//开机延时
	}
	//while(1)
	{
		int i =0;
		for(;i<sizeof(ADCSample)/sizeof(int16);i++)
		{
		switch(i%4)
		{
		case 2:
		case 0:ADCSample[i] = (0);break;
		case 1:ADCSample[i] = (0x7fff);break;
		case 3:ADCSample[i] = (-0x7fff);break;
		}
			
		//	ff+=0.1;
		}
		WindowCalc(ADCSample, 0); // Window Real Data, and convert to
		// differential if it is single-ended
		Bit_Reverse(ADCSample); // Sort Real (Input) Data in bit-reverse
		// order
		Int_FFT(ADCSample, Imag); // Perform FFT on data

	}
	IniDev();//初始化设备
	/////////////////////主循环//////////////////////

	while(1)
	{
		PollingMain();
	}
	return 0;
}
