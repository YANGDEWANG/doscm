#ifndef _ARITH_H_
#define _ARITH_H_
#include <configure.h>

#ifndef ArithType
#define ArithType uint8
#endif

/*****************************************
���޶��Ľ����ڣ��������ޣ���תָ����ƶ�
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
Eddy��		���Ϊtrue��������ת����
return��	���
*****************************************/
ArithType MoveOREddyPointer(ArithType min,ArithType max,ArithType pointer,bool direction,bool Eddy);

/*****************************************
���޶��Ľ����ڣ��������ޣ��ƶ�ָ��
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
return��	���
*****************************************/
uint8 MovePointer(uint8 min,uint8 max,uint8 pointer,bool direction);

/*****************************************
���޶��Ľ����ڣ��������ޣ���תָ��
min��		����
max��		����
pointer��	��ǰָ��
direction��	����true:���false:��С��
return��	���
*****************************************/
uint8 EddyMovePointer(uint8 min,uint8 max,uint8 pointer,bool direction);
#endif//_ARITH_H_
