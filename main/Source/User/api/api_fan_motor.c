/**
 * File : api_fan_motor.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "hw.h"

#include "api_fan_motor.h"
#include "comp.h"

#define FAN_STAY_TIME      300     // 5min @1sec

typedef struct _fan_
{
    U16 OnTime;
    U8 OnOff[FAN_TYPE_MAX];
} SFan_T;

SFan_T Fan;

void InitFanMotor(void)
{
    Fan.OnOff[FAN_TYPE_COLD] = OFF;
    Fan.OnOff[FAN_TYPE_ICE] = OFF;
    Fan.OnTime = 0U;

    TURN_OFF_MOTOR_FAN();
}

void TurnOnFan(EFanType_T type)
{
    Fan.OnOff[type] = ON;
}

void TurnOffFan(EFanType_T type)
{
    Fan.OnOff[type] = OFF;
}

void TurnOnOffFan(EFanType_T type, U8 onOff)
{
    Fan.OnOff[type] = onOff;
}

void ControlFanMotor(void)
{
    /// FAN OFF, COMP OFF
    /// -> RESET TIMER...
    if( (Fan.OnOff[FAN_TYPE_COLD] == OFF)
    && (Fan.OnOff[FAN_TYPE_ICE] == OFF)
    && (Fan.OnOff[FAN_TYPE_ICE_TANK] == OFF)
    && (GetCompOffTime() < 3)
    )
    {
        Fan.OnTime = FAN_STAY_TIME;
    }

    /// FAN CONTROL
    if( (Fan.OnOff[FAN_TYPE_COLD] == ON)
    || (Fan.OnOff[FAN_TYPE_ICE] == ON)
    || ((Fan.OnTime != 0U) && (IsTurnOnCompBldc() == FALSE))
    )
    {
        TURN_ON_MOTOR_FAN();
    }
    else
    {
        TURN_OFF_MOTOR_FAN();
    }

    // ICE TANK FAN Added
    if ((Fan.OnOff[FAN_TYPE_ICE_TANK] == ON)
    && (Fan.OnTime != 0U)
    )
    {
        TURN_ON_MOTOR_ICE_FAN();
    }
    else
    {
        TURN_OFF_MOTOR_ICE_FAN();
    }

    /// FAN TIMER...
    if( Fan.OnTime != 0U )
    {
        Fan.OnTime--;
    }
}
