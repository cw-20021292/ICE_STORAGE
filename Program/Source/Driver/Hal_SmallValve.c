/// @file     Hal_SmallValve.c
/// @date     2025/03/28
/// @author   Jaejin Ham
/// @brief    소형 밸브 제어 file


#include "Global_Header.h"

#if (SMALL_VALVE_COUNT > 0)

U8 gu8SmallValveControTimer = 0;                        /// @brief  소형 밸브 제어 주기 시간 타이머

/// @brief  Valve 제어 관리 자료형
typedef struct {
    U8 au8Status[SMALL_VALVE_ID_MAX_COUNT];             // Valve Status(1:ON, 0:OFF)
    U8 au8Delay1Time[SMALL_VALVE_ID_MAX_COUNT];         // Valve Control Delay Time
    U8 au8Delay2Time[SMALL_VALVE_ID_MAX_COUNT];         // Valve Control Delay Time
}   SSmallValveData_T;

SSmallValveData_T        SSmallValveControlData;


/// @brief      소형 밸브 Initialize
/// @details    소형 밸브 제어 관련 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_SmallValve_Initialize(void)
{
    MEMSET( (void __FAR *)&SSmallValveControlData, 0x00, sizeof(SSmallValveData_T) );
    Hal_AllFeedValve_Close();

    gu8SmallValveControTimer = 0;
}


/// @brief      소형 밸브 control Timer(@1ms interrupt)
/// @details    소형 밸브 제어 주기 시간 타이머를 카운트한다
/// @param      void
/// @return     void
void SmallValveControlTimer(void)
{
    if (gu8SmallValveControTimer < SMALL_VALVE_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8SmallValveControTimer++;
    }
}


/// @brief      소형 밸브 control function(@While)
/// @details    소형 밸브를 제어한다
/// @param      void
/// @return     void
void SmallValveControl(void)
{
    U8 mu8SmallValveIDBuf = 0;

    if (gu8SmallValveControTimer >= SMALL_VALVE_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8SmallValveControTimer = 0;

        for (mu8SmallValveIDBuf = 1 ; mu8SmallValveIDBuf < SMALL_VALVE_ID_MAX_COUNT ; mu8SmallValveIDBuf++)
        {
            if (SSmallValveControlData.au8Delay1Time[mu8SmallValveIDBuf])
            {
                SSmallValveControlData.au8Delay1Time[mu8SmallValveIDBuf]--;
            }

            if (SSmallValveControlData.au8Delay2Time[mu8SmallValveIDBuf])
            {
                SSmallValveControlData.au8Delay2Time[mu8SmallValveIDBuf]--;
            }

            Hal_SmallValveControl(mu8SmallValveIDBuf);
        }
    }
}


/// @brief      Valve control function
/// @details    소형 밸브 제어 상태값들을 설정한다
/// @param      mu8ValveID : defined Valve ID
///             mu8Status : 1(Open), 0(Close)
///             mu8Delay1 : Control Delay Time(@100ms)
///             mu8Delay2 : Change Voltage Delay Time(@100ms)
/// @return     void
void Set_SmallValveControl(U8 mu8ValveID, U8 mu8Status, U8 mu8Delay1, U8 mu8Delay2)
{
    if (mu8Status == OPEN)
    {
        SSmallValveControlData.au8Status[mu8ValveID] = ON;
        SSmallValveControlData.au8Delay1Time[mu8ValveID] = mu8Delay1;
        SSmallValveControlData.au8Delay2Time[mu8ValveID] = mu8Delay2;
    }
    else
    {
        SSmallValveControlData.au8Status[mu8ValveID] = OFF;
        SSmallValveControlData.au8Delay1Time[mu8ValveID] = mu8Delay1;
        SSmallValveControlData.au8Delay2Time[mu8ValveID] = mu8Delay2;
    }
}


/// @brief      모든 소형 밸브 Valve OFF(Close)
/// @details    모든 소형 밸브를 Close 동작 시킨다
/// @param      void
/// @return     void
void Hal_AllSmallValve_Close(void)
{
    U8 mu8SmallValveIDBuf = 0;

    for (mu8SmallValveIDBuf = 1 ; mu8SmallValveIDBuf < SMALL_VALVE_ID_MAX_COUNT ; mu8SmallValveIDBuf++)
    {
        Set_SmallValveControl(mu8SmallValveIDBuf, CLOSE, 0, 0);
    }
}


/// @brief      소형 밸브 Port control function
/// @details    소형 밸브 HW 제어 동작을 한다
/// @param      mu8ValveID : defined Valve ID
/// @return     void
void Hal_SmallValveControl(U8 mu8ValveID)
{
    switch (mu8ValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_COUNT >= 1)
            if (SSmallValveControlData.au8Delay1Time[mu8ValveID] == 0)
            {
                if (SSmallValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_SMALL_VALVE_1();

                    if (SSmallValveControlData.au8Delay2Time[mu8ValveID] == 0)
                    {
                        TURN_ON_SMALL_VALVE_1_TRIG();
                    }
                    else
                    {
                        TURN_OFF_SMALL_VALVE_1_TRIG();
                    }
                }
                else
                {
                    TURN_OFF_SMALL_VALVE_1();

                    if (SSmallValveControlData.au8Delay2Time[mu8ValveID] == 0)
                    {
                        TURN_OFF_SMALL_VALVE_1_TRIG();
                    }
                }
            }
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_COUNT >= 2)
            if (SSmallValveControlData.au8Delay1Time[mu8ValveID] == 0)
            {
                if (SSmallValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_SMALL_VALVE_2();

                    if (SSmallValveControlData.au8Delay2Time[mu8ValveID] == 0)
                    {
                        TURN_ON_SMALL_VALVE_2_TRIG();
                    }
                    else
                    {
                        TURN_OFF_SMALL_VALVE_2_TRIG();
                    }
                }
                else
                {
                    TURN_OFF_SMALL_VALVE_2();

                    if (SSmallValveControlData.au8Delay2Time[mu8ValveID] == 0)
                    {
                        TURN_OFF_SMALL_VALVE_2_TRIG();
                    }
                }
            }
#endif
            break;

        default:
#if (VALVE_COUNT >= 1)
            TURN_OFF_SMALL_VALVE_1();
            TURN_OFF_SMALL_VALVE_1_TRIG();
#endif
#if (VALVE_COUNT >= 2)
            TURN_OFF_SMALL_VALVE_2();
            TURN_OFF_SMALL_VALVE_2_TRIG();
#endif
            break;
    }
}

#endif


// Hal Valve Module ******************************************************************************************

/// @brief      Hal Small Valve Initilaize Module
/// @details    소형 밸브 제어 관련 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_SmallValve_Module_Initialize(void)
{
#if (SMALL_VALVE_COUNT > 0)
    Hal_SmallValve_Initialize();            // 소형 밸브 제어 관련 초기화
#endif
}


/// @brief      Hal Small Valve Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 소형 밸브 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_SmallValve_Module_1ms_Control(void)
{
#if (SMALL_VALVE_COUNT > 0)
    SmallValveControlTimer();               // 소형 밸브 제어 시간 Timer
#endif
}


/// @brief      Hal Small Valve Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 소형 밸브 제어를 한다
/// @param      void
/// @return     void
void Hal_SmallValve_Module_Control(void)
{
#if (SMALL_VALVE_COUNT)
    SmallValveControl();                    // 소형 밸브 제어 함수
#endif
}
