#include <global.h>
#include "kernel.h"
/*
void _RRC_()  //5个指令周期
{
#pragma asm
	RRC A
#pragma endasm
} 
void _RLC_()  //5个指令周期
{
#pragma asm
	RLC A
#pragma endasm
}*/
/*void Delay(uint8 t)//延时10+t*2个指令周期
{
	t=t;
#pragma asm	
	INC R7
DelayLoop:	DJNZ R7,DelayLoop
#pragma endasm
} 

uint8 BCDtoUchar(uint8 in)
{
	in = in;
#pragma asm	
	MOV  A,R7
	SWAP A
	ANL  A,#0FH
	MOV  B,#0AH
	MUL  AB
	MOV  B,A
	MOV  A,R7
	ANL  A,#0FH
	ADD  A,B
	MOV  R7,A
#pragma endasm
//return R7;
} */
