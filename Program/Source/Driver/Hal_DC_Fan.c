/// @file     Hal_CoolingFan.c
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Cooling Fan Control file


#include "Global_Header.h"


#if (DC_FAN_COUNT > 0)

U8 gu8DC_FanControTimer = 0;                    /// @brief  DC FAN 제어 시간 타이머

/// @brief  DC Fan 제어 관리 자료형
typedef struct {
    U8  au8OnOff[DC_FAN_ID_MAX_COUNT];          // Fan Status(1:ON, 0:OFF)
}   SDC_FanData_T;

SDC_FanData_T   SDC_FanControlData;



/// @brief      DC Fan Initialize
/// @details    DC Fan 제어와 관련된 변수와 Port들을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DC_Fan_Initialize(void)
{
    MEMSET( (void __FAR *)&SDC_FanControlData, 0x00, sizeof(SDC_FanData_T) );

    gu8DC_FanControTimer = 0;

    Hal_All_DC_Fan_Off();
}


/// @brief      All DC Fan Port Off
/// @details    모든 DC Fan Port들을 OFF 설정 한다
/// @param      void
/// @return     void
void Hal_All_DC_Fan_Off(void)
{
    U8 mu8DCFanIDBuf = 0;

    for (mu8DCFanIDBuf = 1 ; mu8DCFanIDBuf < DC_FAN_ID_MAX_COUNT ; mu8DCFanIDBuf++)
    {
        Set_DC_Fan_Control(mu8DCFanIDBuf, OFF);
    }
}


/// @brief      DC Fan Control Timer(@1ms)
/// @details    DC Fan 제어 시간 카운트를 한다
/// @param      void
/// @return     void
void DC_FanControlTimer(void)
{
    if (gu8DC_FanControTimer < DC_FAN_CONTROL_TIME_PERIOD)
    {
        gu8DC_FanControTimer++;
    }
}


/// @brief      DC Fan Control(@while)
/// @details    DC Fan 제어를 While문 안에서 수행한다
/// @param      void
/// @return     void
void DC_FanControl(void)
{
    U8 mu8DCFanIDBuf = 0;

    if (gu8DC_FanControTimer >= DC_FAN_CONTROL_TIME_PERIOD)
    {       // 100ms마다 제어
        gu8DC_FanControTimer = 0;

        for (mu8DCFanIDBuf = 1 ; mu8DCFanIDBuf < DC_FAN_ID_MAX_COUNT ; mu8DCFanIDBuf++)
        {
            Hal_DC_Fan_Control(mu8DCFanIDBuf);
        }
    }
}


/// @brief      DC Fan control function
/// @details    DC Fan 제어를 설정한다
/// @param      mu8DCFanID : defined DC Fan ID
///             mu8OnOff : 동작 제어 상태 - 1(On), 0(Off)
/// @return     void
void Set_DC_Fan_Control(U8 mu8DCFanID, U8 mu8OnOff)
{
    SDC_FanControlData.au8OnOff[mu8DCFanID] = mu8OnOff;
}


/// @brief      Check Dc Fan Status
/// @details    DC Fan 동작 상태를 확인하여 반환한다
/// @param      mu8DCFanID : defined DC Fan ID
/// @return     mu8Return : 동작 제어 상태 - 1(On), 0(Off)
U8 Get_DC_Fan_Status(U8 mu8DCFanID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    switch (mu8DCFanID)
    {
        case 1:     // ID 1
#if (DC_FAN_COUNT >= 1)
            mu8Status = GET_STATUS_DC_FAN_1();
#endif
            break;

        case 2:     // ID 2
#if (DC_FAN_COUNT >= 2)
            mu8Status = GET_STATUS_DC_FAN_2();
#endif
            break;

        case 3:     // ID 3
#if (DC_FAN_COUNT >= 3)
            mu8Status = GET_STATUS_DC_FAN_3();
#endif
            break;

        case 4:     // ID 4
#if (DC_FAN_COUNT >= 4)
            mu8Status = GET_STATUS_DC_FAN_4();
#endif
            break;

        case 5:     // ID 5
#if (DC_FAN_COUNT >= 5)
            mu8Status = GET_STATUS_DC_FAN_5();
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

    return  mu8Return;
}


/// @brief      DC Fan Port Control function
/// @details    DC Fan Port의 상태를 설정한다
/// @param      mu8DCFanID : defined DC Fan ID
/// @return     void
void Hal_DC_Fan_Control(U8 mu8DCFanID)
{
    switch (mu8DCFanID)
    {
        case 1:     // ID 1
#if (DC_FAN_COUNT >= 1)
            if (SDC_FanControlData.au8OnOff[mu8DCFanID] == ON)
            {
                TURN_ON_DC_FAN_1();
            }
            else
            {
                TURN_OFF_DC_FAN_1();
            }
#endif
            break;

        case 2:     // ID 2
#if (DC_FAN_COUNT >= 2)
            if (SDC_FanControlData.au8OnOff[mu8DCFanID] == ON)
            {
                TURN_ON_DC_FAN_2();
            }
            else
            {
                TURN_OFF_DC_FAN_2();
            }
#endif
            break;

        case 3:     // ID 3
#if (DC_FAN_COUNT >= 3)
            if (SDC_FanControlData.au8OnOff[mu8DCFanID] == ON)
            {
                TURN_ON_DC_FAN_3();
            }
            else
            {
                TURN_OFF_DC_FAN_3();
            }
#endif
            break;

        case 4:     // ID 4
#if (DC_FAN_COUNT >= 4)
            if (SDC_FanControlData.au8OnOff[mu8DCFanID] == ON)
            {
                TURN_ON_DC_FAN_4();
            }
            else
            {
                TURN_OFF_DC_FAN_4();
            }
#endif
            break;

        case 5:     // ID 5
#if (DC_FAN_COUNT >= 5)
            if (SDC_FanControlData.au8OnOff[mu8DCFanID] == ON)
            {
                TURN_ON_DC_FAN_5();
            }
            else
            {
                TURN_OFF_DC_FAN_5();
            }
#endif
            break;

        default:
#if (DC_FAN_COUNT >= 1)
            TURN_OFF_DC_FAN_1();
#endif
#if (DC_FAN_COUNT >= 2)
            TURN_OFF_DC_FAN_2();
#endif
#if (DC_FAN_COUNT >= 3)
            TURN_OFF_DC_FAN_3();
#endif
#if (DC_FAN_COUNT >= 4)
            TURN_OFF_DC_FAN_4();
#endif
#if (DC_FAN_COUNT >= 5)
            TURN_OFF_DC_FAN_5();
#endif
            break;
    }
}

#endif


// Hal DC Fan Module *****************************************************************************************

/// @brief      Hal DC Fan Initilaize Module
/// @details    DC Fan 제어와 관련된 모든 변수를 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DC_Fan_Module_Initialize(void)
{
#if (DC_FAN_COUNT > 0)
    Hal_DC_Fan_Initialize();                // DC FAN 제어 관련 초기화
#endif
}


/// @brief      Hal DC Fan Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 DC Fan 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DC_Fan_Module_1ms_Control(void)
{
#if (DC_FAN_COUNT > 0)
    DC_FanControlTimer();                   // DC Fan 제어 시간 Counter
#endif
}


/// @brief      Hal DC Fan Module in While Loop
/// @details    Basic Loop 안의 While 문 안에서 DC Fan 제어를 한다
/// @param      void
/// @return     void
void Hal_DC_Fan_Module_Control(void)
{
#if (DC_FAN_COUNT > 0)
    DC_FanControl();                        // DC FAN Control 함수
#endif
}

