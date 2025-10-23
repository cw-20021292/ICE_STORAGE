/// @file     Lib_Water_Heater.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    Heat Tank ���� ���� file

#include "Global_Header.h"


#if (NORMAL_HEATER_USE == USE)                     // ���͸� ����ϴ� ���


/// @brief      ���� ���� �µ� ���� ������ ����
/// @details    ���� ��� �ϱ� ���� ���� �µ� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    F32 gf32TempHeatOff;                    /// @brief  ���� OFF �µ�(������, �Ҽ���)
    F32 gf32TempHeatOn;                     /// @brief  ���� ON �µ�(������, �Ҽ���)
}   SHeatTemp_T;


/// @brief      ����Ʈ �̻�� ���� ���۽� ���� ���� �µ� ���� ���̺�
/// @details    ����Ʈ �̻�� ���� ���۽� �� ��庰 ���� On, Off ���� �µ����� �����Ѵ�
__far const SHeatTemp_T  SSmartPowerSaveHeatTempData[HEIGHT_MODE_COUNT] = {
//  ���� OFF,  ���� ON
    {87.0f,     60.0f},     // ����� 1
    {82.0f,     60.0f},     // ����� 2
    {79.0f,     60.0f},     // ����� 3
    {76.0f,     60.0f},     // ����� 4
};


/// @brief      ���� ���۽� ���� ���� �µ� ���� ���̺�
/// @details    ���� ���۽� �� ��庰 ���� On, Off ���� �µ����� �����Ѵ�
__far const SHeatTemp_T  SPowerSaveHeatTempData[HEIGHT_MODE_COUNT] = {
//  ���� OFF,  ���� ON
    {93.0f,     70.0f},     // ����� 1
    {88.0f,     70.0f},     // ����� 2
    {85.0f,     70.0f},     // ����� 3
    {82.0f,     70.0f},     // ����� 4
};


/// @brief      ���� ���� �µ� ���� ���̺�
/// @details    �� ��庰 ���� On, Off ���� �µ����� �����Ѵ�
__far const SHeatTemp_T  SHeatTempData[HEIGHT_MODE_COUNT] = {
//  ���� OFF,   ���� ON
    {93.0f,     83.0f},     // ����� 1
    {88.0f,     80.0f},     // ����� 2
    {85.0f,     77.0f},     // ����� 3
    {82.0f,     74.0f},     // ����� 4
};


/// @brief      ���� ���� ������ ����
/// @details    ���� ��� �ϱ� ���� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U8 gu8HeightMode;                       /// @brief  �� ��� �ܰ�(0~3)
    U8 gu8FuncStatus;                       /// @brief  �¼� ��� ���� ����
    F32 gf32TargetHighTemp;                 /// @brief  ���� ��ǥ ���� �µ�(������, �Ҽ���)
    F32 gf32TargetLowTemp;                  /// @brief  ���� ��ǥ ���� �µ�(������, �Ҽ���)
    U8 gu8LowLevelDelayTime;                /// @brief  �������� ���� ���� ���� �ð�
    U8 gu8OnDelayTime;                      /// @brief  ���� On �� ���� Ÿ�̸�
    U8 gu8OffDelayTime;                     /// @brief  ���� Off �� ���� Ÿ�̸�
}   SHotData_T;

SHotData_T  SHotData;

U8 gu8HeatTankControlTimer = 0;             /// @brief  �¼� ��ũ ���� �ֱ� Ÿ�̸�


/// @brief      Heat Tank ���� ���� Flag & ���� �ʱ�ȭ
/// @details    Heat Tank ��� ���Ǵ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_HeatTank_Initialize(void)
{
    MEMSET( (void __FAR *)&SHotData, 0x00, sizeof(SHotData_T) );

    gu8HeatTankControlTimer = 0;                    // �¼� ��ũ ���� �ֱ� Ÿ�̸�
    SHotData.gu8HeightMode = HEIGHT_MODE_1;         // �� ��� 1 default
    SHotData.gu8FuncStatus = HOT_ON;                // �¼� ��� ���� ����
    SHotData.gf32TargetHighTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;    // ���� ��ǥ ���� �µ�
    SHotData.gf32TargetLowTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;      // ���� ��ǥ ���� �µ�
    SHotData.gu8LowLevelDelayTime = 0;              // �������� ���� ���� ���� �ð�
    SHotData.gu8OnDelayTime = 0;                    // ���� On �� ���� Ÿ�̸�
    SHotData.gu8OffDelayTime = 0;                   // ���� Off �� ���� Ÿ�̸�
}


/// @brief      Heat Tank Control Timer(@1ms)
/// @details    �¼� ��ũ ���� ��� ���� ���� �ֱ� Ÿ�̸� ī��Ʈ�� �Ѵ�
/// @param      void
/// @return     void
void HeatTankControlTimer(void)
{
    if (gu8HeatTankControlTimer < HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8HeatTankControlTimer++;
    }
}


/// @brief      �¼� ��� ON/OFF ���� ���� �Լ�
/// @details    �¼� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8Status : �¼� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_HotFuncStatus(U8 mu8Status)
{
    SHotData.gu8FuncStatus = mu8Status;
}


/// @brief      �¼� ��� ON/OFF ���� Ȯ�� �Լ�
/// @details    �¼� ��� ���°� ON���� OFF ���� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �¼� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_HotFuncStatus(void)
{
    U8 mu8Return = 0;

    mu8Return = SHotData.gu8FuncStatus;

    return  mu8Return;
}


/// @brief      �¼� ���� ���� ������ Delay �ð� ���� �Լ�
/// @details    �¼� ���� ���� ���� ���� �����ð� Delay�� �ִ� ��� �� �ð��� �����Ѵ�
/// @param      mu8Time : �¼� ���� ���� ���� �� Delay �ð�(@100ms)
/// @return     void
void Set_HotOnDelay(U8 mu8Time)
{
    SHotData.gu8OnDelayTime = mu8Time;
}


/// @brief      ������ ���� ���� �¼� ���� ���� ������ Delay �ð� ���� �Լ�
/// @details    ������ ���� ���� �ٽ� ������ ������ �¼� ���� ���� ���� ���� �����ð� Delay�� �ִ� ��� �� �ð��� �����Ѵ�
/// @param      mu8Time : �¼� ���� ���� ���� �� Delay �ð�(@100ms)
/// @return     void
void Set_LowWaterHeatDelay(U8 mu8Time)
{
    SHotData.gu8LowLevelDelayTime = mu8Time;
}


/// @brief      �� ��� ���� �Լ�
/// @details    �� ��带 �����Ѵ�
/// @param      mu8Step : �� ��� �ܰ� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
/// @return     void
void Set_HeightMode(U8 mu8Step)
{
    SHotData.gu8HeightMode = mu8Step;
}


/// @brief      �� ��� ���� ���� Ȯ�� �Լ�
/// @details    �� ��� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�.
/// @param      void
/// @return     return : �� ��� �ܰ� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
U8 Get_HeightMode(void)
{
    U8 mu8Return = 0;

    mu8Return = SHotData.gu8HeightMode;

    return  mu8Return;
}


/// @brief      ���� �� ���� �Լ�(@While)
/// @details    ���� ��� �����Ѵ�
/// @param      void
/// @return     void
void Lib_HeatTank_Control(void)
{
    U8 mu8AllKeyLock = 0;           // ��ü ��ư ��� ����
    U8 mu8HotWaterLevel = 0;        // �¼� ��ũ ���� ����
    U8 mu8HotError = 0;             // �¼��� ���õ� ���� �߻� ����
    F32 mf32HotTemp = 0;            // �¼� �µ� ���� �µ�
    U8 mu8PowerSaveStatus = 0;      // ���� ���� ���� : 0(�̵���), 1(�Ϲ� ����), 2(�̻�� ����)
    U8 mu8FirstHighLevel = 0;       // ���� �� ���� ������ ���� ����

    if (gu8HeatTankControlTimer >= HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8HeatTankControlTimer = 0;

        mu8AllKeyLock = Get_AllKeyLockStatus();     // ��ü ��ư ��� ����
        mu8HotWaterLevel = Get_HotWaterLevel();     // �¼� ��ũ�� ���� Ȯ��
        mu8HotError = Get_HotError();               // �¼��� ���õ� ���� �߻� ���� Ȯ��
//        mf32HotTemp = Get_Temp(ADC_ID_TH_HOT);      // �¼� �µ� Ȯ��
        mu8PowerSaveStatus = Get_PowerSaveMode();   // ���� ���� ���� ��� ���� Ȯ��
        mu8FirstHighLevel = Get_FirstHighLevelCheck();  // ���� �� ���� ������ ���� ���� Ȯ��

        if (SHotData.gu8OnDelayTime)
        {   // ���Ϳ� �ð��� ���ÿ� ������ ���� ������ ���� ON ��Ȳ�� ���� ���� ���� Delay
            SHotData.gu8OnDelayTime--;
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // ���� Delay ���� ��ũ ���� OFF
            return;
        }

        if (mu8AllKeyLock == ON)
        {   // ��ü ��� ����
            SHotData.gu8LowLevelDelayTime = 0;      // �������� ���� ���� ���� �ð�
            SHotData.gu8OnDelayTime = 0;            // ���� On �� ���� Ÿ�̸�
            SHotData.gu8OffDelayTime = 0;           // ���� Off �� ���� Ÿ�̸�
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // ���� Delay ���� ��ũ ���� OFF
            return;
        }

        if (SHotData.gu8FuncStatus == HOT_OFF)
        {   // �¼� ����� ���� ����
            SHotData.gu8LowLevelDelayTime = 0;      // �������� ���� ���� ���� �ð�
            SHotData.gu8OnDelayTime = 0;            // ���� On �� ���� Ÿ�̸�
            SHotData.gu8OffDelayTime = 0;           // ���� Off �� ���� Ÿ�̸�
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // ���� Delay ���� ��ũ ���� OFF
            return;
        }


        if (mu8HotWaterLevel == HOT_WATER_LEVEL_LOW)
        {   // �¼� ��ũ�� �������� ���
            Set_LowWaterHeatDelay(HEAT_ON_DELAY_TIME_LOW_LEVEL);        // ���� ���� 10��(100 = 10 X 10(100ms))
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // ���� Delay ���� ��ũ ���� OFF
        }
        else
        {   // �¼� ��ũ�� �������� �ƴ� ���
            if (SHotData.gu8LowLevelDelayTime)
            {   // ������ ���� �߻��� �־ ���� ���� �����̰� �ִ� ���
                SHotData.gu8LowLevelDelayTime--;
                Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // ���� Delay ���� ��ũ ���� OFF
            }
            else
            {
                if (mu8FirstHighLevel == SET)
                {   // ���� ������ ���� ���� ����
                    if (mu8HotError == FALSE)
                    {   // ���� ���� �Ұ� ���� ���� �̹߻���
                        if (mu8PowerSaveStatus == POWERSAVE_ID_SMART)
                        {
                            SHotData.gf32TargetHighTemp = SSmartPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SSmartPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        else if (mu8PowerSaveStatus == POWERSAVE_ID_NORMAL)
                        {   // ���� ��� ������ ���
                            SHotData.gf32TargetHighTemp = SPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        else
                        {   // �Ϲ� ���� ���
                            SHotData.gf32TargetHighTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        // ���� ����
                        if (mf32HotTemp > SHotData.gf32TargetHighTemp)
                        {   // ��ǥ �µ����� ���� ��� Heater OFF
                            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);         // ���� Delay ���� ��ũ ���� OFF
                        }
                        else if (mf32HotTemp <= SHotData.gf32TargetLowTemp)
                        {   // ��ǥ �µ����� ���� ��� Heater ON
                            Set_Heater_Control(HEATER_ID_HOT_TANK, ON, 0);          // ���� Delay ���� ��ũ ���� ON
                        }
                        else
                        {   // ���� OFF �µ��� ���� ON �µ� ������ ��쿡�� ���� ���� ����
                        }
                        // ���� ���� ����
                        if (mf32HotTemp > 100.0f)
                        {   // �¼� �µ��� 100���� ������
                            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);         // ���� Delay ���� ��ũ ���� OFF
                        }
                    }
                    else
                    {  // ���� ���� �Ұ� ���� ���� �߻���
                        Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // ���� Delay ���� ��ũ ���� OFF
                    }
                }
                else
                {   // ���� ������ ������ ���� ���� ���
                    Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);                 // ���� Delay ���� ��ũ ���� OFF
                }
            }
        }
    }
}


#endif      // �Ϲ��� ���� ��� ���� ���� END



// Lib HeatTank Module ********************************************************************************************

/// @brief      Lib Heater Module Initilaize
/// @details    �Ϲ� ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_Heater_Module_Initialize(void)
{
#if (NORMAL_HEATER_USE == USE)
    Lib_HeatTank_Initialize();
#endif
}


/// @brief      Lib Heater Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Heater ���� ��� Library
/// @param      void
/// @return     void
void Lib_Heater_Module_1ms_Control(void)
{
#if (NORMAL_HEATER_USE == USE)
    HeaterControlTimer();
#endif
}


/// @brief      Lib Heater Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Heater ���� ��� Library
/// @param      void
/// @return     void
void Lib_Heater_Module_Control(void)
{
#if (NORMAL_HEATER_USE == USE)
    Lib_HeatTank_Control();
#endif
}

