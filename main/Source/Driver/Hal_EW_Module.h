/// @file     Hal_EW_Module.h
/// @date     2025/03/12
/// @author   Jaejin Ham
/// @brief    EW 살균 모듈 동작 제어 관련 file

#ifndef _HAL_EW_MODULE_H_
#define _HAL_EW_MODULE_H_


/*  EW 제어 Function 사용법
    => Set_EW_Control(On/Off data);

    사용 예)
    Set_EW_Control(ON);                 // EW ON
    Set_EW_Control(1);                  // EW ON
    Set_EW_Control(OFF);                // EW OFF
    Set_EW_Control(0);                  // EW OFF
*/


#define EW_MODULE_USE                                   USE         // EW 모듈 사용 여부 - 0(No Use), 1(Use)

#define EW_PWM_CON_NO_USE                               0
#define EW_PWM_CON_REGISTER_USE                         1
#define EW_PWM_CON_TRIG_USE                             2
#define EW_PWM_CONTROL_USE                              EW_PWM_CON_NO_USE   // EW 모듈 전압 가변 사용 여부 - 0(No Use), 1(Use PWM), 2(Use Trig)

// EW port 설정
// 미사용 port는 0으로 처리
#if (EW_PWM_CONTROL_USE == EW_PWM_CON_REGISTER_USE)
//#define EW_MODULE_PWM_START()                         exPwm_Start();  // Code Generate된 EW의 PWM 제어 Start함수 배치
//#define EW_MODULE_PWM_STOP()                          exPwm_Stop();   // Code Generate된 EW의 PWM 제어 Stop함수 배치
#define REGISTER_DEFINE_EW_MODULE_PWM                   TDR11           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define REGISTER_EW_MODULE_PWM_MAX_VALUE                0x3E7F          // PWM 제어 레지스터에 적용할 수 있는   설정 최대값
#define REGISTER_EW_MODULE_PWM_INITIAL_VALUE            0x28            // PWM 제어 레지스터에 적용되는 초기값
#define EW_MODULE_PWM_START()                           0               // Code Generate된 PWM 제어 Start함수 배치
#define EW_MODULE_PWM_STOP()                            0               // Code Generate된 PWM 제어 Stop함수 배치
#endif


#if (EW_MODULE_USE == USE)

// EW Module Port Define
#if (EW_PWM_CONTROL_USE == EW_PWM_CON_REGISTER_USE)
#define EW_PWM_REGISTER                 REGISTER_DEFINE_EW_MODULE_PWM
#endif

#if (EW_PWM_CONTROL_USE == EW_PWM_CON_TRIG_USE)
#define P_EW_TRIG                       P8_bit.no5
#define TURN_ON_EW_TRIG()               { P_EW_TRIG = 1; }
#define TURN_OFF_EW_TRIG()              { P_EW_TRIG = 0; }
#define GET_STATUS_EW_TRIG()            ( P_EW_TRIG )

#endif

#define P_EW_ON                         P8_bit.no4
#define TURN_ON_EW()                    { P_EW_ON = 1; }
#define TURN_OFF_EW()                   { P_EW_ON = 0; }
#define GET_STATUS_EW()                 ( P_EW_ON )


// EW Module의 V 피드백 AD ID 배치
#define EW_MODULE_V_FB_ID               ADC_ID_EW_V_FB
#define EW_MODULE_I_FB_ID               ADC_ID_EW_I_FB

#define EW_CONTROL_TIME_PERIOD          10         // 10ms@1ms

void Hal_EW_Initialize(void);
void Set_EW_Control(U8 mu8Status);
void EW_ControlTimer(void);
void EW_Control(void);
void Hal_EW_Control(void);

#endif


void Hal_EW_Module_Initialize(void);
void Hal_EW_Module_1ms_Control(void);
void Hal_EW_Module_Control(void);

#endif

