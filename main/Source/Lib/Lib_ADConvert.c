/// @file     Lib_ADConvert.c
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    AD Convert function control file

#include "Global_Header.h"


#if (ADC_USE == USE)

U8 gu8AD_Convert_Timer = 0;                     /// @brief  AD Convert ���� �ֱ� Ÿ�̸�
U8 gu8AD_ChannelCount = 0;                      /// @brief  ADC Channal ���� ī����
U8 gu8AD_CheckEnd = 0;                          /// @brief  ADC Check �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)


/// @brief      AD Convert Initialize
/// @details    ADC �������� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void AD_Convert_Initialize(void)
{
    gu8AD_Convert_Timer = 0;
    gu8AD_ChannelCount = 0;
    gu8AD_CheckEnd = 0;
}


/// @brief      AD Convert control Timer(@1ms interrupt)
/// @details    ADC�� �����ϴ� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void ADC_ControlTimer(void)
{
    if (gu8AD_Convert_Timer < AD_CONVERT_TIME_PERIOD)
    {
        gu8AD_Convert_Timer++;
    }
}


/// @brief      AD Convert �Ϸ� ���� ���� �Լ�
/// @details    ADC �Ϸ� ���¸� �����Ѵ�
/// @param      mu8Value : ADC �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_ADC_EndStatus(U8 mu8Value)
{
    gu8AD_CheckEnd = mu8Value;
}


/// @brief      AD Convert ���� ���� Ȯ�� �Լ�
/// @details    ADC �Ϸᰡ �Ǿ����� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ADC �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_ADC_EndStatus(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8AD_CheckEnd;

    return  mu8Return;
}


/// @brief      AD Convert ���� ���� �Լ�
/// @details    �ش� ä���� ADC�� ���� ���� ��Ų��
/// @param      mu8AD_Channel : AD Channel No
/// @return     void
void ADC_CheckStart(U8 mu8AD_Channel)
{
    ADS = mu8AD_Channel;

    ADC_Start();

    Set_ADC_EndStatus(CLEAR);
}


/// @brief      AD Convert �� Ȯ�� �Լ�
/// @details    ���� ä���� ADC���� �о ��ȯ�Ѵ�
/// @param      mu8Resolution : AD Register Shift�� Bit ��
/// @return     return : AD ���� �����
U16 Read_ADC_Operation(U8 mu8Resolution)
{
    U16 mu16AD_Result = 0;

    mu16AD_Result = (U16)(ADCR >> mu8Resolution);

    return  mu16AD_Result;
}


/// @brief      AD Convert �� Ȯ�� �� ��� ��� �Լ�
/// @details    ���� ä���� ADC���� �о ����� ����Ѵ�
/// @param      mu8Channel : AD ������ Channel
/// @return     void
void Read_ADC_Channel(U8 mu8Channel)
{
    U16 mu16AD_result = 0;

#if (AD_CH_CONV_BIT == AD_CONV_8BIT)
    mu16AD_result = Read_ADC_Operation(AD_CH_CONV_8BIT);
#elif (AD_CH_CONV_BIT == AD_CONV_12BIT)
    mu16AD_result = Read_ADC_Operation(AD_CH_CONV_12BIT);
#else
    mu16AD_result = Read_ADC_Operation(AD_CH_CONV_10BIT);
#endif

    AD_AVERAGE_FUNCTION(mu8Channel, mu16AD_result);
}


/// @brief      AD Convert ä�� ���� �� ADC ���� ���� �Լ�
/// @details    ADC�� ä���� �����Ͽ� ���۽�Ų��
/// @param      void
/// @return     void
void ADC_ChannelChangeStart(void)
{
    gu8AD_ChannelCount++;

    if (gu8AD_ChannelCount >= AD_CH_MAX)
    {
        gu8AD_ChannelCount = 0;
    }

    ADC_CheckStart(gu8AD_ChannelCount);
}


/// @brief      AD Convert ���� �Լ�(@1ms)
/// @details    ADC�� Convert ������ �����Ѵ�
/// @param      void
/// @return     void
void AD_Convert_Control(void)
{
    if (gu8AD_Convert_Timer >= AD_CONVERT_TIME_PERIOD)
    {
        gu8AD_Convert_Timer = 0;

        if (gu8AD_CheckEnd == SET)

        {
            switch (gu8AD_ChannelCount)
            {
                case AD_CH_0:
#if (AD_CH_0_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_1_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_1:
#if (AD_CH_1_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_2_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_2:
#if (AD_CH_2_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_3_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_3:
#if (AD_CH_3_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_4_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_4:
#if (AD_CH_4_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_5_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_5:
#if (AD_CH_5_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_6_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_6:
#if (AD_CH_6_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_7_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_7:
#if (AD_CH_7_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_8_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_8:
#if (AD_CH_8_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_9_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_9:
#if (AD_CH_9_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_10_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_10:
#if (AD_CH_10_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_11_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_11:
#if (AD_CH_11_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_12_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_12:
#if (AD_CH_12_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_13_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_13:
#if (AD_CH_13_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_14_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_14:
#if (AD_CH_14_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_15_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_15:
#if (AD_CH_15_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_16_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_16:
#if (AD_CH_16_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_17_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_17:
#if (AD_CH_17_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_18_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_18:
#if (AD_CH_18_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_19_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_19:
#if (AD_CH_19_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_20_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_20:
#if (AD_CH_20_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_21_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_21:
#if (AD_CH_21_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_22_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_22:
#if (AD_CH_22_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_23_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_23:
#if (AD_CH_23_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_24_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_24:
#if (AD_CH_24_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_25_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                case AD_CH_25:
#if (AD_CH_25_USE == USE)
                    Read_ADC_Channel(gu8AD_ChannelCount);
#endif

#if (AD_CH_0_USE == USE)
                    ADC_ChannelChangeStart();
#else
                    gu8AD_ChannelCount++;
                    Set_ADC_EndStatus(SET);
#endif
                    break;

                default:
                    gu8AD_ChannelCount = 0;
                    Set_ADC_EndStatus(CLEAR);
                    break;
            }
        }
        else
        {
        }
   }
}

#endif



// Lib ADC Module ********************************************************************************************

/// @brief      Lib ADC Module Initilaize
/// @details    ADC ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_ADC_Module_Initialize(void)
{
#if (ADC_USE == USE)
    AD_Convert_Initialize();                // AD Convert ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Lib ADC Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� ADC ���� ��� Library
/// @param      void
/// @return     void
void Lib_ADC_Module_1ms_Control(void)
{
#if (ADC_USE == USE)
    ADC_ControlTimer();                     // AD Convert �ð� Counter
#endif
}


/// @brief      Lib ADC Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� ADC ���� ��� Library
/// @param      void
/// @return     void
void Lib_ADC_Module_Control(void)
{
#if (ADC_USE == USE)
    AD_Convert_Control();                   // AD Convert Control �Լ�
#endif
}




