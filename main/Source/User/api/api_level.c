/**
 * File : api_level.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "api_level.h"

#include "api_input.h"

#if ( CONFIG_USE_LEVEL_ROOM_TANK == 1 )

U8 GetLevelRoomTank(void)
{
    U8 uVal = 0U;

    if( GetPinLevelRoomTankLow() == HIGH )
    {
        uVal |= LEVEL_DETECT_LOW;
    }

    if( GetPinLevelRoomTankHigh() == HIGH )
    {
        uVal |= LEVEL_DETECT_HIGH;
    }

    if( GetPinLevelRoomTankOverflow() == HIGH )
    {
        uVal |= LEVEL_DETECT_OVF;
    }

    return uVal;
}

#endif

#if ( CONFIG_USE_LEVEL_HOT_TANK == 1 )

U8 GetLevelHotTank(void)
{
    U8 uVal = 0U;

    /**
     * ���� ��ũ�� ������ ������
     * �¼� ��ũ�� ������ ���� ������ �Ѵ�.
     *
     * ���� ������ ������ ������ ������ ���� 2���� �ִٰ� �����Ѵ�.
    */

    if( GetPinLevelRoomTankLow() == HIGH )
    {
        uVal |= LEVEL_DETECT_LOW;
        uVal |= LEVEL_DETECT_HIGH;
    }

    return uVal;
}

#endif

#if ( CONFIG_USE_LEVEL_COLD_TANK == 1 )

U8 GetLevelColdTank(void)
{
    U8 uVal = 0U;

    if( GetPinLevelColdTankLow() == HIGH )
    {
        uVal |= LEVEL_DETECT_LOW;
    }

    if( GetPinLevelColdTankHigh() == HIGH )
    {
        uVal |= LEVEL_DETECT_HIGH;
    }

    return uVal;
}

#endif

#if ( CONFIG_USE_LEVEL_DRAIN_TANK == 1 )

U8 GetLevelDrainTank(void)
{
    U8 uVal = 0U;

    if( GetPinLevelDrainTankLow() == HIGH )
    {
        uVal |= LEVEL_DETECT_LOW;
    }

    if( GetPinLevelDrainTankHigh() == HIGH )
    {
        uVal |= LEVEL_DETECT_HIGH;
    }

    return uVal;
}

#endif

#if ( CONFIG_USE_LEVEL_ICE_TRAY == 1 )

U8 GetLevelIceTray(void)
{
    U8 uVal = 0U;

    /// Ʈ���� �Լ� �������� �巹�� �������� �����Ǹ� ���� �̶�� ��
    if( GetPinLevelDrainTankLow() == HIGH )
    {
        uVal |= LEVEL_DETECT_LOW;
        uVal |= LEVEL_DETECT_HIGH;
    }

    return uVal;
}

#endif
