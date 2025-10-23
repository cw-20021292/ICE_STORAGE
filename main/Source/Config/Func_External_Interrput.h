/// @file     Func_External_Interrput.h
/// @date     2025/04/25
/// @author   Jaejin Ham
/// @brief    External Interrput ���� �Լ�  File

#ifndef _FUNC_EXT_INTERRUPT_H_
#define _FUNC_EXT_INTERRUPT_H_


/*
    User�� �ܺ� ���ͷ�Ʈ ���� ����ϴ� �Լ���
    => External_Interrupt_TDS_In();             // TDS In ���� ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
    => External_Interrupt_Flowmeter_1();        // ��������1 ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
    => External_Interrupt_Flowmeter_2();        // ��������2 ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
    => External_Interrupt_Flowmeter_3();        // ��������3 ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
    => External_Interrupt_Flowmeter_4();        // ��������4 ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
    => External_Interrupt_Flowmeter_5();        // ��������5 ���� ������ �ܺ� ���ͷ�Ʈ ���� �Լ� �ȿ� �߰��� �Լ�
*/


#if (TDS_IN_USE == USE)       // TDS IN ���� ����
void External_Interrupt_TDS_In(void);
#endif

#if (FLOWMETER_COUNT >= 1)  // �������� 1 ����
void External_Interrupt_Flowmeter_1(void);
#endif

#if (FLOWMETER_COUNT >= 2)  // �������� 2 ����
void External_Interrupt_Flowmeter_2(void);
#endif

#if (FLOWMETER_COUNT >= 3)  // �������� 3 ����
void External_Interrupt_Flowmeter_3(void);
#endif

#if (FLOWMETER_COUNT >= 4)  // �������� 4 ����
void External_Interrupt_Flowmeter_4(void);
#endif

#if (FLOWMETER_COUNT >= 5)  // �������� 5 ����
void External_Interrupt_Flowmeter_5(void);
#endif


#endif

