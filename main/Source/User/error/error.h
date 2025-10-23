#ifndef __ERROR_H__
#define __ERROR_H__

#include "prj_type.h"

/* Error Id List
 *
 * Top - Low priority for error display 
 * Bottom - Top Priority for error dispaly
 */
typedef enum _error_id_
{
    ERR_ID_NONE,
    //ERR_ID_STR_MOUDLE,
    ERR_ID_MICRO_SW_MOVE_DEICING,
    ERR_ID_MICRO_SW_MOVE,
    ERR_ID_MICRO_SW_DETECT,
    ERR_ID_GAS_SW_VALVE,      /* Gas Switch Valve */
    
    ERR_ID_TEMP_EVA_2,
    ERR_ID_TEMP_EVA_1,
    ERR_ID_OVER_HOT_WATER,     /* 온수 과열 */
    ERR_ID_TEMP_HOT_WATER,
    ERR_ID_TEMP_AMBIENT_2,
    ERR_ID_TEMP_AMBIENT,
    ERR_ID_TEMP_ROOM_WATER,
    ERR_ID_TEMP_COLD_WATER,

    ERR_ID_COMP_BLDC_ERR9,   // 7 : BLDC COMPRESSOR ERR9 ( LG : High Voltage error(12), SAMSUNG :  )
    ERR_ID_COMP_BLDC_ERR8,   // 7 : BLDC COMPRESSOR ERR8 ( LG : Low Voltage error(11), SAMSUNG :  )
    ERR_ID_COMP_BLDC_ERR7,   // 7 : BLDC COMPRESSOR ERR7 ( LG : Overtemp error(10), SAMSUNG : )    
    ERR_ID_COMP_BLDC_ERR6,   // 7 : BLDC COMPRESSOR ERR6 ( LG : Overload Protection(9), SAMSUNG : Abnormal Voltage(4) )
    ERR_ID_COMP_BLDC_ERR5,   // 8 : BLDC COMPRESSOR ERR5 ( LG : Communication error(8), SAMSUNG : Disconnection of Communication(6) )
    ERR_ID_COMP_BLDC_ERR4,   // 9 : BLDC COMPRESSOR ERR4 ( LG : IPM Fault(7), SAMSUNG : Overheat of the controller(5) )
    ERR_ID_COMP_BLDC_ERR3,   // 10 : BLDC COMPRESSOR ERR3 ( LG : Over current error(6), SAMSUNG : Over current error(3) )
    ERR_ID_COMP_BLDC_ERR2,   // 11 : BLDC COMPRESSOR ERR2 ( LG : Temp sensing error(5), SAMSUNG : Starting Fail(1) )
    ERR_ID_COMP_BLDC_ERR1,   // 12 : BLDC COMPRESSOR ERR1 ( LG : Ad Offset error(1), SAMSUNG : Current Sensing Tirp(2) )

    ERR_ID_COMP_BLDC_COMM,     // 13 : BLDC COMP COMM ERROR

    ERR_ID_PROTECT_COMP,       // 14 : PROTECTION BLDC COMPRESSOR
    ERR_ID_PROTECT_COMP_COMM,  // 15 : PROTECTION BLDC COMM.

    ERR_ID_COLD_LOW_LEVEL,    /* low */
    ERR_ID_ROOM_HIGH_LEVEL,   /* high */
    ERR_ID_ROOM_LOW_LEVEL,    /* low */
    ERR_ID_ROOM_COMPLEX,      /* high, low */
    ERR_ID_ROOM_OVF,          /* feed valve */

    ERR_ID_CIRCULATE_PUMP,
    ERR_ID_DRAIN_PUMP,
    
    ERR_ID_OUTAGE_WATER,

    ERR_ID_LEAK,
    ERR_ID_MAX
} EErrorId_T;

/* ERROR ISSUED DATE */
typedef struct _error_date_
{
    U16 Year;
    U8 Month;
    U8 Date;
} SErrorDate_T;

typedef struct _memento_
{
    EErrorId_T Error;
    U8 SubError;
    SErrorDate_T ErrorDate;
} SMemento_T;

/* ERROR TYPE */
#define  MEMENTO_LIST_NUM   (5)
typedef struct _error_
{
    EErrorId_T PrevErrorId;               /// Previous ERROR STATUS
    EErrorId_T ErrorId;                   /// Current ERROR STATUS */
    SErrorDate_T ErrorDate;               /// Current ERROR ISSUED DATE

    U8 Status[ERR_ID_MAX];                /// TRUE : ERROR, FALSE : NORMAL
    SErrorDate_T StatusDate[ERR_ID_MAX];  /// Current ERROR ISSUED DATE

    /* Memento */
    SMemento_T MementoList[MEMENTO_LIST_NUM];  /// 발생된 에러
} SError_T;

typedef enum _error_type_
{
    ERROR_TYPE_NONE = 0,      // 0 :

    ERROR_TYPE_NOSFFEED,      // 0 :

    ERROR_TYPE_COLD_MAKE,     // 0 :
    ERROR_TYPE_HOT_MAKE,      // 0 :
    ERROR_TYPE_ICE_MAKE,      // 0 :    
    
    ERROR_TYPE_DRAIN,         // 0 :    
    
    ERROR_TYPE_MAX,           // 0 :
} EErrorType_T;

void InitError(void);

void GetErrorData(SError_T *pData);

void SetErrorId(EErrorId_T id);
EErrorId_T GetErrorId(void);

void GetErrorDate(SErrorDate_T *pDate);

void SetErrorStatus(EErrorId_T id, U8 status);
U8 GetErrorStatus(EErrorId_T id);

void SetErrorStatusDate(EErrorId_T id, U8 status, SErrorDate_T *pDate);
void GetErrorStatusDate(EErrorId_T id, SErrorDate_T *pDate);
U8 IsError(EErrorId_T id);

void SetMementoError(U8 index, EErrorId_T error, U8 subError, SErrorDate_T *pDate);
U8 GetMementoError(U8 index, SMemento_T *pMemento);
U8 IsEmptyMementoError(void);
void ClearMementoError(void);

void UpdateNewError(EErrorId_T error);

void ClearOutageError(void);

U8 IsErrorType(EErrorType_T type);

#endif /* _ERROR_H__ */
