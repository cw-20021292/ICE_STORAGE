/// @file     Lib_Water_Setup.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    ��ǰ ���� ���� ���� file

#include "Global_Header.h"


U8 gu8HotKeyLockStatus = 0;                         /// @brief  �¼� ��ư ��� ���� - 0(�����), 1(���)
U8 gu8AllKeyLockStatus = 0;                         /// @brief  ��ü ��ư ��� ���� - 0(�����), 1(���)


#if (BUZZER_USE == USE)
/// @brief      �� ��� ���� �Լ�
/// @details    �� ��� ���θ� �����Ѵ�
/// @param      mu8SoundType : �� ��� ���� - 0(���� ���), 1(ȿ���� ���)
/// @return     void
void Set_SoundMode(U8 mu8SoundType)
{
    if (mu8SoundType == SOUND_TYPE_MUTE)
    {
        Set_BuzzerMode(SOUND_TYPE_MUTE);
    }
    else
    {
        Set_BuzzerMode(SOUND_TYPE_MELODY);
    }
}


/// @brief      �� ��� ���� ���� Ȯ�� �Լ�
/// @details    �� ��� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �� ��� ���� - 0(���� ���), 1(ȿ���� ���)
U8 Get_SoundMode(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_EEPROM_Data(EEPROM_ID_VOICE_TYPE);

    return  mu8Return;
}
#endif

#if (VOICE_IC_USE == USE)
/// @brief      �� ��� ���� �Լ�
/// @brief      ��µǴ� ���� ��� ����, ����� ����, ���� ũ�⸦ �����Ѵ�
/// @param      mu8Language : �����Ϸ��� ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
///             mu8SoundType : �����Ϸ��� Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
///             mu8Volume : �����Ϸ��� Voice�� ���� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
/// @return     void
void Set_VoiceICSoundMode(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume)
{
    Set_Voice(mu8Language, mu8SoundType, mu8Volume);
}


/// @brief      �� ��� ��� ���� �Լ�
/// @brief      ��µǴ� ���� ��� ������ �����Ѵ�
/// @param      mu8Language : �����Ϸ��� ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
/// @return     void
void Set_VoiceICSoundLanguage(U8 mu8Language)
{
    Set_VoiceLanguage(mu8Language);
}


/// @brief      �� ��� ��� ���� ���� Ȯ�� �Լ�
/// @details    �� ��� ��� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
U8 Get_VoiceICSoundLanguage(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceLanguage();

    return  mu8Return;
}


/// @brief      �� ��� ���� ���� �Լ�
/// @brief      ��µǴ� ���� ����� ������ �����Ѵ�
/// @param      mu8SoundType : �����Ϸ��� Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
/// @return     void
void Set_VoiceICSoundType(U8 mu8SoundType)
{
    Set_VoiceType(mu8SoundType);
}


/// @brief      �� ��� ���� ���� ���� Ȯ�� �Լ�
/// @details    �� ��� ���� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
U8 Get_VoiceICSoundType(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceType();

    return  mu8Return;
}


/// @brief      �� ��� ���� ũ�� ���� �Լ�
/// @brief      ��µǴ� ���� ���� ũ�⸦ �����Ѵ�
/// @param      mu8Volume : �����Ϸ��� Voice�� ���� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
/// @return     void
void Set_VoiceICSoundVolume(U8 mu8Volume)
{
    Set_VoiceVolume(mu8Volume);
}


/// @brief      �� ��� ���� ���� ���� Ȯ�� �Լ�
/// @details    �� ��� ���� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Voice�� ���� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
U8 Get_VoiceICSoundVolume(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceVolume();

    return  mu8Return;
}
#endif


#if (NORMAL_HEATER_USE == USE)                     // �Ϲ� ���͸� ����ϴ� ���

/// @brief      �¼� �� ��� ���� �Լ�
/// @details    �¼� �� ����� �ܰ踦 �����Ѵ�
/// @param      mu8Step : �� ��� �ܰ� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
/// @return     void
void Set_HeightModeStatus(U8 mu8Step)
{
    Set_HeightMode(mu8Step);
}


/// @brief      �¼� �� ��� ���� ���� Ȯ�� �Լ�
/// @details    �¼� �� ��� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �� ��� �ܰ� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
U8 Get_HightModeStatus(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_HeightMode();

    return  mu8Return;
}


/// @brief      �¼� ��� ON/OFF ���� ���� �Լ�
/// @details    �¼� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : �¼� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_HotOnOff(U8 mu8OnOff)
{
    Set_HotFuncStatus(mu8OnOff);
}


/// @brief      �¼� ��� ON/OFF ���� ���� Ȯ�� �Լ�
/// @details    �¼� ��� ON/OFF ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �¼� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_HotOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_HotFuncStatus();

    return  mu8Return;
}

#endif      // �Ϲ��� ���� ��� ���� ���� END


/// @brief      �ü� ��� ON/OFF ���� ���� �Լ�
/// @details    �ü� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : �ü� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_ColdOnOff(U8 mu8OnOff)
{
    Set_ColdFuncStatus(mu8OnOff);
}


/// @brief      �ü� ��� ON/OFF ���� ���� Ȯ�� �Լ�
/// @details    �ü� ��� ON/OFF ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_ColdOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_ColdFuncStatus();

    return  mu8Return;
}


#if  (SMART_NO_USE_POWER_SAVE_USE == USE)
/// @brief      �̻�� ���� ��� ON/OFF ���� ���� �Լ�
/// @details    �̻�� ���� ��� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : �̻�� ���� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_NoUsePowerSaveFucOnOff(U8 mu8OnOff)
{
    Set_NoUsePowerSaveFuncSet(mu8OnOff);
}


/// @brief      �̻�� ���� ��� ON/OFF ���� ���� Ȯ�� �Լ�
/// @details    �̻�� ���� ��� ON/OFF ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �̻�� ���� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_NoUsePowerSaveFucOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_NoUsePowerSaveFuncSet();

    return  mu8Return;
}
#endif


#if (POWER_SAVE_USE == USE)
/// @brief      ���� ��� ON/OFF ���� ���� �Լ�
/// @details    ���� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : ���� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_PowerSaveFucOnOff(U8 mu8OnOff)
{
    Set_PowerSaveFuncSet(mu8OnOff);
}


/// @brief      ���� ��� ON/OFF ���� ���� Ȯ�� �Լ�
/// @details    ���� ��� ON/OFF ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_PowerSaveFucOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_PowerSaveFuncSet();

    return  mu8Return;
}
#endif


/// @brief      �¼� ��ư ��� ��� ON/OFF ���� ���� �Լ�
/// @details    �¼� ��ư ��� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : �¼� ��ư ��ݱ� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_HotKeyLock(U8 mu8OnOff)
{
    gu8HotKeyLockStatus = mu8OnOff;
}


/// @brief      �¼� ��ư ��� ��� ON/OFF ���� Ȯ�� �Լ�
/// @details    �¼� ��ư ��� ��� ���°� ON���� OFF���� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �¼� ��ư ��� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_HotKeyLockStatus(void)
{
    if (gu8HotKeyLockStatus)
    {
        return  HOT_KEY_LOCK_ON;
    }
    else
    {
        return  HOT_KEY_LOCK_OFF;
    }
}


/// @brief      ��ü ��ư ��� ��� ON/OFF ���� ���� �Լ�
/// @details    ��ü ��ư ��� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8OnOff : ��ü ��ư ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_AllKeyLock(U8 mu8OnOff)
{
    gu8AllKeyLockStatus = mu8OnOff;
}


/// @brief      ��ü ��ư ��� ��� ON/OFF ���� Ȯ�� �Լ�
/// @details    ��ü ��ư ��� ��� ���°� ON���� OFF���� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ��ü ��ư ��� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_AllKeyLockStatus(void)
{
    if (gu8AllKeyLockStatus)
    {
        return  ALL_KEY_LOCK_ON;
    }
    else
    {
        return  ALL_KEY_LOCK_OFF;
    }
}



