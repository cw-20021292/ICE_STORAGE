/// @file     Hal_UV.c
/// @date     2025/02/27
/// @author   Jaejin Ham
/// @brief    UV Control file


#include "Global_Header.h"


#if (UV_COUNT > 0)

U8 gu8UV_ControTimer = 0;

/// @brief  UV 제어 관리 자료형
typedef struct {
    U8  au8OnOff[UV_ID_MAX_COUNT];           // UV Status(1:ON, 0:OFF)
}   SUV_Data_T;

SUV_Data_T   SUV_ControlData;


/// @brief      UV Control Initialize
/// @details    UV 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_UV_Initialize(void)
{
    MEMSET( (void __FAR *)&SUV_ControlData, 0x00, sizeof(SUV_Data_T) );

    gu8UV_ControTimer = 0;

    Hal_All_UV_Off();
}


/// @brief      All UV Port Off
/// @details    모든 UV 전원을 OFF 시킨다
/// @param      void
/// @return     void
void Hal_All_UV_Off(void)
{
    U8 mu8UV_IDBuf = 0;

    for (mu8UV_IDBuf = 1 ; mu8UV_IDBuf < UV_ID_MAX_COUNT ; mu8UV_IDBuf++)
    {
        Set_UV_Control(mu8UV_IDBuf, OFF);
    }
}


/// @brief      UV Control Timer(@1ms)
/// @details    UV 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void UV_ControlTimer(void)
{
    if (gu8UV_ControTimer < UV_CONTROL_TIME_PERIOD)
    {
        gu8UV_ControTimer++;
    }
}


/// @brief      UV Control(@while)
/// @details    UV 제어 동작을 한다
/// @param      void
/// @return     void
void UV_Control(void)
{
    U8 mu8UV_IDBuf = 0;

    if (gu8UV_ControTimer >= UV_CONTROL_TIME_PERIOD)
    {       // 100ms마다 제어
        gu8UV_ControTimer = 0;

        for (mu8UV_IDBuf = 1 ; mu8UV_IDBuf < UV_ID_MAX_COUNT ; mu8UV_IDBuf++)
        {
            Hal_UV_Control(mu8UV_IDBuf);
        }
    }
}


/// @brief      UV control function
/// @details    UV 제어 상태값을 설정한다
/// @param      mu8UV_ID : defined UV ID
///             mu8OnOff : UV 동작 상태 - 1(On), 0(Off)
/// @return     void
void Set_UV_Control(U8 mu8UV_ID, U8 mu8OnOff)
{
    SUV_ControlData.au8OnOff[mu8UV_ID] = mu8OnOff;
}


/// @brief      Check UV Status
/// @details    UV 동작 상태를 확인하여 그 상태를 반환한다
/// @param      mu8UV_ID : defined UV ID
/// @return     return : UV 동작 상태 - 1(On), 0(Off)
U8 Get_UV_Status(U8 mu8UV_ID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

#if (UV_COUNT > 0)

    switch (mu8UV_ID)
    {
        case 1:     // ID 1
#if (UV_COUNT >= 1)
            mu8Status = GET_STATUS_UV_1();
#endif
            break;

        case 2:     // ID 2
#if (UV_COUNT >= 2)
            mu8Status = GET_STATUS_UV_2();
#endif
            break;

        case 3:     // ID 3
#if (UV_COUNT >= 3)
            mu8Status = GET_STATUS_UV_3();
#endif
            break;

        case 4:     // ID 4
#if (UV_COUNT >= 4)
            mu8Status = GET_STATUS_UV_4();
#endif
            break;

        case 5:     // ID 5
#if (UV_COUNT >= 5)
            mu8Status = GET_STATUS_UV_5();
#endif
            break;

        case 6:     // ID 6
#if (UV_COUNT >= 6)
            mu8Status = GET_STATUS_UV_6();
#endif
            break;

        case 7:     // ID 7
#if (UV_COUNT >= 7)
            mu8Status = GET_STATUS_UV_7();
#endif
            break;

        case 8:     // ID 8
#if (UV_COUNT >= 8)
            mu8Status = GET_STATUS_UV_8();
#endif
            break;

        case 9:     // ID 9
#if (UV_COUNT >= 9)
            mu8Status = GET_STATUS_UV_9();
#endif
            break;

        case 10:     // ID 10
#if (UV_COUNT >= 10)
            mu8Status = GET_STATUS_UV_10();
#endif
            break;

        default:
            mu8Return = 0;
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


/// @brief      UV Port Control function
/// @details    UV의 HW 제어 동작을 한다
/// @param      mu8UV_ID : defined UV ID
/// @return     void
void Hal_UV_Control(U8 mu8UV_ID)
{
#if (UV_COUNT > 0)

    switch (mu8UV_ID)
    {
        case 1:     // ID 1
#if (UV_COUNT >= 1)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_1();
            }
            else
            {
                TURN_OFF_UV_1();
            }
#endif
            break;

        case 2:     // ID 2
#if (UV_COUNT >= 2)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_2();
            }
            else
            {
                TURN_OFF_UV_2();
            }
#endif
            break;

        case 3:     // ID 3
#if (UV_COUNT >= 3)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_3();
            }
            else
            {
                TURN_OFF_UV_3();
            }
#endif
            break;

        case 4:     // ID 4
#if (UV_COUNT >= 4)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_4();
            }
            else
            {
                TURN_OFF_UV_4();
            }
#endif
            break;

        case 5:     // ID 5
#if (UV_COUNT >= 5)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_5();
            }
            else
            {
                TURN_OFF_UV_5();
            }
#endif
            break;

        case 6:     // ID 6
#if (UV_COUNT >= 6)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_6();
            }
            else
            {
                TURN_OFF_UV_6();
            }
#endif
            break;

        case 7:     // ID 7
#if (UV_COUNT >= 7)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_7();
            }
            else
            {
                TURN_OFF_UV_7();
            }
#endif
            break;

        case 8:     // ID 8
#if (UV_COUNT >= 8)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_8();
            }
            else
            {
                TURN_OFF_UV_8();
            }
#endif
            break;

        case 9:     // ID 9
#if (UV_COUNT >= 9)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_9();
            }
            else
            {
                TURN_OFF_UV_9();
            }
#endif
            break;

        case 10:     // ID 10
#if (UV_COUNT >= 10)
            if (SUV_ControlData.au8OnOff[mu8UV_ID] == ON)
            {
                TURN_ON_UV_10();
            }
            else
            {
                TURN_OFF_UV_10();
            }
#endif
            break;

        default:
#if (UV_COUNT >= 1)
            TURN_OFF_UV_1();
#endif
#if (UV_COUNT >= 2)
            TURN_OFF_UV_2();
#endif
#if (UV_COUNT >= 3)
            TURN_OFF_UV_3();
#endif
#if (UV_COUNT >= 4)
            TURN_OFF_UV_4();
#endif
#if (UV_COUNT >= 5)
            TURN_OFF_UV_5();
#endif
#if (UV_COUNT >= 6)
            TURN_OFF_UV_6();
#endif
#if (UV_COUNT >= 7)
            TURN_OFF_UV_7();
#endif
#if (UV_COUNT >= 8)
            TURN_OFF_UV_8();
#endif
#if (UV_COUNT >= 9)
            TURN_OFF_UV_9();
#endif
#if (UV_COUNT >= 10)
            TURN_OFF_UV_10();
#endif
            break;
    }

#endif
}

#endif


// Hal UV Module *********************************************************************************************

/// @brief      Hal UV Initilaize Module
/// @details    UV 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_UV_Module_Initialize(void)
{
#if (UV_COUNT > 0)
    Hal_UV_Initialize();                    // UV 제어 관련 초기화
#endif
}


/// @brief      Hal UV Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 UV 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_UV_Module_1ms_Control(void)
{
#if (UV_COUNT > 0)
    UV_ControlTimer();                      // UV Control Timer
#endif
}


/// @brief      Hal UV Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 UV 제어를 한다
/// @param      void
/// @return     void
void Hal_UV_Module_Control(void)
{
#if (UV_COUNT > 0)
    UV_Control();                           // UV Control 함수
#endif
}

