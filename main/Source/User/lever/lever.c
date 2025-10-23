/**
 * File : lever.c
 * 
 * Lever
*/
#include "hw.h"
#include "api_key.h"
#include "lever.h"

//#include <string.h>

/* Status */
typedef enum _lever_status_
{
    LEVER_STATUS_NONE = 0,      // 0 :
    LEVER_STATUS_PUSHED,        // 1 :
    LEVER_STATUS_POP,           // 2 :
    LEVER_STATUS_TIMEOUT,       // 3 :
} ELeverStatus_T;

/* Event Timeout condition */
#define EVENT_TIME_OUT_LONG_8S    (800U)     // 8sec
#define EVENT_TIME_OUT_LONG_2S    (200U)     // 2sec
#define EVENT_TIME_OUT_SHORT      (5U)       // 50ms
#define EVENT_TIME_OUT_MAX        EVENT_TIME_OUT_LONG_8S

typedef struct _lever_
{
    U16  val;           // PUSH LEVER VAL
    U16  valPop;        // PUSH LEVER VAL
    U8   status;        // PUSH or POP
    U16  pushedTime;    // PUSH TIME...
    U8   eventFlag;     // Event Flag..
} SLever_T;

SLever_T Lever[LEVER_ID_MAX];

void InitLever(void)
{
    MEMSET( (void __FAR *)&Lever, 0, sizeof(Lever) );
}

U16 GetLeverVal(U8 id)
{
    return Lever[id].val;
}

U16 GetLeverPopVal(U8 id)
{
    return Lever[id].valPop;
}

void SetLeverEventFlag(U8 id, U8 event)
{
    Lever[id].eventFlag |= event;
}

void ClearLeverEventFlag(U8 id, U8 event )
{
    Lever[id].eventFlag &= ~event;
}

U8 IsSetLeverEventFlag(U8 id, U8 event )
{
    if( (Lever[id].eventFlag & event) ==  event )
    {
        return TRUE;
    }

    return FALSE;
}

static void ScanLever(U8 id, U16 val);
void ProcessScanLever(void)
{
    ScanLever(LEVER_ID_WATER, GetLeverWater());
    ScanLever(LEVER_ID_ICE, GetLeverIce());

    ScanLever(LEVER_ID_ICE_AMOUNT, GetLeverIceAmount());   
}

/*
 * timeout 10ms...
 *
 */
static void ScanLever(U8 id, U16 val)
{
    static U16 prevLeverVal[LEVER_ID_MAX];

    /// 1. Get Lever value 
    if( prevLeverVal[id] != val )
    {
        prevLeverVal[ id ] = val;
        Lever[ id ].val = val;  
        if( val == 0U )
        {
            Lever[ id ].valPop = val;  
        }

        if( val != 0 )
        {
            //Init Event
            Lever[ id ].status = LEVER_STATUS_PUSHED;
            Lever[ id ].pushedTime = 0;  
            ClearLeverEventFlag(id, LEVER_EVENT_PUSEHD_ALL);
        }
        else 
        {
            if( Lever[id].pushedTime >= EVENT_TIME_OUT_SHORT )
            {
                Lever[id].status = LEVER_STATUS_POP;
                SetLeverEventFlag(id, LEVER_EVENT_POP);
            }
            else
            {
                Lever[id].status = LEVER_STATUS_NONE;
            }
        }
    }
    
    if( Lever[id].status == LEVER_STATUS_PUSHED )
    {
        Lever[id].pushedTime++;

        if( Lever[id].pushedTime == EVENT_TIME_OUT_LONG_8S )
        {
            SetLeverEventFlag( id, LEVER_EVENT_PUSHED_LONG_8S);
        }
        else if( Lever[id].pushedTime == EVENT_TIME_OUT_LONG_2S )
        {
            SetLeverEventFlag(id, LEVER_EVENT_PUSHED_LONG_2S);
        }
        else if( Lever[id].pushedTime == EVENT_TIME_OUT_SHORT )
        {
            SetLeverEventFlag(id, LEVER_EVENT_PUSHED_SHORT);
        }

        if( Lever[ id ].pushedTime > EVENT_TIME_OUT_MAX )
        {
            Lever[ id ].status = LEVER_STATUS_TIMEOUT;
        }
    }
}
