/// @file     Hal_Motor_IceTray.h
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Tray 모터 제어


#ifndef _HAL_MOTOR_ICE_TRAY_H_
#define _HAL_MOTOR_ICE_TRAY_H_


/*  Ice Tray 모터 제어 Function 사용법
    => Set_IceTrayMode(mu8Mode);                // Ice Tray 이동 모드 설정 - 0(Stop), 1(Init), 2(Up), 3(Down)
    => Get_IceTrayMode();                       // Ice Tray 이동 모드 확인 - 0(Stop), 1(Init), 2(Up), 3(Down)
    => Set_IceTrayValveOut(mu8Status);          // Ice Tray 입수 밸브 동작 상태 설정 - 0(미동작), 1(동작)
    => Get_IceTrayValveOut();                   // Ice Tray 입수 밸브 동작 상태 확인 - 0(미동작), 1(동작)
    => Get_IceTrayInBlocked();                  // Ice Tray 입수 단수 여부 확인 - 0(단수 미발생), 1(단수 발생)
*/


#define MOTOR_ICE_TRAY_USE                      USE

#if (MOTOR_ICE_TRAY_USE == USE)

#define ICE_TRAY_CONTROL_TIME_PERIOD            100         // Ice Tray 제어 주기 100ms@1ms

typedef enum
{
    STEP_STATE_DEACTIVE = 0,
    STEP_STATE_ACTIVE,
} EIceTrayStep_State_T;

typedef enum
{
    STEP_DIRECTION_CW = 0,
    STEP_DIRECTION_CCW,
} EIceTrayStep_Direction_T;

typedef struct
{
    U16 gu16TargetPulse;
    U16 gu16NowPulse;
    U16 gu16count;
    EIceTrayStep_Direction_T Direction;
    EIceTrayStep_State_T state;
} SIceTrayStepmotor_T;


#define P_STEP_MOTOR_ICE_TRAY_1                 P2_bit.no1
#define TURN_ON_MOTOR_ICE_TRAY_1()              { P_STEP_MOTOR_ICE_TRAY_1 = 1; }
#define TURN_OFF_MOTOR_ICE_TRAY_1()             { P_STEP_MOTOR_ICE_TRAY_1 = 0; }
#define GET_STATUS_MOTOR_ICE_TRAY_1()           ( P_STEP_MOTOR_ICE_TRAY_1 )

#define P_STEP_MOTOR_ICE_TRAY_2                 P2_bit.no2
#define TURN_ON_MOTOR_ICE_TRAY_2()              { P_STEP_MOTOR_ICE_TRAY_2 = 1; }
#define TURN_OFF_MOTOR_ICE_TRAY_2()             { P_STEP_MOTOR_ICE_TRAY_2 = 0; }
#define GET_STATUS_MOTOR_ICE_TRAY_2()           ( P_STEP_MOTOR_ICE_TRAY_2 )

#define P_STEP_MOTOR_ICE_TRAY_3                 P2_bit.no3
#define TURN_ON_MOTOR_ICE_TRAY_3()              { P_STEP_MOTOR_ICE_TRAY_3 = 1; }
#define TURN_OFF_MOTOR_ICE_TRAY_3()             { P_STEP_MOTOR_ICE_TRAY_3 = 0; }
#define GET_STATUS_MOTOR_ICE_TRAY_3()           ( P_STEP_MOTOR_ICE_TRAY_3 )

#define P_STEP_MOTOR_ICE_TRAY_4                 P2_bit.no4
#define TURN_ON_MOTOR_ICE_TRAY_4()              { P_STEP_MOTOR_ICE_TRAY_4 = 1; }
#define TURN_OFF_MOTOR_ICE_TRAY_4()             { P_STEP_MOTOR_ICE_TRAY_4 = 0; }
#define GET_STATUS_MOTOR_ICE_TRAY_4()           ( P_STEP_MOTOR_ICE_TRAY_4 )


#define ICE_TRAY_MODE_NONE                      0
#define ICE_TRAY_MODE_INIT                      1
#define ICE_TRAY_MODE_UP                        2
#define ICE_TRAY_MODE_DOWN                      3

#define ICE_TRAY_STEP_NUM                       4

#define ICE_TRAY_HOLD_STEP                      0
#define ICE_TRAY_INIT_PULSE                     1000
#define ICE_TRAY_UP_PULSE                       1000
#define ICE_TRAY_DOWN_PULSE_MAX                 1000

#define ICE_TRAY_DOWN_PULSE_STEP_1              70
#define ICE_TRAY_DOWN_PULSE_STEP_2              70
#define ICE_TRAY_DOWN_PULSE_STEP_3              70
#define ICE_TRAY_DOWN_PULSE_STEP_4              70
#define ICE_TRAY_DOWN_PULSE_STEP_5              70
#define ICE_TRAY_DOWN_PULSE_STEP_6              70
#define ICE_TRAY_DOWN_PULSE_STEP_7              70
#define ICE_TRAY_DOWN_PULSE_STEP_8              70
#define ICE_TRAY_DOWN_PULSE_STEP_9              600

#define ICE_TRAY_DOWN_DELAY_TIME                18          // Ice Tray Down 지연 시간 1800ms@100ms

#define MAX_STEP                                5000

#define ICE_TRAY_STEPMOTOR_ANGLE_PULSE          1050
#define ICE_TRAY_STEPMOTOR_ANGLE_PULSE_2        200

#define BLOCKED_TRAY_IN_CHECK_TIME              1500        // Ice Tray 입수 단수 감지 시간 150sec@100ms
#define BLOCKED_TRAY_CLEAR_TIME                 6000        // Ice Tray 입수 단수 해제 시간 600sec@100ms


void Hal_IceTray_Initialize(void);
void IceTrayControlTimer(void);
void IceTray_StepMotorInterrupt(void);
void control_ice_tray(void);
U8 init_ice_tray(void);
U8 up_ice_tray(void);
U8 down_ice_tray(void);
void control_ice_tray_motor(void);
void ice_tray_motor_operation(U8 mu8Mode);
void run_init_ice_tray(void);
void run_down_ice_tray(void);
void run_up_ice_tray(void);
void run_stop_ice_tray(void);
void Set_IceTrayMode(U8 mu8Mode);
U8 Get_IceTrayMode(void);
void Set_IceTrayValveOut(U8 mu8Status);
U8 Get_IceTrayValveOut(void);
void Hal_IceTrayControl(void);
void check_error_blocked_trayin(void);
U8 Get_IceTrayInBlocked(void);

#endif

void Hal_IceTray_Module_Initialize(void);
void Hal_IceTray_Module_1ms_Control(void);
void Hal_IceTray_Module_Control(void);

#endif
