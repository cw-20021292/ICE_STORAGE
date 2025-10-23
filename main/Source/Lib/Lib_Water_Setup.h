/// @file     Lib_Water_Setup.h
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    제품 설정 관련 제어 file


#ifndef _LIB_WATER_SETUP_H_
#define _LIB_WATER_SETUP_H_


/*  Lib Function 사용법
    Buzzer 사용시
      => Set_SoundMode(mu8SoundType);                     // 음 출력 여부 설정 - 0(무음 모드), 1(효과음 모드)
      => Get_SoundMode();                                 // 음 출력 여부 확인 - 0(무음 모드), 1(효과음 모드)
    Voice IC 사용시
      => Set_VoiceICSoundMode(mu8Language, mu8SoundType, mu8Volume);        // 출력되는 음의 언어 종류, 출력음 종류, 볼륨 크기 설정
      => Set_VoiceICSoundLanguage(mu8Language);                             // 출력하려는 언어 종류 설정 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
      => Get_VoiceICSoundLanguage();                                        // 출력하려는 언어 종류 확인 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
      => Set_VoiceICSoundType(mu8SoundType);                                // 출력하려는 Voice의 종류 설정 - 0(Mute), 1(Melody), 2(Voice)
      => Get_VoiceICSoundType();                                            // 출력하려는 Voice의 종류 확인 - 0(Mute), 1(Melody), 2(Voice)
      => Set_VoiceICSoundVolume(mu8Volume);                                 // 출력하려는 Voice의 볼륨 크기 설정 - 0(1단계) ~ 4(5단계)
      => Get_VoiceICSoundVolume();                                          // 출력하려는 Voice의 볼륨 크기 확인 - 0(1단계) ~ 4(5단계)

    일반 히터를 사용하는 경우
      => Set_HeightModeStatus(mu8Step);                                     // 고도 모드 단계 설정 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
      => Get_HightModeStatus();                                             // 고도 모드 단계 확인 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
      => Set_HotOnOff(mu8OnOff);                                            // 온수 기능 상태 설정 - 0(OFF), 1(ON)
      => Get_HotOnOff();                                                    // 온수 기능 상태 확인 - 0(OFF), 1(ON)

    => Set_ColdOnOff(mu8OnOff);                                             // 냉수 기능 상태 설정 - 0(OFF), 1(ON)
    => Get_ColdOnOff();                                                     // 냉수 기능 상태 확인 - 0(OFF), 1(ON)

    => Set_NoUsePowerSaveFucOnOff(mu8OnOff);                                // 미사용 절전 기능 상태 설정 - 0(OFF), 1(ON)
    => Get_NoUsePowerSaveFucOnOff();                                        // 미사용 절전 기능 상태 확인 - 0(OFF), 1(ON)

    => Set_PowerSaveFucOnOff(mu8OnOff);                                     // 일반 절전 기능 상태 설정 - 0(OFF), 1(ON)
    => Get_PowerSaveFucOnOff();                                             // 일반 절전 기능 상태 확인 - 0(OFF), 1(ON)

    => Set_HotKeyLock(mu8OnOff);                                            // 온수 버튼 잠금 기능 상태 설정 - 0(OFF), 1(ON)
    => Get_HotKeyLockStatus();                                              // 온수 버튼 잠금 기능 상태 확인 - 0(OFF), 1(ON)

    => Set_AllKeyLock(mu8OnOff);                                            // 전체 버튼 기능 상태 설정 - 0(OFF), 1(ON)
    => Get_AllKeyLockStatus();                                              // 전체 버튼 기능 상태 확인 - 0(OFF), 1(ON)
*/



// 음성 관련 설정 ************************************************************************
#define SOUND_TYPE_MUTE                         0
#define SOUND_TYPE_MELODY                       1
#define SOUND_TYPE_VOICE                        2

#define SOUNT_VOLUME_1                          0
#define SOUNT_VOLUME_2                          1
#define SOUNT_VOLUME_3                          2
#define SOUNT_VOLUME_4                          3
#define SOUND_VOLUME_5                          4


#if (BUZZER_USE == USE)
void Set_SoundMode(U8 mu8SoundType);
U8 Get_SoundMode(void);
#endif

#if (VOICE_IC_USE == USE)
void Set_VoiceICSoundMode(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume);
void Set_VoiceICSoundLanguage(U8 mu8Language);
U8 Get_VoiceICSoundLanguage(void);
void Set_VoiceICSoundType(U8 mu8SoundType);
U8 Get_VoiceICSoundType(void);
void Set_VoiceICSoundVolume(U8 mu8Volume);
U8 Get_VoiceICSoundVolume(void);
#endif


// 온수 기능 관련 ************************************************************************
#define HEAT_TEMP_STEP_1                0       // 고도모드1, default
#define HEAT_TEMP_STEP_2                1       // 고도모드2
#define HEAT_TEMP_STEP_3                2       // 고도모드3
#define HEAT_TEMP_STEP_4                3       // 고도모드4

#define HOT_OFF                         0       // 온수 기능 꺼짐
#define HOT_ON                          1       // 온수 기능 켜짐

#if (NORMAL_HEATER_USE == USE)                  // 일반 히터를 사용하는 경우
void Set_HeightModeStatus(U8 mu8Step);
U8 Get_HightModeStatus(void);
void Set_HotOnOff(U8 mu8Status);
U8 Get_HotOnOff(void);
#endif


// 냉수 기능 관련 ************************************************************************
#define COLD_OFF                        0               // 냉수 기능 켜짐
#define COLD_ON                         1               // 냉수 기능 꺼짐

void Set_ColdOnOff(U8 mu8Status);
U8 Get_ColdOnOff(void);

// 절전 기능 관련 ************************************************************************
#define NO_USE_POWER_SAVE_MODE_OFF      0
#define NO_USE_POWER_SAVE_MODE_ON       1

#define POWER_SAVE_MODE_OFF             0
#define POWER_SAVE_MODE_ON              1

#if  (SMART_NO_USE_POWER_SAVE_USE == USE)
void Set_NoUsePowerSaveFucOnOff(U8 mu8OnOff);
U8 Get_NoUsePowerSaveFucOnOff(void);
#endif

#if (POWER_SAVE_USE == USE)
void Set_PowerSaveFucOnOff(U8 mu8OnOff);
U8 Get_PowerSaveFucOnOff(void);
#endif


// 잠금 기능 관련 ************************************************************************
#define HOT_KEY_LOCK_OFF                0
#define HOT_KEY_LOCK_ON                 1

#define ALL_KEY_LOCK_OFF                0
#define ALL_KEY_LOCK_ON                 1

void Set_HotKeyLock(U8 mu8OnOff);
U8 Get_HotKeyLockStatus(void);
void Set_AllKeyLock(U8 mu8OnOff);
U8 Get_AllKeyLockStatus(void);

#endif

