#include "WIFI_cJSON.h"

#ifndef _WIFI_UserInterface_MAX_H_
#define _WIFI_UserInterface_MAX_H_



#if (USER_USE_A2021_SAPINFO == 1)


#endif

//Event Check
// #define EventCheckSize  (2)  // Func, Err
enum{ 
    WIFI_EVENT_CHECK_FUNC = 0,
    WIFI_EVENT_CHECK_ERR,

    WIFI_EVENT_CHECK_MAX
};


void UserWriteEep ( U16 mu8Addr, U8 mu8Val );
U8 UserReadEep ( U16 mu8Addr );
void UserBuzzSound ( U16 mu16Buzz );

U8 GetUserSystemStatus ( U8 mu8StatusType );
void SetUserSystemStatus ( U8 mu8StatusType, U8 mu8Value );

U16 GetUserSystemError ( U16 mu16Err );
U16 GetUserSystemFunction ( U16 mu16Func );
U32 GetUserSystemRamMonitor ( U16 mu16Func, U16 mLen);
I16 GetUserSystemSensor ( U16 mu16Sen );
U16 GetUserSystemPart ( U16 mu16Part );
U16 GetUserSystemPara ( U16 mu16Para );
U32 GetUserSystemAccumulate ( U16 mu16Acc );

void SetUserSystemFunction ( U16 mu16Func, U16 mData );
void SetUserSystemDiagnosis ( U16 mu16DR, I16 mData );
void SetUserSystemRamMonitor (__near U16 mu16Func, I8 *mData );

void SetUserSystemStatusData ( U8 mu8StatusType );

void ClearUserSystemData ( void );
void ClearUserSystemDataA1010 ( void );
void ClearUserSystemDataA1020 ( void );
void SetUserSystemDisplay ( U8 mu8Display );
// void SetUserSystemFOTA ( void );

void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay );

void SetUserSystemJson (U8 mu8JsonKey);
void SetUserSystemJsonObject (  U16 mu16Info, I8 *pBuf );

U16 GetFilterDayData (U16 mu16Data);
I16 GetFilterData (U16 mu16SensorDataFilter, U16 mu16FilterTotalUsage);

void SetUserSystemRamWriteStatus ( U16 mu16Address );
U16 GetUserSystemRamWriteStatus ( U16 mu16Address );

void SystemFunctionEventCheck(void);
void SetUserMac ( U8 mId );
#endif

