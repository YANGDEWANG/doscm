#ifndef _TYPEDEFINE_H_
#define _TYPEDEFINE_H_

#ifndef __AVR__
#define __AVR__
#endif
typedef int8_t 		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;
typedef uint8_t		uchar;
typedef uint16_t	uint;
typedef uint32_t	ulong;
typedef uchar		BYTE;
typedef uchar		uint8;
typedef uint		uint16;
typedef ulong		uint32;
typedef uint64_t	uint64;

typedef	int8	i8;
typedef	int16	i16;
typedef	int32	i32;
typedef	int64	i64;
typedef	uint8	u8;
typedef	uint16	u16;
typedef	uint32	u32;
typedef	uint64	u64;

typedef uchar	_uchar;
typedef uint	_uint;
typedef ulong	_ulong;

typedef int8 	_int8;
typedef int16	_int16;
typedef int32	_int32;
typedef _uchar	_uint8;
typedef _uint	_uint16;

#ifdef __AVR__
#ifndef __cplusplus
typedef uint8	bool;
#endif
#define perchdata 
#define data 
#define code PROGMEM
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

typedef union
{ 
	_int8 b8_0; 
	struct
	{
		_int8 b1_0:1;
		_int8 b1_1:1;
		_int8 b1_2:1;
		_int8 b1_3:1;
		_int8 b1_4:1;
		_int8 b1_5:1;
		_int8 b1_6:1;
		_int8 b1_7:1;
		
	}b1_8;
}b8;
typedef union
{ 
	_int16 b16_1;
	struct
	{
		_int8 b8_0;
		_int8 b8_1;
	}b8_2;
}b16;
typedef union
{ 
	_int32 b32_1; 
	struct
	{
		_int16 b16_0;
		_int16 b16_1;
	}b16_2;
	struct
	{
		_int8 b8_0;
		_int8 b8_1;
		_int8 b8_2;
		_int8 b8_3;
	}b8_4;
}b32;
//---------------------有用宏------------------------------//

#define ISTRUESTOP(ver)	while(ver)
#define GLUE(a, b)     a##b

#define GPORT(x)       GLUE(PORT, x)
#define GDDR(x)        GLUE(DDR, x)
#define GPIN(x)        GLUE(PIN, x)
#define abs(x)	(x>0?x:-x)

static void inline dwmemset(void *dst,u8 dat,u16 size)
{
	u16 i;
	for(i=0;i<size;i++)
	{
		((u8*)dst)[i]=dat;
	}
}

#endif	//_TYPEDEFINE_H
