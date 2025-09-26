/// @file     Hal_ReedSW.h
/// @date     2025/02/25
/// @author   Jaejin Ham
/// @brief    Reed SW Control file


#ifndef _HAL_REED_SW_H_
#define _HAL_REED_SW_H_


/*  Reed Switch 제어 Function 사용법
    => Get_ReedSW_Status(Reed SW ID);

    사용 예)
    u8Return = Get_ReedSW_Status(Reed SW ID)  // Reed SW ID의 현재 확정 상태 확인 - 1(Detect), 0(Undetect)
*/

// Reed SW 최대 10개 사용 가능
typedef enum
{
    REED_SW_ID_NONE = 0,                                // 0 :
    REED_SW_ID_FILTER_COVER,                            // 1 :
    REED_SW_ID_MAX                                      // 11 Max, 11이 넘으면 안됨
} EReedSWID_T;

#define REED_SW_COUNT                                   1           // Reed SW 개수 - MAX 10

#if (REED_SW_COUNT > 0)

// Reed SW port 할당
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
#define PORT_DEFINE_REED_SW_1                           P12_bit.no2      // Reed SW 1
#define PORT_DEFINE_REED_SW_2                           0
#define PORT_DEFINE_REED_SW_3                           0
#define PORT_DEFINE_REED_SW_4                           0
#define PORT_DEFINE_REED_SW_5                           0
#define PORT_DEFINE_REED_SW_6                           0
#define PORT_DEFINE_REED_SW_7                           0
#define PORT_DEFINE_REED_SW_8                           0
#define PORT_DEFINE_REED_SW_9                           0
#define PORT_DEFINE_REED_SW_10                          0

#define TIME_DEFINE_REED_SW_1_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_2_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_3_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_4_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_5_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_6_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_7_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_8_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_9_CHATTERING                20          // 2sec@100ms
#define TIME_DEFINE_REED_SW_10_CHATTERING               20          // 2sec@100ms


#define REED_SW_ID_MAX_COUNT            REED_SW_ID_MAX

// Reed SW Port Define
#define P_REED_SW_1                     PORT_DEFINE_REED_SW_1
#define GET_STATUS_REED_SW_1()          ( P_REED_SW_1 )

#define P_REED_SW_2                     PORT_DEFINE_REED_SW_2
#define GET_STATUS_REED_SW_2()          ( P_REED_SW_2 )

#define P_REED_SW_3                     PORT_DEFINE_REED_SW_3
#define GET_STATUS_REED_SW_3()          ( P_REED_SW_3 )

#define P_REED_SW_4                     PORT_DEFINE_REED_SW_4
#define GET_STATUS_REED_SW_4()          ( P_REED_SW_4 )

#define P_REED_SW_5                     PORT_DEFINE_REED_SW_5
#define GET_STATUS_REED_SW_5()          ( P_REED_SW_5 )

#define P_REED_SW_6                     PORT_DEFINE_REED_SW_6
#define GET_STATUS_REED_SW_6()          ( P_REED_SW_6 )

#define P_REED_SW_7                     PORT_DEFINE_REED_SW_7
#define GET_STATUS_REED_SW_7()          ( P_REED_SW_7 )

#define P_REED_SW_8                     PORT_DEFINE_REED_SW_8
#define GET_STATUS_REED_SW_8()          ( P_REED_SW_8 )

#define P_REED_SW_9                     PORT_DEFINE_REED_SW_9
#define GET_STATUS_REED_SW_9()          ( P_REED_SW_9 )

#define P_REED_SW_10                    PORT_DEFINE_REED_SW_10
#define GET_STATUS_REED_SW_10()         ( P_REED_SW_10 )

#define REED_SW_CONTROL_TIME_PERIOD     100         // 100ms@1ms

#define REED_SW_1_CHATTERING_TIME       TIME_DEFINE_REED_SW_1_CHATTERING
#define REED_SW_2_CHATTERING_TIME       TIME_DEFINE_REED_SW_2_CHATTERING
#define REED_SW_3_CHATTERING_TIME       TIME_DEFINE_REED_SW_3_CHATTERING
#define REED_SW_4_CHATTERING_TIME       TIME_DEFINE_REED_SW_4_CHATTERING
#define REED_SW_5_CHATTERING_TIME       TIME_DEFINE_REED_SW_5_CHATTERING
#define REED_SW_6_CHATTERING_TIME       TIME_DEFINE_REED_SW_6_CHATTERING
#define REED_SW_7_CHATTERING_TIME       TIME_DEFINE_REED_SW_7_CHATTERING
#define REED_SW_8_CHATTERING_TIME       TIME_DEFINE_REED_SW_8_CHATTERING
#define REED_SW_9_CHATTERING_TIME       TIME_DEFINE_REED_SW_9_CHATTERING
#define REED_SW_10_CHATTERING_TIME      TIME_DEFINE_REED_SW_10_CHATTERING

// Reed SW Detect Data
#define REED_SW_1_DETECT                0x0001
#define REED_SW_2_DETECT                0x0002
#define REED_SW_3_DETECT                0x0004
#define REED_SW_4_DETECT                0x0008
#define REED_SW_5_DETECT                0x0010
#define REED_SW_6_DETECT                0x0020
#define REED_SW_7_DETECT                0x0040
#define REED_SW_8_DETECT                0x0080
#define REED_SW_9_DETECT                0x0100
#define REED_SW_10_DETECT               0x0200

void Hal_ReedSW_Initialize(void);
void Reset_ReedSWChatteringTime(U8 mu8ReedSWID);
void ReedSWControlTimer(void);
void Hal_Read_ReedSW(void);
void ReedSWControl(void);
void ConfirmReedSWData(U8 mu8ReedSWID, U8 mu8Value);
U8 Get_ReedSW_Status(U8 mu8ReedSWID);

#endif


void Hal_ReedSW_Module_Initialize(void);
void Hal_ReedSW_Module_1ms_Control(void);
void Hal_ReedSW_Module_Control(void);

#endif

