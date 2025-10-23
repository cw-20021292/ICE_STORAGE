/// @file     Lib_Water_PowerSave.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    ���� ���� ���� file

#include "Global_Header.h"


#if ( (SMART_NO_USE_POWER_SAVE_USE == USE) || (POWER_SAVE_USE == USE) )


#if (PHOTO_SENSOR_USE == USE)
/// @brief      ���� ���� ���� ���� ������
/// @details    ���� ���� ������ ���õ� ��� �ϱ� ���� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U8 gu8BeforeStatus;                     /// @brief  ������ ���� ������ ���� : 0(����), 1(��ο�)
    U8 gu8Status;                           /// @brief  ������ ���� ������ ���� : 0(����), 1(��ο�)
    U16 gu16DarkTimer;                      /// @brief  ��ο� ���� �ð� Ÿ�̸�(@1Sec)
    U16 gu16BrightTimer;                    /// @brief  ���� ���� �ð� Ÿ�̸�(@1Sec)
}   SBrightDetectData_T;
#endif

/// @brief      ���� ���� ������
/// @details    ���� ��� �ϱ� ���� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U8 gu8NoUseFuncStatus;                  /// @brief  �̻�� ���� ��� ���� ���� : 0(OFF), 1(ON)
    U8 gu8FuncStatus;                       /// @brief  ���� ��� ���� ���� : 0(OFF), 1(ON)
    U8 gu8Mode;                             /// @brief  ���� ���� ��� : 0(���� �̵���), 1(�Ϲ� ����), 2(�̻�� ����)
    U8 gu8NoUseTimerSecCount;               /// @brief  ��ǰ �̻�� �ð�(1�� ����)
    U8 gu8NoUseTimerMinCount;               /// @brief  ��ǰ �̻�� �ð�(1�� ����)
    U8 gu8NoUseTimerHourCount;              /// @brief  ��ǰ �̻�� �ð�(1�ð� ����)
    U16 gu16SmartNoUseCheck1HrTimer;        /// @brief  ����Ʈ �̻�� ���� Type �Ǵ� ���� ���� 1�ð� ���� üũ Timer
    U8 gu8ButtonInputCheck;                 /// @brief  ����Ʈ �̻�� ���� üũ �ð� ���� ��ư �Է� ���� Flag
    U8 gu8SmartPowerSaveCheckHour;          /// @brief  ����Ʈ �̻�� ���� �̻�� üũ �ð� Timer(16�ð�)
    U8 gu8SmartPowerSaveLongCheckHour;      /// @brief  ����Ʈ �̻�� ���� ��� �̻�� üũ �ð� Timer(84�ð�)
    U8 gu8SmartPowerSaveControlType;        /// @brief  ����Ʈ �̻������ ���� ���� Ÿ��(1 - GreenWeek, 2 - IntensiveWeek)
    U8 gu8SmartPowerSavePeriodHour;         /// @brief  ����Ʈ �̻������ ���� ���� �ֱ�(7�� = 168�ð�)
    U8 gu8SmartPowerSaveCheckCount;         /// @brief  ����Ʈ �̻������ �̻�� üũ ī����
}   SPowerSaveData_T;

#if (PHOTO_SENSOR_USE == USE)
SBrightDetectData_T     SBrightData;
#endif

SPowerSaveData_T    SPowerSaveData;

U16 gu16PowerSaveControlTimer = 0;            /// @brief  ���� ���� �ֱ� Ÿ�̸�


/// @brief      ���� ���� ���� Flag & ���� �ʱ�ȭ
/// @details    ���� ���۰� ���õ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_PowerSave_Initialize(void)
{
    gu16PowerSaveControlTimer = 0;

#if (PHOTO_SENSOR_USE == USE)
    SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT;     // ���� ���� ���� ����
    SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;           // ���� ���� ���� ����
    SBrightData.gu16DarkTimer = 0;                  // ��ο� ���� �ð� Ÿ�̸�
    SBrightData.gu16BrightTimer = 0;                // ���� ���� �ð� Ÿ�̸�
#endif

    SPowerSaveData.gu8FuncStatus = ON;              // ���� ��� ���� ���� ON
    SPowerSaveData.gu8NoUseFuncStatus = ON;         // �̻�� ���� ���� ���� ON
    SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // ���� ��� �̵��� ����
    SPowerSaveData.gu8NoUseTimerSecCount = 0;       // �̻�� �ð� ī��Ʈ Sec
    SPowerSaveData.gu8NoUseTimerMinCount = 0;       // �̻�� �ð� ī��Ʈ Min
    SPowerSaveData.gu8NoUseTimerHourCount = 0;      // �̻�� �ð� ī��Ʈ Hour
    SPowerSaveData.gu8ButtonInputCheck = CLEAR;     // ����Ʈ �̻�� ���� üũ �Ⱓ ���� ��ư �Է� ���� Flag Clear
    SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;  // ����Ʈ �̻�� ���� 1�ð� üũ Timer
    SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;           // ����Ʈ �̻������ �̻�� üũ �ð� Timer(@hour)
    SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;  // ����Ʈ �̻������ ��� �̻�� üũ �ð� Timer(@hour)
    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // ����Ʈ �̻������ ���� ���� Ÿ��
    SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;          // ����Ʈ �̻������ ���� ���� �ֱ�
    SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;                                     // ����Ʈ �̻������ �̻�� üũ ī����
}


/// @brief      ���� ���� ���� Control Timer(@1ms)
/// @details    ���� ���� ��� ���� ���� �ֱ� Ÿ�̸� ī��Ʈ�� �Ѵ�
/// @param      void
/// @return     void
void PowerSaveControlTimer(void)
{
    if (gu16PowerSaveControlTimer < POWER_SAVE_CONTROL_TIME_PERIOD)
    {   // 1�� ����
        gu16PowerSaveControlTimer++;
    }
}


/// @brief      �̻�� ���� ����� ��� ���� ���� �Լ�
/// @details    �̻�� ���� ����� ����� ������ ������� ���� ������ �����Ѵ�
/// @param      mu8OnOff : ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_NoUsePowerSaveFuncSet(U8 mu8OnOff)
{
    SPowerSaveData.gu8NoUseFuncStatus = mu8OnOff;
}


/// @brief      �̻�� ���� ����� ��� ���� ���� ���� Ȯ�� �Լ�
/// @details    �̻�� ���� ����� ����� ������ ������� ���� ������ �����Ǿ� �ִ� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_NoUsePowerSaveFuncSet(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8NoUseFuncStatus;

    return  mu8Return;
}


/// @brief      ���� ����� ��� ���� ���� �Լ�
/// @details    ���� ����� ����� ������ ������� ���� ������ �����Ѵ�
/// @param      mu8OnOff : ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_PowerSaveFuncSet(U8 mu8OnOff)
{
    SPowerSaveData.gu8FuncStatus = mu8OnOff;
}


/// @brief      ���� ����� ��� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� ����� ����� ������ ������� ���� ������ �����Ǿ� �ִ� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_PowerSaveFuncSet(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8FuncStatus;

    return  mu8Return;
}


/// @brief      ���� ���� ���� ���� ��� ���� �Լ�
/// @details    ���� ������ �������� ���� ��尡 � ������� �����Ѵ�
/// @param      mu8Mode : ���� ���� ���� ���� ��� ���� - 0(���� �̵���), 1(�Ϲ� ����), 2(�̻�� ����)
/// @return     void
void Set_PowerSaveMode(U8 mu8Mode)
{
    SPowerSaveData.gu8Mode = mu8Mode;
}


/// @brief      ���� ���� ���� ���� ��� Ȯ�� �Լ�
/// @details    ���� ������ �������� ���� ��尡 � ������� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� ���� ��� ���� - 0(���� �̵���), 1(�Ϲ� ����), 2(�̻�� ����)
U8 Get_PowerSaveMode(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8Mode;

    return  mu8Return;
}


#if (PHOTO_SENSOR_USE == USE)
/// @brief      ���� ���� ���� ���� ��� ���� �Լ�
/// @details    ���� ������ �������� ���� ��尡 � ������� �����Ѵ�
/// @param      mu8Data : ���� ��� ���� - 0(����), 1(��ο�)
/// @return     void
void Set_BrightnessData(U8 mu8Data)
{
    SBrightData.gu8Status = mu8Data;
}


/// @brief      ���� ������ ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ������ ���� ���� Ȯ���� ��� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ��� ���� - 0(����), 1(��ο�)
U8 Get_BrightnessData(void)
{
    U8 mu8Return = 0;

    mu8Return = SBrightData.gu8Status;

    return  mu8Return;
}
#endif


/// @brief      ���� ��� ���� �Լ�(@1Sec)
/// @details    ���� ��� ������ ���� �����ϴ� �Ѱ� ���� ó�� �Լ��̴�
/// @param      void
/// @return     void
void Lib_PowerSaveControl(void)
{
    if (gu16PowerSaveControlTimer >= POWER_SAVE_CONTROL_TIME_PERIOD)
    {   // 1�� ����
        gu16PowerSaveControlTimer = 0;

#if (PHOTO_SENSOR_USE == USE)
        BrightnessCheckControl();       // ���� ��� ���� ����
#endif
        PowerSaveControl();             // ���� ���� ���� �Ǵ� ����
        SmartNoUsePowerSaveControl();   // �̻�� ���� ���� ���� �Ǵ� ����
    }
}


#if (PHOTO_SENSOR_USE == USE)
/// @brief      ���� ���� üũ ���� �Լ�(1Sec)
/// @details    ���� ������ �̿��Ͽ� ���� ���� �Ǵ� ��ο��� �����ϴ� ������ �Ѵ�
/// @param      void
/// @return     void
void BrightnessCheckControl(void)
{
    U8 mu8AD_Brightness = 0;

    mu8AD_Brightness = Get_ADC_Data(ADC_ID_PHOTO_SENSOR);   // ���� ���� AD Ȯ��

    // ���� ����
    if (mu8AD_Brightness < SLEEP_IN_LUX_DETECT_AD)
    {   // ��ο��� ���
        SBrightData.gu16BrightTimer = 0;        // ���� ���� Ÿ�̸Ӵ� �ʱ�ȭ

        if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
        {   // ���� ���� ���¿��� ���
            if (SBrightData.gu16DarkTimer >= BRIGHT_DETECT_TIME)
            {   // ��ο� ������ 10�� ������
                SBrightData.gu16DarkTimer = 0;
                SBrightData.gu8Status = BRIGHTNESS_DETECT_DARK;
            }
            else
            {
                SBrightData.gu16DarkTimer++;
            }
        }
    }
    else if (mu8AD_Brightness > SLEEP_OUT_LUX_DETECT_AD)
    {   // ����� ���
        SBrightData.gu16DarkTimer = 0;          // ��ο� ���� Ÿ�̸Ӵ� �ʱ�ȭ

        if ((SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK))
        {   // ��ο� ���� ���¿��� ���
            if (SBrightData.gu16BrightTimer >= BRIGHT_DETECT_TIME)
            {   // ���� ������ 10�� ������
                SBrightData.gu16BrightTimer = 0;
                SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
            }
            else
            {
                SBrightData.gu16BrightTimer++;
            }
        }
    }
    else
    {   // ��ο�� ���� ���� ���� �� ������ ��� ���� ���°��� ���� ������ ���� ����
        SBrightData.gu16DarkTimer = 0;
        SBrightData.gu16BrightTimer = 0;
    }
}
#endif


/// @brief      �Ϲ� ���� ���� �Լ�(1Sec)
/// @details    �Ϲ� ���� ���۰� �����Ͽ� ���� ���� ���� ���θ� �����Ѵ�
/// @param      void
/// @return     void
void PowerSaveControl(void)
{
#if (PHOTO_SENSOR_USE == USE)       // ���� ���� ��� ���� ���
    if (SPowerSaveData.gu8FuncStatus == ON)
    {   // ���� ����� ����ϴ� ���
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_NONE)
        {   // ���� ��尡 ���� ������ ���� ���
            if (SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK)
            {   // ��ο��� ������ ������ ���
                SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_DARK;   // ���� �������� ��ο����� ����
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NORMAL;
            }
        }
        else
        {   // �̻�� �����̳� ���� ��尡 ���� ���� ���
            if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
            {   // ������ ������ ������ ���
                SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT; // ���� �������� �������� ����
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // �̻�� ����, ���� ���� ����
                NoUseTimeClear();       // ��ǰ �̻�� �ð� �ʱ�ȭ
            }
        }
    }
    else
    {   // ���� ����� ������� �ʴ� ���
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_NORMAL)
        {   // ���� ���� ���̾�����
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // ���� ���� ����
        }

        if (SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK)
        {   // ��ο��� ������ ������ ���
            SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_DARK;   // ���� �������� ��ο����� ����
        }
        else if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
        {   // ������ ������ ������ ���
            if (SBrightData.gu8BeforeStatus == BRIGHTNESS_DETECT_DARK)
            {   // ������ ��ο��� �����Ǿ��� ���
                SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // �̻�� ����, ���� ���� ����
                NoUseTimeClear();       // ��ǰ �̻�� �ð� �ʱ�ȭ
            }
        }
        else
        {
        }
    }
#else   // ���� ���� �̻�� ���� ���

#endif
}


/// @brief      �̻�� ���� ��� ���� �Լ�(1Sec)
/// @details    �̻�� ���� ���۰� �����Ͽ� ���� ���� ���� ���θ� �����Ѵ�
/// @param      void
/// @return     void
void SmartNoUsePowerSaveControl(void)
{
    U8 mu8AirTempError = 0;

    NoUseTimeCounter();                     // ��ǰ �̻�� �ð� üũ
    DecisionSmartPowerSaveType();           // �̻�� ���� ���� Type �Ǵ�

    mu8AirTempError = Get_AirTempError();   // �ܱ� �µ� ���� ���� �߻� Ȯ��

    if (mu8AirTempError == TRUE)
    {   // �ܱ� �µ� ���� ������ �߻��� ���
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
        {   // ����Ʈ �̻�� ���� ���� ���� ���
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // �Ϲ� ���� ��� ���� ���� �ٽ� üũ�� �� �ֵ��� ���� �̵��� ���·� ��ȯ
        }

        NoUseTimeClear();
    }
    else
    {   // �ܱ� �µ� ���� ������ �߻����� ���� ���
        if (SPowerSaveData.gu8NoUseFuncStatus == ON)
        {   // �̻�� ���� ��� ON��
            if (SPowerSaveData.gu8Mode != POWERSAVE_ID_SMART)
            {   // �̻�� ���� ��� ���� ���� �ƴ� ���
                if (SPowerSaveData.gu8NoUseTimerHourCount >= NO_USE_TIME_OUT)
                {   // �̻�� 9�ð� ��� ��
                    SPowerSaveData.gu8Mode = POWERSAVE_ID_SMART;
                    NoUseTimeClear();
                }
            }
            else
            {   // �̻�� ���� ��� ���� ���� ���
                NoUseTimeClear();
            }
        }
        else
        {   // �̻�� ���� ��� OFF��
            if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
            {   // ����Ʈ �̻�� ���� ���� ���� ���
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // �Ϲ� ���� ��� ���� ���� �ٽ� üũ�� �� �ֵ��� ���� �̵��� ���·� ��ȯ
            }

            NoUseTimeClear();
            SmartNoUseCheckClear();
        }
    }
}


/// @brief      �̻�� �ð� ī���� �ʱ�ȭ �Լ�
/// @details    ��ǰ �̻�� �ð��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void NoUseTimeClear(void)
{
    SPowerSaveData.gu8NoUseTimerSecCount = 0;
    SPowerSaveData.gu8NoUseTimerMinCount = 0;
    SPowerSaveData.gu8NoUseTimerHourCount = 0;
}


/// @brief      �̻�� �ð� ī���� �Լ�(1Sec)
/// @details    ��ǰ �̻�� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void NoUseTimeCounter(void)
{
    if (SPowerSaveData.gu8SmartPowerSaveControlType == SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK)
    {   // ����Ʈ �̻��  ���� �ְ����� �̻�� ���� ��� ���� ����
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
        {   // ����Ʈ �̻�� ���� ���� ���� ���
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // �Ϲ� ���� ��� ���� ���� �ٽ� üũ�� �� �ֵ��� ���� �̵��� ���·� ��ȯ
        }

        NoUseTimeClear();
    }
    else
    {   // ����Ʈ �̻�� ���� �ְ����� �̻�� ���� ��� ���� ���� �Ǵ��ϵ��� �̻�� �ð� üũ
        if (SPowerSaveData.gu8NoUseTimerHourCount < NO_USE_TIME_OUT)
        {   // 9�ð� �̸��̸�
            if (++SPowerSaveData.gu8NoUseTimerSecCount >= 60)
            {   // 1�� ���޽� ����
                SPowerSaveData.gu8NoUseTimerSecCount = 0;       // 1�� ī��Ʈ ������ �� ī��Ʈ Clear

                if (++SPowerSaveData.gu8NoUseTimerMinCount >= 60)
                {   // 1�ð� ���޽� ����
                    SPowerSaveData.gu8NoUseTimerMinCount = 0;   // 1�ð� ī��Ʈ ������ �� ī��Ʈ Clear
                    SPowerSaveData.gu8NoUseTimerHourCount++;
                }
            }
        }
        else
        {   // 9�ð� �̻��̸�
            SPowerSaveData.gu8NoUseTimerSecCount = 0;   // �� ī��Ʈ Clear
            SPowerSaveData.gu8NoUseTimerMinCount = 0;   // �� ī��Ʈ Clear
        }
    }
}


/// @brief      �̻�� ���� ���۰� �����Ͽ� �̻�� üũ �� ��ư �Է� ���� ���� �Լ�
/// @details    �̻�� ���� Type ������ ���� ���� ���� �� ��ư�� �Է� ���� �̷��� ������ ���´�
/// @param      mu8Data : ��ư �Է� ���� �̷� - 0(���Է�), 1(�Է�)
/// @return     void
void Set_SmartNoUsePowerSaveButtonInput(U8 mu8Data)
{
    if (mu8Data)
    {
        SPowerSaveData.gu8ButtonInputCheck = SET;
    }
    else
    {
        SPowerSaveData.gu8ButtonInputCheck = CLEAR;
    }
}


/// @brief      ����Ʈ �̻�� ���� ���� Type �Ǵ� �Լ�
/// @details    ����Ʈ �̻�� ���� ���� ���� ���� Type�� �Ǵ��ϱ� ���� üũ�� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void DecisionSmartPowerSaveType(void)
{
    if (++SPowerSaveData.gu16SmartNoUseCheck1HrTimer >= 3600)
    {   // 1�ð�(3600��@1sec) ����� ���� ����
        SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;     // ����Ʈ �̻�� ���� 1�ð� üũ Timer

        SmartNoUseTimeCounter();

        if (SPowerSaveData.gu8SmartPowerSavePeriodHour == 0)
        {   // ����Ʈ �̻�� ���� ���� üũ �ֱ�(������)�� ����� ��
            if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour == 0)
            {   // ��� �̻�� �ð��� ����� ���·� ������ ���
                SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // Green Week�� ����
            }
            else
            {
                if(SPowerSaveData.gu8SmartPowerSaveCheckCount <= 2)
                {   // �ֱ� �� �̻�� ī��Ʈ�� 2ȸ ����
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK;     // Intensive Week�� ����
                }
                else if(SPowerSaveData.gu8SmartPowerSaveCheckCount >= 4)
                {   // �ֱ� �� �̻�� ī��Ʈ�� 4ȸ �̻�
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;         // Green Week�� ����
                }
                else
                {   // �ֱ� �� �̻�� ī��Ʈ�� 3ȸ�� ���
                    //  ���� ���� type ����
                }
            }

            if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
            {   // ����Ʈ �̻�� ���� ���� ���� ���
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // ������ ����� �̻�� ������ �Ϲ� ���� ��� ���� ���� �ٽ� üũ�� �� �ֵ��� ���� �̵��� ���·� ��ȯ
            }

            NoUseTimeClear();               // �̻�� ���� ���� üũ �ð� �ʱ�ȭ

            SPowerSaveData.gu8ButtonInputCheck = CLEAR;         // ����Ʈ �̻�� ���� üũ �Ⱓ ���� ��ư �Է� ���� Flag
            SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;                   // 16�ð� �ʱ�ȭ
            SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;          // 84�ð� �ʱ�ȭ
            SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;                  // 168�ð� �ʱ�ȭ
            SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;
        }
        else
        {   // ����Ʈ �̻�� ���� ���� üũ �ֱ� 168�ð�(7��) �̳� ���� ��
            if (SPowerSaveData.gu8ButtonInputCheck == SET)
            {   // ��ư �Է��� �߻��� ���
                if (SPowerSaveData.gu8SmartPowerSaveCheckHour == 0)
                {   // ��ư �Է��� �־��� �� ����Ʈ �̻�� üũ �ð�(16�ð�)�� �̹� ����� ���
                    if (SPowerSaveData.gu8SmartPowerSaveCheckCount < 0xFF)
                    {
                        SPowerSaveData.gu8SmartPowerSaveCheckCount++;     // �̻�� ī��Ʈ ����
                    }
                }

                SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;               // 16�ð� �ʱ�ȭ
                SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;      // 84�ð� �ʱ�ȭ
                SPowerSaveData.gu8ButtonInputCheck = CLEAR;
            }
            else
            {   // ��ư �Է��� �̹߻��� ���
                if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour == 0)
                {   // ����Ʈ �̻�� ���� ��� �̻�� üũ �ð�(84�ð�)�� ����� ���
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // ��� Green Week�� ��ȯ
                }
            }
        }
    }
}


/// @brief      ����Ʈ �̻�� ���� ���� Type �Ǵ��� ���� üũ �ð� ī��Ʈ �Լ�
/// @details    ����Ʈ �̻�� ���� ���� Type�� �Ǵ��ϴ� ������ �ð� �� 16�ð� ī��Ʈ�� ī��Ʈ�ٿ� �Ѵ�
/// @param      void
/// @return     void
void SmartNoUseTimeCounter(void)
{
    if (SPowerSaveData.gu8SmartPowerSavePeriodHour)
    {   // ����Ʈ �̻�� ���� ���� üũ �ֱ�(168Hr) ī��Ʈ �ٿ�
        SPowerSaveData.gu8SmartPowerSavePeriodHour--;
    }

    if (SPowerSaveData.gu8SmartPowerSaveCheckHour)
    {   // ����Ʈ �̻�� ���� �̻�� üũ �ð�(16Hr) ī��Ʈ �ٿ�
        SPowerSaveData.gu8SmartPowerSaveCheckHour--;
    }

    if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour)
    {   // ����Ʈ �̻�� ���� ��� �̻�� üũ �ð�(84Hr) ī��Ʈ �ٿ�
        SPowerSaveData.gu8SmartPowerSaveLongCheckHour--;
    }
}


/// @brief      ����Ʈ �̻�� ���� ���� üũ ���� ������ ��� �ʱ�ȭ
/// @details    ����Ʈ �̻�� ���� ���۰� ���õ� üũ �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void SmartNoUseCheckClear(void)
{
    SPowerSaveData.gu8ButtonInputCheck = CLEAR;     // ����Ʈ �̻�� ���� üũ �Ⱓ ���� ��ư �Է� ���� Flag Clear
    SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;  // ����Ʈ �̻�� ���� 1�ð� üũ Timer
    SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;               // 16�ð� �ʱ�ȭ
    SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;      // 84�ð� �ʱ�ȭ
    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;         // ����Ʈ �̻������ ���� ���� Ÿ�� - Green Week
    SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;              // 168�ð� �ʱ�ȭ
    SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;                                 // ����Ʈ �̻������ �̻�� üũ ī����
}


/// @brief      ����, �̻�� ���� ���� ���� �Լ�
/// @details    ��ư �Է� ������ ���� �����̳� �̻�� ���� ������ �ߴ��� �� ����Ѵ�
/// @param      void
/// @return     void
void PowerSaveActEnd(void)
{
    if (SPowerSaveData.gu8Mode != POWERSAVE_ID_NONE)
    {   // ���� �Ǵ� �̻�� ���� �������� ���
        SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;

#if (PHOTO_SENSOR_USE == USE)
        // ���� ���� ���δ� �ٽ� üũ ������ �� �ֵ��� ���� ���� ���·� �ʱ�ȭ �� ������ ��� üũ ���� ������ ��� �ʱ�ȭ
        SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT;
        SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
        SBrightData.gu16BrightTimer = 0;
        SBrightData.gu16DarkTimer = 0;
#endif
    }
}



#endif      // ����Ʈ �̻�� ���� ��� �Ǵ� ���� ��� END

// Lib Power Save Module *********************************************************************************

/// @brief      Lib Power Save Module Initilaize
/// @details    �̻�� ����, ���� �Ǵ� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_PowerSave_Module_Initialize(void)
{
    Lib_PowerSave_Initialize();
}


/// @brief      Lib Power Save Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Power Save ���� ��� Library
/// @param      void
/// @return     void
void Lib_PowerSave_Module_1ms_Control(void)
{
    PowerSaveControlTimer();
}


/// @brief      Lib Power Save Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Power Save ���� ��� Library
/// @param      void
/// @return     void
void Lib_PowerSave_Module_Control(void)
{
    Lib_PowerSaveControl();
}

