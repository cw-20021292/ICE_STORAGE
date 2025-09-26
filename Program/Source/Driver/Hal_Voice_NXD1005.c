/// @file     Hal_Voice_NXD1005.c
/// @date     2025/04/01
/// @author   Jaejin Ham
/// @brief    Voice IC NXD1005 Control file


#include "Global_Header.h"


#if (VOICE_IC_USE == USE)

U8 gu8VoiceControlTime = 0;         // @brief   음성 출력 제어 타이머

U16 gu16VoiceAddress = 0;           // @brief   음성 IC Address
U16 gu16VoiceCommand = 0;           // @brief   음성 IC Command

U8 gu8VoiceVolumeLevel = 0;         // @brief   음성 출력 볼륨
U8 gu8SoundType = 0;                // @brief   출력음 종류 - 0(무음), 1(효과음), 2(음성)

U8 gu8VoiceLanguageType = 0;        // @brief   출력 음성 언어

U8 gu8BeepSoundOut = 0;             // @brief   효과음 출력 여부 - 0(Beep음 미발생), 1(Beep음 발생)
U8 gu8VoiceSoundOut = 0;            // @brief   음성 출력 여부 - 0(음성출력 미발생), 1(음성출력 발생)

U16 gu16VoiceDelayTimer = 0;        // @brief   음성 출력간 delay Time


/// @brief    음성 IC의 음성 출력 볼륨 Registor값 테이블
/// @details  음성 IC의 음성 출력 볼륨을 설정하는 Registor값을 테이블로 정리한다
const U16 au16VoiceVolumeRegList[5] =
{
    VOICE_COMMAND_VOLUME_1,
    VOICE_COMMAND_VOLUME_2,
    VOICE_COMMAND_VOLUME_3,
    VOICE_COMMAND_VOLUME_4,
    VOICE_COMMAND_VOLUME_5
};

/// @brief    음성 IC의 효과음 출력 볼륨 Registor값 테이블
/// @details  음성 IC의 효과음 출력 볼륨을 설정하는 Registor값을 테이블로 정리한다
const U16 au16BeepVolumeRegList[5] =
{
    BEEP_COMMAND_VOLUME_1,
    BEEP_COMMAND_VOLUME_2,
    BEEP_COMMAND_VOLUME_3,
    BEEP_COMMAND_VOLUME_4,
    BEEP_COMMAND_VOLUME_5
};


/// @brief      Voice IC Delay function
/// @details    음성 IC 제어 시 delay 시키는 동작을 한다
/// @param      mu16Time : delay time
/// @return     void
void Delay_NextLab(U16 mu16Time)
{
    while (mu16Time)
    {
        mu16Time--;
        NOP();
    }
}


/// @brief      Voice IC Initialize
/// @details    음성 IC 제어와 관련된 모든 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Voice_Initialize(void)
{
    gu8VoiceControlTime = 0;

    gu16VoiceAddress = 0;
    gu16VoiceCommand = 0;

    gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_3;
    gu8SoundType = SOUND_TYPE_VOICE;                // 출력음 종류

    gu8VoiceLanguageType = LANGUAGE_TYPE_KOREA;     // 출력 음성 언어

    gu8BeepSoundOut = 0;             // 0(Beep음 미발생), 1(Beep음 발생)
    gu8VoiceSoundOut = 0;            // 0(음성출력 미발생), 1(음성출력  발생)

    gu16VoiceDelayTimer = 0;

    Initial_Voice_NextLab();
}


/// @brief      Voice IC Control Timer(@1ms)
/// @details    음성 IC를 제어하는 주기를 카운트 한다
/// @param      void
/// @return     void
void Voice_ControlTimer(void)
{
    if (gu8VoiceControlTime < VOICE_IC_CONTROL_TIME_PERIOD)
    {
        gu8VoiceControlTime++;
    }
}


/// @brief      Voice 동작 함수(@While)
/// @details    음성 IC 출력과 관련해서 While문 안에서 동작하며 제어를 한다
/// @param      void
/// @return     void
void Voice_Control(void)
{
    if (gu8VoiceControlTime >= VOICE_IC_CONTROL_TIME_PERIOD)
    {   // 100ms마다 수행
        gu8VoiceControlTime = 0;

        ProcessVoice_NextLab();

        gu16VoiceCommand = 0;   /*..모두 전송하고 나면 Command 초기화..*/
    }
}


/// @brief      Voice 설정 함수(언어, 출력음 종류, 볼륨)
/// @details    음성 IC를 통해 출력하려는 언어의 종류, 출력음 종류, 출력음 볼륨 크기를 설정한다
/// @param      mu8Language : 설정하려는 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
///             mu8SoundType : 설정하려는 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
///             mu8Volume : 설정하려는 Voice의 크기 - 0(1단계) ~ 4(5단계)
/// @return     void
void Set_Voice(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume)
{
    gu8VoiceLanguageType = mu8Language;
    gu8SoundType = mu8SoundType;
    gu8VoiceVolumeLevel = mu8Volume;
}


/// @brief      Voice 언어 설정 함수
/// @details    음성 IC를 통해 출력하려는 언어의 종류를 설정한다
/// @param      mu8Language : 설정하려는 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
/// @return     void
void Set_VoiceLanguage(U8 mu8Language)
{
    gu8VoiceLanguageType = mu8Language;
}


/// @brief      Voice의 언어 설정 상태 확인 함수
/// @details    음성 IC를 통해 출력하려는 언어의 종류 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 설정된 언어 종류 - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
U8 Get_VoiceLanguage(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8VoiceLanguageType;

    return  mu8Return;
}


/// @brief      Voice 출력음 종류 설정 함수
/// @details    음성 IC로 출력하려는 출력음 종류를 설정한다
/// @param      mu8SoundType : 설정하려는 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
/// @return     void
void Set_VoiceType(U8 mu8SoundType)
{
    gu8SoundType = mu8SoundType;
}


/// @brief      Voice의 출력음 설정 상태 확인 함수
/// @details    음성 IC로 출력하려는 출력음 종류 설정을 확인하여 반환한다
/// @param      void
/// @return     return : 설정된 Voice의 종류 - 0(Mute), 1(Melody), 2(Voice)
U8 Get_VoiceType(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8SoundType;

    return  mu8Return;
}


/// @brief      Voice의 출력음 볼륨을 설정하는 함수
/// @details    음성 IC로 출력하려는 출력음 볼륨의 크기를 설정한다
/// @param      mu8Volume : 설정하려는 Voice의 크기 - 0(1단계) ~ 4(5단계)
/// @return     void
void Set_VoiceVolume(U8 mu8Volume)
{
    gu8VoiceVolumeLevel = mu8Volume;
}


/// @brief      Voice의 출력음 볼륨 설정 상태 확인 함수
/// @details    음성 IC로 출력하려는 출력음 볼륨의 설정 크기를 확인하여 반환한다
/// @param      void
/// @return     return : 설정된 Voice의 크기 - 0(1단계) ~ 4(5단계)
U8 Get_VoiceVolume(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8VoiceVolumeLevel;

    return  mu8Return;
}


/// @brief      설정된 상태에 따라 음성 출력이 가능한 기능인지 체크하는 함수
/// @details    설정된 상태에 따라 음성 출력이 가능한 기능인지 확인하여 출력 여부를 결정한다
/// @param      mu16Address : 출력하고자 하는 음성 Address
/// @return     return : 음성 출력 여부 - 0(음성 미출력), 1(음성 출력)
U8 Check_VoiceOutput(U16 mu16Address)
{
    U8 mu8Return = 0;
    U8 mu8VoiceOutCritical  = 0;
    U8 mu8VoiceOutMute  = 0;
    U8 mu8VoiceOutMelody  = 0;
    U8 mu8VoiceOutVoice  = 0;

    mu8VoiceOutCritical = IsCriticalVoice(mu16Address);
    mu8VoiceOutMute = IsLevel_Mute(mu16Address);
    mu8VoiceOutMelody = IsLevel_Melody(mu16Address);
    mu8VoiceOutVoice = IsLevel_Voice(mu16Address);

    if (mu8VoiceOutCritical == TRUE)
    {
        mu8Return = 1;
    }
    else
    {
        if (gu8SoundType == SOUND_TYPE_MUTE)
        {
            if (mu8VoiceOutMute == TRUE)
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
        }
        else if (gu8SoundType == SOUND_TYPE_MELODY)
        {
            if ( (mu8VoiceOutMute == TRUE) || (mu8VoiceOutMelody == TRUE) )
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
        }
        else if (gu8SoundType == SOUND_TYPE_VOICE)
        {
            if ( (mu8VoiceOutMute == TRUE) || (mu8VoiceOutMelody == TRUE) || (mu8VoiceOutVoice == TRUE) )
            {
                mu8Return = 1;
            }
            else
            {
                mu8Return = 0;
            }
        }
        else
        {
            mu8Return = 0;
        }
    }

    return    mu8Return;
}


/// @brief      Voice Play Set
/// @details    음성 IC의 Address를 배정하여 출력할 수 있도록 한다
/// @param      mu16MemoryAddress : 출력하려는 음성 Address
/// @return     void
void Set_PlayVoice(U16 mu16MemoryAddress)
{
    gu16VoiceAddress = mu16MemoryAddress;

    gu8VoiceSoundOut = Check_VoiceOutput(gu16VoiceAddress);
}


/// @brief      Voice Play Initialize
/// @details    음성 IC를 초기화 시킨다
/// @param      void
/// @return     void
void Initial_Voice_NextLab(void)
{
    P_VOICE_RESET = SET;

    Delay_NextLab(400);

    //set E2, E3 control Register
    gu16VoiceCommand = VOICE_COMMAND_CONTROL2; //set Reg. E2 = 0x60
    SEND_SPI_COMMAND_NEXT_LAB();

    Delay_NextLab(400);

    gu16VoiceCommand = VOICE_COMMAND_CONTROL3; //set Reg. E3 = 0x05
    SEND_SPI_COMMAND_NEXT_LAB();

    Delay_NextLab(400);
}


/// @brief      Voice IC Play Control
/// @details    음성 IC의 음성 출력을 제어한다
/// @param      void
/// @return     void
void ProcessVoice_NextLab(void)
{
    if (gu8VoiceSoundOut == 1)
    {
        VoiceStopNextLab();
        SendVolumeNextLab();
        SendDataNextLab();
        gu8VoiceSoundOut = 0;
        gu16VoiceDelayTimer = 0;
    }
    else
    {
        gu16VoiceDelayTimer = 0;
    }

    gu8BeepSoundOut = 0;
    gu16VoiceCommand = 0;
}


/// @brief      Voice Stop Command
/// @details    음성 IC의 Stop Command를 송신한다
/// @param      void
/// @return     void
void VoiceStopNextLab(void)
{
    gu16VoiceCommand = VOICE_COMMAND_STOP;
    SEND_SPI_COMMAND_NEXT_LAB();
}


/// @brief      Voice Volume Command
/// @details    음성 IC의 볼륨 조절 Command를 송신한다
/// @param      void
/// @return     void
void SendVolumeNextLab(void)
{
    U8 mu8VoiceOutCritical  = 0;

    if ( (gu16VoiceAddress >= VOICE_192_MELODY_DISPENSE_CONTINUOUS)  && (gu16VoiceAddress <= VOICE_199_MELODY_SET_OFF) )
    {   // 효과음 출력인 경우
        gu8BeepSoundOut = 1;
    }
    else
    {
        gu8BeepSoundOut = 0;
    }

    mu8VoiceOutCritical = IsCriticalVoice(gu16VoiceAddress);

    if (mu8VoiceOutCritical == TRUE)
    {   // Critical 관련 음성은 설정과 무관하게 최대 볼륨으로
        gu16VoiceCommand = 0xE100 + VOICE_COMMAND_VOLUME_5;
    }
    else
    {
        if (gu8BeepSoundOut == 1)
        {
            gu16VoiceCommand = 0xE100 + au16BeepVolumeRegList[gu8VoiceVolumeLevel];
        }
        else
        {
            gu16VoiceCommand = 0xE100 + au16VoiceVolumeRegList[gu8VoiceVolumeLevel];
        }
    }

    Delay_NextLab(400);   // 약25us
    SEND_SPI_COMMAND_NEXT_LAB();
    Delay_NextLab(400);   // 약25us
}


/// @brief      Voice Data Command
/// @details    음성 IC로 출력하는 음성 Address Data를 송신한다
/// @param      void
/// @return     void
void SendDataNextLab(void)
{
    if (gu8VoiceLanguageType == LANGUAGE_TYPE_ENGLISH)
    {
        gu16VoiceAddress = gu16VoiceAddress + US_OFFSET;
    }
    else if (gu8VoiceLanguageType == LANGUAGE_TYPE_SPANISH)
    {
        gu16VoiceAddress = gu16VoiceAddress + ES_OFFSET;
    }
    else if (gu8VoiceLanguageType == LANGUAGE_TYPE_CHINESE)
    {
        gu16VoiceAddress = gu16VoiceAddress + CN_OFFSET;
    }
    else if (gu8VoiceLanguageType == LANGUAGE_TYPE_FRANCE)
    {
        gu16VoiceAddress = gu16VoiceAddress + FR_OFFSET;
    }
    else // if (gu8_voice_language_type == LANGUAGE_TYPE_KOREA)
    {
        gu16VoiceAddress = gu16VoiceAddress + KR_OFFSET;
    }

    //set $E4 register = 00 if address is < 256
    if (gu16VoiceAddress < 256)
    {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_256; //phase group=0, high address = 000
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + gu16VoiceAddress; //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x01 if address is 256~511
      else if (gu16VoiceAddress < 512)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_512; //phrase group = 1, high address = 1
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 256); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x02 if address is 512~767
      else if (gu16VoiceAddress < 768)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_768; //phrase group = 2, high address = 010
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 512); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x03 if address is 768~1023
      else if (gu16VoiceAddress < 1024)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1024; //phrase group = 3, high address = 011
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 768); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x04 if address is 1024~1279
      else if (gu16VoiceAddress < 1280)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1280; //phrase group = 4, high address = 100
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1024); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x05 if address is 1280~1535
      else if (gu16VoiceAddress < 1536)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1536; //phrase group = 5, high address = 101
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1280); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x06 if address is 1536~1791
      else if (gu16VoiceAddress < 1792)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1792; //phrase group = 6, high address = 110
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1536); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x07 if address is 1792~2047
      else if (gu16VoiceAddress < 2048)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_2048; //phrase group = 7, high address = 111
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // 약25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1792); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      else
      {
         /*..출력 안함..*/
      }
}


/// @brief      SPI Communication with NEXTLAB Voice IC
/// @details    음성 IC의 SPI 통신을 정의한다
/// @param      void
/// @return     void
void SEND_SPI_COMMAND_NEXT_LAB(void)
{
   U8 mu8i = 0;

   P_VOICE_CSB = 0;                 //start SPI
   Delay_NextLab(24000);            // CSB Low 후 약 1.4ms
   P_VOICE_CLK = 0;              //stand-by status,

   /*16bit 처리*/
   while (mu8i < 16)
   {
      P_VOICE_CLK = 0;
      Delay_NextLab(160);          // 약10us

      if(gu16VoiceCommand & MK_COMMAND)
      {
         P_VOICE_DATA = 1;
      }
      else
      {
         P_VOICE_DATA = 0;
      }

      gu16VoiceCommand <<= 1; //shift left
      Delay_NextLab(160);          // 약10us

      P_VOICE_CLK = 1;

      mu8i++;

       if(mu8i == 8)
       {
           Delay_NextLab(640);      // 약40us (1Byte-1byte 패킷구분)
       }
       else
       {
           Delay_NextLab(320);      // 약20us
       }
   }

   P_VOICE_DATA = 0;
   Delay_NextLab(8);                    // 0.5us
   P_VOICE_CSB = 1; //stand-by status
   Delay_NextLab(320);                  // 20us
}


#endif


// Hal Voice IC Module ***************************************************************************************

/// @brief      Hal Voice Initilaize Module
/// @details    음성 IC 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Voice_Module_Initialize(void)
{
#if (VOICE_IC_USE == USE)
    Voice_Initialize();                     // Voice IC 초기화
#endif
}


/// @brief      Hal Voice Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 음성 IC 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_Voice_Module_1ms_Control(void)
{
#if (VOICE_IC_USE == USE)
    Voice_ControlTimer();                   // Voice IC Control Timer
#endif
}


/// @brief      Hal Voice Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 음성 IC를 제어한다
/// @param      void
/// @return     void
void Hal_Voice_Module_Control(void)
{
#if (VOICE_IC_USE == USE)
    Voice_Control();                        // Voice 제어 함수
#endif
}
