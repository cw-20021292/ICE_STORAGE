#include "WIFI_Common.h"


U8 UserWifiStatusVlaue[USER_SATATUS_MAX] = {0};

U8 GetUserSystemStatus ( U8 mu8StatusType )
{
    /*
    mVal = USER_MODEL_TYPE;
        0 CLEAR: Model 0 (Model_0)
        1 SET: Model 1 (Model_1)
    mVal = USER_FACTORY_STATUS;
        0 CLEAR : Normal
        1 SET : Factory/PCB Test mode
    mVal = USER_RX_STATUS;                      
        0 CLEAR : Possible (Normal) (App Control O)
        1 SET : Impossible (Operating...) (App Control X)
    mVal = USER_SMART_POSSIBLE_STATUS;          
        0 CLEAR : Possible (Normal)
        1 SET : Impossible (Operating...)
    mVal = USER_SMART_SENSING_STATUS;         
        0 CLEAR : Normal
        1 SET : Operating
    mVal = USER_FOTA_POSSIBLE_STATUS;          
        0 CLEAR : Normal
        1 SET : Impossible (Operating...)
    mVal = USER_FACTORY_DIRECTFOTA_STATUS;  
        0 CLEAR : Normal
        1 SET : Direct FOTA mode
    */
    U8 mu8Return = 0U;
    SetUserSystemStatusData(mu8StatusType);
    mu8Return = UserWifiStatusVlaue[mu8StatusType];
    return mu8Return;
}

void SetUserSystemStatus ( U8 mu8StatusType, U8 mu8Value )
{
    UserWifiStatusVlaue[mu8StatusType] = mu8Value;
}

/* User Data Clear */
void ClearUserSystemData ( void )
{
    ClearUserSystemDataA1010();
    ClearUserSystemDataA1020();
}






