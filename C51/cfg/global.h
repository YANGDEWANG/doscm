#ifndef _GLOBAL_ALL_H_
#define _GLOBAL_ALL_H_
#include "configure.h"

#if defined(RE_TEST)
#include "re\test\cfg\global.h"
#elif defined(SEA12)
#include "re\sea12\cfg\global.h"
#elif defined(LCD)
#include "re\LCD\cfg\global.h"
#elif defined(IR_TEST)
#include "re\ir_test\cfg\global.h"
#endif

#endif	//_GLOBAL_ALL_H_	
