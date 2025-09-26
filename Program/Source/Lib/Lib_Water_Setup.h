/// @file     Lib_Water_Setup.h
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    ��ǰ ���� ���� ���� file


#ifndef _LIB_WATER_SETUP_H_
#define _LIB_WATER_SETUP_H_


/*  Lib Function ����
    Buzzer ����
      => Set_SoundMode(mu8SoundType);                     // �� ��� ���� ���� - 0(���� ���), 1(ȿ���� ���)
      => Get_SoundMode();                                 // �� ��� ���� Ȯ�� - 0(���� ���), 1(ȿ���� ���)
    Voice IC ����
      => Set_VoiceICSoundMode(mu8Language, mu8SoundType, mu8Volume);        // ��µǴ� ���� ��� ����, ����� ����, ���� ũ�� ����
      => Set_VoiceICSoundLanguage(mu8Language);                             // ����Ϸ��� ��� ���� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
      => Get_VoiceICSoundLanguage();                                        // ����Ϸ��� ��� ���� Ȯ�� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
      => Set_VoiceICSoundType(mu8SoundType);                                // ����Ϸ��� Voice�� ���� ���� - 0(Mute), 1(Melody), 2(Voice)
      => Get_VoiceICSoundType();                                            // ����Ϸ��� Voice�� ���� Ȯ�� - 0(Mute), 1(Melody), 2(Voice)
      => Set_VoiceICSoundVolume(mu8Volume);                                 // ����Ϸ��� Voice�� ���� ũ�� ���� - 0(1�ܰ�) ~ 4(5�ܰ�)
      => Get_VoiceICSoundVolume();                                          // ����Ϸ��� Voice�� ���� ũ�� Ȯ�� - 0(1�ܰ�) ~ 4(5�ܰ�)

    �Ϲ� ���͸� ����ϴ� ���
      => Set_HeightModeStatus(mu8Step);                                     // �� ��� �ܰ� ���� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
      => Get_HightModeStatus();                                             // �� ��� �ܰ� Ȯ�� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
      => Set_HotOnOff(mu8OnOff);                                            // �¼� ��� ���� ���� - 0(OFF), 1(ON)
      => Get_HotOnOff();                                                    // �¼� ��� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_ColdOnOff(mu8OnOff);                                             // �ü� ��� ���� ���� - 0(OFF), 1(ON)
    => Get_ColdOnOff();                                                     // �ü� ��� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_NoUsePowerSaveFucOnOff(mu8OnOff);                                // �̻�� ���� ��� ���� ���� - 0(OFF), 1(ON)
    => Get_NoUsePowerSaveFucOnOff();                                        // �̻�� ���� ��� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_PowerSaveFucOnOff(mu8OnOff);                                     // �Ϲ� ���� ��� ���� ���� - 0(OFF), 1(ON)
    => Get_PowerSaveFucOnOff();                                             // �Ϲ� ���� ��� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_HotKeyLock(mu8OnOff);                                            // �¼� ��ư ��� ��� ���� ���� - 0(OFF), 1(ON)
    => Get_HotKeyLockStatus();                                              // �¼� ��ư ��� ��� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_AllKeyLock(mu8OnOff);                                            // ��ü ��ư ��� ���� ���� - 0(OFF), 1(ON)
    => Get_AllKeyLockStatus();                                              // ��ü ��ư ��� ���� Ȯ�� - 0(OFF), 1(ON)
*/



// ���� ���� ���� ************************************************************************
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


// �¼� ��� ���� ************************************************************************
#define HEAT_TEMP_STEP_1                0       // �����1, default
#define HEAT_TEMP_STEP_2                1       // �����2
#define HEAT_TEMP_STEP_3                2       // �����3
#define HEAT_TEMP_STEP_4                3       // �����4

#define HOT_OFF                         0       // �¼� ��� ����
#define HOT_ON                          1       // �¼� ��� ����

#if (NORMAL_HEATER_USE == USE)                  // �Ϲ� ���͸� ����ϴ� ���
void Set_HeightModeStatus(U8 mu8Step);
U8 Get_HightModeStatus(void);
void Set_HotOnOff(U8 mu8Status);
U8 Get_HotOnOff(void);
#endif


// �ü� ��� ���� ************************************************************************
#define COLD_OFF                        0               // �ü� ��� ����
#define COLD_ON                         1               // �ü� ��� ����

void Set_ColdOnOff(U8 mu8Status);
U8 Get_ColdOnOff(void);

// ���� ��� ���� ************************************************************************
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


// ��� ��� ���� ************************************************************************
#define HOT_KEY_LOCK_OFF                0
#define HOT_KEY_LOCK_ON                 1

#define ALL_KEY_LOCK_OFF                0
#define ALL_KEY_LOCK_ON                 1

void Set_HotKeyLock(U8 mu8OnOff);
U8 Get_HotKeyLockStatus(void);
void Set_AllKeyLock(U8 mu8OnOff);
U8 Get_AllKeyLockStatus(void);

#endif

