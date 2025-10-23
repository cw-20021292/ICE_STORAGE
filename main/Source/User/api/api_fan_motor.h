/**
 * File : api_fan_motor.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __API_FAN_MOTOR_H__
#define __API_FAN_MOTOR_H__

#include "prj_type.h"

typedef enum _fan_type_
{
    FAN_TYPE_ICE = 0,     /// 0 :
    FAN_TYPE_COLD,        /// 1 :
    FAN_TYPE_ICE_TANK,    /// 3 : ∫∏≥√øÎ æ∆¿ÃΩ∫≈ ≈© ≥ª∫Œ ∆“
    FAN_TYPE_MAX,         /// 2 :
} EFanType_T;

void InitFanMotor(void);
void TurnOnFan(EFanType_T type);
void TurnOffFan(EFanType_T type);
void TurnOnOffFan(EFanType_T type, U8 onOff);

void ControlFanMotor(void);

#endif /* __API_FAN_MOTOR_H__ */