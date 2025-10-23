#include "ice_caretaker.h"
#include "ice_care.h"

#include "relay.h"

typedef enum
{
    STEP_BREAK_ICE_FEED_R,
    STEP_BREAK_ICE_FEED_R_WAIT,  
    STEP_BREAK_ICE_FEED_F,
    STEP_BREAK_ICE_FEED_F_WAIT,

    STEP_BREAK_DONE
} BreakStep_T;

typedef struct _do_break_
{
    BreakStep_T Step;
    U16 TimeOut;
} DoBreak_T;

typedef enum
{
    STEP_POSITION_ICE_FEED_F,
    STEP_POSITION_ICE_FEED_F_WAIT,  

    STEP_POSITION_DONE
} PositionStep_T;

typedef struct _do_position_
{
    PositionStep_T Step;
    U16 TimeOut;
} DoPosition_T;


DoBreak_T    Break;
DoPosition_T Position;

#define ICE_FEED_OP_TIME_1S   (1)  // @1s
#define ICE_FEED_OP_TIME_2S   (2)  // @1s
#define ICE_FEED_OP_TIME_3S   (3)  // @1s
#define ICE_FEED_OP_TIME_4S   (4)  // @1s
#define ICE_FEED_OP_TIME_6S   (6)  // @1s
#define ICE_FEED_OP_TIME_8S   (8)  // @1s

static void InitDoBreak(void)
{
    Break.Step = 0;
    Break.TimeOut = 0;
}

static U8 DoBreak(void)
{
    U8 mu8Done = FALSE;

    switch(Break.Step)
    {
        case STEP_BREAK_ICE_FEED_R :
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOnRelay( RELAY_FEEDER_CCW );
            Break.TimeOut = ICE_FEED_OP_TIME_2S;
            Break.Step++;
            break;

        case STEP_BREAK_ICE_FEED_R_WAIT :
            if( Break.TimeOut != 0U )
            {
                Break.TimeOut--;

                if( Break.TimeOut == 0U )
                {
                    TurnOffRelay( RELAY_FEEDER_CW );
                    TurnOffRelay( RELAY_FEEDER_CCW );
                    Break.Step++;
                }
            }
            break;

        case STEP_BREAK_ICE_FEED_F :
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            Break.TimeOut = ICE_FEED_OP_TIME_2S;
            Break.Step++;
            break;

        case STEP_BREAK_ICE_FEED_F_WAIT :
            if( Break.TimeOut != 0U )
            {
                Break.TimeOut--;

                if( Break.TimeOut == 0U )
                {
                    TurnOffRelay( RELAY_FEEDER_CW );
                    TurnOffRelay( RELAY_FEEDER_CCW );
                    Break.Step++;
                }
            }
            break;

        case STEP_BREAK_DONE :
            mu8Done = TRUE;
            break;

        default :
            Break.Step = 0;
            break;

    }

    return mu8Done;
}


static void InitDoPosition(void)
{
    Position.Step = 0;
    Position.TimeOut = 0;
}

static U8 DoPosition(void)
{
    U8 mu8Done = FALSE;

    switch(Position.Step)
    {
        case STEP_POSITION_ICE_FEED_F :
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            Position.TimeOut = ICE_FEED_OP_TIME_2S;
            Position.Step++;
            break;

        case STEP_POSITION_ICE_FEED_F_WAIT :
            if( Position.TimeOut != 0U )
            {
                Position.TimeOut--;

                if( Position.TimeOut == 0U )
                {
                    TurnOffRelay( RELAY_FEEDER_CW );
                    TurnOffRelay( RELAY_FEEDER_CCW );
                    Position.Step++;
                }
            }
            break;

        case STEP_POSITION_DONE :
            mu8Done = TRUE;
            break;

        default :
            Position.Step = 0;
            break;

    }

    return mu8Done;

}

typedef void(*InitAction_T)(void);
typedef U8(*DoAction_T)(void);
typedef struct _ice_care_event_handler_
{
    U16 Mode;

    InitAction_T  DoInit;
    DoAction_T  DoAction;
} IceCareEventHandler_T;

IceCareEventHandler_T IceCareEventHandler[] =
{
    {ICE_CARE_MODE_BREAK,     InitDoBreak,      DoBreak     },
    {ICE_CARE_MODE_POSITION,  InitDoPosition,   DoPosition  }
};

#define SZ_ICE_CARE_EVENT_LIST  (sizeof(IceCareEventHandler)/sizeof(IceCareEventHandler_T))

void ControlIceCareEventHandler(void)
{
    U8 i;
    U8 mu8Mode;
    U8 mu8Ret;
    IceCareEventHandler_T  *pList = NULL;
    U8 mu8Init = FALSE;

    for( i = 0; i < SZ_ICE_CARE_EVENT_LIST ; i++ )
    {
        pList = &IceCareEventHandler[ i ];

        if( mu8Init == TRUE  
                || IsSetIceCareMode( pList->Mode ) == FALSE )
        {
            if( pList->DoInit != NULL )
            {
                pList->DoInit();
            }
        }
        else
        {
            mu8Init = TRUE;
            mu8Ret = FALSE;
            if( pList->DoAction != NULL )
            {
                mu8Ret = pList->DoAction();
            }

            if( mu8Ret == TRUE )
            {
                ClearIceCareMode( pList->Mode );
                if( pList->DoInit != NULL )
                {
                    pList->DoInit();
                }
            }
        }
    }
}


