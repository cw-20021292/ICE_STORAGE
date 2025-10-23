/// @file     Hal_GeneralADC.c
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    ���� ADC Control file


#include "Global_Header.h"


#if (ADC_USE == USE)                            // ADC ���ϰ� 1���� �ִ� ���

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
U8 gu8CoolTH_ControlTimer = 0;                  /// @brief  �ü� �µ� ���� ���� ���� Ÿ�̸� ī����
U16 gu16Cool1TH_PowerControlTimer = 0;          /// @brief  �ü� �µ� ���� 1 ���� ���� Ÿ�̸�
#if (COOL_THERMISTOR_COUNT == 2)
U16 gu16Cool2TH_PowerControlTimer = 0;          /// @brief  �ü� �µ� ���� 2 ���� ���� Ÿ�̸�
#endif
U16 gu16BootCoolTempCheckTimer = 0;             /// @brief  �ü� �µ� ���� ���� ���� ���� ���� Ÿ�̸�
U8 gu8CoolTH_PowerControlMode = 0;              /// @brief  �ü� �µ� ���� ���� ���� ���
#endif

/// @brief      ADC ��� ���� ���� ������ �� ���̺�
/// @details    ADC�� ����� ����ϱ� ���� AD���� ������ ���Ͽ� ���´�
U8 au8ADCAverageCountSet[26] = { ADC_AVERAGE_COUNT_0,
                                 ADC_AVERAGE_COUNT_1,
                                 ADC_AVERAGE_COUNT_2,
                                 ADC_AVERAGE_COUNT_3,
                                 ADC_AVERAGE_COUNT_4,
                                 ADC_AVERAGE_COUNT_5,
                                 ADC_AVERAGE_COUNT_6,
                                 ADC_AVERAGE_COUNT_7,
                                 ADC_AVERAGE_COUNT_8,
                                 ADC_AVERAGE_COUNT_9,
                                 ADC_AVERAGE_COUNT_10,
                                 ADC_AVERAGE_COUNT_11,
                                 ADC_AVERAGE_COUNT_12,
                                 ADC_AVERAGE_COUNT_13,
                                 ADC_AVERAGE_COUNT_14,
                                 ADC_AVERAGE_COUNT_15,
                                 ADC_AVERAGE_COUNT_16,
                                 ADC_AVERAGE_COUNT_17,
                                 ADC_AVERAGE_COUNT_18,
                                 ADC_AVERAGE_COUNT_19,
                                 ADC_AVERAGE_COUNT_20,
                                 ADC_AVERAGE_COUNT_21,
                                 ADC_AVERAGE_COUNT_22,
                                 ADC_AVERAGE_COUNT_23,
                                 ADC_AVERAGE_COUNT_24,
                                 ADC_AVERAGE_COUNT_25
};


/// @brief      ADC ��� ���� ���� ������ �� ���̺�
/// @details    ADC�� ����� ����ϱ� ���� �迭 ������
typedef struct {
    U16 au16Average[AD_CH_MAX];
    U16 au16Sum[AD_CH_MAX];
    U8  au8Count[AD_CH_MAX];
    U16 au16Max[AD_CH_MAX];
    U16 au16Min[AD_CH_MAX];
    U8 au8CheckStart[AD_CH_MAX];
}   SADC_Data_T;

SADC_Data_T   SADC_Data;


/// @brief      ADC Data Initialize
/// @details    ADC�� ���õ� ��� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_ADC_Initialize(void)
{
    U8 mu8ADC_Buf = 0;

    MEMSET( (void __FAR *)&SADC_Data, 0x00, sizeof(SADC_Data_T) );

    // AD min�� �ʱ�ȭ
    for (mu8ADC_Buf = 0 ; mu8ADC_Buf < AD_CH_MAX ; mu8ADC_Buf++)
    {
        SADC_Data.au8CheckStart[mu8ADC_Buf] = SET;        // ADC Start ���� 1�� ���� - 0(Stop), 1(Start)
        SADC_Data.au16Min[mu8ADC_Buf] = ADC_DATA_MAX;
    }
}


/// @brief      AD�� ��� Ȯ�� �Լ�
/// @details    ID�� �ش��ϴ� ADC �������� ��հ��� ��ȯ�Ѵ�
/// @param      mu8Channel : üũ ��û�ϴ� ADC Channel��
/// @return     return : ��û�� Thermistor ID�� AD�� ��հ�
U16 Get_ADC_Data(U8 mu8Channel)
{
    U16 mu16Return = 0;

    mu16Return = SADC_Data.au16Average[mu8Channel];

    return  mu16Return;
}


/// @brief      ADC Check Start Set function
/// @details    ä���� AD Convert ��� ��� ���� ������ �����Ѵ�
/// @param      mu8Channel : üũ�ϴ� ADC channel��
///             mu8Start : 1(Check Start), 0(Check Stop)
/// @return     void
void Set_Hal_ADC_Check(U8 mu8Channel, U8 mu8Start)
{
    SADC_Data.au8CheckStart[mu8Channel] = mu8Start;
}


/// @brief      ADC All Check Start Set function
/// @details    ��� ä���� AD Convert ��� ��� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void Set_Hal_All_ADC_CheckStart(void)
{
    U8 mu8ADC_Buf = 0;

    for (mu8ADC_Buf = 0 ; mu8ADC_Buf < AD_CH_MAX ; mu8ADC_Buf++)
    {
        SADC_Data.au8CheckStart[mu8ADC_Buf] = SET;        // ADC Start ���� 1�� ���� - 0(Stop), 1(Start)
    }
}


/// @brief      AD Convert ��� ��� �Լ�
/// @details    AD������ ����� ����Ѵ�
/// @param      mu8Channel : ��� ����� AD Channel
///             mu16AD_Value : AD ��
/// @return     void
void Average_ADC(U8 mu8Channel, U16 mu16AD_Value)
{
    U8 mu8CountBuf = 0;

    if (SADC_Data.au8CheckStart[mu8Channel] == 1)
    {       // ADC üũ ���� ���� ��쿡�� ��� ���
        if (mu16AD_Value > SADC_Data.au16Max[mu8Channel])
        {
            SADC_Data.au16Max[mu8Channel] = mu16AD_Value;      // �ִ밪 ����
        }

        if (mu16AD_Value < SADC_Data.au16Min[mu8Channel])
        {
            SADC_Data.au16Min[mu8Channel] = mu16AD_Value;      // �ּҰ� ����
        }

        SADC_Data.au16Sum[mu8Channel] += mu16AD_Value;

        mu8CountBuf = au8ADCAverageCountSet[mu8Channel] + 2;

        SADC_Data.au8Count[mu8Channel]++;

        if (SADC_Data.au8Count[mu8Channel] >= mu8CountBuf)
        {     // �ִ�, �ּҰ��� ������ 10ȸ ��հ� ���
            SADC_Data.au16Average[mu8Channel] = (U16)( ( SADC_Data.au16Sum[mu8Channel] - (SADC_Data.au16Max[mu8Channel] + SADC_Data.au16Min[mu8Channel]) ) / au8ADCAverageCountSet[mu8Channel] );

            SADC_Data.au16Sum[mu8Channel] = 0;
            SADC_Data.au8Count[mu8Channel] = 0;
            SADC_Data.au16Max[mu8Channel] = 0;
            SADC_Data.au16Min[mu8Channel] = ADC_DATA_MAX;
        }
    }
    else
    {       // ADC üũ ���� ���� �ƴ� ���
        SADC_Data.au16Sum[mu8Channel] = 0;
        SADC_Data.au8Count[mu8Channel] = 0;
        SADC_Data.au16Max[mu8Channel] = 0;
        SADC_Data.au16Min[mu8Channel] = ADC_DATA_MAX;
    }
}


#if (COOL_THERMISTOR_POWER_CONTROL == USE)        // �ü� �µ� ���� ���� ���� �����

/// @brief      Cool Thermistor Power Control Initialize
/// @details    �ü� �µ� ���� ���� ����� ���õ� �������� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void CoolTH_ControlInitialize(void)
{
        gu8CoolTH_ControlTimer = 0;
        gu16Cool1TH_PowerControlTimer = 0;
#if (COOL_THERMISTOR_COUNT == 2)
        gu16Cool2TH_PowerControlTimer = 0;
#endif
        gu16BootCoolTempCheckTimer = 6000;                      // ���� �ΰ� �� �µ� ���� ���� Ÿ�̸� 10��@100ms
        gu8CoolTH_PowerControlMode = COOL_TH_NORMAL_MODE;       // �ü� �µ� ���� ���� ���� �Ϲ� ���� ����
}


/// @brief      Cool Thermistor Power Control Timer(@1ms interrupt)
/// @details    1ms ���ͷ�Ʈ ��ƾ �ȿ��� �ü� �µ� ���� ���� ���� Ÿ�̸Ӹ� ī�����Ѵ�
/// @param      void
/// @return     void
void CoolTH_ControlTimer(void)
{
    if (gu8CoolTH_ControlTimer < COOL_TH_POWER_CONTROL_TIME_PERIOD)
    {
        gu8CoolTH_ControlTimer++;
    }
}


/// @brief      Cool Thermistor Power Control ��� ���� �Լ�
/// @details    �ü� �µ� ���� ���� ����� ���õ� ���� ��带 �����Ѵ�
/// @param      mu8Mode : 0(Normal), 1(Test Mode)
/// @return     void
void Set_CoolTH_PowerControl(U8 mu8Mode)
{
    gu8CoolTH_PowerControlMode = mu8Mode;
}


/// @brief      Cool Thermistor 1 Control �Լ�(100ms@1ms)
/// @details    �ü� �µ� ���� 1 ������ ��忡 ���� 100ms �ֱ�� ������ �����Ѵ�
/// @param      mu8ControlMode : ���� ������ ���
/// @return     void
void Cool1TH_Control(U8 mu8ControlMode)
{
    U16 mu16TempADBuff = 0;

    if (mu8ControlMode == TH_OFF_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 0;
        P_COOL1_TH_ON = OFF;
    }
    else if (mu8ControlMode == TH_TEST_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 10;      // AD�� Read �� delay ���� �ٷ� �����ǵ���
        P_COOL1_TH_ON = ON;
    }
    else if (mu8ControlMode == TH_ERROR_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 10;      // AD�� Read �� delay ���� �ٷ� �����ǵ���
        P_COOL1_TH_ON = ON;
    }
    else
    {
        mu16TempADBuff = Get_Temp(ADC_ID_TH_COOL);

        if (mu16TempADBuff <= COOL1_TARGET_TEMP)
        {   // �ü� �µ��� ��ǥ �µ�  ���ϰ� �� ��� ���� ���� �׽� ON
            if (gu16Cool1TH_PowerControlTimer <= 10)
            {   // �µ� ���� �� 1�� Delay
                gu16Cool1TH_PowerControlTimer++;
            }

            P_COOL1_TH_ON = ON;
        }
        else
        {  // �ü� �µ��� ��ǥ �µ� ���� ���� ���
            gu16Cool1TH_PowerControlTimer++;

            if (gu16Cool1TH_PowerControlTimer >= 400)
            {   // 40�� �̻��(40�� �ֱ�) Thermistor ���� ���� �ʱ�ȭ
                gu16Cool1TH_PowerControlTimer = 0;
                P_COOL1_TH_ON = OFF;
            }
            else if (gu16Cool1TH_PowerControlTimer >= 100)
            {   // 10�� ~ 40�� ����(30�ʰ�)�� Thermistor ���� ����
                P_COOL1_TH_ON = OFF;
            }
            else
            {   // 10�ʰ� Thermistor ���� �ΰ�
                P_COOL1_TH_ON = ON;
            }
        }
    }
}


#if (COOL_THERMISTOR_COUNT == 2)
/// @brief      Cool Thermistor 2 Control �Լ�(100ms@1ms)
/// @details    �ü� �µ� ���� 2 ������ ��忡 ���� 100ms �ֱ�� ������ �����Ѵ�
/// @param      mu8ControlMode : ���� ������ ���
/// @return     void
void Cool2TH_Control(U8 mu8ControlMode)
{
    U16 mu16TempADBuff = 0;

    if (mu8ControlMode == TH_OFF_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 0;
        P_COOL2_TH_ON = OFF;
    }
    else if (mu8ControlMode == TH_TEST_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 10;      // AD�� Read �� delay ���� �ٷ� �����ǵ���
        P_COOL2_TH_ON = ON;
    }
    else if (mu8ControlMode == TH_ERROR_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 10;      // AD�� Read �� delay ���� �ٷ� �����ǵ���
        P_COOL2_TH_ON = ON;
    }
    else
    {
        mu16TempADBuff = Get_Temp(ADC_ID_TH_COOL);

        if (mu16TempADBuff <= COOL2_TARGET_TEMP)
        {   // �ü� �µ��� ��ǥ �µ�  ���ϰ� �� ��� ���� ���� �׽� ON
            if (gu16Cool2TH_PowerControlTimer <= 10)
            {   // �µ� ���� �� 1�� Delay
                gu16Cool2TH_PowerControlTimer++;
            }

            P_COOL2_TH_ON = ON;
        }
        else
        {  // �ü� �µ��� ��ǥ �µ� ���� ���� ���
            gu16Cool2TH_PowerControlTimer++;

            if (gu16Cool2TH_PowerControlTimer >= 400)
            {   // 40�� �̻��(40�� �ֱ�) Thermistor ���� ���� �ʱ�ȭ
                gu16Cool2TH_PowerControlTimer = 0;
                P_COOL2_TH_ON = OFF;
            }
            else if (gu16Cool2TH_PowerControlTimer >= 100)
            {   // 10�� ~ 40�� ����(30�ʰ�)�� Thermistor ���� ����
                P_COOL2_TH_ON = OFF;
            }
            else
            {   // 10�ʰ� Thermistor ���� �ΰ�
                P_COOL2_TH_ON = ON;
            }
        }
    }
}

#endif


/// @brief      Cool Thermistor ADC üũ ���� ���� ���� �Լ�(@100ms)
/// @details    �ü� �µ� ������ ���� �ΰ� �� ADC Read�� �� Ÿ�̹��� �����Ѵ�
/// @param      void
/// @return     void
void CoolTH_ADC_CheckStartControl(void)
{
    if (P_COOL1_TH_ON)
    {   // �ü� �µ� ������ ������ �ΰ��� ���
        if ( (gu16BootCoolTempCheckTimer > 0) ||
             ( (gu16Cool1TH_PowerControlTimer >= 10) && (gu16Cool1TH_PowerControlTimer < 100) ) )
        {   // ������ ���� �ð� ����, �� ���Ŀ��� �µ� ������ ������ �ΰ��ǰ� 1�ʰ� ����� �ں��� ����
            Set_Hal_ADC_Check(ADC_ID_TH_COOL, 1);
        }
        else
        {
            Set_Hal_ADC_Check(ADC_ID_TH_COOL, 0);
        }
    }
    else
    {
        Set_Hal_ADC_Check(ADC_ID_TH_COOL, 0);
    }
#if (COOL_THERMISTOR_COUNT == 2)
    if (P_COOL2_TH_ON)
    {   // �ü� �µ� ������ ������ �ΰ��� ���
        if ( (gu16BootCoolTempCheckTimer > 0) ||
             ( (gu16Cool1TH_PowerControlTimer >= 10) && (gu16Cool1TH_PowerControlTimer < 100) ) )
        {   // ������ ���� �ð� ����, �� ���Ŀ��� �µ� ������ ������ �ΰ��ǰ� 1�ʰ� ����� �ں��� ����
            Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 1);
        }
        else
        {
            Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 0);
        }
    }
    else
    {
        Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 0);
    }
#endif
}


/// @brief      Cool Thermistor Power Control �Լ�(100ms@1ms)
/// @details    100ms �ֱ�� ������ �ü� �µ� ���� ���� ���� ��忡 ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void CoolTH_PowerControl(void)
{
    if (gu8CoolTH_ControlTimer >= COOL_TH_POWER_CONTROL_TIME_PERIOD)
    {
        gu8CoolTH_ControlTimer = 0;

        if (gu8CoolTH_PowerControlMode == COOL_TH_OFF_MODE)
        {   // �ü� �µ� ���� ���� ���� OFF��
            Cool1TH_Control(TH_OFF_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_OFF_MODE);
#endif
        }
        if (gu8CoolTH_PowerControlMode == COOL_TH_TEST_MODE)
        {   // �׽�Ʈ ��� ���۽�
            Cool1TH_Control(TH_TEST_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_TEST_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH_ALL_ERROR_CHECK_MODE)
        {   // �ü� �µ� ���� ���� ��� �߻���
            Cool1TH_Control(TH_ERROR_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_ERROR_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH1_ERROR_CHECK_MODE)
        {   // �ü� �µ� ���� 1 ���� ���� ���� ��
            Cool1TH_Control(TH_ERROR_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_NORMAL_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH2_ERROR_CHECK_MODE)
        {   // �ü� �µ� ���� 1 ���� ���� ���� ��
            Cool1TH_Control(TH_NORMAL_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_ERROR_MODE);
#endif
        }
        else
        {   // �Ϲ� ��� ����
            if (gu16BootCoolTempCheckTimer)
            {   // ���� �� ���� �ð� ������ ������ �ü� �µ� ���� ���� �ΰ�
                gu16BootCoolTempCheckTimer--;

                gu16Cool1TH_PowerControlTimer = 0;
                P_COOL1_TH_ON = ON;

#if (COOL_THERMISTOR_COUNT == 2)
                gu16Cool2TH_PowerControlTimer = 0;
                P_COOL2_TH_ON = ON;
#endif
            }
            else
            {   // ���� �� ���� �ð� ��� ���� �µ� ���� ���� �ΰ� ����
                Cool1TH_Control(TH_NORMAL_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
                Cool2TH_Control(TH_NORMAL_MODE);
#endif
            }
        }
    }

    CoolTH_ADC_CheckStartControl();
}

#endif      // if - �ü� �µ� ���� ���� ���� ���۽�


#endif


// Hal ADC Module ********************************************************************************************

/// @brief      Hal ADC Initilaize Module
/// @details    Hal ADC ���� �����͸� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_ADC_Module_Initialize(void)
{
#if (ADC_USE == USE)
    Hal_ADC_Initialize();                   // ADC ������ ��� �ʱ�ȭ
#endif
}



// Hal Cold Temp Sensor Module *******************************************************************************

/// @brief      Hal Cold Thermistor Initilaize Module
/// @details    �ü� �µ� ���� ���� ����� ���õ� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_ColdTH_Module_Initialize(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
    CoolTH_ControlInitialize();             // �ü� �µ� ���� ���� ���� ���� �ʱ�ȭ
#endif

#endif
}


/// @brief      Hal Cold Thermistor Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� �ü� �µ� ���� ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_ColdTH_Module_1ms_Control(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
    CoolTH_ControlTimer();                  // �ü� �µ� ���� ���� ���� �ð� Counter
#endif

#endif
}


/// @brief      Hal Cold Thermistor Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� �ü� �µ� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_ColdTH_Module_Control(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
        CoolTH_PowerControl();                  // �ü� �µ� ���� ���� ���� �Լ�
#endif

#endif
}

