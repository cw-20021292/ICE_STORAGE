#include "ster.h"

#include "room_water.h"
#include "level.h"
#include "err_room_level.h"
#include "water_out.h"
#include "drain.h"
#include "eeprom.h"
#include "rtc.h"
#include "error.h"
#include "health_maker.h"
#include "iocare.h"
#include "ice_maker.h"
#include "health.h"



#define MIN_CONSUMPTION_WATER  5010UL

#define PERIOD_DAY  5   // 5일
#define PASSED_DAY  1
Ster_T Ster;

static U8 IsValidStartSter(void);

/*Init*/
void InitSter(void)
{
    Ster.Start = FALSE;
    Ster.Mode  = STER_MODE_NONE;
    
    Ster.PeriodMode  = STER_PERIOD_MODE_AUTO;
    Ster.PeriodDay   = PERIOD_DAY;
    Ster.PassedDay   = PASSED_DAY;

    Ster.ConsumWater = MIN_CONSUMPTION_WATER;
    Ster.Done        = FALSE;
    Ster.ErrStopOut  = FALSE;
    Ster.Going       = STER_GOING_OFF;
}

void  GetSterData(Ster_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Ster, sizeof( Ster_T ));
}

// 살균  시작/중지
void StartSter(void)
{
    if( IsValidStartSter() == TRUE )
    {
        StartSterMode( STER_MODE_ALL );
    }
}

void StartSterMode(U16 mu16Mode)
{
    Ster.Start = TRUE;
    Ster.Mode  = mu16Mode;

    InitHealthMaker();

    // 에러 카운트 초기화
    // 에러 발생 횟수 1회로 변경
    ClearTankLevelErrorCount();
    SetRoomErrCountLimit( 1 );
}

static void StopSter(void)
{
    Ster.Start = FALSE;
    Ster.Mode  = 0;

    Ster.ConsumWater = MIN_CONSUMPTION_WATER;

    InitHealthDrain();

    // 에러 카운트 초기화
    // 에러 발생 횟수 1회로 변경
    ClearTankLevelErrorCount();
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT );
}

// 살균 종료
U8 StopSoftSter(void)
{
    // STER_MODE_INIT ~ STER_MODE_DRAIN_1ST
    if( IsSetSterMode( STER_MODE_DRAIN_1ST ) == TRUE )
    {
        StopSter();

        return TRUE;
    }
    else if( IsSetSterMode( STER_MODE_START ) == TRUE )
    {
        ClearSterMode( STER_MODE_START );
        InitSterilize();

        return TRUE;
    }
    else if( GetSterMode() == STER_MODE_FEED )
    {
        StopSter();

        return TRUE;
    }

    return FALSE;
}

// 살균 즉시 종료
U8 StopSterError(void)
{
    // STER_MODE_INIT ~ STER_MODE_DRAIN_1ST
    if( IsSetSterMode( STER_MODE_DRAIN_1ST ) == TRUE )
    {
        StopSter();
    }
    else if( IsSetSterMode( STER_MODE_START ) == TRUE )
    {
        // 살균 해제 후, 추출 중지
        StopSter();

        ClearSterMode( STER_MODE_START );
        InitSterilize();

        SetSterErrWaterOut( TRUE );

    }
    else if( GetSterMode() == STER_MODE_FEED )
    {
        StopSter();
    }
    else
    {
        // 살균 해제 후, 추출 중지
        StopSter();
        SetSterErrWaterOut( TRUE );
    }

    return TRUE;
}


U8 IsStartSter(void)
{
    return Ster.Start;
}


/* 상세 제어 모드 */
void SetSterMode(U16 mu16Mode)
{
    Ster.Mode |= mu16Mode;
}

U16 GetSterMode(void)
{
    return Ster.Mode;
}

void ClearSterMode(U16 mu16Mode)
{
    Ster.Mode &= ~mu16Mode;
}

U16 IsSetSterMode(U16 mu16Mode)
{
    if ( (Ster.Mode & mu16Mode ) == mu16Mode )
    {
        return TRUE;
    }
    
    return FALSE;
}

// 살균 완료 상태 확인
U8 SetSterDone(U8 mu8Done)
{
    U8 mu8Ret;

    mu8Ret = Ster.Done;
     Ster.Done = mu8Done;

    return mu8Ret;
}

U8 GetSterDone(void)
{
    return Ster.Done;
}


// 살균 중 에러 발생
void SetSterErrWaterOut(U8 mu8Done)
{
    if( mu8Done != TRUE && mu8Done != FALSE )
    {
        return ;
    }

    if( Ster.ErrStopOut != mu8Done )
    {
        SaveEepromId( EEP_ID_STER_ERR_WATER_OUT );
    }

    Ster.ErrStopOut = mu8Done;
}

U8 GetSterErrWaterOut(void)
{
    return Ster.ErrStopOut;
}



// 살균수 입수 
void SetSterGoing(SterGoingStatus_T mGoing)
{
    if( mGoing > STER_GOING_ON_LV_3 )
    {
        return ;
    }

    if( Ster.Going != mGoing )
    {
        SaveEepromId( EEP_ID_STER_DOING );
    }
    Ster.Going = mGoing;
}

SterGoingStatus_T GetSterGoing(void)
{
    return Ster.Going;
}


U8 ResetSter(void)
{
    if( IsStartSter() == TRUE )
    {
        SetSterGoing( STER_GOING_OFF );
        SetSterErrWaterOut(FALSE);
        StopSter();
        InitSterilize();
        return TRUE;
    }

    return FALSE;
}

// 살균 주기 제어 모드
void SetSterPeriodMode(U8 mu8Mode )
{
    if( Ster.PeriodMode != mu8Mode )
    {
        SaveEepromId(EEP_ID_STER_PERIOD_AUTO_CHANGE);
    }
    Ster.PeriodMode = mu8Mode;
}

U8 GetSterPeriodMode(void)
{
    return Ster.PeriodMode;
}


// 살균 주기 - 설정일
void SetSterPeriodDay(U8 mu8Day)
{
    if( mu8Day != 3 && mu8Day != 5 )
    {
        mu8Day = PERIOD_DAY;
    }

    if( Ster.PeriodDay != mu8Day )
    {
        SaveEepromId( EEP_ID_STER_PERIOD );
    }
    Ster.PeriodDay = mu8Day;
}

U8 GetSterPeriodDay(void)
{
    return Ster.PeriodDay;
}

// 살균 경과일 
void SetSterPassDay(U8 mu8Val)
{
    if(mu8Val < PASSED_DAY || mu8Val > 10 )
    {
        return ;
    }

    if( Ster.PassedDay != mu8Val )
    {
        SaveEepromId(EEP_ID_STER_DAY_PASS);
    }
    Ster.PassedDay = mu8Val;
}

U8 GetSterPassDay(void)
{
    return Ster.PassedDay;
}

/* 살균 동작 가능 확인
 * Return
 *  - TRUE : 가능
 *  - FALSE : 불가능
 */
static U8 IsValidStartSter(void)
{
    if( Ster.PassedDay <= Ster.PeriodDay )
    {
        return FALSE;
    }

    return TRUE;
}


static void CountingConsumptionWater(void)
{
    U8 mu8Sel;

    if( GetWaterOut() == TRUE )
    {
        mu8Sel = GetWaterOutSelect();
        if( Ster.ConsumWater != 0 )
        {
            Ster.ConsumWater--;
        }
    }
}

typedef HMStatus_T(*SterilizeDo_T)(void);
typedef void(*SterilizeInitial_T)(void);
typedef struct _sterilize_event_
{
    U8                      Mode;
    SterilizeDo_T           DoAction;           //Do Function
    SterilizeInitial_T      InitAction;      //Initial Function
} SterilizeEventList_T;

SterilizeEventList_T    SterModeEventList[] =
{
/*      Sterilize Mode,       Function,            Initial,      */
    { STER_MODE_INIT,         DoSterInit,          InitSterInit  },
    { STER_MODE_DRAIN_1ST,    DoHealthDrainInit,   InitHealthDrain },
    { STER_MODE_START,        DoSterilize,         InitSterilize },
    { STER_MODE_DRAIN_2ND,    DoHealthDrainSter,   InitHealthDrain },
    { STER_MODE_RINSE,        DoRinse,             InitRinse     },
    { STER_MODE_DRAIN_3TH,    DoHealthDrainRinse,  InitHealthDrain },
    { STER_MODE_FEED,         DoFeed,              InitFeed      }
};
#define SZ_STER_MODE_EVENT_LIST ( sizeof(SterModeEventList) / sizeof(SterilizeEventList_T) )

static void SterModeEventHandler(void)
{
    HMStatus_T mStatus;
    U8 i;
    SterilizeEventList_T *pList = NULL;
    

    for ( i = 0; i < SZ_STER_MODE_EVENT_LIST ; i++ )
    {
        pList = &SterModeEventList[ i ];

        // Check Mode
        if (IsSetSterMode(pList->Mode) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )     //Do function
            {
                pList->InitAction();           // Initial Function
                ClearSterMode( pList->Mode );  // Mode Clear

                // 마지막 단계가 완료되면, 완료 설정 한다.
                if( pList->Mode == STER_MODE_DRAIN_3TH )
                {
                    SetSterDone( TRUE );
                    SetSterPassDay( 1 );
#if 0

  // IGROO
                    SetIoCareMode( IOC_ID_STER_HISTORY );
#endif
                }
            }
            else if( mStatus == STATUS_ERR )
            {
                // 살균 중 에러 조건 발생되면  중지
                StopSterError();
                ClearSterMode( STER_MODE_ALL );
            }
            break;
        }
    }
}

// 살균 중에 RESET이 되면, 배수 동작부터 다시 한다. 
// 배수 동작은 살균 수 입수가 되었을 때에만 한다.
// 배수 동작 범위는 살균 수 투입 이후 단계에 따라 2단계로 구분한다.
// 더미 탈빙은 해제한다.
static void CheckRebootSter(void)
{
    static U8 mu8Init = FALSE;

    if( mu8Init == FALSE )
    {
        mu8Init = TRUE;

        // 살균 중 RESET, 배수 동작 
        if( GetSterGoing() != STER_GOING_OFF )
        {
            // 더미 탈빙을 해제한다.
            StopDummyDeIcing();

            if( GetSterGoing() == STER_GOING_ON_LV_1 )
            {
                StartSterMode( STER_MODE_LV_1 );
            }
            else if( GetSterGoing() == STER_GOING_ON_LV_2 )
            {
                StartSterMode( STER_MODE_LV_2 );
            }
            else
            {
                StartSterMode( STER_MODE_LV_3 );
            }
        }
    }
}

// 자동 모드일 때, 주기 재계산
// 살균 경과일이 3일 째가 되는 날, 한 번식 계산한다.
void ProcessPeriodDay(void)
{
    static U8 mu8Alarm = TRUE;
    U8 mu8CheckPeriod = FALSE;
    U8 mu8PassedDay = PASSED_DAY;

    mu8PassedDay = GetSterPassDay();

    
    // 0시 0분 0초 ~ 0시 10분 0초 사이에 한 번 동작
    //
    // Note. 해당 시간에 시스템 reset이 발생되면, 
    // 살균 경과일이 증가되는 오류가 발생될 수 있음.
    // 개선 코드적용이 필요함...
    if ( IsAlaramTime(0, 0, 0) == STATUS_ALARAM
            && IsAlaramTime(0, 10, 0) == STATUS_NOT_YET )

    {
        if( mu8Alarm == FALSE )
        {
            mu8Alarm = TRUE;
            mu8CheckPeriod = TRUE;

            // 살균 경과일 카운트 / 저장
            // 살균 설정 상태에서만 증가
            if( IsSetHealthMode( HEALTH_MODE_STER ) == TRUE )
            {
                mu8PassedDay++; 
                SetSterPassDay( mu8PassedDay );
            }

            // 일일 물 사용량 저장
#if 0

  // IGROO

            SetIoCareMode( IOC_ID_USE_WATER_DATA );
#endif
        }
    }
    else
    {
        mu8Alarm = FALSE;
    }

#if 0
    // 자동 모드에서만 동작한다.
    if( Ster.PeriodMode == STER_PERIOD_MODE_FIXED )
    {
        mu8CheckPeriod = FALSE;
        return ;
    }

    if( mu8CheckPeriod == TRUE )
    {
        mu8CheckPeriod = FALSE;

        // 물 사용량에 따른 주기 계산.
        if( mu8PassedDay == 3 )
        {
            if( Ster.ConsumWater == 0 )
            {
                SetSterPeriodDay( 5 ); 
            }
            else
            {
                SetSterPeriodDay( 3 ); 
            }
        }
    }
#endif
}

void ControlSter(void)
{
    CheckRebootSter();

    // 물 사용량 적산
    CountingConsumptionWater();

    // 살균 중이 아니면 종료.
    if( IsStartSter() == FALSE )
    {
        return ;
    }

    // 살균 중 에러 발생
    if( IsErrorTypeSter() == TRUE )
    {
        StopSterError();
        return ;
    }

    // 살균 완료
    if( GetSterMode() == STER_MODE_NONE )
    {
        StopSter();
        return ;
    }

    // 살균 모드별 상세 제어 
    SterModeEventHandler();
}

