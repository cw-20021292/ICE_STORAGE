/// @file     Hal_Stepmotor.c
/// @date     2025/03/12
/// @author   Jaejin Ham
/// @brief    Step Motor 동작 제어 file


#include "Global_Header.h"

#if (STEPMOTOR_COUNT > 0)

/// @brief  Stepmotor 제어 관리 자료형
typedef struct {
    U8 au8Direction[STEPMOTOR_ID_MAX_COUNT];            /// @brief  모터 제어 방향
    U8 au8StepCycle[STEPMOTOR_ID_MAX_COUNT];            /// @brief  모터 제어 핀 동작 Cycle
    U8 au8Move_PPS[STEPMOTOR_ID_MAX_COUNT];             /// @brief  모더를 제어하는 PPS 기준 동작 시간
    U8 au8Cnt_PPS[STEPMOTOR_ID_MAX_COUNT];              /// @brief  모터를 제어하는 PPS 제어 시간
    U16 au16MovingPulse[STEPMOTOR_ID_MAX_COUNT];        /// @brief  모터가 움직일 PPS 값
}   SStepmotorControlData_T;

SStepmotorControlData_T   SStepmotorData;

CU8 au8Step_CW[8] ={1, 2, 3, 4, 5, 6, 7, 8};            /// @brief  Stepmotor CW 제어 단계
CU8 au8Step_CCW[8] = {8, 7, 6, 5, 4, 3, 2, 1};          /// @brief  Stepmotor CCW 제어 단계


/// @brief      Stepmotor 제어 관련 변수 모두 초기화
/// @details    Stepmotor 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Stepmotor_Initialize(void)
{
    U8 mu8StepmotorBuf = 0;

    MEMSET( (void __FAR *)&SStepmotorData, 0x00, sizeof(SStepmotorControlData_T) );

    for (mu8StepmotorBuf = 1 ; mu8StepmotorBuf < STEPMOTOR_ID_MAX_COUNT ; mu8StepmotorBuf++)
    {
        Hal_Stepmotor_Stop(mu8StepmotorBuf);        // port Initialize

#if (STEPMOTOR_COUNT >= 1)
        if (mu8StepmotorBuf == 1)
        {
            SStepmotorData.au8Move_PPS[mu8StepmotorBuf] = STEPMOTOR_1_CONTROL_PPS_MS;
        }
#if (STEPMOTOR_COUNT >= 2)
        else if (mu8StepmotorBuf == 2)
        {
            SStepmotorData.au8Move_PPS[mu8StepmotorBuf] = STEPMOTOR_2_CONTROL_PPS_MS;
        }
#endif
#if (STEPMOTOR_COUNT >= 3)
        else if (mu8StepmotorBuf == 3)
        {
            SStepmotorData.au8Move_PPS[mu8StepmotorBuf] = STEPMOTOR_3_CONTROL_PPS_MS;
        }
#endif
#if (STEPMOTOR_COUNT >= 4)
        else if (mu8StepmotorBuf == 4)
        {
            SStepmotorData.au8Move_PPS[mu8StepmotorBuf] = STEPMOTOR_4_CONTROL_PPS_MS;
        }
#endif
#if (STEPMOTOR_COUNT >= 5)
        else if (mu8StepmotorBuf == 5)
        {
            SStepmotorData.au8Move_PPS[mu8StepmotorBuf] = STEPMOTOR_5_CONTROL_PPS_MS;
        }
#endif
        else
        {
        }
#endif
    }
}


/// @brief      Stepmotor Moving Set 함수
/// @details    Stepmotor 동작 관련 데이터를 설정한다
/// @param      mu8MotorID : 제어하려는 Stepmotor ID
///             mu8Direction : 제어하려는 방향 - 0(CW), 1(CCW)
///             mu16Pulse : 제어하려는 Pulse Count
/// @return     void
void Set_Stepmotor_Move(U8 mu8MotorID, U8 mu8Direction, U16 mu16Pulse)
{
    SStepmotorData.au8Direction[mu8MotorID] = mu8Direction;
    SStepmotorData.au16MovingPulse[mu8MotorID] = mu16Pulse;
}


/// @brief      Stepmotor Moving 상태 확인 함수
/// @details    Stepmotor가 동작 중인지 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     mu8Return : 제어 동작 상태 여부 - 0(Stop), 1(Moving)
U8 Get_Stepmotor_Move_Status(U8 mu8MotorID)
{
    U8 mu8Return = 0;

    if (SStepmotorData.au16MovingPulse[mu8MotorID])
    {
        mu8Return = TRUE;
    }
    else
    {
        mu8Return = FALSE;
    }

    return  mu8Return;
}


/// @brief      Stepmotor Control 함수(@1ms interrupt)
/// @details    1ms 인터럽트 안에서 Stepmotor 관련 제어를 카운트 한다
/// @param      void
/// @return     void
void Hal_StepmotorControl(void)
{
    U8 mu8StepmotorBuf = 0;

    for (mu8StepmotorBuf = 1 ; mu8StepmotorBuf < STEPMOTOR_ID_MAX_COUNT ; mu8StepmotorBuf++)
    {
        if (SStepmotorData.au16MovingPulse[mu8StepmotorBuf] != 0)
        {
            if (++SStepmotorData.au8Cnt_PPS[mu8StepmotorBuf] >= SStepmotorData.au8Move_PPS[mu8StepmotorBuf])
            {
                SStepmotorData.au8Cnt_PPS[mu8StepmotorBuf] = 0;

                SStepmotorData.au16MovingPulse[mu8StepmotorBuf]--;

                Stepmotor_Control(mu8StepmotorBuf);

                if (++SStepmotorData.au8StepCycle[mu8StepmotorBuf] >= 8)
                {
                    SStepmotorData.au8StepCycle[mu8StepmotorBuf] = 0;
                }
            }
        }
        else
        {
            Hal_Stepmotor_Stop(mu8StepmotorBuf);
        }
    }
}


/// @brief      Stepmotor 제어 함수
/// @details    Stepmotor를 제어하는 동작을 한다
/// @param      mu8MotorID : 제어하는 Stepmotor ID
/// @return     void
void Stepmotor_Control(U8 mu8MotorID)
{
    U8 mu8Step_Order = 0;

    if (SStepmotorData.au8Direction[mu8MotorID] == STEPMOTOR_CCW)
    {
        mu8Step_Order = au8Step_CCW[SStepmotorData.au8StepCycle[mu8MotorID]];
    }
    else
    {
        mu8Step_Order = au8Step_CW[SStepmotorData.au8StepCycle[mu8MotorID]];
    }

    switch (mu8MotorID)
    {
        case 1:
            Stepmotor_1_Control(mu8Step_Order);
            break;

        case 2:
            Stepmotor_2_Control(mu8Step_Order);
            break;

        case 3:
            Stepmotor_3_Control(mu8Step_Order);
            break;

        case 4:
            Stepmotor_4_Control(mu8Step_Order);
            break;

        case 5:
            Stepmotor_5_Control(mu8Step_Order);
            break;

        default:
            break;
    }
}


/// @brief      Stepmotor 1 Port 제어 함수
/// @details    Stepmotor 1을 제어하는 HW Port를 제어한다.
/// @param      mu8Step : 제어 단계
/// @return     void
void Stepmotor_1_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 1)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 4:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;
      }
#elif (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_1_2_PHASE_EXCITATION)
// 1-2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 2:
            TURN_ON_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 5:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;
      }
#else
// 1상 여자
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_ON_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_ON_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;

        case 8:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_ON_STEPMOTOR_1_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
            break;
      }
#endif

#endif
}


/// @brief      Stepmotor 2 Port 제어 함수
/// @details    Stepmotor 2를 제어하는 HW Port를 제어한다.
/// @param      mu8Step : 제어 단계
/// @return     void
void Stepmotor_2_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 2)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 4:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;
      }
#elif (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_1_2_PHASE_EXCITATION)
// 1-2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 2:
            TURN_ON_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 5:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;
    }

#else
// 1상 여자
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_ON_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_ON_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;

        case 8:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_ON_STEPMOTOR_2_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
            break;
      }
#endif

#endif
}


/// @brief      Stepmotor 3 Port 제어 함수
/// @details    Stepmotor 3을 제어하는 HW Port를 제어한다.
/// @param      mu8Step : 제어 단계
/// @return     void
void Stepmotor_3_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 3)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 4:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;
      }
#elif (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_1_2_PHASE_EXCITATION)
// 1-2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 2:
            TURN_ON_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 5:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;
    }

#else
// 1상 여자
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_ON_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_ON_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;

        case 8:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_ON_STEPMOTOR_3_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
            break;
      }
#endif

#endif
}


/// @brief      Stepmotor 4 Port 제어 함수
/// @details    Stepmotor 4를 제어하는 HW Port를 제어한다.
/// @param      mu8Step : 제어 단계
/// @return     void
void Stepmotor_4_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 4)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 4:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;
      }
#elif (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_1_2_PHASE_EXCITATION)
// 1-2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 2:
            TURN_ON_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 5:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;
    }

#else
// 1상 여자
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_ON_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_ON_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;

        case 8:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_ON_STEPMOTOR_4_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
            break;
      }
#endif

#endif
}


/// @brief      Stepmotor 5 Port 제어 함수
/// @details    Stepmotor 5를 제어하는 HW Port를 제어한다.
/// @param      mu8Step : 제어 단계
/// @return     void
void Stepmotor_5_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 5)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 4:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;
      }
#elif (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_1_2_PHASE_EXCITATION)
// 1-2상 여자 제어
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 2:
            TURN_ON_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 5:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 8:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;
    }

#else
// 1상 여자
    switch (mu8Step)
    {
        case 1:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 2:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
          break;

        case 3:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 4:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        case 5:
            TURN_ON_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 6:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_ON_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 7:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_ON_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;

        case 8:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_ON_STEPMOTOR_5_4();
            break;

        default:
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
            break;
      }
#endif

#endif
}


/// @brief      Step Motor Stop Function
/// @details    Stepmotor를 정지하는 동작을 한다
/// @param      void
/// @return     void
void Hal_Stepmotor_Stop(U8 mu8MotorID)
{
    switch (mu8MotorID)
    {
        case 1:
#if (STEPMOTOR_COUNT >= 1)
            TURN_OFF_STEPMOTOR_1_1();
            TURN_OFF_STEPMOTOR_1_2();
            TURN_OFF_STEPMOTOR_1_3();
            TURN_OFF_STEPMOTOR_1_4();
#endif
            break;

        case 2:
#if (STEPMOTOR_COUNT >= 2)
            TURN_OFF_STEPMOTOR_2_1();
            TURN_OFF_STEPMOTOR_2_2();
            TURN_OFF_STEPMOTOR_2_3();
            TURN_OFF_STEPMOTOR_2_4();
#endif
            break;

        case 3:
#if (STEPMOTOR_COUNT >= 3)
            TURN_OFF_STEPMOTOR_3_1();
            TURN_OFF_STEPMOTOR_3_2();
            TURN_OFF_STEPMOTOR_3_3();
            TURN_OFF_STEPMOTOR_3_4();
#endif
            break;

        case 4:
#if (STEPMOTOR_COUNT >= 4)
            TURN_OFF_STEPMOTOR_4_1();
            TURN_OFF_STEPMOTOR_4_2();
            TURN_OFF_STEPMOTOR_4_3();
            TURN_OFF_STEPMOTOR_4_4();
#endif
            break;

        case 5:
#if (STEPMOTOR_COUNT >= 5)
            TURN_OFF_STEPMOTOR_5_1();
            TURN_OFF_STEPMOTOR_5_2();
            TURN_OFF_STEPMOTOR_5_3();
            TURN_OFF_STEPMOTOR_5_4();
#endif
            break;

        default:
            break;
    }
}


#endif


// Hal Stepmoter Module **************************************************************************************

/// @brief      Hal Stepmoter Initilaize Module
/// @details    스탭 모터 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Stepmotor_Module_Initialize(void)
{
#if (STEPMOTOR_COUNT > 0)
    Hal_Stepmotor_Initialize();             // Stepmotor 제어 관련 초기화
#endif
}


/// @brief      Hal Stepmotor Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 스탭 모터 제어를 한다
/// @param      void
/// @return     void
void Hal_Stepmotor_Module_1ms_Control(void)
{
#if (STEPMOTOR_COUNT > 0)
    Hal_StepmotorControl();                 // Stepmotor 제어 함수(시간이 밀리면 안되므로 Interrput 루틴 안에서 직접 제어)
#endif
}

