#include <global.h>
#include "VAD.h"

#define clOUT()		(VAD_OUT=false)
#define setOUT()	(VAD_OUT=true)
uint8 VADGet()
{
	uint8 dat = 0;
	setOUT();//ע��ʱ���п��ܵ��ݷŵ�δ��ȫ
	clOUT();
	while((++dat==VAD_MAX)||VAD_IN)//7��ָ������
	{
		//_nop_();���Բ����ʵ���ָ���ӳ�ʱ��������������ڽ���Ϊ9��
	}
	return dat;
}