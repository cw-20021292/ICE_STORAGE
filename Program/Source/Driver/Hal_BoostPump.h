/// @file     Hal_BoostPump.h
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    Boost Pump Control file


#ifndef _HAL_BOOST_PUMP_H_
#define _HAL_BOOST_PUMP_H_


/*  �ν�Ʈ ���� ���� Function ����
    => u8Return = Get_BoostInStatus();              // �ν�Ʈ ���� ���� ���� Ȯ�� - 0(�̿���), 1(����)
    => Set_BoostPump(On/Off Data, Delay Time);      // �ν�Ʈ ���� ���� ���� ����
    => u8Return = Get_BoostPump_Status();           // �ν�Ʈ ���� ���� ���� Ȯ�� - 1(On), 0(Off)

    ��� ��)
    Set_BoostPump(1, 0);        // �ν�Ʈ ���� 0�� ������ �� On
    Set_BoostPump(ON, 0);       // �ν�Ʈ ���� 0�� ������ �� On

    Set_BoostPump(0, 0);        // �ν�Ʈ ���� 0�� ������ �� Off
    Set_BoostPump(OFF, 0);      // �ν�Ʈ ���� 0�� ������ �� Off

    Set_BoostPump(1, 5);        // �ν�Ʈ ���� 500ms ������ �� On
    Set_BoostPump(ON, 5);       // �ν�Ʈ ���� 500ms ������ �� On

    Set_BoostPump(0, 5);        // �ν�Ʈ ���� 500ms ������ �� Off
    Set_BoostPump(OFF, 5);      // �ν�Ʈ ���� 500ms ������ �� Off

    mu8Return = Get_BoostPump_Status();     // mu8Return : 1(ON), 0(OFF)
*/

#define BOOST_PUMP_USE                      USE             // �ν�Ʈ ���� ��� ���� - 0(No USE), 1(USE)

#if (BOOST_PUMP_USE == USE)
// Boost Pump Port �Ҵ�
// �̻�� port�� 0���� ó��
#define PORT_DEFINE_BOOST_PUMP_OUT          P7_bit.no5      // Boost Pump OUT
#define PORT_DEFINE_BOOST_PUMP_IN           0               // Boost Pump IN
#endif


#if (BOOST_PUMP_USE == USE)

// Boost Pump Port Define
#define P_BOOST_OUT                         PORT_DEFINE_BOOST_PUMP_OUT
#define TURN_ON_BOOST_OUT()                 { P_BOOST_OUT = 1; }
#define TURN_OFF_BOOST_OUT()                { P_BOOST_OUT = 0; }
#define GET_STATUS_BOOST_OUT()              ( P_BOOST_OUT )

#define P_BOOST_IN                          PORT_DEFINE_BOOST_PUMP_IN
#define GET_STATUS_BOOST_IN()               ( P_BOOST_IN )


#define BOOST_PUMP_CONTROL_TIME_PERIOD      100         // 100ms@1ms
#define BOOST_PUMP_IN_CHECK_TIME            5           // 100ms@1ms

void Hal_BoostPump_Initialize(void);
void BoostPumpControlTimer(void);
void BoostPumpControl(void);
void BoostInCheck(void);
U8 Get_BoostInStatus(void);
void Set_BoostPump(U8 mu8OnOff, U8 mu8Delay);
U8 Get_BoostPump_Status(void);
void Hal_BoostPump_Control(void);

#endif


void Hal_BoostPump_Module_Initialize(void);
void Hal_BoostPump_Module_1ms_Control(void);
void Hal_BoostPump_Module_Control(void);

#endif

