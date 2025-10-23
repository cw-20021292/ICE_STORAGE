/// @file   Lib_OperationMode.c
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  동작 모드 주관 파일

#include "Global_header.h"


U8 gu8OperaionMode = 0;                 /// @brief   현재 동작 중인 모드 - 일반, 공장 자동화, FCT 모드 등
U8 gu8ChangeOperaionMode = 0;           /// @brief   변경할 동작 모드 - 일반, 공장 자동화, FCT 모드 등

U8 gu8ProductStatus = 0;                /// @brief   제품의 현재 동작 상태(유로를 사용하는 상태)- 대기, 추출, 살균 등


/// @brief      메인 동작 모드 초기화 함수
/// @details    동작 모드 관련 데이터를 모두 초기화 시킨다
/// @param      void
/// @return     void
void InitializeOperationMode(void)
{
    gu8OperaionMode = OPERATION_MODE_ID_NONE;
    gu8ProductStatus = OPERATION_ID_STAND_BY;
}


/// @brief      동작 모드 변경 함수
/// @details    동작 모드를 변환 설정 한다
/// @param      mu8OperationMode : 동작 시킬 모드 - enum으로 선언된 동작 모드 리스트 참조
/// @return     void
void ChangeOperationMode(U8 mu8OperationMode)
{
    gu8ChangeOperaionMode = mu8OperationMode;
}

/// @brief      제품이 현재 동작 중인 모드 확인 함수
/// @details    현재 제품이 동작 중인 모드를 확인하여 반환한다
/// @param      void
/// @return     return : 현재 동작 중인 모드 - enum으로 선언된 동작 모드 리스트 참조
U8 Get_OperationMode(void)
{
    return  gu8OperaionMode;
}


/// @brief      메인 동작 제어 세팅 함수
/// @details    동작 모드에 맞춰 동작 시킬 함수들과 그 동작 타이머를 설정하여 동작이 되게 한다
/// @param      void
/// @return     void
void OperationModeSetup(void)
{
    switch (gu8ChangeOperaionMode)
    {
        case OPERATION_MODE_ID_NORMAL:         // 일반
            if (gu8OperaionMode != OPERATION_MODE_ID_NORMAL)
            {
                gu8OperaionMode = OPERATION_MODE_ID_NORMAL;
                InitializeTimeScheduler();                  // 스케쥴러 초기화
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
                SetupTimeScheduler(WifiControlProcess100ms, 100, ACT_COUNT_INFINITE);   // Wi-Fi 100ms Scheduler 동작
            }
            break;

        case OPERATION_MODE_ID_PCB_TEST:       // FCT 모드(PCB 테스트 모드)
            if (gu8OperaionMode != OPERATION_MODE_ID_PCB_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PCB_TEST;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_FRONT_TEST:     // Front 동작 검사 모드
            if (gu8OperaionMode != OPERATION_MODE_ID_FRONT_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_ID_FRONT_TEST;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:     // PTA 검사 1모드(공장 자동화 1모드)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_1)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_1;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:     // PTA 검사 2모드(공장 자동화 2모드)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_2)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_2;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:     // PTA 검사 3모드(공장 자동화 3모드)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_3)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_3;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:
            if (gu8OperaionMode != OPERATION_MODE_ID_EXTRA_TEST_1)
            {
                gu8OperaionMode = OPERATION_MODE_ID_EXTRA_TEST_1;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:
            if (gu8OperaionMode != OPERATION_MODE_ID_EXTRA_TEST_2)
            {
                gu8OperaionMode = OPERATION_MODE_ID_EXTRA_TEST_2;
                InitializeTimeScheduler();
// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
            }
            break;

        default:    // 일반 동작 모드
            if (gu8OperaionMode != OPERATION_MODE_ID_NORMAL)
            {
                gu8OperaionMode = OPERATION_MODE_ID_NORMAL;
                InitializeTimeScheduler();                  // 스케쥴러 초기화

// 예)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // 새로운 동작 함수 스케쥴 세팅
                SetupTimeScheduler(WifiControlProcess100ms, 100, ACT_COUNT_INFINITE);   // Wi-Fi 100ms Scheduler 동작
            }
            break;
    }
}


/// @brief      제품이 현재 동작 중인 상태 변경 함수
/// @details    현재 제품이 유로를 사용하는 동작 중인 상태를 설정한다
/// @param      mu8Status : 현재 동작 중인 상태(유로를 사용하는 상태) ID - enum으로 선언된 동작 리스트 참조
/// @return     void
void Set_OperatingStatus(U8 mu8StatusID)
{
    gu8ProductStatus = mu8StatusID;
}


/// @brief      제품이 현재 동작 중인 상태 확인 함수
/// @details    현재 제품이 유로를 사용하는 동작 중인 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 현재 동작 중인 상태(유로를 사용하는 상태) - enum으로 선언된 동작 리스트 참조
U8 Get_OperatingStatus(void)
{
    return  gu8ProductStatus;
}


// Lib Operating Mode Module *********************************************************************************

/// @brief      Lib Operating Mode Module Initilaize
/// @details    동작 모드 관련 데이터를 초기화 시킨다
/// @param      void
/// @return     void
void Lib_OperatingMode_Module_Initialize(void)
{
    InitializeOperationMode();              // 동작 모드 제어 관련 초기화
}



