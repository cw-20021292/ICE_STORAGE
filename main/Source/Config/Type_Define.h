/// @file     Type_Define.h
/// @date     2025/02/17
/// @author   Jaejin Ham
/// @brief    프로그램에 사용된 데이터 형태 선언 헤더 file


#ifndef _TYPE_DEFINE_H_
#define _TYPE_DEFINE_H_


#define true                            1
#define false                           0
#define TRUE                            1
#define FALSE                           0

#define HIGH                            1
#define LOW                             0

#define ON                              1
#define OFF                             0
#define N_ON                            0         // NEGATIVE ON
#define N_OFF                           1         // NEGATIVE OFF

#define ENABLE                          1
#define DISABLE                         0

#define SET                             1
#define CLEAR                           0

#define YES                             1
#define NO                              0

#define OPEN                            1
#define CLOSE                           0

#define INDEX_ZERO                      0
#define ZERO                            0

// 선택 option define
#define USE                             1
#define NO_USE                          0


//***************************************************
//*                    DATA TYPES                   *
//***************************************************
typedef char                            I8;
typedef unsigned char                   U8;
typedef signed char                     S8;
typedef const char                      CI8;
typedef const unsigned char             CU8;

typedef int                             I16;
typedef unsigned int                    U16;
typedef signed int                      S16;
typedef const int                       CI16;
typedef const unsigned int              CU16;

typedef unsigned long int               U32;
typedef signed long                     S32;
typedef float                           F32;

typedef double                          D64;


//***************************************************
//*                  struct & union                 *
//***************************************************
typedef struct
{
    unsigned char               b0:1;
    unsigned char               b1:1;
    unsigned char               b2:1;
    unsigned char               b3:1;
    unsigned char               b4:1;
    unsigned char               b5:1;
    unsigned char               b6:1;
    unsigned char               b7:1;
} SBYTE_FIELD;

typedef struct
{
  unsigned char                 low_nibble:4;
  unsigned char                 high_nibble:4;
} SBYTE_FIELD1;

typedef union
{
    unsigned char byte;
    SBYTE_FIELD Bit;
} UTYPE_BYTE;

typedef struct
{
    unsigned char               b0:1;
    unsigned char               b1:1;
    unsigned char               b2:1;
    unsigned char               b3:1;
    unsigned char               b4:1;
    unsigned char               b5:1;
    unsigned char               b6:1;
    unsigned char               b7:1;
    unsigned char               b8:1;
    unsigned char               b9:1;
    unsigned char               b10:1;
    unsigned char               b11:1;
    unsigned char               b12:1;
    unsigned char               b13:1;
    unsigned char               b14:1;
    unsigned char               b15:1;
} SWORD_FIELD;

typedef union
{
    unsigned int                word;
    unsigned char               byte[2];
    SWORD_FIELD                 Bit;
} UTYPE_WORD;

typedef struct
{
	unsigned int                b0:1;
	unsigned int                b1:1;
	unsigned int                b2:1;
	unsigned int                b3:1;
	unsigned int                b4:1;
	unsigned int                b5:1;
	unsigned int                b6:1;
	unsigned int                b7:1;
	unsigned int                b8:1;
	unsigned int                b9:1;
	unsigned int                b10:1;
	unsigned int                b11:1;
	unsigned int                b12:1;
	unsigned int                b13:1;
	unsigned int                b14:1;
	unsigned int                b15:1;
	unsigned int                b16:1;
	unsigned int                b17:1;
	unsigned int                b18:1;
	unsigned int                b19:1;
	unsigned int                b20:1;
	unsigned int                b21:1;
	unsigned int                b22:1;
	unsigned int                b23:1;

	unsigned int                b24:1;
	unsigned int                b25:1;
	unsigned int                b26:1;
	unsigned int                b27:1;
	unsigned int                b28:1;
	unsigned int                b29:1;
	unsigned int                b30:1;
	unsigned int                b31:1;
} SLONG_FIELD;

typedef union
{
	unsigned long               dward;
	unsigned int                word[2];
	unsigned char               byte[4];
	SLONG_FIELD                 Bit;
} UTYPE_LONG;


#endif
