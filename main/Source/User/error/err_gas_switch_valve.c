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
    U8 Start;   // �˻� ����,����

    U8 Error;   // ���� ���� ( TRUE : ����, FALSE : ���� )
    U8 CheckStatus; // �˻�� ���� ( ���� �Ǵ� ���� )
    U16 OnePointTime;   // ���� �µ� Ȯ�� ��ġ �̵� �ð�
    U16 TwoPointTime;   // �� �µ� Ȯ�� ��ġ �̵� �ð�
    U16 DetectTime;  // ���� �ð�
    U16 RetryWaitTime;  // �˻� �ð�

    U8 Count[2];       // ��˻� Ƚ��
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

// gas switch ���� ���� ���� Ȯ�� �Լ�
// eva ������ ������ ��� ���� �ȵ�.
static U8 IsValidCheckErr(void)
{
    // Comp Off
    if( GetCompOnOff() == OFF )
    {
        return FALSE;
    }

    // �ü� ���Ϸ��� ����
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



// EVA �µ� ���� ���� 2point 
static U8 CheckPointTemp(void)
{
    if( ErrGas.OnePointTime != 0 )
    {
        ErrGas.OnePointTime--;

        ErrGas.tEvaCold[0]  = GetTemp( TEMP_ID_EVA_2 );
        ErrGas.tEvaIce[0]   = GetTemp( TEMP_ID_EVA_1 );
    }

    // �� EVA �µ� ����
    if( ErrGas.TwoPointTime != 0 )
    {
        ErrGas.TwoPointTime--;

        return FALSE;   // �� ���� �µ� ���� �̿Ϸ�
    }


    ErrGas.tEvaCold[1] = GetTemp(TEMP_ID_EVA_2 );
    ErrGas.tEvaIce[1]  = GetTemp(TEMP_ID_EVA_1 );
    return TRUE;    // One Point, Two Point ��� ���� �Ϸ�
}


static TEMP_T   CalcDeltaTemp(TEMP_T tEvaFrom, TEMP_T tEvaTo)
{
    TEMP_T tDelta;

    tDelta =  tEvaFrom - tEvaTo;

    return tDelta;
}

// �ø� ��ȯ ��� ��ġ�� ���� Delta temp ���
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

// ���� ���� Ȯ��
static U8 CheckSwitchStatus(void)
{
    // �� Eva �µ��� ���̰� TEMP_GAS_VALVE ���� ũ�� ���������̴�.
    //if( IsValidEvaDeltaTemp(ErrGas.tDelta, TEMP_GAS_VALVE) == TRUE )
    if(  ErrGas.tDelta > TEMP_GAS_VALVE )
    {
        // ���� ��ȭ�� ������ DetectTime updated
        if( ErrGas.CheckStatus == FALSE )   
        {
            ErrGas.DetectTime = DETECT_TIME;
        }
        ErrGas.CheckStatus = TRUE;  // ����
    }
    else
    {
        // ���� ��ȭ�� ������ DetectTime updated
        if( ErrGas.CheckStatus == TRUE )
        {
            ErrGas.DetectTime = DETECT_TIME;
        }
        ErrGas.CheckStatus = FALSE;
    }

    // DetectTime ���� ���� �ؼ� ������ �����Ǵ°�?
    if( ErrGas.DetectTime != 0 )
    {
        ErrGas.DetectTime--;
        return FALSE ;
    }

    return TRUE;
}


// �ø� ��ȯ ��� ���� �߻��� ��õ�
static U8 RetryErrorOnCount(void)
{
    U8 mu8ErrCount;


    // 3ȸ ���� �߻��Ǿ����� ���� �߻�
    mu8ErrCount = GetGasSwitchErrCount();
    if( mu8ErrCount == 0 )
    {
        return FALSE;   // ��õ� �ʰ�, ���� �߻�
    }

    // ��õ� ��� �ð�...
    if( ErrGas.RetryWaitTime != 0 )
    {
        ErrGas.RetryWaitTime--;
    }

    // ���� ī��Ʈ�� ���̰�, �ø� ��ȯ ��긦 ���� �ʱ�ȭ �Ѵ�.
    // TwoPointTime, DetectTime, RetryWaitTime�� �缳���Ѵ�.
    mu8ErrCount--;
    SetGasSwitchErrCount( mu8ErrCount );
    ErrGas.TwoPointTime = TWO_POINT_TIME;
    ErrGas.DetectTime = DETECT_TIME;
    ErrGas.RetryWaitTime = RETRY_WAIT_TIME;


    return TRUE;    // ��õ�
}


// �ø� ��ȯ ��� �ʱ�ȭ
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

// �ø� ��ȯ ��� ���� ����
void ProcessErrGasSwitchValve(void)
{
    // �����, �ø� ��ȯ ��� ��ġ�� ��ȭ�� ������ ��,
    // ���� �������� �ʱ�ȭ ��Ų��.
    if( IsChangedStatusComp() == TRUE 
            || IsChangedStatusGasValve() == TRUE )
    {
        InitVariable(); // ���� �ʱ�ȭ
    }

    // ���� ���� ���� ���� ���� Ȯ��
    if( IsValidCheckErr() == FALSE )
    {
        // ���� ���� ������ �ƴϸ� ����..
        ErrGas.Start    = FALSE;
        return ;
    }

    // �˻� ���������� �ƴϸ� ����.
    if( ErrGas.Start == FALSE )
    {
        return ;
    }

    // �µ� 2point ����( �µ� ��ȭ�� ����� ���� )
    if( CheckPointTemp() == FALSE )
    {
        return ;
    }

    // �µ� ��ȭ�� ���
    CalcDeltaTempOnSide();

    // ���� ���� Ȯ��
    if( CheckSwitchStatus() == FALSE )
    {
        return ;    
    }

    // ���� ������ ���,
    // ���� ����, ���� ī��Ʈ �ʱ�ȭ
    if( ErrGas.CheckStatus == TRUE )
    {
        SetGasSwitchErrCount( COUNT_NUM );
        StopCheckErr( FALSE ); 
        return ;
    }

    // ���� �߻� ( ��õ� 3ȸ �ʰ� )
    if( RetryErrorOnCount() == FALSE )
    {
        StopCheckErr( TRUE );
        return ;
    }

    // �ø� ��ȯ ��� ��ġ�� �ʱ�ȭ ��Ų��.
    ResetGasSwitchValve();
}

