#ifndef _TYPEDEFINE_H_
#define _TYPEDEFINE_H_

#ifndef __C51__
#define __C51__
#endif
#ifdef ALL_IN_DATA 
typedef signed	 char 	data int8;
typedef signed	 int	data int16;
typedef signed	 long	data int32;
typedef unsigned char	data uchar;
typedef unsigned int	data uint;
typedef unsigned long	data ulong;
#else//ALL_IN_DATA
typedef signed	 char 	 int8;
typedef signed	 int	 int16;
typedef signed	 long	 int32;
typedef unsigned char	 uchar;
typedef unsigned int	 uint;
typedef unsigned long	 ulong;
#endif//ALL_IN_DATA

typedef uchar	BYTE;
typedef uchar	uint8;
typedef uint	uint16;
typedef ulong	uint32;

typedef unsigned char	_uchar;
typedef unsigned int	_uint;
typedef unsigned long	_ulong;

typedef signed	 char 	_int8;
typedef signed	 int	_int16;
typedef signed	 long	_int32;
typedef _uchar			_uint8;
typedef _uint			_uint16;

#ifdef __AVR__
typedef uint8	bool;
#define perchdata 
#endif
#ifdef __C51__
#define inline//C51不支持
typedef char code prog_char;
typedef bit	bool;
#define perchdata idata
#endif


//enum Boolean{false,true,};
#define true	(1)
#define false	(0)
#define null    (0)
//---------------------有用宏------------------------------//
#ifndef _BV
#define _BV(b) (1<<b)
#endif
#define GLUE(a, b)     a##b

#define GPORT(x)       GLUE(PORT, x)
#define GDDR(x)        GLUE(DDR, x)
#define GPIN(x)        GLUE(PIN, x)

#endif	//_TYPEDEFINE_H
