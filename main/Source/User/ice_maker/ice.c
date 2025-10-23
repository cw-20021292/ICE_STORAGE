/**
 * File : ice.c
 * 
 * Ice Control
*/
#include "ice.h"
#include "ice_full.h"
#include "ice_low.h"
#include "eeprom.h"
#include "error.h"
#include "health.h"
#include "power_saving.h"
#include "room_water.h"
#include "cold_water.h"
#include "comp.h"
#include "front.h"
#include "drain_water.h"
#include "tank_clean.h"
#include "temp.h"
#include "service.h"
#include "flow_meter.h"
#include "input.h"

#include <string.h>

/* ICE FULL */
#define  ICE_CHECK_TIME      (900)   /// @1sec, 15min

#define  ICE_STOP_TIME       (14400) /// @1sec, 4Hour

/* DE-ICING 진입 시간 */
#define MIN_DEICING_TIME     (180U)  ///@1sec, 3min

#define DEFAULT_RETURN_TIME  (700)   /// 7sec @10ms

#define ICING_MAX_COUNT      (10)    /// 10회 맥스 카운트 ( 10회가지는 냉수채움 안함 )

SIce_T Ice;

void InitIce(void)
{
    Ice.Turbo = FALSE;

    Ice.ConfigMake = TRUE;
    Ice.Make = FALSE;

    Ice.Mode = ICE_MODE_DUMMY_DEICING;

    Ice.FullCheckTimer = 0;
    Ice.StopMakeTimer = 0;

    Ice.DummyDeIcing = FALSE;

    Ice.DeIcingCompTime = MIN_DEICING_TIME;

    Ice.IcingCount = ICING_MAX_COUNT;
}

void GetIceData(SIce_T *pIce)
{
    MEMCPY( (void __FAR *)pIce, (const void __FAR *)&Ice, sizeof(SIce_T) );
}

void SetIceTurbo(U8 val)
{
    if( val != TRUE && val != FALSE )
    {
        return ;
    }
    Ice.Turbo = val;

    SaveEepromId( EEP_ID_CONF_TURBO );
}

U8  GetIceTurbo(void)
{
    return Ice.Turbo;
}

void SetIceConfigMake(U8 val)
{
    if( val != TRUE && val != FALSE )
    {
        return ;
    }

    Ice.ConfigMake = val;

    SaveEepromId(EEP_ID_CONF_MAKE_ICE);
}

U8 GetIceConfigMake(void)
{
    return Ice.ConfigMake;
}

void  SetIceMake(U8 val)
{
   Ice.Make = val;
}

U8 GetIceMake(void)
{
    return Ice.Make;
}


// 제빙 운전 온도 검사 
U8 IsValidDoIcingAmbiTempRange(void)
{
    TEMP_T  tAmbi;

    tAmbi = GetTemp( TEMP_ID_AMBIENT_2 );
    if( tAmbi < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

U8 IsValidDoIcingColdTempRange(void)
{
    TEMP_T tCold;

    tCold = GetTemp(TEMP_ID_COLD_WATER);
    if( tCold < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

void SetIceMakeMode(U16 mode)
{
    Ice.Mode |= mode;
}

void ClearIceMakeMode(U16 mode)
{
    Ice.Mode &= ~mode;
}

U16 IsSetIceMakeMode(U16 mode )
{
    if( (Ice.Mode & mode ) == mode)
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetIceMakeMode(void )
{
    return Ice.Mode;
}

void SetIceFullCheckTimer(U16 val)
{
    Ice.FullCheckTimer = val;
}

// 더미 탈빙 동작 여부 설정/반환
U8 GetIceDummyDeIcing( void )
{
    return Ice.DummyDeIcing;
}

void SetIceDummyDeIcing(U8 val)
{
    Ice.DummyDeIcing = val;
}

// 강제 탈빙을 위한 최소 압축기 운전 시간
U16 GetDeIcingCompOnTime(void)
{
    return Ice.DeIcingCompTime;
}

void ResetDeIcingCompOnTime(void)
{
     Ice.DeIcingCompTime = MIN_DEICING_TIME;
}

void UpdateDeIcingCompOnTime(void)
{
    if( Ice.DeIcingCompTime != 0 )
    {
        Ice.DeIcingCompTime--;
    }
}

void UpdateIcingCount(void)
{
    if( Ice.Make == FALSE )
    {
        return;
    }

    if( Ice.IcingCount < ICING_MAX_COUNT )
    {
        Ice.IcingCount++;
    }
    else
    {
        Ice.IcingCount = 0U;
    }
}

void ClearIcingCount(void)
{
    Ice.IcingCount = ICING_MAX_COUNT;
}

U8 IsMaxIcingCount(void)
{
    if( Ice.IcingCount == ICING_MAX_COUNT )
    {
        return TRUE;
    }

    return FALSE;
}

/* 추출 대기 시간 초과하면 재시도 지연시간 */
#define ICE_OUTAGE_TIMER        600U    //10min // @100ms
void StartIceOutageTimer(void)
{
    Ice.OutageTimer = ICE_OUTAGE_TIMER;
}

static void CheckIceOutage(void)
{
    if( Ice.OutageTimer != 0 )
    {
        Ice.OutageTimer--;
    }

    if( GetFlowMeterLPM() > 0 )
    {
        Ice.OutageTimer = 0;
    }
}

void SetIceTrayMovingRetry(void)
{
    SetIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
}

void StopIceMakeLoad(void)
{
    // 제빙 시퀀스 해제
    ClearIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
    SetIceMakeMode(ICE_MODE_OFF_LOAD);
}

// 이동 탈빙 클리어(void)
void ClearIcingMoveDeicing(void)
{
    ClearIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
}

// 더미 탈빙 
void SetDummyDeicing(void)
{
    // 제빙 시퀀스 해제
    ClearIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);

    // 더미 탈빙
    SetIceMakeMode(ICE_MODE_DUMMY_DEICING);
}

// 제빙 중지
U8 StopIceMake(void)
{
    if( IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE 
            || IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE
            || IsSetIceMakeMode(ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE
            || IsSetIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE )
    {
        // 초기화, 예열, 준비 단계이면 제빙 동작 취소하고 트레이 탈빙 위치로 이동
        ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
        SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_OFF_LOAD );

        return TRUE;
    }
    else if( IsSetIceMakeMode(ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE )
    {
        if( GetDeIcingCompOnTime() == 0 )
        {
            // 제빙 중이면, 탈빙 단계로 이동
            // 단, 제빙 최소 시간 만족 후...
            ClearIceMakeMode(ICE_MODE_ICING);
        }
        else
        {
            // 제빙 시간이 짧은 경우, 그냥 취소...
            ClearIceMakeMode(ICE_MODE_ICING | ICE_MODE_DEICING);
            SetIceMakeMode(ICE_MODE_INIT | ICE_MODE_OFF_LOAD);
        }

        return TRUE;
    }
    else if( IsSetIceMakeMode(ICE_MODE_ICING_MOVE_DEICING) == TRUE )
    {
        ClearIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
        SetIceMakeMode(ICE_MODE_INIT | ICE_MODE_OFF_LOAD);
    }

    // 탈빙 중이면, 탈빙을 완료한다.
    return FALSE;
}

/* 만빙검사 */
void  MakeIceFull(void)
{
    static U8 prevIceFull = FALSE;
    U8 iceFull;

    // 절전 중에 만빙이 되면, StopMakeTimer 만큼 제빙을 중지한다.
    iceFull = GetIceFullStatus();
    if( prevIceFull != iceFull )
    {
        prevIceFull = iceFull;
        if( iceFull == TRUE && GetSavingStatus() == TRUE )
        {
            Ice.StopMakeTimer = ICE_STOP_TIME;
        }
    }
    // 단, 절전이 해제되면 제빙 중지 상태를 해제한다.
    if( GetSavingStatus() == FALSE )
    {
        Ice.StopMakeTimer = 0;
    }

    if( Ice.StopMakeTimer != 0 )
    {
        Ice.StopMakeTimer--;
    }

    // 만빙 검사 중이면, 만빙 검사 주기 타이머 업데이트
    if( IsDoneCheckIceFull() == FALSE )
    {
        SetIceFullCheckTimer(ICE_CHECK_TIME);
    }

    // 만빙 검사 타이머 감소...
    if( Ice.FullCheckTimer != 0 )
    {
        Ice.FullCheckTimer--;
        return ;
    }

    // 만빙 검사 시작
    // 그리고 타이머 reload
    StartCheckIceFull();
    StartCheckIceLow();
}

static U8 IsDetctTankLevel(void)
{
    U8 roomLevel;
    U8 coldlevel;

    roomLevel = GetRoomWaterLevel();

    if( roomLevel == LEVEL_LOW 
        || roomLevel == LEVEL_ERR_LOW
        || roomLevel == LEVEL_ERR_COMPLEX
        || roomLevel == LEVEL_ERR_OVF_LOW )
    {
        return FALSE;
    }

    /**
     * 냉수 저수위 미감지이면 제빙 중지
     * 냉수로 제빙하면서 변경되는 점
     * 냉수에 물이 없을 때 트레이 입수 시 순환펌프가 계속 동작됨
    */
    coldlevel = GetColdWaterLevel();

    if( coldlevel == LEVEL_LOW )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 IsValidMake(void)
{
    // 설정에 의한 중지
    if( Ice.ConfigMake == FALSE )
    {
        return FALSE ;
    }

    /* 외기, 정수 온도 영하 조건 */
    if( IsValidDoIcingColdTempRange() == FALSE 
            || IsValidDoIcingAmbiTempRange() == FALSE )
    {
        return FALSE;
    }

//    /* 강제 순환 배수 */
//    if( IsGoingOnDrainWaterFull() == TRUE )
//    {
//        return FALSE;
//    }

    /* 압축기 구속 시간 */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    // 압축기 보호 시간 초과 
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }

    // 만빙 최초 검사가 완료되지 않은 경우, 제빙 동작하지 않는다.
    if( GetIceFullPowerOnCheck() == FALSE )
    {
        return  FALSE;
    }

    // 정수, 냉수 최초 만수위 조건 검사
    if( GetRoomWaterInitFull() == FALSE 
            || GetColdWaterInitFull() == FALSE )
    {
        return FALSE;
    }

    // 정수 / 냉수 저수위 조건
    if( IsDetctTankLevel() == FALSE )
    {
        return FALSE;
    }

    // 절전 타이머 ( 절전 중 만빙이 된 경우 )
    if( Ice.StopMakeTimer != 0 )
    {
        return FALSE;
    }

    // 탱크 탑 커버 OPEN 시, 제빙 중지
    if( GetServiceCheckTankOpen() == TRUE )
    {
        return FALSE;
    }

    // Tray Braket 미감지 시 , 제빙 중지
    if( GetCurrentInputVal(INPUT_ID_TRAY_BRACKET) == TRUE )
    {
        return FALSE;
    }
    

    // 에러에 의한 중지
    if( IsErrorType(ERROR_TYPE_ICE_MAKE) == TRUE )
    {
        return FALSE;   // 제빙 못함~
    }

    // 단수로 인한 대기...
    if( Ice.OutageTimer != 0 )
    {
        return FALSE;
    }

    // HEALTH 동작 중 ( 살균/배수 )
    if( IsStartHealth() == TRUE )
    {
        return FALSE;
    }

    // 냉수 청소 모드
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    return TRUE;  // 제빙 go~ go~ 
}

void MakeIce(void)
{
    U8 make = FALSE;
    static U8 prevMake = FALSE;

    CheckIceOutage();
  
    if( GetIceFullStatus() == FALSE )
    {
        make = TRUE;
    }
    else
    {
        make = FALSE;
    }

    if( IsValidMake() == FALSE )
    {
        make = FALSE;
    }

    if( prevMake != make )
    {
        prevMake = make;
    
        if( make == TRUE )
        {
            SetIceFullCheckCount();
        }
        else
        {
            ClearIcingCount();
        }
    }

    // Set new make status
    SetIceMake(make);
}
