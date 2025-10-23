/// @file     Lib_Water_LevelSensor.c
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    ���� üũ ���� ���� File

#include "Global_Header.h"

#if (LEVEL_SENSOR_COUNT > 0)

U8 gu8FirstHighSensorDetect = 0;            /// @brief  ���� �ΰ� �� ���� ������ ���� ���� : 0(�̰���), 1(����)
U8 gu8FirstLowSensorDetect = 0;             /// @brief  ���� �ΰ� �� ���� ������ ���� ���� : 0(�̰���), 1(����)

#if (COLD_LOW_SENSOR_USE == USE)            // �ü� ������ ���� ����
U8 gu8ColdWaterLevel = 0;                   /// @brief   Ȯ�� �ü� ���� ����
#endif

U8 gu8WaterLevel = 0;                       /// @brief   Ȯ�� ���� ���� ����

#if (HOT_HIGH_SENSOR_USE == USE)
U8 gu8HotWaterLevel = 0;                    /// @brief   Ȯ�� �¼� ���� ����
#endif


/// @brief      ���� ���� üũ ���� ���� �ʱ�ȭ
/// @details    ���� ������ ���õ� ��� ������ �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void Lib_WaterLevel_Initialize(void)
{
    gu8FirstHighSensorDetect = CLEAR;       /// @brief  ���� �ΰ� �� ���� ������ ���� ���� : 0(�̰���), 1(����)
    gu8FirstLowSensorDetect = CLEAR;        /// @brief  ���� �ΰ� �� ���� ������ ���� ���� : 0(�̰���), 1(����)

#if (COLD_LOW_SENSOR_USE == USE)            // �ü� ������ ���� ����
    gu8ColdWaterLevel = 0;                  /// @brief   Ȯ�� �ü� ���� ����
#endif

    gu8WaterLevel = 0;                      /// @brief   Ȯ�� ���� ���� ����

#if (HOT_HIGH_SENSOR_USE == USE)
    gu8HotWaterLevel = 0;                   /// @brief   Ȯ�� �¼� ���� ����
#endif
}


/// @brief      ���� ���� üũ �Լ�(@while)
/// @details    ���� ���¸� üũ�Ͽ� ������ ��ũ ������ Ȯ���Ѵ�
/// @param      void
/// @return     void
void Lib_WaterLevelCheck(void)
{
    U8 mu8BootComplete = 0;

#if (COLD_LOW_SENSOR_USE == USE)        // �ü� ������ ���� ����
    U8 mu8ColdLowSensor = 0;
#endif

    U8 mu8LowSensor = 0;
#if (MIDDLE_SENSOR_USE == USE)          // �߼��� ���� ����
    U8 mu8MiddleSensor = 0;
#endif
    U8 mu8HightSensor = 0;
    U8 mu8OverFlowSensor = 0;

#if (HOT_HIGH_SENSOR_USE == USE)        // �¼� ������ ���� ����
    U8 mu8HotHighSensor = 0;
#endif

    mu8BootComplete = Get_BootComplete();   // ���� ������ �Ϸ�Ǿ����� Ȯ��
    if (mu8BootComplete == SET)
    {   // ���� ������ �Ϸ�Ǿ�����
#if (COLD_LOW_SENSOR_USE == USE)        // �ü� ������ ���� ����
        // �ü� ���� Ȯ��
        mu8ColdLowSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_LOW);

        if (mu8ColdLowSensor == HIGH)
        {   // �ü� ������ ���� ������
            gu8ColdWaterLevel = COLD_WATER_LEVEL_MIDDLE;
        }
        else
        {   // �ü� ������ ���� �̰�����
            gu8ColdWaterLevel = COLD_WATER_LEVEL_LOW;
        }
#endif

        // ���� ���� Ȯ��
#if (MIDDLE_SENSOR_USE == USE)          // �߼��� ���� ����
        mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);
        mu8MiddleSensor = Get_WaterLevel_Status(LEVEL_ID_MODDLE);
        mu8HightSensor = Get_WaterLevel_Status(LEVEL_ID_HIGH);
        mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

        if (mu8OverFlowSensor || mu8HightSensor)
        {   // OverFlow�� ������ ���� ������
            gu8WaterLevel = WATER_LEVEL_HIGH;

            if (gu8FirstHighSensorDetect == CLEAR)
            {
                gu8FirstHighSensorDetect = SET;
            }

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else if (mu8MiddleSensor)
        {   // �߼��� ���� ������
#if (BOOST_PUMP_USE == USE)
            if ( (gu8WaterLevel == WATER_LEVEL_MIDDLE_LOW) || (gu8WaterLevel == WATER_LEVEL_LOW) )
            {   // ������ �Ǵ� ������������ ���� ��ȭ�� �߻��� ���
                Set_BoostPumpControlChangeStep(BOOST_CONTROL_STEP1);
            }
#endif
            gu8WaterLevel = WATER_LEVEL_MIDDLE_HIGH;

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else if (mu8LowSensor)
        {   // ������ ���� ������
#if (BOOST_PUMP_USE == USE)
            if (gu8WaterLevel == WATER_LEVEL_LOW)
            {   // ���������� ���� ��ȭ�� �߻��� ���
                Set_BoostPumpControlChangeStep(BOOST_CONTROL_STEP1);
            }
#endif
            gu8WaterLevel = WATER_LEVEL_MIDDLE_LOW;
#if (COLD_LOW_SENSOR_USE == NO_USE)        // �ü� ������ ���� �̻���
            gu8ColdWaterLevel = COLD_WATER_LEVEL_HIGH;
#endif
#if (HOT_HIGH_SENSOR_USE == NO_USE)        // �¼� ������ ���� �̻���
            gu8HotWaterLevel = HOT_WATER_LEVEL_HIGH;
#endif

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else
        {   // ��� ���� ���� �̰�����
            gu8WaterLevel = WATER_LEVEL_LOW;
        }
#else
        mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);
        mu8HightSensor = Get_WaterLevel_Status(LEVEL_ID_HIGH);
        mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

        if (mu8OverFlowSensor || mu8HightSensor)
        {   // OverFlow�� ������ ���� ������
            gu8WaterLevel = WATER_LEVEL_HIGH;

            if (gu8FirstHighSensorDetect == CLEAR)
            {
                gu8FirstHighSensorDetect = SET;
            }

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else if (mu8LowSensor)
        {   // ������ ���� ������
#if (BOOST_PUMP_USE == USE)
            if (gu8WaterLevel == WATER_LEVEL_LOW)
            {   // ���������� ���� ��ȭ�� �߻��� ���
                Set_BoostPumpControlChangeStep(BOOST_CONTROL_STEP1);
            }
#endif
            gu8WaterLevel = WATER_LEVEL_MIDDLE;
#if (COLD_LOW_SENSOR_USE == NO_USE)        // �ü� ������ ���� �̻���
            gu8ColdWaterLevel = COLD_WATER_LEVEL_HIGH;
#endif
#if (HOT_HIGH_SENSOR_USE == NO_USE)        // �¼� ������ ���� �̻���
            gu8HotWaterLevel = HOT_WATER_LEVEL_HIGH;
#endif

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else
        {   // ��� ���� ���� �̰�����
            gu8WaterLevel = WATER_LEVEL_LOW;
        }

#endif

#if (HOT_HIGH_SENSOR_USE == USE)
        // �¼� ���� üũ(�¼� ��ũ ������ ���غ� Ÿ�� ���)
        mu8HotHighSensor = Get_ElecLevel_Status();

        if (mu8HotHighSensor == HIGH)
        {
            gu8HotWaterLevel = HOT_WATER_LEVEL_HIGH;
        }
        else
        {
            gu8HotWaterLevel = HOT_WATER_LEVEL_LOW;
        }
    }
#endif
}


/// @brief      �ü� ��ũ�� ���� ���� Ȯ�� �Լ�
/// @details    �ü� ��ũ�� ���� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��ũ�� ���� ���� - 0(������), 1(�߼���), 2(������)
U8 Get_ColdWaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8ColdWaterLevel;

    return  mu8Return;
}


/// @brief      �¼� ��ũ�� ���� ���� Ȯ�� �Լ�
/// @details    �¼� ��ũ�� ���� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �¼� ��ũ�� ���� ���� - 0(������), 1(�߼���), 2(������)
U8 Get_HotWaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8HotWaterLevel;

    return  mu8Return;
}


/// @brief      ���� ��ũ�� ���� ���� Ȯ�� �Լ�
/// @details    ���� ��ũ�� ���� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ��ũ�� ���� ����
///                          - �߼��� �ִ� ��� 0(������), 1(��������), 2(�߰����), 3(������)
///                          - �߼��� ���� ��� 0(������), 1(�߼���), 2(������)
U8 Get_WaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8WaterLevel;

    return  mu8Return;
}


/// @brief      ���� ��ũ�� ���� �� ���� ������ ���� ���� Ȯ�� �Լ�
/// @details    ���� �Ϸ� �� ���� ��ũ�� ���� �������� ������ �̷��� �ִ��� üũ�Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ������ ���� ���� - 0(�̰���), 1(����)
U8 Get_FirstHighLevelCheck(void)
{
    if (gu8FirstHighSensorDetect)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ��ũ�� ���� �� ���� ������ ���� ���� Ȯ�� �Լ�
/// @details    ���� �Ϸ� �� ���� ��ũ�� ���� �������� ������ �̷��� �ִ��� üũ�Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ������ ���� ���� - 0(�̰���), 1(����)
U8 Get_FirstLowLevelCheck(void)
{
    if (gu8FirstLowSensorDetect)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif      // ���� ������ �ִ� ��� END


// Lib Water Level Module *********************************************************************************

/// @brief      Lib ���� ���� Module Initilaize
/// @details    ���� ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_WaterLevel_Module_Initialize(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterLevel_Initialize();
#endif
}


/// @brief      Lib ���� ���� Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� ���� ���� ���� ��� Library
/// @param      void
/// @return     void
void Lib_WaterLevel_Module_Control(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterLevelCheck();
#endif
}


