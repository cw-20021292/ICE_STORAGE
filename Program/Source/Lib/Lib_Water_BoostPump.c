/// @file     Lib_Water_BoostPump.c
/// @date     2025/06/02
/// @author   Jaejin Ham
/// @brief    Booster Pump ���� ���� file


#include "Global_Header.h"

#if (BOOST_PUMP_USE == USE)

U8 gu8BoostPumpOnStep = 0;                          /// @brief  �ν�Ʈ ���� ���� Ƚ�� �ܰ�
U16 gu16BoostPumpPeriodTimer = 0;                   /// @brief  �ν�Ʈ ���� ���� ���� �ֱ� Ÿ�̸�
U16 gu16BoostPumpControlTimer = 0;                  /// @brief  �ν�Ʈ ���� ���� Timer


/// @brief      Lib Boost Pump ���� �ʱ�ȭ �Լ�
/// @details    Boost Pump ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_BoostPump_Initialize(void)
{
    gu8BoostPumpOnStep = 0;             // �ν�Ʈ ���� ���� Ƚ�� �ܰ�
    gu16BoostPumpPeriodTimer = 0;       // �ν�Ʈ ���� ���� ���� �ֱ� Ÿ�̸�
    gu16BoostPumpControlTimer = 0;      // �ν�Ʈ ���� ���� Timer
}


/// @brief      Boost Pump control Timer(@1ms interrupt)
/// @details    �ν�Ʈ ���� ��� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void BoostPump_ControlTimer(void)
{
    if (gu16BoostPumpPeriodTimer < BOOST_PUMP_CONTROL_PERIOD)
    {
        gu16BoostPumpPeriodTimer++;
    }
}


/// @brief      �ν�Ʈ ���� ���� �ܰ踦 ���� �����ϴ� �Լ�
/// @details    �ν�Ʈ ���� ���� ���� �ܰ踦 �����Ͽ� �������ش�
/// @param      mu8Step : �ν�Ʈ ���� ���� ���� - Step(0~3)
/// @return     void
void Set_BoostPumpControlChangeStep(U8 mu8Step)
{
    gu8BoostPumpOnStep = mu8Step;
    gu16BoostPumpControlTimer = 0;
}


/// @brief      Boost Pump ���� �Լ�(1sec)
/// @details    �ν�Ʈ ������ ������ ��Ȳ�� ���� �����ϴ� ������ �Ѵ�
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
        {   //  �ν�Ʈ ������ ����� ��� ����
            mu8FeedValveStatus = Get_ValveStatus(VALVE_ID_WATER_IN);

            if (mu8FeedValveStatus == ON)
            {   // Feed Valve�� ���� ���
                if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP1)
                {   // 1�ܰ��� ���
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP1_TIME)
                    {   // �ν�Ʈ ����ð��� 40��(40�� X 60��)�� �Ѵ� ���
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP2;   // 2�ܰ��
                        Set_BoostPump(OFF, 0);
                    }
                    else
                    {   // 40�а� ON
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(ON, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP2)
                {   // 2�ܰ��� ���
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP2_TIME)
                    {   // �ν�Ʈ ����ð��� 20��(20�� X 60��)�� �Ѵ� ���
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP3;   // 3�ܰ��
                        Set_BoostPump(ON, 0);
                    }
                    else
                    {   // 20�а� OFF
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(OFF, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP3)
                {   // 3�ܰ��� ���
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP3_TIME)
                    {   // �ν�Ʈ ����ð��� 40��(40�� X 60��)�� �Ѵ� ���
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP4;   // 4�ܰ��
                        Set_BoostPump(OFF, 0);
                    }
                    else
                    {   // 40�а� ON
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(ON, 0);
                    }
                }
                else if (gu8BoostPumpOnStep == BOOST_CONTROL_STEP4)
                {   // 4�ܰ��� ���
                    if (gu16BoostPumpControlTimer >= BOOST_CONTROL_STEP4_TIME)
                    {   // �ν�Ʈ ����ð��� 120��(120�� X 60��)�� �Ѵ� ���
                        gu16BoostPumpControlTimer = 0;
                        gu8BoostPumpOnStep = BOOST_CONTROL_STEP3;   // �ٽ� 3�ܰ��
                        Set_BoostPump(ON, 0);
                    }
                    else
                    {   // 120�а� OFF
                        gu16BoostPumpControlTimer++;
                        Set_BoostPump(OFF, 0);
                    }
                }
                else
                {   // 4�ܰ谡 �Ѿ�� ��� 1�ܰ�� �ʱ�ȭ
                    gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
                    Set_BoostPump(OFF, 0);
                    gu16BoostPumpControlTimer = 0;
                }
            }
            else
            {   // // Feed Valve�� ���� ���
                gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
                Set_BoostPump(OFF, 0);
                gu16BoostPumpControlTimer = 0;
            }
        }
        else
        {   // Boost Pump�� ������� ���� ���
            gu8BoostPumpOnStep = BOOST_CONTROL_STEP1;
            Set_BoostPump(OFF, 0);
            gu16BoostPumpControlTimer = 0;
        }
    }
}


#endif    // Boost Pump USE END


// Lib Boost Pump Module *********************************************************************************

/// @brief      Lib Boost Pump Module Initilaize
/// @details    �ν�Ʈ ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_BoostPump_Module_Initialize(void)
{
#if (BOOST_PUMP_USE == USE)    
    Lib_BoostPump_Initialize();
#endif    
}


/// @brief      Lib Boost Pump Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Boost Pump ���� ��� Library
/// @param      void
/// @return     void
void Lib_BoostPump_Module_1ms_Control(void)
{
#if (BOOST_PUMP_USE == USE)        
    BoostPump_ControlTimer();
#endif
}


/// @brief      Lib Boost Pump Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Boost Pump ���� ��� Library
/// @param      void
/// @return     void
void Lib_BoostPump_Module_Control(void)
{
#if (BOOST_PUMP_USE == USE)    
    Lib_BoostPump_Control();
#endif    
}


