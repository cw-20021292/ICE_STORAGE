/**
 * File : error.c
 * 
 * Error
*/
#include "error.h"
#include "eeprom.h"
#include "rtc.h"
#include "comp_bldc.h"
#include "flow_meter.h"
#include "display_lib.h"
#include "process_display.h"

SError_T Err;

void InitError(void)
{
    MEMSET( (void __FAR *)&Err, 0, sizeof(SError_T) );
}

void GetErrorData(SError_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Err, sizeof(SError_T) );
}

void SetErrorId(EErrorId_T id)
{
    if( id >= ERR_ID_MAX )
    {
        return;
    }

    Err.ErrorId = id;
}

EErrorId_T GetErrorId(void)
{
    return Err.ErrorId;
}

void GetErrorDate(SErrorDate_T *pDate)
{
    TimeData_T time;

    if( pDate != NULL )
    {
        GetRtcTime( &time );

        pDate->Year = time.Year + 2000;
        pDate->Month = time.Month;
        pDate->Date = time.Date;
    }
}

void SetErrorStatus(EErrorId_T id, U8 status)
{
    if( id >= ERR_ID_MAX )
    {
        return;
    }

    Err.Status[id] = status;
}

U8 GetErrorStatus(EErrorId_T id)
{
    return Err.Status[id];
}

void SetErrorStatusDate(EErrorId_T id, U8 status, SErrorDate_T *pDate)
{
    if( id < ERR_ID_MAX ) 
    {
        Err.Status[id] = status;
        MEMCPY( (void __FAR *)&Err.StatusDate[id], (const void __FAR *)pDate, sizeof(SErrorDate_T) );
    }
}

void GetErrorStatusDate(EErrorId_T id, SErrorDate_T *pDate)
{
    if( id < ERR_ID_MAX ) 
    {
        MEMCPY( (void __FAR *)pDate, (const void __FAR *)&Err.StatusDate[id], sizeof(SErrorDate_T) );
    }
}

U8	IsError(EErrorId_T id)
{
    return Err.Status[id];
}

void SetMementoError(U8 index, EErrorId_T error, U8 subError, SErrorDate_T *pDate)
{
    if( index >= MEMENTO_LIST_NUM )
    {
        return ;
    }

    if( error >= ERR_ID_MAX )
    {
        return ;
    }

    Err.MementoList[index].Error = error;
    Err.MementoList[index].SubError = subError;
    MEMCPY( (void __FAR *)&Err.MementoList[index].ErrorDate, (const void __FAR *)pDate, sizeof(SErrorDate_T));
}

U8 GetMementoError(U8 index, SMemento_T *pMemento)
{
    if( index < MEMENTO_LIST_NUM )
    {
        MEMCPY( (void __FAR *)pMemento, (const void __FAR *)&Err.MementoList[index], sizeof(SMemento_T) );
        return TRUE;
    }

    return FALSE;
}

U8 IsEmptyMementoError(void)
{
    U8 num = 0U;
    SMemento_T memento;

    num = (MEMENTO_LIST_NUM - (1));
    GetMementoError(num, &memento);
    if( memento.Error == ERR_ID_NONE )
    {
        return TRUE;
    }

    return FALSE;
}

void ClearMementoError(void)
{
    U8  i = 0U;
    SErrorDate_T ErrorDate;

    MEMSET( (void __FAR *)&ErrorDate, 0, sizeof(SErrorDate_T) );
    for( i=0; i<MEMENTO_LIST_NUM; i++ )
    {
        SetMementoError(i, ERR_ID_NONE, COMP_BLDC_ERR_ID_NONE, &ErrorDate);
        SaveEepromId(EEP_ID_MEMENTO_1);
        SaveEepromId(EEP_ID_MEMENTO_2);
        SaveEepromId(EEP_ID_MEMENTO_3);
        SaveEepromId(EEP_ID_MEMENTO_4);
        SaveEepromId(EEP_ID_MEMENTO_5);
    }
}

static void UpdateMementoError(SMemento_T *pMemento)
{
    if( pMemento->Error != ERR_ID_NONE )
    {
        // 바로 직전 에러와 현재 에러가 같으면 메멘토 리스트에
        // 추가하지 않는다. 
        if( Err.MementoList[MEMENTO_LIST_NUM - 1].Error 
            != pMemento->Error )
        {
            // 0번이 가장 마지막 에러
            // 4번이 가장 최근 에러
            MEMCPY( (void __FAR *)&Err.MementoList[ 0 ], (const void __FAR *)&Err.MementoList[1], sizeof(SMemento_T) * (MEMENTO_LIST_NUM - 1) ); 
            MEMCPY( (void __FAR *)&Err.MementoList[ MEMENTO_LIST_NUM - 1 ], (const void __FAR *)pMemento, sizeof(SMemento_T) );

            SaveEepromId(EEP_ID_MEMENTO_1);
            SaveEepromId(EEP_ID_MEMENTO_2);
            SaveEepromId(EEP_ID_MEMENTO_3);
            SaveEepromId(EEP_ID_MEMENTO_4);
            SaveEepromId(EEP_ID_MEMENTO_5);
        }
    }
}

void UpdateNewError(EErrorId_T error)
{
    SMemento_T memento;

    // EVA 에러는 Skip
    if( error == ERR_ID_TEMP_EVA_1 
            || error == ERR_ID_TEMP_EVA_2 )
    {
        return;
    }

    if( Err.ErrorId != error )
    {
        Err.PrevErrorId = Err.ErrorId;

        memento.Error = error;
        if( (memento.Error >= ERR_ID_COMP_BLDC_ERR9)
            && (memento.Error <= ERR_ID_COMP_BLDC_ERR1) )
        {
            memento.SubError = (U8)GetCompBldcLastedError();
        }
        else
        {
            memento.SubError = 0U;
        }
        
        GetErrorStatusDate(memento.Error, &memento.ErrorDate);

        UpdateMementoError(&memento);

        StartDispTimerId(DISP_TIMER_ID_ERROR);
    }

    Err.ErrorId = error;
}

/// 단수 에러 해제
void ClearOutageError(void)
{
    /// 단수 해제...
    if( GetErrorStatus(ERR_ID_OUTAGE_WATER) == TRUE )
    {
        SetErrorStatus(ERR_ID_OUTAGE_WATER, FALSE);
        SetOutageStatus(OUTAGE_TYPE_WATER, FLOW_SUPPLY);
    }
}

/// Nos Valve 잠금 에러
//U8 IsErrorTypeNosFeed(void)
U8 IsErrorNosFeed(void)
{
    if( IsError(ERR_ID_ROOM_OVF) == TRUE 
            || IsError(ERR_ID_ROOM_COMPLEX) == TRUE 
            || IsError(ERR_ID_ROOM_HIGH_LEVEL) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

/// 냉수 생성 중지 에러
//U8 IsErrorTypeColdWater(void)
U8 IsErrorColdMake(void)
{
    if( IsError(ERR_ID_COLD_LOW_LEVEL) == TRUE 
            || IsError(ERR_ID_TEMP_COLD_WATER) == TRUE 
            || IsError(ERR_ID_CIRCULATE_PUMP) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR1) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR2) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR3) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR4) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR5) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR6) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR7) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR8) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR9) == TRUE
            || IsError(ERR_ID_COMP_BLDC_COMM) == TRUE
            || IsError(ERR_ID_PROTECT_COMP) == TRUE
            || IsError(ERR_ID_PROTECT_COMP_COMM) == TRUE       
      )
    {
        return TRUE;
    }

    return FALSE;
}

/* 온수 생성 중지 에러 */
//U8 IsErrorTypeHotWater(void)
U8 IsErrorHotMake(void)
{
    if( IsError(ERR_ID_TEMP_HOT_WATER) == TRUE
            || IsError(ERR_ID_OVER_HOT_WATER) == TRUE
            || IsError(ERR_ID_ROOM_COMPLEX) == TRUE
            || IsError(ERR_ID_ROOM_HIGH_LEVEL) == TRUE 
            || IsError(ERR_ID_ROOM_OVF) == TRUE 
            || IsError(ERR_ID_ROOM_LOW_LEVEL) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

//U8 IsErrorTypeIce(void)
U8 IsErrorIceMake(void)
{
    if( IsError(ERR_ID_TEMP_COLD_WATER) == TRUE
            || IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE
            || IsError(ERR_ID_ROOM_COMPLEX) == TRUE
            || IsError(ERR_ID_ROOM_HIGH_LEVEL) == TRUE 
            || IsError(ERR_ID_ROOM_OVF) == TRUE 
            || IsError(ERR_ID_ROOM_LOW_LEVEL) == TRUE 
            || IsError(ERR_ID_COLD_LOW_LEVEL) == TRUE 
            || IsError(ERR_ID_DRAIN_PUMP) == TRUE 
            || IsError(ERR_ID_MICRO_SW_DETECT) == TRUE 
            || IsError(ERR_ID_MICRO_SW_MOVE) == TRUE
            || IsError(ERR_ID_MICRO_SW_MOVE_DEICING) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR1) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR2) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR3) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR4) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR5) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR6) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR7) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR8) == TRUE
            || IsError(ERR_ID_COMP_BLDC_ERR9) == TRUE
            || IsError(ERR_ID_COMP_BLDC_COMM) == TRUE
            || IsError(ERR_ID_PROTECT_COMP) == TRUE
            || IsError(ERR_ID_PROTECT_COMP_COMM) == TRUE
      )
    {
        return TRUE;
    }

    return FALSE;
}

//U8 IsErrorTypeDrainWater(void)
U8 IsErrorDrainWater(void)
{
    if(  IsError(ERR_ID_ROOM_COMPLEX) == TRUE
            || IsError(ERR_ID_ROOM_HIGH_LEVEL) == TRUE 
            || IsError(ERR_ID_ROOM_OVF) == TRUE 
            || IsError(ERR_ID_ROOM_LOW_LEVEL) == TRUE 
            || IsError(ERR_ID_COLD_LOW_LEVEL) == TRUE 
            || IsError(ERR_ID_DRAIN_PUMP) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

//U8 IsErrorTypeSter(void)
U8 IsErrorSter(void)
{
    if( IsErrorDrainWater() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsErrorType(EErrorType_T type)
{
    U8 isError = FALSE;

    switch( type )
    {
        case ERROR_TYPE_NOSFFEED:
            isError = IsErrorNosFeed();
            break;
    
        case ERROR_TYPE_COLD_MAKE:
            isError = IsErrorColdMake();
            break;

        case ERROR_TYPE_HOT_MAKE:
            isError = IsErrorHotMake();
            break;
        
        case ERROR_TYPE_ICE_MAKE:
            isError = IsErrorIceMake();
            break;

        case ERROR_TYPE_DRAIN :
            isError = IsErrorDrainWater();
            break;

        default :
            break;
    }

    return isError;
}
