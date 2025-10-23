/// @file     Hal_Flowmeter.h
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    Flowmeter Control file


#ifndef _HAL_FLOW_METER_H_
#define _HAL_FLOW_METER_H_


/*  Flowmeter 제어 Function 사용법
    => Get_Flowmeter_Hz(Flowmeter ID);                     // ID에 해당하는 Flowmeter의 Hz 확인
    => Set_FlowmeterCountdownPulse(mu8FlowmeterID, Pulse); // ID에 해당하는 Flowmeter에 Count Down 하려는 Pulse 갯수 할당
    => Get_FlowmeterCountdownEnd(mu8FlowmeterID);          // ID에 해당하는 Flowmeter에 할당했던 Pulse 갯수의 Count Down 완료 체크 - 0(미완료), 1(완료)

    사용 예)
    u8Return = Get_Flowmeter_Hz(FLOWMETER_1);               // FLOWMETER_1 ID의 유량센서 Hz값 확인
    Set_FlowmeterCountdownPulse(FLOWMETER_1, 1000);         // FLOWMETER_1 ID의 유량센서에 Count Down할 1000 Pulse 할당
    u8Return = Get_FlowmeterCountdownEnd(FLOWMETER_1);      // FLOWMETER_1 ID의 pulse Count Down 완료 여부 확인
*/


// Flowmeter 최대 5개 사용 가능
typedef enum
{
    FLOWMETER_ID_NONE = 0,                              // 0 :
    FLOWMETER_ID_FLOW_1,                                // 1 : 입수 유량 센서
    FLOWMETER_ID_FLOW_2,                                // 2 :
    FLOWMETER_ID_FLOW_3,                                // 3 :
    FLOWMETER_ID_MAX                                    // 6 Max, 6이 넘으면 안됨
} EFlowmeterID_T;

#define FLOWMETER_COUNT                                 1           // Flowmeter 개수 - MAX 5

//#define EW_MODULE_PWM_START()                         exStart();  // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
//#define EW_MODULE_PWM_STOP()                          exStop();   // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#if (FLOWMETER_COUNT >= 1)
#define FLOWMETER_1_ID                                  FLOWMETER_ID_FLOW_1   // Flowmeter 1의 설정된 ID
#define FLOWMETER_1_INTERRUPT_START()                   R_Config_INTC_INTP2_Start();    // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
#define FLOWMETER_1_INTERRUPT_STOP()                    R_Config_INTC_INTP2_Stop();     // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#endif

#if (FLOWMETER_COUNT >= 2)
#define FLOWMETER_2_ID                                  FLOWMETER_ID_FLOW_2     // Flowmeter 2의 설정된 ID
#define FLOWMETER_2_INTERRUPT_START()                   0                       // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
#define FLOWMETER_2_INTERRUPT_STOP()                    0                       // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#endif

#if (FLOWMETER_COUNT >= 3)
#define FLOWMETER_3_ID                                  FLOWMETER_ID_FLOW_3     // Flowmeter 3의 설정된 ID
#define FLOWMETER_3_INTERRUPT_START()                   0                       // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
#define FLOWMETER_3_INTERRUPT_STOP()                    0                       // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#endif

#if (FLOWMETER_COUNT >= 4)
#define FLOWMETER_4_ID                                  0                       // Flowmeter 4의 설정된 ID
#define FLOWMETER_4_INTERRUPT_START()                   0                       // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
#define FLOWMETER_4_INTERRUPT_STOP()                    0                       // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#endif

#if (FLOWMETER_COUNT >= 5)
#define FLOWMETER_5_ID                                  0                       // Flowmeter 5의 설정된 ID
#define FLOWMETER_5_INTERRUPT_START()                   0                       // Code Generate된 Flowmeter 외부 인터럽트 Start함수 배치
#define FLOWMETER_5_INTERRUPT_STOP()                    0                       // Code Generate된 Flowmeter 외부 인터럽트 Stop함수 배치
#endif


#if (FLOWMETER_COUNT > 0)

#define FLOWMETER_ID_MAX_COUNT                  FLOWMETER_ID_MAX

#define FLOWMETER_CHECK_CONTROL_TIME_PERIOD     500         // 500ms@1ms

void Flowmeter_Initialize(void);

#if (FLOWMETER_COUNT >= 1)
void Flowmeter_1_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 2)
void Flowmeter_2_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 3)
void Flowmeter_3_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 4)
void Flowmeter_4_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 5)
void Flowmeter_5_Interrupt(void);
#endif

void Flowmeter_CheckControl(void);
void Flowmeter_Hz_Calculation(void);
U16 Get_Flowmeter_Hz(U8 mu8FlowmeterID);
void Set_FlowmeterCountdownPulse(U8 mu8FlowmeterID, U16 mu16Pulse);
U16 Get_FlowmeterCountdownPulse(U8 mu8FlowmeterID);
void Flowmeter_Pulse_CountDownHz(U8 mu8FlowmeterID);
U8 Get_FlowmeterCountdownEnd(U8 mu8FlowmeterID);
void Set_FlowmeterTestHz(U8 mu8FlowmeterID, U16 mu16Hz);
U16 Get_FlowmeterTestHz(U8 mu8FlowmeterID);


#endif


void Hal_Flowmeter_Module_Initialize(void);
void Hal_Flowmeter_Module_1ms_Control(void);

#endif

