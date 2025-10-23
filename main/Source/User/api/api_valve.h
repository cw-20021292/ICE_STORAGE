/**
 * File : api_valve.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_VALVE_H__
#define __API_VALVE_H__

#include "prj_type.h"

typedef enum _valve_id_
{
    VALVE_ID_NOS = 0,             // 0 : Critical ���� �� ���� �Լ��� ���� ���� ���
    VALVE_ID_FEED,                // 1 : ���� �Լ� ���
    VALVE_ID_COLD_IN,             // 2 : �ü� �Լ� ���
    VALVE_ID_ROOM_OUT,            // 3 : ���� ��� ( ���� ) - ��� ���� ��Ʈ�� ���־�� ��
    VALVE_ID_HOT_OUT,             // 4 : ���� ��� ( �¼� ) - ��� ���� ��Ʈ�� ���־�� ��
    VALVE_ID_COLD_CIRCULATE,      // 5 : ���� ��� ( �ü� ) - ��� ���� ��Ʈ�� ���־�� ��
    VALVE_ID_ICE_WATER_OUT,       // 6 : ���� ��� ( ������ ) - ��� ���� ��Ʈ�� ���־�� �� 
    VALVE_ID_ICE_TRAY_IN,         // 7 : ���̽� Ʈ���� �Լ� ���
    VALVE_ID_DRAIN,               // 8 : �ü� �巹�� ���
    VAVLE_ID_NOS_FLOW,            // 9 : ���̽� Ʈ���� �Լ� �� ���� ���� ������ ���
    VALVE_ID_EXTRA,               // 10 : ���� ���
    VALVE_ID_MAX,                 // 11 : MAX
} EValveId_T;

void API_OpenValve(EValveId_T id);
void API_CloseValve(EValveId_T id);

#endif  /* __API_VALVE_H__ */
