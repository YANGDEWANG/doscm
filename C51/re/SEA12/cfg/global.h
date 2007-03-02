#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "configure.h"
#include "user_ini.h"
#include "..\system\eeprom_ex.h"
#include "..\system\boolean.h"
#include <MessageString.h>
#include "..\UI\display.h"
#include "..\system\kernel.h"
#include "..\polling.h"
#include "..\dev\M62446.h"
extern uint8 perchdata dislinebuf[DIS_LINEBUF_SIZE];//×Ö·û²Ù×÷»º´æ
extern prog_char * code MessageString[MS_MAX];
#endif	//_GLOBAL_H_	
