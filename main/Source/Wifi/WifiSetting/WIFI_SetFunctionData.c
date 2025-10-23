
/*******************************************************************************
&Description :
    A1010 ~ A101x
    Function Table
  
&History : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetFunctionData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf );
static void EvtWifiRxFunc ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/

static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   0x0000,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
};


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiFuncEventList ( void )
{
    return WifiFuncList;
}


U8 IsExceptId ( U16 mId )
{
    WifiTxFuncEventList_T *pList = NULL;
    U16 mu16Size = 0;
    U8 mi = 0;

    pList = (WifiTxFuncEventList_T *)GetWifiFuncDataEventList();
    mu16Size = GetWifiFuncEventListSize();

    for ( mi = 0 ; mi < mu16Size; mi++ )
    {
        if (mId == (pList+mi)->Id)
        {
            break;
        }
    }

    if ( ((pList+mi)->mu8Type) == TYPE_SMART_DIAGNOSIS_CONTROL )  // smart diagnosis control : START or STOP
    { 
        return TRUE;
    }
    return FALSE;

    // if ( mId == 0x0040 ) 
    // {
    //     return TRUE;
    // }
    
}

/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];

    _MEMSET_( (void __far*) mai8Buf, '\0', sizeof(mai8Buf) );

    ConvertDataHexToAscii( TYPE_FUNC_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxFunc ( U16 mId, I8 *pBuf )
{
    ApplyFunctionData( mId, pBuf );
}

