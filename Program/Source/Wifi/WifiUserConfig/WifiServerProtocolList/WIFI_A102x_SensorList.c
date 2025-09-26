
/*******************************************************************************
&Description :
    A1020 ~ A102x
    Sensor Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/


#include "WIFI_A102x_SensorList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/

/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiSensorProtocolList[] =
{   /*  API ID                          Data Type     */
    {   WIFI_SENSOR_0001_COLD_TEMP,    TYPE_SIGNED      },
    {   WIFI_SENSOR_0002_HOT_TEMP,     TYPE_SIGNED      },
    {   WIFI_SENSOR_0003_WATER_TEMP,   TYPE_SIGNED      },
    {   WIFI_SENSOR_0004_OUT_TEMP,     TYPE_SIGNED      },
    {   WIFI_SENSOR_000E_WATT_DATA,    TYPE_SENSOR_WATT },
    {   WIFI_SENSOR_000F_COMP_DATA,    TYPE_UNSIGNED    },
    {   WIFI_SENSOR_0014_HEATER_DATA,  TYPE_UNSIGNED    },
};

#define SZ_SENSOR_EVENT_LIST    (sizeof(WifiSensorProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiSensorDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiSensorProtocolList;
    }
    else
    {

    }
}

U16 GetWifiSensorEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_SENSOR_EVENT_LIST;
    }
    else
    {

    }
}

/* [User Sensor Data] Product -> Server */
I16 GetUserSystemSensor ( U16 mu16Sen )
{
    I16 mi16Data = 0;

    switch ( mu16Sen )
    {
        case WIFI_SENSOR_0001_COLD_TEMP:
            
        break;

        case WIFI_SENSOR_0002_HOT_TEMP:
        
        break;
        case WIFI_SENSOR_0003_WATER_TEMP:

        break;
        case WIFI_SENSOR_0004_OUT_TEMP:

        break;
        case WIFI_SENSOR_000E_WATT_DATA:

        break;
        case WIFI_SENSOR_000F_COMP_DATA:

        break;
        case WIFI_SENSOR_0014_HEATER_DATA:

        break;

        default:
            mi16Data = 0;
        break;
    }
    return mi16Data;
}

/* User Data Clear (Called when wifi is not connected) */
void ClearUserSystemDataA1020 ( void )
{

}