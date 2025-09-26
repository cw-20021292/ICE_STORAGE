/// @file     Lib_Water_LevelSensor.c
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    수위 체크 동작 관련 File

#include "Global_Header.h"

#if (LEVEL_SENSOR_COUNT > 0)

U8 gu8FirstHighSensorDetect = 0;            /// @brief  전원 인가 후 최초 만수위 감지 여부 : 0(미감지), 1(감지)
U8 gu8FirstLowSensorDetect = 0;             /// @brief  전원 인가 후 최초 저수위 감지 여부 : 0(미감지), 1(감지)

#if (COLD_LOW_SENSOR_USE == USE)            // 냉수 저수위 센서 사용시
U8 gu8ColdWaterLevel = 0;                   /// @brief   확정 냉수 수위 상태
#endif

U8 gu8WaterLevel = 0;                       /// @brief   확정 정수 수위 상태

#if (HOT_HIGH_SENSOR_USE == USE)
U8 gu8HotWaterLevel = 0;                    /// @brief   확정 온수 수위 상태
#endif


/// @brief      수위 감지 체크 관련 변수 초기화
/// @details    수위 감지와 관련된 모든 변수를 초기화 한다
/// @param      void
/// @return     void
void Lib_WaterLevel_Initialize(void)
{
    gu8FirstHighSensorDetect = CLEAR;       /// @brief  전원 인가 후 최초 만수위 감지 여부 : 0(미감지), 1(감지)
    gu8FirstLowSensorDetect = CLEAR;        /// @brief  전원 인가 후 최초 저수위 감지 여부 : 0(미감지), 1(감지)

#if (COLD_LOW_SENSOR_USE == USE)            // 냉수 저수위 센서 사용시
    gu8ColdWaterLevel = 0;                  /// @brief   확정 냉수 수위 상태
#endif

    gu8WaterLevel = 0;                      /// @brief   확정 정수 수위 상태

#if (HOT_HIGH_SENSOR_USE == USE)
    gu8HotWaterLevel = 0;                   /// @brief   확정 온수 수위 상태
#endif
}


/// @brief      수위 상태 체크 함수(@while)
/// @details    수위 상태를 체크하여 현재의 탱크 수위를 확정한다
/// @param      void
/// @return     void
void Lib_WaterLevelCheck(void)
{
    U8 mu8BootComplete = 0;

#if (COLD_LOW_SENSOR_USE == USE)        // 냉수 저수위 센서 사용시
    U8 mu8ColdLowSensor = 0;
#endif

    U8 mu8LowSensor = 0;
#if (MIDDLE_SENSOR_USE == USE)          // 중수위 센서 사용시
    U8 mu8MiddleSensor = 0;
#endif
    U8 mu8HightSensor = 0;
    U8 mu8OverFlowSensor = 0;

#if (HOT_HIGH_SENSOR_USE == USE)        // 온수 만수위 센서 사용시
    U8 mu8HotHighSensor = 0;
#endif

    mu8BootComplete = Get_BootComplete();   // 부팅 동작이 완료되었는지 확인
    if (mu8BootComplete == SET)
    {   // 부팅 동작이 완료되었으면
#if (COLD_LOW_SENSOR_USE == USE)        // 냉수 저수위 센서 사용시
        // 냉수 수위 확정
        mu8ColdLowSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_LOW);

        if (mu8ColdLowSensor == HIGH)
        {   // 냉수 저수위 센서 감지시
            gu8ColdWaterLevel = COLD_WATER_LEVEL_MIDDLE;
        }
        else
        {   // 냉수 저수위 센서 미감지시
            gu8ColdWaterLevel = COLD_WATER_LEVEL_LOW;
        }
#endif

        // 정수 수위 확정
#if (MIDDLE_SENSOR_USE == USE)          // 중수위 센서 사용시
        mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);
        mu8MiddleSensor = Get_WaterLevel_Status(LEVEL_ID_MODDLE);
        mu8HightSensor = Get_WaterLevel_Status(LEVEL_ID_HIGH);
        mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

        if (mu8OverFlowSensor || mu8HightSensor)
        {   // OverFlow나 만수위 센서 감지시
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
        {   // 중수위 센서 감지시
#if (BOOST_PUMP_USE == USE)
            if ( (gu8WaterLevel == WATER_LEVEL_MIDDLE_LOW) || (gu8WaterLevel == WATER_LEVEL_LOW) )
            {   // 저수위 또는 중저수위에서 수위 변화가 발생한 경우
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
        {   // 저수위 센서 감지시
#if (BOOST_PUMP_USE == USE)
            if (gu8WaterLevel == WATER_LEVEL_LOW)
            {   // 저수위에서 수위 변화가 발생한 경우
                Set_BoostPumpControlChangeStep(BOOST_CONTROL_STEP1);
            }
#endif
            gu8WaterLevel = WATER_LEVEL_MIDDLE_LOW;
#if (COLD_LOW_SENSOR_USE == NO_USE)        // 냉수 저수위 센서 미사용시
            gu8ColdWaterLevel = COLD_WATER_LEVEL_HIGH;
#endif
#if (HOT_HIGH_SENSOR_USE == NO_USE)        // 온수 만수위 센서 미사용시
            gu8HotWaterLevel = HOT_WATER_LEVEL_HIGH;
#endif

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else
        {   // 모든 수위 센서 미감지시
            gu8WaterLevel = WATER_LEVEL_LOW;
        }
#else
        mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);
        mu8HightSensor = Get_WaterLevel_Status(LEVEL_ID_HIGH);
        mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

        if (mu8OverFlowSensor || mu8HightSensor)
        {   // OverFlow나 만수위 센서 감지시
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
        {   // 저수위 센서 감지시
#if (BOOST_PUMP_USE == USE)
            if (gu8WaterLevel == WATER_LEVEL_LOW)
            {   // 저수위에서 수위 변화가 발생한 경우
                Set_BoostPumpControlChangeStep(BOOST_CONTROL_STEP1);
            }
#endif
            gu8WaterLevel = WATER_LEVEL_MIDDLE;
#if (COLD_LOW_SENSOR_USE == NO_USE)        // 냉수 저수위 센서 미사용시
            gu8ColdWaterLevel = COLD_WATER_LEVEL_HIGH;
#endif
#if (HOT_HIGH_SENSOR_USE == NO_USE)        // 온수 만수위 센서 미사용시
            gu8HotWaterLevel = HOT_WATER_LEVEL_HIGH;
#endif

            if (gu8FirstLowSensorDetect == CLEAR)
            {
                gu8FirstLowSensorDetect = SET;
            }
        }
        else
        {   // 모든 수위 센서 미감지시
            gu8WaterLevel = WATER_LEVEL_LOW;
        }

#endif

#if (HOT_HIGH_SENSOR_USE == USE)
        // 온수 수위 체크(온수 탱크 수위는 전극봉 타입 사용)
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


/// @brief      냉수 탱크의 수위 상태 확인 함수
/// @details    냉수 탱크의 수위 상태를 반환한다
/// @param      void
/// @return     return : 냉수 탱크의 수위 상태 - 0(저수위), 1(중수위), 2(만수위)
U8 Get_ColdWaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8ColdWaterLevel;

    return  mu8Return;
}


/// @brief      온수 탱크의 수위 상태 확인 함수
/// @details    온수 탱크의 수위 상태를 반환한다
/// @param      void
/// @return     return : 온수 탱크의 수위 상태 - 0(저수위), 1(중수위), 2(만수위)
U8 Get_HotWaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8HotWaterLevel;

    return  mu8Return;
}


/// @brief      정수 탱크의 수위 상태 확인 함수
/// @details    정수 탱크의 수위 상태를 반환한다
/// @param      void
/// @return     return : 정수 탱크의 수위 상태
///                          - 중수위 있는 경우 0(저수위), 1(중저수위), 2(중고수위), 3(만수위)
///                          - 중수위 없는 경우 0(저수위), 1(중수위), 2(만수위)
U8 Get_WaterLevel(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8WaterLevel;

    return  mu8Return;
}


/// @brief      정수 탱크의 부팅 후 최초 만수위 감지 여부 확인 함수
/// @details    부팅 완료 후 정수 탱크가 최초 만수위를 감지한 이력이 있는지 체크하여 그 값을 반환한다
/// @param      void
/// @return     return : 최초 만수위 감지 여부 - 0(미감지), 1(감지)
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


/// @brief      정수 탱크의 부팅 후 최초 저수위 감지 여부 확인 함수
/// @details    부팅 완료 후 정수 탱크가 최초 저수위를 감지한 이력이 있는지 체크하여 그 값을 반환한다
/// @param      void
/// @return     return : 최초 저수위 감지 여부 - 0(미감지), 1(감지)
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

#endif      // 수위 센서가 있는 경우 END


// Lib Water Level Module *********************************************************************************

/// @brief      Lib 수위 감지 Module Initilaize
/// @details    수위 감지 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_WaterLevel_Module_Initialize(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterLevel_Initialize();
#endif
}


/// @brief      Lib 수위 감지 Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 수위 감지 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_WaterLevel_Module_Control(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    Lib_WaterLevelCheck();
#endif
}


