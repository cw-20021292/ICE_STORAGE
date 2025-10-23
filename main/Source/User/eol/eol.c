/**
 * File : eol.c
 * 
 * Eol
*/
#include "eol.h"

#include "error.h"
#include "comp_bldc.h"
#include "process_display.h"
#include "sound.h"

#define  DEFAULT_LIMIT_TIME_VALUE      (1800U)    /* 3min = 3min x 60sec = 180, @100msec */
#define  DEFAULT_COMM_ERR_TIMER        (30U)      /* 3sec, @100msec */

typedef struct _end_of_line_
{
    U8 Status;                    /// START or STOP
    EEolType_T Type;              /// TYPE, EOL_TYPE_FRONT, EOL_TYPE_LOAD
    EEolMode_T Mode;              /// READY, LOAD

    EEolTestMode_T  TestMode;     /// 검사모드1, 검사모드2

    /* CHECK INPUT */
    U32 InputTest;                /// Input Test Value
    U8  InputCnt[EOL_ID_KEY_MAX];

    /* LOAD TEST */
    U8 mu8LoadComm;               /// START or STOP - 부하 검사 통신 시작 */

    U8 DrainWater;                /// START or STOP - 냉수 탱크 비움 */  
    U8 Circulate;                 /// START or STOP - 냉수 탱크 순환 */  

    /* Error */
    U8 ErrComm;

    /* Timer */
    U16 TestTimer;
    U16 LimitTimer;
    U16 CommErrTimer;

    /* CHECK STATUS */
    U32 CheckList[EOL_CHECK_ID_MAX];
} SEol_T;

SEol_T Eol;

/**
 * @ brief            Function For Init End Of Line
 * @ detail           none
 * @ param            none
 * @ return           none
**/
void InitEol(void)
{
    MEMSET( (void __FAR *)&Eol.InputCnt[0], 0x00, EOL_ID_KEY_MAX );
    MEMSET( (void __FAR *)&Eol.CheckList[0], 0x00, EOL_CHECK_ID_MAX );

    Eol.Status       = FALSE; 
    Eol.LimitTimer   = DEFAULT_LIMIT_TIME_VALUE;
    Eol.TestTimer    = DEFAULT_TEST_TIME_VALUE;
    Eol.CommErrTimer = DEFAULT_COMM_ERR_TIMER;

    Eol.Mode         = EOL_MODE_READY;

    Eol.DrainWater   = FALSE;
    Eol.Circulate    = FALSE;

    // 초기 값 설정을 0x01(에러)로 설정한다.
#if 0
    SetEolCheckStatus( EOL_CHK_ID_ICE_FULL, 1 );
    SetEolCheckStatus( EOL_CHK_ID_CIRCULATE_PUMP, 0 );
    SetEolCheckStatus( EOL_CHK_ID_DRAIN_PUMP, 0 );      
    SetEolCheckStatus( EOL_CHK_ID_CSI_MOUDLE, 0 );

    SetEolCheckStatus( EOL_CHK_ID_UV_ICE_DOOR, 0 );
    SetEolCheckStatus( EOL_CHK_ID_UV_WATER_OUT, 0 );
    SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TANK, 0 );
    SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TRAY, 0 );
    
    SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING, 0 );
    SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING, 0 );
    SetEolCheckStatus( EOL_CHK_ID_SYSTEM_COLD, 0 );
    SetEolCheckStatus( EOL_CHK_ID_SYSTEM_ICE,  0 );
    SetEolCheckStatus( EOL_CHK_ID_EEPROM,  0xA5 );
#endif
}

/**
 * @ brief              Function For Start End Of Line
 * @ detail             none
 * @ param     xEtype   Eol Type ( Main Load, Front Load, SubKey )
 * @ return             none
**/
void StartEol(EEolType_T type)
{
    if( Eol.LimitTimer == 0 )
    {
        return ;
    }

    Eol.Status = TRUE;
    Eol.Type = type;
}

/**
 * @ brief              Function For Stop End Of Line
 * @ detail             none
 * @ param              none
 * @ return             none
**/
void StopEol(void)
{
    Eol.Status = FALSE;
}

U8 GetEolStatus(void)
{
    return Eol.Status;
}

/**
 * @ brief              Function For Get Eol Type
 * @ detail             none
 * @ param              none
 * @ return             Eol Type ( Main Load, Front Load, SubKey )
**/
EEolType_T GetEolType(void)
{
    return Eol.Type;
}

/**
 * @ brief              Function For Get If Limit Time Of Eol is Expired
 * @ detail             none
 * @ param              none
 * @ return             Result ( TRUE : Expired, FALSE : No Expired )
**/
U8 IsExpiredEolLimitTimer(void)
{
    if( Eol.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

/**
 * @ brief              Function For Start Eol Main Load
 * @ detail             none
 * @ param              none
 * @ return             Result ( TRUE : 진입조건 만족, FALSE : 진입조건 안됨 )
**/
U8 StartEolLoad(void)
{
    // EOL 테스트 진입 시간이 만료된,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return FALSE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return FALSE;
    }

    // EOL 시작
    StartEol( EOL_TYPE_LOAD );

    // 버전 표시
    StartDisplayInit();
    SetVersionDisp(0);

    // COMP COMM ERROR CLEAR
    SetCompBldcErrorComm(FALSE);

    // 에러 Clear 및 팝업 제거
    SetErrorId(ERR_ID_NONE);

    return TRUE;
}

void SetEolMode(EEolMode_T mode)
{
    Eol.Mode = mode;
}

EEolMode_T GetEolMode(void)
{
    return Eol.Mode;
}

/**
 * @ brief              Function For Set Eol Test Mode
 * @ detail             none
 * @ param     xEmode   Eol Mode ( MODE0, MODE1, MODE2 )
 * @ return             none
**/
void SetEolTestMode(EEolTestMode_T mode)
{
    Eol.TestMode = mode;
}

/**
 * @ brief              Function For Get Eol Test Mode
 * @ detail             none
 * @ param              none
 * @ return             Eol Mode ( MODE0, MODE1, MODE2 )
**/
EEolTestMode_T GetEolTestMode( void )
{
    return Eol.TestMode;
}

void SetEolTestTimer(U16 time)
{
    Eol.TestTimer = time;
}

U16 GetEolTestTimer(void)
{
    return Eol.TestTimer;
}

// Drain Water
void StartEolDrainWater(void)
{
    Eol.DrainWater = TRUE;
}

void StopEolDrainWater(void)
{
    Eol.DrainWater = FALSE;
}

U8 GetEolDrainWater(void)
{
    return Eol.DrainWater;
}

// Cold Water Circulate
void StartEolCirculate(void)
{
    Eol.Circulate = TRUE;
}

void StopEolCirculate(void)
{
    Eol.Circulate = FALSE;
}

U8 GetEolCirculate(void)
{
    return Eol.Circulate;
}

// Test Input
void SetEolTestInputBit(U32 maskBit)
{
    Eol.InputTest |= maskBit;
}

void ClearEolTestInputBit(U32 maskBit)
{
    Eol.InputTest &= ~maskBit;
}

void SetEolTestInputVal(U32 maskBit)
{
    Eol.InputTest = maskBit;
}

U32 GetEolTestInputVal(void)
{
    return Eol.InputTest;
}

void SetEolTestInputCnt(U8 id, U8 maxCnt)
{
    if( Eol.InputCnt[id] < maxCnt )
    {
        Eol.InputCnt[id]++;
    }
}

U8 GetEolTestInputCnt(U8 id)
{
    return Eol.InputCnt[id];
}

void ClearEolTestInputCnt(void)
{
    MEMSET( (void __FAR *)&Eol.InputCnt[0], 0x00, EOL_ID_KEY_MAX);
}

// Error
void SetEolErrorComm(U8 error)
{
    Eol.ErrComm = error;
}

U8 GetEolErrorComm(void)
{
    return Eol.ErrComm;
}

U32 GetEolCheckStatus(EEolCheckId_T id)
{
    if( id >= EOL_CHECK_ID_MAX )
    {
        return 0xFFFFFFFF; // index error
    }

    return Eol.CheckList[id];
}

void SetEolCheckStatus(EEolCheckId_T id, U32 val)
{
    if( id >= EOL_CHECK_ID_MAX )
    {
        return;
    }

    Eol.CheckList[id] = val;
}

void UpdateEolTimer(void)
{
    if( Eol.LimitTimer != 0 )
    {
        Eol.LimitTimer--;
    }

    if( Eol.TestTimer != 0 )
    {
        Eol.TestTimer--;
    }
}

U8 EOL_Drain(void)
{
    if( GetEolDrainWater() == FALSE )
    {
        //SetEolTestInputBit( MK_EOL_KEY_STER );
        StartEolDrainWater();
        return SOUND_SETUP;
    }

    //ClearEolTestInputBit( MK_EOL_KEY_STER );
    //StopEolDrainWater();
    return SOUND_ERROR;
}
