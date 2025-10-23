/**
 * File : temp.h
 * 
 * Temp Sensor
*/
#ifndef __TEMP_H__
#define __TEMP_H__

#include "prj_type.h"

typedef enum _temp_id
{
    TEMP_ID_AMBIENT = 0,      // 0 :
    TEMP_ID_AMBIENT_2,        // 0 :
    TEMP_ID_ROOM_WATER,       // 0 :
    TEMP_ID_COLD_WATER,       // 0 :
    TEMP_ID_EVA_1,            // 0 :
    TEMP_ID_EVA_2,            // 0 :
    TEMP_ID_HOT_WATER,        // 0 :
    TEMP_ID_MAX               // 0 :
} ETempId_T;

#define DEFAULT_INIT_TEMP_READ_TIME    (600U)  /// 10min @1sec
#define DEFAULT_TEMP_READ_TIME         (10U)  /// 10sec @1sec

TEMP_T GetTemp(U8 id);

// 센서 읽기 동작 시간
// 센서 읽기 자연 시간 반환
U16 GetTempSensorTime(U8 id);

// mu8Id : TEMP_ID_XXXX
// mu16TimeOut : @1sec..
void TurnOnTempSensor(U8 id, U16 timeOut);
void TurnOffTempSensor(U8 id);

// Override
void SetOverrideTemp(U8 id, U8 override, TEMP_T temp);
TEMP_T GetOverrideTemp(U8 id, U8 override);

void ProcessTemp(void);

#endif /* __TEMP_H__ */
