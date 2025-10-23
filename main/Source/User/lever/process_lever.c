#include "hw.h"
#include "prj_type.h"
#include "process_lever.h"
#include "lever.h"
#include "lever_common_handler.h"
#include "lever_handler.h"
#include "sound.h"
#include "process_display.h"
#include "front.h"
#include "key.h"
#include "smart_saving.h"
#include "ster.h"
#include "drain_water.h"
#include "power_saving.h"
#include "hot_water.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "ice_door.h"

#include "eol.h"
#include "eol_lever_handler.h"

#include "fct.h"
#include "fct_lever_handler.h"


static U8 IsValidIceLever(void)
{
    if( GetIceWaterSelect() == SEL_ICE_WATER )
    {
        if( (GetWaterOut() == TRUE)
             && (GetWaterOutSelect() == SEL_WATER_COLD) )
        {
            /// 얼음물 추출 안되는 경고성 표시?
            return FALSE;
        }
    }  

    return TRUE;
}

static U8 IsValidWaterLever(void)
{
    if( GetWaterOutSelect() == SEL_WATER_COLD )
    {
        if( IsStartIceDoorWater() == TRUE )
        {
            /// 냉수 추출 안되는 경고성 표시? 
            return FALSE;
        }
    }

    return TRUE;
}

static U8 IsValidLeverCondition(U8 id)
{
#if 0
    /* 살균 */
    if( IsStartSter() == TRUE )
    {
        StartDisplaySter();
        return FALSE;
    }
#endif

    /* 전체 잠금 */
    if( GetLockAll() == LOCK )
    {
        StartDisplayLockAll();
        return FALSE;
    }

#if 0
    // 살균 시간 보정 
    if( GetDispAdjustHourConfig() == TRUE )
    {
        return FALSE;
    }
#endif

    // 자동 배수 해제
    if( GetDispDrainWaterConfig() == TRUE )
    {
        return FALSE;
    }

    // 온수 고도 설정
    if( GetAltidueConfig() == TRUE )
    {
        return FALSE;
    }

    // 잠금 버튼이 눌린 경우, 추출 중지
    if( (GetKeyVal() & K_HOT_LOCK) == K_HOT_LOCK )
    {
        return FALSE;
    }

    if( id == LEVER_ID_WATER )
    {
        return IsValidWaterLever();
    }

    return IsValidIceLever();
}

static void EventHandler(U8 id, SLeverEventList_T *pList)
{
    U8 mu8Sound;
    static U8 mu8ErrSkip[LEVER_ID_MAX]  = {FALSE};

    // 레버 입력 short 
    if( IsSetLeverEventFlag(id, LEVER_EVENT_PUSHED_SHORT) == TRUE )
    {
        ClearLeverEventFlag(id,  LEVER_EVENT_PUSHED_SHORT);

        if( IsValidLeverCondition(id) == FALSE )
        {
            mu8ErrSkip[id] = TRUE;
            Sound( SOUND_ERROR );
        }
        else if((pList + id)->pActionEvent_1 != NULL )
        {
            mu8Sound = (pList + id)->pActionEvent_1();
            Sound( mu8Sound );
        }

       // ResetUnusedSavingTime();
        ResetSmartUnusedSavingTime();    // UnusedSaving
        WakeUpSavingMode();
        ResetDrainWater();
        ResetUnusedTime();
        //SetSterDone( FALSE );
    }
    // 레버 입력 LONG
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_2S) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_2S);

        if( (pList + id )->pActionEvent_2 != NULL )
        {
            mu8Sound = (pList + id )->pActionEvent_2();
            Sound( mu8Sound );
        }
    }
    // 레버 입력 LONG-2
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_8S) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_8S);

        if( (pList + id)->pActionEvent_3 != NULL )
        {
            mu8Sound = (pList + id )->pActionEvent_3();
            Sound( mu8Sound );
        }
    }
    // 레버 입력 POP
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_POP) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_POP);

        // 입력시 에러 발생되면, POP 이벤틀르 Skip한다.
        if( mu8ErrSkip[id] == TRUE )
        {
            mu8ErrSkip[id] = FALSE;
            return ;
        }

        if( (pList + id)->pActionEvent_4 != NULL )
        {
            mu8Sound = (pList + id)->pActionEvent_4();
            Sound( mu8Sound );
        }
    }
}

void ProcessLeverEventHandler(void)
{
    SLeverEventList_T  *pEventList = NULL;
    U8 mu8KeyEventSize = 0;


    if( GetDisplayInit() == FALSE )
    {
        return ;
    }

    if( GetEolStatus() == TRUE )
    {
        // EOL
        pEventList = (SLeverEventList_T *)GetEolLeverEventList();
    }
    else if( GetFctStatus() == TRUE )
    {
        // FCT
        pEventList = (SLeverEventList_T *)GetFctLeverEventList();
    }
    else
    {
        // NORMAL
        pEventList = (SLeverEventList_T *)GetLeverEventList();
    }

    EventHandler(LEVER_ID_WATER, pEventList);
    EventHandler(LEVER_ID_ICE, pEventList);

    // Lever Ice & Key Amout
    EventHandler(LEVER_ID_ICE_AMOUNT, pEventList);
}
