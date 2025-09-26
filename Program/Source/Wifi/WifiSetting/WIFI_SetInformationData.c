
/*******************************************************************************
&Description :
    A2010 ~ A201x
    Information Table
  
&History : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetInformationData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxInfo ( U16 mId, I8 *pBuf );
static void EvtWifiRxInfo ( U16 mId, I8 *pBuf );
static void EvtWifiRxJsonInfo ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
static const WifiTxFuncList_T WifiInformationList[] = 
{   /*  ID                              Tx Function      Rx Function     */
	{   0x0000,                        EvtWifiTxInfo,   EvtWifiRxInfo   },	

};


static const WifiTxFuncList_T WifiInformationList_Json[] = // TOPIC : MContent ()
{   /*  ID                              Tx Function         Rx Function     */
    {   0x0000,                        EvtWifiTxInfo,    EvtWifiRxJsonInfo    },
};


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiInformationEventList ( void )
{
    return WifiInformationList;
}

void* GetWifiInformationJsonEventList ( void ) // Json
{
    return WifiInformationList_Json;
}



U8 IsPositiveData ( U16 mId )
{
    WifiNormalList_T *pList = NULL;
    U16 mu16Size = 0;
    U8 mi = 0;

    pList = (WifiNormalList_T *)GetWifiInfoDataEventList();
    mu16Size = GetWifiInfoEventListSize();

    for ( mi = 0 ; mi < mu16Size; mi++ )
    {
        if (mId == (pList+mi)->Id)
        {
            break;
        }
    }
    if ( ((pList+mi)->u8DataType) == TYPE_INFO_TEMP )  // CURRENT_TEMP, HIGH_TEMP, LOW_TEMP 
    { 
        return FALSE; // Negative
    }
    return TRUE; // Positive

    // if ( ( mId == WIFI_INFO_0014_CURRENT_TEMP )
    //     || ( mId == WIFI_INFO_0015_HIGH_TEMP )
    //     || ( mId == WIFI_INFO_0016_LOW_TEMP ) )
    // {
    //     return FALSE; // Negative
    // }
    // return TRUE; // Positive
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxInfo ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];
    
    _MEMSET_( (void __far*) mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii( TYPE_INFO_RQST_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxInfo ( U16 mId, I8 *pBuf )
{
    ApplyInfoData( mId, pBuf );
}

static void EvtWifiRxJsonInfo ( U16 mId, I8 *pBuf )
{
    ApplyInfoJsonData(mId, pBuf);
}


