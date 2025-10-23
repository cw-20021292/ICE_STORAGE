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
#define ICE_FULL_CHECK_COUNT  (2U)   /// 최종 만빙 확전 전 카운트
#else
#define ICE_FULL_CHECK_COUNT  (0U)   /// 최종 만빙 확전 전 카운트
#endif

SIceFull_T IceFull;

void InitIceFull(void)
{
    IceFull.Full = FALSE;            /// 만빙 상태
    IceFull.WeakFull = FALSE;            /// Weak 만빙 상태

    IceFull.FullCheckCount = 0U;     /// 만빙 검사 횟수 ( ice가 make 조건일 때만 이 횟수만큼 체크되어야 함 )

    IceFull.Start = TRUE;            /// 만빙 검사 시작 FLAG
    IceFull.CheckTime = CHECK_TIME;  /// 만빙 검사 시간
    IceFull.CheckCount = 0;          /// ADC 기준치 초과 횟수

    IceFull.PowerOnCheck = FALSE;    ///  전원 RESET 후, 최초 검사 실행 여부
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
    /// 만빙이 풀릴 시에만 0이기 때문에 추가횟수 초기화 해도 됨
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


/* 만빙 검사 동작 진행 여부 반환
 *
 * Return :
 *  TRUE - 검사 완료
 *  FALSE - 검사 중...
 */


U8 IsDoneCheckIceFull(void)
{
    if( IceFull.Start == TRUE )
    {
        return FALSE; // 검사 진행 중..
    }

    return TRUE; // 검사 종료
}

void CheckIceFull(void)
{
    if( IceFull.Start == FALSE )
    {
        return ;
    }

    /* 1. 만빙 검사 - IR 출력 */
    if( GET_STATUS_ICE_INFRARED_HIGH() == 0 ) 
    {
        TURN_ON_ICE_INFRARED_HIGH();

        IceFull.CheckTime = CHECK_TIME;
        IceFull.CheckCount = 0;
        return ;
    }

    /* 2. 만빙 검사 - ADC 값 측정*/
    /* 만빙 수신부 10초간 확인, 얼음이 없는 경우 ADC 값이 높게 나옴 */ 
    if( GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH) >= ICE_FULL_ADC_LEVEL )
    {
        IceFull.CheckCount++;
    }

    IceFull.CheckTime--;
    if( IceFull.CheckTime != 0 )
    {
        return ;
    }

    /* 3. 검사 종료 후, 결과 판정 */
    IceFull.Start = FALSE;
    TURN_OFF_ICE_INFRARED_HIGH();

    if( IceFull.CheckCount >= CHECK_CONDITION )
    {
        IceFull.WeakFull = FALSE;     /// 얼음 없음
        IceFull.Full = FALSE;         /// 얼음 없음 
        IceFull.FullCheckCount = 0U;
    }
    else
    {
        IceFull.WeakFull = TRUE;  /// Weak 만빙

        if( IceFull.FullCheckCount == 0U )
        {
            IceFull.Full = TRUE;   /// 만빙
        }
    }

    /* 4. Power On Check */
    IceFull.PowerOnCheck = TRUE;
}
