/**
 * File : drain.c
 * 
 * Drain Tank Level Control Module
*/
#include "drain.h"
#include "drain_pump.h"
#include "level.h"
#include "eeprom.h"
#include "ice_full.h"

typedef struct _drain_
{
    U8 InitDrain;         /// �巹�� ��ũ �ʱ�ȭ FLAG
    U8 InitDrainReady;    /// �巹�� ��ũ �ʱ�ȭ READY
    U16 InitTime;         /// �巹�� ��ũ �ʱ�ȭ�� ��12�� ���۽� �ִ� ���۽ð� 2��
    U8 Level;             /// �巹�� ��ũ ����
    
    U32 RottenTime;       /// ���ι� ����. ( �巹�� ������ ���� ���� �ð� )
    U16 MinOnTimeErrLow;  /// ������ ���� ����� �ּ� ���� ���� �ð�
    U16 MinOnTime;        /// ���� �ǵ�� ���� ���� off �� �ּ� ���� �ð�
} SDrain_T;

SDrain_T Drain;

#define INITIAL_DRAIN_MAX  (200)                       /// /20sec, @100msec
#define ROTTEN_TIME        ( 3UL * 60UL * 60UL * 10UL ) /// 3hour, @100ms
#define MIN_ON_TIME        ( 40UL * 10UL )             /// 40sec, @100ms
#define MIN_HALF_ON_TIME   ( 20UL * 10UL )             /// 20sec, @100ms */

/*Init*/
void  InitDrain(void)
{
    Drain.InitDrain = FALSE;
    Drain.InitDrainReady = FALSE;
    Drain.InitTime = 0;
    Drain.Level = LEVEL_LOW;

    Drain.RottenTime = ROTTEN_TIME;
    Drain.MinOnTimeErrLow = 0;
    Drain.MinOnTime = 0;
}

/* Drain Level */
void SetDrainLevel(U8 val)
{
    Drain.Level = val;
}

U8 GetDrainLevel(void)
{
    return Drain.Level;
}

/*InitDrain*/
void SetInitDrain(U8 val)
{
    Drain.InitDrain = val;
}

U8 GetInitDrain(void)
{
    return Drain.InitDrain;
}

void SetInitDrainReady(U8 val)
{
    Drain.InitDrainReady = val;
}

U8 GetInitDrainReady(void)
{
    return Drain.InitDrainReady;
}

void StartDrainPumpOnLevel(void)
{
    if( Drain.Level != LEVEL_LOW )
    {
        TurnOnDrainPump();
    }
}

/// ������ �Ǹ� ��� ������ �Ѵ�.
static U8 IsSetIceFull(U8 level)
{
    static U8 prev = FALSE;
    U8 cur;

    cur = GetIceFullStatus();
    if( prev != cur )
    {
        prev = cur;
        if( cur == TRUE && level != LEVEL_LOW )
        {
            return TRUE;   // turn on drian pump
        }
    }

    return FALSE; // do not turn on drain pump
}

/* �巹�� ��ũ ���� */
void ControlDrainLevel(void)
{
    static U8 initTime = 30; /// 3sec @100ms

    /* Wait Delay Time for Detect Level sensor */
    if( initTime != 0 )
    {
        initTime--;
        return ;
    }

    /* Update Watere Level */
    Drain.Level = GetTankLevel(LEVEL_ID_DRAIN);

    /* Init Drain */
    if( Drain.InitDrain == FALSE )
    {
        /// �ʱ� ��� �ð� ����
        //// ������ ���ϰ� ������ ����...
        if( GetDrainPumpStatus() == ON )
        {
            Drain.InitTime++;

            if( GetDrainPumpLoadStatus() == FALSE 
                    || Drain.InitTime > INITIAL_DRAIN_MAX )
            {
                TurnOffDrainPump();
                Drain.InitTime = 0;
                Drain.InitDrain = TRUE;
            }
        }

        /// ������ �Ʒ� �ܼ� ���Ÿ� ���� 20�� ���� ���� ���
#if (CONFIG_TANK_TYPE == TANK_RO)
        if( Drain.Level == LEVEL_LOW 
                && GetTankLevel( LEVEL_ID_COLD ) == LEVEL_LOW 
                   && GetTankLevel( LEVEL_ID_ROOM ) == LEVEL_LOW )
#else
        if( Drain.Level == LEVEL_LOW )
#endif
        {
            TurnOnDrainPump();
        }
        else if( Drain.Level != LEVEL_LOW )
        {
            TurnOnDrainPump();
        }
        else 
        {
            Drain.InitDrain = TRUE;
        }

        return;
    }


    if( GetDrainPumpStatus() == OFF )
    {
        Drain.MinOnTime = MIN_HALF_ON_TIME;
        
        // ������ ������ ���� �Ǹ� ������
        if( Drain.Level == LEVEL_HIGH )
        {
            TurnOnDrainPump();
        }
        // ���� ���� ���� ���� ���¿��� ���۽ÿ��� �ּ� ���� �ð� ����
        else if( Drain.Level == LEVEL_ERR_LOW )
        {
            TurnOnDrainPump();
            Drain.MinOnTimeErrLow = MIN_ON_TIME;
        }

        // ������ �Ǵ� ������ 1ȸ ��� ����
        if( IsSetIceFull(Drain.Level) == TRUE )
        {
            TurnOnDrainPump();
        }

        // ������ ���� ���� ���·� ��ð� ����ϸ�,
        // ���� ��°� �����ϱ� ���� ��� ����
        if( Drain.Level != LEVEL_MID )
        {
            Drain.RottenTime = ROTTEN_TIME;
        }
        else
        {
            if( Drain.RottenTime != 0 )
            {
                Drain.RottenTime--;
            }
            else
            {
                TurnOnDrainPump();
            }
        }
    }
    else 
    {
        Drain.RottenTime = ROTTEN_TIME;

        // ������ ����, ������ �̰��� ���ǿ��� ���� ���� �ð�.
        // ������ ���۵Ǹ� ������ �̰��� ���·� ����.
        // �̶� ���� �ּ� ���� �ð��� ����
        if( Drain.Level == LEVEL_LOW 
                && Drain.MinOnTimeErrLow == 0 )
        {
            TurnOffDrainPump();
        }
        else if( Drain.Level == LEVEL_MID 
                || Drain.Level == LEVEL_LOW )
        {
            // ������ ������ ��� ���� ������ ���,
            // ���� �ǵ���� ��ȸ�� �����̸� ���� off
            if( GetDrainPumpLoadStatus() == FALSE 
                    && Drain.MinOnTime == 0 )
            {
                TurnOffDrainPump();
            }
        }

        // �ּ� ���� �ð�
        if( Drain.MinOnTimeErrLow != 0 )
        {
            Drain.MinOnTimeErrLow--;
        }
        
        if( Drain.MinOnTime != 0 )
        {
            Drain.MinOnTime--;
        }
    }
}
