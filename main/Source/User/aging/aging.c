/**
 * File : aging.c
 *
 * Aging ( Quility Aging Test )
**/
#include "aging.h"

#include "water_out.h"
//#include "effluent.h"
#include "sound.h"

//#include "input_water.h"
#include "temp.h"
//#include "display_popup.h"
#include "display_lib.h"
#include "front.h"
//#include "water_out_flushing.h"

#include "process_display.h"
#include "amount.h"
#include "room_water.h"
#include "cold_water.h"
#include "ice_door.h"

#define DEFAULT_LIMIT_TIME_VALUE      (3000U)    /* 5min = 5min x 60sec = 300sec, @100msec */

typedef struct _aging_
{
    U8 mUstart;
    U8 mUstep;
    U16 mUdelay;
    
    U32 mUlimitTimer;

    U32 mUcycleCnt;

    U32 mUiceDoorOpenTime; // 얼음 추출 되는 시간 타이머
} SAging_T;

SAging_T gStAging;

void InitAging(void)
{
    MEMSET( (void __FAR *)&gStAging, 0x00, sizeof(SAging_T) );

    gStAging.mUlimitTimer = DEFAULT_LIMIT_TIME_VALUE;
}

void StartAging(void)
{
    gStAging.mUstart = TRUE;
}

void StopAging(void)
{
    gStAging.mUstart = FALSE;
}

U8 GetAging(void)
{
    return gStAging.mUstart;
}

U8 IsExpiredAgingLimitTimer(void)
{
    if( gStAging.mUlimitTimer != 0 )
    {
        return FALSE;         // No Expired!
    }

    return TRUE;              // Expired!
}

static void UpdateAgingTimer(void)
{
    if( gStAging.mUlimitTimer != 0 )
    {
        gStAging.mUlimitTimer--;
    }
}

U32 GetCycleCount(void)
{
    return (U32)(gStAging.mUcycleCnt / (10UL));
}

static void CountCycle(void)
{
    if( gStAging.mUcycleCnt < 1999 )
    {
        gStAging.mUcycleCnt++;
    }
}

static U8 IsValidAging(void)
{
    if( GetAging() == FALSE )
    {
        return FALSE;
    }  

    return TRUE;
}

static U8 IsValidPauseChp(void)
{
    /*
    if( GetWaterOutFlushRoomDrain() == TRUE )
    {
        return TRUE;
    }

    if( GetWaterOutFlushColdDrain() == TRUE )
    {
        return TRUE;
    }
    */

    if( GetRoomWaterInitFull() == FALSE )
    {
        return TRUE;
    }

    if( GetColdWaterInitFull() == FALSE )
    {
        return TRUE;
    }

    if( GetTemp(TEMP_ID_HOT_WATER) <= 70.0f )
    {
        return TRUE;
    }

    if( GetTemp(TEMP_ID_COLD_WATER) >= 10.0f )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsValidPauseHp(void)
{
    /*
    if( GetWaterOutFlushRoomDrain() == TRUE )
    {
        return TRUE;
    }

    if( Get_HeatTankWaterLevel() == FALSE )
    {
        return TRUE;
    }
    */

    if( GetRoomWaterInitFull() == FALSE )
    {
        return TRUE;
    }
    
    if( GetTemp(TEMP_ID_HOT_WATER) <= 70.0f )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsValidPause(void)
{
    U8 uResult = FALSE;

    /*
    if( GetSystem() == SYSTEM_TYPE_HP )
    {
        uResult = IsValidPauseHp();
    }
    else
    {
        uResult = IsValidPauseChp();
    } */

    uResult = IsValidPauseChp();

    return uResult;
}

typedef enum _aging_chp_step_
{
    AGING_CHP_STEP_INIT = 0,
    AGING_CHP_STEP_COLD_OUT,
    AGING_CHP_STEP_COLD_OUT_WAIT, 
    AGING_CHP_STEP_PAUSE_1,
    AGING_CHP_STEP_ROOM_OUT,
    AGING_CHP_STEP_ROOM_OUT_WAIT, 
    AGING_CHP_STEP_PAUSE_2,
    AGING_CHP_STEP_HOT_OUT,
    AGING_CHP_STEP_HOT_OUT_WAIT, 
    AGING_CHP_STEP_PAUSE_3,
    AGING_CHP_STEP_ICE_OUT,
    AGING_CHP_STEP_ICE_OUT_WAIT, 
    AGING_CHP_STEP_PAUSE_4,
    AGING_CHP_STEP_MAX,
} EAgingChpStep_T;

#define PAUSE_TIME_COUNT (6000)          // 10min * 60sec = 600sec @100ms
#define ICE_DOOR_OPEN_TIME_20SEC (200)   //  20sec @100ms
static void AgingChp(void)
{
    switch(gStAging.mUstep)
    {
        case AGING_CHP_STEP_INIT:
           gStAging.mUstep++;
           break;

        case AGING_CHP_STEP_COLD_OUT:
            //ShowDispPopUp(POPUP_ID_PLEASE_WAIT, POPUP_TYPE_FIX, 0);
            SetWaterOutSelect(SEL_WATER_COLD);                /// 물종류 선택
            SetWaterOutContinue(TRUE);
            SetWaterOutFixedAmountType(FIXED_AMOUNT_500ML);   /// 용량 선택
            StartWaterOut();                                  /// 추출 시작
            Sound(SOUND_EFFLUENT);
            //StopDisplayTimerId( DISP_TIMER_ID_KEY_AMOUNT );
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_COLD_OUT_WAIT:
            if( GetWaterOut() == TRUE )
            {
                break;
            }

            SetWaterOutContinue(FALSE);
            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_PAUSE_1:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {
                gStAging.mUstep++;
            }
            break;

        case AGING_CHP_STEP_ROOM_OUT:
            //ShowDispPopUp(POPUP_ID_PLEASE_WAIT, POPUP_TYPE_FIX, 0);
            SetWaterOutSelect(SEL_WATER_ROOM);                /// 물종류 선택
            SetWaterOutContinue(TRUE);
            SetWaterOutFixedAmountType(FIXED_AMOUNT_500ML);   /// 용량 선택
            StartWaterOut();                                  /// 추출 시작
            Sound(SOUND_EFFLUENT);
            //StopDisplayTimerId( DISP_TIMER_ID_KEY_AMOUNT );
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_ROOM_OUT_WAIT:
            if( GetWaterOut() == TRUE )
            {
                break;
            }
            
            SetWaterOutContinue(FALSE);
            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_PAUSE_2:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {
                gStAging.mUstep++;
            }
            break;

        case AGING_CHP_STEP_HOT_OUT:
            //ShowDispPopUp(POPUP_ID_PLEASE_WAIT, POPUP_TYPE_FIX, 0);
            SetWaterOutSelect(SEL_WATER_HOT);                 /// 물종류 선택
            SetWaterOutContinue(TRUE);
            SetWaterOutFixedAmountType(FIXED_AMOUNT_500ML);   /// 용량 선택
            StartWaterOut();                                  /// 추출 시작
            Sound(SOUND_EFFLUENT);
            //StopDisplayTimerId( DISP_TIMER_ID_KEY_AMOUNT );
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_HOT_OUT_WAIT:
            if( GetWaterOut() == TRUE )
            {
                break;
            }
            
            SetWaterOutContinue(FALSE);
            //CountCycle();
            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_PAUSE_3:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {
               gStAging.mUstep++;
            }
            break;

        case AGING_CHP_STEP_ICE_OUT:
            OpenIceDoor();
            Sound(SOUND_EFFLUENT);
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_ICE_OUT_WAIT:
            if( gStAging.mUiceDoorOpenTime < ICE_DOOR_OPEN_TIME_20SEC )
            {
                gStAging.mUiceDoorOpenTime++;
                break;
            }

            CountCycle();
            CloseIceDoor();
            gStAging.mUiceDoorOpenTime = 0;
            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_CHP_STEP_PAUSE_4:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {
                gStAging.mUstep = AGING_CHP_STEP_INIT;
            }
            break;

        default :
            gStAging.mUstep = AGING_CHP_STEP_INIT;
            break;
    }
}

#if 0
typedef enum _aging_hp_step_
{
    AGING_HP_STEP_INIT = 0,
    AGING_HP_STEP_ROOM_OUT,
    AGING_HP_STEP_ROOM_OUT_WAIT, 
    AGING_HP_STEP_PAUSE_1,
    AGING_HP_STEP_HOT_OUT,
    AGING_HP_STEP_HOT_OUT_WAIT, 
    AGING_HP_STEP_PAUSE_2,
    AGING_HP_STEP_MAX,
} EAgingHpStep_T;

static void AgingHp(void)
{
    switch(gStAging.mUstep)
    {
        case AGING_HP_STEP_INIT:
           gStAging.mUstep++;
           break;

        case AGING_HP_STEP_ROOM_OUT:
            ShowDispPopUp(POPUP_ID_PLEASE_WAIT, POPUP_TYPE_FIX, 0);
            SetWaterOutSelect( SEL_WATER_ROOM );                 // 臾쇱?瑜????
            SetWaterOutFixedAmountType( AMOUNT_ID_4OZ );    // ?⑸웾 ???
            StartWaterOut();                                     // 異붿텧 ???
            Sound(SOUND_EFFLUENT);
            StopDisplayTimerId( DISP_TIMER_ID_KEY_AMOUNT );
            gStAging.mUstep++;
            break;

        case AGING_HP_STEP_ROOM_OUT_WAIT:
            if( GetWaterOut() == TRUE )
            {
                break;
            }

            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_HP_STEP_PAUSE_1:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {   
                gStAging.mUstep++;
            }
            break;

        case AGING_HP_STEP_HOT_OUT:
            ShowDispPopUp(POPUP_ID_PLEASE_WAIT, POPUP_TYPE_FIX, 0);
            SetWaterOutSelect( SEL_WATER_HOT );                 // 臾쇱?瑜????
            SetWaterOutFixedAmountType( AMOUNT_ID_4OZ );    // ?⑸웾 ???
            StartWaterOut();                                     // 異붿텧 ???
            Sound(SOUND_ID_EFFLUENT);
            StopDisplayTimerId( DISP_TIMER_ID_KEY_AMOUNT );
            gStAging.mUstep++;
            break;

        case AGING_HP_STEP_HOT_OUT_WAIT:
            if( GetWaterOut() == TRUE )
            {
                break;
            }

            CountCycle();
            gStAging.mUdelay = PAUSE_TIME_COUNT;
            gStAging.mUstep++;
            break;

        case AGING_HP_STEP_PAUSE_2:
            if( gStAging.mUdelay != 0 )
            {
               gStAging.mUdelay--;
            }
            else
            {
                gStAging.mUstep = AGING_HP_STEP_INIT;
            }
            break;

        default :
            gStAging.mUstep = AGING_HP_STEP_INIT;
            break;

    }
}
#endif

static void ControlAging(void)
{
    if( GetAging() == FALSE )
    {
        return;
    }

    if( IsValidPauseChp() == TRUE )
    {
       gStAging.mUdelay = PAUSE_TIME_COUNT;
       return;
    }

    AgingChp();
}

// @100ms
void ProcessAging(void)
{
    UpdateAgingTimer();

    ControlAging();
}
