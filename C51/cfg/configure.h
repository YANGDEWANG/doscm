#ifndef _CONFIGURE_ALL_H_
#define _CONFIGURE_ALL_H_


#define MZ_01


#if defined(RE_TEST)
#include "re\test\cfg\configure.h"
#elif defined(SEA12)
#include "re\sea12\cfg\configure.h"
#elif defined(LCD)
#include "re\LCD\cfg\configure.h"
#elif defined(IR_TEST)
#include "re\ir_test\cfg\configure.h"
#elif defined(MZ_01)
#include "re\MZ_01\cfg\configure.h"
#endif

#endif//_CONFIGURE_ALL_H_
