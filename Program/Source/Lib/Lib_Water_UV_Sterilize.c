/// @file     Lib_Water_UV_Sterilize.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    ��ǰ ���� ���� ���� file

#include "Global_Header.h"

#if (UV_COUNT > 0)

U8 gu8UV_SterilizeControlTimer = 0;         /// @brief  UV ��� ������ ���� ���� ���� Ÿ�̸�

U8 gu8UV_SterilizePeriodTimeSec = 0;        /// @brief  UV ��� ���� ���� üũ�� ���� 1sec Ÿ�̸�
U8 gu8UV_SterilizePeriodTimeMin = 0;        /// @brief  UV ��� ���� ���� üũ�� ���� 1min Ÿ�̸�

/// @brief      ���� ���� �µ� ���� ������ ����
/// @details    ���� ��� �ϱ� ���� ���� �µ� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U16 au16UV_SterilizePeriodTime[STERILIZE_OPERATION_ID_MAX];     /// @brief  UV ��� �ֱ� Ÿ�̸�
    U8 au8UV_SterilizeStatus[STERILIZE_OPERATION_ID_MAX];           /// @brief  UV ��� ���� ����
    U16 au16_UV_OnTime[STERILIZE_OPERATION_ID_MAX];                 /// @brief  UV ����(ON) �ð�
}   SUV_SterilizeData_T;

SUV_SterilizeData_T     SUV_SterilizeData;


/// @brief      UV ��� ���� ���� �ֱ� �ð� ���̺�
/// @details    UV ��� ������ �����ϴ� �ֱ� �ð��� ���̺�� ������ ���´�
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


/// @brief      UV ��� ���� �ð� ���̺�
/// @details    UV ��� ������ �����ϴ� �ð��� ���̺�� ������ ���´�
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


/// @brief      UV ��� ���� ���� �ʱ�ȭ �Լ�
/// @details    UV ��� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Initialize(void)
{
    gu8UV_SterilizeControlTimer = 0;

    gu8UV_SterilizePeriodTimeSec = 0;
    gu8UV_SterilizePeriodTimeMin = 0;

    MEMSET( (void __FAR *)&SUV_SterilizeData, 0x00, sizeof(SUV_SterilizeData_T) );
}


/// @brief      UV ��� Control Timer(@1ms)
/// @details    UV ��� ��� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
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
        {   // 1�� ī��Ʈ
            gu8UV_SterilizePeriodTimeSec = 0;

            if (gu8UV_SterilizePeriodTimeMin < 59)
            {   // 1�� ī��Ʈ
                gu8UV_SterilizePeriodTimeMin++;
            }
        }
    }
}


/// @brief      UV ��� ���� ���� �Լ�
/// @details    UV ��� ���� ������ �����Ѵ�
/// @param      mu8ID : UV ��� ���� ID
///             mu8Status : UV ��� ���� ���� - 0(�̵���), 1(����)
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


/// @brief      UV ��� ���� ���� Ȯ�� �Լ�
/// @details    UV ��� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8ID : UV ��� ���� ID
/// @return     return : UV ��� ���� ���� - 0(�̵���), 1(����)
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


/// @brief      UV ��� ���� ���� �Լ�(@1min)
/// @details    UV ��� ���� ���� ���� �Ǵ��� �Ѵ�
/// @param      void
/// @return     void
void UV_SterilizeStartControl(void)
{
    U8 mu8i = 0;

    for (mu8i = 1 ; mu8i < STERILIZE_OPERATION_ID_MAX ; mu8i++)
    {
        if (++SUV_SterilizeData.au16UV_SterilizePeriodTime[mu8i] >= au16UV_SterilizePeroidTable[mu8i])
        {   // ��� �ֱ� �ð� ����
            SUV_SterilizeData.au16UV_SterilizePeriodTime[mu8i] = 0;
            Set_UV_SterilizeStatus(mu8i, SET);     // ��� ���� ���� ����
        }
    }
}


/// @brief      UV ��� ���� ����(@1min)
/// @details    UV ��� UV ON ������ �����Ѵ�
/// @param      void
/// @return     void
void UV_SterilizeOnControl(void)
{
    U8 mu8i = 0;
    U8 mu8UV_SterilizeStatus = 0;

    for (mu8i = 1 ; mu8i < STERILIZE_OPERATION_ID_MAX ; mu8i++)
    {
        mu8UV_SterilizeStatus = Get_UV_SterilizeStatus(mu8i);      // ��� ���� ���� Ȯ��

        if (mu8UV_SterilizeStatus == TRUE)
        {   // ��� ���� ���̸�
            Set_UV_Control(mu8i, ON);      // UV ON

            if (++SUV_SterilizeData.au16_UV_OnTime[mu8i] >= au16UV_SterilizeOnTimeTable[mu8i])
            {   // ��� ���� �ð� �Ϸ��
                SUV_SterilizeData.au16_UV_OnTime[mu8i] = 0;
                Set_UV_SterilizeStatus(mu8i, CLEAR);                // ��� ���� ����
            }
        }
        else
        {   // ��� ���� ���� �ƴϸ�
            Set_UV_Control(mu8i, OFF);      // UV OFF

            SUV_SterilizeData.au16_UV_OnTime[mu8i] = 0;
        }
    }
}


/// @brief      UV ��� ���� �Լ�
/// @details    UV ��� ������ �����Ѵ�
/// @param      void
/// @return     void
void Lib_UV_SterilizeControl(void)
{
    if (gu8UV_SterilizePeriodTimeMin >= 59)
    {   // 1�� ����� ����
        gu8UV_SterilizePeriodTimeMin = 0;

        UV_SterilizeStartControl();       // UV ��� ���� ���� �Ǵ�
        UV_SterilizeOnControl();          // UV ��� ON ����
    }
}


// Lib Tank Water Input Module *********************************************************************************

/// @brief      Lib Heater Module Initilaize
/// @details    �Ϲ� ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_Initialize(void)
{
#if (UV_COUNT > 0)
    Lib_UV_Sterilize_Initialize();
#endif
}


/// @brief      Lib UV ��� ��� in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� UV ��� ���� ��� Library
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_1ms_Control(void)
{
#if (UV_COUNT > 0)
    UV_SterilizeControlTimer();
#endif
}


/// @brief      Lib UV ��� ���� Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� UV ��� ���� ��� Library
/// @param      void
/// @return     void
void Lib_UV_Sterilize_Module_Control(void)
{
#if (UV_COUNT > 0)
    Lib_UV_SterilizeControl();
#endif
}


#endif
