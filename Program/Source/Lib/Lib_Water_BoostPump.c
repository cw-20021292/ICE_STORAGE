/// @file     Lib_Water_BoostPump.c
/// @date     2025/06/02
/// @author   Jaejin Ham
/// @brief    Booster Pump 동작 제어 file


#include "Global_Header.h"

#if (BOOST_PUMP_USE == USE)

U8 gu8BoostPumpOnStep = 0;                          /// @brief  부스트 펌프 가동 횟수 단계
U16 gu16BoostPumpPeriodTimer = 0;                   /// @brief  부스트 펌프 제어 동작 주기 타이머
U16 gu16BoostPumpControlTimer = 0;                  /// @brief  부스트 펌프 제어 Timer


/// @brief      Lib Boost Pump 변수 초기화 함수
/// @details    Boost Pump 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Lib_BoostPump_Initialize(void)
{
    gu8BoostPumpOnStep = 0;             // 부스트 펌프 가동 횟수 단계
    gu16BoostPumpPeriodTimer = 0;       // 부스트 펌프 제어 동작 주기 타이머
    gu16BoostPumpControlTimer = 0;      // 부스트 펌프 제어 Timer
}


/// @brief      Boost Pump control Timer(@1ms interrupt)
/// @details    부스트 펌프 제어를 위한 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void BoostPump_ControlTimer(void)
{
    if (gu16BoostPumpPeriodTimer < BOOST_PUMP_CONTROL_PERIOD)
    {
        gu16BoostPumpPeriodTimer++;
    }
}


/// @brief      부스트 펌프 제어 단계를 변경 설정하는 함수
/// @details    부스트 펌프 제어 동작 단계를 변경하여 설정해준다
/// @param      mu8Step : 부스트 펌프 제어 동작 - Step(0~3)
/// @return     void
void Set_BoostPumpControlChangeStep(U8 mu8Step)
{
    gu8BoostPumpOnStep = mu8Step;
    gu16BoostPumpControlTimer = 0;
}


/// @brief      Boost Pump 제어 함수(1sec)
/// @details    부스트 펌프의 동작을 상황에 맞춰 제어하는 동작을 한다
/// @param      void
/// @return     void
void Lib_BoostPump_Control(void)
{
    U8 mu8BoostIn = 0;
    U8 mu8FeedValveStatus = 0;

    if (gu16BoostPumpPeriodTimer >= BOOST_PUMP_CONTROL_PERIOD)
    {
        gu16BoostPumpPeriodTimer = 0;

        mu8BoostIn = Get_BoostInStatus();

        if (mu8BoostIn == TRUE)
        {   //  부스트 펌프가 연결된 경우 제어
            mu8FeedValveStatus = Get_ValveStatus(VALVE_ID_WATER_IN);

            if (mu8FeedValveStatus == ON)
            {   // Feed Valve가 열린 경우
                if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP1)
                {   // 1단계의 경우
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP1_TIME)
                    {   // 부스트 제어시간이 40분(40분 X 60초)이 넘는 경우
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP2;   // 2단계로
                        Set_BoostPump(OFF, 0);
                    }
                    else
                    {   // 40분간 ON
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(ON, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP2)
                {   // 2단계의 경우
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP2_TIME)
                    {   // 부스트 제어시간이 20분(20분 X 60초)이 넘는 경우
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP3;   // 3단계로
                        Set_BoostPump(ON, 0);
                    }
                    else
                    {   // 20분간 OFF
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(OFF, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP3)
                {   // 3단계의 경우
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP3_TIME)
                    {   // 부스트 제어시간이 40분(40분 X 60초)이 넘는 경우
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP4;   // 4단계로
                        Set_BoostPump(OFF, 0);
                    }
                    else
                    {   // 40분간 ON
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(ON, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP4)
                {   // 4단계의 경우
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP4_TIME)
                    {   // 부스트 제어시간이 120분(120분 X 60초)이 넘는 경우
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP3;   // 다시 3단계로
                        Set_BoostPump(ON, 0);
                    }
                    else
                    {   // 120분간 OFF
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(OFF, 0);
                    }
                }
                else
                {   // 4단계가 넘어가는 경우 1단계로 초기화
                    gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
                    Set_BoostPump(OFF, 0);
                    gu16BoostPumpControlTimer = 0;
                }
            }
            else
            {   // // Feed Valve가 닫힌 경우
                gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
                Set_BoostPump(OFF, 0);
                gu16BoostPumpControlTimer = 0;
            }
        }
        else
        {   // Boost Pump가 연결되지 않은 경우
            gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
            Set_BoostPump(OFF, 0);
            gu16BoostPumpControlTimer = 0;
        }
    }
}


#endif    // Boost Pump USE END


// Lib Boost Pump Module *********************************************************************************

/// @brief      Lib Boost Pump Module Initilaize
/// @details    부스트 펌프 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_BoostPump_Module_Initialize(void)
{
#if (BOOST_PUMP_USE == USE)    
    Lib_BoostPump_Initialize();
#endif    
}


/// @brief      Lib Boost Pump Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Boost Pump 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_BoostPump_Module_1ms_Control(void)
{
#if (BOOST_PUMP_USE == USE)        
    BoostPump_ControlTimer();
#endif
}


/// @brief      Lib Boost Pump Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Boost Pump 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_BoostPump_Module_Control(void)
{
#if (BOOST_PUMP_USE == USE)    
    Lib_BoostPump_Control();
#endif    
}


