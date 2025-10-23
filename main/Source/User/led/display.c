/**
 * File : display.c
 * 
 * Led Display
*/
#include "display.h"
#include "display_lib.h"

#include "led.h"
#include "drain.h"
#include "water_out.h"
#include "power_saving.h"
#include "ice.h"
#include "hot_water.h"
#include "cold_water.h"
#include "front.h"
#include "temp.h"
#include "error.h"
#include "ice_water_out.h"
#include "purifier.h"

#include "segment.h"
#include "segment_ui.h"

void SetLedParameter(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
         /// Type (DIM, DUYT)
         SetLedType(i, LED_TYPE_DUTY_DIM);

         /// On Duty
         SetLedOnDuty(i, LED_DUTY_ON_100);

         /// Off Duty
         SetLedOffDuty(i, LED_DUTY_OFF);
    }

    /// Duty 튜닝 적용
    //SetLedOnDuty(LED_ID_UV_CARE_SYSTEM, LED_DUTY_ON_70);
    SetLedOnDuty(LED_ID_STATUS_ICE_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_MAX, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_ICE_PREPARING, LED_DUTY_ON_90);

    SetLedOnDuty(LED_ID_STATUS_ICE_FULL, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_WATER_PREPARING, LED_DUTY_ON_90);

#if 0
    SetLedOnDuty(LED_ID_AMOUNT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_COLD, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ROOM, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_HOT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE_WATER_AMOUNT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE, LED_DUTY_ON_90);
#endif

    SetLedOnDuty(LED_ID_WELCOME_RIGHT_WHITE, LED_DUTY_ON_50);
    SetLedOnDuty(LED_ID_WELCOME_LEFT_WHITE, LED_DUTY_ON_50);

}

void SetLedAllType(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
         SetLedType(i, LED_TYPE_DUTY_DIM);    
    }
}

void SetLedAllOnDuty(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
         SetLedOnDuty(i, LED_DUTY_ON_100);    
    }

    /// Duty 튜닝 적용
    //SetLedOnDuty(LED_ID_UV_CARE_SYSTEM, LED_DUTY_ON_70);
    SetLedOnDuty(LED_ID_STATUS_ICE_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_MAX, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_ICE_PREPARING, LED_DUTY_ON_90);

    SetLedOnDuty(LED_ID_STATUS_ICE_FULL, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_STATUS_WATER_PREPARING, LED_DUTY_ON_90);

#if 0
    SetLedOnDuty(LED_ID_AMOUNT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_COLD, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ROOM, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_HOT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE_WATER_AMOUNT, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE_WATER, LED_DUTY_ON_90);
    SetLedOnDuty(LED_ID_ICE, LED_DUTY_ON_90);
 #endif
 
    SetLedOnDuty(LED_ID_WELCOME_RIGHT_WHITE, LED_DUTY_ON_50);
    SetLedOnDuty(LED_ID_WELCOME_LEFT_WHITE, LED_DUTY_ON_50);
}

void SetLedSpecificOnDuty(U8 led, U8 duty)
{
    SetLedOnDuty(led, duty);    
}

void SetLedAllOffDuty(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
         SetLedOffDuty(i, LED_DUTY_OFF);    
    }
}

void SetLedSpecificOffDuty(U8 led, U8 duty)
{
    SetLedOffDuty(led, duty);
}

void SetLedDispSavingAllOnDuty(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
         SetLedOnDuty(i, LED_DUTY_ON_30);    
    }
}

void DispUvLedOffDuty(void)
{
    SetLedSpecificOffDuty(LED_ID_UV_ICE_TANK, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_UV_ICE_TRAY, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_UV_ICE_FAUCET, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_UV_WATER_FAUCET, LED_DUTY_ON_30);   
}

void DispIceAmountLedOffDuty(void)
{
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_LOW, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_MIDDLE, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_HIGH, LED_DUTY_ON_30);
}

void DispAmountLedOffDuty(void)
{
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_LOW, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_MIDDLE, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_HIGH, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_INFINITE, LED_DUTY_ON_30);  
}

void DispWaterOutBarLedOffDuty(void)
{
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_LOW, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_MIDDLE, LED_DUTY_ON_30);
    SetLedSpecificOffDuty(LED_ID_STATUS_BAR_HIGH, LED_DUTY_ON_30);
}

void TurnOffAllLED(void)
{
    OffAllLed();
}

void TurnOnAllLED(void)
{
    OnAllLed();
}

// 얼음 추출
// 0 : circle OFF
// 1 ~ 4 circle ON
void DispIceOut(U8 onOff)
{
    if( onOff == OFF )
    {
        // WELCOM 
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
    }
    else
    {
        // WELCOM 
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, ON);
    }
}

// 물 추출
void DispWaterOut(U8 onOff)
{
    if( onOff == OFF )
    {
        // WELCOM 
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
    }
    else
    {
        // WELCOM 
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, ON);
    }
}

typedef enum _temp_level_
{
    TEMP_LEVEL_NONE = 0,
    TEMP_LEVEL_WEAK,  
    TEMP_LEVEL_STRONG,  
} ETempLevel_T;
// hysteresis
#define TEMP_OFFSET   (2.0f)
static U8 GetHotTempCondition(TEMP_T temp)
{
    static U8 level = TEMP_LEVEL_NONE;

#if 0
    if(  ( GetConfigHot() == FALSE )
             || ( IsErrorType(ERROR_TYPE_HOT_MAKE) == TRUE ) )
    {
        uLevel = TEMP_LEVEL_NONE;
        return uLevel;
    }
#endif

    if( GetHotWaterConfigMake() == FALSE )
    {
        level = TEMP_LEVEL_NONE;
        return level;
    }

    if( IsErrorType(ERROR_TYPE_HOT_MAKE) == TRUE )
    {
        level = TEMP_LEVEL_WEAK;
        return level;
    }

    if( level == TEMP_LEVEL_NONE )
    {
        if( temp >= (GetHotWaterTargetOffTemp() - TEMP_OFFSET) )
        {
            level = TEMP_LEVEL_STRONG;     // strong
        }
        else
        {
            level = TEMP_LEVEL_WEAK;      // weak
        }
    }
    else if( level == TEMP_LEVEL_WEAK )
    {
        if( temp >= (GetHotWaterTargetOffTemp() - TEMP_OFFSET) )
        {
            level = TEMP_LEVEL_STRONG;
        }
    }
    else if( level == TEMP_LEVEL_STRONG )
    {
        if( temp < ((GetHotWaterTargetOffTemp() - TEMP_OFFSET)-(6.0f)) )
        {
            level = TEMP_LEVEL_WEAK;
        }
    }

    return level;
}


// hysteresis
static U8 GetColdTempCondition(TEMP_T temp)
{
    static U8 level = TEMP_LEVEL_NONE;

#if 0
    if(  ( GetConfigCold() == FALSE )
             || ( IsErrorType(ERROR_TYPE_COLD_MAKE) == TRUE ) )
    {
        uLevel = TEMP_LEVEL_NONE;
        return uLevel;
    }
#endif

    if( GetColdWaterConfigMake() == FALSE )             
    {
        level = TEMP_LEVEL_NONE;
        return level;
    }

    if( IsErrorType(ERROR_TYPE_COLD_MAKE) == TRUE )
    {
        level = TEMP_LEVEL_WEAK;
        return level;
    }

    if( level == TEMP_LEVEL_NONE )
    {
        if( temp > 6.0f )    // 6도 기준
        {
            level = TEMP_LEVEL_WEAK;
        }
        else
        {
            level = TEMP_LEVEL_STRONG;
        }
    }
    else if( level == TEMP_LEVEL_WEAK )
    {
        if( temp <= 6.0f )
        {
            level = TEMP_LEVEL_STRONG;
        }
    }
    else if( level == TEMP_LEVEL_STRONG )
    {
        if( temp > ((6.0f)+(5.0f)) )
        {
            level = TEMP_LEVEL_WEAK;
        }
    }

    return level;
}

U8 DispTempStatus(U8 sel, TEMP_T temp)
{
    U8 level = 0U;

    if( sel == SEL_WATER_HOT )
    {
        level = GetHotTempCondition(temp);

        // 약이면 7초 디밍 후 점등
        // 강이면 표시 없음      
        if( level == TEMP_LEVEL_WEAK )
        {
            if( IsExpiredDispTimer(DISP_TIMER_ID_KEY_HOT) == TRUE )
            {
                return TEMP_STATUS_HEATING;   /// 점등
            }

            return TEMP_STATUS_HEATING_DIM;   /// 디밍 점멸
        }

        return TEMP_STATUS_NONE;
    }
    else if( sel == SEL_WATER_COLD )
    {
        level = GetColdTempCondition(temp);

        // 약이면 7초 디밍 후 점등
        // 강이면 표시 없음      
        if( level == TEMP_LEVEL_WEAK )
        {
            if( IsExpiredDispTimer(DISP_TIMER_ID_KEY_COLD) == TRUE )
            {
                return TEMP_STATUS_COOLING;   /// 점등
            }

            return TEMP_STATUS_COOLING_DIM;   /// 디밍 점멸
        }

        return TEMP_STATUS_NONE;
    }

    // ROOM
    return TEMP_STATUS_NONE;

}

// 냉수 온도 3단계 표시
void DispColdTempLevel(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_COOLING, onOff);
}

// 온수 온도 3단계 표시
void DispHotTempLevel(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_HEATING, onOff);
}

// 냉온정 선택
void DispWaterSel(U8 sel)
{
    if( sel == SEL_WATER_COLD )
    {
        OnOffLed(LED_ID_COLD, ON);
        OnOffLed(LED_ID_ROOM, OFF);
        OnOffLed(LED_ID_HOT, OFF);

        /// Color 출력
        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, ON);
        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);   
    }
    else if( sel == SEL_WATER_HOT )
    {
        OnOffLed(LED_ID_COLD, OFF);
        OnOffLed(LED_ID_ROOM, OFF);
        OnOffLed(LED_ID_HOT, ON);

        /// Color 출력
        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_RED, ON);
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF); 
    }
    else
    {
        // SEL_WATER_ROOM
        OnOffLed(LED_ID_COLD, OFF);
        OnOffLed(LED_ID_ROOM, ON);
        OnOffLed(LED_ID_HOT, OFF);

        /// Color 출력
        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, ON); 
    }
}

// 얼음 선택
void DispIceSel(U8 sel)
{
    if( sel == SEL_ICE_WATER )
    {
        OnOffLed(LED_ID_ICE, OFF);
        OnOffLed(LED_ID_ICE_WATER, ON);

        /// Color 출력
        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, ON);
        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF); 
    }
    else
    {
        // SEL_ICE
        OnOffLed(LED_ID_ICE, ON);
        OnOffLed(LED_ID_ICE_WATER, OFF);

        /// Color 출력
        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, ON); 
    }
}

/* 물 추출 LED 제어 
 *
 * Param
 *  - mu8OnOff : ON, OFF 
 */
void DispWelcomWaterWhite(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, onOff);
}

void DispWelcomWaterRed(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_RIGHT_RED, onOff);
}

void DispWelcomWaterGreen(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, onOff);
}

void DispWelcomWaterBlue(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, onOff);
}

void DispWelcomIceWhite(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_LEFT_WHITE, onOff);
}

void DispWelcomIceRed(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_LEFT_RED, onOff);
}

void DispWelcomIceGreen(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_LEFT_GREEN, onOff);
}

void DispWelcomIceBlue(U8 onOff)
{
    OnOffLed(LED_ID_WELCOME_LEFT_BLUE, onOff);
}

void DispUvCareSystem(U8 onOff)
{
    OnOffLed(LED_ID_UV_CARE_SYSTEM, onOff);
}

// 물양 종류 - 변경 중
void DispAmountSel(U8 amount)
{

#if 0

  // ICE 4.0

    HAL_DimmingLed( AMOUNT_USER,        ON );
    HAL_DimmingLed( AMOUNT_INFINITY,    ON );
    HAL_DimmingLed( AMOUNT_500,         ON );
    HAL_DimmingLed( AMOUNT_250,         ON );
    HAL_DimmingLed( AMOUNT_120,         ON );

    // AMOUNT 
    HAL_TurnOnOffLED( AMOUNT, ON );

    // AMOUNT TYPE
    if( mu8Amount == FIXED_AMOUNT_INFINITY )
    {
        HAL_TurnOnOffLED( AMOUNT_USER,      OFF );
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  ON );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( mu8Amount == FIXED_AMOUNT_USER )
    {
        HAL_TurnOnOffLED( AMOUNT_USER,      ON );
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( mu8Amount == FIXED_AMOUNT_TWO )
    {
        HAL_TurnOnOffLED( AMOUNT_USER,      OFF );
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       ON );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( mu8Amount == FIXED_AMOUNT_ONE )
    {
        HAL_TurnOnOffLED( AMOUNT_USER,      OFF );
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       ON );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else
    {
        // FIXED AMOUNT_HALF
        HAL_TurnOnOffLED( AMOUNT_USER,      OFF );
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       ON );
    }
#endif
}

// 물양 종류  표시 - 최종 상태
void DispIceWaterAmount(U8 onOff)
{
    OnOffLed(LED_ID_ICE_WATER_AMOUNT, onOff);
}

void DispAmount(U8 onOff)
{
    OnOffLed(LED_ID_AMOUNT, onOff);
}

/// 얼음 잠김
void DispIceLock(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_LOCK, onOff);
}

/// 얼음물 잠김
void DispIceWaterLock(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, onOff);
}

/// 얼음물 용량 잠김
void DispIceWaterAmountLock(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, onOff);
}

/// 온수 잠김
void DispHotLock(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_HOT_LOCK, onOff);
}

void DispRoomWater(U8 onOff)
{
    OnOffLed(LED_ID_ROOM, onOff);
}

void DispPowerSaving(U8 onOff)
{
    /// blink
}

void DispSter(U8 onOff)
{
    /// blink
}

void DispSterWaterFaucet(U8 onOff)
{
    OnOffLed(LED_ID_UV_WATER_FAUCET, onOff);
}

void DispSterIceFaucet(U8 onOff)
{
    OnOffLed(LED_ID_UV_ICE_FAUCET, onOff);
}

void DispSterIceRoom(U8 onOff)
{
    OnOffLed(LED_ID_UV_ICE_TANK, onOff);
}

void DispSterIceTray(U8 onOff)
{
    OnOffLed(LED_ID_UV_ICE_TRAY, onOff);
}

void DispStatusIceWater(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER, onOff);
}

void DispStatusMax(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_MAX, onOff);
}

void DispStatusWater(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_WATER, onOff);
}




void DispDrain(U8 onOff)
{
    /// blink
}

void DispColdWater(U8 onOff)
{
    OnOffLed(LED_ID_COLD, onOff);
}

void DispService(U8 onOff)
{
    /// blink
}

void DispConty(U8 onOff)
{
    /// blink
}

void DispIce(U8 onOff)
{
    OnOffLed(LED_ID_ICE, onOff);
}

void DispIceWater(U8 onOff)
{
    OnOffLed(LED_ID_ICE_WATER, onOff);
}

void DispIceMake(U8 onOff)
{
    OnOffLed(LED_ID_ICE_OFF, onOff);
}

void DispColdMake(U8 onOff)
{
    OnOffLed(LED_ID_COLD_OFF, onOff);
}

void DispUnitMl(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_UNIT_ML, onOff);
}

void DispConty500mL(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_BAR_LOW, onOff);
}

void DispConty1L(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_BAR_MIDDLE, onOff);
}

void DispConty1D5L(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_BAR_HIGH, onOff);
}

void DispContyInfinity(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_BAR_INFINITE, onOff);
}

void DispTankCleanBlink(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_BAR_LOW, onOff);
    OnOffLed(LED_ID_STATUS_BAR_MIDDLE, onOff);
    OnOffLed(LED_ID_STATUS_BAR_HIGH, onOff);
    OnOffLed(LED_ID_STATUS_BAR_INFINITE, onOff);
}

void DispIceFullHigh(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_FULL, onOff);
}

void DispIceFullLow(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_PREPARING, onOff);
}

void DispWaterPreparing(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_WATER_PREPARING, onOff);
}

void DispUnitCelsius(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, onOff);
}

void DispHotWater(U8 onOff)
{
    OnOffLed(LED_ID_HOT, onOff);
}

void DispConfUnusedSaveTrueBlink(U8 onOff)
{
    /// blink
}

void DispConfUnusedSaveFalseBlink(U8 onOff)
{
    /// blink
}

void DispLockIce(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_LOCK, onOff);
}

void DispLockIceWater(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, onOff);
}

void DispLockIceWaterAmount(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, onOff);
}

void DispLockAll(U8 onOff)
{
    OnOffLed(LED_ID_ICE_LOCK, onOff);
    OnOffLed(LED_ID_HOT_LOCK, onOff);
}

void DispLockAllSetBlink(U8 onOff)
{
    OnOffLed(LED_ID_ICE_LOCK, onOff);
    OnOffLed(LED_ID_HOT_LOCK, onOff);
}

void DispLockAllBlink(U8 onOff)
{
    OnOffLed(LED_ID_ICE_LOCK, onOff);
    OnOffLed(LED_ID_HOT_LOCK, onOff);
}

void DispLock(U8 onOff)
{
    /// blink
}

void DispIceMaker(U8 onOff)
{
    // blink
}

void DispIceTurbo(U8 onOff)
{
    /// blink
}

void DispSettingIceLock(U8 onOff)
{
    OnOffLed(LED_ID_ICE_LOCK, onOff);
}

void DispSettingIceLockBlink(U8 onOff)
{
    OnOffLed(LED_ID_AMOUNT, onOff);
    OnOffLed(LED_ID_ICE_LOCK, onOff);
}

void DispSettingIceWaterLock(U8 onOff)
{
    OnOffLed(LED_ID_ICE_WATER_LOCK, onOff);
}

void DispSettingIceWaterLockBlink(U8 onOff)
{
    OnOffLed(LED_ID_AMOUNT, onOff);
    OnOffLed(LED_ID_ICE_WATER_LOCK, onOff);
}

void DispSettingIceWaterAmountLockBlink(U8 onOff)
{
    OnOffLed(LED_ID_ICE_WATER_AMOUNT, onOff);
    OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, onOff);
}

void DispSettingIceWaterAmountNotiBlink(U8 onOff)
{
    OnOffLed(LED_ID_ICE_WATER_AMOUNT, onOff);
    OnOffLed(LED_ID_ICE_WATER, onOff);
}

void DispSettingHotLock(U8 onOff)
{
    OnOffLed(LED_ID_HOT_LOCK, onOff);
}

void DispSettingHotLockBlink(U8 onOff)
{
    OnOffLed(LED_ID_AMOUNT, onOff);
    OnOffLed(LED_ID_HOT_LOCK, onOff);
}

void DispLockIceSetBlink(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_LOCK, onOff);
}

void DispLockIceWaterSetBlink(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, onOff);
}

void DispLockHotSetBlink(U8 onOff)
{
    OnOffLed(LED_ID_STATUS_HOT_LOCK, onOff);
}

void DispMainVersionBar(void)
{
    OnOffGroupASeg(SEG_1, SEG_B);
}

void DispFrontVersionBar(void)
{
    OnOffGroupASeg(SEG_1, SEG_C);
}

void DispVersion(U8 val)
{
    /// Segment Display ( Version )
    DispGaSegmentDigitsVer((U16)val);   
}

// 버전 표시
U8 DispVersionOnTime(void)
{
    static U16 delay = 200U;    // 2sec.. @10ms..

    if( delay != 0)
    {
        delay--;

        DispMainVersionBar();
        DispVersion(GetMainVerMiner());

        return FALSE;
    }

    return TRUE;
}

U8 DispFrontVersionOnTime(void)
{
    static U16 delay = 200U;    // 2sec.. @10ms..

    if( delay != 0)
    {
        delay--;

        DispFrontVersionBar();
        DispVersion(GetFrontVerMiner());

        return FALSE;
    }

    return TRUE;
}

/// segment
void AllOnSegment(void)
{
    AllOnGroupASeg();
}

void AllOffSegment(void)
{
    AllOffGroupASeg();
}

void DispSegStr(const char *str)
{
    DispGaSegmentStr(str);
}

void DispSegDigits(U16   amount)
{
    DispGaSegmentDigits(amount);
}

void DispSegNegativeDigits(I16    amount)
{
    DispGaSegmentNegativeDigits(amount);
}

void DispSegInfinity(void)
{
    DispGaSegment(SEG_OFF, SEG_EFAD, SEG_AD, SEG_ABCD);
}

void DispSegError(U8 code)
{
    OnOffGroupASeg( SEG_1,  SEG_OFF  );
    DispGaSegmentStrDigit("E", code);
}

void DispSegStrDigit(const char *str, U8 digits)
{
    DispGaSegmentStrDigit(str, digits);
}

typedef struct _segment_action_
{
    U8 Seg_1000;
    U8 Seg_100;
    U8 Seg_10;
    U8 Seg_1;
} SSegmentAction_T;

const static SSegmentAction_T WaterOutInfinity[] =
{
    { SEG_OFF,  SEG_OFF, SEG_OFF, SEG_OFF   },
    { SEG_OFF,  SEG_E,   SEG_OFF, SEG_OFF   },
    { SEG_OFF,  SEG_EF,  SEG_OFF, SEG_OFF   },
    { SEG_OFF,  SEG_EFA, SEG_OFF, SEG_OFF   },
    { SEG_OFF,  SEG_EFA, SEG_A,   SEG_OFF   },
    { SEG_OFF,  SEG_EFA, SEG_A,   SEG_A     },
    { SEG_OFF,  SEG_EFA, SEG_A,   SEG_AB    },
    { SEG_OFF,  SEG_EFA, SEG_A,   SEG_ABC   },
    { SEG_OFF,  SEG_EFA, SEG_A,   SEG_ABCD  },
    { SEG_OFF,  SEG_EFA, SEG_AD,  SEG_ABCD  },
    { SEG_OFF,  SEG_EFAD,SEG_AD,  SEG_ABCD  },

    { SEG_OFF,  SEG_FAD, SEG_AD,  SEG_ABCD  },
    { SEG_OFF,  SEG_AD,  SEG_AD,  SEG_ABCD  },
    { SEG_OFF,  SEG_D,   SEG_AD,  SEG_ABCD  },
    { SEG_OFF,  SEG_D,   SEG_D,   SEG_ABCD  },
    { SEG_OFF,  SEG_D,   SEG_D,   SEG_BCD   },
    { SEG_OFF,  SEG_D,   SEG_D,   SEG_CD    },
    { SEG_OFF,  SEG_D,   SEG_D,   SEG_D     },
    { SEG_OFF,  SEG_D,   SEG_D,   SEG_OFF   },
    { SEG_OFF,  SEG_D,   SEG_OFF, SEG_OFF   },   
};

#define SZ_INFINITY_LIST    (sizeof(WaterOutInfinity)/sizeof(SSegmentAction_T))

#define DELAY_INFINITY   (2)  // 200ms @100ms
U8 infinityCount = 0U;
U8 infinityDelay = DELAY_INFINITY;
void ResetInfinityCount(void)
{
    infinityCount = 0;
    infinityDelay = DELAY_INFINITY;
}

// 연속 추출 화면 표시
// > 화면 돌아가는 표시
// > mL 표시
void DispWaterOutAmountInfinity(void)
{
    SSegmentAction_T *pVal;

    pVal = &WaterOutInfinity[infinityCount];
    OnOffGroupASeg(SEG_1, pVal->Seg_1000);
    OnOffGroupASeg(SEG_2, pVal->Seg_100);
    OnOffGroupASeg(SEG_3, pVal->Seg_10);
    OnOffGroupASeg(SEG_4, pVal->Seg_1);

    if( infinityDelay != 0 )
    {
        infinityDelay--;
        return ;
    }

    infinityDelay = DELAY_INFINITY;
    infinityCount++;

    if( infinityCount >= SZ_INFINITY_LIST )
    {
        infinityCount = 0;
    }
}

const static SSegmentAction_T WaterOutLever[] =
{
    { SEG_OFF, SEG_A,   SEG_A,   SEG_A   },
    { SEG_OFF, SEG_AG,  SEG_AG,  SEG_AG  },
    { SEG_OFF, SEG_AGD, SEG_AGD, SEG_AGD }, 
};

#define SZ_LEVER_LIST    (sizeof(WaterOutLever)/sizeof(SSegmentAction_T))

#define DELAY_LEVER   (5)  // 200ms @100ms
U8 leverCount = 0U;
U8 leverDelay = DELAY_LEVER;
void ResetLeverCount(void)
{
    leverCount = 0;
    leverDelay = DELAY_LEVER;
}

// 연속 추출 화면 표시
// > 화면 돌아가는 표시
// > mL 표시
void DispWaterOutLever(void)
{
    SSegmentAction_T *pVal;

    pVal = &WaterOutLever[leverCount];
    OnOffGroupASeg(SEG_1, pVal->Seg_1000);
    OnOffGroupASeg(SEG_2, pVal->Seg_100);
    OnOffGroupASeg(SEG_3, pVal->Seg_10);
    OnOffGroupASeg(SEG_4, pVal->Seg_1);

    if( leverDelay != 0 )
    {
        leverDelay--;
        return ;
    }

    leverDelay = DELAY_LEVER;
    leverCount++;

    if( leverCount >= SZ_LEVER_LIST )
    {
        leverCount = 0;
    }
}

// 7세그먼트에 캐릭터 표시
void DispSegmentChar(U8 id)
{
    if( id == SEGMENT_CHAR_ID_ON )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,  CHAR_O );
        //OnOffGroupASeg( SEGMENT_10,   CHAR_n );
        //OnOffGroupASeg( SEGMENT_1,    CHAR_SPACE);
    }
    else if( id == SEGMENT_CHAR_ID_OFF )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,  CHAR_O );
        //OnOffGroupASeg( SEGMENT_10,   CHAR_F );
        //OnOffGroupASeg( SEGMENT_1,    CHAR_F );
    }
    else if( id == SEGMENT_CHAR_ID_CON )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,  CHAR_C );
        //OnOffGroupASeg( SEGMENT_10,   CHAR_o );
        //OnOffGroupASeg( SEGMENT_1,    CHAR_n );
    }
    else if( id == SEGMENT_CHAR_ID_STACK_BAR_CENTER )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   STACK_BAR_CENTER  );
        //OnOffGroupASeg( SEGMENT_10,    STACK_BAR_CENTER  );
        //OnOffGroupASeg( SEGMENT_1,     STACK_BAR_CENTER  );
    }
    else if( id == SEGMENT_CHAR_ID_STACK_BAR_TOP )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   STACK_BAR_TOP  );
        //OnOffGroupASeg( SEGMENT_10,    STACK_BAR_TOP  );
        //OnOffGroupASeg( SEGMENT_1,     STACK_BAR_TOP  );
    }
    else if( id == SEGMENT_CHAR_ID_BAR_BOTTOM )
    {
        //OnOffGroupASeg( SEGMENT_1000,  CHAR_SPACE  );
        ///OnOffGroupASeg( SEGMENT_100,   BAR_BOTTOM  );
        //OnOffGroupASeg( SEGMENT_10,    BAR_BOTTOM  );
        ///OnOffGroupASeg( SEGMENT_1,     BAR_BOTTOM  );
    }
    else if( id == SEGMENT_CHAR_ID_BAR_CENTER )
    {
        //OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   BAR_CENTER  );
        //OnOffGroupASeg( SEGMENT_10,    BAR_CENTER  );
        //OnOffGroupASeg( SEGMENT_1,     BAR_CENTER  );
    }
    else if( id == SEGMENT_CHAR_ID_BAR_TOP )
    {
        ///OnOffGroupASeg( SEGMENT_1000, CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   BAR_TOP  );
        //OnOffGroupASeg( SEGMENT_10,    BAR_TOP  );
        //OnOffGroupASeg( SEGMENT_1,     BAR_TOP  );
    }
    else if( id == SEGMENT_CHAR_ID_CHP )
    {
        //OnOffGroupASeg( SEGMENT_1000,  CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   CHAR_C  );
        //OnOffGroupASeg( SEGMENT_10,    CHAR_H  );
        //OnOffGroupASeg( SEGMENT_1,     CHAR_P  );
    }
    else if( id == SEGMENT_CHAR_ID_CP )
    {
        //OnOffGroupASeg( SEGMENT_1000,  CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   CHAR_C  );
        //OnOffGroupASeg( SEGMENT_10,    CHAR_P  );
        //OnOffGroupASeg( SEGMENT_1,     CHAR_SPACE  );
    }
    else if( id == SEGMENT_CHAR_ID_CIRCLE )
    {
        //OnOffGroupASeg( SEGMENT_1000,  CHAR_SPACE  );
        //OnOffGroupASeg( SEGMENT_100,   BAR_BRAKET_LEFT );
        //OnOffGroupASeg( SEGMENT_10,    BAR_TOP_BOTTOM );
        //OnOffGroupASeg( SEGMENT_1,     BAR_BRAKET_RIGHT );
    }
    else if( id == SEGMENT_CHAR_ID_ICE_1 )
    {
        OnOffGroupASeg( SEG_1,  SEG_OFF  );
        OnOffGroupASeg( SEG_2,   RING_TOP );
        OnOffGroupASeg( SEG_3,    SEG_OFF );
        OnOffGroupASeg( SEG_4,     SEG_OFF );
    }
    else if( id == SEGMENT_CHAR_ID_ICE_2 )
    {
        OnOffGroupASeg( SEG_1,  SEG_OFF  );
        OnOffGroupASeg( SEG_2,   RING_BOTTOM );
        OnOffGroupASeg( SEG_3,    RING_TOP );
        OnOffGroupASeg( SEG_4,     SEG_OFF );
    }
    else if( id == SEGMENT_CHAR_ID_ICE_3 )
    {
        OnOffGroupASeg( SEG_1,  SEG_OFF  );
        OnOffGroupASeg( SEG_2,   RING_TOP );
        OnOffGroupASeg( SEG_3,    RING_BOTTOM );
        OnOffGroupASeg( SEG_4,     RING_TOP );
    }
    else if( id == SEGMENT_CHAR_ID_ICE_4 )
    {
        OnOffGroupASeg( SEG_1,  SEG_OFF  );
        OnOffGroupASeg( SEG_2,   RING_BOTTOM );
        OnOffGroupASeg( SEG_3,    RING_TOP );
        OnOffGroupASeg( SEG_4,     RING_BOTTOM );
    }
    else if( id == SEGMENT_CHAR_ID_CENTER_1 )
    {
        //DispCenterVal( 1 );
    }
    else if( id == SEGMENT_CHAR_ID_CENTER_2 )
    {
        //DispCenterVal( 2 );
    }
    else if( id == SEGMENT_CHAR_ID_CENTER_3 )
    {
        //DispCenterVal( 3 );
    }
    else if( id == SEGMENT_CHAR_ID_SE )
    {
        //DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        //DispSegment( SEGMENT_100,   CHAR_SPACE );
        //DispSegment( SEGMENT_10,    CHAR_S );
        //DispSegment( SEGMENT_1,     CHAR_E );
    }
    else if( id == SEGMENT_CHAR_ID_CO )
    {
        //DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        //DispSegment( SEGMENT_100,   CHAR_SPACE );
        //DispSegment( SEGMENT_10,    CHAR_C );
        //DispSegment( SEGMENT_1,     CHAR_O );
    }
    else if( id == SEGMENT_CHAR_ID_STE )
    {
        //DispSegment( SEGMENT_1000,  CHAR_SPACE );
        //DispSegment( SEGMENT_100,   CHAR_S );
        //DispSegment( SEGMENT_10,    CHAR_T );
        //DispSegment( SEGMENT_1,     CHAR_E );
    }
    else
    {
        // mu8CharId == SPACE
        //DispSegment( SEGMENT_1000, CHAR_SPACE );
        //DispSegment( SEGMENT_100,  CHAR_SPACE );
        //DispSegment( SEGMENT_10,   CHAR_SPACE );
        //DispSegment( SEGMENT_1,    CHAR_SPACE );
    }
}

