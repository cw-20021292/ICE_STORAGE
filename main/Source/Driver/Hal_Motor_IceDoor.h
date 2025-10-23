/// @file     Hal_Motor_IceDoor.h
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Door 모터 제어


#ifndef _HAL_MOTOR_ICE_DOOR_H_
#define _HAL_MOTOR_ICE_DOOR_H_


/*  Ice Door 모터 제어 Function 사용법
    => Set_IceDoorCW_Status(mu8Status);                 // Ice Door CW 회전 상태 설정 - 0(미동작), 1(동작)
    => Get_IceDoorCW_Status();                          // Ice Door CW 회전 상태 확인 - 0(미동작), 1(동작)
    => Set_IceDoorCloseStatus(mu8Status);               // Ice Door 닫힘 동작 상태 설정 - 0(미동작), 1(동작)
    => Get_IceDoorCloseStatus();                        // Ice Door 닫힘 동작 상태 확인 - 0(미동작), 1(동작)
    => Set_IceDoorStatus(mu8Status);                    // Ice Door 열림 상태 설정 - 0(닫힘), 1(열림)
    => Get_IceDoorStatus();                             // Ice Door 열림 상태 확인 - 0(닫힘), 1(열림)
*/


#define MOTOR_ICE_DOOR_USE                      USE

#if (MOTOR_ICE_DOOR_USE == USE)

#define ICE_DOOR_CONTROL_TIME_PERIOD            100         // Ice Door 제어 주기 100ms@1ms

#define P_STEP_MOTOR_ICE_DOOR_1                 P7_bit.no1
#define TURN_ON_MOTOR_ICE_DOOR_1()              { P_STEP_MOTOR_ICE_DOOR_1 = 1; }
#define TURN_OFF_MOTOR_ICE_DOOR_1()             { P_STEP_MOTOR_ICE_DOOR_1 = 0; }
#define GET_STATUS_MOTOR_ICE_DOOR_1()           ( P_STEP_MOTOR_ICE_DOOR_1 )

#define P_STEP_MOTOR_ICE_DOOR_2                 P7_bit.no2
#define TURN_ON_MOTOR_ICE_DOOR_2()              { P_STEP_MOTOR_ICE_DOOR_2 = 1; }
#define TURN_OFF_MOTOR_ICE_DOOR_2()             { P_STEP_MOTOR_ICE_DOOR_2 = 0; }
#define GET_STATUS_MOTOR_ICE_DOOR_2()           ( P_STEP_MOTOR_ICE_DOOR_2 )

#define P_STEP_MOTOR_ICE_DOOR_3                 P7_bit.no3
#define TURN_ON_MOTOR_ICE_DOOR_3()              { P_STEP_MOTOR_ICE_DOOR_3 = 1; }
#define TURN_OFF_MOTOR_ICE_DOOR_3()             { P_STEP_MOTOR_ICE_DOOR_3 = 0; }
#define GET_STATUS_MOTOR_ICE_DOOR_3()           ( P_STEP_MOTOR_ICE_DOOR_3 )

#define P_STEP_MOTOR_ICE_DOOR_4                 P7_bit.no4
#define TURN_ON_MOTOR_ICE_DOOR_4()              { P_STEP_MOTOR_ICE_DOOR_4 = 1; }
#define TURN_OFF_MOTOR_ICE_DOOR_4()             { P_STEP_MOTOR_ICE_DOOR_4 = 0; }
#define GET_STATUS_MOTOR_ICE_DOOR_4()           ( P_STEP_MOTOR_ICE_DOOR_4 )


/*..hui [20-3-26오후 7:24:07] ICE Door 1500->1000.. 아이스2.0 사양..*/
/*#define STEP_ANGLE2 1000*/

/*..hui [20-3-26오후 7:24:23] 아이스3.0은 이 값이 적정 사양..*/
/*#define STEP_ANGLE2 670*/


/*..hui [20-3-26오후 7:30:12] 50펄스 정도 더 주는걸로..*/
//#define STEP_ANGLE2 750
//#define STEP_ANGLE2 950
/*..sean [25-07-11] 도어 안닫힘 현상으로 100pulse 추가 ..*/
#define ICE_DOOR_STEPMOTOR_ANGLE_PULSE          1050


void Hal_IceDoor_Initialize(void);
void Set_IceDoorCW_Status(U8 mu8Status);
U8 Get_IceDoorCW_Status(void);
void IceDoorControlTimer(void);
void IceDoor_StepMotorInterrupt(void);
void motor_ice_door_output(void);
void Set_IceDoorCloseStatus(U8 mu8Status);
U8 Get_IceDoorCloseStatus(void);
void ice_door_close_24_hour(void);
void Hal_IceDoorControl(void);
void Set_IceDoorStatus(U8 mu8Status);
U8 Get_IceDoorStatus(void);

#endif

void Hal_IceDoor_Module_Initialize(void);
void Hal_IceDoor_Module_1ms_Control(void);
void Hal_IceDoor_Module_Control(void);

#endif
