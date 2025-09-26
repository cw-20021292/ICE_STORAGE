/// @file     Hal_Heater.c
/// @date     2025/02/17
/// @author   Jaejin Ham
/// @brief    Heater Control file


#include "Global_Header.h"

#if (NORMAL_HEATER_USE == USE)

#if (HEATER_COUNT > 0)

U8 gu8HeaterControTimer = 0;                        /// @brief  �Ϲ� ��ũ�� ���� ���� �ֱ� �ð� Ÿ�̸�

/// @brief  Valve ���� ���� �ڷ���
typedef struct {
    U8  au8Status[HEATER_ID_MAX_COUNT];             // Heater Status(1:ON, 0:OFF)
    U8  au8DelayTime[HEATER_ID_MAX_COUNT];          // Heater Control Delay Time
}   SHeaterData_T;

SHeaterData_T   SHeaterControlData;


/// @brief      Heater Port Initialize
/// @details    �Ϲ� ���� ���� ���� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Heater_Initialize(void)
{
    MEMSET( (void __FAR *)&SHeaterControlData, 0x00, sizeof(SHeaterData_T) );

    gu8HeaterControTimer = 0;

    Hal_All_Heater_Off();
}


/// @brief      All heater Port Off
/// @details    ��� ���� ���� Port�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_All_Heater_Off(void)
{
    U8 mu8HeaterIDBuf = 0;

    for (mu8HeaterIDBuf = 1 ; mu8HeaterIDBuf < HEATER_ID_MAX_COUNT ; mu8HeaterIDBuf++)
    {
        Set_Heater_Control(mu8HeaterIDBuf, OFF, 0);
    }
}


/// @brief      Heater Control Timer(@1ms interrupt)
/// @details    �Ϲ� ���� ���� �ֱ� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void HeaterControlTimer(void)
{
    if (gu8HeaterControTimer < HEATER_CONTROL_TIME_PERIOD)
    {
        gu8HeaterControTimer++;
    }
}


/// @brief      Heater Control Function(@while)
/// @details    �Ϲ� ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void HeaterControl(void)
{
    U8 mu8HeaterIDBuf = 0;

    if (gu8HeaterControTimer >= HEATER_CONTROL_TIME_PERIOD)
    {       // 100ms���� ����
        gu8HeaterControTimer = 0;

        for (mu8HeaterIDBuf = 1 ; mu8HeaterIDBuf < HEATER_ID_MAX_COUNT ; mu8HeaterIDBuf++)
        {
            if (SHeaterControlData.au8DelayTime[mu8HeaterIDBuf])
            {
                SHeaterControlData.au8DelayTime[mu8HeaterIDBuf]--;
            }

            Hal_Heater_Control(mu8HeaterIDBuf);
        }
    }
}


/// @brief      Heater control function
/// @details    �Ϲ� ���� ���� ���� ���� ���°����� �����Ѵ�
/// @param      mu8HeaterID : defined Heater ID
///             mu8OnOff : ���� ���� ���� - 1(On), 0(Off)
///             mu8Delay : Control Delay Time(@100ms)
/// @return     void
void Set_Heater_Control(U8 mu8HeaterID, U8 mu8OnOff, U8 mu8Delay)
{
    SHeaterControlData.au8Status[mu8HeaterID] = ON;
    SHeaterControlData.au8DelayTime[mu8HeaterID] = mu8Delay;
}


/// @brief      Check Heater Status
/// @details    �Ϲ� ���� ���� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8HeaterID : defined heater ID
/// @return     return : ���� ���� ���� - 1(On), 0(Off)
U8 Get_Heater_Status(U8 mu8HeaterID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

#if (HEATER_COUNT > 0)

    switch (mu8HeaterID)
    {
        case 1:     // ID 1
#if (HEATER_COUNT >= 1)
            mu8Status = GET_STATUS_HEATER_1();
#endif
            break;

        case 2:     // ID 2
#if (HEATER_COUNT >= 2)
            mu8Status = GET_STATUS_HEATER_2();
#endif
            break;

        case 3:     // ID 3
#if (HEATER_COUNT >= 3)
            mu8Status = GET_STATUS_HEATER_3();
#endif
            break;

        case 4:     // ID 4
#if (HEATER_COUNT >= 4)
            mu8Status = GET_STATUS_HEATER_4();
#endif
            break;

        case 5:     // ID 5
#if (HEATER_COUNT >= 5)
            mu8Status = GET_STATUS_HEATER_5();
#endif
            break;

        case 6:     // ID 6
#if (HEATER_COUNT >= 6)
            mu8Status = GET_STATUS_HEATER_6();
#endif
            break;

        case 7:     // ID 7
#if (HEATER_COUNT >= 7)
            mu8Status = GET_STATUS_HEATER_7();
#endif
            break;

        case 8:     // ID 8
#if (HEATER_COUNT >= 8)
            mu8Status = GET_STATUS_HEATER_8();
#endif
            break;

        case 9:     // ID 9
#if (HEATER_COUNT >= 9)
            mu8Status = GET_STATUS_HEATER_9();
#endif
            break;

        case 10:     // ID 10
#if (HEATER_COUNT >= 10)
            mu8Status = GET_STATUS_HEATER_10();
#endif
            break;

        default:
            mu8Status = 0;
            break;
    }


    if (mu8Status)
    {
        mu8Return = 1;
    }
    else
    {
        mu8Return = 0;
    }

#else

    mu8Return = 0;

#endif

    return  mu8Return;
}


/// @brief      Heater Port Control function
/// @details    �Ϲ� ��ũ�� ���� ���� ���� ������ �Ѵ�
/// @param      mu8HeaterID : defined Heater ID
/// @return     void
void Hal_Heater_Control(U8 mu8HeaterID)
{
    switch (mu8HeaterID)
    {
        case 1:     // ID 1
#if (HEATER_COUNT >= 1)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_1();
                }
                else
                {
                    TURN_OFF_HEATER_1();
                }
            }
#endif
            break;

        case 2:     // ID 2
#if (HEATER_COUNT >= 2)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_2();
                }
                else
                {
                    TURN_OFF_HEATER_2();
                }
            }
#endif
            break;

        case 3:     // ID 3
#if (HEATER_COUNT >= 3)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_3();
                }
                else
                {
                    TURN_OFF_HEATER_3();
                }
            }
#endif
            break;

        case 4:     // ID 4
#if (HEATER_COUNT >= 4)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_4();
                }
                else
                {
                    TURN_OFF_HEATER_4();
                }
            }
#endif
            break;

        case 5:     // ID 5
#if (HEATER_COUNT >= 5)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_5();
                }
                else
                {
                    TURN_OFF_HEATER_5();
                }
            }
#endif
            break;

        case 6:     // ID 6
#if (HEATER_COUNT >= 6)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_6();
                }
                else
                {
                    TURN_OFF_HEATER_6();
                }
            }
#endif
            break;

        case 7:     // ID 7
#if (HEATER_COUNT >= 7)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_7();
                }
                else
                {
                    TURN_OFF_HEATER_7();
                }
            }
#endif
            break;

        case 8:     // ID 8
#if (HEATER_COUNT >= 8)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_8();
                }
                else
                {
                    TURN_OFF_HEATER_8();
                }
            }
#endif
            break;

        case 9:     // ID 9
#if (HEATER_COUNT >= 9)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_9();
                }
                else
                {
                    TURN_OFF_HEATER_9();
                }
            }
#endif
            break;

        case 10:     // ID 10
#if (HEATER_COUNT >= 10)
            if (SHeaterControlData.au8DelayTime[mu8HeaterID] == 0)
            {
                if (SHeaterControlData.au8Status[mu8HeaterID] == ON)
                {
                    TURN_ON_HEATER_10();
                }
                else
                {
                    TURN_OFF_HEATER_10();
                }
            }
#endif
            break;

        default:
#if (HEATER_COUNT >= 1)
            TURN_OFF_HEATER_1();
#endif
#if (HEATER_COUNT >= 2)
            TURN_OFF_HEATER_2();
#endif
#if (HEATER_COUNT >= 3)
            TURN_OFF_HEATER_3();
#endif
#if (HEATER_COUNT >= 4)
            TURN_OFF_HEATER_4();
#endif
#if (HEATER_COUNT >= 5)
            TURN_OFF_HEATER_5();
#endif
#if (HEATER_COUNT >= 6)
            TURN_OFF_HEATER_6();
#endif
#if (HEATER_COUNT >= 7)
            TURN_OFF_HEATER_7();
#endif
#if (HEATER_COUNT >= 8)
            TURN_OFF_HEATER_8();
#endif
#if (HEATER_COUNT >= 9)
            TURN_OFF_HEATER_9();
#endif
#if (HEATER_COUNT >= 10)
            TURN_OFF_HEATER_10();
#endif
            break;
    }
}



/// @brief      Heater ���� Port ���� ���� �Լ�
/// @details    �Ϲ� ��ũ�� ���� ��� ���� Port�� ���� ���� ��Ų��
/// @param      mu8HeaterID : defined heater ID
///             mu8OnOff : ���� ���� ������ - 0(OFF), 1(ON)
/// @return     void
void Set_HeaterPort(U8 mu8HeaterID, U8 mu8OnOff)
{
#if (HEATER_COUNT > 0)

    switch (mu8HeaterID)
    {
        case 1:     // ID 1
#if (HEATER_COUNT >= 1)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_1();
            }
            else
            {
                TURN_OFF_HEATER_1();
            }
#endif
            break;

        case 2:     // ID 2
#if (HEATER_COUNT >= 2)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_2();
            }
            else
            {
                TURN_OFF_HEATER_2();
            }
#endif
            break;

        case 3:     // ID 3
#if (HEATER_COUNT >= 3)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_3();
            }
            else
            {
                TURN_OFF_HEATER_3();
            }
#endif
            break;

        case 4:     // ID 4
#if (HEATER_COUNT >= 4)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_4();
            }
            else
            {
                TURN_OFF_HEATER_4();
            }
#endif
            break;

        case 5:     // ID 5
#if (HEATER_COUNT >= 5)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_5();
            }
            else
            {
                TURN_OFF_HEATER_5();
            }
#endif
            break;

        case 6:     // ID 6
#if (HEATER_COUNT >= 6)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_6();
            }
            else
            {
                TURN_OFF_HEATER_6();
            }
#endif
            break;

        case 7:     // ID 7
#if (HEATER_COUNT >= 7)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_7();
            }
            else
            {
                TURN_OFF_HEATER_7();
            }
#endif
            break;

        case 8:     // ID 8
#if (HEATER_COUNT >= 8)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_8();
            }
            else
            {
                TURN_OFF_HEATER_8();
            }
#endif
            break;

        case 9:     // ID 9
#if (HEATER_COUNT >= 9)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_9();
            }
            else
            {
                TURN_OFF_HEATER_9();
            }
#endif
            break;

        case 10:     // ID 10
#if (HEATER_COUNT >= 10)
            if (mu8OnOff)
            {
                TURN_ON_HEATER_10();
            }
            else
            {
                TURN_OFF_HEATER_10();
            }
#endif
            break;

        default:
#if (HEATER_COUNT >= 1)
            TURN_OFF_HEATER_1();
#endif
#if (HEATER_COUNT >= 2)
            TURN_OFF_HEATER_2();
#endif
#if (HEATER_COUNT >= 3)
            TURN_OFF_HEATER_3();
#endif
#if (HEATER_COUNT >= 4)
            TURN_OFF_HEATER_4();
#endif
#if (HEATER_COUNT >= 5)
            TURN_OFF_HEATER_5();
#endif
#if (HEATER_COUNT >= 6)
            TURN_OFF_HEATER_6();
#endif
#if (HEATER_COUNT >= 7)
            TURN_OFF_HEATER_7();
#endif
#if (HEATER_COUNT >= 8)
            TURN_OFF_HEATER_8();
#endif
#if (HEATER_COUNT >= 9)
            TURN_OFF_HEATER_9();
#endif
#if (HEATER_COUNT >= 10)
            TURN_OFF_HEATER_10();
#endif

            break;
    }

#endif
}

#endif

#endif



// Hal Heater Module *****************************************************************************************

/// @brief      Hal Heater Initilaize Module
/// @details    �Ϲ� ��ũ ���� ����� ���õ� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Heater_Module_Initialize(void)
{
#if (NORMAL_HEATER_USE == USE)

#if (HEATER_COUNT > 0)
    Hal_Heater_Initialize();                // ���� ���� ���� �ʱ�ȭ
#endif

#endif
}


/// @brief      Hal Heater Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� �Ϲ� ��ũ ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_Heater_Module_1ms_Control(void)
{
#if (NORMAL_HEATER_USE == USE)

#if (HEATER_COUNT > 0)
    HeaterControlTimer();                   // ���� ���� �ð� Counter
#endif

#endif
}


/// @brief      Hal Heater Module in While Loop
/// @details    Basic Loop ���� While �� �ȿ��� �Ϲ� ��ũ ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_Heater_Module_Control(void)
{
#if (NORMAL_HEATER_USE == USE)

#if (HEATER_COUNT > 0)
    HeaterControl();                        // ���� Control �Լ�
#endif

#endif
}

