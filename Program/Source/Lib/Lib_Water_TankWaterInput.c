/// @file     Lib_Water_TankWaterInput.c
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    물탱크 입수 동작 관련 File

#include "Global_Header.h"

#if (LEVEL_SENSOR_COUNT > 0)


/// @brief      물탱크 급수 동작 함수
/// @details    물탱크 급수 동작을 제어한다
/// @param      void
/// @return     void
void Lib_WaterInputControl(void)
{
    U8 mu8Error = 0;
    U8 mu8WaterLevel = 0;

    mu8Error = Get_ErrorAboutWaterInput();      // 입수 관련 에러 발생 체크
    mu8WaterLevel = Get_WaterLevel();

    // 정수 물탱크 입수
    if (mu8Error == FALSE)
    {     // 입수 불가 조건 에러 미발생시
        Set_ValveControl(VALVE_NOS_ID_WATER_IN, NOS, OFF, 0);

        // 항상 정수 물 탱크 수위 최고수위 유지
        if (mu8WaterLevel < WATER_LEVEL_HIGH)
        {   // 만수위가 아닌 경우
            Set_ValveControl(VALVE_ID_WATER_IN, FEED, ON, 0);
        }
        else
        {   // 만수위인 경우
            Set_ValveControl(VALVE_ID_WATER_IN, FEED, OFF, 0);
        }
  }
  else
  {  // 입수 불가 조건 에러 발생시
    Set_ValveControl(VALVE_NOS_ID_WATER_IN, NOS, ON, 0);
    Set_ValveControl(VALVE_ID_WATER_IN, FEED, OFF, 0);
  }
}

#endif


// Lib Tank Water Input Module *********************************************************************************

/// @brief      Lib 탱크 입수 제어 Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 탱크 입수 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_WaterInput_Module_Control(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterInputControl();
#endif
}

