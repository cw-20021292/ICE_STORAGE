/**
 * File : api_step_motor.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __API_STEP_MOTOR_H__
#define __API_STEP_MOTOR_H__

#include "prj_type.h"

#define	SM_ID_0			(0)   /// GAS SWITCH
#define	SM_ID_1			(1)   /// ICE DOOR
#define	SM_ID_2			(2)   /// HOTGAS SWITCH
#define	SM_ID_MAX		(3)

/* @at 2ms, */
#define	SM_SPEED_0	(0)	  /// 1ms = 1000pps
#define	SM_SPEED_1	(1)	  /// 30ms = 33pps
#define	SM_SPEED_2	(2)   /// 3ms = 333pps

typedef	short int 	sm_step_t;
typedef	short int 	sm_speed_t;

void InitStepMotor(void);
void InitStepVal(U8 _id);

void SetInActive(U8 _id, U8 _in_active);
U8 GetInActive(U8 _id);

void SetTargetStep(U8 _id, sm_step_t step);
sm_step_t GetTargetStep(U8 _id);

void SetCurrentStep(U8 _id, sm_step_t step);
sm_step_t GetCurrentStep(U8 _id);

void SetHoldStep(U8 _id, sm_step_t step);

void SetStepSpeed(U8 _id, sm_speed_t	speed);
sm_speed_t GetStepSpeed(U8 _id);

void  SetStepSpeedCount(U8 _id, U8 _cnt);
U8 GetStepSpeedCount(U8 _id);
void DecStepSpeedCount(U8 _id);

U8 IsDoneMoveStep(U8 _id);
void StopMove(U8 id);
U8 SetTargetStepDone(U8 id, sm_step_t step);

void MoveStep(U8 _id);
void ControlStepMotor(U8	_id);

#endif /* __API_STEP_MOTOR_H__ */
