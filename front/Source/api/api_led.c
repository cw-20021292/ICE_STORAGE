/**
 * File : api_led.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_led.h"
#include "hal_timer.h"

void LedInit(void)
{
    HAL_TIMER01_START();
}

void LedOnOffUvCareSystem(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_UV_CARE_SYSTEM();
    }
    else
    {
        TURN_OFF_LED_UV_CARE_SYSTEM();
    }
}

void LedOnOffUvIceTank(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_UV_ICE_TANK();
    }
    else
    {
        TURN_OFF_LED_UV_ICE_TANK();
    }
}

void LedOnOffUvIceTray(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_UV_ICE_TRAY();
    }
    else
    {
        TURN_OFF_LED_UV_ICE_TRAY();
    }
}

void LedOnOffUvIceFaucet(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_UV_ICE_FAUCET();
    }
    else
    {
        TURN_OFF_LED_UV_ICE_FAUCET();
    }
}

void LedOnOffUvWaterFaucet(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_UV_WATER_FAUCET();
    }
    else
    {
        TURN_OFF_LED_UV_WATER_FAUCET();
    }
}

void LedOnOffStatusIceWater(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_WATER();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_WATER();
    }
}

void LedOnOffStatusMax(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_MAX();
    }
    else
    {
        TURN_OFF_LED_STATUS_MAX();
    }
}

void LedOnOffStatusWater(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_WATER();
    }
    else
    {
        TURN_OFF_LED_STATUS_WATER();
    }
}

void LedOnOffSeg1A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_1A();
    }
    else
    {
        TURN_OFF_LED_SEG_1A();
    }
}

void LedOnOffSeg1B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_1B();
    }
    else
    {
        TURN_OFF_LED_SEG_1B();
    }
}

void LedOnOffStatusIcePreparing(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_PREPARING();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_PREPARING();
    }
}

void LedOnOffStatusIceFull(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_FULL();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_FULL();
    }
}

void LedOnOffStatusWaterPreparing(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_WATER_PREPARING();
    }
    else
    {
        TURN_OFF_LED_STATUS_WATER_PREPARING();
    }
}

void LedOnOffStatusUnitCelsius(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_UNIT_CELSIUS();
    }
    else
    {
        TURN_OFF_LED_STATUS_UNIT_CELSIUS();
    }
}

void LedOnOffSeg2A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2A();
    }
    else
    {
        TURN_OFF_LED_SEG_2A();
    }
}

void LedOnOffSeg2B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2B();
    }
    else
    {
        TURN_OFF_LED_SEG_2B();
    }
}

void LedOnOffSeg2C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2C();
    }
    else
    {
        TURN_OFF_LED_SEG_2C();
    }
}

void LedOnOffSeg2D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2D();
    }
    else
    {
        TURN_OFF_LED_SEG_2D();
    }
}

void LedOnOffSeg2E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2E();
    }
    else
    {
        TURN_OFF_LED_SEG_2E();
    }
}

void LedOnOffSeg2F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2F();
    }
    else
    {
        TURN_OFF_LED_SEG_2F();
    }
}

void LedOnOffSeg2G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_2G();
    }
    else
    {
        TURN_OFF_LED_SEG_2G();
    }
}

void LedOnOffSeg3A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3A();
    }
    else
    {
        TURN_OFF_LED_SEG_3A();
    }
}

void LedOnOffSeg3B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3B();
    }
    else
    {
        TURN_OFF_LED_SEG_3B();
    }
}

void LedOnOffSeg3C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3C();
    }
    else
    {
        TURN_OFF_LED_SEG_3C();
    }
}

void LedOnOffSeg3D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3D();
    }
    else
    {
        TURN_OFF_LED_SEG_3D();
    }
}

void LedOnOffSeg3E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3E();
    }
    else
    {
        TURN_OFF_LED_SEG_3E();
    }
}

void LedOnOffSeg3F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3F();
    }
    else
    {
        TURN_OFF_LED_SEG_3F();
    }
}

void LedOnOffSeg3G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_3G();
    }
    else
    {
        TURN_OFF_LED_SEG_3G();
    }
}

void LedOnOffSeg4A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4A();
    }
    else
    {
        TURN_OFF_LED_SEG_4A();
    }
}

void LedOnOffSeg4B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4B();
    }
    else
    {
        TURN_OFF_LED_SEG_4B();
    }
}

void LedOnOffSeg4C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4C();
    }
    else
    {
        TURN_OFF_LED_SEG_4C();
    }
}

void LedOnOffSeg4D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4D();
    }
    else
    {
        TURN_OFF_LED_SEG_4D();
    }
}

void LedOnOffSeg4E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4E();
    }
    else
    {
        TURN_OFF_LED_SEG_4E();
    }
}

void LedOnOffSeg4F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4F();
    }
    else
    {
        TURN_OFF_LED_SEG_4F();
    }
}

void LedOnOffSeg4G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_SEG_4G();
    }
    else
    {
        TURN_OFF_LED_SEG_4G();
    }
}

void LedOnOffStatusBarLow(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_BAR_LOW();
    }
    else
    {
        TURN_OFF_LED_STATUS_BAR_LOW();
    }
}

void LedOnOffStatusBarMiddle(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_BAR_MIDDLE();
    }
    else
    {
        TURN_OFF_LED_STATUS_BAR_MIDDLE();
    }
}

void LedOnOffStatusBarHigh(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_BAR_HIGH();
    }
    else
    {
        TURN_OFF_LED_STATUS_BAR_HIGH();
    }
}

void LedOnOffStatusBarInfinite(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_BAR_INFINITE();
    }
    else
    {
        TURN_OFF_LED_STATUS_BAR_INFINITE();
    }
}

void LedOnOffStatusUnitMl(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_UNIT_ML();
    }
    else
    {
        TURN_OFF_LED_STATUS_UNIT_ML();
    }
}

void LedOnOffWelcomeRightBlue(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_RIGHT_BLUE();
    }
    else
    {
        TURN_OFF_LED_WELCOME_RIGHT_BLUE();
    }
}

void LedOnOffWelcomeRightGreen(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_RIGHT_GREEN();
    }
    else
    {
        TURN_OFF_LED_WELCOME_RIGHT_GREEN();
    }
}

void LedOnOffWelcomeRightRed(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_RIGHT_RED();
    }
    else
    {
        TURN_OFF_LED_WELCOME_RIGHT_RED();
    }
}

void LedOnOffWelcomeRightWhite(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_RIGHT_WHITE();
    }
    else
    {
        TURN_OFF_LED_WELCOME_RIGHT_WHITE();
    }
}

void LedOnOffWelcomeLeftBlue(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_LEFT_BLUE();
    }
    else
    {
        TURN_OFF_LED_WELCOME_LEFT_BLUE();
    }
}

void LedOnOffWelcomeLeftGreen(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_LEFT_GREEN();
    }
    else
    {
        TURN_OFF_LED_WELCOME_LEFT_GREEN();
    }
}

void LedOnOffWelcomeLeftRed(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_LEFT_RED();
    }
    else
    {
        TURN_OFF_LED_WELCOME_LEFT_RED();
    }
}

void LedOnOffWelcomeLeftWhite(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_WELCOME_LEFT_WHITE();
    }
    else
    {
        TURN_OFF_LED_WELCOME_LEFT_WHITE();
    }
}

void LedOnOffAmount(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_AMOUNT();
    }
    else
    {
        TURN_OFF_LED_AMOUNT();
    }
}

void LedOnOffCold(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_COLD();
    }
    else
    {
        TURN_OFF_LED_COLD();
    }
}

void LedOnOffRoom(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ROOM();
    }
    else
    {
        TURN_OFF_LED_ROOM();
    }
}

void LedOnOffHot(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_HOT();
    }
    else
    {
        TURN_OFF_LED_HOT();
    }
}

void LedOnOffIceWaterAmount(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE_WATER_AMOUNT();
    }
    else
    {
        TURN_OFF_LED_ICE_WATER_AMOUNT();
    }
}

void LedOnOffIceWater(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE_WATER();
    }
    else
    {
        TURN_OFF_LED_ICE_WATER();
    }
}

void LedOnOffIce(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE();
    }
    else
    {
        TURN_OFF_LED_ICE();
    }
}

void LedOnOffStausCooling(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_COOLING();
    }
    else
    {
        TURN_OFF_LED_STATUS_COOLING();
    }
}

void LedOnOffStausLowWater(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_LOW_WATER();
    }
    else
    {
        TURN_OFF_LED_STATUS_LOW_WATER();
    }
}

void LedOnOffStausHeating(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_HEATING();
    }
    else
    {
        TURN_OFF_LED_STATUS_HEATING();
    }
}

void LedOnOffStausHotLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_HOT_LOCK();
    }
    else
    {
        TURN_OFF_LED_STATUS_HOT_LOCK();
    }
}

void LedOnOffStausIceWaterAmountLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_WATER_AMOUNT_LOCK();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_WATER_AMOUNT_LOCK();
    }
}

void LedOnOffStausIceWaterLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_WATER_LOCK();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_WATER_LOCK();
    }
}

void LedOnOffStausIceLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_STATUS_ICE_LOCK();
    }
    else
    {
        TURN_OFF_LED_STATUS_ICE_LOCK();
    }
}

void LedOnOffAllLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ALL_LOCK();
    }
    else
    {
        TURN_OFF_LED_ALL_LOCK();
    }
}

void LedOnOffColdOff(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_COLD_OFF();
    }
    else
    {
        TURN_OFF_LED_COLD_OFF();
    }
}

void LedOnOffHotOff(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_HOT_OFF();
    }
    else
    {
        TURN_OFF_LED_HOT_OFF();
    }
}

void LedOnOffHotLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_HOT_LOCK();
    }
    else
    {
        TURN_OFF_LED_HOT_LOCK();
    }
}

void LedOnOffIceOff(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE_OFF();
    }
    else
    {
        TURN_OFF_LED_ICE_OFF();
    }
}

void LedOnOffIceWaterLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE_WATER_LOCK();
    }
    else
    {
        TURN_OFF_LED_ICE_WATER_LOCK();
    }
}

void LedOnOffIceLock(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_ICE_LOCK();
    }
    else
    {
        TURN_OFF_LED_ICE_LOCK();
    }
}
