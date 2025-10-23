/**
 * File : relay.c
 * 
 * Relay
*/
#include "relay.h"

#define MK_EX_TYPE  (RELAY_COMP | RELAY_HEATER)
#define MK_CON_TYPE (~MK_EX_TYPE)

/* Relay 제어는 배차적인 제어 방식과 동시 제어 방식으로 구분된다.
 *
 * 배타 제어 방식은 1초 간격으로 제어된다.
 * 동시에 turn on 될 수 없다.
 *
 * 동시 제어 방식은 제어 주기 타이머에 영향을 받지 않으며,
 * 동시에 turn on 될 수 있다.
 *
*/
typedef struct _relay_
{
    U16 Cmd;      // On/Off command ( 1: ON, 0: OFF )
    U16 Cur;      // current status ( 1: ON, 0: OFF )

    // exclusive type
    U16 CmdEx;      // On/Off command ( 1: ON, 0: OFF )
    U16 CurEx;      // current status ( 1: ON, 0: OFF )

    // concurrent type
    U16 CmdCon;   
    U16 CurCon;   

    U8 TEST_Count;   // TEST counter varaible
} SRelay_T;

SRelay_T Relay;

void InitRelay(void)
{
    Relay.Cmd = 0;
    Relay.Cur = 0;

    Relay.CmdEx = 0;
    Relay.CurEx = 0;

    Relay.CmdCon = 0;
    Relay.CurCon = 0;
    Relay.TEST_Count = 0;
}

void TurnOnRelay(U16 relay)
{
    if( (relay & MK_EX_TYPE) != 0 )
    {
        Relay.CmdEx |= (relay & MK_EX_TYPE);
    }
    else
    {
        Relay.CmdCon |= (relay & MK_CON_TYPE);
    }
}

void TurnOffRelay(U16 relay)
{
    Relay.Cmd &= ~relay;

    if( (relay & MK_EX_TYPE) != 0 )
    {
        Relay.CmdEx &= ~(relay & MK_EX_TYPE);
    }
    else
    {
        Relay.CmdCon &= ~(relay & MK_CON_TYPE);
    }
}

void TurnOffRelayAll(void)
{
    Relay.Cmd = 0U;
    Relay.CmdEx = 0U;
    Relay.CmdCon = 0U;
}

U8 IsTurnOnRelay(U16 relay)
{
    U16 cmd = 0U;

    cmd = Relay.CmdEx | Relay.CmdCon;
    if( (cmd & relay) == relay )
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetRelayStatus(void)
{
    U16 cmd = 0U;

    cmd = Relay.CurEx | Relay.CurCon;
    return cmd;
}

static U16 OnOffRelay(U8 ex, U16 cur, U16 cmd)
{
    U8 i = 0U;
    U16 val = 0U;
    U16 diff = 0U;

    /// Check different between command and current status
    diff = cur ^ cmd;
    if( diff == 0 )
    {
        /// 제어 명령과 현재 상태에 차이가 없으면 종료
        return cur;
    }

    /// OFF 명령 부터 처리
    for( i=0; i<16; i++ )
    {
        // bit shift하면서 differ한 부분을 찾는다.
        val = 1 << i;
        if( diff & val )
        {
            // bit가 clear이면 off제어
            if( ( cmd & val ) == 0 )
            {
                OffRelay(i);
                cur &= ~val;
            }
        }
    }

    /// ON 명령 처리
    diff = cur ^ cmd;   
    if( diff == 0 )
    {
        /// 제어 명령과 현재 상태에 차이가 없으면 종료
        return cur;
    }

    for( i=0; i<16; i++ )
    {
        /// bit shift하면서 differ 한 부분을 찾는다.
        val = 1 << i;
        if( diff & val )
        {
            // bit가 set이면 on 제어
            if( ( cmd & val) != 0 )
            {
                OnRelay( i );
                cur |= val;

                // 배타적으로 한 번에 하나의 relay만 on 할 수 있음
                if( ex == TRUE )
                {
                    return cur;
                }
            }
        }
    }

    return cur;
}

static void ControlRelayExclusive(void)
{
    Relay.CurEx = OnOffRelay(TRUE, Relay.CurEx, Relay.CmdEx);
}

static void ControlRelayConcurrent(void)
{
    Relay.CurCon = OnOffRelay(FALSE, Relay.CurCon, Relay.CmdCon);
}

void ProcessRelayExclusive(void)
{
    ControlRelayExclusive();
}

void ProcessRelayConcurrent(void)
{
    ControlRelayConcurrent();
}

#if 0
void TEST_ControlRelay(void)
{
    U16 cmd;

    cmd = 1 << relay.TEST_Count;
    TurnOffRelay(0xFFFF);   // turn off all relay
    TurnOnRelay(cmd);   // turn on relay
    if( relay.TEST_Count++ >= MAX_RELAY_NUM )
    {
        relay.TEST_Count = 0;
    }

    ControlRelayExclusive();
    ControlRelayConcurrent();
}
#endif
