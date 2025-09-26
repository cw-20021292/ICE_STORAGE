/// @file     Hal_Input_IceTray.c
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ICE TRAY ���� ���� ����

#include "Global_Header.h"


#if (ICE_TRAY_INPUT_CHECK_USE == USE)

U8 gu8InputIceTrayControlTimer = 0;                 /// @brief  Ice Tray ���� ���� �ð� ī����

U16 gu16IceLevelDelay = 0;                          /// @brief  Ice Tray ���� Ȯ�� Delay �ð�

// �������̼���
U8 gu8IceLEV = 0;                                   /// @brief  Ice Tray ���� ��ġ ����
U8 gu8BeIceLEV = 0;                                 /// @brief  Ice Tray ���� ��ġ ����
U8 gu8IceTrayLEV = 0;                               /// @brief  Ice Tray Ȯ�� ��ġ ����



/// @brief      Ice Tray ���� ���� ���� �ʱ�ȭ
/// @details    Ice Tray ������ ���õ� ������ �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Input_IceTray_Initialize(void)
{
    gu8InputIceTrayControlTimer = 0;

    gu16IceLevelDelay = 0;

    gu8IceLEV = 0;
    gu8BeIceLEV = 0;
    gu8IceTrayLEV = 0;
}


/// @brief      Ice Tray control Timer(@1ms interrupt)
/// @details    Ice Tray ���� ���� �ֱ� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Input_IceTrayControlTimer(void)
{
    if (gu8InputIceTrayControlTimer < INPUT_ICE_TRAY_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8InputIceTrayControlTimer++;
    }
}


/// @brief      Ice Tray Micro SW�� ���� ���� �Լ�
/// @details    Ice Tray�� Micro SW�� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Input_IceTray_Micro_SW(void)
{
    U8 mu8MicroSW_HighBuf = 0;
    U8 mu8MicroSW_LowBuf = 0;

    if (gu8InputIceTrayControlTimer >= INPUT_ICE_TRAY_CONTROL_TIME_PERIOD)
    {
        gu8InputIceTrayControlTimer = 0;

        if(gu16IceLevelDelay)
        {
            gu16IceLevelDelay--;            // Ice Tray ������
        }
        else
        {

        }

        //============================================================ Ʈ���� ��ġ ����
        mu8MicroSW_HighBuf = GET_STATUS_ICE_TRAY_MICRO_SW_HIGH();
        mu8MicroSW_LowBuf = GET_STATUS_ICE_TRAY_MICRO_SW_LOW();

        if ( (mu8MicroSW_HighBuf == CLEAR) && (mu8MicroSW_LowBuf == SET) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ICE_MAKING;       // ��������
        }
        else if ( (mu8MicroSW_HighBuf == SET) && (mu8MicroSW_LowBuf == CLEAR) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ICE_THROW;        // ��������
        }
        else if ( (mu8MicroSW_HighBuf == SET) && (mu8MicroSW_LowBuf == SET) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_MOVING;           // �̵���
        }
        else if ( (mu8MicroSW_HighBuf == CLEAR) && (mu8MicroSW_LowBuf == CLEAR) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ERROR;            // ����
        }
        else
        {

        }

        if (gu8IceLEV != gu8BeIceLEV)
        {   // ���� ��ġ�� ���� ��ġ ���°� �ٸ� ���
            gu8BeIceLEV = gu8IceLEV;
            gu16IceLevelDelay = ICE_TRAY_DELAY_TIME;           // Ʈ���� �ð� ������
        }
        else
        {   // ���� ��ġ�� ���� ��ġ ���°� ���� ���
            if(!gu16IceLevelDelay)
            {   // ���� �ð� ���� Ȯ��
                gu8IceTrayLEV = gu8IceLEV;
            }
        }
    }
}


/// @brief      Ice Tray ��ġ ���� Ȯ�� �Լ�
/// @details    Ice Tray ��ġ ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Tray ��ġ ���� - 0(��������), 1(��������), 2(�̵���), 3(����)
U8 Get_IceTrayPosition(void)
{
    return  gu8IceTrayLEV;
}


#endif


// Hal Water Input Ice Tray Module ***************************************************************************************

/// @brief      Hal Ice Tray ���� Initilaize Module
/// @details    Ice Tray ���� ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_Initialize(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Hal_Input_IceTray_Initialize();
#endif
}


/// @brief      Hal Ice Tray ���� Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� Ice Tray ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_1ms_Control(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Input_IceTrayControlTimer();
#endif
}


/// @brief      Hal Ice Tray ���� Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� Ice Tray ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_Control(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Input_IceTray_Micro_SW();
#endif
}
