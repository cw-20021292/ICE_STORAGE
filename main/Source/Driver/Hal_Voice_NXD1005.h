/// @file     Hal_Voice_NXD1005.h
/// @date     2025/04/01
/// @author   Jaejin Ham
/// @brief    Voice IC NXD1005 Control file


#ifndef _HAL_VOICE_IC_H_
#define _HAL_VOICE_IC_H_


/*  Voice IC(NXD1005) ���� Function ����
    1. Voice Setup
     => Set_Voice(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume);
     => Set_VoiceLanguage(U8 mu8Language);
     => Set_VoiceType(U8 mu8SoundType);
     => Set_VoiceVolume(U8 mu8Volume);
            mu8Language : �����Ϸ��� ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
            mu8SoundType : �����Ϸ��� Voice�� ���� - 0(Voice), 1(Melody), 2(Mute)
            mu8Volume : �����Ϸ��� Voice�� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)

     => Get_VoiceLanguage();            // ���� ������ ��� ���� Ȯ��
     => Get_VoiceType();                // ���� ������ ����� ����(����, ��ε�, ����) Ȯ��
     => Get_VoiceVolume();              // ���� ������ ��� ���� ���� Ȯ��

    2. Voice, Melody Out
     => Set_PlayVoice(U16 mu16MemoryAddress);        // Address�� ���� ���

    ��� ��)
    Set_Voice(LANGUAGE_TYPE_KOREA, SOUND_TYPE_VOICE, SOUND_VOLUME_LEVEL_3);
    Set_VoiceLanguage(LANGUAGE_TYPE_KOREA);
    Set_VoiceType(SOUND_TYPE_VOICE);
    Set_VoiceVolume(SOUND_VOLUME_LEVEL_3);

    u8Return = Get_VoiceLanguage();
    u8Return = Get_VoiceType();
    u8Return = Get_VoiceVolume();

*/

#define VOICE_IC_USE                                    USE         // Voice IC ��� ���� : 0(No Use), 1(Use)

#if (VOICE_IC_USE == USE)
// Voice IC port �Ҵ�
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
#define PORT_DEFINE_VOICE_RESET                         P3_bit.no6
#define PORT_DEFINE_VOICE_CSB                           P3_bit.no5
#define PORT_DEFINE_VOICE_CLK                           P3_bit.no4
#define PORT_DEFINE_VOICE_DATA                          P3_bit.no3

#define SOUND_VOLUME_LEVEL_1                   0
#define SOUND_VOLUME_LEVEL_2                   1
#define SOUND_VOLUME_LEVEL_3                   2
#define SOUND_VOLUME_LEVEL_4                   3
#define SOUND_VOLUME_LEVEL_5                   4

#define LANGUAGE_TYPE_KOREA                    0
#define LANGUAGE_TYPE_ENGLISH                  1
#define LANGUAGE_TYPE_SPANISH                  2
#define LANGUAGE_TYPE_CHINESE                  3
#define LANGUAGE_TYPE_FRANCE                   4

#define LANGUAGE_MAX                           4

/*NXD Address*/
#define VOICE_START 0x00
//==============================================================
#define KR_OFFSET                               0       // �ѱ���
#define US_OFFSET                               200     // ����
#define ES_OFFSET                               400     // �����ξ�
#define CN_OFFSET                               600     // �߱���
#define FR_OFFSET                               800     // ��������

#define P_VOICE_RESET                           PORT_DEFINE_VOICE_RESET
#define P_VOICE_CSB                             PORT_DEFINE_VOICE_CSB
#define P_VOICE_CLK                             PORT_DEFINE_VOICE_CLK
#define P_VOICE_DATA                            PORT_DEFINE_VOICE_DATA

#define MK_COMMAND                              0x8000

/*NXD Command*/
#define VOICE_COMMAND_ADDRESS_0                 0xE000

// ���� ���
#define VOICE_COMMAND_VOLUME_5                  0x0021  /* 33 - volume 8 */
#define VOICE_COMMAND_VOLUME_4                  0x0024  /* 36 - volume 7 */
#define VOICE_COMMAND_VOLUME_3                  0x0027  /* 39 - volume 6 */
#define VOICE_COMMAND_VOLUME_2                  0x002A  /* 42 - volume 5 */
#define VOICE_COMMAND_VOLUME_1                  0x0030  /* 48 - volume 4 */

// ȿ����
#define BEEP_COMMAND_VOLUME_5                   0x0021  /* 33 - volume 8 */
#define BEEP_COMMAND_VOLUME_4                   0x0024  /* 36 - volume 7 */
#define BEEP_COMMAND_VOLUME_3                   0x0027  /* 39 - volume 6 */
#define BEEP_COMMAND_VOLUME_2                   0x002A  /* 42 - volume 5 */
#define BEEP_COMMAND_VOLUME_1                   0x0030  /* 48 - volume 4 */

#define VOICE_COMMAND_VOLUME_INITIAL            0xE121     /* -9 dB */
#define VOICE_COMMAND_VOLUME_MINIUM             0xE100
#define VOICE_COMMAND_VOLUME_MAXIUM             0xE157

#define VOICE_COMMAND_CONTROL2                  0xE260 /* operating mode: normal play */
                                                       /* Repeat: 1 time play */
#define VOICE_COMMAND_STOP                      0xE210

#define VOICE_COMMAND_CONTROL3                  0xE305 //..Disable, �񵿱�, 18bit, disable..//
#define VOICE_COMMAND_ADDRESS_256               0xE400
#define VOICE_COMMAND_ADDRESS_512               0xE401
#define VOICE_COMMAND_ADDRESS_768               0xE402
#define VOICE_COMMAND_ADDRESS_1024              0xE403
#define VOICE_COMMAND_ADDRESS_1280              0xE404
#define VOICE_COMMAND_ADDRESS_1536              0xE405
#define VOICE_COMMAND_ADDRESS_1792              0xE406
#define VOICE_COMMAND_ADDRESS_2048              0xE407

#define VOICE_IC_CONTROL_TIME_PERIOD            100         // 100ms@1ms


void Delay_NextLab(U16 mu16Time);
void Voice_Initialize(void);
void Voice_ControlTimer(void);
void Voice_Control(void);
void Set_Voice(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume);
void Set_VoiceLanguage(U8 mu8Language);
U8 Get_VoiceLanguage(void);
void Set_VoiceType(U8 mu8SoundType);
U8 Get_VoiceType(void);
void Set_VoiceVolume(U8 mu8Volume);
U8 Get_VoiceVolume(void);
U8 Check_VoiceOutput(U16 mu16Address);
void Set_PlayVoice(U16 mu16MemoryAddress);

void Initial_Voice_NextLab(void);
void ProcessVoice_NextLab(void);
void VoiceStopNextLab(void);
void SendVolumeNextLab(void);
void SendDataNextLab(void);
void SEND_SPI_COMMAND_NEXT_LAB(void);

#endif


void Hal_Voice_Module_Initialize(void);
void Hal_Voice_Module_1ms_Control(void);
void Hal_Voice_Module_Control(void);

#endif

