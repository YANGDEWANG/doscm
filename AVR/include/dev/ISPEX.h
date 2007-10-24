#include "ISP.h"
#include <avr/pgmspace.h>
#ifndef _ISPEX_H_
#define _ISPEX_H_

#include "ISPEX.h"
#include <util/delay.h>

#ifndef _IN_ISPEX_C_
extern bool ISP_Erasure();
extern bool ISP_WriteFlash(prog_char* data,u16 size);
extern bool ISP_WriteEeporm(prog_char* data,u16 size);
extern bool ISP_VerifyFlash(prog_char* data,u16 size);
extern bool ISP_VerifyEeeprom(prog_char* data,u16 size);
extern bool ISP_WriteLockBit(uint8 data);
extern bool ISP_WriteFuse(uint8 LFuseByte,uint8 HFuseByte);
#endif//_IN_ISPEX_C_
#endif//_ISPEX_H_
