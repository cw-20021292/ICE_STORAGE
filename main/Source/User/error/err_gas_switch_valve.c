#include "err_gas_switch_valve.h"
#include "gas_switch_valve.h"
#include "comp.h"
#include "valve.h"
#include "level.h"
#include "temp.h"
#include "error.h"

#define ONE_POINT_TIME     10U     // @100ms, 1sec = 10x10 = 10
#define TWO_POINT_TIME     1800U   // @100ms, 3min = 3x60x10 = 3000
#define DETECT_TIME        100U    // @100ms, 10sec = 10x10 = 100
#define RETRY_WAIT_TIME    600U    // @100ms, 3min = 1x60x10 = 1800
#define COUNT_NUM          5
#define TEMP_GAS_VALVE     3.0f
typedef struct _err_gas_valve_
{
    U8 Start;   // 검사 시작,중지

    U8 Error;   // 에러 상태 ( TRUE : 에러, FALSE : 정상 )
    U8 CheckStatus; // 검사시 상태 ( 에러 또는 정상 )
    U16 OnePointTime;   // 최초 온도 확인 위치 이동 시간
    U16 TwoPointTime;   // 비교 온도 확인 위치 이동 시간
    U16 DetectTime;  // 검출 시간
    U16 RetryWaitTime;  // 검사 시간

    U8 Count[2];       // 재검사 횟수
    TEMP_T tEvaCold[2];
    TEMP_T tEvaIce[2];
    TEMP_T tDelta;
} ErrGasValve_T;

ErrGasValve_T ErrGas;

void InitErrGasSwitchValve(void)
{
    ErrGas.Start = FALSE;
    ErrGas.CheckStatus = FALSE;

    ErrGas.Error = FALSE;
    ErrGas.OnePointTime  = 0;
    ErrGas.TwoPointTime  = 0;
    ErrGas.DetectTime = 0;
    ErrGas.RetryWaitTime = 0;
    ErrGas.Count[0]     = COUNT_NUM;
    ErrGas.Count[1]     = COUNT_NUM;

    ErrGas.tEvaCold[0] = 0.0f;
    ErrGas.tEvaCold[1] = 0.0f;
    ErrGas.tEvaIce[0]  = 0.0f;
    ErrGas.tEvaIce[1]  = 0.0f;
    ErrGas.tDelta  = 0.0f;
}

U8 CheckErrGasSwitchValve(U8 mu8Error)
{
    return ErrGas.Error;
}

U8 ReleaseErrGasSwitchValve(U8 mu8Error)
{
    return ErrGas.Error;
}


static U8 GetGasSwitchErrCount(void)
{
    if( GetGasSwitchStatus() == GAS_SWITCH_COLD )
    {
        return ErrGas.Count[0];
    }

    return ErrGas.Count[1];
}


static void SetGasSwitchErrCount(U8 mu8ErrCount )
{
    if( GetGasSwitchStatus() == GAS_SWITCH_COLD )
    {
        ErrGas.Count[0] = mu8ErrCount;
    }
    else
    {
        ErrGas.Count[1] = mu8ErrCount;
    }
}


// Check Comp Status ( ON -> OFF or OFF -> ON )
// RETURN 
//  Chanaged : TRUE
//  Stay : FALSE
static U8 IsChangedStatusComp(void)
{
    static U8 mu8PrevCompStatus     = OFF;
    U8 mu8CurCompStatus;


    mu8CurCompStatus = GetCompOnOff();
    if( mu8PrevCompStatus != mu8CurCompStatus )
    {
        mu8PrevCompStatus = mu8CurCompStatus;

        if( mu8CurCompStatus == ON )
        {
            return TRUE;    // Changed
        }
    }

    return FALSE;   // Stay
}

// Check Comp Status ( ON -> OFF or OFF -> ON )
// RETURN 
//  Chanaged : TRUE
//  Stay : FALSE
static U8 IsChangedStatusGasValve(void)
{
    static U8 mu8PrevGasValveStatus     = OFF;
    U8 mu8CurGasValveStatus;


    mu8CurGasValveStatus = GetGasSwitchStatus();
    if( mu8PrevGasValveStatus != mu8CurGasValveStatus )
    {
        mu8PrevGasValveStatus = mu8CurGasValveStatus;

        return TRUE;    // Changed
    }

    return FALSE;   // Stay
}


static void InitVariable(void)
{
    ErrGas.Start         = TRUE;
    ErrGas.CheckStatus   = FALSE;
    ErrGas.OnePointTime  = ONE_POINT_TIME;
    ErrGas.TwoPointTime  = TWO_POINT_TIME;
    ErrGas.DetectTime    = DETECT_TIME;
    ErrGas.RetryWaitTime = RETRY_WAIT_TIME;
}

// gas switch 에러 검출 여부 확인 함수
// eva 센서가 에러인 경우 검출 안됨.
static U8 IsValidCheckErr(void)
{
    // Comp Off
    if( GetCompOnOff() == OFF )
    {
        return FALSE;
    }

    // 냉수 부하량이 변경
    if( IsOpenValve( VALVE_COLD_IN ) == ON 
            || IsOpenValve( VALVE_COLD_CIRCULATE ) == ON 
            || IsOpenValve( VALVE_ICE_WATER_OUT ) == ON 
            || GetTankLevel( LEVEL_ID_COLD ) != LEVEL_HIGH )
    {
        return FALSE;
    }

    // Err Eva-1
    if( IsError(ERR_ID_TEMP_EVA_1) == TRUE )
    {
        return FALSE;
    }

    // Err Eva-2
    if( IsError(ERR_ID_TEMP_EVA_2) == TRUE )
    {
        return FALSE;
    }


    return TRUE;
}

static void StopCheckErr(U8 mu8Err )
{
    ErrGas.Start = FALSE;
    ErrGas.Error = mu8Err;
}



// EVA 온도 측정 비교할 2point 
static U8 CheckPointTemp(void)
{
    if( ErrGas.OnePointTime != 0 )
    {
        ErrGas.OnePointTime--;

        ErrGas.tEvaCold[0]  = GetTemp( TEMP_ID_EVA_2 );
        ErrGas.tEvaIce[0]   = GetTemp( TEMP_ID_EVA_1 );
    }

    // 비교 EVA 온도 측정
    if( ErrGas.TwoPointTime != 0 )
    {
        ErrGas.TwoPointTime--;

        return FALSE;   // 비교 측정 온도 측정 미완료
    }


    ErrGas.tEvaCold[1] = GetTemp(TEMP_ID_EVA_2 );
    ErrGas.tEvaIce[1]  = GetTemp(TEMP_ID_EVA_1 );
    return TRUE;    // One Point, Two Point 모두 측정 완료
}


static TEMP_T   CalcDeltaTemp(TEMP_T tEvaFrom, TEMP_T tEvaTo)
{
    TEMP_T tDelta;

    tDelta =  tEvaFrom - tEvaTo;

    return tDelta;
}

// 냉매 전환 밸브 위치에 따른 Delta temp 계산
static void CalcDeltaTempOnSide(void)
{
    if( GetGasSwitchStatus() == GAS_SWITCH_COLD )
    {
        ErrGas.tDelta  = CalcDeltaTemp( ErrGas.tEvaCold[0], ErrGas.tEvaCold[1] );
    }
    else
    {
        ErrGas.tDelta  = CalcDeltaTemp( ErrGas.tEvaIce[0], ErrGas.tEvaIce[1] );
    }
}

//static U8 IsValidEvaDeltaTemp(TEMP_T tEva, TEMP_T tDelta )
//{
//    if( tEva > tDelta )
//    {
//        return TRUE;
//    }
//
//    return FALSE;
//}

// 에러 상태 확정
static U8 CheckSwitchStatus(void)
{
    // 두 Eva 온도의 차이가 TEMP_GAS_VALVE 보다 크면 정상조건이다.
    //if( IsValidEvaDeltaTemp(ErrGas.tDelta, TEMP_GAS_VALVE) == TRUE )
    if(  ErrGas.tDelta > TEMP_GAS_VALVE )
    {
        // 상태 변화가 있으면 DetectTime updated
        if( ErrGas.CheckStatus == FALSE )   
        {
            ErrGas.DetectTime = DETECT_TIME;
        }
        ErrGas.CheckStatus = TRUE;  // 정상
    }
    else
    {
        // 상태 변화가 있으면 DetectTime updated
        if( ErrGas.CheckStatus == TRUE )
        {
            ErrGas.DetectTime = DETECT_TIME;
        }
        ErrGas.CheckStatus = FALSE;
    }

    // DetectTime 동안 연속 해서 조건이 유지되는가?
    if( ErrGas.DetectTime != 0 )
    {
        ErrGas.DetectTime--;
        return FALSE ;
    }

    return TRUE;
}


// 냉매 전환 밸브 에러 발생시 재시도
static U8 RetryErrorOnCount(void)
{
    U8 mu8ErrCount;


    // 3회 연속 발생되었으면 에러 발생
    mu8ErrCount = GetGasSwitchErrCount();
    if( mu8ErrCount == 0 )
    {
        return FALSE;   // 재시도 초과, 에러 발생
    }

    // 재시도 대기 시간...
    if( ErrGas.RetryWaitTime != 0 )
    {
        ErrGas.RetryWaitTime--;
    }

    // 에러 카운트를 줄이고, 냉매 전환 밸브를 원점 초기화 한다.
    // TwoPointTime, DetectTime, RetryWaitTime을 재설정한다.
    mu8ErrCount--;
    SetGasSwitchErrCount( mu8ErrCount );
    ErrGas.TwoPointTime = TWO_POINT_TIME;
    ErrGas.DetectTime = DETECT_TIME;
    ErrGas.RetryWaitTime = RETRY_WAIT_TIME;


    return TRUE;    // 재시도
}


// 냉매 전환 밸브 초기화
static void ResetGasSwitchValve(void)
{
    GasSwitchInit();
    if( GetGasSwitchStatus() == GAS_SWITCH_COLD )
    {
        GasSwitchCold();
    }
    else
    {
        GasSwitchIce();
    }
}

// 냉매 전환 밸브 에러 검출
void ProcessErrGasSwitchValve(void)
{
    // 압축기, 냉매 전환 밸브 위치에 변화가 생겼을 때,
    // 관련 변수들을 초기화 시킨다.
    if( IsChangedStatusComp() == TRUE 
            || IsChangedStatusGasValve() == TRUE )
    {
        InitVariable(); // 변수 초기화
    }

    // 에러 검출 동작 수행 여부 확인
    if( IsValidCheckErr() == FALSE )
    {
        // 에러 검출 조건이 아니면 중지..
        ErrGas.Start    = FALSE;
        return ;
    }

    // 검사 시작조건이 아니면 종료.
    if( ErrGas.Start == FALSE )
    {
        return ;
    }

    // 온도 2point 측정( 온도 변화량 계산을 위한 )
    if( CheckPointTemp() == FALSE )
    {
        return ;
    }

    // 온도 변화량 계산
    CalcDeltaTempOnSide();

    // 에러 상태 확인
    if( CheckSwitchStatus() == FALSE )
    {
        return ;    
    }

    // 정상 조건인 경우,
    // 에러 해제, 에러 카운트 초기화
    if( ErrGas.CheckStatus == TRUE )
    {
        SetGasSwitchErrCount( COUNT_NUM );
        StopCheckErr( FALSE ); 
        return ;
    }

    // 에러 발생 ( 재시도 3회 초과 )
    if( RetryErrorOnCount() == FALSE )
    {
        StopCheckErr( TRUE );
        return ;
    }

    // 냉매 전환 밸브 위치를 초기화 시킨다.
    ResetGasSwitchValve();
}

