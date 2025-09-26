/// @file     Hal_Instantaneous_Heater.c
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Instantaneous Heater Control file


#include "Global_Header.h"


#if (INSTANT_HEATER_USE == USE)

U8 gu8InstHeaterControlTimer = 0;                   /// @brief  �����¼� ���� ���� Ÿ�̸�

U8 gu8InstHeaterTriacControlCycle = 0;              /// @brief  Triac ���� �ð� ����Ŭ
U8 gu8InstHeaterTriacControlDuty = 0;               /// @brief  Triac Duty ���� �ð�

// ���� �¼� ���� ���� ���� �ڷ���
typedef struct {
    U8  gu8RelayOn;                                 /// @brief  ���� �¼� Heater Relay Status(1:ON, 0:OFF)
    U8  gu8TriacDuty;                               /// @brief  ���� �¼� Heater Triac Control Duty(0~100%)
    U8  gu8Delay;                                   /// @brief  ���� �¼� Heater ���� �� Delay �ð�
}   SInstHeaterData_T;

SInstHeaterData_T   SInstHeaterControlData;


/// @brief      �����¼� ���� ���� ���� ������ �ʱ�ȭ
/// @details    �����¼� ���� ���� ���� �����͸� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Initialize(void)
{
    MEMSET( (void __FAR *)&SInstHeaterControlData, 0x00, sizeof(SInstHeaterData_T) );

    gu8InstHeaterControlTimer = 0;

    gu8InstHeaterTriacControlCycle = 0;
    gu8InstHeaterTriacControlDuty = 0;

    Hal_Inst_Heater_Relay_Off();
    Hal_Inst_Heater_Triac_Off();
}


/// @brief      ���� �¼� ���� ���� Ÿ�̸�(@1ms interrupt)
/// @details    ���� �¼� ���� ���� �ð��� 1ms ���ͷ�Ʈ �ȿ��� ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void InstHeaterControlTimer(void)
{
    if (gu8InstHeaterControlTimer < INST_HEATER_CONTROL_TIME_PERIOD)
    {
        gu8InstHeaterControlTimer++;
    }
}


/// @brief      ���� �¼� ���� ���� �Լ�(@While��)
/// @details    ���� �¼� ���� ��� Whlie�� �ȿ��� ó���Ѵ�
/// @param      void
/// @return     void
void InstHeaterControl(void)
{
    if (gu8InstHeaterControlTimer >= INST_HEATER_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8InstHeaterControlTimer = 0;

        if (SInstHeaterControlData.gu8Delay)
        {
            SInstHeaterControlData.gu8Delay--;
        }

        Hal_Inst_Heater_Control();
    }

}


/// @brief      �����¼� ���� ������� �����Ѵ�
/// @details    �����¼� ���� ��� �ϴ� ������� �����Ѵ�
/// @param      mu8Relay : Relay ���� ���� - 1(On), 0(Off)
///             mu8duty : Triac Duty - 0 ~ 100 (%)
///             mu8DelayTime : ���� ���� �� Delay Time
/// @return     void
void Set_Inst_HeaterControl(U8 mu8Relay, U8 mu8Duty, U8 mu8DelayTime)
{
    SInstHeaterControlData.gu8RelayOn = mu8Relay;
    SInstHeaterControlData.gu8TriacDuty = mu8Duty;
    SInstHeaterControlData.gu8Delay = mu8DelayTime;
}


/// @brief      �����¼� ���� Triac ���� Duty Ȯ�� �Լ�
/// @details    �����¼� ���� Triac ��� �ϴ� Duty����� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Triac Duty - 0 ~ 100 (%)
U8 Get_Inst_HeaterTriacDuty(void)
{
    return  SInstHeaterControlData.gu8TriacDuty;
}


/// @brief      �����¼� ���� ���� �Լ�
/// @details    �����¼� ���͸� �����Ѵ�
/// @param      void
/// @return     void
void Hal_Inst_Heater_Control(void)
{
    if (SInstHeaterControlData.gu8Delay == 0)
    {
        Set_Inst_Heater_Relay(SInstHeaterControlData.gu8RelayOn);
        Set_Inst_Heater_Triac_Duty(SInstHeaterControlData.gu8TriacDuty);
    }
}


/// @brief      �����¼� ���� Relay ����
/// @brief      �����¼� ���� Relay�� �����Ѵ�
/// @param      mu8OnOff : �����¼� Relay ���� ���� - 0(Off), 1(On)
/// @return     void
void Set_Inst_Heater_Relay(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        Hal_Inst_Heater_Relay_On();
    }
    else
    {
        Hal_Inst_Heater_Relay_Off();
    }
}


/// @brief      �����¼� ���� Triac ���� �Լ�
/// @details    �����¼� ���� Triac�� �����Ѵ�
/// @param      mu8duty : Triac Control Duty - 0~100
/// @return     void
void Set_Inst_Heater_Triac_Duty(U8 mu8Duty)
{
    if (mu8Duty >= 100)
    {
        gu8InstHeaterTriacControlDuty = INST_HEATER_TRIAC_CONTROL_CYCLE_TIME;
    }
    else
    {
        gu8InstHeaterTriacControlDuty = (U8)(mu8Duty * ((F32)(INST_HEATER_TRIAC_CONTROL_CYCLE_TIME) / 100.0f));
    }
}


/// @brief      �����¼� ���� ���� ���� Ȯ�� �Լ�
/// @details    �����¼� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �����¼� ���� ���� ���� - 3(Heater All On), 2(Heater Relay On), 1(Heater Triac On), 0(All Off)
U8 Get_Inst_Heater_Status(void)
{
    U8 mu8RelayStatus = 0;
    U8 mu8TriacStatus = 0;

    mu8RelayStatus = GET_STATUS_INST_HEATER_RELAY();
    mu8TriacStatus = GET_STATUS_INST_HEATER_TRIAC();

    if ( (mu8RelayStatus != 0) && (mu8TriacStatus != 0) )
    {
        return INST_HEATER_ALL_ON;
    }
    else if (mu8TriacStatus != 0)
    {
        return INST_HEATER_TRIAC_ON;
    }
    else if (mu8RelayStatus != 0)
    {
        return INST_HEATER_RELAY_ON;
    }
    else
    {
        return INST_HEATER_ALL_OFF;
    }
}


/// @brief      �����¼� ���� Relay ON �Լ�
/// @details    �����¼� ���� �����̸� ON ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Relay_On(void)
{
    TURN_ON_INST_HEATER_RELAY();
}


/// @brief      �����¼� ���� Relay OFF �Լ�
/// @details    �����¼� ���� �����̸� OFF ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Relay_Off(void)
{
    TURN_OFF_INST_HEATER_RELAY();
}


/// @brief      �����¼� ���� Triac ON �Լ�
/// @details    �����¼� ���� Triac�� ON ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_On(void)
{
    TURN_ON_INST_HEATER_TRIAC();
}


/// @brief      �����¼� ���� Triac OFF �Լ�
/// @details    �����¼� ���� Triac�� OFF ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_Off(void)
{
    TURN_OFF_INST_HEATER_TRIAC();
}


/// @brief      �����¼� Triac�� Duty ���� �Լ�(�ݵ�� 1ms Timer Interrupt���� ó��)
/// @details    �����¼� Triac�� Duty ��� �°� ���� ON ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_Duty_Control(void)
{
    if (gu8InstHeaterTriacControlDuty == 0)
    {
        Hal_Inst_Heater_Triac_Off();
    }
    else
    {
        if (gu8InstHeaterTriacControlCycle <= gu8InstHeaterTriacControlDuty)
        {
            Hal_Inst_Heater_Triac_On();
        }
        else
        {
            Hal_Inst_Heater_Triac_Off();
        }
    }

    if (++gu8InstHeaterTriacControlCycle > INST_HEATER_TRIAC_CONTROL_CYCLE_TIME)
    {
        gu8InstHeaterTriacControlCycle = 0;
    }
}


#endif


// Hal Heater Module *****************************************************************************************

/// @brief      Hal Inst Heater Initilaize Module
/// @details    �����¼� ���� ���� ���� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_Initialize(void)
{
#if (INSTANT_HEATER_USE == USE)
    Hal_Inst_Heater_Initialize();           // ���� �¼� ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Inst Heater Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� �¼� ���� ����� ���õ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_1ms_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeaterControlTimer();               // ���� �¼� ���� ���� �ð� Counter
    Hal_Inst_Heater_Triac_Duty_Control();   // ���� �¼� Triac Port ���� �Լ�
#endif
}


/// @brief      Hal Inst Heater Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� �¼� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeaterControl();                    // ���� �¼� ���� Control �Լ�
#endif
}

