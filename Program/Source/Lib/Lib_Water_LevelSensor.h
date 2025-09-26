/// @file     Lib_Water_LevelSensor.h
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    ���� üũ ���� ���� File


#ifndef _LIB_WATER_LEVEL_SENSOR_H_
#define _LIB_WATER_LEVEL_SENSOR_H_


/*  Lib Function ����
    => Get_ColdWaterLevel();                            // �ü� ��ũ�� ���� ���� Ȯ�� - 0(������), 1(�߼���), 2(������)
    => Get_HotWaterLevel();                             // �¼� ��ũ�� ���� ���� Ȯ�� - 0(������), 1(�߼���), 2(������)
    => Get_WaterLevel();                                // ���� ��ũ�� ���� ���� Ȯ��
                                                           - �߼��� �ִ� ��� 0(������), 1(��������), 2(�߰����), 3(������)
                                                           - �߼��� ���� ��� 0(������), 1(�߼���), 2(������)
    => Get_FirstHighLevelCheck();                       // ���� ������ ���� ���� Ȯ�� - 0(�̰���), 1(����)
    => Get_FirstLowLevelCheck();                        // ���� ������ ���� ���� Ȯ�� - 0(�̰���), 1(����)
*/


#if (LEVEL_SENSOR_COUNT > 0)

// ����
#define COLD_LOW_SENSOR_USE             USE           // �ü� ������ ���� ���
#define MIDDLE_SENSOR_USE               NO_USE        // �߼��� ���� ���
#define HOT_HIGH_SENSOR_USE             USE           // �¼� ������ ���� ���

#define COLD_WATER_LEVEL_LOW            0x00
#define COLD_WATER_LEVEL_MIDDLE         0x01
#define COLD_WATER_LEVEL_HIGH           0x02

#if (MIDDLE_SENSOR_USE == USE)
#define WATER_LEVEL_LOW                 0x00
#define WATER_LEVEL_MIDDLE_LOW          0x01
#define WATER_LEVEL_MIDDLE_HIGH         0x02
#define WATER_LEVEL_HIGH                0x03
#else                                   // �߼��� ���� �̻���
#define WATER_LEVEL_LOW                 0x00
#define WATER_LEVEL_MIDDLE              0x01
#define WATER_LEVEL_HIGH                0x02
#endif

#define HOT_WATER_LEVEL_LOW             0x00
#define HOT_WATER_LEVEL_MIDDLE          0x01
#define HOT_WATER_LEVEL_HIGH            0x02


void Lib_WaterLevel_Initialize(void);
void Lib_WaterLevelCheck(void);

U8 Get_ColdWaterLevel(void);
U8 Get_HotWaterLevel(void);
U8 Get_WaterLevel(void);
U8 Get_FirstHighLevelCheck(void);
U8 Get_FirstLowLevelCheck(void);

#endif      // ���� ������ �ִ� ��� END


void Lib_WaterLevel_Module_Initialize(void);
void Lib_WaterLevel_Module_Control(void);

#endif

