#ifndef __NFC_H__
#define __NFC_H__

#include "prj_type.h"

// User 
#define NFC_USER_A   0U
#define NFC_USER_B   1U
#define NFC_USER_C   2U


// Info
#define  NFC_START_CODE       0xAA
#define  NFC_MODEL_1          "CHPI\0"    // CHP-5800L 
#define  NFC_MODEL_2          "58XXL"    // CHP-5800L 
#define  NFC_GROUP_TYPE       0x51              // Water Purifier 
#define  NFC_END_CODE         0x55
typedef struct _information_
{
    U8 Start;
    U8 Group;
    char Model[10];
    U8 End;
} NFC_Info_T;


// Last Tag Time
typedef struct _last_tag_time_
{
    U8 Year;
    U8 Month;
    U8 Date;
    U8 Hour;
    U8 Min;
    U8 Sec;
} NFC_TagTime_T;


// Water GreenPlug Date
typedef struct _green_plug_
{
    U8 Week;
    U8 Day;
    U8 WeekFlag;
    U8 StartTime;    // GP TIME
} NFC_GreenPlug_T;


// Water Used Day
typedef struct _water_used_days_
{
    U8 UsedDay;
    U8 Over_60_Day;
} NFC_WaterUsedDay_T;


typedef struct _water_state_
{
    U8 WaterAmount;     // 마지막 추출한 물의 양
    U8 WaterType;       // 마지막 추출한 물의 종류
} NFC_WaterState_T;


typedef struct _ster_
{
    U8 PassedDay;   // 살균일로 부터 지난 기간(day)
} NFC_SterPassedDay_T;


#define  NFC_STER_CONF_MODE_MANUAL  0
#define  NFC_STER_CONF_MODE_AUTO    1

#define  NFC_STER_CONF_CYCLE_3DAY   3
#define  NFC_STER_CONF_CYCLE_5DAY   5
typedef struct _ster_config_
{
    U8 Mode;      // 모드 (0:수동, 1:자동)
    U8 Hour;      // 시간(hex) - 0~23(hex)
    U8 Min;       // 분(hex) - 0,30(hex)
    U8 Period;     // 살균 주기 - 3,5(dec)
} NFC_SterConf_T;


#define  NFC_WATER_CUP_HALF      0x12
#define  NFC_WATER_CUP_SINGLE    0x25
#define  NFC_WATER_CUP_DOUBLE    0x50
#define  NFC_WATER_CUP_CONTY     0xFE

#define  NFC_WATER_TYPE_ROOM     1
#define  NFC_WATER_TYPE_COLD     2
#define  NFC_WATER_TYPE_HOT      3
#define  NFC_WATER_TYPE_ION      4
#define  NFC_WATER_TYPE_CUP      5
#define  NFC_WATER_TYPE_MANUAL   6
#define  NFC_WATER_TYPE_SODA     7

#define  NFC_USER_ID_1           1
#define  NFC_USER_ID_2           2
#define  NFC_USER_ID_3           3
typedef struct _user_water_use_state_
{
    U8 CupType;          // 반잔(0x12), 한잔(0x25), 두잔(0x50), 연속(0xfe)
    U8 WaterType;    
    U8 User;             // A(1), B(2), C(3)
} NFC_UserUseState_T;


typedef struct _water_use_history_
{
    U8 Cold;      // 냉수 일일 물 사용량  ( 1-0.5잔, 4-2잔 ... )
    U8 Hot;       // 온수 일일 물 사용량
    U8 Room;      // 정수 일일 물 사용량
} NFC_WaterHis_T;


#define NFC_STER_CONF_TYPE_NONE        0x00
#define NFC_STER_CONF_TYPE_PAUCET      0x01  // 파우셋 살균
#define NFC_STER_CONF_TYPE_FLOW        0x10  // 유로 살균
#define NFC_STER_CONF_TYPE_CIRCULATE   0x40  // 순활 살균
typedef struct _ster_history_
{
    U8 Year;      // 년(hex)
    U8 Month;      // 월(hex)
    U8 Date;      // 일(hex)
    U8 Type;      // 0x01: 파우셋, 0x10: 유로, 0x40: 순환
} NFC_SterHis_T;


typedef struct _error_history_
{
    U8 Year;      // 년(hex)
    U8 Month;      // 월(hex)
    U8 Date;      // 일(hex)
    U8 Hour;      // 시(hex)
    U8 Error;     // error code
} NFC_ErrorHis_T;

typedef struct _error_state_
{
    U8 Error;
} NFC_ErrorState_T;

typedef struct _service_state_
{
    U8 CheckDay;
} NFC_ServiceState_T;

typedef struct _user_used_history_
{
    U8 Year;
    U8 Month;
    U8 Date;
    U8 Hour;
    U8 User;
    U8 WaterType;
    U8 WaterAmount;
} NFC_UserHis_T;


// Info
U8 NFC_InitInfo(void);
U8 NFC_WriteInfo(NFC_Info_T *pData);
U8 NFC_ReadInfor(NFC_Info_T *pData);

// Last Tag Time
U8 NFC_InitLastTagTime(void);
U8 NFC_WriteLastTagTime(NFC_TagTime_T *pData);
U8 NFC_ReadLastTagTime(NFC_TagTime_T *pData);

// Water GreenPlug Date
U8 NFC_InitGreenPlug(void);
U8 NFC_WriteGreenPlug(NFC_GreenPlug_T *pData);
U8 NFC_ReadGreenPlug(NFC_GreenPlug_T *pData); 

// Water Used Day
U8 NFC_InitWaterUsedDay(void);
U8 NFC_WriteWaterUsedDay(NFC_WaterUsedDay_T *pData);
U8 NFC_ReadWaterUsedDay(NFC_WaterUsedDay_T *pData);

// Water State
U8 NFC_InitWaterState(void);
U8 NFC_WriteWaterState(NFC_WaterState_T *pData);
U8 NFC_ReadWaterState(NFC_WaterState_T *pData);

// Ster 
U8 NFC_InitSterPassedDay(void);
U8 NFC_WriteSterPassedDay(NFC_SterPassedDay_T *pData);
U8 NFC_ReadSterPassedDay(NFC_SterPassedDay_T *pData);

// Ster Config
U8 NFC_InitSterConf(U8 mu8Mode, U8 mu8Hour, U8 mu8Min, U8 mu8Period );
U8 NFC_WriteSterConf(NFC_SterConf_T *pData);
U8 NFC_ReadSterConf(NFC_SterConf_T *pData);

// User use state
U8 NFC_InitUserUseState(void);
U8 NFC_WriteUserUseState(U16 mu16User, NFC_UserUseState_T *pData);
U8 NFC_ReadUserUseState(U16 mu16User, NFC_UserUseState_T *pData);

// Water History
// mu16Day : 1 ~ 60 
// mu8StartDay : 1 ~ 60
// mu16Num : 1 ~ 60
U8 NFC_InitWaterHistory(void);
U8 NFC_WriteWaterHistory(U16 mu16Day, NFC_WaterHis_T *pData);
U8 NFC_ReadWaterHistory(U16 mu16StartDay, U16 mu16Num, NFC_WaterHis_T *pData );

// Ster History
U8 NFC_InitSterHistory(void);
U8 NFC_WriteSterHistory(U16 mu16Day, NFC_SterHis_T *pData);
U8 NFC_ReadSterHistory(U16 mu16StartDay, U16 mu16Num, NFC_SterHis_T *pData);

// Error History
U8 NFC_InitErrorHistory(void);
U8 NFC_WriteErrorHistory(U16 mu16Count, NFC_ErrorHis_T *pData);
U8 NFC_ReadErrorHistory(U16 mu16StartDay, U16 mu16Num, NFC_ErrorHis_T *pData);

// Error State
U8 NFC_InitErrorState(U8 mu8Val);
U8 NFC_WriteErrorState(NFC_ErrorState_T *pData);
U8 NFC_ReadErrorState(NFC_ErrorState_T *pData);

// Service(Heart Service) State
U8 NFC_InitServiceState(U8 mu8Val);
U8 NFC_WriteServiceState(NFC_ServiceState_T *pData);
U8 NFC_ReadServiceState(NFC_ServiceState_T *pData);

// User Hisotry
U8 NFC_InitUserHistory(void);
U8 NFC_WriteUserHistory(U16 mu16Count, NFC_UserHis_T *pData);
U8 NFC_ReadUserHistory(U16 mu16StartDay, U16 mu16Num, NFC_UserHis_T *pData);

// Erase all blocks
U8 NFC_EraseAll(void);

#endif /* __NFC_H__ */
