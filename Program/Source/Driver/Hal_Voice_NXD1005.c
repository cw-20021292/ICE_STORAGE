/// @file     Hal_Voice_NXD1005.c
/// @date     2025/04/01
/// @author   Jaejin Ham
/// @brief    Voice IC NXD1005 Control file


#include "Global_Header.h"


#if (VOICE_IC_USE == USE)

U8 gu8VoiceControlTime = 0;         // @brief   ���� ��� ���� Ÿ�̸�

U16 gu16VoiceAddress = 0;           // @brief   ���� IC Address
U16 gu16VoiceCommand = 0;           // @brief   ���� IC Command

U8 gu8VoiceVolumeLevel = 0;         // @brief   ���� ��� ����
U8 gu8SoundType = 0;                // @brief   ����� ���� - 0(����), 1(ȿ����), 2(����)

U8 gu8VoiceLanguageType = 0;        // @brief   ��� ���� ���

U8 gu8BeepSoundOut = 0;             // @brief   ȿ���� ��� ���� - 0(Beep�� �̹߻�), 1(Beep�� �߻�)
U8 gu8VoiceSoundOut = 0;            // @brief   ���� ��� ���� - 0(������� �̹߻�), 1(������� �߻�)

U16 gu16VoiceDelayTimer = 0;        // @brief   ���� ��°� delay Time


/// @brief    ���� IC�� ���� ��� ���� Registor�� ���̺�
/// @details  ���� IC�� ���� ��� ������ �����ϴ� Registor���� ���̺�� �����Ѵ�
const U16 au16VoiceVolumeRegList[5] =
{
    VOICE_COMMAND_VOLUME_1,
    VOICE_COMMAND_VOLUME_2,
    VOICE_COMMAND_VOLUME_3,
    VOICE_COMMAND_VOLUME_4,
    VOICE_COMMAND_VOLUME_5
};

/// @brief    ���� IC�� ȿ���� ��� ���� Registor�� ���̺�
/// @details  ���� IC�� ȿ���� ��� ������ �����ϴ� Registor���� ���̺�� �����Ѵ�
const U16 au16BeepVolumeRegList[5] =
{
    BEEP_COMMAND_VOLUME_1,
    BEEP_COMMAND_VOLUME_2,
    BEEP_COMMAND_VOLUME_3,
    BEEP_COMMAND_VOLUME_4,
    BEEP_COMMAND_VOLUME_5
};


/// @brief      Voice IC Delay function
/// @details    ���� IC ���� �� delay ��Ű�� ������ �Ѵ�
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
/// @details    ���� IC ����� ���õ� ��� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Voice_Initialize(void)
{
    gu8VoiceControlTime = 0;

    gu16VoiceAddress = 0;
    gu16VoiceCommand = 0;

    gu8VoiceVolumeLevel = SOUND_VOLUME_LEVEL_3;
    gu8SoundType = SOUND_TYPE_VOICE;                // ����� ����

    gu8VoiceLanguageType = LANGUAGE_TYPE_KOREA;     // ��� ���� ���

    gu8BeepSoundOut = 0;             // 0(Beep�� �̹߻�), 1(Beep�� �߻�)
    gu8VoiceSoundOut = 0;            // 0(������� �̹߻�), 1(�������  �߻�)

    gu16VoiceDelayTimer = 0;

    Initial_Voice_NextLab();
}


/// @brief      Voice IC Control Timer(@1ms)
/// @details    ���� IC�� �����ϴ� �ֱ⸦ ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Voice_ControlTimer(void)
{
    if (gu8VoiceControlTime < VOICE_IC_CONTROL_TIME_PERIOD)
    {
        gu8VoiceControlTime++;
    }
}


/// @brief      Voice ���� �Լ�(@While)
/// @details    ���� IC ��°� �����ؼ� While�� �ȿ��� �����ϸ� ��� �Ѵ�
/// @param      void
/// @return     void
void Voice_Control(void)
{
    if (gu8VoiceControlTime >= VOICE_IC_CONTROL_TIME_PERIOD)
    {   // 100ms���� ����
        gu8VoiceControlTime = 0;

        ProcessVoice_NextLab();

        gu16VoiceCommand = 0;   /*..��� �����ϰ� ���� Command �ʱ�ȭ..*/
    }
}


/// @brief      Voice ���� �Լ�(���, ����� ����, ����)
/// @details    ���� IC�� ���� ����Ϸ��� ����� ����, ����� ����, ����� ���� ũ�⸦ �����Ѵ�
/// @param      mu8Language : �����Ϸ��� ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
///             mu8SoundType : �����Ϸ��� Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
///             mu8Volume : �����Ϸ��� Voice�� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
/// @return     void
void Set_Voice(U8 mu8Language, U8 mu8SoundType, U8 mu8Volume)
{
    gu8VoiceLanguageType = mu8Language;
    gu8SoundType = mu8SoundType;
    gu8VoiceVolumeLevel = mu8Volume;
}


/// @brief      Voice ��� ���� �Լ�
/// @details    ���� IC�� ���� ����Ϸ��� ����� ������ �����Ѵ�
/// @param      mu8Language : �����Ϸ��� ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
/// @return     void
void Set_VoiceLanguage(U8 mu8Language)
{
    gu8VoiceLanguageType = mu8Language;
}


/// @brief      Voice�� ��� ���� ���� Ȯ�� �Լ�
/// @details    ���� IC�� ���� ����Ϸ��� ����� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ ��� ���� - 0(KR), 1(EN), 2(ES), 3(CN), 4(FR)
U8 Get_VoiceLanguage(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8VoiceLanguageType;

    return  mu8Return;
}


/// @brief      Voice ����� ���� ���� �Լ�
/// @details    ���� IC�� ����Ϸ��� ����� ������ �����Ѵ�
/// @param      mu8SoundType : �����Ϸ��� Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
/// @return     void
void Set_VoiceType(U8 mu8SoundType)
{
    gu8SoundType = mu8SoundType;
}


/// @brief      Voice�� ����� ���� ���� Ȯ�� �Լ�
/// @details    ���� IC�� ����Ϸ��� ����� ���� ������ Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Voice�� ���� - 0(Mute), 1(Melody), 2(Voice)
U8 Get_VoiceType(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8SoundType;

    return  mu8Return;
}


/// @brief      Voice�� ����� ������ �����ϴ� �Լ�
/// @details    ���� IC�� ����Ϸ��� ����� ������ ũ�⸦ �����Ѵ�
/// @param      mu8Volume : �����Ϸ��� Voice�� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
/// @return     void
void Set_VoiceVolume(U8 mu8Volume)
{
    gu8VoiceVolumeLevel = mu8Volume;
}


/// @brief      Voice�� ����� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� IC�� ����Ϸ��� ����� ������ ���� ũ�⸦ Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Voice�� ũ�� - 0(1�ܰ�) ~ 4(5�ܰ�)
U8 Get_VoiceVolume(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8VoiceVolumeLevel;

    return  mu8Return;
}


/// @brief      ������ ���¿� ���� ���� ����� ������ ������� üũ�ϴ� �Լ�
/// @details    ������ ���¿� ���� ���� ����� ������ ������� Ȯ���Ͽ� ��� ���θ� �����Ѵ�
/// @param      mu16Address : ����ϰ��� �ϴ� ���� Address
/// @return     return : ���� ��� ���� - 0(���� �����), 1(���� ���)
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
/// @details    ���� IC�� Address�� �����Ͽ� ����� �� �ֵ��� �Ѵ�
/// @param      mu16MemoryAddress : ����Ϸ��� ���� Address
/// @return     void
void Set_PlayVoice(U16 mu16MemoryAddress)
{
    gu16VoiceAddress = mu16MemoryAddress;

    gu8VoiceSoundOut = Check_VoiceOutput(gu16VoiceAddress);
}


/// @brief      Voice Play Initialize
/// @details    ���� IC�� �ʱ�ȭ ��Ų��
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
/// @details    ���� IC�� ���� ����� �����Ѵ�
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
/// @details    ���� IC�� Stop Command�� �۽��Ѵ�
/// @param      void
/// @return     void
void VoiceStopNextLab(void)
{
    gu16VoiceCommand = VOICE_COMMAND_STOP;
    SEND_SPI_COMMAND_NEXT_LAB();
}


/// @brief      Voice Volume Command
/// @details    ���� IC�� ���� ���� Command�� �۽��Ѵ�
/// @param      void
/// @return     void
void SendVolumeNextLab(void)
{
    U8 mu8VoiceOutCritical  = 0;

    if ( (gu16VoiceAddress >= VOICE_192_MELODY_DISPENSE_CONTINUOUS)  && (gu16VoiceAddress <= VOICE_199_MELODY_SET_OFF) )
    {   // ȿ���� ����� ���
        gu8BeepSoundOut = 1;
    }
    else
    {
        gu8BeepSoundOut = 0;
    }

    mu8VoiceOutCritical = IsCriticalVoice(gu16VoiceAddress);

    if (mu8VoiceOutCritical == TRUE)
    {   // Critical ���� ������ ������ �����ϰ� �ִ� ��������
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

    Delay_NextLab(400);   // ��25us
    SEND_SPI_COMMAND_NEXT_LAB();
    Delay_NextLab(400);   // ��25us
}


/// @brief      Voice Data Command
/// @details    ���� IC�� ����ϴ� ���� Address Data�� �۽��Ѵ�
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
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + gu16VoiceAddress; //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x01 if address is 256~511
      else if (gu16VoiceAddress < 512)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_512; //phrase group = 1, high address = 1
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 256); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x02 if address is 512~767
      else if (gu16VoiceAddress < 768)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_768; //phrase group = 2, high address = 010
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 512); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x03 if address is 768~1023
      else if (gu16VoiceAddress < 1024)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1024; //phrase group = 3, high address = 011
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 768); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x04 if address is 1024~1279
      else if (gu16VoiceAddress < 1280)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1280; //phrase group = 4, high address = 100
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1024); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x05 if address is 1280~1535
      else if (gu16VoiceAddress < 1536)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1536; //phrase group = 5, high address = 101
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1280); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x06 if address is 1536~1791
      else if (gu16VoiceAddress < 1792)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_1792; //phrase group = 6, high address = 110
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1536); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      //set $E4 register = 0x07 if address is 1792~2047
      else if (gu16VoiceAddress < 2048)
      {                                       //set phrase group & higher 3 bit address on $E4 register
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_2048; //phrase group = 7, high address = 111
          SEND_SPI_COMMAND_NEXT_LAB();
          //set lower 8 bit address on $E0 register
          Delay_NextLab(400);   // ��25us
          gu16VoiceCommand = VOICE_COMMAND_ADDRESS_0 + (gu16VoiceAddress - 1792); //set lower 8 bit address on $E0 register
          SEND_SPI_COMMAND_NEXT_LAB();
      }
      else
      {
         /*..��� ����..*/
      }
}


/// @brief      SPI Communication with NEXTLAB Voice IC
/// @details    ���� IC�� SPI ����� �����Ѵ�
/// @param      void
/// @return     void
void SEND_SPI_COMMAND_NEXT_LAB(void)
{
   U8 mu8i = 0;

   P_VOICE_CSB = 0;                 //start SPI
   Delay_NextLab(24000);            // CSB Low �� �� 1.4ms
   P_VOICE_CLK = 0;              //stand-by status,

   /*16bit ó��*/
   while (mu8i < 16)
   {
      P_VOICE_CLK = 0;
      Delay_NextLab(160);          // ��10us

      if(gu16VoiceCommand & MK_COMMAND)
      {
         P_VOICE_DATA = 1;
      }
      else
      {
         P_VOICE_DATA = 0;
      }

      gu16VoiceCommand <<= 1; //shift left
      Delay_NextLab(160);          // ��10us

      P_VOICE_CLK = 1;

      mu8i++;

       if(mu8i == 8)
       {
           Delay_NextLab(640);      // ��40us (1Byte-1byte ��Ŷ����)
       }
       else
       {
           Delay_NextLab(320);      // ��20us
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
/// @details    ���� IC ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Voice_Module_Initialize(void)
{
#if (VOICE_IC_USE == USE)
    Voice_Initialize();                     // Voice IC �ʱ�ȭ
#endif
}


/// @brief      Hal Voice Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� IC ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_Voice_Module_1ms_Control(void)
{
#if (VOICE_IC_USE == USE)
    Voice_ControlTimer();                   // Voice IC Control Timer
#endif
}


/// @brief      Hal Voice Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� IC�� �����Ѵ�
/// @param      void
/// @return     void
void Hal_Voice_Module_Control(void)
{
#if (VOICE_IC_USE == USE)
    Voice_Control();                        // Voice ���� �Լ�
#endif
}
