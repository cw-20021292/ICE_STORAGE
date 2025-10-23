/// @file     Hal_Motor_GasSwitch.h
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Gas Switch 스탭모터 제어

#ifndef _HAL_MOTOR_GAS_SWITCH_H_
#define _HAL_MOTOR_GAS_SWITCH_H_


/*  Gas Switch 모터 제어 Function 사용법
    => Set_CompActStatus(mu8Status);                    // 냉각기 동작 상태 설정 - 0(미동작), 1(동작)
    => Get_CompActStatus();                             // 냉각기 동작 상태 확인 - 0(미동작), 1(동작)
    => Set_CompReactDelayStatus(mu8Status);             // 냉각기 가동 전 지연 동작 상태 설정 - 0(미동작), 1(동작)
    => Get_CompReactDelayStatus();                      // 냉각기 가동 전 지연 동작 상태 확인 - 0(미동작), 1(동작)
    => Get_GasSwitchStatus();                           // Gas Switch의 현제 제어 상태 확인 - 0(냉수), 1(얼음), 2(핫가스)
*/


#define MOTOR_GAS_SWITCH_USE                                USE

#if (MOTOR_GAS_SWITCH_USE == USE)

#define GAS_SWITCH_CONTROL_TIME_PERIOD                      100         // Gas Switch 제어 주기 100ms@1ms

/// @brief      Gas Switch 초기화 동작 제어 Step 테이블
/// @details    Gas Switch 초기화 동작 제어 Step을 테이블로 정의한다
typedef enum
{
    INIT_POWER_ON = 0,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_COLD,
    INIT_WAIT_COLD,

    INIT_DONE
} EGasSwitch_Init_Step_T;


/// @brief      Gas Switch 냉수 동작 제어 Step 테이블
/// @details    Gas Switch 냉수 동작 제어 Step을 테이블로 정의한다
typedef enum
{
    COLD_POWER_ON = 0,
    COLD_WAIT_POWER_ON,

    COLD_CHECK_POSITION,

    COLD_MOVE,
    COLD_WAIT,

    COLD_DONE
} EGasSwitch_Cold_Step_T;


/// @brief      Gas Switch 얼음 동작 제어 Step 테이블
/// @details    Gas Switch 얼음 동작 제어 Step을 테이블로 정의한다
typedef enum
{
    ICE_POWER_ON = 0,
    ICE_WAIT_POWER_ON,

    ICE_CHECK_POSITION,

    ICE_MOVE,
    ICE_WAIT,

    ICE_DONE
} EGasSwitch_Ice_Step_T;


/// @brief      Gas Switch 핫가스 동작 제어 Step 테이블
/// @details    Gas Switch 핫가스 동작 제어 Step을 테이블로 정의한다
typedef enum
{
    HOTGAS_POWER_ON = 0,
    HOTGAS_WAIT_POWER_ON,

    HOTGAS_CHECK_POSITION,

    HOTGAS_MOVE,
    HOTGAS_WAIT,

    HOTGAS_DONE
} EGasSwitch_HotGas_Step_T;


#define P_STEP_MOTOR_GAS_SW_1                P4_bit.no1
#define TURN_ON_MOTOR_GAS_SW_1()             { P_STEP_MOTOR_GAS_SW_1 = 1; }
#define TURN_OFF_MOTOR_GAS_SW_1()            { P_STEP_MOTOR_GAS_SW_1 = 0; }
#define GET_STATUS_MOTOR_GAS_SW_1()          ( P_STEP_MOTOR_GAS_SW_1 )

#define P_STEP_MOTOR_GAS_SW_2                P4_bit.no2
#define TURN_ON_MOTOR_GAS_SW_2()             { P_STEP_MOTOR_GAS_SW_2 = 1; }
#define TURN_OFF_MOTOR_GAS_SW_2()            { P_STEP_MOTOR_GAS_SW_2 = 0; }
#define GET_STATUS_MOTOR_GAS_SW_2()          ( P_STEP_MOTOR_GAS_SW_2 )

#define P_STEP_MOTOR_GAS_SW_3                P4_bit.no3
#define TURN_ON_MOTOR_GAS_SW_3()             { P_STEP_MOTOR_GAS_SW_3 = 1; }
#define TURN_OFF_MOTOR_GAS_SW_3()            { P_STEP_MOTOR_GAS_SW_3 = 0; }
#define GET_STATUS_MOTOR_GAS_SW_3()          ( P_STEP_MOTOR_GAS_SW_3 )

#define P_STEP_MOTOR_GAS_SW_4                P4_bit.no4
#define TURN_ON_MOTOR_GAS_SW_4()             { P_STEP_MOTOR_GAS_SW_4 = 1; }
#define TURN_OFF_MOTOR_GAS_SW_4()            { P_STEP_MOTOR_GAS_SW_4 = 0; }
#define GET_STATUS_MOTOR_GAS_SW_4()          ( P_STEP_MOTOR_GAS_SW_4 )


#define GAS_SWITCH_COLD                     0
#define GAS_SWITCH_ICE                      1
#define GAS_SWITCH_HOTGAS                   2


#define GAS_SWITCH_MODE_NONE                0x00
#define GAS_SWITCH_MODE_INIT                0x01
#define GAS_SWITCH_MODE_COLD                0x02
#define GAS_SWITCH_MODE_ICE                 0x04
#define GAS_SWITCH_MODE_HOTGAS              0x08


#define	PHASE_STEP_NUM		                8           // Gas Switch 스탭모터 제어 위상 개수

#define GAS_SWITCH_COLD_STEP_VAL            (24)
#define GAS_SWITCH_ICE_STEP_VAL             (64)
#define GAS_SWITCH_HOTGAS_STEP_VAL          (104)
#define GAS_SWITCH_END_STEP_VAL             (200)
#define GAS_SWITCH_INIT_STEP_VAL            (-15)

#define POWER_ON_TIME                       10          // Gas Switch 전원 ON 시간 1sec@100ms

#define	DEFAULT_HOLD_STEP	                5		    // Gas Switch 스탭모터 Hold 시간


void Set_CompActStatus(U8 mu8Status);
U8 Get_CompActStatus(void);
void Set_CompReactDelayStatus(U8 mu8Status);
U8 Get_CompReactDelayStatus(void);
U8 Get_GasSwitchStatus(void);
void Hal_GasSwitch_Initialize(void);
void GasSwitchControlTimer(void);
void Hal_GasSwitchControl(void);
U8 InitSwitch(void);
U8 ColdSwitch(void);
U8 IceSwitch(void);
U8 HotGasSwitch(void);
void ProcessGasSwitch(void);
void GasSwitch_StepMotorTimerInterrupt(void);
void HAL_ControlStepMotor(void);
void HAL_MoveStep(void);
void HAL_ControlGasValveMotor(U8 mu8Mode);
void GasSwitchInit(void);
void GasSwitchCold(void);
void GasSwitchIce(void);
void GasSwitchHotGas(void);

#endif

void Hal_GasSwitch_Module_Initialize(void);
void Hal_GasSwitch_Module_1ms_Control(void);
void Hal_GasSwitch_Module_Control(void);


#endif
