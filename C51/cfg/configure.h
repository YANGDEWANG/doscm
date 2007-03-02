#ifndef _CONFIGURE_ALL_H_
#define _CONFIGURE_ALL_H_


#define IR_TEST


#if defined(RE_TEST)
#include "re\test\cfg\configure.h"
#elif defined(SEA12)
#include "re\sea12\cfg\configure.h"
#elif defined(LCD)
#include "re\LCD\cfg\configure.h"
#elif defined(IR_TEST)
#include "re\ir_test\cfg\configure.h"
#endif

#endif//_CONFIGURE_ALL_H_
