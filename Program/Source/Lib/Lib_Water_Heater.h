/// @file     Lib_Water_Heater.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    Heat Tank ���� ���� file

#ifndef _LIB_WATER_HEATER_H_
#define _LIB_WATER_HEATER_H_


/*  Lib Function ����
    => SSmartPowerSaveHeatTempData[HEIGHT_MODE_COUNT], SPowerSaveHeatTempData[HEIGHT_MODE_COUNT], SHeatTempData[HEIGHT_MODE_COUNT]
       ���̺��� �������� ���� SW ���� ��⿡�� �˾Ƽ� ������

    => Set_HotFuncStatus(mu8Status);                    // �¼� ��� ���� ���� ���� - 0(OFF), 1(ON)
    => Get_HotFuncStatus();                             // �¼� ��� ���� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_HotOnDelay(mu8Time);                         // �¼� ���� ���� ���� �� Delay �ð� ���� - 100ms ����

    => Set_HeightMode(mu8Step);                         // �� ��� �ܰ� ���� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
    => Get_HeightMode();                                // �� ��� �ܰ� Ȯ�� - 0(1�ܰ�), 1(2�ܰ�), 2(3�ܰ�), 3(4�ܰ�)
*/



#define HEIGHT_MODE_COUNT                           4       // �� ����� ��� ����

#define HEIGHT_MODE_1                               0       // default
#define HEIGHT_MODE_2                               1
#define HEIGHT_MODE_3                               2
#define HEIGHT_MODE_4                               3

#if (NORMAL_HEATER_USE == USE)                  // �Ϲ� ���͸� ����ϴ� ���

#define HEAT_TANK_CONTROL_TIME_PERIOD               100     // �¼� ��ũ ���� �ð� �ֱ� : 100ms(@1ms)

#define HEAT_OFF_DELAY_TIME                         0       // ���� OFF �� Delay �ð� 0�� (@100ms)
#define HEAT_ON_DELAY_TIME_LOW_LEVEL                100     // ������ ���� �̰��� �� �簨���� ���� ON �� Delay �ð� 10�� (@100ms)

void Lib_HeatTank_Initialize(void);
void HeatTankControlTimer(void);
void Set_HotFuncStatus(U8 mu8Status);
U8 Get_HotFuncStatus(void);
void Set_HotOnDelay(U8 mu8Time);
void Set_LowWaterHeatDelay(U8 mu8Time);
void Set_HeightMode(U8 mu8Step);
U8 Get_HeightMode(void);
void Lib_HeatTank_Control(void);


#endif      // �Ϲ��� ���� ��� ���� ���� END

void Lib_Heater_Module_Initialize(void);
void Lib_Heater_Module_1ms_Control(void);
void Lib_Heater_Module_Control(void);

#endif

