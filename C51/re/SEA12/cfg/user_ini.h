#ifndef _USER_INI_H_
#define _USER_INI_H_
#include "../dev/M62446.h"

#define INTPUT_5_1	(M62446OUTPUTPORT_3)
#define INTPUT_CD	(M62446OUTPUTPORT_2)
#define INTPUT_AUX	(M62446OUTPUTPORT_1|M62446OUTPUTPORT_2)

#define DEF_INTPUT INTPUT_5_1
#define DEF_VOLUME 40
#define DEF_ADJUST_VOLUME 0


//¼ÌµçÆ÷
sbit JJ = P2^3;
#define ONJJ()	(JJ=false)
#define OFFJJ() (JJ=true)
//ok ¼ì²â
sbit ok_mode = P2^4;



 /*
sbit INT0 = 0xB2;       // External interrupt 0 
sbit INT1 = 0xB3;      // External interrupt 1 
//------------------------------------------------
//TMOD Bit Values
//------------------------------------------------
#define T0_M0_   0x01
#define T0_M1_   0x02
#define T0_CT_   0x04
#define T0_GATE_ 0x08
#define T1_M0_   0x10
#define T1_M1_   0x20
#define T1_CT_   0x40
#define T1_GATE_ 0x80

#define T1_MASK_ 0xF0
#define T0_MASK_ 0x0F
 */
#endif//_USER_INI_H_
