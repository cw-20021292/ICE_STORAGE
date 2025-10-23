/// @file   Lib_OperationMode.c
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  ���� ��� �ְ� ����

#include "Global_header.h"


U8 gu8OperaionMode = 0;                 /// @brief   ���� ���� ���� ��� - �Ϲ�, ���� �ڵ�ȭ, FCT ��� ��
U8 gu8ChangeOperaionMode = 0;           /// @brief   ������ ���� ��� - �Ϲ�, ���� �ڵ�ȭ, FCT ��� ��

U8 gu8ProductStatus = 0;                /// @brief   ��ǰ�� ���� ���� ����(���θ� ����ϴ� ����)- ���, ����, ��� ��


/// @brief      ���� ���� ��� �ʱ�ȭ �Լ�
/// @details    ���� ��� ���� �����͸� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void InitializeOperationMode(void)
{
    gu8OperaionMode = OPERATION_MODE_ID_NONE;
    gu8ProductStatus = OPERATION_ID_STAND_BY;
}


/// @brief      ���� ��� ���� �Լ�
/// @details    ���� ��带 ��ȯ ���� �Ѵ�
/// @param      mu8OperationMode : ���� ��ų ��� - enum���� ����� ���� ��� ����Ʈ ����
/// @return     void
void ChangeOperationMode(U8 mu8OperationMode)
{
    gu8ChangeOperaionMode = mu8OperationMode;
}

/// @brief      ��ǰ�� ���� ���� ���� ��� Ȯ�� �Լ�
/// @details    ���� ��ǰ�� ���� ���� ��带 Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� ��� - enum���� ����� ���� ��� ����Ʈ ����
U8 Get_OperationMode(void)
{
    return  gu8OperaionMode;
}


/// @brief      ���� ���� ���� ���� �Լ�
/// @details    ���� ��忡 ���� ���� ��ų �Լ���� �� ���� Ÿ�̸Ӹ� �����Ͽ� ������ �ǰ� �Ѵ�
/// @param      void
/// @return     void
void OperationModeSetup(void)
{
    switch (gu8ChangeOperaionMode)
    {
        case OPERATION_MODE_ID_NORMAL:         // �Ϲ�
            if (gu8OperaionMode != OPERATION_MODE_ID_NORMAL)
            {
                gu8OperaionMode = OPERATION_MODE_ID_NORMAL;
                InitializeTimeScheduler();                  // �����췯 �ʱ�ȭ
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
                SetupTimeScheduler(WifiControlProcess100ms, 100, ACT_COUNT_INFINITE);   // Wi-Fi 100ms Scheduler ����
            }
            break;

        case OPERATION_MODE_ID_PCB_TEST:       // FCT ���(PCB �׽�Ʈ ���)
            if (gu8OperaionMode != OPERATION_MODE_ID_PCB_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PCB_TEST;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_FRONT_TEST:     // Front ���� �˻� ���
            if (gu8OperaionMode != OPERATION_MODE_ID_FRONT_TEST)
            {
                gu8OperaionMode = OPERATION_MODE_ID_FRONT_TEST;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_1:     // PTA �˻� 1���(���� �ڵ�ȭ 1���)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_1)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_1;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_2:     // PTA �˻� 2���(���� �ڵ�ȭ 2���)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_2)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_2;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_PTA_TEST_3:     // PTA �˻� 3���(���� �ڵ�ȭ 3���)
            if (gu8OperaionMode != OPERATION_MODE_ID_PTA_TEST_3)
            {
                gu8OperaionMode = OPERATION_MODE_ID_PTA_TEST_3;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_1:
            if (gu8OperaionMode != OPERATION_MODE_ID_EXTRA_TEST_1)
            {
                gu8OperaionMode = OPERATION_MODE_ID_EXTRA_TEST_1;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        case OPERATION_MODE_ID_EXTRA_TEST_2:
            if (gu8OperaionMode != OPERATION_MODE_ID_EXTRA_TEST_2)
            {
                gu8OperaionMode = OPERATION_MODE_ID_EXTRA_TEST_2;
                InitializeTimeScheduler();
// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
            }
            break;

        default:    // �Ϲ� ���� ���
            if (gu8OperaionMode != OPERATION_MODE_ID_NORMAL)
            {
                gu8OperaionMode = OPERATION_MODE_ID_NORMAL;
                InitializeTimeScheduler();                  // �����췯 �ʱ�ȭ

// ��)           SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);            // ���ο� ���� �Լ� ������ ����
                SetupTimeScheduler(WifiControlProcess100ms, 100, ACT_COUNT_INFINITE);   // Wi-Fi 100ms Scheduler ����
            }
            break;
    }
}


/// @brief      ��ǰ�� ���� ���� ���� ���� ���� �Լ�
/// @details    ���� ��ǰ�� ���θ� ����ϴ� ���� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� ���� ���� ����(���θ� ����ϴ� ����) ID - enum���� ����� ���� ����Ʈ ����
/// @return     void
void Set_OperatingStatus(U8 mu8StatusID)
{
    gu8ProductStatus = mu8StatusID;
}


/// @brief      ��ǰ�� ���� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� ��ǰ�� ���θ� ����ϴ� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� ����(���θ� ����ϴ� ����) - enum���� ����� ���� ����Ʈ ����
U8 Get_OperatingStatus(void)
{
    return  gu8ProductStatus;
}


// Lib Operating Mode Module *********************************************************************************

/// @brief      Lib Operating Mode Module Initilaize
/// @details    ���� ��� ���� �����͸� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_OperatingMode_Module_Initialize(void)
{
    InitializeOperationMode();              // ���� ��� ���� ���� �ʱ�ȭ
}



