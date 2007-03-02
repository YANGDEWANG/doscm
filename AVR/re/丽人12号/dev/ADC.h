#include <configure.h>
#ifndef _ADC_H_
#define _ADC_H_

#ifndef ADC_CFG
//---------------------ADC配置--------------------------//
#define ADC_CFG
#define ADC_MUXCFG			(ADC_REFS_AVCC|_BV(ADLAR))
#define ADC_SAMPLE_COUNT	64
#define ADC_PS				ADC_PS_64
//--------------------ADC配置完-------------------------//
#endif//ADC_CFG

/*
--------------------------------------------------------------
ADC 多工选择寄存器－ ADMUX
REFS1	REFS0	参考电压选择
0		0		AREF， 内部Vref 关闭
0		1		AVCC， AREF 引脚外加滤波电容
1		0		保留
1		1		2.56V 的片内基准电压源， AREF 引脚外加滤波电容
ADLAR: ADC 转换结果左对齐
Bits 3:0 – MUX3:0: 模拟通道选择位
--------------------------------------------------------------
*/
#define ADC_REFS_AREF (0)
#define ADC_REFS_AVCC (_BV(REFS0))
#define ADC_REFS_256  (_BV(REFS0)|_BV(REFS1))
/*
ADC 控制和状态寄存器A －ADCSRA
--------------------------------------------------------------
Bit 7 – ADEN: ADC 使能
Bit 6 – ADSC: ADC 开始转换
Bit 5 – ADFR: ADC 连续转换选择
Bit 4 – ADIF: ADC 中断标志
Bit 3 – ADIE: ADC 中断使能
Bits 2:0 – ADPS2:0: ADC 预分频器选择位
--------------------------------------------------------------
*/
//ADC 预分频器选择位
#define ADC_PS_2	1
#define ADC_PS_4	2
#define ADC_PS_8	3
#define ADC_PS_16	4
#define ADC_PS_32	5
#define ADC_PS_64	6
#define ADC_PS_128	7



extern int16 ADCSample[ADC_SAMPLE_COUNT];
extern volatile bool	ADCEnd;//ADC结束

void StADC(uint8 Channel);//开始ADC
#endif	//_ADC_H_
