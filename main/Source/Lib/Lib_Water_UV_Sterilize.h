/// @file     Lib_Water_UV_Sterilize.h
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    ��ǰ ���� ���� ���� file


#ifndef _LIB_WATER_UV_STERILIZE_H_
#define _LIB_WATER_UV_STERILIZE_H_


/*  Lib Function ����
    => UV ��� ���� ���� �ֱ�� ���� �ð��� �������ָ� SW ���� ����� �˾Ƽ� ������
*/



#if (UV_COUNT > 0)

/// @brief      ��ǰ UV ��� ���� ���°�
/// @details    ��ǰ UV ��� ���� ���°��� ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    STERILIZE_OPERATION_ID_STAND_BY = 0,                // 0 :
    STERILIZE_OPERATION_ID_UV_WATER_TANK,               // 1 : ���� ��ũ UV ���
    STERILIZE_OPERATION_ID_UV_COLD_TANK_BACK,           // 2 : �ü� ��ũ(�޸�) UV ���
    STERILIZE_OPERATION_ID_UV_ICE_TANK,                 // 3 : ���� ��ũ UV ���
    STERILIZE_OPERATION_ID_UV_ICE_TRAY,                 // 4 : ���� Ʈ���� UV ���
    STERILIZE_OPERATION_ID_UV_WATER_FAUCET,             // 5 : �� ����� UV ���
    STERILIZE_OPERATION_ID_UV_ICE_FAUCET,               // 6 : ���� ����� UV ���
    STERILIZE_OPERATION_ID_UV_COLD_TANK_FRONT,          // 7 : �ü� ��ũ(�ո�) UV ���
    STERILIZE_OPERATION_ID_MAX                          // 8 Max, 8�� ������ �ȵ�
} EProductUVStatus_T;

#define UV_STERILIZE_OPERATION_COUNT                    7       // UV ��� ���� ����

#define UV_STERILIZE_CONTROL_TIME_PERIOD                100     // UV ��� ���� ���� �ֱ� Ÿ�̸� 100ms@1ms
#if (UV_STERILIZE_OPERATION_COUNT >= 1)
#define UV_1_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_1_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 2)
#define UV_2_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_2_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 3)
#define UV_3_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_3_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 4)
#define UV_4_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_4_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 5)
#define UV_5_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_5_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 6)
#define UV_6_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_6_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif
#if (UV_STERILIZE_OPERATION_COUNT >= 7)
#define UV_7_STERILIZE_START_TIME_PERIOD                360     // UV ��� ���� ���� �ֱ� 6hour = 360min@1min
#define UV_7_STERILIZE_ON_TIME                          20      // UV ��� ���� �ð� 20min@1min
#endif


void Lib_UV_Sterilize_Initialize(void);
void UV_SterilizeControlTimer(void);
void Set_UV_SterilizeStatus(U8 mu8ID, U8 mu8Status);
U8 Get_UV_SterilizeStatus(U8 mu8ID);
void UV_SterilizeStartControl(void);
void UV_SterilizeOnControl(void);
void Lib_UV_SterilizeControl(void);

void Lib_UV_Sterilize_Module_Initialize(void);
void Lib_UV_Sterilize_Module_1ms_Control(void);
void Lib_UV_Sterilize_Module_Control(void);

#endif

#endif

