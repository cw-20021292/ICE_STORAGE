/// @file   Lib_OperationMode.h
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  동작 모드 주관 헤더 파일

#ifndef _LIB_OPERATION_MODE_H_
#define _LIB_OPERATION_MODE_H_


/*  Lib Operation Mode Function 사용법
    => ChangeOperationMode(mu8OperationMode);       // 동작시키려는 모드값을 설정 - EProductMode_T의 리스트 ID값
    => Get_OperationMode();                         // 현재 동작중인 모드값 확인 - EProductMode_T의 리스트 ID값
    => OperationModeSetup();                        // 현재 동작 중인 모드 내에서 동작되는 함수들을 정의해 놓는 함수
                                                       이 함수 내에 모드별로 자신이 동작시키려고 하는 함수들을 배치

    <정수기>
    => Set_OperatingStatus(mu8StatusID);            // 유로를 사용하는 동작 중 동작시키려는 모드값 설정 - EProductUseWaterStatus_T의 리스트 ID값
    => Get_OperatingStatus();                       // 유로를 사용하는 동작 중 현재 동작중인 모드값 확인 - EProductUseWaterStatus_T의 리스트 ID값
*/


// 동작 모드
/// @brief      제품 동작 상태값
/// @details    제품 동작 상태값을 구조 리스트로 정리한다
typedef enum
{
    OPERATION_MODE_ID_NONE = 0,                 // 0 : 동작 없음
    OPERATION_MODE_ID_NORMAL,                   // 1 : 일반모드
    OPERATION_MODE_ID_PCB_TEST,                 // 2 : PCB Test 모드
    OPERATION_MODE_ID_FRONT_TEST,               // 3 : Front Test 모드
    OPERATION_MODE_ID_PTA_TEST_1,               // 4 : PTA 1모드(PTA모드는 모드별로 순서 반드시 지켜서 배열할 것)
    OPERATION_MODE_ID_PTA_TEST_2,               // 5 : PTA 2모드(PTA모드는 모드별로 순서 반드시 지켜서 배열할 것)
    OPERATION_MODE_ID_PTA_TEST_3,               // 6 : PTA 3모드(PTA모드는 모드별로 순서 반드시 지켜서 배열할 것)
    OPERATION_MODE_ID_EXTRA_TEST_1,             // 7 : 여분의 테스트 모드1
    OPERATION_MODE_ID_EXTRA_TEST_2,             // 8 : 여분의 테스트 모드2
    OPERATION_MODE_ID_MAX                       // Max
} EProductMode_T;


/// @brief      제품 동작 상태값(유로를 사용하는 동작)
/// @details    제품 동작 상태값(유로를 사용하는 동작)을 구조 리스트로 정리한다
typedef enum
{
    OPERATION_ID_STAND_BY = 0,                  // 0 :
    OPERATION_ID_EFFLUENT,                      // 1
    OPERATION_ID_EW_STERILIZE,                  // 2
    OPERATION_ID_HOT_STERILIZE,                 // 3
    OPERATION_ID_DRAIN,                         // 4
    OPERATION_ID_TANK_DRAIN,                    // 5
    OPERATION_ID_INSTALL_FLUSHING,              // 6
    OPERATION_ID_FILTER_FLUSHING,               // 7
    OPERATION_ID_MAX                            // Max
} EProductUseWaterStatus_T;


void InitializeOperationMode(void);
void ChangeOperationMode(U8 mu8OperationMode);
U8 Get_OperationMode(void);
void OperationModeSetup(void);

void Set_OperatingStatus(U8 mu8StatusID);
U8 Get_OperatingStatus(void);

void Lib_OperatingMode_Module_Initialize(void);

#endif

