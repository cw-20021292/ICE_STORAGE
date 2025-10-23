
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_UserInterface.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"
#include "WIFI_SetInformationData.h"
#include "WIFI_Status.h"

typedef struct _interface_
{
    /* EEp Read / Write */
    void (*UserWriteEep)( U16 mAddr, U8 mVal );
    U8 (*UserReadEep)( U16 mAddr );
    
    /* Buzzer / User Interface Data */
    void (*SetUserSound)( U16 mBuzz );
    U8 (*GetUserStatus) ( U8 mVal );
    void (*SetUserDisplay)( U8 mVal );
    void (*ClearUserData) ( void );

    /* User Data Matching */
    U32 (*GetUserA9010Func)( U16 mVal, U16 mLen);
    U16 (*GetUserA1010Func)( U16 mVal );
    I16 (*GetUserA1020Sens)( U16 mVal );
    U16 (*GetUserA1030Err)( U16 mVal );
    U16 (*GetUserA1080Part)( U16 mVal );
    U32 (*GetUserA10A0Acc)( U16 mVal );
    U16 (*GetUserA1050Para)( U16 mVal );

    /* App Control -> User System Function Data */
    void (*SetUserA1010Func)( U16 mId, U16 mVal );

    /*  Diagnosis result -> User System Diagnosis Result Data */
    void (*SetUserA1091DR)( U16 mId, I16 mVal );

    /* RAM Monitor */
    void (*SetUserA9010Func)( U16 mId, I8 *mVal );
    
    /* Time Setting */
    void (*SetUserTime)( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay );

    /* Json Setting */
    void (*SetUserA2012Json)( U16 mu16Info, I8 *pBuf );

}InterFaceWIFI_T;

InterFaceWIFI_T interface;

static void InitInterfaceWIFI ( InterFaceWIFI_T *pInterface );

/* User Function Matching */ 
void InitializeWifiUserInterface ( void )
{
    InterFaceWIFI_T  mInterface;

    mInterface.UserWriteEep = UserWriteEep;
    mInterface.UserReadEep = UserReadEep;
    
    mInterface.SetUserSound = UserBuzzSound;
    mInterface.GetUserStatus = GetUserSystemStatus;
    mInterface.SetUserDisplay = SetUserSystemDisplay;
    mInterface.ClearUserData = ClearUserSystemData; 
    
    mInterface.GetUserA9010Func = GetUserSystemRamMonitor;
    mInterface.GetUserA1010Func = GetUserSystemFunction;
    mInterface.GetUserA1020Sens = GetUserSystemSensor;
    mInterface.GetUserA1030Err = GetUserSystemError;
    mInterface.GetUserA1080Part = GetUserSystemPart;
    mInterface.GetUserA10A0Acc = GetUserSystemAccumulate;
    mInterface.GetUserA1050Para = GetUserSystemPara;
    
    mInterface.SetUserA1010Func = SetUserSystemFunction;
    mInterface.SetUserA1091DR = SetUserSystemDiagnosis;

    mInterface.SetUserA9010Func = SetUserSystemRamMonitor;

    mInterface.SetUserTime = SetUserSystemTime;
#if (USER_USE_A2021_SAPINFO == 1)
    mInterface.SetUserA2012Json = SetUserSystemJsonObject;
#endif
    InitInterfaceWIFI( &mInterface );
}

static void InitInterfaceWIFI ( InterFaceWIFI_T *pInterface )
{
    interface.UserWriteEep = pInterface->UserWriteEep;
    interface.UserReadEep = pInterface->UserReadEep;
    
    interface.SetUserSound = pInterface->SetUserSound;
    interface.GetUserStatus = pInterface->GetUserStatus;
    interface.SetUserDisplay = pInterface->SetUserDisplay;
    interface.ClearUserData = pInterface->ClearUserData;

    interface.GetUserA1010Func = pInterface->GetUserA1010Func;
    interface.GetUserA1020Sens = pInterface->GetUserA1020Sens;
    interface.GetUserA1030Err = pInterface->GetUserA1030Err;
    interface.GetUserA1080Part = pInterface->GetUserA1080Part;
    interface.GetUserA10A0Acc = pInterface->GetUserA10A0Acc;
    interface.GetUserA1050Para = pInterface->GetUserA1050Para;
    interface.GetUserA9010Func = pInterface->GetUserA9010Func;

    interface.SetUserA1010Func = pInterface->SetUserA1010Func;
    interface.SetUserA1091DR = pInterface->SetUserA1091DR;
    interface.SetUserA9010Func = pInterface->SetUserA9010Func;
    
    interface.SetUserTime = pInterface->SetUserTime;
#if (USER_USE_A2021_SAPINFO == 1)
    interface.SetUserA2012Json = pInterface->SetUserA2012Json;
#endif
}

void WifiWriteEep ( U16 mAddr, U8 mVal )
{
    interface.UserWriteEep(mAddr, mVal);
}

U8 WifiReadEep ( U16 mAddr )
{
    U8 mu8Data = 0U;
    mu8Data = interface.UserReadEep(mAddr);
    return mu8Data;
}

void SetWifiBuzzSound ( U16 m16Buzz )
{
    if ( GetWifiFotaStatus( STATUS_FOTA_MCU ) == CLEAR )
    {
        interface.SetUserSound(m16Buzz);
    }
}

/* User System Status */
U8 GetUserInterface ( U8 mVal )
{
    U8 mu8Return = 0U;
    mu8Return = interface.GetUserStatus(mVal); 
    return mu8Return; 
}

/* User Display Setting */
void SetUserDisplay ( U8 mDisplay )
{
    if ( mDisplay == USER_DISPLAY_FOTA )
    {
        WifiWriteEep(EEPROM_ADDR_FOTA_MCU,SET);
    }
    else if ( mDisplay == USER_DISPLAY_SELFCHECK )
    {

    }
    else if ( mDisplay == USER_DISPLAY_PARTCHECK )
    {

    }
    else
    {

    }
    interface.SetUserDisplay(mDisplay);
}

/* User Data Clear */
void ClearUserData ( void ) 
{
    interface.ClearUserData();
}

U32 GetUserMonitorData ( U16 mu16Func, U16 mu16Len )
{
    U32 mu32Data = 0U;
    mu32Data = interface.GetUserA9010Func(mu16Func, mu16Len);
    return mu32Data;
}

U16 GetUserFunctionData ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1010Func(mu16Func);
    return mu16Data;
}

I16 GetUserSensorData ( U16 mu16Sen )
{ 
    I16 mi16Data = 0;
    mi16Data = interface.GetUserA1020Sens(mu16Sen);
    return mi16Data;
}

U16 GetUserErrorData ( U16 mu16Err )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1030Err(mu16Err);
    return mu16Data;
}

U16 GetUserPartData ( U16 mu16Part )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1080Part(mu16Part);
    return mu16Data;
}

U32 GetUserAccumulateData ( U16 mu16Acc )
{
    U32 mu32Data = 0U;
    mu32Data = interface.GetUserA10A0Acc(mu16Acc);
    return mu32Data;
}

void SetUserFunctionData ( U16 mu16Func, U16 mData )
{
    interface.SetUserA1010Func(mu16Func,mData);
}

void SetUserDiagnosisData ( U16 mu16Func, I16 mData )
{
    interface.SetUserA1091DR(mu16Func,mData);
}

void SetUserMonitorData ( U16 mu16address, I8 *mData )
{
    interface.SetUserA9010Func(mu16address, mData);
}

void SetUserTimeSetting ( U16* pData )
{
    interface.SetUserTime(pData[WIFI_INFO_0000_YEAR],pData[WIFI_INFO_0001_MONTH],pData[WIFI_INFO_0002_DATE],
        pData[WIFI_INFO_0003_HOUR],pData[WIFI_INFO_0004_MINUTE],pData[WIFI_INFO_0005_SECOND],pData[WIFI_INFO_0006_DAY]);
}

void SetUserJsonSetting (U16 mId, I8* pData)
{
    interface.SetUserA2012Json(mId, pData);
}

void StartFotaOperation ( void )
{
#if (USER_FOTA_ENABLE != 0)
/* G23WIFI주석처리 
    BOOT_setOTARequest();
    */
#endif
}


