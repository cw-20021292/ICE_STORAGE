/**
 * File : comp.c
 * 
 * Comp Module
*/
#include "comp_bldc.h"

#include "relay.h"
#include "timer.h"

#define COMP_STABLE_TIME    (3U)   // 3sec...
#define OFF_TIME_5SEC       (5U)   // 30sec..
#define OFF_TIME_30SEC      (30U)  // 30sec..

SCompBldc_T CompBldc;

void InitCompBldc(void)
{
    CompBldc.target = 0U;
    CompBldc.current = 0U;
    CompBldc.max = MAX_RPS;
    CompBldc.min = MIN_RPS;
    CompBldc.temp = 0U;
    
    CompBldc.error = COMP_BLDC_ERR_ID_NONE;
    CompBldc.lastedError = COMP_BLDC_ERR_ID_NONE;
    CompBldc.errorComm = FALSE;
    CompBldc.protect = FALSE;
    CompBldc.testComm = FALSE;
    
    CompBldc.onStableTime = 0U;
    CompBldc.offMaxTime = 0U;
}

void GetCompBldcPwmData(SCompBldc_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&CompBldc, sizeof(SCompBldc_T) );
}

/// SET/GET TARGET RPS
void SetCompBldcTargetRps(RPS_T rps)
{
    CompBldc.target = rps;
}

RPS_T GetCompBldcTargetRps(void)
{
    return CompBldc.target;
}

/// SET/GET CURRENT RPS
void SetCompBldcCurrentRps(RPS_T rps)
{
    CompBldc.current = rps;
}

RPS_T GetCompBldcCurrentRps(void)
{
    return CompBldc.current;
}

// ERROR CODE
void SetCompBldcError(ECompBldcErrorId_T id)
{
    if( id >= COMP_BLDC_ERR_ID_OUT_OF_RANGE )
    {
        id = COMP_BLDC_ERR_ID_OUT_OF_RANGE;
    }

    if( id != COMP_BLDC_ERR_ID_NONE )
    {
        if( CompBldc.lastedError != id )
        {
            CompBldc.lastedError = id;
        }
    }

    CompBldc.error = id;
}

ECompBldcErrorId_T GetCompBldcError(void)
{
    return CompBldc.error;
}

void SetCompBldcLastedError(ECompBldcErrorId_T id)
{
    CompBldc.lastedError = id;
}

ECompBldcErrorId_T GetCompBldcLastedError(void)
{
    return CompBldc.lastedError;
}

/* ERROR CODE Conversion */
typedef struct _comp_bldc_error_conv_list_
{
    ECompBldcErrorId_T    error;
    ECompBldcErrorCode_T  code;
} CompBldcErrConvList_T;

CompBldcErrConvList_T CompBldcErrConvList[] =
{
    {COMP_BLDC_ERR_ID_AD_OFFSET,       COMP_BLDC_ERR_CODE_1},
    {COMP_BLDC_ERR_ID_TEMP_SENSING,    COMP_BLDC_ERR_CODE_2},
    {COMP_BLDC_ERR_ID_OVER_CURRENT,    COMP_BLDC_ERR_CODE_3},
    {COMP_BLDC_ERR_ID_IPM_FAULT,       COMP_BLDC_ERR_CODE_4},
    {COMP_BLDC_ERR_ID_COMM,            COMP_BLDC_ERR_CODE_5},
    {COMP_BLDC_ERR_ID_OVERLOAD_PROT,   COMP_BLDC_ERR_CODE_6},
    {COMP_BLDC_ERR_ID_OVERTEMP,        COMP_BLDC_ERR_CODE_7},   
    {COMP_BLDC_ERR_ID_LOW_VOLTAGE,     COMP_BLDC_ERR_CODE_8},
    {COMP_BLDC_ERR_ID_HIGH_VOLTAGE,    COMP_BLDC_ERR_CODE_9},
    {COMP_BLDC_ERR_ID_OUT_OF_RANGE,    COMP_BLDC_ERR_CODE_10},
};

#define SZ_COMP_BLDC_ERR_CONV_LIST     ( sizeof(CompBldcErrConvList) / sizeof(CompBldcErrConvList_T) )

static U8 FindCompBldcErrorId(ECompBldcErrorId_T id, U8 *pindex )
{
    U8 i = 0;
    U8 result = FALSE;
    CompBldcErrConvList_T *pList = NULL;

    for( i=0; i<SZ_COMP_BLDC_ERR_CONV_LIST; i++ )
    {
        pList = &CompBldcErrConvList[i];
    
        if( pList->error == id )
        {
            *pindex = i;
            result = TRUE;
            break;
        }
    }

    return result;
}

ECompBldcErrorCode_T GetCompBldcErrorCode(ECompBldcErrorId_T id)
{
    U8 findResult = 0;
    U8 index = 0;
    CompBldcErrConvList_T *pList = NULL;

    if( id >= COMP_BLDC_ERR_ID_MAX )
    {
        return COMP_BLDC_ERR_CODE_NONE;
    }

    findResult = FindCompBldcErrorId(id, &index);
    if( findResult == FALSE )
    {
        return COMP_BLDC_ERR_CODE_NONE;
    }

    pList = &CompBldcErrConvList[index];

    return pList->code;
}

/// ERROR Commnuication
void SetCompBldcErrorComm(U8     error)
{
    CompBldc.errorComm = error;
}

U8 GetCompBldcErrorComm(void)
{
    return CompBldc.errorComm;
}

/// PROTECT
void SetCompBldcProtect(U8 protect)
{
    CompBldc.protect = protect;
}

U8 GetCompBldcProtect(void)
{
    return CompBldc.protect;
}

U8 IsCompBldcProtectMode(void)
{
    if( CompBldc.error != COMP_BLDC_ERR_ID_NONE )
    {
        return TRUE;    /// Protect
    }

    if( CompBldc.errorComm  == TRUE )
    {
        return TRUE;    /// Protect
    }

    return FALSE;
}

void SetCompBldcCommTest(U8 test)
{
    CompBldc.testComm = test;
}

U8 GetCompBldcCommTest(void)
{
    return CompBldc.testComm;
}

/// IS TURN ON COMP???
U8 IsTurnOnCompBldc(void)
{
    if( CompBldc.target > 0 )
            //&& CompBldc.ErrorCode == ERR_BLDC_NONE )
    {
        return TRUE;
    }

    return FALSE;
}

/*
 * BLDC_COMP_ON_STEP_CHECK_HOT_HEATER : 
 * 순간온수 식 일 때 Comp를 키면 순간 큰 전류가 걸리는데
 * 차단기가 내려갈 수 있음 이를 방지하기 위해 
 * 히터와 Comp를 동시에 동작시키지 않음
*/
typedef enum _comp_bldc_on_step_
{
    //COMP_ON_BLDC_STEP_CHECK_HOT_HEATER = 0,  
    COMP_ON_BLDC_STEP_INIT = 0,
    COMP_ON_BLDC_STEP_WAIT,
    COMP_ON_BLDC_STEP_CONTROL_COMP_BLDC,
    COMP_ON_BLDC_STEP_RUNNING,
} ECompBldcOnStep_T;

static U8 TurnOn(U8 step)
{
    switch( step )
    {
#if 0
        case COMP_ON_BLDC_STEP_CHECK_HOT_HEATER:
            if( IsExpiredHeaterOnTime() == TRUE )
            {
                step++;
            }
            break;
#endif
    
        case COMP_ON_BLDC_STEP_INIT:
            TurnOnRelay( RELAY_COMP );
            CompBldc.onStableTime = COMP_STABLE_TIME;

            step++;
            break;

        case COMP_ON_BLDC_STEP_WAIT:
            if( CompBldc.onStableTime != 0 )
            {
                CompBldc.onStableTime--;
            }
            else
            {
                step++;
            }
            break;

        case COMP_ON_BLDC_STEP_CONTROL_COMP_BLDC:
            StartTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_REQUEST, 0);   /// 즉시 실행 후 2초마다 주기적으로 RPS 요청하기 위함
            StartTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_RX_ERR, SEC(COMP_BLDC_ERR_COMM_TIMEOUT));
            /// RX_ERR 타이머 값은 BLDC COMP 통신이 정상적이면 다시 시작이기 때문에 정상적이면 Expire 되지 않음

            step++;
            break;

        case COMP_ON_BLDC_STEP_RUNNING:
            // Doing... Running..
            break;

        default:
            break;
    }

    return step;
}

/// 보호동작에 의해서 압축기 OFF시에는
/// 대기 시간을 5초로 단축한다.
/// 압축기 자체 보호동작에 의해서 이미 압축기는 OFF상태이다.
static U16 GetCompOffMaxTime(void)
{
    if( CompBldc.protect == TRUE )
    {
        return OFF_TIME_5SEC;
    }

    return OFF_TIME_30SEC;
}

typedef enum _comp_bldc_off_step_
{
    COMP_BLDC_OFF_STEP_INIT,
    COMP_BLDC_OFF_STEP_WAIT_CURRENT_RPS,
    COMP_BLDC_OFF_STEP_WAIT,
    COMP_BLDC_OFF_STEP_WAIT_BEFORE_OFF_RELAY,
} ECompBldcOffStep;

static U8 TurnOff(U8 step)
{
    switch( step )
    {
        case COMP_BLDC_OFF_STEP_INIT:
            CompBldc.offMaxTime = GetCompOffMaxTime();
            step++;
            break;

        case COMP_BLDC_OFF_STEP_WAIT_CURRENT_RPS:
            if( CompBldc.offMaxTime != 0 )
            {
                CompBldc.offMaxTime--;
            }
            else
            {
                CompBldc.offMaxTime = COMP_STABLE_TIME;
                step++;
                break;
            }
            
            if( GetCompBldcCurrentRps() == 0 )
            {
                step++;
            }
            break;

        case COMP_BLDC_OFF_STEP_WAIT:
            if( CompBldc.onStableTime != 0 )
            {
                CompBldc.onStableTime--;
            }
            else
            {
                step++;
            }
            break;

        case COMP_BLDC_OFF_STEP_WAIT_BEFORE_OFF_RELAY:
            TurnOffRelay(RELAY_COMP);

            SetCompBldcCurrentRps(0);     // feedback rps 초기화
            SetCompBldcProtect(FALSE);    // 보호 동작 해제

            StopTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_REQUEST);
            StopTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_RX_ERR);
            break;

        default:
            break;
    }

    return step;
}

/// CONTROL RPS...   @ 1sec..
void ControlCompBldc(void)
{
    static U8 step = 0U;
    static RPS_T prevRps = 0U;
    RPS_T targetRps = 0U;

    targetRps = GetCompBldcTargetRps();
    if( targetRps != prevRps )
    {

        if( (prevRps == 0) && (targetRps > 0) )
        {
            /// off -> on
            step = 0;
        }
        else if( (prevRps > 0) && (targetRps == 0) )
        {
            /// on -> off
            step = 0;
        }

        prevRps = targetRps;
    }

    if( targetRps > 0 )
    {
        step = TurnOn(step);
    }
    else
    {
        step = TurnOff(step);
    }
}

