#include <global.h>
#include "arith.h"

/*****************************************
���޶��Ľ����ڣ��������ޣ���תָ��
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
return��	���
*****************************************/
uint8 EddyMovePointer(uint8 min,uint8 max,uint8 pointer,bool direction)
{
	return MoveOREddyPointer(min,max,pointer,direction,true);
}
