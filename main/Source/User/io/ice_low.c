/**
 * File : ice_low.h
 * 
 * Ice Low
*/
#include "hw.h"
#include "api_adc.h"
#include "ice_low.h"

#define  ICE_LOW_ADC_LEVEL   (300) /* 1.47V */

#define  CHECK_TIME           (100) /* @100ms, 10sec */
#define  CHECK_CONDITION      (50) 

SIceLow_T IceLow;

void InitIceLow(void)
{
    IceLow.Low = FALSE;            /// ���� ����

    IceLow.Start = TRUE;            /// ���� �˻� ���� FLAG
    IceLow.CheckTime = CHECK_TIME;  /// ���� �˻� �ð�
    IceLow.CheckCount = 0;          /// ADC ����ġ �ʰ� Ƚ��

    IceLow.PowerOnCheck = FALSE;    ///  ���� RESET ��, ���� �˻� ���� ����
}

void GetIceLowData(SIceLow_T *pIceLow)
{
    MEMCPY( (void __FAR *)pIceLow, (const void __FAR *)&IceLow, sizeof( SIceLow_T ));
}

U8 GetIceLowStatus(void)
{
    return IceLow.Low;
}

U8 GetIceLowPowerOnCheck(void)
{
    return IceLow.PowerOnCheck;
}

void StartCheckIceLow(void)
{
    IceLow.Start = TRUE;
}

void StopCheckIceLow(void)
{
    IceLow.Start = FALSE;
}


/* ���� �˻� ���� ���� ���� ��ȯ
 *
 * Return :
 *  TRUE - �˻� �Ϸ�
 *  FALSE - �˻� ��...
 */


U8 IsDoneCheckIceLow(void)
{
    if( IceLow.Start == TRUE )
    {
        return FALSE; // �˻� ���� ��..
    }

    return TRUE; // �˻� ����
}

void CheckIceLow(void)
{
    if( IceLow.Start == FALSE )
    {
        return ;
    }

    /* 1. ���� �˻� - IR ��� */
    if( GET_STATUS_ICE_INFRARED_LOW() == 0 ) 
    {
        TURN_ON_ICE_INFRARED_LOW();

        IceLow.CheckTime = CHECK_TIME;
        IceLow.CheckCount = 0;
        return ;
    }

    /* 2. ���� �˻� - ADC �� ����*/
    /* ���� ���ź� 10�ʰ� Ȯ��, ������ ���� ��� ADC ���� ���� ���� */ 
    if( GetAdcData(ADC_ID_SENSOR_INFRARED_LOW) >= ICE_LOW_ADC_LEVEL )
    {
        IceLow.CheckCount++;
    }

    IceLow.CheckTime--;
    if( IceLow.CheckTime != 0 )
    {
        return ;
    }

    /* 3. �˻� ���� ��, ��� ���� */
    IceLow.Start = FALSE;
    TURN_OFF_ICE_INFRARED_LOW();

    if( IceLow.CheckCount >= CHECK_CONDITION )
    {
        IceLow.Low = FALSE;    // ���� ����
    }
    else
    {
        IceLow.Low = TRUE;     // ����
    }

    /* 4. Power On Check */
    IceLow.PowerOnCheck = TRUE;
}
