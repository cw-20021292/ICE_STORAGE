/**
 * File : ice_feeder.c
 * 
 * Ice Feeder
*/
#include "ice_feeder.h"

#include "relay.h"

typedef enum _ice_feeder_step_
{
    ICE_FEEDER_STEP_INIT = 0,
    ICE_FEEDER_STEP_CW,
    ICE_FEEDER_STEP_WAIT,
} EIceFeederStep_T;

typedef struct _ice_feeder_
{
    U8 Start;

    EIceFeederStep_T Step;
    U16 Time;
} SIceFeeder_T;

SIceFeeder_T IceFeeder;

#define ICE_FEEDER_CW_TIME    (20U)   /// 2sec @100ms
#define ICE_FEEDER_WAIT_TIME  (5U)    /// 0.5sec @100ms

void InitIceFeeder(void)
{
    IceFeeder.Start = FALSE;

    IceFeeder.Step = ICE_FEEDER_STEP_INIT;
    IceFeeder.Time = 0U;
}

void StartIceFeeder(void)
{
    IceFeeder.Start = TRUE;
}

void StopIceFeeder(void)
{
    IceFeeder.Start = FALSE;
}

static void ControlIceFeeder(void)
{
    if( IceFeeder.Start == FALSE )
    {
        InitIceFeeder();
        return;
    }

    switch( IceFeeder.Step )
    {
        case ICE_FEEDER_STEP_INIT:
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            IceFeeder.Time = ICE_FEEDER_CW_TIME;
            IceFeeder.Step++;
            break;

        case ICE_FEEDER_STEP_CW:
            if( IceFeeder.Time != 0U )
            {
                IceFeeder.Time--;

                if( IceFeeder.Time == 0U )
                {
                    TurnOffRelay( RELAY_FEEDER_CW );
                    TurnOffRelay( RELAY_FEEDER_CCW );
                    IceFeeder.Time = ICE_FEEDER_WAIT_TIME;
                    IceFeeder.Step++;
                }
            }
            break;

        case ICE_FEEDER_STEP_WAIT:
            if( IceFeeder.Time != 0U )
            {
                IceFeeder.Time--;

                if( IceFeeder.Time == 0U )
                {
                    IceFeeder.Step = ICE_FEEDER_STEP_INIT;
                }
            }
            break;

        default:
            InitIceFeeder();
            break;
    }    
}

void ProcessIceFeeder(void)
{
    ControlIceFeeder();
}
