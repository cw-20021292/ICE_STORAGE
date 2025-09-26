/// @file     Temp_Table.h
/// @date     2025/05/21
/// @author   Jaejin Ham
/// @brief    �µ� ���� Table


#ifndef _TEMP_TABLE_H_
#define _TEMP_TABLE_H_


/*  ���� Function ����
    => ����ϴ� �µ� ������ USE�� ����, �̻�� �µ� ������ NO_USE�� ����
    => Get_Temp(mu8ID);                         // ID�� �ش��ϴ� �µ� ������ �µ���(Float��) Ȯ�� - ��) 20�� = 20.0f
*/


// �ü� - ���ǻ���: �� �µ� ���� ���� ��� ���� �Ұ�
#define CONFIG_TEMP_TABLE_COLD_JAMESTECH        USE     // �ü��µ����� / ���ӽ���
#define CONFIG_TEMP_TABLE_COLD_RATTRON          USE     // �ü��µ����� / ��Ʈ��

// ����
#define CONFIG_TEMP_TABLE_ROOM_JAMESTECH        USE     // �����µ����� / ���ӽ���

// �¼� ��ũ
#define CONFIG_TEMP_TABLE_HOT_TANK_JAMESTECH    NO_USE     // �¼��µ����� / ���ӽ��� / ��ũ��

// �����¼�
#define CONFIG_TEMP_TABLE_HOT_IN_SK             USE     // �¼��µ����� / SK / �����¼� �Լ�(T1)
#define CONFIG_TEMP_TABLE_HOT_BODY_SK           USE     // �¼��µ����� / SK / �����¼� ���� Body(= �����¼� ���(T2))
#define CONFIG_TEMP_TABLE_HOT_OUT_SK            USE     // �¼��µ����� / SK / �����¼� ���(T2)

// ������� ���ͽ�ũ
#define CONFIG_TEMP_TABLE_HEATSINK_JAMESTECH    USE     // ��Ʈ��ũ / ���ӽ��� / ������� ��Ʈ��ũ

// �ܱ�
#define CONFIG_TEMP_TABLE_AIR_JAMESTECH         USE     // �ܱ�µ����� / ���ӽ���

// Evaporator
#define CONFIG_TEMP_TABLE_EVA                   NO_USE     // ���߱�µ�����

// FAUCET
#define CONFIG_TEMP_TABLE_FAUCET_OUT_SK         NO_USE     // �Ŀ�� ����� �µ� ����(��Ʈ�� �� ����) (= �����¼� �Լ�)

// TDS
#define CONFIG_TEMP_TABLE_TDS_IN_SK             NO_USE     // TDS ���� / SK
#define CONFIG_TEMP_TABLE_TDS_OUT_SK            NO_USE     // TDS ���� / SK


// ����ϴ� �µ� �������� ADC ID���� ����
#define COOL_THERMISTOR_1_ID                    ADC_ID_TH_COOL
#define ROOM_THERMISTOR_ID                      0
#define HOT_THERMISTOR_ID                       0
#define HOT_IN_THERMISTOR_ID                    ADC_ID_TH_HOT_IN
#define HOT_BODY_THERMISTOR_ID                  ADC_ID_TH_HOT_BODY
#define HOT_OUT_THERMISTOR_ID                   ADC_ID_TH_HOT_OUT
#define TEM_HEATSINK_THERMISTOR_ID              ADC_ID_TH_HEATSINK
#define AIR_THERMISTOR_ID                       ADC_ID_TH_AIR
#define FAUCET_OUT_THERMISTOR_ID                0
#define TDS_IN_THERMISTOR_ID                    0
#define TDS_OUT_THERMISTOR_ID                   0
#define EVA_THERMISTOR_ID                       0


F32 ConvInt2Temp(I16 mi16temp);
U8 ConvTemp2Char(F32 mf32temp);

#if ( (CONFIG_TEMP_TABLE_COLD_JAMESTECH == USE) || (CONFIG_TEMP_TABLE_COLD_RATTRON == USE) )
F32 ConvAdc2Temp_ColdWater(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_ROOM_JAMESTECH == USE)
F32 ConvAdc2Temp_Room(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_TANK_JAMESTECH == USE)
F32 ConvAdc2Temp_HotTank(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_IN_SK == USE)
F32 ConvAdc2Temp_HotIn(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_BODY_SK == USE)
F32 ConvAdc2Temp_HotBody(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_OUT_SK == USE)
F32 ConvAdc2Temp_HotOut(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HEATSINK_JAMESTECH == USE)
F32 ConvAdc2Temp_Heatsink(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_AIR_JAMESTECH == USE)
F32 ConvAdc2Temp_Air(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_FAUCET_OUT_SK == USE)
F32 ConvAdc2Temp_FaucetOut(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_EVA == USE)
F32 ConvAdc2Temp_Eva(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_TDS_IN_SK == USE)
F32 ConvAdc2Temp_TdsIn(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_TDS_OUT_SK == USE)
F32 ConvAdc2Temp_TdsOut(U16 mu16Adc);
#endif

F32 Get_Temp(U8 mu8ID);

#endif
