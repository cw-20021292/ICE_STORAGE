/// @file     Hal_ReedSW.c
/// @date     2025/02/25
/// @author   Jaejin Ham
/// @brief    Reed SW Control file


#include "Global_Header.h"


#if (REED_SW_COUNT > 0)

U8 gu8ReedSWControlTimer = 0;                       /// @brief  Reed SW ���� ���� �ֱ� �ð� Ÿ�̸�

U16 gu16ReedSWConfirmValue = 0;                     /// @brief  Reed SW ���� Confirm ����(Bit �Ҵ�)

/// @brief  Reed SW�� ���� Chattering �ð��� ����Ʈ
U8 au8ReedSWChatteringTimeSet[11] = { 0,
                                      REED_SW_1_CHATTERING_TIME,
                                      REED_SW_2_CHATTERING_TIME,
                                      REED_SW_3_CHATTERING_TIME,
                                      REED_SW_4_CHATTERING_TIME,
                                      REED_SW_5_CHATTERING_TIME,
                                      REED_SW_6_CHATTERING_TIME,
                                      REED_SW_7_CHATTERING_TIME,
                                      REED_SW_8_CHATTERING_TIME,
                                      REED_SW_9_CHATTERING_TIME,
                                      REED_SW_10_CHATTERING_TIME };

/// @brief  Reed SW ���� ���� �ڷ���
typedef struct {
    U8  au8PreStatus[REED_SW_ID_MAX_COUNT];          // Reed SW Previous Status
    U8  au8CurrentStatus[REED_SW_ID_MAX_COUNT];      // Reed SW Current Status
    U8  au8ChatteringTime[REED_SW_ID_MAX_COUNT];     // Reed SW Chattering Time
}   SReedData_T;

SReedData_T   SReedSWControlData;


/// @brief      Reed SW Initialize
/// @details    Reed SW ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_ReedSW_Initialize(void)
{
    U8 mu8ReedSWBuf = 0;

    MEMSET( (void __FAR *)&SReedSWControlData, 0x00, sizeof(SReedData_T) );

    gu8ReedSWControlTimer = 0;

    gu16ReedSWConfirmValue = 0;

    // Reed SW Chattering Time Initialize
    for (mu8ReedSWBuf = 1 ; mu8ReedSWBuf < REED_SW_ID_MAX_COUNT ; mu8ReedSWBuf++)
    {
        Reset_ReedSWChatteringTime(mu8ReedSWBuf);
    }
}


/// @brief      Reed SW Chattering Time Initialize
/// @details    Reed SW�� ���� Chattering �ð��� �ʱ�ȭ ���� �Ѵ�
/// @param      mu8ReedSWID : define�� Reed SW�� ID
/// @return     void
void Reset_ReedSWChatteringTime(U8 mu8ReedSWID)
{
    SReedSWControlData.au8ChatteringTime[mu8ReedSWID] = au8ReedSWChatteringTimeSet[mu8ReedSWID];
}



/// @brief      Reed SW Control Timer(@1ms interrupt)
/// @details    Reed SW ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void ReedSWControlTimer(void)
{
    if (gu8ReedSWControlTimer < REED_SW_CONTROL_TIME_PERIOD)
    {
        gu8ReedSWControlTimer++;
    }
}


/// @brief      Reed SW HW Read
/// @details    Reed SW�� HW ���¸� Read �Ѵ�
/// @param      void
/// @return     void
void Hal_Read_ReedSW(void)
{
#if (REED_SW_COUNT > 0)

#if (REED_SW_COUNT >= 1)
    SReedSWControlData.au8CurrentStatus[1] = GET_STATUS_REED_SW_1();
#endif
#if (REED_SW_COUNT >= 2)
    SReedSWControlData.au8CurrentStatus[2] = GET_STATUS_REED_SW_2();
#endif
#if (REED_SW_COUNT >= 3)
    SReedSWControlData.au8CurrentStatus[3] = GET_STATUS_REED_SW_3();
#endif
#if (REED_SW_COUNT >= 4)
    SReedSWControlData.au8CurrentStatus[4] = GET_STATUS_REED_SW_4();
#endif
#if (REED_SW_COUNT >= 5)
    SReedSWControlData.au8CurrentStatus[5] = GET_STATUS_REED_SW_5();
#endif
#if (REED_SW_COUNT >= 6)
    SReedSWControlData.au8CurrentStatus[6] = GET_STATUS_REED_SW_6();
#endif
#if (REED_SW_COUNT >= 7)
    SReedSWControlData.au8CurrentStatus[7] = GET_STATUS_REED_SW_7();
#endif
#if (REED_SW_COUNT >= 8)
    SReedSWControlData.au8CurrentStatus[8] = GET_STATUS_REED_SW_8();
#endif
#if (REED_SW_COUNT >= 9)
    SReedSWControlData.au8CurrentStatus[9] = GET_STATUS_REED_SW_9();
#endif
#if (REED_SW_COUNT >= 10)
    SReedSWControlData.au8CurrentStatus[10] = GET_STATUS_REED_SW_10();
#endif

#endif
}


/// @brief      Reed SW Control Function(@while)
/// @details    Reed SW ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void ReedSWControl(void)
{
    U8 mu8ReedSWIDBuf = 0;

    if (gu8ReedSWControlTimer >= REED_SW_CONTROL_TIME_PERIOD)
    {       // 100ms���� ����
        gu8ReedSWControlTimer = 0;

        Hal_Read_ReedSW();          // Reed SW HW Read

        for (mu8ReedSWIDBuf = 1 ; mu8ReedSWIDBuf < REED_SW_ID_MAX_COUNT ; mu8ReedSWIDBuf++)
        {
            if (SReedSWControlData.au8PreStatus[mu8ReedSWIDBuf] == SReedSWControlData.au8CurrentStatus[mu8ReedSWIDBuf])
            {   // Reed SW�� ���� ���¿� ���� ���°� ���� ���
                if (SReedSWControlData.au8ChatteringTime[mu8ReedSWIDBuf])
                {
                    SReedSWControlData.au8ChatteringTime[mu8ReedSWIDBuf]--;

                    if (SReedSWControlData.au8ChatteringTime[mu8ReedSWIDBuf] == 0)
                    {
                        ConfirmReedSWData(mu8ReedSWIDBuf, SReedSWControlData.au8CurrentStatus[mu8ReedSWIDBuf]);        // Reed SW Data Ȯ��
                    }
                }
                else
                {
                    ConfirmReedSWData(mu8ReedSWIDBuf, SReedSWControlData.au8CurrentStatus[mu8ReedSWIDBuf]);        // Reed SW Data Ȯ��
                }
            }
            else
            {   // Reed SW�� ���� ���¿� ���� ���°� �ٸ� ���
                SReedSWControlData.au8PreStatus[mu8ReedSWIDBuf] = SReedSWControlData.au8CurrentStatus[mu8ReedSWIDBuf];      // ������ ����
                Reset_ReedSWChatteringTime(mu8ReedSWIDBuf);     // Chattering �ð� �ʱ�ȭ
            }
        }
    }
}



/// @brief      Reed SW ������ Ȯ�� �Լ�
/// @details    Reed SW ���� ���¸� ���������� Ȯ���Ͽ� �� ���°��� �����Ѵ�
/// @param      mu8ReedSWID : define�� Reed SW�� ID
///             mu8Value : ���� ���°� - 1(detect), 0(undetect)
/// @return     void
void ConfirmReedSWData(U8 mu8ReedSWID, U8 mu8Value)
{
    switch (mu8ReedSWID)
    {
        case 1:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_1_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_1_DETECT;
            }
            break;

        case 2:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_2_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_2_DETECT;
            }
            break;

        case 3:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_3_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_3_DETECT;
            }
            break;

        case 4:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_4_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_4_DETECT;
            }
            break;

        case 5:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_5_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_5_DETECT;
            }
            break;

        case 6:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_6_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_6_DETECT;
            }
            break;

        case 7:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_7_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_7_DETECT;
            }
            break;

        case 8:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_8_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_8_DETECT;
            }
            break;

        case 9:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_9_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_9_DETECT;
            }
            break;

        case 10:
            if (mu8Value)
            {
                gu16ReedSWConfirmValue |= REED_SW_10_DETECT;
            }
            else
            {
                gu16ReedSWConfirmValue &= ~REED_SW_10_DETECT;
            }
            break;

        default:
            break;
    }
}


/// @brief      Check Reed SW Status
/// @details    Reed SW�� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8ReedSWID : define�� Reed SW�� ID
/// @return     mu8Return : ���� ���°� - 1(Detect), 0(Undetect)
U8 Get_ReedSW_Status(U8 mu8ReedSWID)
{
    U8 mu8Return = 0;

    switch (mu8ReedSWID)
    {
        case 1:
            if (gu16ReedSWConfirmValue & REED_SW_1_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 2:
            if (gu16ReedSWConfirmValue & REED_SW_2_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 3:
            if (gu16ReedSWConfirmValue & REED_SW_3_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 4:
            if (gu16ReedSWConfirmValue & REED_SW_4_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 5:
            if (gu16ReedSWConfirmValue & REED_SW_5_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 6:
            if (gu16ReedSWConfirmValue & REED_SW_6_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 7:
            if (gu16ReedSWConfirmValue & REED_SW_7_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 8:
            if (gu16ReedSWConfirmValue & REED_SW_8_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 9:
            if (gu16ReedSWConfirmValue & REED_SW_9_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        case 10:
            if (gu16ReedSWConfirmValue & REED_SW_10_DETECT)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
            break;

        default:
            break;
    }

    return  mu8Return;
}

#endif

// Hal Reed Switch Module ************************************************************************************

/// @brief      Hal Reed Switch Initilaize Module
/// @details    ���� ����ġ ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_ReedSW_Module_Initialize(void)
{
#if (REED_SW_COUNT > 0)
    Hal_ReedSW_Initialize();                // Reed SW ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Reed Switch Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ����ġ ���� ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_ReedSW_Module_1ms_Control(void)
{
#if (REED_SW_COUNT > 0)
    ReedSWControlTimer();                   // Reed SW Check �ð� Counter
#endif
}


/// @brief      Hal Reed Switch Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ����ġ ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_ReedSW_Module_Control(void)
{
#if (REED_SW_COUNT > 0)
    ReedSWControl();                        // Reed SW üũ �Լ�
#endif
}



