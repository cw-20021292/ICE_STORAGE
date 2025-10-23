/// @file     Lib_Water_UV_Sterilize.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    제품 설정 관련 제어 file

#include "Global_Header.h"

#if (UV_COUNT > 0)

U8 gu8UV_SterilizeControlTimer = 0;         /// @brief  UV 살균 동작을 위한 제어 동작 타이머

U8 gu8UV_SterilizePeriodTimeSec = 0;        /// @brief  UV 살균 동작 시작 체크를 위한 1sec 타이머
U8 gu8UV_SterilizePeriodTimeMin = 0;        /// @brief  UV 살균 동작 시작 체크를 위한 1min 타이머

/// @brief      히팅 제어 온도 기준 데이터 구성
/// @details    히팅 제어를 하기 위한 기준 온도 데이터들을 구조체로 선언한다
typedef struct
{
    U16 au16UV_SterilizePeriodTime[STERILIZE_OPERATION_ID_MAX];     /// @brief  UV 살균 주기 타이머
    U8 au8UV_SterilizeStatus[STERILIZE_OPERATION_ID_MAX];           /// @brief  UV 살균 동작 상태
    U16 au16_UV_OnTime[STERILIZE_OPERATION_ID_MAX];                 /// @brief  UV 동작(ON) 시간
}   SUV_SterilizeData_T;

SUV_SterilizeData_T     SUV_SterilizeData;


/// @brief      UV 살균 동작 시작 주기 시간 테이블
/// @details    UV 살균 동작을 시작하는 주기 시간을 테이블로 설정해 놓는다
__far const U16 au16UV_SterilizePeroidTable[8] =
{
    0,
    UV_1_STERILIZE_START_TIME_PERIOD,
    UV_2_STERILIZE_START_TIME_PERIOD,
    UV_3_STERILIZE_START_TIME_PERIOD,
    UV_4_STERILIZE_START_TIME_PERIOD,
    UV_5_STERILIZE_START_TIME_PERIOD,
    UV_6_STERILIZE_START_TIME_PERIOD,
    UV_7_STERILIZE_START_TIME_PERIOD,
};


/// @brief      UV 살균 동작 시간 테이블
/// @details    UV 살균 동작을 진행하는 시간을 테이블로 설정해 놓는다
__far const U16 au16UV_SterilizeOnTimeTable[8] =
{
    0,
    UV_1_STERILIZE_ON_TIME,
    UV_2_STERILIZE_ON_TIME,
    UV_3_STERILIZE_ON_TIME,
    UV_4_STERILIZE_ON_TIME,
    UV_5_STERILIZE_ON_TIME,
    UV_6_STERILIZE_ON_TIME,
    UV_7_STERILIZE_ON_TIME,
};


/// @brief      UV 살균 관련 변수 초기화 함수
/// @details    UV 살균 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Initialize(void)
{
    gu8UV_SterilizeControlTimer = 0;

    gu8UV_SterilizePeriodTimeSec = 0;
    gu8UV_SterilizePeriodTimeMin = 0;

    MEMSET( (void __FAR *)&SUV_SterilizeData, 0x00, sizeof(SUV_SterilizeData_T) );
}


/// @brief      UV 살균 Control Timer(@1ms)
/// @details    UV 살균 제어를 위한 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void UV_SterilizeControlTimer(void)
{
    if (gu8UV_SterilizeControlTimer < UV_STERILIZE_CONTROL_TIME_PERIOD)
    {
        gu8UV_SterilizeControlTimer++;
    }
    else
    {
        gu8UV_SterilizeControlTimer = 0;

        if (++gu8UV_SterilizePeriodTimeSec >= 10)
        {   // 1초 카운트
            gu8UV_SterilizePeriodTimeSec = 0;

            if (gu8UV_SterilizePeriodTimeMin < 59)
            {   // 1분 카운트
                gu8UV_SterilizePeriodTimeMin++;
            }
        }
    }
}


/// @brief      UV 살균 동작 설정 함수
/// @details    UV 살균 동작 진행을 설정한다
/// @param      mu8ID : UV 살균 동작 ID
///             mu8Status : UV 살균 동작 상태 - 0(미동작), 1(동작)
/// @return     void
void Set_UV_SterilizeStatus(U8 mu8ID, U8 mu8Status)
{
    if (mu8Status)
    {
        SUV_SterilizeData.au8UV_SterilizeStatus[mu8ID] = SET;
    }
    else
    {
        SUV_SterilizeData.au8UV_SterilizeStatus[mu8ID] = CLEAR;
    }
}


/// @brief      UV 살균 동작 상태 확인 함수
/// @details    UV 살균 동작 상태를 확인하여 반환한다
/// @param      mu8ID : UV 살균 동작 ID
/// @return     return : UV 살균 동작 상태 - 0(미동작), 1(동작)
U8 Get_UV_SterilizeStatus(U8 mu8ID)
{
    if (SUV_SterilizeData.au8UV_SterilizeStatus[mu8ID])
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      UV 살균 시작 제어 함수(@1min)
/// @details    UV 살균 동작 시작 조건 판단을 한다
/// @param      void
/// @return     void
void UV_SterilizeStartControl(void)
{
    U8 mu8i = 0;

    for (mu8i = 1 ; mu8i < STERILIZE_OPERATION_ID_MAX ; mu8i++)
    {
        if (++SUV_SterilizeData.au16UV_SterilizePeriodTime[mu8i] >= au16UV_SterilizePeroidTable[mu8i])
        {   // 살균 주기 시간 마다
            SUV_SterilizeData.au16UV_SterilizePeriodTime[mu8i] = 0;
            Set_UV_SterilizeStatus(mu8i, SET);     // 살균 동작 시작 설정
        }
    }
}


/// @brief      UV 살균 동작 제어(@1min)
/// @details    UV 살균 UV ON 동작을 제어한다
/// @param      void
/// @return     void
void UV_SterilizeOnControl(void)
{
    U8 mu8i = 0;
    U8 mu8UV_SterilizeStatus = 0;

    for (mu8i = 1 ; mu8i < STERILIZE_OPERATION_ID_MAX ; mu8i++)
    {
        mu8UV_SterilizeStatus = Get_UV_SterilizeStatus(mu8i);      // 살균 동작 상태 확인

        if (mu8UV_SterilizeStatus == TRUE)
        {   // 살균 동작 중이면
            Set_UV_Control(mu8i, ON);      // UV ON

            if (++SUV_SterilizeData.au16_UV_OnTime[mu8i] >= au16UV_SterilizeOnTimeTable[mu8i])
            {   // 살균 동작 시간 완료시
                SUV_SterilizeData.au16_UV_OnTime[mu8i] = 0;
                Set_UV_SterilizeStatus(mu8i, CLEAR);                // 살균 동작 종료
            }
        }
        else
        {   // 살균 동작 중이 아니면
            Set_UV_Control(mu8i, OFF);      // UV OFF

            SUV_SterilizeData.au16_UV_OnTime[mu8i] = 0;
        }
    }
}


/// @brief      UV 살균 동작 함수
/// @details    UV 살균 동작을 제어한다
/// @param      void
/// @return     void
void Lib_UV_SterilizeControl(void)
{
    if (gu8UV_SterilizePeriodTimeMin >= 59)
    {   // 1분 경과시 마다
        gu8UV_SterilizePeriodTimeMin = 0;

        UV_SterilizeStartControl();       // UV 살균 동작 시작 판단
        UV_SterilizeOnControl();          // UV 살균 ON 제어
    }
}


// Lib Tank Water Input Module *********************************************************************************

/// @brief      Lib Heater Module Initilaize
/// @details    일반 히터 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_Initialize(void)
{
#if (UV_COUNT > 0)
    Lib_UV_Sterilize_Initialize();
#endif
}


/// @brief      Lib UV 살균 모듈 in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 UV 살균 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_1ms_Control(void)
{
#if (UV_COUNT > 0)
    UV_SterilizeControlTimer();
#endif
}


/// @brief      Lib UV 살균 제어 Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 UV 살균 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_Control(void)
{
#if (UV_COUNT > 0)
    Lib_UV_SterilizeControl();
#endif
}


#endif
