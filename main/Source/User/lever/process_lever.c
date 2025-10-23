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
            /// ������ ���� �ȵǴ� ��� ǥ��?
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
            /// �ü� ���� �ȵǴ� ��� ǥ��? 
            return FALSE;
        }
    }

    return TRUE;
}

static U8 IsValidLeverCondition(U8 id)
{
#if 0
    /* ��� */
    if( IsStartSter() == TRUE )
    {
        StartDisplaySter();
        return FALSE;
    }
#endif

    /* ��ü ��� */
    if( GetLockAll() == LOCK )
    {
        StartDisplayLockAll();
        return FALSE;
    }

#if 0
    // ��� �ð� ���� 
    if( GetDispAdjustHourConfig() == TRUE )
    {
        return FALSE;
    }
#endif

    // �ڵ� ��� ����
    if( GetDispDrainWaterConfig() == TRUE )
    {
        return FALSE;
    }

    // �¼� �� ����
    if( GetAltidueConfig() == TRUE )
    {
        return FALSE;
    }

    // ��� ��ư�� ���� ���, ���� ����
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

    // ���� �Է� short 
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
    // ���� �Է� LONG
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_2S) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_2S);

        if( (pList + id )->pActionEvent_2 != NULL )
        {
            mu8Sound = (pList + id )->pActionEvent_2();
            Sound( mu8Sound );
        }
    }
    // ���� �Է� LONG-2
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_8S) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_8S);

        if( (pList + id)->pActionEvent_3 != NULL )
        {
            mu8Sound = (pList + id )->pActionEvent_3();
            Sound( mu8Sound );
        }
    }
    // ���� �Է� POP
    else if( IsSetLeverEventFlag(id, LEVER_EVENT_POP) == TRUE )
    {
        ClearLeverEventFlag(id, LEVER_EVENT_POP);

        // �Է½� ���� �߻��Ǹ�, POP �̺�Ʋ�� Skip�Ѵ�.
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
