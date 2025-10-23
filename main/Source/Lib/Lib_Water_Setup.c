/// @file     Lib_Water_Setup.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    제품 설정 관련 제어 file

#include "Global_Header.h"


U8 gu8HotKeyLockStatus = 0;                         /// @brief  온수 버튼 잠금 상태 - 0(미잠김), 1(잠김)
U8 gu8AllKeyLockStatus = 0;                         /// @brief  전체 버튼 잠금 상태 - 0(미잠김), 1(잠김)


#if (BUZZER_USE == USE)
/// @brief      음 출력 설정 함수
/// @details    음 출력 여부를 설정한다
/// @param      mu8SoundType : 음 출력 상태 - 0(무음 모드), 1(효과음 모드)
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


/// @brief      음 출력 설정 상태 확인 함수
/// @details    음 출력 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 음 출력 상태 - 0(무음 모드), 1(효과음 모드)
U8 Get_SoundMode(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_EEPROM_Data(EEPROM_ID_VOICE_TYPE);

    return  mu8Return;
}
#endif

#if (VOICE_IC_USE == USE)
/// @brief      음 출력 설정 함수
/// @brief      출력되는 음의 언어 종류, 출력음 종류, 볼륨 크기를 설정한다
/// @param      mu8Language : 설정하려는 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
///             mu8SoundType : 설정하려는 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
///             mu8Volume : 설정하려는 Voice의 볼륨 크기 - 0(1단계) ~ 4(5단계)
/// @return     void
void Set_VoiceICSoundMode(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume)
{
    Set_Voice(mu8Language, mu8SoundType, mu8Volume);
}


/// @brief      음 출력 언어 설정 함수
/// @brief      출력되는 음의 언어 종류를 설정한다
/// @param      mu8Language : 설정하려는 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
/// @return     void
void Set_VoiceICSoundLanguage(U8 mu8Language)
{
    Set_VoiceLanguage(mu8Language);
}


/// @brief      음 출력 언어 설정 상태 확인 함수
/// @details    음 출력 언어 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 설정된 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
U8 Get_VoiceICSoundLanguage(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceLanguage();

    return  mu8Return;
}


/// @brief      음 출력 종류 설정 함수
/// @brief      출력되는 음의 출력음 종류를 설정한다
/// @param      mu8SoundType : 설정하려는 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
/// @return     void
void Set_VoiceICSoundType(U8 mu8SoundType)
{
    Set_VoiceType(mu8SoundType);
}


/// @brief      음 출력 종류 설정 상태 확인 함수
/// @details    음 출력 종류 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 설정된 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
U8 Get_VoiceICSoundType(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceType();

    return  mu8Return;
}


/// @brief      음 출력 볼륨 크기 설정 함수
/// @brief      출력되는 음의 볼륨 크기를 설정한다
/// @param      mu8Volume : 설정하려는 Voice의 볼륨 크기 - 0(1단계) ~ 4(5단계)
/// @return     void
void Set_VoiceICSoundVolume(U8 mu8Volume)
{
    Set_VoiceVolume(mu8Volume);
}


/// @brief      음 출력 볼륨 설정 상태 확인 함수
/// @details    음 출력 볼륨 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 설정된 Voice의 볼륨 크기 - 0(1단계) ~ 4(5단계)
U8 Get_VoiceICSoundVolume(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_VoiceVolume();

    return  mu8Return;
}
#endif


#if (NORMAL_HEATER_USE == USE)                     // 일반 히터를 사용하는 경우

/// @brief      온수 고도 모드 설정 함수
/// @details    온수 고도 모드의 단계를 설정한다
/// @param      mu8Step : 고도 모드 단계 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
/// @return     void
void Set_HeightModeStatus(U8 mu8Step)
{
    Set_HeightMode(mu8Step);
}


/// @brief      온수 고도 모드 설정 상태 확인 함수
/// @details    온수 고도 모드 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 고도 모드 단계 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
U8 Get_HightModeStatus(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_HeightMode();

    return  mu8Return;
}


/// @brief      온수 기능 ON/OFF 상태 설정 함수
/// @details    온수 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 온수 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_HotOnOff(U8 mu8OnOff)
{
    Set_HotFuncStatus(mu8OnOff);
}


/// @brief      온수 기능 ON/OFF 설정 상태 확인 함수
/// @details    온수 기능 ON/OFF 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 온수 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_HotOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_HotFuncStatus();

    return  mu8Return;
}

#endif      // 일반형 히터 사용 여부 조건 END


/// @brief      냉수 기능 ON/OFF 상태 설정 함수
/// @details    냉수 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 냉수 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_ColdOnOff(U8 mu8OnOff)
{
    Set_ColdFuncStatus(mu8OnOff);
}


/// @brief      냉수 기능 ON/OFF 설정 상태 확인 함수
/// @details    냉수 기능 ON/OFF 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 냉수 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_ColdOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_ColdFuncStatus();

    return  mu8Return;
}


#if  (SMART_NO_USE_POWER_SAVE_USE == USE)
/// @brief      미사용 절전 기능 ON/OFF 상태 설정 함수
/// @details    미사용 절전 기능 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 미사용 절전 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_NoUsePowerSaveFucOnOff(U8 mu8OnOff)
{
    Set_NoUsePowerSaveFuncSet(mu8OnOff);
}


/// @brief      미사용 절전 기능 ON/OFF 설정 상태 확인 함수
/// @details    미사용 절전 기능 ON/OFF 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 미사용 절전 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_NoUsePowerSaveFucOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_NoUsePowerSaveFuncSet();

    return  mu8Return;
}
#endif


#if (POWER_SAVE_USE == USE)
/// @brief      절전 기능 ON/OFF 상태 설정 함수
/// @details    절전 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 절전 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_PowerSaveFucOnOff(U8 mu8OnOff)
{
    Set_PowerSaveFuncSet(mu8OnOff);
}


/// @brief      절전 기능 ON/OFF 설정 상태 확인 함수
/// @details    절전 기능 ON/OFF 설정 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 절전 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_PowerSaveFucOnOff(void)
{
    U8 mu8Return = 0;

    mu8Return = Get_PowerSaveFuncSet();

    return  mu8Return;
}
#endif


/// @brief      온수 버튼 잠금 기능 ON/OFF 상태 설정 함수
/// @details    온수 버튼 잠금 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 온수 버튼 잠금금 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_HotKeyLock(U8 mu8OnOff)
{
    gu8HotKeyLockStatus = mu8OnOff;
}


/// @brief      온수 버튼 잠금 기능 ON/OFF 상태 확인 함수
/// @details    온수 버튼 잠금 기능 상태가 ON인지 OFF인지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 온수 버튼 잠금 기능 설정 상태 - 0(OFF), 1(ON)
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


/// @brief      전체 버튼 잠금 기능 ON/OFF 상태 설정 함수
/// @details    전체 버튼 잠금 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8OnOff : 전체 버튼 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_AllKeyLock(U8 mu8OnOff)
{
    gu8AllKeyLockStatus = mu8OnOff;
}


/// @brief      전체 버튼 잠금 기능 ON/OFF 상태 확인 함수
/// @details    전체 버튼 잠금 기능 상태가 ON인지 OFF인지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 전체 버튼 잠금 기능 설정 상태 - 0(OFF), 1(ON)
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



