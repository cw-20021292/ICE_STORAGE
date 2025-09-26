/// @file     Hal_WaterLevelSensor.c
/// @date     2025/03/14
/// @author   Jaejin Ham
/// @brief    Water Level Sensor Control file


#include "Global_Header.h"


#if (LEVEL_SENSOR_COUNT > 0)

U8 gu8LevelSensorControlTimer = 0;              /// @brief  수위 센서 감지 제어 주기 타이머

U16 gu16LevelSensorConfirmValue = 0;            /// @brief  수위 센서 감지 Confirm 상태(Bit 할당)

/// @brief  수위 센서별 감지 chattering 시간 설정
U8 gu16LevelSensorChatteringTimeSet[11] = { 0,
                                            LEVEL_SENSOR_1_CHATTERING_TIME,
                                            LEVEL_SENSOR_2_CHATTERING_TIME,
                                            LEVEL_SENSOR_3_CHATTERING_TIME,
                                            LEVEL_SENSOR_4_CHATTERING_TIME,
                                            LEVEL_SENSOR_5_CHATTERING_TIME,
                                            LEVEL_SENSOR_6_CHATTERING_TIME,
                                            LEVEL_SENSOR_7_CHATTERING_TIME,
                                            LEVEL_SENSOR_8_CHATTERING_TIME,
                                            LEVEL_SENSOR_9_CHATTERING_TIME,
                                            LEVEL_SENSOR_10_CHATTERING_TIME };


/// @brief  Level Sensor 제어 관리 자료형
typedef struct {
    U8  au8PreStatus[LEVEL_SENSOR_ID_MAX_COUNT];            // Level Sensor Previous Status
    U8  au8CurrentStatus[LEVEL_SENSOR_ID_MAX_COUNT];        // Level Sensor Current Status
    U8  au8ChatteringTime[LEVEL_SENSOR_ID_MAX_COUNT];       // Level Sensor Chattering Time
}   SLevelSensorData_T;

SLevelSensorData_T   SLevelSensorControlData;

#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)     // 전극봉 수위 센서 사용시(온수)
U8 gu8ElecLevelPowerControlTimer = 0;           /// @brief  전극봉 수위센서 전원 제어 Timer
U8 gu8ElecLevelCheckDelay = 0;                  /// @brief  전극봉 수위센서 수위 상태 체크 전 Delay
U8 gu8ElecLevelCheckTimer = 0;                  /// @brief  전극봉 수위센서 수위 상태 체크 주기 타이머
U8 gu8ElecLevelPowerDirection = 0;              /// @brief  전극봉 수위센서 전원 제어 방향 변수
U8 gu8ElecLevelDetectTime = 0;                  /// @brief  전극봉 수위센서 감지 시간
U8 gu8ElecLevelUndetectTime = 0;                /// @brief  전극봉 수위센서 미감지 시간
U8 gu8ElecLevelConfirmStatus = 0;               /// @brief  전극봉 수위센서 상태값 - 0(Undetect), 1(Detect)
#endif


/// @brief      Level Sensor Initialize
/// @details    수위 센서 감지와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_LevelSensor_Initialize(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    U8 mu8LevelSensorBuf = 0;

    MEMSET( (void __FAR *)&SLevelSensorControlData, 0x00, sizeof(SLevelSensorData_T) );

    gu8LevelSensorControlTimer = 0;

    gu16LevelSensorConfirmValue = 0;

    // Level Sensor Chattering Time Initialize
    for (mu8LevelSensorBuf = 1 ; mu8LevelSensorBuf < LEVEL_SENSOR_ID_MAX_COUNT ; mu8LevelSensorBuf++)
    {
        Reset_LevelSensorChatteringTime(mu8LevelSensorBuf);
    }
#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
    gu8ElecLevelPowerControlTimer = 0;
    gu8ElecLevelCheckDelay = 0;
    gu8ElecLevelCheckTimer = 0;
    gu8ElecLevelPowerDirection = 0;
    gu8ElecLevelDetectTime = 0;
    gu8ElecLevelUndetectTime = 0;
    gu8ElecLevelConfirmStatus = 0;
#endif
}


/// @brief      Level Sensor Control Timer(@1ms interrupt)
/// @details    수위 센서 감지 제어 주기 시간 등을 카운트 한다
/// @param      void
/// @return     void
void LevelSensor_ControlTimer(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    if (gu8LevelSensorControlTimer < LEVEL_SENSOR_CONTROL_TIME_PERIOD)
    {
        gu8LevelSensorControlTimer++;
    }
#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
    if (gu8ElecLevelPowerControlTimer < ELEC_LEVEL_SENSOR_CONTROL_TIME_PERIOD)
    {
        gu8ElecLevelPowerControlTimer++;
    }

    if (gu8ElecLevelCheckTimer < ELEC_LEVEL_SENSOR_CHECK_TIME_PERIOD)
    {
        gu8ElecLevelCheckTimer++;
    }

    if (gu8ElecLevelCheckDelay)
    {
        gu8ElecLevelCheckDelay--;
    }

#endif
}


#if (LEVEL_SENSOR_COUNT > 0)
/// @brief      Level Sensor Chattering Time Initialize
/// @details    수위 센서 감지 Chattering 시간을 초기화 시킨다
/// @param      mu8LevelID : defined Level Sensor ID
/// @return     void
void Reset_LevelSensorChatteringTime(U8 mu8LevelID)
{
    // Level Sensor Chattering Time Initialize
    SLevelSensorControlData.au8ChatteringTime[mu8LevelID] = gu16LevelSensorChatteringTimeSet[mu8LevelID];
}


/// @brief      Level Sensor HW Read
/// @details    수위 센서의 HW의 Port값을 Read 한다
/// @param      void
/// @return     void
void Hal_Read_LevelSensor(void)
{
#if (LEVEL_SENSOR_COUNT >= 1)
    SLevelSensorControlData.au8CurrentStatus[1] = GET_STATUS_LEVEL_SENSOR_1();
#endif
#if (LEVEL_SENSOR_COUNT >= 2)
    SLevelSensorControlData.au8CurrentStatus[2] = GET_STATUS_LEVEL_SENSOR_2();
#endif
#if (LEVEL_SENSOR_COUNT >= 3)
    SLevelSensorControlData.au8CurrentStatus[3] = GET_STATUS_LEVEL_SENSOR_3();
#endif
#if (LEVEL_SENSOR_COUNT >= 4)
    SLevelSensorControlData.au8CurrentStatus[4] = GET_STATUS_LEVEL_SENSOR_4();
#endif
#if (LEVEL_SENSOR_COUNT >= 5)
    SLevelSensorControlData.au8CurrentStatus[5] = GET_STATUS_LEVEL_SENSOR_5();
#endif
#if (LEVEL_SENSOR_COUNT >= 6)
    SLevelSensorControlData.au8CurrentStatus[6] = GET_STATUS_LEVEL_SENSOR_6();
#endif
#if (LEVEL_SENSOR_COUNT >= 7)
    SLevelSensorControlData.au8CurrentStatus[7] = GET_STATUS_LEVEL_SENSOR_7();
#endif
#if (LEVEL_SENSOR_COUNT >= 8)
    SLevelSensorControlData.au8CurrentStatus[8] = GET_STATUS_LEVEL_SENSOR_8();
#endif
#if (LEVEL_SENSOR_COUNT >= 9)
    SLevelSensorControlData.au8CurrentStatus[9] = GET_STATUS_LEVEL_SENSOR_9();
#endif
#if (LEVEL_SENSOR_COUNT >= 10)
    SLevelSensorControlData.au8CurrentStatus[10] = GET_STATUS_LEVEL_SENSOR_10();
#endif
}


/// @brief      Level Sensor Control Function(@while)
/// @details    수위 센서 감지 제어 동작을 한다
/// @param      void
/// @return     void
void LevelSensorControl(void)
{
    U8 mu8LevelSensorIDBuf = 0;

    if (gu8LevelSensorControlTimer >= LEVEL_SENSOR_CONTROL_TIME_PERIOD)
    {       // 100ms마다 제어
        gu8LevelSensorControlTimer = 0;

        Hal_Read_LevelSensor();

        for (mu8LevelSensorIDBuf = 1 ; mu8LevelSensorIDBuf < LEVEL_SENSOR_ID_MAX_COUNT ; mu8LevelSensorIDBuf++)
        {
            if (SLevelSensorControlData.au8PreStatus[mu8LevelSensorIDBuf] == SLevelSensorControlData.au8CurrentStatus[mu8LevelSensorIDBuf])
            {   // 이전 상태와 현재 상태가 같은 경우
                if (SLevelSensorControlData.au8ChatteringTime[mu8LevelSensorIDBuf])
                {
                    SLevelSensorControlData.au8ChatteringTime[mu8LevelSensorIDBuf]--;

                    if (SLevelSensorControlData.au8ChatteringTime[mu8LevelSensorIDBuf] == 0)
                    {
                        ConfirmLevelSensorData(mu8LevelSensorIDBuf, SLevelSensorControlData.au8CurrentStatus[mu8LevelSensorIDBuf]);
                    }
                }
                else
                {
                    ConfirmLevelSensorData(mu8LevelSensorIDBuf, SLevelSensorControlData.au8CurrentStatus[mu8LevelSensorIDBuf]);
                }
            }
            else
            {   // 이전 상태와 현재 상태가 다른 경우
                SLevelSensorControlData.au8PreStatus[mu8LevelSensorIDBuf] = SLevelSensorControlData.au8CurrentStatus[mu8LevelSensorIDBuf];
                Reset_ReedSWChatteringTime(mu8LevelSensorIDBuf);     // Chattering 시간 초기화
            }
        }
    }
}



/// @brief      Level Sensor Data 확정 함수
/// @details    수위 센서 감지값을 확정 판단을 한다
/// @param      mu8LevelID : defined Level Sensor ID
///             mu8Value : 1(detect), 0(undetect) Data
/// @return     void
void ConfirmLevelSensorData(U8 mu8LevelID, U8 mu8Value)
{
    switch (mu8LevelID)
    {
        case 1:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_1_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_1_DETECT;
            }
            break;

        case 2:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_2_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_2_DETECT;
            }
            break;

        case 3:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_3_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_3_DETECT;
            }
            break;

        case 4:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_4_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_4_DETECT;
            }
            break;

        case 5:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_5_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_5_DETECT;
            }
            break;

        case 6:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_6_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_6_DETECT;
            }
            break;

        case 7:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_7_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_7_DETECT;
            }
            break;

        case 8:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_8_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_8_DETECT;
            }
            break;

        case 9:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_9_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_9_DETECT;
            }
            break;

        case 10:
            if (mu8Value)
            {
                gu16LevelSensorConfirmValue |= LEVEL_SENSOR_10_DETECT;
            }
            else
            {
                gu16LevelSensorConfirmValue &= ~LEVEL_SENSOR_10_DETECT;
            }
            break;

        default:
            break;
    }
}


/// @brief      Check Water Level Sensor Status
/// @details    수위 센서 감지 상태를 확인하여 반환한다
/// @param      mu8SensorID : defined Level Sensor ID
/// @return     return : 수위 센서 감지값 - 1(Detect), 0(Undetect)
U8 Get_WaterLevel_Status(U8 mu8SensorID)
{
    U8 mu8Return = 0;

    switch (mu8SensorID)
    {
        case 1:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_1_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 2:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_2_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 3:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_3_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 4:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_4_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 5:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_5_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }

        case 6:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_6_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 7:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_7_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 8:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_8_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 9:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_9_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 10:
            if (gu16LevelSensorConfirmValue & LEVEL_SENSOR_10_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        default:
            mu8Return = 0;
            break;
    }

    return  mu8Return;
}

#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
/// @brief      전극봉 수위센서 체크 제어 함수(@While)
/// @details    전극봉 수위센서 감지 제어 동작을 한다
/// @param      void
/// @return     void
void Elec_Level_CheckControl(void)
{
    if (gu8ElecLevelPowerControlTimer >= ELEC_LEVEL_SENSOR_CONTROL_TIME_PERIOD)
    {       // 전원 제어
        gu8ElecLevelPowerControlTimer = 0;

        gu8ElecLevelPowerDirection ^= 1;

        if (gu8ElecLevelPowerDirection == 0)
        {
            TURN_ON_ELEC_LEVEL_SENSOR_POWER_1();
            TURN_ON_ELEC_LEVEL_SENSOR_POWER_2();
        }
        else
        {
            TURN_OFF_ELEC_LEVEL_SENSOR_POWER_1();
            TURN_OFF_ELEC_LEVEL_SENSOR_POWER_2();
            gu8ElecLevelCheckDelay = ELEC_LEVEL_SENSOR_CHECK_DELAY_TIME;
        }
    }

    // AD 측정 세팅
    if ( (gu8ElecLevelPowerDirection == 0) && (gu8ElecLevelCheckDelay == 0) )
    {
//        Set_Hal_ADC_Check(ELECTRODE_LEVEL_SENSOR_ADC_ID, 1);
    }
    else
    {
//        Set_Hal_ADC_Check(ELECTRODE_LEVEL_SENSOR_ADC_ID, 0);
    }

    // 수위 상태 판단
    if (gu8ElecLevelCheckTimer < ELEC_LEVEL_SENSOR_CHECK_TIME_PERIOD)
    {
        gu8ElecLevelCheckTimer = 0;

        Elec_LevelSensorConfirm();
    }
}


/// @brief      전극봉 수위 센서 상태 확정 함수
/// @details    전극봉 수위 센서 상태 확정 판단을 한다
/// @param      void
/// @return     void
void Elec_LevelSensorConfirm(void)
{
    U16 mu16ElecLevelSensorADBuf = 0;

//    mu16ElecLevelSensorADBuf = Get_ADC_Data(ELECTRODE_LEVEL_SENSOR_ADC_ID);

    if (mu16ElecLevelSensorADBuf < ELEC_LEVEL_SENSOR_HIGH_LEVEL_VALUE)
    {
        gu8ElecLevelUndetectTime = 0;

        if (gu8ElecLevelDetectTime >= ELEC_LEVEL_SENSOR_CHATTERING_TIME)
        {
            gu8ElecLevelConfirmStatus = 1;
        }
        else
        {
            gu8ElecLevelDetectTime++;
        }
    }
    else if (mu16ElecLevelSensorADBuf > ELEC_LEVEL_SENSOR_LOW_LEVEL_VALUE)
    {
        gu8ElecLevelDetectTime = 0;

        if (gu8ElecLevelUndetectTime >= ELEC_LEVEL_SENSOR_CHATTERING_TIME)
        {
            gu8ElecLevelConfirmStatus = 0;
        }
        else
        {
            gu8ElecLevelUndetectTime++;
        }
	}
    else
    {
        gu8ElecLevelUndetectTime = 0;
        gu8ElecLevelDetectTime = 0;
    }
}


/// @brief      전극봉 수위 센서 상태 확인 함수
/// @details    전극봉 수위 센서 감지 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 수위 센서 감지 상태 반환 - 0(Undetect), 1(Detect)
U8 Get_ElecLevel_Status(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8ElecLevelConfirmStatus;

    return  mu8Return;
}

#endif


// Hal Level Sensor Module ***********************************************************************************

/// @brief      Hal Level Sensor Initilaize Module
/// @details    수위 센서 감지 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_LevelSensor_Module_Initialize(void)
{
#if ( (LEVEL_SENSOR_COUNT > 0) || (ELECTRODE_LEVEL_SENSOR_USE == USE) )
    Hal_LevelSensor_Initialize();           // 수위 센서 관련 초기화
#endif
}


/// @brief      Hal Level Sensor Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 수위 센서 감지 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_LevelSensor_Module_1ms_Control(void)
{
#if ( (LEVEL_SENSOR_COUNT > 0) || (ELECTRODE_LEVEL_SENSOR_USE == USE) )
    LevelSensor_ControlTimer();             // 수위 센서 체크 제어 시간 Counter
#endif
}


/// @brief      Hal Level Sensor Module in While Loop
/// @details    Basic Loop 안 While문 안에서 수위 센서 감지 제어를 한다
/// @param      void
/// @return     void
void Hal_LevelSensor_Module_Control(void)
{
#if (LEVEL_SENSOR_COUNT > 0)
    LevelSensorControl();                   // 수위 센서 체크 함수
#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
    Elec_Level_CheckControl();              // 전극봉 수위 센서 체크 제어
#endif
}

