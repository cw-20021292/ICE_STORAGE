/// @file     Lib_Water_TankWaterInput.c
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    ����ũ �Լ� ���� ���� File

#include "Global_Header.h"

#if (LEVEL_SENSOR_COUNT > 0)


/// @brief      ����ũ �޼� ���� �Լ�
/// @details    ����ũ �޼� ������ �����Ѵ�
/// @param      void
/// @return     void
void Lib_WaterInputControl(void)
{
    U8 mu8Error = 0;
    U8 mu8WaterLevel = 0;

    mu8Error = Get_ErrorAboutWaterInput();      // �Լ� ���� ���� �߻� üũ
    mu8WaterLevel = Get_WaterLevel();

    // ���� ����ũ �Լ�
    if (mu8Error == FALSE)
    {     // �Լ� �Ұ� ���� ���� �̹߻���
        Set_ValveControl(VALVE_NOS_ID_WATER_IN, NOS, OFF, 0);

        // �׻� ���� �� ��ũ ���� �ְ���� ����
        if (mu8WaterLevel < WATER_LEVEL_HIGH)
        {   // �������� �ƴ� ���
            Set_ValveControl(VALVE_ID_WATER_IN, FEED, ON, 0);
        }
        else
        {   // �������� ���
            Set_ValveControl(VALVE_ID_WATER_IN, FEED, OFF, 0);
        }
  }
  else
  {  // �Լ� �Ұ� ���� ���� �߻���
    Set_ValveControl(VALVE_NOS_ID_WATER_IN, NOS, ON, 0);
    Set_ValveControl(VALVE_ID_WATER_IN, FEED, OFF, 0);
  }
}

#endif


// Lib Tank Water Input Module *********************************************************************************

/// @brief      Lib ��ũ �Լ� ���� Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� ��ũ �Լ� ���� ��� Library
/// @param      void
/// @return     void
void Lib_WaterInput_Module_Control(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterInputControl();
#endif
}

