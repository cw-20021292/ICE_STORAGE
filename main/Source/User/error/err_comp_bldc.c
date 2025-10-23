/**
 * File : error_bldc_comp.c
 * 
 * Bldc Comp Module
*/
#include "err_comp_bldc.h"

#include "comp.h"

#define CONFIG_COMP_ERROR_ONESHOT   (0)

#if( CONFIG_COMP_ERROR_ONESHOT == 1 )
#define ERR_COUNT_NUM       (0)  // 에러 반복 발생 카운트 ( 1회 발생 에러 )
#define ERR_PRT_COUNT_NUM   (0)
#else
#define ERR_COUNT_NUM       (10) // 에러 반복 발생 카운트 ( 10회 발생 에러 )
#define ERR_PRT_COUNT_NUM   (10)
#endif
//#define RELEASE_TIME        (12UL*60UL*60UL*10UL)  // 12hour = 12h*60m*60s*10 @100ms
#define RELEASE_TIME        (10UL*60UL*10UL)  // 10min = 10m*60s*10 @100ms

typedef struct _err_comp_bldc_
{
    ECompBldcErrorId_T Error;

    U8                 CountList[COMP_BLDC_ERR_ID_MAX];
    U8                 PrtCountList[COMP_BLDC_ERR_ID_MAX];

    U32                ReleaseTime[COMP_BLDC_ERR_ID_MAX];
} SErrCompBldc_T;

SErrCompBldc_T ErrCompBldc;

static void ResetCompBldcErrorCount(void)
{
    U8 i = 0U;

    ErrCompBldc.CountList[ 0 ] = 0;
    ErrCompBldc.PrtCountList[ 0 ] = 0;
    ErrCompBldc.ReleaseTime[ 0 ] = 0;
    for( i=1; i<COMP_BLDC_ERR_ID_MAX; i++ )
    {
        ErrCompBldc.CountList[i] = ERR_COUNT_NUM;
        ErrCompBldc.PrtCountList[i] = ERR_PRT_COUNT_NUM;
        ErrCompBldc.ReleaseTime[i] = RELEASE_TIME;
    }    
}

void InitErrCompBldc(void)
{
    ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;

    ResetCompBldcErrorCount();
}

U8 GetErrCompBldcCount(U8 type, ECompBldcErrorId_T error)
{
    if( type == 0 )
    {
        return ErrCompBldc.PrtCountList[error];
    }

    return ErrCompBldc.CountList[error];
}

// 에러 동작이 발생된 이력이 있으면, TRUE 반환
static U8 IsErrorCount(ECompBldcErrorId_T error)
{
    if( ErrCompBldc.CountList[error] < ERR_COUNT_NUM )
    {
        return TRUE;
    }

    return FALSE;
}

// 보호동작이 발생된 이력이 있으면, TRUE 반환
static U8 IsPrtCount(ECompBldcErrorId_T error)
{
    if( ErrCompBldc.PrtCountList[error] < ERR_PRT_COUNT_NUM )
    {
        return TRUE;
    }

    return FALSE;
}

static void CountDownError(ECompBldcErrorId_T error)
{
    if( ErrCompBldc.CountList[error] != 0 )
    {
        ErrCompBldc.CountList[error]--;
    }
}

static void ClearError(void)
{
    SetCompBldcError(COMP_BLDC_ERR_ID_NONE);
}

/******************************************************************************************************************
 *PROTECT
 */
U8 CheckProtectCompBldc(U8 xError)
{
    ECompBldcErrorId_T error;

    error = GetCompBldcError();
    if( error != COMP_BLDC_ERR_ID_NONE )
    {
        ErrCompBldc.Error = error;

        // error
        if( ErrCompBldc.PrtCountList[error] != 0 )
        {
            ErrCompBldc.PrtCountList[error]--;
            ClearError();
        }
        else
        {
            ErrCompBldc.PrtCountList[error] = ERR_PRT_COUNT_NUM;

            CountDownError(error);
            SetCompBldcProtect(TRUE);

            return TRUE;    // error
        }
    }

    return FALSE;   // normal
}

U8 ReleaseProtectCompBldc(U8 xError)
{
    if( GetCompBldcProtect() == FALSE )
    {
        return FALSE;   // normal
    }

    return TRUE;    // error
}

// 에러 발생 검사
// 에러 발생 전 10분 기준으로 에러 카운트 초기화
/**
 * @ brief                       Function For Bldc Comp Error1 ( Code : E81 )
 * @ detail                      LG : Ad Offset error(1), SAMSUNG : Current Sensing Tirp(2)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr1(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_AD_OFFSET] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_AD_OFFSET  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_AD_OFFSET] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_AD_OFFSET]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_AD_OFFSET) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_AD_OFFSET) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_AD_OFFSET] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error2 ( Code : E82 )
 * @ detail                      Temp sensing error(5), SAMSUNG : Starting Fail(1)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr2(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_TEMP_SENSING] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_TEMP_SENSING  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_TEMP_SENSING] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_TEMP_SENSING]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_TEMP_SENSING) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_TEMP_SENSING) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_TEMP_SENSING] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error3 ( Code : E83 )
 * @ detail                      LG : Over current error(6), SAMSUNG : Over current error(3)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr3(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_OVER_CURRENT] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_OVER_CURRENT  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVER_CURRENT] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVER_CURRENT]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_OVER_CURRENT) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_OVER_CURRENT) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVER_CURRENT] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error4 ( Code : E84 )
 * @ detail                      LG : IPM Fault(7), SAMSUNG : Overheat of the controller(5)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr4(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_IPM_FAULT] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_IPM_FAULT  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_IPM_FAULT] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_IPM_FAULT]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_IPM_FAULT) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_IPM_FAULT) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_IPM_FAULT] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error5 ( Code : E85 )
 * @ detail                      LG : Communication error(8), SAMSUNG : Disconnection of Communication(6)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr5(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_COMM] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_COMM  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_COMM] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_COMM]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_COMM) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_COMM) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_COMM] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error6 ( Code : E86 )
 * @ detail                      LG : Overload Protection(9), SAMSUNG : Abnormal Voltage(4)
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr6(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_OVERLOAD_PROT] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_OVERLOAD_PROT  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERLOAD_PROT] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERLOAD_PROT]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_OVERLOAD_PROT) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_OVERLOAD_PROT) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERLOAD_PROT] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error7 ( Code : E87 )
 * @ detail                      LG : Overtemp error(10), SAMSUNG : 
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr7(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_OVERTEMP] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_OVERTEMP  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERTEMP] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERTEMP]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_OVERTEMP) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_OVERTEMP) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_OVERTEMP] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error8 ( Code : E88 )
 * @ detail                      LG : Low Voltage error(11), SAMSUNG :  
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr8(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_LOW_VOLTAGE] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_LOW_VOLTAGE  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_LOW_VOLTAGE] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_LOW_VOLTAGE]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_LOW_VOLTAGE) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_LOW_VOLTAGE) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_LOW_VOLTAGE] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

/**
 * @ brief                       Function For Bldc Comp Error9 ( Code : E89 )
 * @ detail                      LG : High Voltage error(12), SAMSUNG : 
 * @ param      xUerror          Error
 * @ return                      Error
**/
U8 CheckErrCompBldcErr9(U8 xError)
{
    if( ErrCompBldc.Error != COMP_BLDC_ERR_ID_NONE )
    {
        if( ErrCompBldc.CountList[COMP_BLDC_ERR_ID_HIGH_VOLTAGE] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == COMP_BLDC_ERR_ID_HIGH_VOLTAGE  )
            {
                ErrCompBldc.Error = COMP_BLDC_ERR_ID_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_HIGH_VOLTAGE] != 0 )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_HIGH_VOLTAGE]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // 컴프가 OFF 된 상태 이거나 
        // 컴프가 ON 된 상태에서 에러가 발생된 이력이 없으면 10분 초기화
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount(COMP_BLDC_ERR_ID_HIGH_VOLTAGE) == FALSE 
                && IsPrtCount(COMP_BLDC_ERR_ID_HIGH_VOLTAGE) == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[COMP_BLDC_ERR_ID_HIGH_VOLTAGE] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}
