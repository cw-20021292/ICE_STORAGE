/**
 * File : valve.c
 * 
 * Valve Control
*/
#include "valve.h"

#include "api_valve.h"

#define TEST_VALVE_ONOFF   (0)

typedef struct _valve_
{
    U16 Cmd;      // On/Off command ( 1: ON, 0: OFF )
    U16 Cur;      // current status ( 1: ON, 0: OFF )
    U16 BootTime;     // 제어 지연 시간..

    U8 TEST_Count;   // TEST counter variable
} SValve_T;

SValve_T  Valve;

void InitValve(void)
{
    // NOS VALVE is Normal Open
    Valve.Cmd = NORMAL_OPEN_VALVES;
    Valve.Cur = NORMAL_OPEN_VALVES;   
    Valve.BootTime = 500;   // Boot Wait @10ms

    Valve.TEST_Count = 0;
}

void OpenValve(U16	valve)
{
    Valve.Cmd |= valve;
}

void CloseValve(U16 valve)
{
    Valve.Cmd &= ~valve;
}

U16 GetValveStatus(void)
{
    return Valve.Cur;
}

#if( TEST_VALVE_ONOFF == 1 )
U16 the_valve_cmd = 0x0000;
#endif
U16 GetValveCmd(void)
{

#if( TEST_VALVE_ONOFF == 1 )
    return the_valve_cmd;
#else
    return Valve.Cmd;
#endif
}

U8 IsOpenValve(U16 valve)
{
    if( (Valve.Cur & valve ) == valve )
    {
        return TRUE;
    }

    return FALSE;
}

void ControlValve(void)
{
    U8 i = 0U;
    U16 val = 0U;
    U16 diff = 0U;
    U16 cmd = 0U;

    /* Check Boot Time... all off valves.. */
    if( Valve.BootTime != 0 )
    {
        Valve.BootTime--;
        return ;
    }

    cmd = GetValveCmd();

    /* Check different between command and current status */
    diff = Valve.Cur ^ cmd;
    if( diff == 0 )
    {
        return;
    }

    /* First - turn off */
    for( i=0; i<16; i++ )
    {
        val = 1 << i;
        if( diff & val )
        {
            if( (cmd & val) == 0 )
            {
                API_CloseValve(i);
                Valve.Cur &= ~val;
            }
        }
    }

    /* And then... turn on valve */
    diff = Valve.Cur ^ cmd;
    if( diff == 0 )
    {
        return ;
    }
    for( i = 0 ; i < 16 ; i++ )
    {
        val = 1 << i;
        if( diff & val )
        {
            if( (cmd & val) != 0 )
            {
                API_OpenValve(i);
                Valve.Cur |= val;
            }
        }
    }
}

void TEST_ControlValve(void)
{
    U16 cmd;

    cmd = 1U << Valve.TEST_Count;
    CloseValve(0xFFFF);  // close all valve
    OpenValve(cmd);   // open valve
    if( Valve.TEST_Count++ >= MAX_VALVE_NUM )
    {
        Valve.TEST_Count =0;
    }

    ControlValve();
}
