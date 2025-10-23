/// @file     Basic_Loop.c
/// @date     2025/02/25
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    �⺻ ���� �Լ�  File


#include "Global_Header.h"


/*  �ڵ� �� ������� �ʴ� �Լ����� ��� �ּ� ó���Ͽ� �̻�� ó�� ��Ų��    */

U16 gu16BootTime = 0;
U8 gu8BootComplete = 0;


/// @brief      ��� Fuction Initialize(���α׷� ���۽� Initial)
/// @details    ���α׷� ���� ���� �� ���۰� ���õ� ��� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Base_Initialize(void)
{
// * Hal (HW) *********************************************************************************
    // Input **********************************************************************************
    Hal_ADC_Module_Initialize();            // ADC ������ ��� �ʱ�ȭ
    Hal_Flowmeter_Module_Initialize();      // ���� ���� ���� �ʱ�ȭ
    Hal_ReedSW_Module_Initialize();         // Reed SW ���� ���� �ʱ�ȭ
    Hal_RTC_Module_Initialize();            // RTC ���� �ʱ�ȭ
    Hal_TDS_Module_Initialize();            // TDS ���� �ʱ�ȭ
    Hal_LevelSensor_Module_Initialize();    // ���� ���� ���� �ʱ�ȭ
    /* ���� ���� */
    Hal_Input_IceTray_Module_Initialize();  // Ice Tray ���� ���� ���� �ʱ�ȭ

    // Output *********************************************************************************
    Hal_AC_Motor_Module_Initialize();       // AC ���� ���� �ʱ�ȭ
    Hal_BoostPump_Module_Initialize();      // �ν�Ʈ ���� ���� �ʱ�ȭ
    Hal_Buzzer_Module_Initialize();         // ������ ���� ���� �ʱ�ȭ
    Hal_DC_Fan_Module_Initialize();         // DC FAN ���� ���� �ʱ�ȭ
    Hal_DC_Pump_Module_Initialize();        // DC Pump ���� ���� �ʱ�ȭ
    Hal_EW_Module_Initialize();             // EW ��� ���� ���� �ʱ�ȭ
    Hal_LeakSensor_Module_Initialize();     // ���� ���� ���� ���� ���� �ʱ�ȭ
    Hal_Stepmotor_Module_Initialize();      // Stepmotor ���� ���� �ʱ�ȭ
    Hal_StirringMoter_Module_Initialize();  // ���� ���� ���� ���� �ʱ�ȭ
    Hal_ColdTH_Module_Initialize();         // �ü� �µ� ���� ���� ���� ���� �ʱ�ȭ
    Hal_UV_Module_Initialize();             // UV ���� ���� �ʱ�ȭ
    Hal_Voice_Module_Initialize();          // Voice IC �ʱ�ȭ
    Hal_SmallValve_Module_Initialize();     // ���� ��� ���� ���� �ʱ�ȭ
    Hal_Valve_Module_Initialize();          // �Ϲ� ��� ���� ���� �ʱ�ȭ
    /* ���� ���� */
    Hal_Heater_Module_Initialize();         // �Ϲ� ���� ���� �ʱ�ȭ
    Hal_Inst_Heater_Module_Initialize();    // �����¼� ���� ���� �ʱ�ȭ
    /* �ð� ���� */
    Hal_BLDC_Comp_Module_Initialize();      // BLDC Comp ���� ���� �ʱ�ȭ
    Hal_Comp_Module_Initialize();           // ������ Comp ���� ���� �ʱ�ȭ
    Hal_TEM_Module_Initialize();            // TEM ���� ���� �ʱ�ȭ
    /* ���� ���� */
    Hal_SwingBar_Module_Initialize();       // Swing Bar ���� ���� �ʱ�ȭ
    Hal_AC_Motor_IceMaking_Module_Initialize();     // ���� ���� AC ���͵� ���� �ʱ�ȭ
    Hal_GasSwitch_Module_Initialize();      // Gas Switch ���� ���� �ʱ�ȭ
    Hal_IceDoor_Module_Initialize();        // Ice Door ���� ���� �ʱ�ȭ
    Hal_IceTray_Module_Initialize();        // Ice Tray ���� ���� �ʱ�ȭ

// * Lib (Function) ***************************************************************************
    Lib_ADC_Module_Initialize();            // AD Convert ���� ���� �ʱ�ȭ
    Lib_BoostPump_Module_Initialize();      // �ν�Ʈ ���� ���� ���� �ʱ�ȭ
    Lib_OperatingMode_Module_Initialize();  // ���� ��� ���� ���� �ʱ�ȭ
    Lib_PowerSave_Module_Initialize();      // ���� �Ǵ� ���� ���� �ʱ�ȭ
    Lib_TimeScheduler_Module_Initialize();  // Time Scheduler ���� �ʱ�ȭ
    Lib_UV_Sterilize_Module_Initialize();   // UV ��� ���� ���� �ʱ�ȭ
    Lib_WaterError_Module_Initialize();     // ������ ���� üũ ���� ���� �ʱ�ȭ
    Lib_WaterLevel_Module_Initialize();     // ���� ���� ���� ���� �ʱ�ȭ
    Lib_Effluent_Module_Initialize();       // ���� ���� ���� �ʱ�ȭ
    Lib_PTA_Mode_Module_Initialize();       // PTA ��� ���� ���� �ʱ�ȭ
    /* ���� ���� */
    Lib_Heater_Module_Initialize();         // ���� ���� ���� �ʱ�ȭ
    Lib_InstHeater_Module_Initialize();     // ���� �¼� ���� ���� �ʱ�ȭ
    /* �ð� ���� */
    Lib_NormalComp_Module_Initialize();     // ������ Comp ���� ���� �ʱ�ȭ
    Lib_BLDC_Comp_Module_Initialize();      // BLDC Comp ���� ���� �ʱ�ȭ
    Lib_TEM_Module_Initialize();            // TEM ���� ���� �ʱ�ȭ
    Lib_CoolFan_Module_Initialize();        // �ð� FAN ���� ���� �ʱ�ȭ
    /* ���� ���� */
    Lib_IceMaking_ModeDecision_Module_Initialize();     // �ð�,���� ��� �Ǵ� ���� ���� �ʱ�ȭ
    Lib_IceMaking_Module_Initialize();      // ���� ���� ���� ���� �ʱ�ȭ

// * ��Ÿ **************************************************************************************
    Boot_Initialize();
}


/// @brief      Base Interval Timer(in 1ms Timer Interrupt)
/// @details    1ms ���۽�ų �Լ����� �߰��Ͽ� 1ms Interrput���� ������ �� �ֵ��� �Ѵ�
/// @param      void
/// @return     void
void Base_Timer_1ms(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // �Ϲ� ���� ���
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_Flowmeter_Module_1ms_Control();     // Flowmeter Check ����
            Hal_ReedSW_Module_1ms_Control();        // Reed SW Check �ð� Counter
            Hal_RTC_Module_1ms_Control();           // RTC Process Control Time Counter
            Hal_TDS_Module_1ms_Control();           // TDS In �Ǵ� Out ���� ���� �ð� Counter
            Hal_BLDC_Motor_Module_1ms_Control();
            Hal_LevelSensor_Module_1ms_Control();   // ���� ���� üũ ���� �ð� Counter
            /* ���� ���� */
            Hal_Input_IceTray_Module_1ms_Control(); // Ice Tray ���� ���� �ð� Counter

            // Output *********************************************************************************
            Hal_BoostPump_Module_1ms_Control();     // Boost Pump ���� �ð� Counter
            Hal_Buzzer_Module_1ms_Control();        // ���� ���� �Լ�
            Hal_DC_Fan_Module_1ms_Control();        // DC Fan ���� �ð� Counter
            Hal_DC_Pump_Module_1ms_Control();       // DC Pump ���� �ð� Counter
            Hal_EW_Module_1ms_Control();            // EW ��� ���� �ð� Counter
            Hal_Stepmotor_Module_1ms_Control();     // Stepmotor ���� �Լ�(�ð��� �и��� �ȵǹǷ� Interrput ��ƾ �ȿ��� ���� ����)
            Hal_StirringMotor_Module_1ms_Control(); // ���ݸ��� ���� �ð� Counter
            Hal_ColdTH_Module_1ms_Control();        // �ü� �µ� ���� ���� ���� �ð� Counter
            Hal_UV_Module_1ms_Control();            // UV Control Timer
            Hal_Voice_Module_1ms_Control();         // Voice IC Control Timer
            Hal_SmallValve_Module_1ms_Control();    // ���� ��� ���� �ð� Timer
            Hal_Valve_Module_1ms_Control();         // �Ϲ� ��� ���� �ð� Timer
            Hal_HumidTempSensor_Module_1ms_Control(); // ����/�µ� ���� ���� �ð� Counter
            /* ���� ���� */
            Hal_Heater_Module_1ms_Control();        // �Ϲ� ���� ���� �ð� Counter
            Hal_Inst_Heater_Module_1ms_Control();   // �����¼� ���� ���� �ð� Counter
            /* �ð� ���� */
            Hal_BLDC_Comp_Module_1ms_Control();     // BLDC Comp ���� �ð� Counter
            Hal_Comp_Module_1ms_Control();          // ������ Comp ���� �ð� Counter
            Hal_TEM_Module_1ms_Control();           // TEM ���� �ð� Counter
            /* ���� ���� */
            Hal_SwingBar_Module_1ms_Control();      // Swing Bar ���� �ð� Counter
            Hal_AC_Motor_IceMaking_Module_1ms_Control();    // ���� ���� AC ���� ���� �ð� Counter
            Hal_GasSwitch_Module_1ms_Control();     // Gas Switch ���� �ð� Counter
            Hal_IceDoor_Module_1ms_Control();       // Ice Door ���� �ð� Counter
            Hal_IceTray_Module_1ms_Control();       // Ice Tray ���� �ð� Counter

            // * Lib (Function) ***************************************************************************
            Lib_ADC_Module_1ms_Control();           // ADC ���� ���� �ð� Counter
            Lib_BoostPump_Module_1ms_Control();     // �ν�Ʈ ���� ���� �ð� Counter
            Lib_TimeScheduler_Module_1ms_Control(); // Time Scehduler ���� �ð�  Counter
            Lib_UV_Sterilize_Module_1ms_Control();  // UV ��� ���� �ð� Counter
            Lib_WaterError_Module_1ms_Control();    // ������ ���� üũ ���� �ð� Counter
            Lib_Effluent_Module_1ms_Control();      // ���� ���� �ð� Timer
            Lib_PTA_Mode_Module_1ms_Control();      // PTA �˻� ���� �ð� Timer
            /* ���� ���� */
            Lib_Heater_Module_1ms_Control();        // ���� �Ǵ� ���� ���� �ð�  Counter
            Lib_InstHeater_Module_1ms_Control();    // ���� �¼� ���� ���� �ð� Timer
            /* �ð� ���� */
            Lib_NormalComp_Module_1ms_Control();    // ������ Comp ���� �ð� Timer
            Lib_BLDC_Comp_Module_1ms_Control();     // BLDC Comp ���� �ð� Timer
            Lib_TEM_Module_1ms_Control();           // TEM ���� �ð� Timer
            Lib_CoolFan_Module_1ms_Control();       // �ð� FAN ���� �ð� Timer
            /* ���� ���� */
            Lib_IceMaking_ModeDecision_Module_1ms_Control();    // �ð�, ���� ��� �Ǵ� ���� �ð� Timer
            Lib_IceMaking_Module_1ms_Control();     // ���� ���� ���� �ð� Timer
            /*û���� ���� ����*/
            Lib_BLDCMotor_Module_1ms_Control();      // BLDC Motor ���� ���� �ð� Counter

            // * ��Ÿ **************************************************************************************
            BootTimeCheck();
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA �׽�Ʈ ���2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA �׽�Ʈ ���3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // ���� �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // ���� �׽�Ʈ ���2
            break;

        default:
            break;

    }
}


/// @brief      Base Interval Timer(in 100us Timer Interrupt)
/// @details    100us ���۽�ų �Լ����� �߰��Ͽ� 100us Interrput���� ������ �� �ֵ��� �Ѵ�
/// @param      void
/// @return     void
void Base_Timer_100us(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // �Ϲ� ���� ���
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_TDS_Module_100us_Control();         // TDS Out ���� ���� �ð� Counter

            // Output *********************************************************************************

            // * Lib (Function) ***************************************************************************

            // * ��Ÿ **************************************************************************************
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA �׽�Ʈ ���2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA �׽�Ʈ ���3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // ���� �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // ���� �׽�Ʈ ���2
            break;

        default:
            break;

    }
}


/// @brief      While �� �ȿ��� ���� �Լ�����  ó���ϴ� ��ƾ(in While Loop)
/// @details    While ���� �߰��Ͽ� �ݺ� �����ϵ��� �� �Լ����� �߰��Ͽ� ���۽�Ų��
/// @param      void
/// @return     void
void FunctionProcess_In_WhileLoop(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    switch (mu8OperationMode)
    {
        case OPERATION_MODE_ID_NORMAL:          // �Ϲ� ���� ���
            // * Hal (HW) *********************************************************************************
            // Input **********************************************************************************
            Hal_LevelSensor_Module_Control();       // ���� ���� ���� �Լ�
            Hal_ReedSW_Module_Control();            // Reed SW üũ �Լ�
            Hal_RTC_Module_Control();               // RTC ���� ���� �Լ�
            Hal_TDS_Module_Control();               // TDS ���� ���� �Լ�
            /* ���� ���� */
            Hal_Input_IceTray_Module_Control();     // Ice Tray ���� ���� �Լ�

            // Output **********************************************************************************
            Hal_AC_Motor_Module_Control();          // AC ���� ���� ����
            Hal_BoostPump_Module_Control();         // �ν�Ʈ ���� ���� �Լ�
            Hal_ColdTH_Module_Control();            // �ü� �µ� ���� ���� ���� �Լ�
            Hal_DC_Fan_Module_Control();            // DC FAN Control �Լ�
            Hal_DC_Pump_Module_Control();           // DC Pump Control �Լ�
            Hal_EW_Module_Control();                // EW ��� ���� �Լ�
            Hal_LeakSensor_Module_Control();        // ���� ���� ���� ���� �Լ�
            Hal_StirringMotor_Module_Control();     // ���� ���� ���� �Լ�
            Hal_UV_Module_Control();                // UV Control �Լ�
            Hal_Voice_Module_Control();             // Voice ���� �Լ�
            Hal_SmallValve_Module_Control();        // ���� ��� ���� �Լ�
            Hal_Valve_Module_Control();             // �Ϲ� ��� ���� �Լ�
            /* ���� ���� */
            Hal_Heater_Module_Control();            // �Ϲ� ���� ���� �Լ�
            Hal_Inst_Heater_Module_Control();       // ���� �¼� ���� ���� �Լ�
            /* �ð� ���� */
            Hal_BLDC_Comp_Module_Control();         // BLDC Comp ���� �Լ�
            Hal_Comp_Module_Control();              // ������ Comp ���� �Լ�
            Hal_TEM_Module_Control();               // TEM ����
            /* ���� ���� */
            Hal_SwingBar_Module_Control();          // Swing Bar ���� �Լ�
            Hal_AC_Motor_IceMaking_Module_Control();    // ���� ���� AC ���� ���� �Լ�
            Hal_GasSwitch_Module_Control();         // Gas Switch ���� �Լ�
            Hal_IceDoor_Module_Control();           // Ice Door ���� �Լ�
            Hal_IceTray_Module_Control();           // Ice Tray ���� �Լ�

            // * Lib (Function) ***************************************************************************
            Lib_ADC_Module_Control();               // ADC ���� ����
            Lib_BoostPump_Module_Control();         // �ν�Ʈ ���� ���� ����
            Lib_PowerSave_Module_Control();         // �̻�� ����, ���� �Ǵ� ���� ����
            Lib_TimeScheduler_Module_Control();     // Time Scheduler ���� ����
            Lib_UV_Sterilize_Module_Control();      // UV ��� ���� ����
            Lib_WaterError_Module_Control();        // ������ ���� üũ ���� ����
            Lib_WaterLevel_Module_Control();        // ���� ���� ���� ����
            Lib_WaterInput_Module_Control();        // ��ũ �Լ� ���� ����
            Lib_Effluent_Module_Control();          // ���� ���� ����
            Lib_PTA_Mode_Module_Control();          // PTA �˻� ����
            /* ���� ���� */
            Lib_Heater_Module_Control();            // ���� ����
            Lib_InstHeater_Module_Control();        // ���� �¼� ����
            /* �ð� ���� */
            Lib_NormalComp_Module_Control();        // ������ Comp ����
            Lib_BLDC_Comp_Module_Control();         // BLDC Comp ����
            Lib_TEM_Module_Control();               // TEM ����
            Lib_CoolFan_Module_Control();           // �ð� FAN ����
            /* ���� ���� */
            Lib_IceMaking_ModeDecision_Module_Control();    // �ð�, ���� ��� �Ǵ� ����
            Lib_IceMaking_Module_Control();         // ���� ���� ����

            // * ��Ÿ **************************************************************************************
            break;

        case OPERATION_MODE_ID_PCB_TEST:        // PCB �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_FRONT_TEST:      // Front �׽�Ʈ ���
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:      // PTA �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:      // PTA �׽�Ʈ ���2
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:      // PTA �׽�Ʈ ���3
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:    // ���� �׽�Ʈ ���1
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:    // ���� �׽�Ʈ ���2
            break;

        default:
            break;

    }
}



/// @brief      ���� ���� ���� ���� �ʱ�ȭ
/// @details    ���� ���۰� ���õ� �������� ��� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void Boot_Initialize(void)
{
    gu16BootTime = BOOT_TIME;
    gu8BootComplete = CLEAR;
}


/// @brief      ���� ���� �ð� Ÿ�̸� ī��Ʈ(@1ms)
/// @details    ���� ������ �ϴ� �ð��� ī��Ʈ �Ѵ�
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


/// @brief      ��ǰ ���� �ð� �ܿ� Ȯ��
/// @details    ��ǰ ���� �� �����ð� ���� ������ �����ϴ� �ð� �ܿ� Ȯ��
/// @param      void
/// @return     return : ���� ���� �ܿ� �ð�(100ms ����)
U16 Get_BootTime(void)
{
    return  gu16BootTime;
}


/// @brief      ��ǰ ������ �Ϸ�Ǿ����� Ȯ���ϴ� �Լ�
/// @details    ��ǰ ���� �� �����ð� ���� ������ �Ϸ��Ͽ����� üũ�Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� �Ϸ� ���� - 0(���� ��), 1(���� �Ϸ�)
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


// User�� �ڽŰ� ���õ� Timer, Interrupt �� Start �Լ��� ������ �� ����������������������������������������������

/// @brief      ��� ���ͷ�Ʈ(Timer ��) ���� �Լ�
/// @details    ��� ����ϴ� ���ͷ�Ʈ���� ���۵� �� �ֵ��� Start �Լ����� �߰��Ͽ� ���ͷ�Ʈ���� ���� ���۽�Ų��
/// @param      void
/// @return     void
void Interrupt_Start(void)
{
    Timer_1ms_Start();
    Timer_100us_Start();
    ADC_CheckStart(0);
}


/// @brief      ��� ��� ���� �Լ�
/// @details    ��� ��� ���� SFR�� ���� ��ų �� �ֵ��� Start �Լ����� �߰��Ͽ� ��ŵ��� ���� ���۽�Ų��.
/// @param      void
/// @return     void
void Serial_Start(void)
{
    R_Config_UART0_Start();         // UART0 Start
    R_Config_UART2_Start();         // UART2 Start
//    BLDC_COMP_UART_START();
}



