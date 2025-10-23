/**
 * File : display_eol.c
 * 
 * Led Display Eol
*/
#include "hw.h"
#include "led.h"

#include "display_eol.h"
#include "display.h"
#include "water_out.h"
#include "service.h"
#include "eol.h"
#include "temp.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"
#include "err_temp.h"
#include "api_adc.h"

#define INTERVAL_CHECK_SEGMENT    (4)
#define INTERVAL_CHECK_SUBSEGMENT (2)
static void EolSubFrontCheckIce(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);
                OnOffLed(LED_ID_ICE, OFF);
            }
            else
            {
                OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckIceWater(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_ICE_WATER, OFF);
            }
            else
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckIceWaterAmount(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);
                OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
            }
            else
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckHot(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);
                OnOffLed(LED_ID_HOT, OFF);
            }
            else
            {
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckCold(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_COLD, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_COLD, OFF);
                OnOffLed(LED_ID_STATUS_COOLING, OFF);
            }
            else
            {
                OnOffLed(LED_ID_COLD, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckIceLock(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
            }
            break;

        case 4:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
            }
            break;

        case 5:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
            }
            break;

        case 6:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
                OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
            }
            break;

        case 7:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
                OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);
                OnOffLed(LED_ID_STATUS_WATER_PREPARING, OFF);
            }
            else
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
                OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckIceWaterLock(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
            }
            break;

        case 4:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
            }
            break;

        case 5:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
            }
            break;

        case 6:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
                OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
            }
            break;

        case 7:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
                OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
            }
            else
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
                OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckHotLock(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
            }
            break;

        case 4:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
            }
            break;

        case 5:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
                OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
            }
            break;

        case 6:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
                OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
                OnOffLed(LED_ID_UV_WATER_FAUCET, OFF);
            }
            else
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
                OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolFrontCheckSegment(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SEGMENT;    /// 400ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
            }
            break;

        case 4:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
            }
            break;

        case 5:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
            }
            break;

        case 6:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);

                interval = INTERVAL_CHECK_SEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
            }
            break;

        case 7:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);
                OnOffLed(LED_ID_SEG_2G, OFF);
                OnOffLed(LED_ID_SEG_3G, OFF);
                OnOffLed(LED_ID_SEG_4G, OFF);
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SEGMENT;
            checkStep = 0U;
            break;
    }   
}

static void EolSubFrontCheckSegment(void)
{
    static U8 checkStep = 0U;
    static U8 interval = 0U;

    if( interval != 0 )
    {
        interval--;
    }

    switch(checkStep)
    {
        case 0:
            interval = INTERVAL_CHECK_SUBSEGMENT;    /// 200ms
            checkStep++;
            break;

        case 1:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            break;

        case 2:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
            }
            break;

        case 3:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
            }
            break;

        case 4:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
            }
            break;

        case 5:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
            }
            break;

        case 6:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);

                interval = INTERVAL_CHECK_SUBSEGMENT;
                checkStep++;
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
            }
            break;

        case 7:
            if( interval == 0 )
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);
                OnOffLed(LED_ID_SEG_2G, OFF);
                OnOffLed(LED_ID_SEG_3G, OFF);
                OnOffLed(LED_ID_SEG_4G, OFF);
            }
            else
            {
                OnOffLed(LED_ID_SEG_2F, OFF);
                OnOffLed(LED_ID_SEG_3F, OFF);
                OnOffLed(LED_ID_SEG_4F, OFF);
                OnOffLed(LED_ID_SEG_2A, OFF);
                OnOffLed(LED_ID_SEG_3A, OFF);
                OnOffLed(LED_ID_SEG_4A, OFF);
                OnOffLed(LED_ID_SEG_1A, OFF);
                OnOffLed(LED_ID_SEG_2B, OFF);
                OnOffLed(LED_ID_SEG_3B, OFF);
                OnOffLed(LED_ID_SEG_4B, OFF);
                OnOffLed(LED_ID_SEG_1B, OFF);
                OnOffLed(LED_ID_SEG_2C, OFF);
                OnOffLed(LED_ID_SEG_3C, OFF);
                OnOffLed(LED_ID_SEG_4C, OFF);
                OnOffLed(LED_ID_SEG_2D, OFF);
                OnOffLed(LED_ID_SEG_3D, OFF);
                OnOffLed(LED_ID_SEG_4D, OFF);
                OnOffLed(LED_ID_SEG_2E, OFF);
                OnOffLed(LED_ID_SEG_3E, OFF);
                OnOffLed(LED_ID_SEG_4E, OFF);
            }
            break;

        default:
            interval = INTERVAL_CHECK_SUBSEGMENT;
            checkStep = 0U;
            break;
    }   
}

// EOL - FRONT check button..
static void EolFrontCheckInput(void)
{
    static U8 blink = 10;

    U32 val = 0UL;

    U8 valCntIce = 0;
    U8 valCntAmount = 0;

    val = GetEolTestInputVal();
    valCntIce = GetEolTestInputCnt(EOL_ID_KEY_ICE);
    valCntAmount = GetEolTestInputCnt(EOL_ID_KEY_AMOUNT);
    
    TurnOffAllLED();

    if( (GetEolTestInputVal() & MK_EOL_RBK_FRONT_LAST) == MK_EOL_RBK_FRONT_LAST
         && (GetEolTestInputCnt(EOL_ID_KEY_ICE) == 5)
         && (GetEolTestInputCnt(EOL_ID_KEY_AMOUNT) == 5)
    )
    {
        if( blink < 5 )
        {
            TurnOnAllLED();
        }
        else
        {
            TurnOffAllLED();
        }
    }
    else
    {
        if( (val & MK_EOL_LEVER_WATER ) != 0 )
        {
            TurnOnAllLED();

            if( (val & MK_EOL_KEY_ICE ) != 0 || (val & MK_EOL_KEY_ICE ) != 1 )
            {
                switch(valCntIce)
                {           
                    case 1 :
                        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, ON);
                        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
                        break;

                    case 2 :
                        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, ON);
                        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
                        break;

                    case 3 :
                        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_RED, ON);
                        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
                        break;

                    case 4 :
                        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, ON);
                        break;

                    case 5 :
                        OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
                        OnOffLed(LED_ID_ICE, OFF);
                        OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);
                        break;

                    default :
                        break;
                }                
            }

            if( (val & MK_EOL_KEY_ICE_WATER ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);
            }

            if( (val & MK_EOL_KEY_ICE_WATER_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);
            }

            if( (val & MK_EOL_KEY_HOT ) != 0 )
            {
                OnOffLed(LED_ID_HOT, OFF);
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);
            }

            if( (val & MK_EOL_KEY_ROOM ) != 0 )
            {
                OnOffLed(LED_ID_ROOM, OFF);
            }

            if( (val & MK_EOL_KEY_COLD ) != 0 )
            {
                OnOffLed(LED_ID_COLD, OFF);
                OnOffLed(LED_ID_STATUS_COOLING, OFF);
            }

            if( (val & MK_EOL_KEY_AMOUNT ) != 0 || (val & MK_EOL_KEY_AMOUNT ) != 1 )
            {
                switch(valCntAmount)
                {
                    case 1 :
                        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, ON);
                        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
                        break;

                    case 2 :
                        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, ON);
                        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
                        break;

                    case 3 :
                        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_RED, ON);
                        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
                        break;

                    case 4 :
                        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, ON);
                        break;

                    case 5 :
                        OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
                        OnOffLed(LED_ID_AMOUNT, OFF);
                        break;

                    default :
                        break;
                } 
                
            }

            if( (val & MK_EOL_KEY_ICE_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
                OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);
                OnOffLed(LED_ID_ICE_LOCK, OFF);
            }

            if( (val & MK_EOL_KEY_ICE_WATER_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_STATUS_WATER_PREPARING, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
                OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
            }

            if( (val & MK_EOL_KEY_HOT_LOCK ) != 0 )
            {
                //AllOffSegment();
                EolFrontCheckSegment();
                OnOffLed(LED_ID_HOT_LOCK, OFF);
            }

            if( (val & MK_EOL_LEVER_ICE ) != 0 )
            {
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
                OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
                OnOffLed(LED_ID_UV_WATER_FAUCET, OFF);
            }
        }
        else
        {
            ClearEolTestInputCnt();
            ClearEolTestInputBit(MK_EOL_KEY_ICE);
            ClearEolTestInputBit(MK_EOL_KEY_HOT);
            ClearEolTestInputBit(MK_EOL_KEY_ROOM);
            ClearEolTestInputBit(MK_EOL_KEY_COLD);
            ClearEolTestInputBit(MK_EOL_KEY_AMOUNT);
            ClearEolTestInputBit(MK_EOL_KEY_ICE_WATER);
            ClearEolTestInputBit(MK_EOL_KEY_ICE_WATER_AMOUNT);
            ClearEolTestInputBit(MK_EOL_KEY_ICE_LOCK);
            ClearEolTestInputBit(MK_EOL_KEY_ICE_WATER_LOCK);
            ClearEolTestInputBit(MK_EOL_KEY_HOT_LOCK);
            ClearEolTestInputBit(MK_EOL_LEVER_ICE);
        }
    }

    if( blink != 0 )
    {
        blink--;
    }
    else
    {
        blink = 10;
    }
}

static U8 EolCheckInputError(void)
{
    U16 tempAdc = 0U;
 
    /// 수위센서 
    if( GET_STATUS_LEVEL_ROOM_TANK_LOW() == HIGH  ) /// 정수 저수위 감지
    {
        /// Error
        DispSegStr("ELP");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_ROOM_TANK_HIGH() == HIGH  ) /// 정수 만수위 감지
    {
        /// Error
        DispSegStr("EHP");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_COLD_TANK_LOW() == HIGH  ) /// 냉수 저수위 감지
    {
        /// Error
        DispSegStr("ELC");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_COLD_TANK_HIGH() == HIGH  ) /// 냉수 만수위 감지
    {
        /// Error
        DispSegStr("EHC");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_DRAIN_TANK_LOW() == HIGH  ) /// 드레인 저수위 감지
    {
        /// Error
        DispSegStr("ELd");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_DRAIN_TANK_HIGH() == HIGH  ) /// 드레인 만수위 감지
    {
        /// Error
        DispSegStr("EHd");
        return TRUE;
    }

    if( GET_STATUS_LEVEL_ROOM_TANK_OVERFLOW() == HIGH  ) /// 정수 오플러우 감지
    {
        /// Error
        DispSegStr("EoF");
        return TRUE;
    }


    /// 온도센서
    tempAdc = GetAdcData(ADC_ID_TEMP_COLD_WATER);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("ECT");
        return TRUE;
    }

    tempAdc = GetAdcData(ADC_ID_TEMP_HOT_WATER);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("EHT");
        return TRUE;
    }

    tempAdc = GetAdcData(ADC_ID_TEMP_AMBIENT);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("EoT");
        return TRUE;
    }

    tempAdc = GetAdcData(ADC_ID_TEMP_AMBIENT_2);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("EoT");
        return TRUE;
    }

#if 0
    tempAdc = GetAdcData(ADC_ID_TEMP_EVA_1);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("EAT");
        return TRUE;
    }
#endif

    tempAdc = GetAdcData(ADC_ID_TEMP_EVA_2);
    if( GetTempErrorType(tempAdc) != STATUS_NORMAL )
    {
        /// Error
        DispSegStr("EAT");
        return TRUE;
    }

    /// 탱크 커버 리드 스위치
    if( GET_STATUS_REED_TANK_OPEN() == HIGH )   // 탱크 커버 미감지
    {
        DispSegStr("ET1");
        return TRUE;
    }
   
    return FALSE;
}

static void EolMainFrontCheckInput(void)
{
    static U8 mu8Blink = 10;
    U32 mu32Val = 0UL;
    static U8 delay = 20U;

    mu32Val = GetEolTestInputVal();

    TurnOffAllLED();

    if( EolCheckInputError() == TRUE )
    {
        return;
    }

    if( ((GetEolTestInputVal() & MK_EOL_LOAD_MAIN_FRONT_LAST) == MK_EOL_LOAD_MAIN_FRONT_LAST)
         && (delay == 0U) )
    {
        if( mu8Blink < 5 )
        {
            TurnOnAllLED();
        }
        else
        {
            TurnOffAllLED();
        }
    }
    else
    {
        if( (mu32Val & MK_EOL_LEVER_WATER ) != 0 )
        {
            TurnOnAllLED();

            if( (mu32Val & MK_EOL_KEY_ICE ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_RED, ON);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF); 
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                OnOffLed(LED_ID_ICE, OFF);
                OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);
            }
            
            if( (mu32Val & MK_EOL_KEY_ICE_WATER ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, ON); 
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                OnOffLed(LED_ID_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_ICE_WATER_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF); 
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, ON);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_HOT ) != 0 )
            {
                OnOffLed(LED_ID_HOT, OFF);
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_ROOM ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            
                OnOffLed(LED_ID_ROOM, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_COLD ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            
                OnOffLed(LED_ID_COLD, OFF);
                OnOffLed(LED_ID_STATUS_COOLING, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_AMOUNT, OFF);
                
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_ICE_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_MAX, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
                OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);
                OnOffLed(LED_ID_STATUS_WATER_PREPARING, OFF);   
            }

            if( (mu32Val & MK_EOL_KEY_ICE_WATER_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
                OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
                OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
                OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
                OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
                OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_HOT_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
                OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
                OnOffLed(LED_ID_UV_ICE_TANK, OFF);
                OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
                OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
                OnOffLed(LED_ID_UV_WATER_FAUCET, OFF);
            }

            if( (mu32Val & MK_EOL_LEVER_ICE ) != 0 )
            {
                AllOffSegment();

                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF); 
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);

                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);

                if( delay != 0U )
                {
                    delay--;
                }
            }
        }
    }

    if( mu8Blink != 0 )
    {
        mu8Blink--;
    }
    else
    {
        mu8Blink = 10;
    }

}

static void EolSubFrontCheckInput(void)
{
    static U8 blink = 10U;
    static U8 delay = 20U;

    U32 val = 0UL;

    U8 valCntIce = 0;
    U8 valCntAmount = 0;

    val = GetEolTestInputVal();
    
    TurnOffAllLED();

    if( ((GetEolTestInputVal() & MK_EOL_LOAD_SUB_FRONT_LAST) == MK_EOL_LOAD_SUB_FRONT_LAST)
         && (delay == 0U) ) 
    {
        if( blink < 5 )
        {
            TurnOnAllLED();
        }
        else
        {
            TurnOffAllLED();
        }
    }
    else
    {
        if( (val & MK_EOL_LEVER_WATER ) != 0 )
        {
            TurnOnAllLED();

            if( (val & MK_EOL_KEY_ICE ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_RED, ON);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                EolSubFrontCheckIce();
            }

            if( (val & MK_EOL_KEY_ICE_WATER ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, ON);
                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                EolSubFrontCheckIceWater();
            }

            if( (val & MK_EOL_KEY_ICE_WATER_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, ON);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
            
                EolSubFrontCheckIceWaterAmount();
            }

            if( (val & MK_EOL_KEY_HOT ) != 0 )
            {
                EolSubFrontCheckHot();
            }

            if( (val & MK_EOL_KEY_ROOM ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            
                OnOffLed(LED_ID_ROOM, OFF);
            }

            if( (val & MK_EOL_KEY_COLD ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            
                EolSubFrontCheckCold();
            }

            if( (val & MK_EOL_KEY_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, ON);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
            
                OnOffLed(LED_ID_AMOUNT, OFF);
            }

            if( (val & MK_EOL_KEY_ICE_LOCK ) != 0 )
            {
                EolSubFrontCheckIceLock();
            }

            if( (val & MK_EOL_KEY_ICE_WATER_LOCK ) != 0 )
            {
                EolSubFrontCheckIceWaterLock();
            }

            if( (val & MK_EOL_KEY_HOT_LOCK ) != 0 )
            {
                //AllOffSegment();
                //EolFrontCheckSegment();
                EolSubFrontCheckHotLock();
            }

            if( (val & MK_EOL_LEVER_ICE ) != 0 )
            {
                EolSubFrontCheckSegment();

                OnOffLed(LED_ID_WELCOME_LEFT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);

                OnOffLed(LED_ID_WELCOME_RIGHT_BLUE, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_GREEN, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_RED, OFF);
                OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);

                if( delay != 0U )
                {
                    delay--;
                }
            }
        }
    }

    if( blink != 0 )
    {
        blink--;
    }
    else
    {
        blink = 10;
    }
}

/// LOAD TEST, LED 표시
static void EolCheckInput(void)
{
    static U8 mu8Blink = 10;
    U32 mu32Val = 0UL;

    mu32Val = GetEolTestInputVal();

    TurnOffAllLED();

    if( (mu32Val & MK_EOL_TANK_BRACKET) != 0 )
    {
        /// 감지(점등) 조건 만족 시 점등 유지
        OnOffLed(LED_ID_STATUS_MAX, ON);
        OnOffLed(LED_ID_STATUS_ICE_WATER, ON);
        OnOffLed(LED_ID_STATUS_WATER, ON);
        OnOffLed(LED_ID_STATUS_ICE_FULL, ON);
        OnOffLed(LED_ID_STATUS_ICE_PREPARING, ON);
        OnOffLed(LED_ID_STATUS_WATER_PREPARING, ON);
        OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, ON);
        OnOffLed(LED_ID_STATUS_UNIT_ML, ON);
        OnOffLed(LED_ID_STATUS_BAR_LOW, ON);
        OnOffLed(LED_ID_STATUS_BAR_MIDDLE, ON);
        OnOffLed(LED_ID_STATUS_BAR_HIGH, ON);
        OnOffLed(LED_ID_STATUS_BAR_INFINITE, ON);
        AllOnSegment();
    }
    else
    {
        OnOffLed(LED_ID_STATUS_MAX, OFF);
        OnOffLed(LED_ID_STATUS_ICE_WATER, OFF);
        OnOffLed(LED_ID_STATUS_WATER, OFF);
        OnOffLed(LED_ID_STATUS_ICE_FULL, OFF);
        OnOffLed(LED_ID_STATUS_ICE_PREPARING, OFF);
        OnOffLed(LED_ID_STATUS_WATER_PREPARING, OFF);
        OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
        OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
        OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
        OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
        OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
        OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
        AllOffSegment();
    }

    if( (mu32Val & MK_EOL_TANK_COVER) != 0 )
    {
        /// 미감지(점멸) -> 감지(점등) 조건 만족 시 점등 유지
        OnOffLed(LED_ID_UV_CARE_SYSTEM, ON);
        OnOffLed(LED_ID_UV_ICE_TANK, ON);
        OnOffLed(LED_ID_UV_ICE_TRAY, ON);
        OnOffLed(LED_ID_UV_ICE_FAUCET, ON);
        OnOffLed(LED_ID_UV_WATER_FAUCET, ON);
    }
    else
    {
        /// 미판단, 감지(점등)->미감지(점멸) 조건 만속 시 점멸 유지
        if( mu8Blink < 5 )
        {
            OnOffLed(LED_ID_UV_CARE_SYSTEM, ON);
            OnOffLed(LED_ID_UV_ICE_TANK, ON);
            OnOffLed(LED_ID_UV_ICE_TRAY, ON);
            OnOffLed(LED_ID_UV_ICE_FAUCET, ON);
            OnOffLed(LED_ID_UV_WATER_FAUCET, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
            OnOffLed(LED_ID_UV_ICE_TANK, OFF);
            OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
            OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
            OnOffLed(LED_ID_UV_WATER_FAUCET, OFF);
        }
        
    }

    if( (mu32Val & MK_EOL_LEVER_WATER ) != 0 )
    {
        if( ((mu32Val & MK_EOL_TANK_BRACKET) != 0)
            && ((mu32Val & MK_EOL_TANK_COVER) != 0)
        )
        {
            TurnOnAllLED();

            if( (mu32Val & MK_EOL_KEY_ICE ) != 0 )
            {
                OnOffLed(LED_ID_ICE, OFF);
                OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);

                if( (mu32Val & MK_EOL_KEY_ICE_DONE) != 0 )
                {
                    OnOffLed(LED_ID_ICE, ON);
                }
                else
                {
                    OnOffLed(LED_ID_ICE, OFF);
                }
            }           
            
            if( (mu32Val & MK_EOL_KEY_ICE_WATER ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_LOCK, OFF);

                if( (mu32Val & MK_EOL_KEY_ICE_WATER_DONE) != 0 )
                {
                    OnOffLed(LED_ID_ICE_WATER, ON);
                }
                else
                {
                    OnOffLed(LED_ID_ICE_WATER, OFF);
                }
            }

            if( (mu32Val & MK_EOL_KEY_ICE_WATER_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
                OnOffLed(LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_HOT ) != 0 )
            {
                OnOffLed(LED_ID_HOT, OFF);
                OnOffLed(LED_ID_STATUS_HOT_LOCK, OFF);
                OnOffLed(LED_ID_STATUS_HEATING, OFF);

                if( (mu32Val & MK_EOL_KEY_HOT_OUT_END ) != 0 )
                {
                    OnOffLed(LED_ID_HOT, ON);
                }
                else
                {
                    if( (mu32Val & MK_EOL_KEY_HOT_DP ) != 0 )
                    {
                        if( mu8Blink < 5 )
                        {
                            OnOffLed(LED_ID_HOT, ON);
                        }
                        else
                        {
                            OnOffLed(LED_ID_HOT, OFF);
                        }
                    }
                }
            }

            if( (mu32Val & MK_EOL_KEY_ROOM ) != 0 )
            {
                OnOffLed(LED_ID_ROOM, OFF);

                if( (mu32Val & MK_EOL_KEY_ROOM_OUT_END ) != 0 )
                {
                    OnOffLed(LED_ID_ROOM, ON);
                }
                else
                {
                    if( (mu32Val & MK_EOL_KEY_ROOM_DP ) != 0 )
                    {
                        if( mu8Blink < 5 )
                        {
                            OnOffLed(LED_ID_ROOM, ON);
                        }
                        else
                        {
                            OnOffLed(LED_ID_ROOM, OFF);
                        }
                    }
                }
            }

            if( (mu32Val & MK_EOL_KEY_COLD ) != 0 )
            {
                OnOffLed(LED_ID_COLD, OFF);
                OnOffLed(LED_ID_STATUS_COOLING, OFF);

                if( (mu32Val & MK_EOL_KEY_COLD_OUT_END ) != 0 )
                {
                    OnOffLed(LED_ID_COLD, ON);
                }
                else
                {
                    if( (mu32Val & MK_EOL_KEY_COLD_DP ) != 0 )
                    {
                        if( mu8Blink < 5 )
                        {
                            OnOffLed(LED_ID_COLD, ON);
                        }
                        else
                        {
                            OnOffLed(LED_ID_COLD, OFF);
                        }
                    }
                }
            }

            if( (mu32Val & MK_EOL_KEY_AMOUNT ) != 0 )
            {
                OnOffLed(LED_ID_AMOUNT, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_ICE_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_ICE_LOCK, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_ICE_WATER_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_ICE_WATER_LOCK, OFF);
            }

            if( (mu32Val & MK_EOL_KEY_HOT_LOCK ) != 0 )
            {
                OnOffLed(LED_ID_HOT_LOCK, OFF);
            }
        }  
    }

    if( mu8Blink != 0 )
    {
        mu8Blink--;
    }
    else
    {
        mu8Blink = 10;
    }
}

static void EolCheckInputDone(void)
{
    static U8 mu8Blink = 10;
    U32 mu32Val = 0UL;

    mu32Val = GetEolTestInputVal();

    TurnOffAllLED();

    if( GetEolCheckStatus( EOL_CHECK_ID_FLOW_METER_CUR ) != 0 )
    {
        DispSegStr("1o");

        if( (mu32Val & MK_EOL_KEY_ICE_DONE) != 0 )
        {
            SetEolTestInputBit(MK_EOL_KEY_ICE_TRAY_FLOW);
        }
        else if( (mu32Val & MK_EOL_KEY_ICE_WATER_DONE) != 0 )
        {
            SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_FLOW);
        }
    }
    else
    {
        DispSegStr("T1");
    }

    if( (mu32Val & MK_EOL_KEY_ICE_DONE) != 0 )
    {
        OnOffLed(LED_ID_ICE, ON);
    }
    else
    {
        OnOffLed(LED_ID_ICE, OFF);
    }

    if( (mu32Val & MK_EOL_KEY_ICE_WATER_DONE) != 0 )
    {
        OnOffLed(LED_ID_ICE_WATER, ON);
    }
    else
    {
        OnOffLed(LED_ID_ICE_WATER, OFF);
    }

    if( (mu32Val & MK_EOL_KEY_ICE_WATER_AMOUNT_DONE) != 0 )
    {
        if( mu8Blink < 5 )
        {
            OnOffLed(LED_ID_ICE_WATER_AMOUNT, ON);
        }
        else
        {
            OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
        }
    }
    
    if( mu8Blink != 0 )
    {
        mu8Blink--;
    }
    else
    {
        mu8Blink = 10;
    }
}

/// FRONT 검사 DISPLAY
/// 1. 키 버튼 입력,
/// 2. 조도 센서 감지
/// 3. 서비스 점검 
static void FrontTest(void)
{
    // Button 
    EolFrontCheckInput();
}

static void MainFrontTest(void)
{
    // Button 
    EolMainFrontCheckInput();
}

static void SubFrontTest(void)
{
    // Button 
    EolSubFrontCheckInput();
}

// EOL - LOAD TEST 모드
extern U8 EOL_Comp;
static void LoadTest(void)
{
    static U8 mu8Blink = 10;
    U8 mu8Mode;
    U8 mu8SelWater = SEL_WATER_ROOM;

    TEMP_T coil_cold = GetTemp(TEMP_ID_EVA_1);
    TEMP_T coil_ice = GetTemp(TEMP_ID_EVA_2);

    // 만약 버튼 EOL TEST FLAG가 SET되어 있으면,
    // EOL 통신 상태만 표시 하기위해 모든 LED를 다시 OFF한다.
    mu8Mode = GetEolMode();
    if( mu8Mode != EOL_MODE_READY 
            && mu8Mode != EOL_MODE_DONE )
    {
        U32 mu32Val = 0UL;

        mu32Val = GetEolTestInputVal();
    
        TurnOffAllLED();

        OnOffLed(LED_ID_ICE, ON);
        OnOffLed(LED_ID_STATUS_ICE_LOCK, ON);

        if( mu8Mode == EOL_MODE_SEALED_INIT 
            || mu8Mode == EOL_MODE_SEALED )
        {
            OnOffLed(LED_ID_ICE, OFF);
            OnOffLed(LED_ID_STATUS_ICE_LOCK, OFF);
        
            if( EOL_Comp == ON )
            {
                if( mu8Blink < 5 )
                {
                    OnOffLed(LED_ID_AMOUNT, ON);
                }
                else
                {
                    OnOffLed(LED_ID_AMOUNT, OFF);
                }
            }
        
            if( IsGasSwitchMoving() == TRUE
                 || IsHotGasSwitchMoving() == TRUE )
            {
                if( mu8Blink < 5 )
                {
                    OnOffLed(LED_ID_ICE, ON);
                    OnOffLed(LED_ID_HOT, ON);
                    OnOffLed(LED_ID_COLD, ON);
                }
                else
                {
                    OnOffLed(LED_ID_ICE, OFF);
                    OnOffLed(LED_ID_HOT, OFF);
                    OnOffLed(LED_ID_COLD, OFF);
                }
            }
            else
            {
                if( (GetGasSwitchStatus() == GAS_SWITCH_ICE)
                     && (GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL)
                )
                {
                    /// 제빙 위치
                    OnOffLed(LED_ID_ICE, ON);
                    DispSegStrDigit("E", (U8)coil_ice);
                    
                }
                else if( (GetGasSwitchStatus() == GAS_SWITCH_ICE)
                     && (GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT)      
                )
                {
                    /// 탈빙 위치
                    OnOffLed(LED_ID_HOT, ON);
                    DispSegStrDigit("H", (U8)coil_ice);
                }
                else if( (GetGasSwitchStatus() == GAS_SWITCH_COLD)
                     && (GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL)      
                )
                {
                    /// 냉각 위치
                    OnOffLed(LED_ID_COLD, ON);
                    DispSegStrDigit("C", (U8)coil_cold);
                }
            }

            if( (mu32Val & MK_EOL_KEY_ICE_WATER_AMOUNT_DONE) != 0 )
            {
                if( mu8Blink < 5 )
                {
                    OnOffLed(LED_ID_ICE_WATER_AMOUNT, ON);
                }
                else
                {
                    OnOffLed(LED_ID_ICE_WATER_AMOUNT, OFF);
                }
            }
        }
    }
    else
    {
        if( mu8Mode == EOL_MODE_READY )
        {
            EolCheckInput();
        }
        else if( mu8Mode == EOL_MODE_DONE )
        {
            EolCheckInputDone();
        }        
    }

    if( mu8Blink != 0 )
    {
        mu8Blink--;
    }
    else
    {
        mu8Blink = 10;
    }
}

void ProcessDisplayEol(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        /// FRONT 검사 모드
        FrontTest();
    }
    else if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        /// FRONT 검사 모드
        MainFrontTest();
    }
    else if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        /// FRONT 검사 모드
        SubFrontTest();
    }
    else
    {
        /// LOAD 검사 모드
        LoadTest();
    }
}
