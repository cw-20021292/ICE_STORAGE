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
    IceLow.Low = FALSE;            /// 저빙 상태

    IceLow.Start = TRUE;            /// 저빙 검사 시작 FLAG
    IceLow.CheckTime = CHECK_TIME;  /// 저빙 검사 시간
    IceLow.CheckCount = 0;          /// ADC 기준치 초과 횟수

    IceLow.PowerOnCheck = FALSE;    ///  전원 RESET 후, 최초 검사 실행 여부
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


/* 만빙 검사 동작 진행 여부 반환
 *
 * Return :
 *  TRUE - 검사 완료
 *  FALSE - 검사 중...
 */


U8 IsDoneCheckIceLow(void)
{
    if( IceLow.Start == TRUE )
    {
        return FALSE; // 검사 진행 중..
    }

    return TRUE; // 검사 종료
}

void CheckIceLow(void)
{
    if( IceLow.Start == FALSE )
    {
        return ;
    }

    /* 1. 만빙 검사 - IR 출력 */
    if( GET_STATUS_ICE_INFRARED_LOW() == 0 ) 
    {
        TURN_ON_ICE_INFRARED_LOW();

        IceLow.CheckTime = CHECK_TIME;
        IceLow.CheckCount = 0;
        return ;
    }

    /* 2. 만빙 검사 - ADC 값 측정*/
    /* 만빙 수신부 10초간 확인, 얼음이 없는 경우 ADC 값이 높게 나옴 */ 
    if( GetAdcData(ADC_ID_SENSOR_INFRARED_LOW) >= ICE_LOW_ADC_LEVEL )
    {
        IceLow.CheckCount++;
    }

    IceLow.CheckTime--;
    if( IceLow.CheckTime != 0 )
    {
        return ;
    }

    /* 3. 검사 종료 후, 결과 판정 */
    IceLow.Start = FALSE;
    TURN_OFF_ICE_INFRARED_LOW();

    if( IceLow.CheckCount >= CHECK_CONDITION )
    {
        IceLow.Low = FALSE;    // 얼음 없음
    }
    else
    {
        IceLow.Low = TRUE;     // 만빙
    }

    /* 4. Power On Check */
    IceLow.PowerOnCheck = TRUE;
}
