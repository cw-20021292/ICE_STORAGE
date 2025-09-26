/// @file     Hal_Valve.c
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Valve Control file


#include "Global_Header.h"


#if ( (VALVE_COUNT > 0) || (VALVE_NOS_COUNT > 0) )

U8 gu8ValveControTimer = 0;

#if (VALVE_COUNT > 0)
/// @brief  Feed Valve 제어 관리 자료형
typedef struct {
    U8  au8Status[VALVE_ID_MAX_COUNT];           // Valve Status(1:ON, 0:OFF)
    U8  au8DelayTime[VALVE_ID_MAX_COUNT];        // Valve Control Delay Time
}   SValveData_T;

SValveData_T        SValveControlData;           // Feed Valve Control Data
#endif

#if (VALVE_NOS_COUNT > 0)
/// @brief  Nos Valve 제어 관리 자료형
typedef struct {
    U8  au8Status[VALVE_NOS_ID_MAX_COUNT];       // Nos Valve Status(1:ON, 0:OFF)
    U8  au8DelayTime[VALVE_NOS_ID_MAX_COUNT];    // Nos Valve Control Delay Time
}   SNosValveData_T;

SNosValveData_T     SNosValveControlData;        // Nos Valve Control Data
#endif


/// @brief      Feed Valve Initialize
/// @details    Feed 밸브 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Valve_Initialize(void)
{
#if (VALVE_COUNT > 0)
    MEMSET( (void __FAR *)&SValveControlData, 0x00, sizeof(SValveData_T) );
    Hal_AllFeedValve_Close();
#endif

#if (VALVE_NOS_COUNT > 0)
    MEMSET( (void __FAR *)&SNosValveControlData, 0x00, sizeof(SNosValveData_T) );
    Hal_AllNosValve_Open();
#endif

    gu8ValveControTimer = 0;
}


/// @brief      Valve control Timer(@1ms interrupt)
/// @details    Feed 밸브 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void ValveControlTimer(void)
{
    if (gu8ValveControTimer < VALVE_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8ValveControTimer++;
    }
}


/// @brief      Valve control function(@While)
/// @details    Feed 밸브 제어 동작을 한다
/// @param      void
/// @return     void
void ValveControl(void)
{
    U8 mu8ValveIDBuf = 0;

    if (gu8ValveControTimer >= VALVE_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8ValveControTimer = 0;

#if (VALVE_COUNT > 0)
        for (mu8ValveIDBuf = 1 ; mu8ValveIDBuf < VALVE_ID_MAX_COUNT ; mu8ValveIDBuf++)
        {
            if (SValveControlData.au8DelayTime[mu8ValveIDBuf])
            {
                SValveControlData.au8DelayTime[mu8ValveIDBuf]--;
            }

            Hal_ValveControl(mu8ValveIDBuf);
        }
#endif

#if (VALVE_NOS_COUNT > 0)
        for (mu8ValveIDBuf = 1 ; mu8ValveIDBuf < VALVE_NOS_ID_MAX_COUNT ; mu8ValveIDBuf++)
        {
            if (SNosValveControlData.au8DelayTime[mu8ValveIDBuf])
            {
                SNosValveControlData.au8DelayTime[mu8ValveIDBuf]--;
            }

            Hal_NosValveControl(mu8ValveIDBuf);
        }
#endif

    }
}


/// @brief      Valve control function
/// @details    Feed 밸브 제어 상태값을 설정한다
/// @param      mu8ValveID : defined Valve ID
///             mu8ValveType : Valve Type(0:Feed, 1:Nos)
///             mu8Status : 밸브 동작 상태 - 1(Open), 0(Close)
///             mu8Delay : Control Delay Time(@100ms)
/// @return     void
void Set_ValveControl(U8 mu8ValveID, U8 mu8ValveType, U8 mu8Status, U8 mu8Delay)
{
    switch (mu8ValveType)
    {
#if (VALVE_COUNT > 0)
        case FEED:      // Feed Valve인 경우

            if (mu8Status == OPEN)
            {
                SValveControlData.au8Status[mu8ValveID] = ON;
                SValveControlData.au8DelayTime[mu8ValveID] = mu8Delay;
            }
            else
            {
                SValveControlData.au8Status[mu8ValveID] = OFF;
                SValveControlData.au8DelayTime[mu8ValveID] = mu8Delay;
            }
            break;
#endif

#if (VALVE_NOS_COUNT > 0)
        case NOS:       // Nos Valve인 경우
            if (mu8Status == OPEN)
            {
                SNosValveControlData.au8Status[mu8ValveID] = N_ON;
                SNosValveControlData.au8DelayTime[mu8ValveID] = mu8Delay;
            }
            else
            {
                SNosValveControlData.au8Status[mu8ValveID] = N_OFF;
                SNosValveControlData.au8DelayTime[mu8ValveID] = mu8Delay;
            }
            break;
#endif

        default:
#if (VALVE_COUNT > 0)
            SValveControlData.au8Status[mu8ValveID] = OFF;
            SValveControlData.au8DelayTime[mu8ValveID] = 0;
#endif

#if (VALVE_NOS_COUNT > 0)
            SNosValveControlData.au8Status[mu8ValveID] = N_ON;
            SNosValveControlData.au8DelayTime[mu8ValveID] = 0;
#endif

            break;
    }
}


#if (VALVE_COUNT > 0)
/// @brief      All Feed Valve OFF(Close)
/// @details    모든 Feed 밸브 전원을 OFF 시킨다
/// @param      void
/// @return     void
void Hal_AllFeedValve_Close(void)
{
    U8 mu8ValveIDBuf = 0;

    for (mu8ValveIDBuf = 1 ; mu8ValveIDBuf < VALVE_ID_MAX_COUNT ; mu8ValveIDBuf++)
    {
        Set_ValveControl(mu8ValveIDBuf, FEED, CLOSE, 0);
    }
}


/// @brief      Valve Port control function
/// @details    Feed 밸브 HW 제어 동작을 한다
/// @param      mu8ValveID : defined Valve ID
/// @return     void
void Hal_ValveControl(U8 mu8ValveID)
{
    switch (mu8ValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_COUNT >= 1)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_1();
                }
                else
                {
                    TURN_OFF_VALVE_1();
                }
            }
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_COUNT >= 2)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_2();
                }
                else
                {
                    TURN_OFF_VALVE_2();
                }
            }
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_COUNT >= 3)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_3();
                }
                else
                {
                    TURN_OFF_VALVE_3();
                }
            }
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_COUNT >= 4)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_4();
                }
                else
                {
                    TURN_OFF_VALVE_4();
                }
            }
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_COUNT >= 5)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_5();
                }
                else
                {
                    TURN_OFF_VALVE_5();
                }
            }
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_COUNT >= 6)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_6();
                }
                else
                {
                    TURN_OFF_VALVE_6();
                }
            }
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_COUNT >= 7)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_7();
                }
                else
                {
                    TURN_OFF_VALVE_7();
                }
            }
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_COUNT >= 8)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_8();
                }
                else
                {
                    TURN_OFF_VALVE_8();
                }
            }
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_COUNT >= 9)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_9();
                }
                else
                {
                    TURN_OFF_VALVE_9();
                }
            }
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_COUNT >= 10)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_10();
                }
                else
                {
                    TURN_OFF_VALVE_10();
                }
            }
#endif
            break;

        case 11:     // Valve ID 11
#if (VALVE_COUNT >= 11)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_11();
                }
                else
                {
                    TURN_OFF_VALVE_11();
                }
            }
#endif
            break;

        case 12:     // Valve ID 12
#if (VALVE_COUNT >= 12)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_12();
                }
                else
                {
                    TURN_OFF_VALVE_12();
                }
            }
#endif
            break;

        case 13:     // Valve ID 13
#if (VALVE_COUNT >= 13)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_13();
                }
                else
                {
                    TURN_OFF_VALVE_13();
                }
            }
#endif
            break;

        case 14:     // Valve ID 14
#if (VALVE_COUNT >= 14)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_14();
                }
                else
                {
                    TURN_OFF_VALVE_14();
                }
            }
#endif
            break;

        case 15:     // Valve ID 15
#if (VALVE_COUNT >= 15)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_15();
                }
                else
                {
                    TURN_OFF_VALVE_15();
                }
            }
#endif
            break;

        case 16:     // Valve ID 16
#if (VALVE_COUNT >= 16)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_16();
                }
                else
                {
                    TURN_OFF_VALVE_16();
                }
            }
#endif
            break;

        case 17:     // Valve ID 17
#if (VALVE_COUNT >= 17)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_17();
                }
                else
                {
                    TURN_OFF_VALVE_17();
                }
            }
#endif
            break;

        case 18:     // Valve ID 18
#if (VALVE_COUNT >= 18)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_18();
                }
                else
                {
                    TURN_OFF_VALVE_18();
                }
            }
#endif
            break;

        case 19:     // Valve ID 19
#if (VALVE_COUNT >= 19)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_19();
                }
                else
                {
                    TURN_OFF_VALVE_19();
                }
            }
#endif
            break;

        case 20:     // Valve ID 20
#if (VALVE_COUNT >= 20)
            if (SValveControlData.au8DelayTime[mu8ValveID] == 0)
            {
                if (SValveControlData.au8Status[mu8ValveID] == ON)
                {
                    TURN_ON_VALVE_20();
                }
                else
                {
                    TURN_OFF_VALVE_20();
                }
            }
#endif
            break;

        default:
#if (VALVE_COUNT >= 1)
            TURN_OFF_VALVE_1();
#endif
#if (VALVE_COUNT >= 2)
            TURN_OFF_VALVE_2();
#endif
#if (VALVE_COUNT >= 3)
            TURN_OFF_VALVE_3();
#endif
#if (VALVE_COUNT >= 4)
            TURN_OFF_VALVE_4();
#endif
#if (VALVE_COUNT >= 5)
            TURN_OFF_VALVE_5();
#endif
#if (VALVE_COUNT >= 6)
            TURN_OFF_VALVE_6();
#endif
#if (VALVE_COUNT >= 7)
            TURN_OFF_VALVE_7();
#endif
#if (VALVE_COUNT >= 8)
            TURN_OFF_VALVE_8();
#endif
#if (VALVE_COUNT >= 9)
            TURN_OFF_VALVE_9();
#endif
#if (VALVE_COUNT >= 10)
            TURN_OFF_VALVE_10();
#endif
#if (VALVE_COUNT >= 11)
            TURN_OFF_VALVE_11();
#endif
#if (VALVE_COUNT >= 12)
            TURN_OFF_VALVE_12();
#endif
#if (VALVE_COUNT >= 13)
            TURN_OFF_VALVE_13();
#endif
#if (VALVE_COUNT >= 14)
            TURN_OFF_VALVE_14();
#endif
#if (VALVE_COUNT >= 15)
            TURN_OFF_VALVE_15();
#endif
#if (VALVE_COUNT >= 16)
            TURN_OFF_VALVE_16();
#endif
#if (VALVE_COUNT >= 17)
            TURN_OFF_VALVE_17();
#endif
#if (VALVE_COUNT >= 18)
            TURN_OFF_VALVE_18();
#endif
#if (VALVE_COUNT >= 19)
            TURN_OFF_VALVE_19();
#endif
#if (VALVE_COUNT >= 20)
            TURN_OFF_VALVE_20();
#endif
            break;
    }
}


/// @brief      Valve Port direct control function
/// @details    Feed 밸브 HW 전원 제어를 상태값 설정이 아닌 Port를 직접 제어한다
/// @param      mu8ValveID : defined Valve ID
///             mu8OnOff : 동작 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_ValvePort(U8 mu8ValveID, U8 mu8OnOff)
{
    switch (mu8ValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_COUNT >= 1)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_1();
            }
            else
            {
                TURN_OFF_VALVE_1();
            }
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_COUNT >= 2)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_2();
            }
            else
            {
                TURN_OFF_VALVE_2();
            }
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_COUNT >= 3)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_3();
            }
            else
            {
                TURN_OFF_VALVE_3();
            }
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_COUNT >= 4)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_4();
            }
            else
            {
                TURN_OFF_VALVE_4();
            }
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_COUNT >= 5)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_5();
            }
            else
            {
                TURN_OFF_VALVE_5();
            }
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_COUNT >= 6)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_6();
            }
            else
            {
                TURN_OFF_VALVE_6();
            }
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_COUNT >= 7)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_7();
            }
            else
            {
                TURN_OFF_VALVE_7();
            }
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_COUNT >= 8)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_8();
            }
            else
            {
                TURN_OFF_VALVE_8();
            }
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_COUNT >= 9)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_9();
            }
            else
            {
                TURN_OFF_VALVE_9();
            }
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_COUNT >= 10)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_10();
            }
            else
            {
                TURN_OFF_VALVE_10();
            }
#endif
            break;

        case 11:     // Valve ID 11
#if (VALVE_COUNT >= 11)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_11();
            }
            else
            {
                TURN_OFF_VALVE_11();
            }
#endif
            break;

        case 12:     // Valve ID 12
#if (VALVE_COUNT >= 12)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_12();
            }
            else
            {
                TURN_OFF_VALVE_12();
            }
#endif
            break;

        case 13:     // Valve ID 13
#if (VALVE_COUNT >= 13)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_13();
            }
            else
            {
                TURN_OFF_VALVE_13();
            }
#endif
            break;

        case 14:     // Valve ID 14
#if (VALVE_COUNT >= 14)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_14();
            }
            else
            {
                TURN_OFF_VALVE_14();
            }
#endif
            break;

        case 15:     // Valve ID 15
#if (VALVE_COUNT >= 15)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_15();
            }
            else
            {
                TURN_OFF_VALVE_15();
            }
#endif
            break;

        case 16:     // Valve ID 16
#if (VALVE_COUNT >= 16)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_16();
            }
            else
            {
                TURN_OFF_VALVE_16();
            }
#endif
            break;

        case 17:     // Valve ID 17
#if (VALVE_COUNT >= 17)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_17();
            }
            else
            {
                TURN_OFF_VALVE_17();
            }
#endif
            break;

        case 18:     // Valve ID 18
#if (VALVE_COUNT >= 18)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_18();
            }
            else
            {
                TURN_OFF_VALVE_18();
            }
#endif
            break;

        case 19:     // Valve ID 19
#if (VALVE_COUNT >= 19)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_19();
            }
            else
            {
                TURN_OFF_VALVE_19();
            }
#endif
            break;

        case 20:     // Valve ID 20
#if (VALVE_COUNT >= 20)
            if (mu8OnOff)
            {
                TURN_ON_VALVE_20();
            }
            else
            {
                TURN_OFF_VALVE_20();
            }
#endif
            break;

        default:
#if (VALVE_COUNT >= 1)
            TURN_OFF_VALVE_1();
#endif
#if (VALVE_COUNT >= 2)
            TURN_OFF_VALVE_2();
#endif
#if (VALVE_COUNT >= 3)
            TURN_OFF_VALVE_3();
#endif
#if (VALVE_COUNT >= 4)
            TURN_OFF_VALVE_4();
#endif
#if (VALVE_COUNT >= 5)
            TURN_OFF_VALVE_5();
#endif
#if (VALVE_COUNT >= 6)
            TURN_OFF_VALVE_6();
#endif
#if (VALVE_COUNT >= 7)
            TURN_OFF_VALVE_7();
#endif
#if (VALVE_COUNT >= 8)
            TURN_OFF_VALVE_8();
#endif
#if (VALVE_COUNT >= 9)
            TURN_OFF_VALVE_9();
#endif
#if (VALVE_COUNT >= 10)
            TURN_OFF_VALVE_10();
#endif
#if (VALVE_COUNT >= 11)
            TURN_OFF_VALVE_11();
#endif
#if (VALVE_COUNT >= 12)
            TURN_OFF_VALVE_12();
#endif
#if (VALVE_COUNT >= 13)
            TURN_OFF_VALVE_13();
#endif
#if (VALVE_COUNT >= 14)
            TURN_OFF_VALVE_14();
#endif
#if (VALVE_COUNT >= 15)
            TURN_OFF_VALVE_15();
#endif
#if (VALVE_COUNT >= 16)
            TURN_OFF_VALVE_16();
#endif
#if (VALVE_COUNT >= 17)
            TURN_OFF_VALVE_17();
#endif
#if (VALVE_COUNT >= 18)
            TURN_OFF_VALVE_18();
#endif
#if (VALVE_COUNT >= 19)
            TURN_OFF_VALVE_19();
#endif
#if (VALVE_COUNT >= 20)
            TURN_OFF_VALVE_20();
#endif
            break;
    }
}


/// @brief      Check Valve Status
/// @details    ID에 해당하는 밸브의 동작 상태를 확인하여 그 상태를 반환한다
/// @param      mu8ValveID : defined valve ID
/// @return     return : 밸브 동작 상태 - 1(On), 0(Off)
U8 Get_ValveStatus(U8 mu8ValveID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    switch (mu8ValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_COUNT >= 1)
            mu8Status = GET_STATUS_VALVE_1();
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_COUNT >= 2)
            mu8Status = GET_STATUS_VALVE_2();
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_COUNT >= 3)
            mu8Status = GET_STATUS_VALVE_3();
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_COUNT >= 4)
            mu8Status = GET_STATUS_VALVE_4();
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_COUNT >= 5)
            mu8Status = GET_STATUS_VALVE_5();
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_COUNT >= 6)
            mu8Status = GET_STATUS_VALVE_6();
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_COUNT >= 7)
            mu8Status = GET_STATUS_VALVE_7();
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_COUNT >= 8)
            mu8Status = GET_STATUS_VALVE_8();
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_COUNT >= 9)
            mu8Status = GET_STATUS_VALVE_9();
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_COUNT >= 10)
            mu8Status = GET_STATUS_VALVE_10();
#endif
            break;

        case 11:     // Valve ID 11
#if (VALVE_COUNT >= 11)
            mu8Status = GET_STATUS_VALVE_11();
#endif
            break;

        case 12:     // Valve ID 12
#if (VALVE_COUNT >= 12)
            mu8Status = GET_STATUS_VALVE_12();
#endif
            break;

        case 13:     // Valve ID 13
#if (VALVE_COUNT >= 13)
            mu8Status = GET_STATUS_VALVE_13();
#endif
            break;

        case 14:     // Valve ID 14
#if (VALVE_COUNT >= 14)
            mu8Status = GET_STATUS_VALVE_14();
#endif
            break;

        case 15:     // Valve ID 15
#if (VALVE_COUNT >= 15)
            mu8Status = GET_STATUS_VALVE_15();
#endif
            break;

        case 16:     // Valve ID 16
#if (VALVE_COUNT >= 16)
            mu8Status = GET_STATUS_VALVE_16();
#endif
            break;

        case 17:     // Valve ID 17
#if (VALVE_COUNT >= 17)
            mu8Status = GET_STATUS_VALVE_17();
#endif
            break;

        case 18:     // Valve ID 18
#if (VALVE_COUNT >= 18)
            mu8Status = GET_STATUS_VALVE_18();
#endif
            break;

        case 19:     // Valve ID 19
#if (VALVE_COUNT >= 19)
            mu8Status = GET_STATUS_VALVE_19();
#endif
            break;

        case 20:     // Valve ID 20
#if (VALVE_COUNT >= 20)
            mu8Status = GET_STATUS_VALVE_20();
#endif
            break;

        default:
            mu8Status = 0;
            break;
    }

    if (mu8Status)
    {
        mu8Return = ON;
    }
    else
    {
        mu8Return = OFF;
    }

    return  mu8Return;
}


#endif


#if (VALVE_NOS_COUNT > 0)
/// @brief      All Nos Valve OFF(Open)
/// @details    모든 Nos 밸브의 전원을 OFF 시킨다
/// @param      void
/// @return     void
void Hal_AllNosValve_Open(void)
{
    U8 mu8ValveIDBuf = 0;

    for (mu8ValveIDBuf = 1 ; mu8ValveIDBuf < VALVE_NOS_ID_MAX_COUNT ; mu8ValveIDBuf++)
    {
        Set_ValveControl(mu8ValveIDBuf, NOS, OPEN, 0);
    }
}


/// @brief      Nos Valve Port control function
/// @details    Nos 밸브의 HW 제어 동작을 한다
/// @param      mu8NosValveID : defined Nos Valve ID
/// @return     void
void Hal_NosValveControl(U8 mu8NosValveID)
{
    switch (mu8NosValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_NOS_COUNT >= 1)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_1();
                }
                else
                {
                    TURN_ON_NOS_VALVE_1();
                }
            }
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_NOS_COUNT >= 2)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_2();
                }
                else
                {
                    TURN_ON_NOS_VALVE_2();
                }
            }
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_NOS_COUNT >= 3)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_3();
                }
                else
                {
                    TURN_ON_NOS_VALVE_3();
                }
            }
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_NOS_COUNT >= 4)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_4();
                }
                else
                {
                    TURN_ON_NOS_VALVE_4();
                }
            }
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_NOS_COUNT >= 5)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_5();
                }
                else
                {
                    TURN_ON_NOS_VALVE_5();
                }
            }
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_NOS_COUNT >= 6)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_6();
                }
                else
                {
                    TURN_ON_NOS_VALVE_6();
                }
            }
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_NOS_COUNT >= 7)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_7();
                }
                else
                {
                    TURN_ON_NOS_VALVE_7();
                }
            }
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_NOS_COUNT >= 8)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_8();
                }
                else
                {
                    TURN_ON_NOS_VALVE_8();
                }
            }
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_NOS_COUNT >= 9)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_9();
                }
                else
                {
                    TURN_ON_NOS_VALVE_9();
                }
            }
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_NOS_COUNT >= 10)
            if (SNosValveControlData.au8DelayTime[mu8NosValveID] == 0)
            {
                if (SNosValveControlData.au8Status[mu8NosValveID] == N_ON)
                {
                    TURN_OFF_NOS_VALVE_10();
                }
                else
                {
                    TURN_ON_NOS_VALVE_10();
                }
            }
#endif
            break;

        default:
#if (VALVE_NOS_COUNT >= 1)
            TURN_OFF_NOS_VALVE_1();
#endif
#if (VALVE_NOS_COUNT >= 2)
            TURN_OFF_NOS_VALVE_2();
#endif
#if (VALVE_NOS_COUNT >= 3)
            TURN_OFF_NOS_VALVE_3();
#endif
#if (VALVE_NOS_COUNT >= 4)
            TURN_OFF_NOS_VALVE_4();
#endif
#if (VALVE_NOS_COUNT >= 5)
            TURN_OFF_NOS_VALVE_5();
#endif
#if (VALVE_NOS_COUNT >= 6)
            TURN_OFF_NOS_VALVE_6();
#endif
#if (VALVE_NOS_COUNT >= 7)
            TURN_OFF_NOS_VALVE_7();
#endif
#if (VALVE_NOS_COUNT >= 8)
            TURN_OFF_NOS_VALVE_8();
#endif
#if (VALVE_NOS_COUNT >= 9)
            TURN_OFF_NOS_VALVE_9();
#endif
#if (VALVE_NOS_COUNT >= 10)
            TURN_OFF_NOS_VALVE_10();
#endif
            break;
    }
}


/// @brief      Nos Valve Port direct control function
/// @details    Nos 밸브 HW 제어 동작을 한다
/// @param      mu8ValveID : defined Valve ID
///             mu8OnOff : 동작 상태- 0(OFF), 1(ON)
/// @return     void
void Set_NosValvePort(U8 mu8NosValveID, U8 mu8OnOff)
{
    switch (mu8NosValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_NOS_COUNT >= 1)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_1();
            }
            else
            {
                TURN_OFF_NOS_VALVE_1();
            }
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_NOS_COUNT >= 2)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_2();
            }
            else
            {
                TURN_OFF_NOS_VALVE_2();
            }
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_NOS_COUNT >= 3)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_3();
            }
            else
            {
                TURN_OFF_NOS_VALVE_3();
            }
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_NOS_COUNT >= 4)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_4();
            }
            else
            {
                TURN_OFF_NOS_VALVE_4();
            }
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_NOS_COUNT >= 5)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_5();
            }
            else
            {
                TURN_OFF_NOS_VALVE_5();
            }
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_NOS_COUNT >= 6)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_6();
            }
            else
            {
                TURN_OFF_NOS_VALVE_6();
            }
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_NOS_COUNT >= 7)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_7();
            }
            else
            {
                TURN_OFF_NOS_VALVE_7();
            }
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_NOS_COUNT >= 8)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_8();
            }
            else
            {
                TURN_OFF_NOS_VALVE_8();
            }
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_NOS_COUNT >= 9)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_9();
            }
            else
            {
                TURN_OFF_NOS_VALVE_9();
            }
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_NOS_COUNT >= 10)
            if (mu8OnOff)
            {
                TURN_ON_NOS_VALVE_10();
            }
            else
            {
                TURN_OFF_NOS_VALVE_10();
            }
#endif
            break;

        default:
#if (VALVE_NOS_COUNT >= 1)
            TURN_OFF_NOS_VALVE_1();
#endif
#if (VALVE_NOS_COUNT >= 2)
            TURN_OFF_NOS_VALVE_2();
#endif
#if (VALVE_NOS_COUNT >= 3)
            TURN_OFF_NOS_VALVE_3();
#endif
#if (VALVE_NOS_COUNT >= 4)
            TURN_OFF_NOS_VALVE_4();
#endif
#if (VALVE_NOS_COUNT >= 5)
            TURN_OFF_NOS_VALVE_5();
#endif
#if (VALVE_NOS_COUNT >= 6)
            TURN_OFF_NOS_VALVE_6();
#endif
#if (VALVE_NOS_COUNT >= 7)
            TURN_OFF_NOS_VALVE_7();
#endif
#if (VALVE_NOS_COUNT >= 8)
            TURN_OFF_NOS_VALVE_8();
#endif
#if (VALVE_NOS_COUNT >= 9)
            TURN_OFF_NOS_VALVE_9();
#endif
#if (VALVE_NOS_COUNT >= 10)
            TURN_OFF_NOS_VALVE_10();
#endif
            break;
    }
}


/// @brief      Check Nos Valve Status
/// @details    ID에 해당하는 NOS 밸브의 동작 상태를 확인하여 그 상태를 반환한다
/// @param      mu8ValveID : defined valve ID
/// @return     return : 밸브 동작 상태 - 1(On), 0(Off)
U8 Get_NosValveStatus(U8 mu8ValveID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    switch (mu8ValveID)
    {
        case 1:     // Valve ID 1
#if (VALVE_NOS_COUNT >= 1)
            mu8Status = GET_STATUS_NOS_VALVE_1();
#endif
            break;

        case 2:     // Valve ID 2
#if (VALVE_NOS_COUNT >= 2)
            mu8Status = GET_STATUS_NOS_VALVE_2();
#endif
            break;

        case 3:     // Valve ID 3
#if (VALVE_NOS_COUNT >= 3)
            mu8Status = GET_STATUS_NOS_VALVE_3();
#endif
            break;

        case 4:     // Valve ID 4
#if (VALVE_NOS_COUNT >= 4)
            mu8Status = GET_STATUS_NOS_VALVE_4();
#endif
            break;

        case 5:     // Valve ID 5
#if (VALVE_NOS_COUNT >= 5)
            mu8Status = GET_STATUS_NOS_VALVE_5();
#endif
            break;

        case 6:     // Valve ID 6
#if (VALVE_NOS_COUNT >= 6)
            mu8Status = GET_STATUS_NOS_VALVE_6();
#endif
            break;

        case 7:     // Valve ID 7
#if (VALVE_NOS_COUNT >= 7)
            mu8Status = GET_STATUS_NOS_VALVE_7();
#endif
            break;

        case 8:     // Valve ID 8
#if (VALVE_NOS_COUNT >= 8)
            mu8Status = GET_STATUS_NOS_VALVE_8();
#endif
            break;

        case 9:     // Valve ID 9
#if (VALVE_NOS_COUNT >= 9)
            mu8Status = GET_STATUS_NOS_VALVE_9();
#endif
            break;

        case 10:     // Valve ID 10
#if (VALVE_NOS_COUNT >= 10)
            mu8Status = GET_STATUS_NOS_VALVE_10();
#endif
            break;

        default:
            mu8Status = 0;
            break;
    }

    if (mu8Status)
    {
        mu8Return = ON;
    }
    else
    {
        mu8Return = OFF;
    }

    return  mu8Return;
}

#endif

#endif


// Hal Valve Module ******************************************************************************************

/// @brief      Hal Valve Initilaize Module
/// @details    일반 밸브 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Valve_Module_Initialize(void)
{
#if ( (VALVE_COUNT > 0) || (VALVE_NOS_COUNT > 0) )
    Hal_Valve_Initialize();                 // 밸브 제어 관련 초기화
#endif
}


/// @brief      Hal Valve Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 일반 밸브 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_Valve_Module_1ms_Control(void)
{
#if ( (VALVE_COUNT > 0) || (VALVE_NOS_COUNT > 0) )
    ValveControlTimer();                    // 밸브 제어 시간 Counter
#endif
}


/// @brief      Hal Valve Module in While Loop
/// @details    Basic Loop 안 While문 안에서 일반 밸브 제어를 한다
/// @param      void
/// @return     void
void Hal_Valve_Module_Control(void)
{
#if ( (VALVE_COUNT > 0) || (VALVE_NOS_COUNT > 0) )
    ValveControl();                         // 밸브류 Control 함수
#endif
}

