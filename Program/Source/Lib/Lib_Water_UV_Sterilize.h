/// @file     Lib_Water_UV_Sterilize.h
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    제품 설정 관련 제어 file


#ifndef _LIB_WATER_UV_STERILIZE_H_
#define _LIB_WATER_UV_STERILIZE_H_


/*  Lib Function 사용법
    => UV 살균 제어 동작 주기와 동작 시간을 설정해주면 SW 제어 모듈이 알아서 동작함
*/



#if (UV_COUNT > 0)

/// @brief      제품 UV 살균 동작 상태값
/// @details    제품 UV 살균 동작 상태값을 구조 리스트로 정리한다
typedef enum
{
    STERILIZE_OPERATION_ID_STAND_BY = 0,                // 0 :
    STERILIZE_OPERATION_ID_UV_WATER_TANK,               // 1 : 정수 탱크 UV 살균
    STERILIZE_OPERATION_ID_UV_COLD_TANK_BACK,           // 2 : 냉수 탱크(뒷면) UV 살균
    STERILIZE_OPERATION_ID_UV_ICE_TANK,                 // 3 : 얼음 탱크 UV 살균
    STERILIZE_OPERATION_ID_UV_ICE_TRAY,                 // 4 : 얼음 트레이 UV 살균
    STERILIZE_OPERATION_ID_UV_WATER_FAUCET,             // 5 : 물 추출부 UV 살균
    STERILIZE_OPERATION_ID_UV_ICE_FAUCET,               // 6 : 얼음 추출부 UV 살균
    STERILIZE_OPERATION_ID_UV_COLD_TANK_FRONT,          // 7 : 냉수 탱크(앞면) UV 살균
    STERILIZE_OPERATION_ID_MAX                          // 8 Max, 8이 넘으면 안됨
} EProductUVStatus_T;

#define UV_STERILIZE_OPERATION_COUNT                    7       // UV 살균 종류 개수

#define UV_STERILIZE_CONTROL_TIME_PERIOD                100     // UV 살균 제어 동작 주기 타이머 100ms@1ms
#if (UV_STERILIZE_OPERATION_COUNT >= 1)
#define UV_1_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_1_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 2)
#define UV_2_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_2_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 3)
#define UV_3_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_3_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 4)
#define UV_4_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_4_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 5)
#define UV_5_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_5_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 6)
#define UV_6_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_6_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 7)
#define UV_7_STERILIZE_START_TIME_PERIOD                360     // UV 살균 시작 동작 주기 6hour = 360min@1min
#define UV_7_STERILIZE_ON_TIME                          20      // UV 살균 동작 시간 20min@1min
#endif


void Lib_UV_Sterilize_Initialize(void);
void UV_SterilizeControlTimer(void);
void Set_UV_SterilizeStatus(U8 mu8ID, U8 mu8Status);
U8 Get_UV_SterilizeStatus(U8 mu8ID);
void UV_SterilizeStartControl(void);
void UV_SterilizeOnControl(void);
void Lib_UV_SterilizeControl(void);

void Lib_UV_Sterilize_Module_Initialize(void);
void Lib_UV_Sterilize_Module_1ms_Control(void);
void Lib_UV_Sterilize_Module_Control(void);

#endif

#endif

