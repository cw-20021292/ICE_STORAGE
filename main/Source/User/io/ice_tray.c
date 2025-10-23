#include "hw.h"
#include "ice_tray.h"
#include "valve.h"
#include "relay.h"
#include "ice.h"
#include "util.h"
#include "circulate_pump.h"

#include "comp.h"
#include "hotgas_switch_valve.h"

/* RETRY */
#define  STARTING_TIME                (500UL)    // 5sec @10ms
#define  MOVING_TIME                  (5000UL)   // 50sec @10ms
#define  DEICING_DETECT_WAIT_TIME     (100U)     // 1000ms @10ms
#define  ICING_DETECT_WAIT_TIME       (75U)      // 750ms @10ms

/* RETRY - ERROR COUNT */
#define  ERR_ICING_COUNT              (10U)      // ERROR COUNT, MOVE DECING -> ICING 
#define  ERR_DEICING_COUNT            (3U)       // ERROR COUNT, MOVE ICING -> DEICING
#define  ERR_DEICING_ACCUM_COUNT      (10U)      // ERROR ACCUMULATIVE COUNT, MOVE ICING -> DEICING

/* MODE */
#define ICE_TRAY_MODE_NONE            (0x00)
#define ICE_TRAY_MODE_INIT            (0x01)
#define ICE_TRAY_MODE_DEICING         (0x02)
#define ICE_TRAY_MODE_ICING           (0x04)

/* POSITION */
#define  DETECT      LOW
#define  UNDETECTED  HIGH

/* ICING POSITION CHECK TIMER */
#define DEFAULT_CHECK_TIMER           (1000U) // @10ms, 10sec

/* ICING / DE-ICING STEP */
enum
{
    STEP_START = 0,
    STEP_CHECK_STARTING,
    STEP_CHECK_MOVING,
    STEP_DONE,

    // Recovery Reversing 
    STEP_RR = 10,
    STEP_RR_CTRL,

    // Recovery Faild Deicing
    STEP_RFD_START = 20,
    STEP_RFD_CTRL,

    // Recovery Coil Frezzing
    STEP_RCF_START = 30,
    STEP_RCF,
    STEP_RCF_CTRL,
    STEP_RCF_REPEAT,

    STEP_ERROR = 40,
};

// Command Types (Recovery Coil Frezzing)
enum
{
    RCF_WAIT,
    RCF_TRAY,
    RCF_HEATER,
};


/* 탈빙 중... */

/* Timer Resolution :  @10ms */
static const U32 RR_WaitTimeList[] = 
{
    500, 500, 500,       /* 5sec, 5sec, 5sec */
};
#define  SZ_RR_WAIT_TIME_LIST   (sizeof(RR_WaitTimeList)/sizeof(U32))


// 해빙 동작
typedef struct _rcf_cmd_list_
{
    U8 Cmd;
    U32 Time;
} RCF_List_T;


static const RCF_List_T RCF_CommandList[] = 
{
    { RCF_WAIT,      60000UL }, // @10ms, 600sec 10min
    { RCF_TRAY,      1000UL },
    { RCF_HEATER,    2000UL },

    // 1
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 2
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 3
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 4
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 5
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 6
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 7
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 8
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },
    
    // 9
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

    // 10
    { RCF_TRAY,      500UL },
    { RCF_WAIT,      60000UL },

};
#define  SZ_RCF_CMD_LIST   (sizeof(RCF_CommandList)/sizeof(RCF_List_T))


IceTray_T  IceTray;

void InitIceTray(void)
{
    IceTray.Status   = ICE_TRAY_MOVING;
    IceTray.Mode     = ICE_TRAY_MODE_INIT;
    IceTray.ErrorMoving       = FALSE;
    IceTray.ErrorMovingDeicing = FALSE;
    IceTray.ErrorMicroSwitch  = FALSE;
    IceTray.ErrorIcingCount   = 0;
    IceTray.ErrorDeIcingCount = 0;
    IceTray.ErrorDeIcingAccumCount = 0;

    IceTray.IcingStep   = 0;
    IceTray.DeIcingStep = 0;
    IceTray.DeIcingDetectWaitTime = 0;
    IceTray.IcingDetectWaitTime = 0;

    /* TIME */
    IceTray.StartTime   = 0UL;
    IceTray.MovingTime  = 0UL;

    /* POSITION */
    IceTray.Posi_Icing     = UNDETECTED;
    IceTray.Posi_DeIcing   = UNDETECTED;
    IceTray.CheckTimer     = DEFAULT_CHECK_TIMER;

    /* Recovery Reserving */
    IceTray.RR_Count  = 0;
    IceTray.RR_WaitTime    = 0UL;

    /* Recovery Coil Freezing */
    IceTray.RCF_Cmd        = RCF_WAIT;
    IceTray.RCF_Time       = 0;
    IceTray.RCF_CmdCount   = 0;

    /* Recovery Faild Deicing */
    IceTray.RFD_WaitTime   = 0;


}

void GetIceTrayData( IceTray_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&IceTray, sizeof( IceTray_T ) );
}

U8 IsRetryIceTray(void)
{
    if( IceTray.IcingStep >= STEP_RR )
    {
        return 1;    // Going on retry..
    }

    if( IceTray.DeIcingStep >= STEP_RR )
    {
        return 2;    // Going on retry..
    }

    return 0;   // 정상 동작
}


U8 IsErrorIceTrayCoilFrezzing(void)
{
    if( IceTray.DeIcingStep >= STEP_RCF_START )
    {
        return TRUE;
    }

    return FALSE;
}

void ResetErrorIceTrayAccumCount(void )
{
    IceTray.ErrorDeIcingAccumCount = 0;
}

U8 GetIceTrayStatus(void)
{
    return IceTray.Status;
}

static void SetIceTrayStatus(U8 mu8Status)
{
    IceTray.Status = mu8Status;
}

static void SetMode(U8 mu8Mode )
{
    IceTray.Mode |= mu8Mode;
}

static void ClearMode(U8 mu8Mode )
{
    IceTray.Mode &= ~mu8Mode;
}


U8 GetIceTrayErrorMoving(void)
{
    return IceTray.ErrorMoving;
}

U8 GetIceTrayErrorMovingDeicing(void)
{
    return IceTray.ErrorMovingDeicing;
}

U8 GetIceTrayErrorMicroSwitch(void)
{
    return IceTray.ErrorMicroSwitch;
}

static void ClearIceTrayMode(void)
{
    IceTray.StartTime   = 0UL;
    IceTray.MovingTime  = 0UL;

    /* Recovery Reserving */
    IceTray.RR_Count  = 0;
    IceTray.RR_WaitTime    = 0UL;

    /* Recovery Coil Freezing */
    IceTray.RCF_Cmd        = RCF_WAIT;
    IceTray.RCF_Time       = 0;
    IceTray.RCF_CmdCount   = 0;

    /* Recovery Faild Deicing */
    IceTray.RFD_WaitTime   = 0;

    IceTray.DeIcingDetectWaitTime = 0;
    IceTray.IcingDetectWaitTime = 0;
}

void MoveIceTrayIcingEol(void)
{
    ClearMode( ICE_TRAY_MODE_INIT );
    MoveIceTrayIcing();
}

static U8 IsSetMode(U8 mu8Mode);
void MoveIceTrayIcing(void)
{
    SetMode( ICE_TRAY_MODE_ICING );
    IceTray.IcingStep = 0;

    if( IsSetMode( ICE_TRAY_MODE_DEICING ) == TRUE )
    {
        ClearIceTrayMode();
        ClearMode( ICE_TRAY_MODE_DEICING );
    }
}

void MoveIceTrayDeIcing(void)
{
    SetMode( ICE_TRAY_MODE_DEICING );
    IceTray.DeIcingStep = 0;

    if( IsSetMode( ICE_TRAY_MODE_ICING ) == TRUE )
    {
        ClearIceTrayMode();
        ClearMode( ICE_TRAY_MODE_ICING );
    }
}

/* 마이크로 스위치 감지 */
static void DetectPosition(void)
{
    U8 mu8Val_1;
    U8 mu8Val_2;


    /* DETECT POSITION ICING */
    do{
        mu8Val_1 = GET_STATUS_MICRO_ICE();
        Delay_US(3);
        mu8Val_2 = GET_STATUS_MICRO_ICE();
    }while( mu8Val_1 != mu8Val_2 );

    IceTray.Posi_Icing = mu8Val_1;


    /* DETECT POSITION DE-ICING */
    do{
        mu8Val_1 = GET_STATUS_MICRO_DEICE();
        Delay_US(3);
        mu8Val_2 = GET_STATUS_MICRO_DEICE();
    }while( mu8Val_1 != mu8Val_2 );

    IceTray.Posi_DeIcing = mu8Val_1;
}

static void MoveTrayIcing(void)
{
    TurnOffRelay( RELAY_ICE_TRAY_CW );
    TurnOnRelay( RELAY_ICE_TRAY_CCW );
}

static void MoveTrayDeIcing(void)
{
    TurnOnRelay( RELAY_ICE_TRAY_CW );
    TurnOffRelay( RELAY_ICE_TRAY_CCW );
}

static void StopTray(void)
{
    TurnOffRelay( RELAY_ICE_TRAY_CCW );
    TurnOffRelay( RELAY_ICE_TRAY_CW );
}

/* 아이스 트레이 마이크 스위치 동시 감지 검사
 * ERROR이면 return false
 * 정상이면 return true
 */
static U8 CheckErrorIceTrayPosition(void)
{
    static U16 mu16WaitTimer = 0;   // wait 1min..
    static U8 mu8Count     = 3;   


    if( mu16WaitTimer != 0 )
    {
        mu16WaitTimer--;

        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        return FALSE; // Error
    }

    if( IceTray.Posi_DeIcing == DETECT 
            && IceTray.Posi_Icing == DETECT )
    {
        mu8Count--;
        if( mu8Count == 0 )
        {
            IceTray.ErrorMicroSwitch = TRUE;
        }
        else
        {
            mu16WaitTimer = 6000U;
        }

        return FALSE;   // Error
    }

    mu8Count = 3;   // Reset error count

    return TRUE;
}

/* 타이머 변수 제어 */
static void UpdateTimer(void)
{
    if( IceTray.StartTime != 0UL )
    {
        IceTray.StartTime--;
    }

    if( IceTray.MovingTime != 0UL )
    {
        IceTray.MovingTime--;
    }

    if( IceTray.RR_WaitTime != 0UL )
    {
        IceTray.RR_WaitTime--;
    }

    if( IceTray.RCF_Time != 0UL )
    {
        IceTray.RCF_Time--;
    }

    // ICING
    if( IceTray.RFD_WaitTime != 0UL )
    {
        IceTray.RFD_WaitTime--;
    }

    // DEICING
    if( IceTray.DeIcingDetectWaitTime != 0UL )
    {
        IceTray.DeIcingDetectWaitTime--;
    }

    // ICING
    if( IceTray.IcingDetectWaitTime != 0UL )
    {
        IceTray.IcingDetectWaitTime--;
    }
}


/* 제빙 위치로 이동 */
static U8 MoveIcingRetry(void)
{
    switch( IceTray.IcingStep )
    {
        case STEP_START:
            // 제빙 위치가 아니면, 이동 시작
            if( IceTray.Posi_Icing == UNDETECTED )
            {
                MoveTrayIcing();
                SetIceTrayStatus( ICE_TRAY_MOVING );

                IceTray.MovingTime  = MOVING_TIME; // 50sec
            }

            IceTray.StartTime   = STARTING_TIME;  // 5sec
            IceTray.IcingStep++;
            break;

            // 동기 모터 기동 검사
        case STEP_CHECK_STARTING:
            if( IceTray.Posi_DeIcing == UNDETECTED )
            {
                // Starting Good!!
                IceTray.StartTime   = 0;
                IceTray.IcingStep++;
                break;
            }

            // If starting fail... Go retry step
            if( IceTray.StartTime == 0 )
            {
                IceTray.IcingStep = STEP_RR;
            }
            break;

        case STEP_CHECK_MOVING:
            if( IceTray.Posi_Icing == DETECT )
            {
                if( IceTray.IcingDetectWaitTime == 0 )
                {
                    StopTray();

                    IceTray.MovingTime  = 0;
                    IceTray.IcingStep++;
                    break;
                }
            }
            else
            {
                IceTray.IcingDetectWaitTime = ICING_DETECT_WAIT_TIME;
            }

            // If moving fail.. Go retry step
            if( IceTray.MovingTime == 0 )
            {
                IceTray.IcingStep = STEP_RFD_START;
            }
            break;

        case STEP_DONE:
            IceTray.IcingStep    = 0;
            IceTray.RR_Count     = 0;
            IceTray.RCF_Cmd      = RCF_WAIT;
            IceTray.RCF_Time     = 0;
            IceTray.RCF_CmdCount = 0;
            IceTray.ErrorIcingCount = 0;

            return TRUE;
            break;

            // Recovery Reversing
        case STEP_RR:
            StopTray();
            IceTray.RR_WaitTime  = RR_WaitTimeList [ IceTray.RR_Count ];  // 5sec
            IceTray.RR_Count++;

            IceTray.IcingStep++;
            break;

        case STEP_RR_CTRL:
            if( IceTray.RR_Count >= SZ_RR_WAIT_TIME_LIST )
            {
                IceTray.RR_Count = 0;
                IceTray.IcingStep = STEP_RFD_START;
            }
            else if( IceTray.RR_WaitTime == 0 )
            {
                IceTray.IcingStep = STEP_START;
            }
            break;

            // Recovery Faild Deicing
        case STEP_RFD_START:
            // 재시도 횟수가 3회 이상이면 에러 발생
            StopIceMakeLoad();
            StopTray();
            IceTray.ErrorIcingCount++;
            if( IceTray.ErrorIcingCount >= ERR_ICING_COUNT )
            {
                IceTray.IcingStep = STEP_ERROR;
                break;
            }

            //IceTray.RFD_WaitTime = 120000UL; // @10ms / 20min
            //IceTray.RFD_WaitTime = (60000UL); // @10ms / 10min
            IceTray.RFD_WaitTime = (540000UL); // @10ms / 1h 30min
            IceTray.IcingStep++;

            SetIceTrayMovingRetry();
            break;

        case STEP_RFD_CTRL:
            if( IceTray.RFD_WaitTime == 0
                || IsSetIceMakeMode(ICE_MODE_ICING_MOVE_DEICING) == FALSE )
            {
                ClearIcingMoveDeicing();
                // 강제 탈빙 시작
                SetDummyDeicing();

                // 종료
                IceTray.IcingStep    = 0;
                IceTray.RR_Count     = 0;
                IceTray.RCF_Cmd      = RCF_WAIT;
                IceTray.RCF_Time     = 0;
                IceTray.RCF_CmdCount = 0;

                return TRUE;
            }
            break;


        case STEP_ERROR:
            IceTray.ErrorMoving = TRUE;
            break;

    }

    return FALSE;
}


/* 탈빙 위치로 이동 */
static U8 MoveDeIcingRetry(void)
{
    switch( IceTray.DeIcingStep )
    {
        case STEP_START:
            if( IceTray.Posi_DeIcing == UNDETECTED )
            {
                MoveTrayDeIcing();
                SetIceTrayStatus( ICE_TRAY_MOVING );

                IceTray.MovingTime  = MOVING_TIME; // 50sec
            }

            IceTray.StartTime   = STARTING_TIME;  // 5sec
            IceTray.DeIcingStep++;
            break;

        case STEP_CHECK_STARTING:
            if( IceTray.Posi_Icing == UNDETECTED )
            {
                // Starting Good!!
                IceTray.StartTime   = 0;
                IceTray.DeIcingStep++;
                break;
            }

            // If starting fail... Go retry step
            if( IceTray.StartTime == 0 )
            {
                IceTray.DeIcingStep = STEP_RR;
            }
            break;

        case STEP_CHECK_MOVING:
            if( IceTray.Posi_DeIcing == DETECT )
            {
                if( IceTray.DeIcingDetectWaitTime == 0 )
                {
                    StopTray();

                    IceTray.MovingTime  = 0;
                    IceTray.DeIcingStep++;
                    break;
                }
            }
            else
            {
                IceTray.DeIcingDetectWaitTime = DEICING_DETECT_WAIT_TIME;
            }

            // 트레이 이동 시간 초과이면, Recovery Coil Frezzing 동작
            if( IceTray.MovingTime == 0 )
            {
                IceTray.DeIcingStep = STEP_RCF_START;
            }
            break;

            // 이동 완료
        case STEP_DONE:
            IceTray.DeIcingStep  = 0;
            IceTray.RR_Count     = 0;
            IceTray.RCF_Cmd      = RCF_WAIT;
            IceTray.RCF_Time     = 0;
            IceTray.RCF_CmdCount = 0;
            IceTray.ErrorDeIcingCount   = 0;

            return TRUE;
            break;


            // Recovery Reversing
        case STEP_RR:
            StopTray();
            IceTray.RR_WaitTime  = RR_WaitTimeList[ IceTray.RR_Count ];  // 5sec
            IceTray.RR_Count++;

            IceTray.DeIcingStep++;
            break;

        case STEP_RR_CTRL:
            if( IceTray.RR_Count >= SZ_RR_WAIT_TIME_LIST )
            {
                IceTray.RR_Count = 0;
                IceTray.DeIcingStep = STEP_RCF_START;
            }
            else if( IceTray.RR_WaitTime == 0 )
            {
                IceTray.DeIcingStep = STEP_START;
            }
            break;

        case STEP_RCF_START:
            // 재시도 횟수가 3회 이상이면 에러 발생
            IceTray.ErrorDeIcingCount++;

            // 재시도 횟수가 누적해서 3회 이상하면 에러 발생
            IceTray.ErrorDeIcingAccumCount++;

            if( IceTray.ErrorDeIcingCount > ERR_DEICING_COUNT
                    || IceTray.ErrorDeIcingAccumCount >= ERR_DEICING_ACCUM_COUNT )
            {
                IceTray.DeIcingStep = STEP_ERROR;
                break;
            }

            // 그렇지 않으면 해빙 동작 시작...
            IceTray.RCF_CmdCount = 0;
            IceTray.DeIcingStep++;
            break;

        case STEP_RCF:
            StopTray();

            IceTray.RCF_Cmd  = RCF_CommandList[ IceTray.RCF_CmdCount ].Cmd;  
            IceTray.RCF_Time = RCF_CommandList[ IceTray.RCF_CmdCount ].Time; 
            IceTray.RCF_CmdCount++;

            IceTray.DeIcingStep++;
            break;

        case STEP_RCF_CTRL:
            // 해빙 제어 시간이 완료되면 다음 스텝으로...
            if( IceTray.RCF_Time == 0 )
            {
                OpenValve(VALVE_NOS_FLOW);
                CloseValve(VALVE_ICE_TRAY_IN);
                TurnOffCirculatePumpTrayIn();
#if 0

  // IGLOO
                TurnOffRelay( RELAY_ICE_HEATER );
#endif

                IceTray.DeIcingStep++;
            } 
            else
            {
                // 해빙을 위한 제어 동작 ( 트레이 입수, 히터, 대기 )
                if( IceTray.RCF_Cmd == RCF_TRAY )
                {
                    CloseValve(VALVE_NOS_FLOW);
                    OpenValve(VALVE_ICE_TRAY_IN);
                    TurnOnCirculatePumpTrayIn();
#if 0

  // IGLOO
                    TurnOffRelay( RELAY_ICE_HEATER );
#endif
                }
                else if( IceTray.RCF_Cmd == RCF_HEATER )
                {
                    OpenValve(VALVE_NOS_FLOW);
                    CloseValve(VALVE_ICE_TRAY_IN);
                    TurnOffCirculatePumpTrayIn();
#if 0

  // IGLOO
                    TurnOnRelay( RELAY_ICE_HEATER );
#endif
                }
                else 
                {
                    OpenValve(VALVE_NOS_FLOW);
                    CloseValve(VALVE_ICE_TRAY_IN);
                    TurnOffCirculatePumpTrayIn();
#if 0

  // IGLOO
                    TurnOffRelay( RELAY_ICE_HEATER );
#endif
                }
            }
            break;

        case STEP_RCF_REPEAT:
            // 해빙 제어가 완료되면, 트레이 이동을 재시도...
            if( IceTray.RCF_CmdCount >= SZ_RCF_CMD_LIST )
            {
                IceTray.RCF_CmdCount = 0;
                IceTray.DeIcingStep = STEP_START;
            }
            else
            {
                IceTray.DeIcingStep = STEP_RCF;
            }
            break;

        case STEP_ERROR:

            SetIceMakeMode( ICE_MODE_OFF );
            IceTray.ErrorMovingDeicing = TRUE;
            break;

    }

    return FALSE;
}


// 트레이 제어가 모두 완료되고 나서
// 제빙 위치 미감지면, 위치 찾아가기
static U16 CheckPositionIcing(void)
{
    // 트레이 제어 완료되었는가?
    if( IceTray.Mode != ICE_TRAY_MODE_NONE )
    {
        return DEFAULT_CHECK_TIMER;
    }

    // 제빙 위치에 대해서는 동작한다.
    if( GetIceTrayStatus() != ICE_TRAY_ICING )
    {
        return DEFAULT_CHECK_TIMER;
    }

    // 제빙 위치에서 정상적으로 위치 감지가 되면 종료..
    if(IceTray.Posi_Icing == DETECT )
    {
        return DEFAULT_CHECK_TIMER;
    }

    // 그렇지 않으면 CheckTimer 이후에 다시 제빙 위치로 이동하자
    if( IceTray.CheckTimer != 0 )
    {
        IceTray.CheckTimer--;
        return IceTray.CheckTimer;
    }

    MoveIceTrayIcing();
    return DEFAULT_CHECK_TIMER;
}


static U8 IsSetMode(U8 mu8Mode)
{
    if( (IceTray.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}


void ControlIceTray(void)
{
    U8 mu8Ret = FALSE;


    /* Update Timer */
    UpdateTimer();

    /* Detect Tray Position */
    DetectPosition();


    /* ERROR */
    if( IceTray.ErrorMoving == TRUE 
            || IceTray.ErrorMovingDeicing == TRUE 
            || IceTray.ErrorMicroSwitch == TRUE)
    {
        ClearIceMakeMode( ICE_MODE_INIT );
    
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );

        return ;
    }


    /* Check Error - Tray Position */
    if( CheckErrorIceTrayPosition() == FALSE )
    {
        if( IceTray.ErrorMicroSwitch == TRUE )
        {
            SetIceMakeMode( ICE_MODE_OFF );
        }
        return ;
    }

    /* Init */
    if( IsSetMode( ICE_TRAY_MODE_INIT ) == TRUE )
    {
        mu8Ret = MoveDeIcingRetry();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_TRAY_MODE_INIT );
            SetIceTrayStatus( ICE_TRAY_DEICING );
        }
        return ;
    }

    /* Move Decing Position */
    if( IsSetMode( ICE_TRAY_MODE_DEICING ) == TRUE )
    {
        mu8Ret = MoveDeIcingRetry();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_TRAY_MODE_DEICING );
            SetIceTrayStatus( ICE_TRAY_DEICING );
        }
        return ;
    }

    /* Move Icing Position */
    if( IsSetMode( ICE_TRAY_MODE_ICING ) == TRUE )
    {
        mu8Ret = MoveIcingRetry();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_TRAY_MODE_ICING );
            SetIceTrayStatus( ICE_TRAY_ICING );
        }
        return ;
    }

    // 위치 이동시 재 원점으로 이동 
    // 제빙 위치에서 마이크로 스위치가 감지되지 않는 경우,
    // 다시 제빙 위치로 강제 이동
    IceTray.CheckTimer = CheckPositionIcing(); 
}


