/**
 * File : ice_full.h
 * 
 * Ice Full
*/
#include "hw.h"
#include "api_adc.h"
#include "ice_full.h"

#define ICE_FULL_ADC_LEVEL    (300) /* 1.47V */

#define CHECK_TIME            (100) /* @100ms, 10sec */
#define CHECK_CONDITION       (50) 

#if( CONFIG_MAKE_MORE == 1 )
#define ICE_FULL_CHECK_COUNT  (2U)   /// ���� ���� Ȯ�� �� ī��Ʈ
#else
#define ICE_FULL_CHECK_COUNT  (0U)   /// ���� ���� Ȯ�� �� ī��Ʈ
#endif

SIceFull_T IceFull;

void InitIceFull(void)
{
    IceFull.Full = FALSE;            /// ���� ����
    IceFull.WeakFull = FALSE;            /// Weak ���� ����

    IceFull.FullCheckCount = 0U;     /// ���� �˻� Ƚ�� ( ice�� make ������ ���� �� Ƚ����ŭ üũ�Ǿ�� �� )

    IceFull.Start = TRUE;            /// ���� �˻� ���� FLAG
    IceFull.CheckTime = CHECK_TIME;  /// ���� �˻� �ð�
    IceFull.CheckCount = 0;          /// ADC ����ġ �ʰ� Ƚ��

    IceFull.PowerOnCheck = FALSE;    ///  ���� RESET ��, ���� �˻� ���� ����
}

void GetIceFullData(SIceFull_T *pIceFull)
{
    MEMCPY( (void __FAR *)pIceFull, (const void __FAR *)&IceFull, sizeof( SIceFull_T ));
}

U8 GetIceFullStatus(void)
{
    return IceFull.Full;
}

U8 GetIceWeakFullStatus(void)
{
    return IceFull.WeakFull;
}

void SetIceFullCheckCount(void)
{
    /// ������ Ǯ�� �ÿ��� 0�̱� ������ �߰�Ƚ�� �ʱ�ȭ �ص� ��
    if( IceFull.FullCheckCount == 0 )
    {
        IceFull.FullCheckCount = ICE_FULL_CHECK_COUNT;
    }
}

void UpdateIceFullCheckCount(void)
{
    if( IceFull.WeakFull == FALSE )
    {
        return;
    }

    if( IceFull.FullCheckCount != 0 )
    {
        IceFull.FullCheckCount--;
    }
}

U8 GetIceFullPowerOnCheck(void)
{
    return IceFull.PowerOnCheck;
}

void StartCheckIceFull(void)
{
    IceFull.Start = TRUE;
}

void StopCheckIceFull(void)
{
    IceFull.Start = FALSE;
}


/* ���� �˻� ���� ���� ���� ��ȯ
 *
 * Return :
 *  TRUE - �˻� �Ϸ�
 *  FALSE - �˻� ��...
 */


U8 IsDoneCheckIceFull(void)
{
    if( IceFull.Start == TRUE )
    {
        return FALSE; // �˻� ���� ��..
    }

    return TRUE; // �˻� ����
}

void CheckIceFull(void)
{
    if( IceFull.Start == FALSE )
    {
        return ;
    }

    /* 1. ���� �˻� - IR ��� */
    if( GET_STATUS_ICE_INFRARED_HIGH() == 0 ) 
    {
        TURN_ON_ICE_INFRARED_HIGH();

        IceFull.CheckTime = CHECK_TIME;
        IceFull.CheckCount = 0;
        return ;
    }

    /* 2. ���� �˻� - ADC �� ����*/
    /* ���� ���ź� 10�ʰ� Ȯ��, ������ ���� ��� ADC ���� ���� ���� */ 
    if( GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH) >= ICE_FULL_ADC_LEVEL )
    {
        IceFull.CheckCount++;
    }

    IceFull.CheckTime--;
    if( IceFull.CheckTime != 0 )
    {
        return ;
    }

    /* 3. �˻� ���� ��, ��� ���� */
    IceFull.Start = FALSE;
    TURN_OFF_ICE_INFRARED_HIGH();

    if( IceFull.CheckCount >= CHECK_CONDITION )
    {
        IceFull.WeakFull = FALSE;     /// ���� ����
        IceFull.Full = FALSE;         /// ���� ���� 
        IceFull.FullCheckCount = 0U;
    }
    else
    {
        IceFull.WeakFull = TRUE;  /// Weak ����

        if( IceFull.FullCheckCount == 0U )
        {
            IceFull.Full = TRUE;   /// ����
        }
    }

    /* 4. Power On Check */
    IceFull.PowerOnCheck = TRUE;
}
