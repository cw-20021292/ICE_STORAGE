/// @file   Lib_OperationMode.h
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  ���� ��� �ְ� ��� ����

#ifndef _LIB_OPERATION_MODE_H_
#define _LIB_OPERATION_MODE_H_


/*  Lib Operation Mode Function ����
    => ChangeOperationMode(mu8OperationMode);       // ���۽�Ű���� ��尪�� ���� - EProductMode_T�� ����Ʈ ID��
    => Get_OperationMode();                         // ���� �������� ��尪 Ȯ�� - EProductMode_T�� ����Ʈ ID��
    => OperationModeSetup();                        // ���� ���� ���� ��� ������ ���۵Ǵ� �Լ����� ������ ���� �Լ�
                                                       �� �Լ� ���� ��庰�� �ڽ��� ���۽�Ű���� �ϴ� �Լ����� ��ġ

    <������>
    => Set_OperatingStatus(mu8StatusID);            // ���θ� ����ϴ� ���� �� ���۽�Ű���� ��尪 ���� - EProductUseWaterStatus_T�� ����Ʈ ID��
    => Get_OperatingStatus();                       // ���θ� ����ϴ� ���� �� ���� �������� ��尪 Ȯ�� - EProductUseWaterStatus_T�� ����Ʈ ID��
*/


// ���� ���
/// @brief      ��ǰ ���� ���°�
/// @details    ��ǰ ���� ���°��� ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    OPERATION_MODE_ID_NONE = 0,                 // 0 : ���� ����
    OPERATION_MODE_ID_NORMAL,                   // 1 : �Ϲݸ��
    OPERATION_MODE_ID_PCB_TEST,                 // 2 : PCB Test ���
    OPERATION_MODE_ID_FRONT_TEST,               // 3 : Front Test ���
    OPERATION_MODE_ID_PTA_TEST_1,               // 4 : PTA 1���(PTA���� ��庰�� ���� �ݵ�� ���Ѽ� �迭�� ��)
    OPERATION_MODE_ID_PTA_TEST_2,               // 5 : PTA 2���(PTA���� ��庰�� ���� �ݵ�� ���Ѽ� �迭�� ��)
    OPERATION_MODE_ID_PTA_TEST_3,               // 6 : PTA 3���(PTA���� ��庰�� ���� �ݵ�� ���Ѽ� �迭�� ��)
    OPERATION_MODE_ID_EXTRA_TEST_1,             // 7 : ������ �׽�Ʈ ���1
    OPERATION_MODE_ID_EXTRA_TEST_2,             // 8 : ������ �׽�Ʈ ���2
    OPERATION_MODE_ID_MAX                       // Max
} EProductMode_T;


/// @brief      ��ǰ ���� ���°�(���θ� ����ϴ� ����)
/// @details    ��ǰ ���� ���°�(���θ� ����ϴ� ����)�� ���� ����Ʈ�� �����Ѵ�
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

