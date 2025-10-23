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

#define PERIOD_DAY  5   // 5��
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

// ���  ����/����
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

    // ���� ī��Ʈ �ʱ�ȭ
    // ���� �߻� Ƚ�� 1ȸ�� ����
    ClearTankLevelErrorCount();
    SetRoomErrCountLimit( 1 );
}

static void StopSter(void)
{
    Ster.Start = FALSE;
    Ster.Mode  = 0;

    Ster.ConsumWater = MIN_CONSUMPTION_WATER;

    InitHealthDrain();

    // ���� ī��Ʈ �ʱ�ȭ
    // ���� �߻� Ƚ�� 1ȸ�� ����
    ClearTankLevelErrorCount();
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT );
}

// ��� ����
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

// ��� ��� ����
U8 StopSterError(void)
{
    // STER_MODE_INIT ~ STER_MODE_DRAIN_1ST
    if( IsSetSterMode( STER_MODE_DRAIN_1ST ) == TRUE )
    {
        StopSter();
    }
    else if( IsSetSterMode( STER_MODE_START ) == TRUE )
    {
        // ��� ���� ��, ���� ����
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
        // ��� ���� ��, ���� ����
        StopSter();
        SetSterErrWaterOut( TRUE );
    }

    return TRUE;
}


U8 IsStartSter(void)
{
    return Ster.Start;
}


/* �� ���� ��� */
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

// ��� �Ϸ� ���� Ȯ��
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


// ��� �� ���� �߻�
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



// ��ռ� �Լ� 
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

// ��� �ֱ� ���� ���
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


// ��� �ֱ� - ������
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

// ��� ����� 
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

/* ��� ���� ���� Ȯ��
 * Return
 *  - TRUE : ����
 *  - FALSE : �Ұ���
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

                // ������ �ܰ谡 �Ϸ�Ǹ�, �Ϸ� ���� �Ѵ�.
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
                // ��� �� ���� ���� �߻��Ǹ�  ����
                StopSterError();
                ClearSterMode( STER_MODE_ALL );
            }
            break;
        }
    }
}

// ��� �߿� RESET�� �Ǹ�, ��� ���ۺ��� �ٽ� �Ѵ�. 
// ��� ������ ��� �� �Լ��� �Ǿ��� ������ �Ѵ�.
// ��� ���� ������ ��� �� ���� ���� �ܰ迡 ���� 2�ܰ�� �����Ѵ�.
// ���� Ż���� �����Ѵ�.
static void CheckRebootSter(void)
{
    static U8 mu8Init = FALSE;

    if( mu8Init == FALSE )
    {
        mu8Init = TRUE;

        // ��� �� RESET, ��� ���� 
        if( GetSterGoing() != STER_GOING_OFF )
        {
            // ���� Ż���� �����Ѵ�.
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

// �ڵ� ����� ��, �ֱ� ����
// ��� ������� 3�� °�� �Ǵ� ��, �� ���� ����Ѵ�.
void ProcessPeriodDay(void)
{
    static U8 mu8Alarm = TRUE;
    U8 mu8CheckPeriod = FALSE;
    U8 mu8PassedDay = PASSED_DAY;

    mu8PassedDay = GetSterPassDay();

    
    // 0�� 0�� 0�� ~ 0�� 10�� 0�� ���̿� �� �� ����
    //
    // Note. �ش� �ð��� �ý��� reset�� �߻��Ǹ�, 
    // ��� ������� �����Ǵ� ������ �߻��� �� ����.
    // ���� �ڵ������� �ʿ���...
    if ( IsAlaramTime(0, 0, 0) == STATUS_ALARAM
            && IsAlaramTime(0, 10, 0) == STATUS_NOT_YET )

    {
        if( mu8Alarm == FALSE )
        {
            mu8Alarm = TRUE;
            mu8CheckPeriod = TRUE;

            // ��� ����� ī��Ʈ / ����
            // ��� ���� ���¿����� ����
            if( IsSetHealthMode( HEALTH_MODE_STER ) == TRUE )
            {
                mu8PassedDay++; 
                SetSterPassDay( mu8PassedDay );
            }

            // ���� �� ��뷮 ����
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
    // �ڵ� ��忡���� �����Ѵ�.
    if( Ster.PeriodMode == STER_PERIOD_MODE_FIXED )
    {
        mu8CheckPeriod = FALSE;
        return ;
    }

    if( mu8CheckPeriod == TRUE )
    {
        mu8CheckPeriod = FALSE;

        // �� ��뷮�� ���� �ֱ� ���.
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

    // �� ��뷮 ����
    CountingConsumptionWater();

    // ��� ���� �ƴϸ� ����.
    if( IsStartSter() == FALSE )
    {
        return ;
    }

    // ��� �� ���� �߻�
    if( IsErrorTypeSter() == TRUE )
    {
        StopSterError();
        return ;
    }

    // ��� �Ϸ�
    if( GetSterMode() == STER_MODE_NONE )
    {
        StopSter();
        return ;
    }

    // ��� ��庰 �� ���� 
    SterModeEventHandler();
}

