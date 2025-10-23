/**
 * File : temp_table.h
 * 
 * Temp Sensor Table
*/
#ifndef __TEMP_TABLE_H__
#define __TEMP_TABLE_H__

#include "prj_type.h"

#define CONFIG_TEMP_TABLE_COLD_JAMESTECH        (1)     // �ü��µ����� / ���ӽ���
#define CONFIG_TEMP_TABLE_COLD_RATTRON          (0)     // �ü��µ����� / ��Ʈ��

#define CONFIG_TEMP_TABLE_ROOM                  (1)     // �����µ����� / ���ӽ��� (�ü��µ�����)

#define CONFIG_TEMP_TABLE_HOT_JAMESTECH_TANK    (1)     // �¼��µ����� / ���ӽ��� / ��ũ��
#define CONFIG_TEMP_TABLE_HOT_BODY_SK           (0)     // �¼��µ����� / SK / �����¼� ���� �ٵ�
#define CONFIG_TEMP_TABLE_HOT_IN_SK             (1)     // �¼��µ����� / SK / �����¼� �Լ� ( T1 )
#define CONFIG_TEMP_TABLE_HOT_OUT_SK            (0)     // �¼��µ����� / SK / �����¼� ��� ( T2 )

#define CONFIG_TEMP_TABLE_HEATSINK              (0)     // ��Ʈ��ũ / ���ӽ��� / ������� ��Ʈ��ũ

#define CONFIG_TEMP_TABLE_AMBI                  (1)     // �ܱ�µ����� / ���ӽ���

#define CONFIG_TEMP_TABLE_EVA                   (1)     // ���߱�µ����� / ?

#define CONFIG_TEMP_TABLE_ICE_TRAY_IN           (0)     // ���̽�Ʈ�����Լ��µ����� / ?

#define CONFIG_TEMP_TABLE_TDS                   (0)     // TDS ���� / ?

#if( CONFIG_TEMP_TABLE_COLD_JAMESTECH == 1 )
TEMP_T ConvAdc2Temp_ColdWater_Jamestech(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_COLD_RATTRON  == 1 )
TEMP_T ConvAdc2Temp_ColdWater_Rattron(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_ROOM  == 1 )
TEMP_T ConvAdc2Temp_RoomWater(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_JAMESTECH_TANK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Jamestech_Tank(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_BODY_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Sk_Body(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_IN_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_In_Sk(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_OUT_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Out_Sk(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HEATSINK  == 1 )
TEMP_T ConvAdc2Temp_Heatsink_Jamestech(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_AMBI  == 1 )
TEMP_T ConvAdc2Temp_Ambient_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_EVA  == 1 )
TEMP_T ConvAdc2Temp_Eva_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_ICE_TRAY_IN  == 1 )
TEMP_T ConvAdc2Temp_IceTray_In_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_TDS  == 1 )
TEMP_T ConvAdc2Temp_Tds_In_(U16 adc);
TEMP_T ConvAdc2Temp_Tds_Out_(U16 adc);
#endif

#endif
