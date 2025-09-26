/// @file     Hal_DC_Pump.c
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    DC Pump Control file


#include "Global_Header.h"


#if (DC_PUMP_COUNT > 0)

U8 gu8DC_PumpControTimer = 0;           /// @brief  DC Pump 제어 시간 타이머

#if (PUMP_1_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump1PWMValue = 0;           /// @brief  DC Pump1 PWM 제어 레지스터값
U8 gu8DC_Pump1OnDelay = 0;              /// @brief  DC Pump1 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump1OffDelay = 0;             /// @brief  DC Pump1 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_2_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump2PWMValue = 0;           /// @brief  DC Pump2 PWM 제어 레지스터값
U8 gu8DC_Pump2OnDelay = 0;              /// @brief  DC Pump2 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump2OffDelay = 0;             /// @brief  DC Pump2 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_3_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump3PWMValue = 0;           /// @brief  DC Pump3 PWM 제어 레지스터값
U8 gu8DC_Pump3OnDelay = 0;              /// @brief  DC Pump3 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump3OffDelay = 0;             /// @brief  DC Pump3 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_4_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump4PWMValue = 0;           /// @brief  DC Pump4 PWM 제어 레지스터값
U8 gu8DC_Pump4OnDelay = 0;              /// @brief  DC Pump4 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump4OffDelay = 0;             /// @brief  DC Pump4 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_5_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump5PWMValue = 0;           /// @brief  DC Pump5 PWM 제어 레지스터값
U8 gu8DC_Pump5OnDelay = 0;              /// @brief  DC Pump5 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump5OffDelay = 0;             /// @brief  DC Pump5 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_6_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump6PWMValue = 0;           /// @brief  DC Pump6 PWM 제어 레지스터값
U8 gu8DC_Pump6OnDelay = 0;              /// @brief  DC Pump6 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump6OffDelay = 0;             /// @brief  DC Pump6 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_7_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump7PWMValue = 0;           /// @brief  DC Pump7 PWM 제어 레지스터값
U8 gu8DC_Pump7OnDelay = 0;              /// @brief  DC Pump7 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump7OffDelay = 0;             /// @brief  DC Pump7 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_8_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump8PWMValue = 0;           /// @brief  DC Pump8 PWM 제어 레지스터값
U8 gu8DC_Pump8OnDelay = 0;              /// @brief  DC Pump8 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump8OffDelay = 0;             /// @brief  DC Pump8 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_9_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump9PWMValue = 0;           /// @brief  DC Pump9 PWM 제어 레지스터값
U8 gu8DC_Pump9OnDelay = 0;              /// @brief  DC Pump9 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump9OffDelay = 0;             /// @brief  DC Pump9 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif

#if (PUMP_10_PWM_USE > PUMP_PWM_NO_USE)
U16 gu16DC_Pump10PWMValue = 0;          /// @brief  DC Pump10 PWM 제어 레지스터값
U8 gu8DC_Pump10OnDelay = 0;             /// @brief  DC Pump10 PWM 제어와 밸브 ON 동작 사이의 Delay 시간
U8 gu8DC_Pump10OffDelay = 0;            /// @brief  DC Pump10 PWM 제어 후 밸브 OFF 동작 사이의 Delay 시간
#endif


/// @brief  DC Pump 제어 관리 자료형
typedef struct {
    U8  au8Status[DC_PUMP_ID_MAX_COUNT];             // DC Pump Status(1:ON, 0:OFF)
    U16  au16DelayTime[DC_PUMP_ID_MAX_COUNT];        // DC Pump Control Delay Time
}   SDC_PumpData_T;

SDC_PumpData_T      SDC_PumpControlData;    // DC Pump Control Data


/// @brief      DC Pump Initialize
/// @details    DC Pump 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DC_Pump_Initialize(void)
{
    MEMSET( (void __FAR *)&SDC_PumpControlData, 0x00, sizeof(SDC_PumpData_T) );

    Hal_All_DC_Pump_Off();

    gu8DC_PumpControTimer = 0;

#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump1PWMValue = 0;
    gu8DC_Pump1OnDelay = PUMP_1_PWM_ON_DELAY;
    gu8DC_Pump1OffDelay = PUMP_1_PWM_OFF_DELAY;
#elif(PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump1OnDelay = PUMP_1_TRIG_ON_DELAY;
    gu8DC_Pump1OffDelay = PUMP_1_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump2PWMValue = 0;
    gu8DC_Pump2OnDelay = PUMP_2_PWM_ON_DELAY;
    gu8DC_Pump2OffDelay = PUMP_2_PWM_OFF_DELAY;
#elif(PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump2OnDelay = PUMP_2_TRIG_ON_DELAY;
    gu8DC_Pump2OffDelay = PUMP_2_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump3PWMValue = 0;
    gu8DC_Pump3OnDelay = PUMP_3_PWM_ON_DELAY;
    gu8DC_Pump3OffDelay = PUMP_3_PWM_OFF_DELAY;
#elif(PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump3OnDelay = PUMP_3_TRIG_ON_DELAY;
    gu8DC_Pump3OffDelay = PUMP_3_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump4PWMValue = 0;
    gu8DC_Pump4OnDelay = PUMP_4_PWM_ON_DELAY;
    gu8DC_Pump4OffDelay = PUMP_4_PWM_OFF_DELAY;
#elif(PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump4OnDelay = PUMP_4_TRIG_ON_DELAY;
    gu8DC_Pump4OffDelay = PUMP_4_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump5PWMValue = 0;
    gu8DC_Pump5OnDelay = PUMP_5_PWM_ON_DELAY;
    gu8DC_Pump5OffDelay = PUMP_5_PWM_OFF_DELAY;
#elif(PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump5OnDelay = PUMP_5_TRIG_ON_DELAY;
    gu8DC_Pump5OffDelay = PUMP_5_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump6PWMValue = 0;
    gu8DC_Pump6OnDelay = PUMP_6_PWM_ON_DELAY;
    gu8DC_Pump6OffDelay = PUMP_6_PWM_OFF_DELAY;
#elif(PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump6OnDelay = PUMP_6_TRIG_ON_DELAY;
    gu8DC_Pump6OffDelay = PUMP_6_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump7PWMValue = 0;
    gu8DC_Pump7OnDelay = PUMP_7_PWM_ON_DELAY;
    gu8DC_Pump7OffDelay = PUMP_7_PWM_OFF_DELAY;
#elif(PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump7OnDelay = PUMP_7_TRIG_ON_DELAY;
    gu8DC_Pump7OffDelay = PUMP_7_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump8PWMValue = 0;
    gu8DC_Pump8OnDelay = PUMP_8_PWM_ON_DELAY;
    gu8DC_Pump8OffDelay = PUMP_8_PWM_OFF_DELAY;
#elif(PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump8OnDelay = PUMP_8_TRIG_ON_DELAY;
    gu8DC_Pump8OffDelay = PUMP_8_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump9PWMValue = 0;
    gu8DC_Pump9OnDelay = PUMP_9_PWM_ON_DELAY;
    gu8DC_Pump9OffDelay = PUMP_9_PWM_OFF_DELAY;
#elif(PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump9OnDelay = PUMP_9_TRIG_ON_DELAY;
    gu8DC_Pump9OffDelay = PUMP_9_TRIG_OFF_DELAY;
#else
#endif

#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
    gu16DC_Pump10PWMValue = 0;
    gu8DC_Pump10OnDelay = PUMP_10_PWM_ON_DELAY;
    gu8DC_Pump10OffDelay = PUMP_10_PWM_OFF_DELAY;
#elif(PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
    gu8DC_Pump10OnDelay = PUMP_10_TRIG_ON_DELAY;
    gu8DC_Pump10OffDelay = PUMP_10_TRIG_OFF_DELAY;
#else
#endif
}


/// @brief      DC Pump control Timer(@1ms interrupt)
/// @details    DC Pump 제어 시간 타이머를 카운트 한다
/// @param      void
/// @return     void
void DC_PumpControlTimer(void)
{
    if (gu8DC_PumpControTimer < DC_PUMP_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8DC_PumpControTimer++;
    }
}


/// @brief      DC Pump control function(@While)
/// @details    DC Pump 제어 동작을 While문 안에서 수행한다
/// @param      void
/// @return     void
void DC_PumpControl(void)
{
    U8 mu8PumpIDBuf = 0;

    if (gu8DC_PumpControTimer >= DC_PUMP_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8DC_PumpControTimer = 0;

        for (mu8PumpIDBuf = 1 ; mu8PumpIDBuf < DC_PUMP_ID_MAX_COUNT ; mu8PumpIDBuf++)
        {
            if (SDC_PumpControlData.au16DelayTime[mu8PumpIDBuf])
            {
                SDC_PumpControlData.au16DelayTime[mu8PumpIDBuf]--;
            }

            Hal_DC_PumpControl(mu8PumpIDBuf);
        }
    }
}


/// @brief      DC Pump control function
/// @details    DC Pump의 제어 상태를 설정한다
/// @param      mu8PumpID : defined Pump ID
///             mu8Status : 1(ON), 0(OFF)
///             mu16Delay : Control Delay Time(@100ms)
/// @return     void
void Set_DC_PumpControl(U8 mu8PumpID, U8 mu8Status, U16 mu16Delay)
{
    if (mu8Status == ON)
    {
        SDC_PumpControlData.au8Status[mu8PumpID] = ON;
        SDC_PumpControlData.au16DelayTime[mu8PumpID] = mu16Delay;
    }
    else
    {
        SDC_PumpControlData.au8Status[mu8PumpID] = OFF;
        SDC_PumpControlData.au16DelayTime[mu8PumpID] = mu16Delay;
    }
}


/// @brief      DC Pump 동작 상태 확인 함수
/// @details    ID 에 해당하는 DC Pump의 동작 상태를 확인하여 반환한다
/// @param      mu8PumpID : defined Pump ID
/// @return     return : 동작 상태 - 0(FALSE, 미동작), 1(TRUE, 동작)
U8 Get_DC_PumpStatus(U8 mu8PumpID)
{
    if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      All DC Pump OFF
/// @details    모든 DC Pump를 OFF 시킨다
/// @param      void
/// @return     void
void Hal_All_DC_Pump_Off(void)
{
    U8 mu8PumpIDBuf = 0;

    for (mu8PumpIDBuf = 1 ; mu8PumpIDBuf < DC_PUMP_ID_MAX_COUNT ; mu8PumpIDBuf++)
    {
        Set_DC_PumpControl(mu8PumpIDBuf, OFF, 0);
    }
}


/// @brief      DC Pump Port control function(100ms)
/// @details    DC Pump의 Port 제어를 한다
/// @param      mu8PumpID : defined DC Pump ID
/// @return     void
void Hal_DC_PumpControl(U8 mu8PumpID)
{
    switch (mu8PumpID)
    {
        case 1:     // ID 1
#if (DC_PUMP_COUNT >= 1)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {       // Pump On 이면
                    Hal_DC_Pump_1_On();
                }
                else
                {       // Pump Off 이면
                    Hal_DC_Pump_1_Off();
                }
            }
#endif
            break;

        case 2:     // ID 2
#if (DC_PUMP_COUNT >= 2)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_2_On();
                }
                else
                {
                    Hal_DC_Pump_2_Off();
                }
            }
#endif
            break;

        case 3:     // ID 3
#if (DC_PUMP_COUNT >= 3)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_3_On();
                }
                else
                {
                    Hal_DC_Pump_3_Off();
                }
            }
#endif
            break;

        case 4:     // ID 4
#if (DC_PUMP_COUNT >= 4)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_4_On();
                }
                else
                {
                    Hal_DC_Pump_4_Off();
                }
            }
#endif
            break;

        case 5:     // ID 5
#if (DC_PUMP_COUNT >= 5)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_5_On();
                }
                else
                {
                    Hal_DC_Pump_5_Off();
                }
            }
#endif
            break;

        case 6:     // ID 6
#if (DC_PUMP_COUNT >= 6)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_6_On();
                }
                else
                {
                    Hal_DC_Pump_6_Off();
                }
            }
#endif
            break;

        case 7:     // ID 7
#if (DC_PUMP_COUNT >= 7)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_7_On();
                }
                else
                {
                    Hal_DC_Pump_7_Off();
                }
            }
#endif
            break;

        case 8:     // ID 8
#if (DC_PUMP_COUNT >= 8)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_8_On();
                }
                else
                {
                    Hal_DC_Pump_8_Off();
                }
            }
#endif
            break;

        case 9:     // ID 9
#if (DC_PUMP_COUNT >= 9)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_9_On();
                }
                else
                {
                    Hal_DC_Pump_9_Off();
                }
            }
#endif
            break;

        case 10:     // ID 10
#if (DC_PUMP_COUNT >= 10)
            if (SDC_PumpControlData.au16DelayTime[mu8PumpID] == 0)
            {
                if (SDC_PumpControlData.au8Status[mu8PumpID] == ON)
                {
                    Hal_DC_Pump_10_On();
                }
                else
                {
                    Hal_DC_Pump_10_Off();
                }
            }
#endif
            break;

        default:
#if (DC_PUMP_COUNT >= 1)
            Hal_DC_Pump_1_Off();
#endif
#if (DC_PUMP_COUNT >= 2)
            Hal_DC_Pump_2_Off();
#endif
#if (DC_PUMP_COUNT >= 3)
            Hal_DC_Pump_3_Off();
#endif
#if (DC_PUMP_COUNT >= 4)
            Hal_DC_Pump_4_Off();
#endif
#if (DC_PUMP_COUNT >= 5)
            Hal_DC_Pump_5_Off();
#endif
#if (DC_PUMP_COUNT >= 6)
            Hal_DC_Pump_6_Off();
#endif
#if (DC_PUMP_COUNT >= 7)
            Hal_DC_Pump_7_Off();
#endif
#if (DC_PUMP_COUNT >= 8)
            Hal_DC_Pump_8_Off();
#endif
#if (DC_PUMP_COUNT >= 9)
            Hal_DC_Pump_9_Off();
#endif
#if (DC_PUMP_COUNT >= 10)
            Hal_DC_Pump_10_Off();
#endif
            break;
    }
}


/// @brief      DC Pump의 실제 HW 동작 상태 확인 함수
/// @details    ID 에 해당하는 DC Pump의 실제 HW 동작 상태를 확인하여 반환한다
/// @param      mu8PumpID : defined Pump ID
/// @return     return : 동작 상태 - 0(FALSE, 미동작), 1(TRUE, 동작)
U8 Get_DC_PumpStatus_HW(U8 mu8PumpID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    switch (mu8PumpID)
    {
        case 1:     // Pump ID 1
#if (DC_PUMP_COUNT >= 1)
            mu8Status = GET_STATUS_PUMP_1();
#endif
            break;

        case 2:     // Pump ID 2
#if (DC_PUMP_COUNT >= 2)
            mu8Status = GET_STATUS_PUMP_2();
#endif
            break;

        case 3:     // Pump ID 3
#if (DC_PUMP_COUNT >= 3)
            mu8Status = GET_STATUS_PUMP_3();
#endif
            break;

        case 4:     // Pump ID 4
#if (DC_PUMP_COUNT >= 4)
            mu8Status = GET_STATUS_PUMP_4();
#endif
            break;

        case 5:     // Pump ID 5
#if (DC_PUMP_COUNT >= 5)
            mu8Status = GET_STATUS_PUMP_5();
#endif
            break;

        case 6:     // Pump ID 6
#if (DC_PUMP_COUNT >= 6)
            mu8Status = GET_STATUS_PUMP_6();
#endif
            break;

        case 7:     // Pump ID 7
#if (DC_PUMP_COUNT >= 7)
            mu8Status = GET_STATUS_PUMP_7();
#endif
            break;

        case 8:     // Pump ID 8
#if (DC_PUMP_COUNT >= 8)
            mu8Status = GET_STATUS_PUMP_8();
#endif
            break;

        case 9:     // Pump ID 9
#if (DC_PUMP_COUNT >= 9)
            mu8Status = GET_STATUS_PUMP_9();
#endif
            break;

        case 10:     // Pump ID 10
#if (DC_PUMP_COUNT >= 10)
            mu8Status = GET_STATUS_PUMP_10();
#endif
            break;

        default:
            mu8Status = 0;
            break;
    }

    if (mu8Status)
    {
        mu8Return = TRUE;
    }
    else
    {
        mu8Return = FALSE;
    }

    return  mu8Return;
}


/// @brief      DC Pump PWM control function
/// @details    DC Pump의 PWM 제어를 수행한다
/// @param      mu8PumpID : defined Pump ID
///             mu16RegValue : Register Value
/// @return     void
void Set_PumpPWMControl(U8 mu8PumpID, U16 mu16RegValue)
{
    switch (mu8PumpID)
    {
        case 1:
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump1PWMValue = mu16RegValue;
#endif
            break;

        case 2:
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump2PWMValue = mu16RegValue;
#endif
            break;

        case 3:
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump3PWMValue = mu16RegValue;
#endif
            break;

        case 4:
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump4PWMValue = mu16RegValue;
#endif
            break;

        case 5:
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump5PWMValue = mu16RegValue;
#endif
            break;

        case 6:
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump6PWMValue = mu16RegValue;
#endif
            break;

        case 7:
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump7PWMValue = mu16RegValue;
#endif
            break;

        case 8:
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump8PWMValue = mu16RegValue;
#endif
            break;

        case 9:
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump9PWMValue = mu16RegValue;
#endif
            break;

        case 10:
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump10PWMValue = mu16RegValue;
#endif
            break;

        default:
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump1PWMValue = 0;
#endif
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump2PWMValue = 0;
#endif
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump3PWMValue = 0;
#endif
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump4PWMValue = 0;
#endif
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump5PWMValue = 0;
#endif
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump6PWMValue = 0;
#endif
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump7PWMValue = 0;
#endif
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump8PWMValue = 0;
#endif
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump9PWMValue = 0;
#endif
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
            gu16DC_Pump10PWMValue = 0;
#endif
            break;
    }
}


#if (DC_PUMP_COUNT >= 1)

/// @brief      DC Pump 1 ON
/// @details    DC Pump 1의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_1_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_1();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump1OffDelay = PUMP_1_PWM_OFF_DELAY;

        if (gu8DC_Pump1OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_1_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_1_PWM = gu16DC_Pump1PWMValue;
                DC_PUMP_1_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_1_PWM = gu16DC_Pump1PWMValue;
            }

            TURN_ON_PUMP_1();
        }
        else
        {
            gu8DC_Pump1OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_1_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_1_PWM = gu16DC_Pump1PWMValue;
                DC_PUMP_1_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_1_PWM = gu16DC_Pump1PWMValue;
            }

            TURN_OFF_PUMP_1();
        }
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump1OffDelay = PUMP_1_TRIG_OFF_DELAY;

        if (gu8DC_Pump1OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_1_TRIG();
            TURN_ON_PUMP_1();
        }
        else
        {
            gu8DC_Pump1OnDelay--;

            TURN_ON_PUMP_1_TRIG();
            TURN_OFF_PUMP_1();
        }
#else
        TURN_ON_PUMP_1();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump1OffDelay = PUMP_1_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_1_PWM = gu16DC_Pump1PWMValue;
        TURN_ON_PUMP_1();
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump1OffDelay = PUMP_1_TRIG_OFF_DELAY;
        TURN_ON_PUMP_1_TRIG();
        TURN_ON_PUMP_1();
#else
        TURN_ON_PUMP_1();
#endif
    }
}


/// @brief      DC Pump 1 OFF
/// @details    DC Pump 1의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_1_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_1();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump1OnDelay = PUMP_1_PWM_ON_DELAY;

        if (gu8DC_Pump1OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_1_PWM = 0;
            DC_PUMP_1_PWM_STOP();

            TURN_OFF_PUMP_1();
        }
        else
        {
            gu8DC_Pump1OffDelay--;

            REGISTER_DEFINE_DC_PUMP_1_PWM = 0;
            DC_PUMP_1_PWM_STOP();

            TURN_ON_PUMP_1();
        }
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump1OnDelay = PUMP_1_TRIG_ON_DELAY;

        if (gu8DC_Pump1OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_1_TRIG();
            TURN_OFF_PUMP_1();
        }
        else
        {
            gu8DC_Pump1OffDelay--;

            TURN_ON_PUMP_1_TRIG();
            TURN_OFF_PUMP_1();
        }
#else
        TURN_OFF_PUMP_1();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump1OnDelay = PUMP_1_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_1_PWM = 0;
        DC_PUMP_1_PWM_STOP();
        TURN_OFF_PUMP_1();
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump1OnDelay = PUMP_1_TRIG_ON_DELAY;
        TURN_OFF_PUMP_1_TRIG();
        TURN_OFF_PUMP_1();
#else
        TURN_OFF_PUMP_1();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 2)

/// @brief      DC Pump 2 ON
/// @details    DC Pump 2의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_2_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_2();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump2OffDelay = PUMP_2_PWM_OFF_DELAY;

        if (gu8DC_Pump2OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_2_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_2_PWM = gu16DC_Pump2PWMValue;
                DC_PUMP_2_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_2_PWM = gu16DC_Pump2PWMValue;
            }

            TURN_ON_PUMP_2();
        }
        else
        {
            gu8DC_Pump2OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_2_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_2_PWM = gu16DC_Pump2PWMValue;
                DC_PUMP_2_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_2_PWM = gu16DC_Pump2PWMValue;
            }

            TURN_OFF_PUMP_2();
        }
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump2OffDelay = PUMP_2_TRIG_OFF_DELAY;

        if (gu8DC_Pump2OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_2_TRIG();
            TURN_ON_PUMP_2();
        }
        else
        {
            gu8DC_Pump2OnDelay--;

            TURN_ON_PUMP_2_TRIG();
            TURN_OFF_PUMP_2();
        }
#else
        TURN_ON_PUMP_2();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump2OffDelay = PUMP_2_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_2_PWM = gu16DC_Pump2PWMValue;
        TURN_ON_PUMP_2();
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump2OffDelay = PUMP_2_TRIG_OFF_DELAY;
        TURN_ON_PUMP_2_TRIG();
        TURN_ON_PUMP_2();
#else
        TURN_ON_PUMP_2();
#endif
    }
}


/// @brief      DC Pump 2 OFF
/// @details    DC Pump 2의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_2_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_2();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump2OnDelay = PUMP_2_PWM_ON_DELAY;

        if (gu8DC_Pump2OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_2_PWM = 0;
            DC_PUMP_2_PWM_STOP();

            TURN_OFF_PUMP_2();
        }
        else
        {
            gu8DC_Pump2OffDelay--;

            REGISTER_DEFINE_DC_PUMP_2_PWM = 0;
            DC_PUMP_2_PWM_STOP();

            TURN_ON_PUMP_2();
        }
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump2OnDelay = PUMP_2_TRIG_ON_DELAY;

        if (gu8DC_Pump2OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_2_TRIG();
            TURN_OFF_PUMP_2();
        }
        else
        {
            gu8DC_Pump2OffDelay--;

            TURN_ON_PUMP_2_TRIG();
            TURN_OFF_PUMP_2();
        }
#else
        TURN_OFF_PUMP_2();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump2OnDelay = PUMP_2_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_2_PWM = 0;
        DC_PUMP_2_PWM_STOP();
        TURN_OFF_PUMP_2();
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump2OnDelay = PUMP_2_TRIG_ON_DELAY;
        TURN_OFF_PUMP_2_TRIG();
        TURN_OFF_PUMP_2();
#else
        TURN_OFF_PUMP_2();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 3)

/// @brief      DC Pump 3 ON
/// @details    DC Pump 3의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_3_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_3();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump3OffDelay = PUMP_3_PWM_OFF_DELAY;

        if (gu8DC_Pump3OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_3_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_3_PWM = gu16DC_Pump3PWMValue;
                DC_PUMP_3_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_3_PWM = gu16DC_Pump3PWMValue;
            }

            TURN_ON_PUMP_3();
        }
        else
        {
            gu8DC_Pump3OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_3_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_3_PWM = gu16DC_Pump3PWMValue;
                DC_PUMP_3_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_3_PWM = gu16DC_Pump3PWMValue;
            }

            TURN_OFF_PUMP_3();
        }
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump3OffDelay = PUMP_3_TRIG_OFF_DELAY;

        if (gu8DC_Pump3OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_3_TRIG();
            TURN_ON_PUMP_3();
        }
        else
        {
            gu8DC_Pump3OnDelay--;

            TURN_ON_PUMP_3_TRIG();
            TURN_OFF_PUMP_3();
        }
#else
        TURN_ON_PUMP_3();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump3OffDelay = PUMP_3_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_3_PWM = gu16DC_Pump3PWMValue;
        TURN_ON_PUMP_3();
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump3OffDelay = PUMP_3_TRIG_OFF_DELAY;
        TURN_ON_PUMP_3_TRIG();
        TURN_ON_PUMP_3();
#else
        TURN_ON_PUMP_3();
#endif
    }
}


/// @brief      DC Pump 3 OFF
/// @details    DC Pump 3의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_3_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_3();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump3OnDelay = PUMP_3_PWM_ON_DELAY;

        if (gu8DC_Pump3OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_3_PWM = 0;
            DC_PUMP_3_PWM_STOP();

            TURN_OFF_PUMP_3();
        }
        else
        {
            gu8DC_Pump3OffDelay--;

            REGISTER_DEFINE_DC_PUMP_3_PWM = 0;
            DC_PUMP_3_PWM_STOP();

            TURN_ON_PUMP_3();
        }
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump3OnDelay = PUMP_3_TRIG_ON_DELAY;

        if (gu8DC_Pump3OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_3_TRIG();
            TURN_OFF_PUMP_3();
        }
        else
        {
            gu8DC_Pump3OffDelay--;

            TURN_ON_PUMP_3_TRIG();
            TURN_OFF_PUMP_3();
        }
#else
        TURN_OFF_PUMP_3();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump3OnDelay = PUMP_3_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_3_PWM = 0;
        DC_PUMP_3_PWM_STOP();
        TURN_OFF_PUMP_3();
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump3OnDelay = PUMP_3_TRIG_ON_DELAY;
        TURN_OFF_PUMP_3_TRIG();
        TURN_OFF_PUMP_3();
#else
        TURN_OFF_PUMP_3();
#endif
    }

}

#endif

#if (DC_PUMP_COUNT >= 4)

/// @brief      DC Pump 4 ON
/// @details    DC Pump 4의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_4_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_4();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump4OffDelay = PUMP_4_PWM_OFF_DELAY;

        if (gu8DC_Pump4OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_4_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_4_PWM = gu16DC_Pump4PWMValue;
                DC_PUMP_4_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_4_PWM = gu16DC_Pump4PWMValue;
            }

            TURN_ON_PUMP_4();
        }
        else
        {
            gu8DC_Pump4OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_4_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_4_PWM = gu16DC_Pump4PWMValue;
                DC_PUMP_4_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_4_PWM = gu16DC_Pump4PWMValue;
            }

            TURN_OFF_PUMP_4();
        }
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump4OffDelay = PUMP_4_TRIG_OFF_DELAY;

        if (gu8DC_Pump4OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_4_TRIG();
            TURN_ON_PUMP_4();
        }
        else
        {
            gu8DC_Pump4OnDelay--;

            TURN_ON_PUMP_4_TRIG();
            TURN_OFF_PUMP_4();
        }
#else
        TURN_ON_PUMP_4();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump4OffDelay = PUMP_4_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_4_PWM = gu16DC_Pump4PWMValue;
        TURN_ON_PUMP_4();
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump4OffDelay = PUMP_4_TRIG_OFF_DELAY;
        TURN_ON_PUMP_4_TRIG();
        TURN_ON_PUMP_4();
#else
        TURN_ON_PUMP_4();
#endif
    }
}


/// @brief      DC Pump 4 OFF
/// @details    DC Pump 4의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_4_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_4();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump4OnDelay = PUMP_4_PWM_ON_DELAY;

        if (gu8DC_Pump4OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_4_PWM = 0;
            DC_PUMP_4_PWM_STOP();

            TURN_OFF_PUMP_4();
        }
        else
        {
            gu8DC_Pump4OffDelay--;

            REGISTER_DEFINE_DC_PUMP_4_PWM = 0;
            DC_PUMP_4_PWM_STOP();

            TURN_ON_PUMP_4();
        }
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump4OnDelay = PUMP_4_TRIG_ON_DELAY;

        if (gu8DC_Pump4OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_4_TRIG();
            TURN_OFF_PUMP_4();
        }
        else
        {
            gu8DC_Pump4OffDelay--;

            TURN_ON_PUMP_4_TRIG();
            TURN_OFF_PUMP_4();
        }
#else
        TURN_OFF_PUMP_4();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump4OnDelay = PUMP_4_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_4_PWM = 0;
        DC_PUMP_4_PWM_STOP();
        TURN_OFF_PUMP_4();
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump4OnDelay = PUMP_4_TRIG_ON_DELAY;
        TURN_OFF_PUMP_4_TRIG();
        TURN_OFF_PUMP_4();
#else
        TURN_OFF_PUMP_4();
#endif
    }

}

#endif

#if (DC_PUMP_COUNT >= 5)

/// @brief      DC Pump 5 ON
/// @details    DC Pump 5의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_5_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_5();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump5OffDelay = PUMP_5_PWM_OFF_DELAY;

        if (gu8DC_Pump5OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_5_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_5_PWM = gu16DC_Pump5PWMValue;
                DC_PUMP_5_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_5_PWM = gu16DC_Pump5PWMValue;
            }

            TURN_ON_PUMP_5();
        }
        else
        {
            gu8DC_Pump5OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_5_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_5_PWM = gu16DC_Pump5PWMValue;
                DC_PUMP_5_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_5_PWM = gu16DC_Pump5PWMValue;
            }

            TURN_OFF_PUMP_5();
        }
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump5OffDelay = PUMP_5_TRIG_OFF_DELAY;

        if (gu8DC_Pump5OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_5_TRIG();
            TURN_ON_PUMP_5();
        }
        else
        {
            gu8DC_Pump5OnDelay--;

            TURN_ON_PUMP_5_TRIG();
            TURN_OFF_PUMP_5();
        }
#else
        TURN_ON_PUMP_5();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump5OffDelay = PUMP_5_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_5_PWM = gu16DC_Pump5PWMValue;
        TURN_ON_PUMP_5();
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump5OffDelay = PUMP_5_TRIG_OFF_DELAY;
        TURN_ON_PUMP_5_TRIG();
        TURN_ON_PUMP_5();
#else
        TURN_ON_PUMP_5();
#endif
    }
}


/// @brief      DC Pump 5 OFF
/// @details    DC Pump 5의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_5_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_5();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump5OnDelay = PUMP_5_PWM_ON_DELAY;

        if (gu8DC_Pump5OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_5_PWM = 0;
            DC_PUMP_5_PWM_STOP();

            TURN_OFF_PUMP_5();
        }
        else
        {
            gu8DC_Pump5OffDelay--;

            REGISTER_DEFINE_DC_PUMP_5_PWM = 0;
            DC_PUMP_5_PWM_STOP();

            TURN_ON_PUMP_5();
        }
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump5OnDelay = PUMP_5_TRIG_ON_DELAY;

        if (gu8DC_Pump5OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_5_TRIG();
            TURN_OFF_PUMP_5();
        }
        else
        {
            gu8DC_Pump5OffDelay--;

            TURN_ON_PUMP_5_TRIG();
            TURN_OFF_PUMP_5();
        }
#else
        TURN_OFF_PUMP_5();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump5OnDelay = PUMP_5_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_5_PWM = 0;
        DC_PUMP_5_PWM_STOP();
        TURN_OFF_PUMP_5();
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump5OnDelay = PUMP_5_TRIG_ON_DELAY;
        TURN_OFF_PUMP_5_TRIG();
        TURN_OFF_PUMP_5();
#else
        TURN_OFF_PUMP_5();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 6)

/// @brief      DC Pump 6 ON
/// @details    DC Pump 6의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_6_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_6();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump6OffDelay = PUMP_6_PWM_OFF_DELAY;

        if (gu8DC_Pump6OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_6_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_6_PWM = gu16DC_Pump6PWMValue;
                DC_PUMP_6_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_6_PWM = gu16DC_Pump6PWMValue;
            }

            TURN_ON_PUMP_6();
        }
        else
        {
            gu8DC_Pump6OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_6_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_6_PWM = gu16DC_Pump6PWMValue;
                DC_PUMP_6_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_6_PWM = gu16DC_Pump6PWMValue;
            }

            TURN_OFF_PUMP_6();
        }
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump6OffDelay = PUMP_6_TRIG_OFF_DELAY;

        if (gu8DC_Pump6OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_6_TRIG();
            TURN_ON_PUMP_6();
        }
        else
        {
            gu8DC_Pump6OnDelay--;

            TURN_ON_PUMP_6_TRIG();
            TURN_OFF_PUMP_6();
        }
#else
        TURN_ON_PUMP_6();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump6OffDelay = PUMP_6_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_6_PWM = gu16DC_Pump6PWMValue;
        TURN_ON_PUMP_6();
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump6OffDelay = PUMP_6_TRIG_OFF_DELAY;
        TURN_ON_PUMP_6_TRIG();
        TURN_ON_PUMP_6();
#else
        TURN_ON_PUMP_6();
#endif
    }
}


/// @brief      DC Pump 6 OFF
/// @details    DC Pump 6의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_6_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_6();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump6OnDelay = PUMP_6_PWM_ON_DELAY;

        if (gu8DC_Pump6OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_6_PWM = 0;
            DC_PUMP_6_PWM_STOP();

            TURN_OFF_PUMP_6();
        }
        else
        {
            gu8DC_Pump6OffDelay--;

            REGISTER_DEFINE_DC_PUMP_6_PWM = 0;
            DC_PUMP_6_PWM_STOP();

            TURN_ON_PUMP_6();
        }
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump6OnDelay = PUMP_6_TRIG_ON_DELAY;

        if (gu8DC_Pump6OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_6_TRIG();
            TURN_OFF_PUMP_6();
        }
        else
        {
            gu8DC_Pump6OffDelay--;

            TURN_ON_PUMP_6_TRIG();
            TURN_OFF_PUMP_6();
        }
#else
        TURN_OFF_PUMP_6();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump6OnDelay = PUMP_6_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_6_PWM = 0;
        DC_PUMP_6_PWM_STOP();
        TURN_OFF_PUMP_6();
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump6OnDelay = PUMP_6_TRIG_ON_DELAY;
        TURN_OFF_PUMP_6_TRIG();
        TURN_OFF_PUMP_6();
#else
        TURN_OFF_PUMP_6();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 7)

/// @brief      DC Pump 7 ON
/// @details    DC Pump 7의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_7_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_7();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump7OffDelay = PUMP_7_PWM_OFF_DELAY;

        if (gu8DC_Pump7OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_7_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_7_PWM = gu16DC_Pump7PWMValue;
                DC_PUMP_7_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_7_PWM = gu16DC_Pump7PWMValue;
            }

            TURN_ON_PUMP_7();
        }
        else
        {
            gu8DC_Pump7OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_7_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_7_PWM = gu16DC_Pump7PWMValue;
                DC_PUMP_7_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_7_PWM = gu16DC_Pump7PWMValue;
            }

            TURN_OFF_PUMP_7();
        }
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump7OffDelay = PUMP_7_TRIG_OFF_DELAY;

        if (gu8DC_Pump7OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_7_TRIG();
            TURN_ON_PUMP_7();
        }
        else
        {
            gu8DC_Pump7OnDelay--;

            TURN_ON_PUMP_7_TRIG();
            TURN_OFF_PUMP_7();
        }
#else
        TURN_ON_PUMP_7();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump7OffDelay = PUMP_7_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_7_PWM = gu16DC_Pump7PWMValue;
        TURN_ON_PUMP_7();
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump7OffDelay = PUMP_7_TRIG_OFF_DELAY;
        TURN_ON_PUMP_7_TRIG();
        TURN_ON_PUMP_7();
#else
        TURN_ON_PUMP_7();
#endif
    }
}


/// @brief      DC Pump 7 OFF
/// @details    DC Pump 7의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_7_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_7();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump7OnDelay = PUMP_7_PWM_ON_DELAY;

        if (gu8DC_Pump7OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_7_PWM = 0;
            DC_PUMP_7_PWM_STOP();

            TURN_OFF_PUMP_7();
        }
        else
        {
            gu8DC_Pump7OffDelay--;

            REGISTER_DEFINE_DC_PUMP_7_PWM = 0;
            DC_PUMP_7_PWM_STOP();

            TURN_ON_PUMP_7();
        }
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump7OnDelay = PUMP_7_TRIG_ON_DELAY;

        if (gu8DC_Pump7OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_7_TRIG();
            TURN_OFF_PUMP_7();
        }
        else
        {
            gu8DC_Pump7OffDelay--;

            TURN_ON_PUMP_7_TRIG();
            TURN_OFF_PUMP_7();
        }
#else
        TURN_OFF_PUMP_7();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump7OnDelay = PUMP_7_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_7_PWM = 0;
        DC_PUMP_7_PWM_STOP();
        TURN_OFF_PUMP_7();
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump7OnDelay = PUMP_7_TRIG_ON_DELAY;
        TURN_OFF_PUMP_7_TRIG();
        TURN_OFF_PUMP_7();
#else
        TURN_OFF_PUMP_7();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 8)

/// @brief      DC Pump 8 ON
/// @details    DC Pump 8의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_8_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_8();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump8OffDelay = PUMP_8_PWM_OFF_DELAY;

        if (gu8DC_Pump8OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_8_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_8_PWM = gu16DC_Pump8PWMValue;
                DC_PUMP_8_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_8_PWM = gu16DC_Pump8PWMValue;
            }

            TURN_ON_PUMP_8();
        }
        else
        {
            gu8DC_Pump8OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_8_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_8_PWM = gu16DC_Pump8PWMValue;
                DC_PUMP_8_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_8_PWM = gu16DC_Pump8PWMValue;
            }

            TURN_OFF_PUMP_8();
        }
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump8OffDelay = PUMP_8_TRIG_OFF_DELAY;

        if (gu8DC_Pump8OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_8_TRIG();
            TURN_ON_PUMP_8();
        }
        else
        {
            gu8DC_Pump8OnDelay--;

            TURN_ON_PUMP_8_TRIG();
            TURN_OFF_PUMP_8();
        }
#else
        TURN_ON_PUMP_8();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump8OffDelay = PUMP_8_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_8_PWM = gu16DC_Pump8PWMValue;
        TURN_ON_PUMP_8();
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump8OffDelay = PUMP_8_TRIG_OFF_DELAY;
        TURN_ON_PUMP_8_TRIG();
        TURN_ON_PUMP_8();
#else
        TURN_ON_PUMP_8();
#endif
    }
}


/// @brief      DC Pump 8 OFF
/// @details    DC Pump 8의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_8_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_8();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump8OnDelay = PUMP_8_PWM_ON_DELAY;

        if (gu8DC_Pump8OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_8_PWM = 0;
            DC_PUMP_8_PWM_STOP();

            TURN_OFF_PUMP_8();
        }
        else
        {
            gu8DC_Pump8OffDelay--;

            REGISTER_DEFINE_DC_PUMP_8_PWM = 0;
            DC_PUMP_8_PWM_STOP();

            TURN_ON_PUMP_8();
        }
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump8OnDelay = PUMP_8_TRIG_ON_DELAY;

        if (gu8DC_Pump8OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_8_TRIG();
            TURN_OFF_PUMP_8();
        }
        else
        {
            gu8DC_Pump8OffDelay--;

            TURN_ON_PUMP_8_TRIG();
            TURN_OFF_PUMP_8();
        }
#else
        TURN_OFF_PUMP_8();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump8OnDelay = PUMP_8_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_8_PWM = 0;
        DC_PUMP_8_PWM_STOP();
        TURN_OFF_PUMP_8();
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump8OnDelay = PUMP_8_TRIG_ON_DELAY;
        TURN_OFF_PUMP_8_TRIG();
        TURN_OFF_PUMP_8();
#else
        TURN_OFF_PUMP_8();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 9)

/// @brief      DC Pump 9 ON
/// @details    DC Pump 9의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_9_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_9();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump9OffDelay = PUMP_9_PWM_OFF_DELAY;

        if (gu8DC_Pump9OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_9_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_9_PWM = gu16DC_Pump9PWMValue;
                DC_PUMP_9_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_9_PWM = gu16DC_Pump9PWMValue;
            }

            TURN_ON_PUMP_9();
        }
        else
        {
            gu8DC_Pump9OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_9_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_9_PWM = gu16DC_Pump9PWMValue;
                DC_PUMP_9_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_9_PWM = gu16DC_Pump9PWMValue;
            }

            TURN_OFF_PUMP_9();
        }
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump9OffDelay = PUMP_9_TRIG_OFF_DELAY;

        if (gu8DC_Pump9OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_9_TRIG();
            TURN_ON_PUMP_9();
        }
        else
        {
            gu8DC_Pump9OnDelay--;

            TURN_ON_PUMP_9_TRIG();
            TURN_OFF_PUMP_9();
        }
#else
        TURN_ON_PUMP_9();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump9OffDelay = PUMP_9_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_9_PWM = gu16DC_Pump9PWMValue;
        TURN_ON_PUMP_9();
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump9OffDelay = PUMP_9_TRIG_OFF_DELAY;
        TURN_ON_PUMP_9_TRIG();
        TURN_ON_PUMP_9();
#else
        TURN_ON_PUMP_9();
#endif
    }
}


/// @brief      DC Pump 9 OFF
/// @details    DC Pump 9의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_9_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_9();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump9OnDelay = PUMP_9_PWM_ON_DELAY;

        if (gu8DC_Pump9OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_9_PWM = 0;
            DC_PUMP_9_PWM_STOP();

            TURN_OFF_PUMP_9();
        }
        else
        {
            gu8DC_Pump9OffDelay--;

            REGISTER_DEFINE_DC_PUMP_9_PWM = 0;
            DC_PUMP_9_PWM_STOP();

            TURN_ON_PUMP_9();
        }
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump9OnDelay = PUMP_9_TRIG_ON_DELAY;

        if (gu8DC_Pump9OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_9_TRIG();
            TURN_OFF_PUMP_9();
        }
        else
        {
            gu8DC_Pump9OffDelay--;

            TURN_ON_PUMP_9_TRIG();
            TURN_OFF_PUMP_9();
        }
#else
        TURN_OFF_PUMP_9();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump9OnDelay = PUMP_9_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_9_PWM = 0;
        DC_PUMP_9_PWM_STOP();
        TURN_OFF_PUMP_9();
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump9OnDelay = PUMP_9_TRIG_ON_DELAY;
        TURN_OFF_PUMP_9_TRIG();
        TURN_OFF_PUMP_9();
#else
        TURN_OFF_PUMP_9();
#endif
    }

}

#endif


#if (DC_PUMP_COUNT >= 10)

/// @brief      DC Pump 10 ON
/// @details    DC Pump 10의 On 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_10_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_10();

    if (mu8Status == OFF)
    {   // Pump On 상태가 아니었으면
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump10OffDelay = PUMP_10_PWM_OFF_DELAY;

        if (gu8DC_Pump10OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            if (REGISTER_DEFINE_DC_PUMP_10_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_10_PWM = gu16DC_Pump10PWMValue;
                DC_PUMP_10_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_10_PWM = gu16DC_Pump10PWMValue;
            }

            TURN_ON_PUMP_10();
        }
        else
        {
            gu8DC_Pump10OnDelay--;

            if (REGISTER_DEFINE_DC_PUMP_10_PWM == 0)
            {
                REGISTER_DEFINE_DC_PUMP_10_PWM = gu16DC_Pump10PWMValue;
                DC_PUMP_10_PWM_START();
            }
            else
            {
                REGISTER_DEFINE_DC_PUMP_10_PWM = gu16DC_Pump10PWMValue;
            }

            TURN_OFF_PUMP_10();
        }
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump10OffDelay = PUMP_10_TRIG_OFF_DELAY;

        if (gu8DC_Pump10OnDelay == 0)
        {   // 전압 가변 ON 동작 후 일정 Delay 후 Pump On
            TURN_ON_PUMP_10_TRIG();
            TURN_ON_PUMP_10();
        }
        else
        {
            gu8DC_Pump10OnDelay--;

            TURN_ON_PUMP_10_TRIG();
            TURN_OFF_PUMP_10();
        }
#else
        TURN_ON_PUMP_10();
#endif
    }
    else
    {   // Pump On 상태이면
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump10OffDelay = PUMP_10_PWM_OFF_DELAY;
        REGISTER_DEFINE_DC_PUMP_10_PWM = gu16DC_Pump10PWMValue;
        TURN_ON_PUMP_10();
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump10OffDelay = PUMP_10_TRIG_OFF_DELAY;
        TURN_ON_PUMP_10_TRIG();
        TURN_ON_PUMP_10();
#else
        TURN_ON_PUMP_10();
#endif
    }
}


/// @brief      DC Pump 10 OFF
/// @details    DC Pump 10의 Off 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_10_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_PUMP_10();

    if (mu8Status == ON)
    {   // Pump On 상태이면
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump10OnDelay = PUMP_10_PWM_ON_DELAY;

        if (gu8DC_Pump10OffDelay == 0)
        {   // 전압 가변 Off 동작 후 일정 Delay 후 Pump Off
            REGISTER_DEFINE_DC_PUMP_10_PWM = 0;
            DC_PUMP_10_PWM_STOP();

            TURN_OFF_PUMP_10();
        }
        else
        {
            gu8DC_Pump10OffDelay--;

            REGISTER_DEFINE_DC_PUMP_10_PWM = 0;
            DC_PUMP_10_PWM_STOP();

            TURN_ON_PUMP_10();
        }
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump10OnDelay = PUMP_10_TRIG_ON_DELAY;

        if (gu8DC_Pump10OffDelay == 0)
        {   // Pump Off 후 일정 Delay 후 Trig port Off
            TURN_OFF_PUMP_10_TRIG();
            TURN_OFF_PUMP_10();
        }
        else
        {
            gu8DC_Pump10OffDelay--;

            TURN_ON_PUMP_10_TRIG();
            TURN_OFF_PUMP_10();
        }
#else
        TURN_OFF_PUMP_10();
#endif
    }
    else
    {   // Pump Off 상태이면
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
        gu8DC_Pump10OnDelay = PUMP_10_PWM_ON_DELAY;
        REGISTER_DEFINE_DC_PUMP_10_PWM = 0;
        DC_PUMP_10_PWM_STOP();
        TURN_OFF_PUMP_10();
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
        gu8DC_Pump10OnDelay = PUMP_10_TRIG_ON_DELAY;
        TURN_OFF_PUMP_10_TRIG();
        TURN_OFF_PUMP_10();
#else
        TURN_OFF_PUMP_10();
#endif
    }

}

#endif


#endif


// Hal DC Pump Module ****************************************************************************************

/// @brief      Hal DC Pump Initilaize Module
/// @details    DC Pump 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DC_Pump_Module_Initialize(void)
{
#if (DC_PUMP_COUNT > 0)
    Hal_DC_Pump_Initialize();
#endif
}


/// @brief      Hal DC Pump Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 DC Pump 제어 타이며를 카운트 한다
/// @param      void
/// @return     void
void Hal_DC_Pump_Module_1ms_Control(void)
{
#if (DC_PUMP_COUNT > 0)
    DC_PumpControlTimer();                  // DC Pump 제어 시간 Counter
#endif
}


/// @brief      Hal DC Pump Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 DC Pump를 제어한다
/// @param      void
/// @return     void
void Hal_DC_Pump_Module_Control(void)
{
#if (DC_PUMP_COUNT > 0)
    DC_PumpControl();                       // DC Pump Control 함수
#endif
}




