/// @file     Hal_Stepmotor.c
/// @date     2025/03/12
/// @author   Jaejin Ham
/// @brief    Step Motor ���� ���� file


#include "Global_Header.h"

#if (STEPMOTOR_COUNT > 0)

/// @brief  Stepmotor ���� ���� �ڷ���
typedef struct {
    U8 au8Direction[STEPMOTOR_ID_MAX_COUNT];            /// @brief  ���� ���� ����
    U8 au8StepCycle[STEPMOTOR_ID_MAX_COUNT];            /// @brief  ���� ���� �� ���� Cycle
    U8 au8Move_PPS[STEPMOTOR_ID_MAX_COUNT];             /// @brief  ����� �����ϴ� PPS ���� ���� �ð�
    U8 au8Cnt_PPS[STEPMOTOR_ID_MAX_COUNT];              /// @brief  ���͸� �����ϴ� PPS ���� �ð�
    U16 au16MovingPulse[STEPMOTOR_ID_MAX_COUNT];        /// @brief  ���Ͱ� ������ PPS ��
}   SStepmotorControlData_T;

SStepmotorControlData_T   SStepmotorData;

CU8 au8Step_CW[8] ={1, 2, 3, 4, 5, 6, 7, 8};            /// @brief  Stepmotor CW ���� �ܰ�
CU8 au8Step_CCW[8] = {8, 7, 6, 5, 4, 3, 2, 1};          /// @brief  Stepmotor CCW ���� �ܰ�


/// @brief      Stepmotor ���� ���� ���� ��� �ʱ�ȭ
/// @details    Stepmotor ���� ���� �������� ��� �ʱ�ȭ ��Ų��
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


/// @brief      Stepmotor Moving Set �Լ�
/// @details    Stepmotor ���� ���� �����͸� �����Ѵ�
/// @param      mu8MotorID : �����Ϸ��� Stepmotor ID
///             mu8Direction : �����Ϸ��� ���� - 0(CW), 1(CCW)
///             mu16Pulse : �����Ϸ��� Pulse Count
/// @return     void
void Set_Stepmotor_Move(U8 mu8MotorID, U8 mu8Direction, U16 mu16Pulse)
{
    SStepmotorData.au8Direction[mu8MotorID] = mu8Direction;
    SStepmotorData.au16MovingPulse[mu8MotorID] = mu16Pulse;
}


/// @brief      Stepmotor Moving ���� Ȯ�� �Լ�
/// @details    Stepmotor�� ���� ������ ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� ���� - 0(Stop), 1(Moving)
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


/// @brief      Stepmotor Control �Լ�(@1ms interrupt)
/// @details    1ms ���ͷ�Ʈ �ȿ��� Stepmotor ���� ��� ī��Ʈ �Ѵ�
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


/// @brief      Stepmotor ���� �Լ�
/// @details    Stepmotor�� �����ϴ� ������ �Ѵ�
/// @param      mu8MotorID : �����ϴ� Stepmotor ID
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


/// @brief      Stepmotor 1 Port ���� �Լ�
/// @details    Stepmotor 1�� �����ϴ� HW Port�� �����Ѵ�.
/// @param      mu8Step : ���� �ܰ�
/// @return     void
void Stepmotor_1_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 1)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2�� ���� ����
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
// 1-2�� ���� ����
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
// 1�� ����
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


/// @brief      Stepmotor 2 Port ���� �Լ�
/// @details    Stepmotor 2�� �����ϴ� HW Port�� �����Ѵ�.
/// @param      mu8Step : ���� �ܰ�
/// @return     void
void Stepmotor_2_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 2)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2�� ���� ����
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
// 1-2�� ���� ����
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
// 1�� ����
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


/// @brief      Stepmotor 3 Port ���� �Լ�
/// @details    Stepmotor 3�� �����ϴ� HW Port�� �����Ѵ�.
/// @param      mu8Step : ���� �ܰ�
/// @return     void
void Stepmotor_3_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 3)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2�� ���� ����
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
// 1-2�� ���� ����
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
// 1�� ����
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


/// @brief      Stepmotor 4 Port ���� �Լ�
/// @details    Stepmotor 4�� �����ϴ� HW Port�� �����Ѵ�.
/// @param      mu8Step : ���� �ܰ�
/// @return     void
void Stepmotor_4_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 4)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2�� ���� ����
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
// 1-2�� ���� ����
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
// 1�� ����
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


/// @brief      Stepmotor 5 Port ���� �Լ�
/// @details    Stepmotor 5�� �����ϴ� HW Port�� �����Ѵ�.
/// @param      mu8Step : ���� �ܰ�
/// @return     void
void Stepmotor_5_Control(U8 mu8Step)
{
#if (STEPMOTOR_COUNT >= 5)

#if (STEPMOTOR_CONTROL_TYPE == STEPMOTOR_2_PHASE_EXCITATION)
// 2�� ���� ����
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
// 1-2�� ���� ����
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
// 1�� ����
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
/// @details    Stepmotor�� �����ϴ� ������ �Ѵ�
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
/// @details    ���� ���� ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Stepmotor_Module_Initialize(void)
{
#if (STEPMOTOR_COUNT > 0)
    Hal_Stepmotor_Initialize();             // Stepmotor ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Stepmotor Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_Stepmotor_Module_1ms_Control(void)
{
#if (STEPMOTOR_COUNT > 0)
    Hal_StepmotorControl();                 // Stepmotor ���� �Լ�(�ð��� �и��� �ȵǹǷ� Interrput ��ƾ �ȿ��� ���� ����)
#endif
}

