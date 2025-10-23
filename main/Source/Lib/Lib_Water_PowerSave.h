/// @file     Lib_Water_PowerSave.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    ���� ���� ���� file

#ifndef _LIB_WATER_POWER_SAVE_H_
#define _LIB_WATER_POWER_SAVE_H_


/*  Lib Function ����
    => Set_NoUsePowerSaveFuncSet(mu8OnOff);             // �̻�� ���� ��� ���� ���� ���� - 0(OFF), 1(ON)
    => Get_NoUsePowerSaveFuncSet();                     // �̻�� ���� ��� ���� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_PowerSaveFuncSet(mu8OnOff);                  // �Ϲ� ���� ��� ���� ���� ���� - 0(OFF), 1(ON)
    => Get_PowerSaveFuncSet();                          // �Ϲ� ���� ��� ���� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_PowerSaveMode(mu8Mode);                      // ���� ���� ���� ���� ��� ���� - 0(���� �̵���), 1(�Ϲ� ����), 2(�̻�� ����)
    => Get_PowerSaveMode();                             // ���� ���� ���� ���� ��� Ȯ�� - 0(���� �̵���), 1(�Ϲ� ����), 2(�̻�� ����)

    => Set_BrightnessData(mu8Data);                     // ���� ��� ���� ���� - 0(����), 1(��ο�)
    => Get_BrightnessData();                            // ���� ��� ���� Ȯ�� - 0(����), 1(��ο�)
*/


#define SMART_NO_USE_POWER_SAVE_USE                 USE     // ����Ʈ �̻�� ���� ��� ��� ����
#define POWER_SAVE_USE                              USE     // ���� ��� ��� ����

#define PHOTO_SENSOR_USE                            NO_USE     // ���� ���� ��� ���� : 0(NO_USE), 1(USE)


#if ( (SMART_NO_USE_POWER_SAVE_USE == USE) || (POWER_SAVE_USE == USE) )

#define POWER_SAVE_CONTROL_TIME_PERIOD          1000    // �¼� ��ũ ���� �ð� �ֱ� : 1Sec(@1ms)

#if (PHOTO_SENSOR_USE == USE)
#define BRIGHT_DETECT_TIME                      600     // ��� ��ȭ ���� Ȯ�� �ð� 10min(@1sec)

// ���� ���� �׽�Ʈ ���(5mm ������)
// 1.5 Lux - 354  440  400
// 3.5 Lux - 693  770  730
#define SLEEP_IN_LUX_DETECT_AD                  360     // ���� ��� ���� ���� ��� 1.5 Lux AD��
#define SLEEP_OUT_LUX_DETECT_AD                 700     // ���� ��� ���� ���� ��� 3.5 Lux AD��

#define BRIGHTNESS_DETECT_BRIGHT                0       // ���� ����
#define BRIGHTNESS_DETECT_DARK                  1       // ��ο� ����
#endif


/// @brief      ������� ���� ����Ʈ
/// @details    ������� ������ ����Ʈ�� �����Ѵ�
typedef enum
{
    POWERSAVE_ID_NONE = 0,                  // 0 : ���� �̵���
    POWERSAVE_ID_NORMAL,                    // 1 : �Ϲ� ���� ��� ����
    POWERSAVE_ID_SMART,                     // 2 : ����Ʈ �̻�� ���� ��� ����
    POWERSAVE_ID_MAX                        // Max
} EPowerSaveID_T;


// ����Ʈ �̻�� ���� ����
#define NO_USE_TIME_OUT                         9       // 9�ð�, �̻�� ���� ��� ���� ���� üũ �ð�

#define	SMART_POWER_SAVE_TIME_PERIOD            168     // 7days = 168hour, 1���� �ֱ� �ð�
#define SMART_POWER_SAVE_TIME_NO_USE            16      // 16hr, ��ǰ �̻�� üũ �ð�
#define SMART_POWER_SAVE_TIME_NO_USE_LONG       84      // 84hr, ��ǰ ��� �̻�� üũ �ð�

#define SMART_POWER_SAVE_TYPE_NONE              0
#define SMART_POWER_SAVE_TYPE_GREEN_WEEK        1
#define SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK    2

void Lib_PowerSave_Initialize(void);
void PowerSaveControlTimer(void);
void Set_NoUsePowerSaveFuncSet(U8 mu8OnOff);
U8 Get_NoUsePowerSaveFuncSet(void);
void Set_PowerSaveFuncSet(U8 mu8OnOff);
U8 Get_PowerSaveFuncSet(void);
void Set_PowerSaveMode(U8 mu8Mode);
U8 Get_PowerSaveMode(void);

#if (PHOTO_SENSOR_USE == USE)
void Set_BrightnessData(U8 mu8Data);
U8 Get_BrightnessData(void);
#endif

void Lib_PowerSaveControl(void);

#if (PHOTO_SENSOR_USE == USE)
void BrightnessCheckControl(void);
#endif

void PowerSaveControl(void);
void SmartNoUsePowerSaveControl(void);
void NoUseTimeClear(void);
void NoUseTimeCounter(void);
void Set_SmartNoUsePowerSaveButtonInput(U8 mu8Data);
void DecisionSmartPowerSaveType(void);
void SmartNoUseTimeCounter(void);
void SmartNoUseCheckClear(void);
void PowerSaveActEnd(void);

#endif      // ����Ʈ �̻�� ���� ��� �Ǵ� ���� ��� END

void Lib_PowerSave_Module_Initialize(void);
void Lib_PowerSave_Module_1ms_Control(void);
void Lib_PowerSave_Module_Control(void);

#endif

