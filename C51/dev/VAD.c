#include <global.h>
#include "VAD.h"

#define clOUT()		(VAD_OUT=false)
#define setOUT()	(VAD_OUT=true)
uint8 VADGet()
{
	uint8 dat = 0;
	setOUT();//注意时间有可能电容放电未完全
	clOUT();
	while((++dat==VAD_MAX)||VAD_IN)//7个指令周期
	{
		//_nop_();可以插入适当的指令延长时间插入后上面的周期将变为9个
	}
	return dat;
}