/**
 * File : relay.c
 * 
 * Relay
*/
#include "relay.h"

#define MK_EX_TYPE  (RELAY_COMP | RELAY_HEATER)
#define MK_CON_TYPE (~MK_EX_TYPE)

/* Relay ����� �������� ���� ��İ� ���� ���� ������� ���еȴ�.
 *
 * ��Ÿ ���� ����� 1�� �������� ����ȴ�.
 * ���ÿ� turn on �� �� ����.
 *
 * ���� ���� ����� ���� �ֱ� Ÿ�̸ӿ� ������ ���� ������,
 * ���ÿ� turn on �� �� �ִ�.
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
        /// ���� ��ɰ� ���� ���¿� ���̰� ������ ����
        return cur;
    }

    /// OFF ��� ���� ó��
    for( i=0; i<16; i++ )
    {
        // bit shift�ϸ鼭 differ�� �κ��� ã�´�.
        val = 1 << i;
        if( diff & val )
        {
            // bit�� clear�̸� off����
            if( ( cmd & val ) == 0 )
            {
                OffRelay(i);
                cur &= ~val;
            }
        }
    }

    /// ON ��� ó��
    diff = cur ^ cmd;   
    if( diff == 0 )
    {
        /// ���� ��ɰ� ���� ���¿� ���̰� ������ ����
        return cur;
    }

    for( i=0; i<16; i++ )
    {
        /// bit shift�ϸ鼭 differ �� �κ��� ã�´�.
        val = 1 << i;
        if( diff & val )
        {
            // bit�� set�̸� on ����
            if( ( cmd & val) != 0 )
            {
                OnRelay( i );
                cur |= val;

                // ��Ÿ������ �� ���� �ϳ��� relay�� on �� �� ����
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
