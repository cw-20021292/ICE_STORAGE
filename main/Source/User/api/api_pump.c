/**
 * File : api_pump.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"
#include "api_pump.h"

#include "hal_timer.h"

#define MAX_DUTY_PERCENTAGE    (100)
#define MIN_DUTY_PERCENTAGE    (0)

/**
 * @brief             Function For Check Duty
 * @detail            Limit Max Duty or Min Duty
 * @param     duty    output Duty ( percentage )
 * @return            Checked Duty
**/
static U8 CheckDuty(U8 duty)
{
    U8 currentDuty = 0U;

#if 0
    duty = MAX_DUTY_PERCENTAGE - duty;
#endif
      
    if( duty >= MAX_DUTY_PERCENTAGE )
    {
        currentDuty = MAX_DUTY_PERCENTAGE;
    }
    else if( duty <= MIN_DUTY_PERCENTAGE )
    {
        currentDuty = MIN_DUTY_PERCENTAGE;
    }
    else
    {
        currentDuty = duty;
    }

    return currentDuty;
}

U8 IsTurnOnDrainPump(void)
{
    if( HAL_TIMER05_GET_TOE() != 0 )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsTurnOnIceDrainPump(void)
{
    if( HAL_TIMER13_GET_TOE() != 0 )
    {
        return TRUE;
    }

    return FALSE;
}


/**
 * @brief             Function For Convert Duty 2 Tdr
 * @detail            TAU0 / Channel4 (master), Channel5(slave) / Pwm / 10kHz
 * @detail            100% Tdr = TDR04(0x0C7F), 50% Tdr = 0640
 * @detail            TDR About Duty = ( (Duty) * (MasterTdr+1) / 100 )
 * @param     duty    output Duty ( percentage )
 * @return            none
*/
static U16 PumpDrainConvDuty2Tdr(U8 duty)
{
    U8  currentDuty = 0U;
    U16 tdr = 0U;

    currentDuty = CheckDuty(duty);

    tdr = (U16)( ((U32)currentDuty) * (TIMER04_TDR + 1) / (100) );

    return tdr;
}

/**
 * @brief             Function For Convert Duty 2 Tdr
 * @detail            TAU0 / Channel4 (master), Channel5(slave) / Pwm / 10kHz
 * @detail            100% Tdr = TDR04(0x0C7F), 50% Tdr = 0640
 * @detail            TDR About Duty = ( (Duty) * (MasterTdr+1) / 100 )
 * @param     duty    output Duty ( percentage )
 * @return            none
*/
static U16 PumpIceDrainConvDuty2Tdr(U8 duty)
{
    U8  currentDuty = 0U;
    U16 tdr = 0U;

    currentDuty = CheckDuty(duty);

    tdr = (U16)( ((U32)currentDuty) * (TIMER12_TDR + 1) / (100) );

    return tdr;
}

void TurnOnPump(SPumpId_T pumpId, U8 duty)
{
    if( pumpId == PUMP_ID_DRAIN )
    {
        HAL_TIMER04_05_START();
        HAL_TIMER05_SET_TDR(PumpDrainConvDuty2Tdr(duty));
    }
    else if( pumpId == PUMP_ID_CIRCULATE )
    {
        TURN_OFF_PUMP_CIRCULATE_TRIG();
        TURN_ON_PUMP_CIRCULATE();
    }
    else if( pumpId == PUMP_ID_ICE_DRAIN )
    {
        HAL_TIMER12_13_START();
        HAL_TIMER13_SET_TDR(PumpIceDrainConvDuty2Tdr(duty));
    }
    else if( pumpId == PUMP_ID_OPTION )
    {
        TURN_ON_PUMP_OPTION();
    }    
}

void TurnOffPump(SPumpId_T pumpId)
{
    if( pumpId == PUMP_ID_DRAIN )
    {
        HAL_TIMER04_05_STOP();
        HAL_TIMER05_SET_TDR(PumpDrainConvDuty2Tdr(0));
    }
    else if( pumpId == PUMP_ID_CIRCULATE )
    {
        TURN_OFF_PUMP_CIRCULATE_TRIG();
        TURN_OFF_PUMP_CIRCULATE();
    }
    else if( pumpId == PUMP_ID_ICE_DRAIN )
    {
        HAL_TIMER12_13_STOP();
        HAL_TIMER13_SET_TDR(PumpIceDrainConvDuty2Tdr(0));
    }
    else if( pumpId == PUMP_ID_OPTION )
    {
        TURN_OFF_PUMP_OPTION();
    }
}
