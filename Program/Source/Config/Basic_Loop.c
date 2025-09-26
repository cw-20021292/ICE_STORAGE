/// @file     Basic_Loop.c
/// @date     2025/02/25
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    기본 동작 함수  File


#include "Global_Header.h"


/*  코드 중 사용하지 않는 함수들은 모두 주석 처리하여 미사용 처리 시킨다    */

U16 gu16BootTime = 0;
U8 gu8BootComplete = 0;


/// @brief      모든 Fuction Initialize(프로그램 시작시 Initial)
/// @details    프로그램 동작 시작 전 동작과 관련된 모든 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Base_Initialize(void)
{
// * Hal (HW) *********************************************************************************
    // Input **********************************************************************************
    Hal_ADC_Module_Initialize();            // ADC 데이터 모두 초기화
    Hal_Flowmeter_Module_Initialize();      // 유량 센서 관련 초기화
    Hal_ReedSW_Module_Initialize();         // Reed SW 제어 관련 초기화
    Hal_RTC_Module_Initialize();            // RTC 관련 초기화
    Hal_TDS_Module_Initialize();            // TDS 관련 초기화
    Hal_LevelSensor_Module_Initialize();    // 수위 센서 관련 초기화
    /* 얼음 관련 */
    Hal_Input_IceTray_Module_Initialize();  // Ice Tray 감지 제어 관련 초기화

    // Output *********************************************************************************
    Hal_AC_Motor_Module_Initialize();       // AC 모터 관련 초기화
    Hal_BoostPump_Module_Initialize();      // 부스트 펌프 관련 초기화
    Hal_Buzzer_Module_Initialize();         // 부저음 제어 관련 초기화
    Hal_DC_Fan_Module_Initialize();         // DC FAN 제어 관련 초기화
    Hal_DC_Pump_Module_Initialize();        // DC Pump 제어 관련 초기화
    Hal_EW_Module_Initialize();             // EW 모듈 제어 관련 초기화
    Hal_LeakSensor_Module_Initialize();     // 누수 센서 전원 제어 관련 초기화
    Hal_Stepmotor_Module_Initialize();      // Stepmotor 제어 관련 초기화
    Hal_StirringMoter_Module_Initialize();  // 교반 모터 제어 관련 초기화
    Hal_ColdTH_Module_Initialize();         // 냉수 온도 센서 전원 제어 관련 초기화
    Hal_UV_Module_Initialize();             // UV 제어 관련 초기화
    Hal_Voice_Module_Initialize();          // Voice IC 초기화
    Hal_SmallValve_Module_Initialize();     // 소형 밸브 제어 관련 초기화
    Hal_Valve_Module_Initialize();          // 일반 밸브 제어 관련 초기화
    /* 히팅 관련 */
    Hal_Heater_Module_Initialize();         // 일반 히터 관련 초기화
    Hal_Inst_Heater_Module_Initialize();    // 순간온수 히터 관련 초기화
    /* 냉각 관련 */
    Hal_BLDC_Comp_Module_Initialize();      // BLDC Comp 제어 관련 초기화
    Hal_Comp_Module_Initialize();           // 정속형 Comp 제어 관련 초기화
    Hal_TEM_Module_Initialize();            // TEM 제어 관련 초기화
    /* 얼음 관련 */
    Hal_SwingBar_Module_Initialize();       // Swing Bar 제어 관련 초기화
    Hal_AC_Motor_IceMaking_Module_Initialize();     // 제빙 관련 AC 모터들 관련 초기화
    Hal_GasSwitch_Module_Initialize();      // Gas Switch 제어 관련 초기화
    Hal_IceDoor_Module_Initialize();        // Ice Door 제어 관련 초기화
    Hal_IceTray_Module_Initialize();        // Ice Tray 제어 관련 초기화

// * Lib (Function) ***************************************************************************
    Lib_ADC_Module_Initialize();            // AD Convert 동작 관련 초기화
    Lib_BoostPump_Module_Initialize();      // 부스트 펌프 동작 관련 초기화
    Lib_OperatingMode_Module_Initialize();  // 동작 모드 제어 관련 초기화
    Lib_PowerSave_Module_Initialize();      // 절전 판단 동작 관련 초기화
    Lib_TimeScheduler_Module_Initialize();  // Time Scheduler 관련 초기화
    Lib_UV_Sterilize_Module_Initialize();   // UV 살균 제어 관련 초기화
    Lib_WaterError_Module_Initialize();     // 정수기 에러 체크 제어 관련 초기화
    Lib_WaterLevel_Module_Initialize();     // 수위 감지 제어 관련 초기화
    Lib_Effluent_Module_Initialize();       // 추출 제어 관련 초기화
    Lib_PTA_Mode_Module_Initialize();       // PTA 모드 제어 관련 초기화
    /* 히팅 관련 */
    Lib_Heater_Module_Initialize();         // 히터 동작 관련 초기화
    Lib_InstHeater_Module_Initialize();     // 순간 온수 제어 관련 초기화
    /* 냉각 관련 */
    Lib_NormalComp_Module_Initialize();     // 정속형 Comp 제어 관련 초기화
    Lib_BLDC_Comp_Module_Initialize();      // BLDC Comp 제어 관련 초기화
    Lib_TEM_Module_Initialize();            // TEM 제어 관련 초기화
    Lib_CoolFan_Module_Initialize();        // 냉각 FAN 제어 관련 초기화
    /* 얼음 관련 */
    Lib_IceMaking_ModeDecision_Module_Initialize();     // 냉각,제빙 모드 판단 제어 관련 초기화
    Lib_IceMaking_Module_Initialize();      // 제빙 동작 제어 관련 초기화

// * 기타 **************************************************************************************
    Boot_Initialize();
}


/// @brief      Base Interval Timer(in 1ms Timer Interrupt)
/// @details    1ms 동작시킬 함수들을 추가하여 1ms Interrput에서 동작할 수 있도록 한다
/// @param      void
/// @return     void
void Base_Timer_1ms(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // 일반 동작 모드
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_Flowmeter_Module_1ms_Control();     // Flowmeter Check 제어
            Hal_ReedSW_Module_1ms_Control();        // Reed SW Check 시간 Counter
            Hal_RTC_Module_1ms_Control();           // RTC Process Control Time Counter
            Hal_TDS_Module_1ms_Control();           // TDS In 또는 Out 센서 제어 시간 Counter
            Hal_BLDC_Motor_Module_1ms_Control();
            Hal_LevelSensor_Module_1ms_Control();   // 수위 센서 체크 제어 시간 Counter
            /* 얼음 관련 */
            Hal_Input_IceTray_Module_1ms_Control(); // Ice Tray 감지 제어 시간 Counter

            // Output *********************************************************************************
            Hal_BoostPump_Module_1ms_Control();     // Boost Pump 제어 시간 Counter
            Hal_Buzzer_Module_1ms_Control();        // 부저 제어 함수
            Hal_DC_Fan_Module_1ms_Control();        // DC Fan 제어 시간 Counter
            Hal_DC_Pump_Module_1ms_Control();       // DC Pump 제어 시간 Counter
            Hal_EW_Module_1ms_Control();            // EW 모듈 제어 시간 Counter
            Hal_Stepmotor_Module_1ms_Control();     // Stepmotor 제어 함수(시간이 밀리면 안되므로 Interrput 루틴 안에서 직접 제어)
            Hal_StirringMotor_Module_1ms_Control(); // 교반모터 제어 시간 Counter
            Hal_ColdTH_Module_1ms_Control();        // 냉수 온도 센서 전원 제어 시간 Counter
            Hal_UV_Module_1ms_Control();            // UV Control Timer
            Hal_Voice_Module_1ms_Control();         // Voice IC Control Timer
            Hal_SmallValve_Module_1ms_Control();    // 소형 밸브 제어 시간 Timer
            Hal_Valve_Module_1ms_Control();         // 일반 밸브 제어 시간 Timer
            Hal_HumidTempSensor_Module_1ms_Control(); // 습도/온도 센서 제어 시간 Counter
            /* 히팅 관련 */
            Hal_Heater_Module_1ms_Control();        // 일반 히터 제어 시간 Counter
            Hal_Inst_Heater_Module_1ms_Control();   // 순간온수 히터 제어 시간 Counter
            /* 냉각 관련 */
            Hal_BLDC_Comp_Module_1ms_Control();     // BLDC Comp 제어 시간 Counter
            Hal_Comp_Module_1ms_Control();          // 정속형 Comp 제어 시간 Counter
            Hal_TEM_Module_1ms_Control();           // TEM 제어 시간 Counter
            /* 얼음 관련 */
            Hal_SwingBar_Module_1ms_Control();      // Swing Bar 제어 시간 Counter
            Hal_AC_Motor_IceMaking_Module_1ms_Control();    // 제빙 관련 AC 모터 제어 시간 Counter
            Hal_GasSwitch_Module_1ms_Control();     // Gas Switch 제어 시간 Counter
            Hal_IceDoor_Module_1ms_Control();       // Ice Door 제어 시간 Counter
            Hal_IceTray_Module_1ms_Control();       // Ice Tray 제어 시간 Counter

            // * Lib (Function) ***************************************************************************
            Lib_ADC_Module_1ms_Control();           // ADC 동작 제어 시간 Counter
            Lib_BoostPump_Module_1ms_Control();     // 부스트 펌프 제어 시간 Counter
            Lib_TimeScheduler_Module_1ms_Control(); // Time Scehduler 제어 시간  Counter
            Lib_UV_Sterilize_Module_1ms_Control();  // UV 살균 제어 시간 Counter
            Lib_WaterError_Module_1ms_Control();    // 정수기 에러 체크 제어 시간 Counter
            Lib_Effluent_Module_1ms_Control();      // 추출 제어 시간 Timer
            Lib_PTA_Mode_Module_1ms_Control();      // PTA 검사 제어 시간 Timer
            /* 히팅 관련 */
            Lib_Heater_Module_1ms_Control();        // 히터 판단 동작 제어 시간  Counter
            Lib_InstHeater_Module_1ms_Control();    // 순간 온수 히터 제어 시간 Timer
            /* 냉각 관련 */
            Lib_NormalComp_Module_1ms_Control();    // 정속형 Comp 제어 시간 Timer
            Lib_BLDC_Comp_Module_1ms_Control();     // BLDC Comp 제어 시간 Timer
            Lib_TEM_Module_1ms_Control();           // TEM 제어 시간 Timer
            Lib_CoolFan_Module_1ms_Control();       // 냉각 FAN 제어 시간 Timer
            /* 얼음 관련 */
            Lib_IceMaking_ModeDecision_Module_1ms_Control();    // 냉각, 제빙 모드 판단 제어 시간 Timer
            Lib_IceMaking_Module_1ms_Control();     // 제빙 동작 제어 시간 Timer
            /*청정기 모터 관련*/
            Lib_BLDCMotor_Module_1ms_Control();      // BLDC Motor 동작 제어 시간 Counter

            // * 기타 **************************************************************************************
            BootTimeCheck();
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB 테스트 모드
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front 테스트 모드
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA 테스트 모드1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA 테스트 모드2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA 테스트 모드3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // 여분 테스트 모드1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // 여분 테스트 모드2
            break;

        default:
            break;

    }
}


/// @brief      Base Interval Timer(in 100us Timer Interrupt)
/// @details    100us 동작시킬 함수들을 추가하여 100us Interrput에서 동작할 수 있도록 한다
/// @param      void
/// @return     void
void Base_Timer_100us(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // 일반 동작 모드
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_TDS_Module_100us_Control();         // TDS Out 센서 제어 시간 Counter

            // Output *********************************************************************************

            // * Lib (Function) ***************************************************************************

            // * 기타 **************************************************************************************
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB 테스트 모드
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front 테스트 모드
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA 테스트 모드1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA 테스트 모드2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA 테스트 모드3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // 여분 테스트 모드1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // 여분 테스트 모드2
            break;

        default:
            break;

    }
}


/// @brief      While 문 안에서 동작 함수들을  처리하는 루틴(in While Loop)
/// @details    While 문에 추가하여 반복 동작하도록 할 함수들을 추가하여 동작시킨다
/// @param      void
/// @return     void
void FunctionProcess_In_WhileLoop(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // 일반 동작 모드
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_LevelSensor_Module_Control();       // 수위 센서 제어 함수
            Hal_ReedSW_Module_Control();            // Reed SW 체크 함수
            Hal_RTC_Module_Control();               // RTC 관련 제어 함수
            Hal_TDS_Module_Control();               // TDS 관련 제어 함수
            /* 얼음 관련 */
            Hal_Input_IceTray_Module_Control();     // Ice Tray 감지 제어 함수

            // Output **********************************************************************************
            Hal_AC_Motor_Module_Control();          // AC 모터 전원 제어
            Hal_BoostPump_Module_Control();         // 부스트 펌프 제어 함수
            Hal_ColdTH_Module_Control();            // 냉수 온도 센서 전원 제어 함수
            Hal_DC_Fan_Module_Control();            // DC FAN Control 함수
            Hal_DC_Pump_Module_Control();           // DC Pump Control 함수
            Hal_EW_Module_Control();                // EW 모듈 제어 함수
            Hal_LeakSensor_Module_Control();        // 누수 센서 전원 제어 함수
            Hal_StirringMotor_Module_Control();     // 교반 모터 제어 함수
            Hal_UV_Module_Control();                // UV Control 함수
            Hal_Voice_Module_Control();             // Voice 제어 함수
            Hal_SmallValve_Module_Control();        // 소형 밸브 제어 함수
            Hal_Valve_Module_Control();             // 일반 밸브 제어 함수
            /* 히팅 관련 */
            Hal_Heater_Module_Control();            // 일반 히터 제어 함수
            Hal_Inst_Heater_Module_Control();       // 순간 온수 히터 제어 함수
            /* 냉각 관련 */
            Hal_BLDC_Comp_Module_Control();         // BLDC Comp 제어 함수
            Hal_Comp_Module_Control();              // 정속형 Comp 제어 함수
            Hal_TEM_Module_Control();               // TEM 제어
            /* 얼음 관련 */
            Hal_SwingBar_Module_Control();          // Swing Bar 제어 함수
            Hal_AC_Motor_IceMaking_Module_Control();    // 제빙 관련 AC 모터 제어 함수
            Hal_GasSwitch_Module_Control();         // Gas Switch 제어 함수
            Hal_IceDoor_Module_Control();           // Ice Door 제어 함수
            Hal_IceTray_Module_Control();           // Ice Tray 제어 함수

            // * Lib (Function) ***************************************************************************
            Lib_ADC_Module_Control();               // ADC 동작 제어
            Lib_BoostPump_Module_Control();         // 부스트 펌프 동작 제어
            Lib_PowerSave_Module_Control();         // 미사용 절전, 절전 판단 동작 제어
            Lib_TimeScheduler_Module_Control();     // Time Scheduler 동작 제어
            Lib_UV_Sterilize_Module_Control();      // UV 살균 동작 제어
            Lib_WaterError_Module_Control();        // 정수기 에러 체크 동작 제어
            Lib_WaterLevel_Module_Control();        // 수위 감지 동작 제어
            Lib_WaterInput_Module_Control();        // 탱크 입수 동작 제어
            Lib_Effluent_Module_Control();          // 추출 동작 제어
            Lib_PTA_Mode_Module_Control();          // PTA 검사 제어
            /* 히팅 관련 */
            Lib_Heater_Module_Control();            // 히터 제어
            Lib_InstHeater_Module_Control();        // 순간 온수 제어
            /* 냉각 관련 */
            Lib_NormalComp_Module_Control();        // 정속형 Comp 제어
            Lib_BLDC_Comp_Module_Control();         // BLDC Comp 제어
            Lib_TEM_Module_Control();               // TEM 제어
            Lib_CoolFan_Module_Control();           // 냉각 FAN 제어
            /* 얼음 관련 */
            Lib_IceMaking_ModeDecision_Module_Control();    // 냉각, 제빙 모드 판단 제어
            Lib_IceMaking_Module_Control();         // 제빙 동작 제어

            // * 기타 **************************************************************************************
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB 테스트 모드
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front 테스트 모드
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA 테스트 모드1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA 테스트 모드2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA 테스트 모드3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // 여분 테스트 모드1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // 여분 테스트 모드2
            break;

        default:
            break;

    }
}



/// @brief      부팅 동작 관련 변수 초기화
/// @details    부팅 동작과 관련된 변수들을 모두 초기화 한다
/// @param      void
/// @return     void
void Boot_Initialize(void)
{
    gu16BootTime = BOOT_TIME;
    gu8BootComplete = CLEAR;
}


/// @brief      부팅 동작 시간 타이머 카운트(@1ms)
/// @details    부팅 동작을 하는 시간을 카운트 한다
/// @param      void
/// @return     void
void BootTimeCheck(void)
{
    if (gu16BootTime == 0)
    {
        gu8BootComplete = SET;
    }
    else
    {
        gu16BootTime--;
    }
}


/// @brief      제품 부팅 시간 잔여 확인
/// @details    제품 부팅 후 일정시간 부팅 동작을 진행하는 시간 잔여 확인
/// @param      void
/// @return     return : 부팅 동작 잔여 시간(100ms 단위)
U16 Get_BootTime(void)
{
    return  gu16BootTime;
}


/// @brief      제품 부팅이 완료되었는지 확인하는 함수
/// @details    제품 부팅 후 일정시간 부팅 동작을 완료하였는지 체크하여 그 상태를 반환한다
/// @param      void
/// @return     return : 부팅 동작 완료 여부 - 0(부팅 중), 1(부팅 완료)
U8 Get_BootComplete(void)
{
    if (gu8BootComplete)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


// User가 자신과 관련된 Timer, Interrupt 등 Start 함수를 열거할 것 ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

/// @brief      모든 인터럽트(Timer 등) 시작 함수
/// @details    모든 사용하는 인터럽트들이 시작될 수 있도록 Start 함수들을 추가하여 인터럽트들을 동작 시작시킨다
/// @param      void
/// @return     void
void Interrupt_Start(void)
{
    Timer_1ms_Start();
    Timer_100us_Start();
    ADC_CheckStart(0);
}


/// @brief      모든 통신 시작 함수
/// @details    모든 통신 관련 SFR을 동작 시킬 수 있도록 Start 함수들을 추가하여 통신들을 동작 시작시킨다.
/// @param      void
/// @return     void
void Serial_Start(void)
{
    R_Config_UART0_Start();         // UART0 Start
    R_Config_UART2_Start();         // UART2 Start
//    BLDC_COMP_UART_START();
}



