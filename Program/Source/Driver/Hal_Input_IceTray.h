/// @file     Hal_Input_IceTray.h
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ICE TRAY ���� ���� ��� ����

#ifndef _HAL_INPUT_ICE_TRAY_H_
#define _HAL_INPUT_ICE_TRAY_H_


/*  Ice Tray ��ġ ���� ���� Function ����
    => Get_IceTrayPosition();                       // Ice Tray ��ġ ���� Ȯ�� - 0(��������), 1(��������), 2(�̵���), 3(����)

    ��� ��)
    u8Return = Get_IceTrayPosition();
*/

#define ICE_TRAY_INPUT_CHECK_USE                    USE

#if (ICE_TRAY_INPUT_CHECK_USE == USE)

#define INPUT_ICE_TRAY_CONTROL_TIME_PERIOD          100     // Ice Tray ���� ���� �ֱ� 100ms@1ms

#define ICE_TRAY_POSITION_ICE_MAKING                0       // ��������
#define ICE_TRAY_POSITION_ICE_THROW                 1       // ��������
#define ICE_TRAY_POSITION_MOVING                    2       // �̵���
#define ICE_TRAY_POSITION_ERROR                     3       // ����

#define ICE_TRAY_DELAY_TIME                         5       // Ʈ���� �ð� ������ 500ms@100ms

// Port Define
#define P_ICE_TRAY_MICRO_SW_HIGH                    P12_bit.no0
#define GET_STATUS_ICE_TRAY_MICRO_SW_HIGH()         ( P_ICE_TRAY_MICRO_SW_HIGH )
#define P_ICE_TRAY_MICRO_SW_LOW                     P12_bit.no1
#define GET_STATUS_ICE_TRAY_MICRO_SW_LOW()          ( P_ICE_TRAY_MICRO_SW_LOW )

void Hal_Input_IceTray_Initialize(void);
void Input_IceTrayControlTimer(void);
void Input_IceTray_Micro_SW(void);
U8 Get_IceTrayPosition(void);

#endif

void Hal_Input_IceTray_Module_Initialize(void);
void Hal_Input_IceTray_Module_1ms_Control(void);
void Hal_Input_IceTray_Module_Control(void);


#endif
