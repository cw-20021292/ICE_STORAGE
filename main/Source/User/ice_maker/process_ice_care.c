#include "process_ice_care.h"

#include "ice_caretaker.h"
#include "ice_care.h"

#include "ice_door.h"
#include "ice_full.h"

#define ICE_CARE_BREAK_DETECT_TIME      (10800U)    // 3h * 60min * 60sec  @1s
#define ICE_CARE_POSITION_DETECT_TIME   (21600U)    // 6h * 60min * 60sec  @1s

typedef U8(*CheckCondition_T)(void);
typedef U8(*ExtraCondition_T)(void);
typedef struct _ice_care_list_
{
    U16  Mode;
    U8   IsSet;

    U16  TimeCurrent;
    U16  TimeTarget;

    CheckCondition_T  CheckCondition;
    ExtraCondition_T  ExtraCondition;
} IceCareList_T;

static U8 CheckBreakCondition(void)
{
    if( IsSetIceCareMode(ICE_CARE_MODE_BREAK) == TRUE )
    {
        return FALSE;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 CheckPositionCondition(void)
{
    if( IsSetIceCareMode(ICE_CARE_MODE_POSITION) == TRUE )
    {
        return FALSE;
    }

    if( IsOpenIceDoor() == FALSE )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 CheckPositionExtraCondition(void)
{
    if( GetIceFullStatus() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

IceCareList_T IceCareList[] =
{
    {ICE_CARE_MODE_BREAK,    FALSE, ICE_CARE_BREAK_DETECT_TIME,    ICE_CARE_BREAK_DETECT_TIME,    CheckBreakCondition,    NULL },    // IceCareBreak
    {ICE_CARE_MODE_POSITION, FALSE, ICE_CARE_POSITION_DETECT_TIME, ICE_CARE_POSITION_DETECT_TIME, CheckPositionCondition, CheckPositionExtraCondition }    // IceCarePosition
};

#define SZ_ICE_CARE_LIST  (sizeof(IceCareList)/sizeof(IceCareList_T))

void CheckIceCareMode(void)
{
    U8 i=0;
    IceCareList_T *pList = NULL;

    for(i=0; i<SZ_ICE_CARE_LIST; i++)
    {
        pList = &IceCareList[i];
    
        if( pList->CheckCondition() == TRUE )
        {
            if( pList->TimeCurrent != 0 )
            {  
                pList->TimeCurrent--;
            }
            else
            {
                pList->TimeCurrent = pList->TimeTarget;
                pList->IsSet = TRUE;

                if( pList->ExtraCondition != NULL )
                {
                    if(pList->ExtraCondition() == FALSE )
                    {
                        pList->IsSet = FALSE;
                    }
                }

                if( pList->IsSet == TRUE )
                {
                    SetIceCareMode(pList->Mode);
                }
            }
        }
        else
        {
            pList->TimeCurrent = pList->TimeTarget;
            pList->IsSet = FALSE;
        }
    }
}

void ControlIceCare(void)
{
    if( IsValidIceCare() == FALSE )
    {
        // ¸ðµç mode clear
        ClearIceCareMode(ICE_CARE_MODE_ALL);
        return;
    }

    if( GetConfigCareBreak() == FALSE )
    {
        ClearIceCareMode(ICE_CARE_MODE_BREAK);
    }

    if( GetConfigCarePosition() == FALSE )
    {
        ClearIceCareMode(ICE_CARE_MODE_POSITION);
    }

    ControlIceCareEventHandler();
}

void ProcessIceCare(void)
{
    CheckIceCareMode();

    ControlIceCare();
}


