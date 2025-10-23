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
    VALVE_ID_NOS = 0,             // 0 : Critical 에러 시 원수 입수를 막기 위한 밸브
    VALVE_ID_FEED,                // 1 : 원수 입수 밸브
    VALVE_ID_COLD_IN,             // 2 : 냉수 입수 밸브
    VALVE_ID_ROOM_OUT,            // 3 : 추출 밸브 ( 정수 ) - 밸브 전원 컨트롤 해주어야 함
    VALVE_ID_HOT_OUT,             // 4 : 추출 밸브 ( 온수 ) - 밸브 전원 컨트롤 해주어야 함
    VALVE_ID_COLD_CIRCULATE,      // 5 : 추출 밸브 ( 냉수 ) - 밸브 전원 컨트롤 해주어야 함
    VALVE_ID_ICE_WATER_OUT,       // 6 : 추출 밸브 ( 얼음물 ) - 밸브 전원 컨트롤 해주어야 함 
    VALVE_ID_ICE_TRAY_IN,         // 7 : 아이스 트레이 입수 밸브
    VALVE_ID_DRAIN,               // 8 : 냉수 드레인 밸브
    VAVLE_ID_NOS_FLOW,            // 9 : 아이스 트레이 입수 시 추출 유로 막음용 밸브
    VALVE_ID_EXTRA,               // 10 : 여분 밸브
    VALVE_ID_MAX,                 // 11 : MAX
} EValveId_T;

void API_OpenValve(EValveId_T id);
void API_CloseValve(EValveId_T id);

#endif  /* __API_VALVE_H__ */
