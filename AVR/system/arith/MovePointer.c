#include <global.h>
#include "arith.h"

/*****************************************
���޶��Ľ����ڣ��������ޣ��ƶ�ָ��
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
return��	���
*****************************************/
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)
{
	return MoveOREddyPointer(min,max,pointer,direction,false);
}
