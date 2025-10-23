#include "front.h"
#include "eeprom.h"
#include "ice.h"

typedef struct _front_
{
    U8 configDeco[DECO_LED_MAX];      // Deco LED ON/OFF

    /* KEY - LOCK */
    U8 lockAll;             // LOCK or UNLOCK
    U8 lockHot;             // LOCK or UNLOCK
    U8 lockIce;             // LOCK or UNLOCK
    U8 lockIceWater;        // LOCK or UNLOCK
    U8 lockIceWaterAmount;  // LOCK or UNLOCK

    U8 factorySetup;    // FALSE or TRUE

    U32 unusedTime;     // @min
} SFront_T;

SFront_T front;

void InitFront(void)
{
    front.configDeco[DECO_LED_WELCOME_ICE] = TRUE;
    front.configDeco[DECO_LED_WELCOME_WATER] = TRUE;
    front.lockAll = UNLOCK;
    front.lockHot = UNLOCK;
    front.lockIce = UNLOCK;
    front.lockIceWater = UNLOCK;
    front.lockIceWaterAmount = UNLOCK;

    front.factorySetup = FALSE;

    front.unusedTime = 0UL;
}

void SetFactroySetup(U8 setup)
{
    front.factorySetup = setup;
}

U8 GetFactorySetup(void)
{
    return front.factorySetup;
}

// ���� �ʱ�ȭ RESET�� ���, 
// �� ����� ���� �ʱ�ȭ �Ϸ�. 
// �״��� EEPROM ���� ���� ���Ŀ� ȣ��Ǿ�� �Ѵ�.
// ���� ���
// 1. ���� ���� Ż�� ���
// 2. ����� ���� �ð� �ʱ�ȭ?
void CheckFactroySetup(void)
{
    if( front.factorySetup == FALSE )
    {
        ClearIceMakeMode(ICE_MODE_DUMMY_DEICING);

        front.factorySetup = TRUE;
        SaveEepromId(EEP_ID_FACTORY_SETUP);
    }
}

void SetConfigDeco(U8 id, U8 onOff)
{
    if( onOff != TRUE && onOff != FALSE )
    {
        return ;
    }
    front.configDeco[id] = onOff;
    SaveEepromId(EEP_ID_CONF_DECO_LED);
}

U8 GetConfigDeco(U8 id)
{
    return front.configDeco[id];
}

void SetLockAll(U8 lock)
{
    if( lock != LOCK && lock != UNLOCK )
    {
        return ;
    }

    front.lockAll = lock;
    SaveEepromId(EEP_ID_LOCK_ALL);
}

U8 GetLockAll(void)
{
    return front.lockAll;
}

void SetLockHot(U8 lock)
{
    if( lock != LOCK && lock != UNLOCK )
    {
        return ;
    }

    front.lockHot = lock;
    SaveEepromId( EEP_ID_LOCK_HOT );
}

U8 GetLockHot(void)
{
    return front.lockHot;
}

void SetLockIce(U8 lock )
{
    if( lock != LOCK && lock != UNLOCK )
    {
        return ;
    }

    front.lockIce = lock;
    SaveEepromId( EEP_ID_LOCK_ICE );
}

U8 GetLockIce(void)
{
    return front.lockIce;
}

void SetLockIceWater(U8 lock)
{
    if( lock != LOCK && lock != UNLOCK )
    {
        return ;
    }

    front.lockIceWater = lock;
    SaveEepromId(EEP_ID_LOCK_ICE_WATER);
}

U8 GetLockIceWater(void)
{
    return front.lockIceWater;
}

/// ������ �뷮 ���
void SetLockIceWaterAmount(U8 lock)
{
    if( lock != LOCK && lock != UNLOCK )
    {
        return ;
    }

    front.lockIceWaterAmount = lock;
    SaveEepromId(EEP_ID_LOCK_ICE_WATER_AMOUNT);
}

U8 GetLockIceWaterAmount(void)
{
    return front.lockIceWaterAmount;
}

// �� ���� �̻�� �ð�
U32 GetSystemUnusedTime(void)
{
    return front.unusedTime;
}

void ResetUnusedTime(void)
{
    front.unusedTime = 0UL;
}

void CountingUnusedTime(void)
{
    if( front.unusedTime <= 0xFFFFFFFFUL )
    {
        front.unusedTime++;
    }
}
