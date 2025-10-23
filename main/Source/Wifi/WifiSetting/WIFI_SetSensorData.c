
/*******************************************************************************
&Description :
    A1020 ~ A102x
    Sensor Table
  
&History : 
    ShinHM 2021.04.14 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetSensorData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiSensor ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/

static const WifiTxFuncList_T WifiSensorList[] = 
{   /*  ID               Tx Function     Rx Function */
    {   0x0000,         EvtWifiSensor,      NULL    }, 

};




/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiSensorEventList ( void )
{
    return WifiSensorList;
}



// U8 IsPointData ( U8 mType, U16 mId )
// {
//     WifiNormalList_T *pListNormalID = NULL;
//     U16 mu16Size = 0;
//     U8 mi=0;

//     pListNormalID = (WifiNormalList_T *)GetWifiSensorDataEventList();
//     mu16Size = GetWifiSensorEventListSize();

//     for ( mi = 0 ; mi < mu16Size; mi++ )
//     {
//         if (mId == (pListNormalID+mi)->Id)
//         {
//             break;
//         }
//     }
//     // if ( mType == TYPE_SENSOR_CMD && mId == 0x000E )
//     if ( mType == TYPE_SENSOR_CMD && ((pListNormalID+mi)->u8DataType) == TYPE_SENSOR_WATT )
//     {
//         return TRUE; 
//     }
//     return FALSE; 
// }


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiSensor ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];
    
    _MEMSET_( (void __far*) mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii( TYPE_SENSOR_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}


