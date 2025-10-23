
/*******************************************************************************
&Description :
    Make Tx Data File (Function/Error/Sensor/Para/Part Data)
    MQTT Protocol (Message Queuing Telemetry Transport)
    JSON Format (JavaScript Object Notation)
    
&History : 
    ShinHM 2021.2.22 update
    -. Organize

&History : ssg@coway.com 2024.08.24 update
    - 모니터링 CMD 추가
    - DATA 이벤트 처리 추가
    - SDK CMD List 및 DATA TYPE 추가
`	*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_TxMake.h"
#include "WIFI_Status.h"
#include "WIFI_TxForm.h"
#include "WIFI_TxProtocol.h"
#include "WIFI_UserInterface.h"
#include "WIFI_RxServer.h"

#include "WIFI_SetCommon.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetInformationData.h"
#include "WIFI_SetAccumulateData.h"
#include "WIFI_SetMonitorData.h"


/***** Define *****************************************************************/


/***** Variable ***************************************************************/


/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static U8 SelectTxType ( void );
static void MakeTxData ( U8 mu8TxPara );
static U16 MakeTagId ( I8 *pBuf, U16 mLen, U16 mId );
static U16 MakeTagData ( I8 *pBuf, U16 mLen, I8 *pData );
static U16 DeleteComma ( I8 *pBuf, U16 mLen );
static void* GetCmdList ( E_WIFI_TX_TYPE_T mType );
static void* GetCmdListEventID ( E_WIFI_TX_TYPE_T mType );
static void* GetCmdListNormalID ( E_WIFI_TX_TYPE_T mType );
static U16 GetCmdSize ( E_WIFI_TX_TYPE_T mType );


/***** Table ******************************************************************/


/***** Global Function ********************************************************/
void WifiTxDataSelect ( void )
{
    __far WifiTxTypeList_T *pList = NULL;
    U8 i = 0U;
    U8 mSize = 0U;
    U8 mCmd = 0U;
    U32 mData = 0UL;

    /* Data Type Select */
    mCmd = SelectTxType();
    mData = GetWifiSendStatus(mCmd);
    
    /* Get Type List */
    pList = (__far WifiTxTypeList_T *)GetWifiTxTypeEventList(mCmd);
    mSize = (U8)GetWifiTxTypeListSize(mCmd);

    /* Make Tx Data */
    for ( i = 0 ; i < mSize; i++ )
    {
        U32 mId = 0UL;
    	U32 mCheck = 0UL;
        mId = (pList+i)->Id;
        mCheck = (mData&mId);
        if ( mCheck == mId )
        {
            if ( (pList+i)->mu8Seq == WIFI_TX_SEQ_SEND )
            {
                SetWifiTxStatus( STATUS_TX_STATE, SET );
            }
            SetWifiTxStatus( STATUS_TX_SEQ, (pList+i)->mu8Seq );
            MakeTxData( (pList+i)->mu8Para );
            ClearWifiSendStatus( mCmd, mId );
            SetWifiTxStatus( STATUS_TX_SEQ, WIFI_TX_SEQ_NULL );
            break;
        }
    }
}

/***** Local Function *********************************************************/
static U8 SelectTxType ( void )
{
    U8 mCmd = 0U;
    U32 mData = 0UL;
    
    mData = GetWifiSendStatus( TX_ATCMD );
    if ( mData == 0 )
    {
        mCmd = TX_DATA;
    }
    else
    {
        mCmd = TX_ATCMD;
    }
    return mCmd;
}

/* Make Tx Data */
static void MakeTxData ( U8 mu8TxPara )
{
    __far WifiTxCommandList_T *pCmdList = NULL;
    U16 mu16CmdSize = 0U;
    U8 i = 0U;
    E_WIFI_TX_TYPE_T mType = 0U;
    U8 mu8Target = 0U;
    U8 mu8RequestId = 0U;

    WifiTxFuncList_T *pList = NULL;
    WifiTxFuncEventList_T *pListEventID = NULL;
    WifiTxFuncList_Monitor_T *pListMonitor = NULL;
    WifiNormalList_T *pListNormalID = NULL;

    U16 mu16Size = 0U;
    U16 mu16Len = 0U;   
    U16 mAddress = 0U;
    U16 mAddress_len = 0U;
    U8 mData = 0U;
    U16 mu16ControlID = 0U; 
    U16 mId = 0U;
    U8 mu8DataType = 0U;
    U8 mu8DataInfoDataCount = 0U;

    static I8 mai8AtCmd[LENGTH_AT_CMD_MAX];
    static I8 mai8Api[LENGTH_API_NO+1];
    static I8 mai8GetDataBuf[LENGTH_WIFI_DATA_ONE_MAX];	
    static I8 mai8DataBuff[LENGTH_WIFI_ALL_MAX_N];   

    _MEMSET_( (void __far*) mai8AtCmd, '\0', sizeof(mai8AtCmd) );
    _MEMSET_( (void __far*) mai8Api, '\0', sizeof(mai8Api) );
    _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
    _MEMSET_( (void __far*) mai8DataBuff, '\0', sizeof(mai8DataBuff) );
    
    /* Data Matching (Tx Data table Matching) */
    mData = GetWifiBLESendStatus(TX_BLE_WIFI);
    pCmdList = (__far WifiTxCommandList_T *)GetWifiTxCmdEventList(mData);
    mu16CmdSize = GetWifiTxCmdListSize(mData);

    for ( i = 0 ; i < mu16CmdSize; i++ )
    {
        if ( mu8TxPara == (pCmdList+i)->Para )
        {
            mType = (pCmdList+i)->Type;
            mu8Target = (pCmdList+i)->Target;
            mu8RequestId = (pCmdList+i)->ReId;
            far_sprintf_s( mai8AtCmd, 0, (pCmdList+i)->AtCmd );
            far_sprintf_s( mai8Api, 0, (pCmdList+i)->Api );
            break;
        }
    }
    
    /* Get Data List */
    if (mType == TYPE_MONITOR_DATA_CMD ) // RAM Monitor
    {
        pListMonitor = GetCmdList( mType );
        mu16Size = GetCmdSize( mType );
        if ( pListMonitor != NULL )
        {
            for ( i = 0 ; i < mu16Size; i++ )
                {
                    WifiTxFun_Monitor_T pFun_M;
                    pFun_M = pListMonitor->TxFunc_M; 
                    _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                    mAddress = gu16_START_RAM_Address[i]; // rx parsing ram address 
                    mAddress_len = gu16_RAM_Address_len[i];
                    pFun_M(mAddress, mai8GetDataBuf, mAddress_len);
                    mu16Len = MakeTagId( mai8DataBuff, mu16Len, mAddress );
                    mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                }
        }
    }
    else if(mType == TYPE_BLE_CERT_CMD)
    {
        mId = GetWifiCertID();
        mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
        _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
        ConvertDataHexToAscii(mType, mId, mai8GetDataBuf);
        mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
    }
    else
    {
        pList = GetCmdList( mType );
        pListEventID = GetCmdListEventID( mType );
        pListNormalID = GetCmdListNormalID( mType );
        mu16Size = GetCmdSize( mType );

        if (mu8TxPara == PARA_STATUS_SEND_CONTROL)
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc; 
            mu16ControlID = GetWifiSendStatusControl();
            SetWifiSendStatusControl(0U);
            if (pList != NULL ){
                U8 mu8pListControId = 0U;

                if (mu16ControlID != 0U) 
                {
                    for ( i = 0 ; i < mu16Size; i++ )
                    {
                        if ((pListEventID+i)->Id == mu16ControlID)
                        {
                            mu8pListControId = i;
                            break;
                        }
                    }
                    if ( (pListEventID+mu8pListControId)->mu8Type == TYPE_A1014 )    // A1014 Protocol
                    {
                        _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                        mId = mu16ControlID;
                        pFun( mId, mai8GetDataBuf );
                        mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                        mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                    }
                }
            }
        }
        /* Make Data (Sensor / PART ) */
        else if ((mu8TxPara == PARA_SENSOR_SEND) || (mu8TxPara == PARA_SENSOR_RESPONSE)
                || (mu8TxPara == PARA_PART_SEND) || (mu8TxPara == PARA_PART_RESPONSE)
                || (mu8TxPara == PARA_EXAMINE_SEND) )
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc;
            for ( i = 0 ; i < mu16Size; i++ )
            {
                _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                mId = (pListNormalID+i)->Id;
                pFun( mId, mai8GetDataBuf );
                mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
            }
        }
        /* Make Data ( Infomation ) */
        else if ((mu8TxPara == PARA_INFO_REQUEST) || (mu8TxPara == PARA_INFO_REQUEST_USER)
                || (mu8TxPara == PARA_INFO_REQUEST_DATA))
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc;
            for ( i = 0 ; i < mu16Size; i++ )
            {
                _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                
                if (mu8TxPara == PARA_INFO_REQUEST_DATA)
                {
                    mu8DataType = (pListNormalID+i)->u8DataType;
                    if ((mu8DataType == TYPE_INFO_NORMAL)
                        || (mu8DataType == TYPE_INFO_WEATHER)
                        || (mu8DataType == TYPE_INFO_AQI)
                        || (mu8DataType == TYPE_INFO_TEMP))
                    {
                        mId = (pListNormalID+i)->Id;
                        pFun( mId, mai8GetDataBuf );
                        mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                        mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                        mu8DataInfoDataCount++;
                    }
                }
                else if ((mu8TxPara == PARA_INFO_REQUEST) || (mu8TxPara == PARA_INFO_REQUEST_USER))
                {
                    mu8DataType = (pListNormalID+i)->u8DataType;
                    if ((mu8DataType == TYPE_INFO_NORMAL)
                        || (mu8DataType == TYPE_INFO_WEATHER)
                        || (mu8DataType == TYPE_INFO_AQI)
                        || (mu8DataType == TYPE_INFO_TEMP)
                        || (mu8DataType == TYPE_INFO_TIME))
                    {
                        mId = (pListNormalID+i)->Id;
                        pFun( mId, mai8GetDataBuf );
                        mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                        mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                        mu8DataInfoDataCount++;
                    }
                }
            }
        }
        /* Make Data ( Infomation - DIAGNOSIS_RESULT_REQUEST ) */
        else if ((mu8TxPara == PARA_INFO_REQUEST_DRR)) 
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc;
            for ( i = 0 ; i < mu16Size; i++ )
            {
                mu8DataType = (pListNormalID+i)->u8DataType;
                if (mu8DataType == TYPE_INFO_DIAGNOSIS_RQ)
                {
                    _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                    mId = (pListNormalID+i)->Id;
                    pFun( mId, mai8GetDataBuf );
                    mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                    mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                    mu8DataInfoDataCount++;
                }
            }
        }
        /* Make Data ( Infomation - JSON ) */
        else if ((mu8TxPara == PARA_INFO_REQUEST_JSON)) 
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc;
            for ( i = 0 ; i < mu16Size; i++ )
            {
                mu8DataType = (pListNormalID+i)->u8DataType;
                if (mu8DataType == TYPE_INFO_JSON)
                {
                    _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                    mId = (pListNormalID+i)->Id;
                    pFun( mId, mai8GetDataBuf );
                    mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                    mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
                    mu8DataInfoDataCount++;
                }
            }
        }
        /* Make Event Data ( FUNCTION / ERROR )*/ 
        else
        {
            WifiTxFun_T pFun;
            pFun = (pList)->TxFunc;
            for ( i = 0 ; i < mu16Size; i++ )
            {
                _MEMSET_( (void __far*) mai8GetDataBuf, '\0', sizeof(mai8GetDataBuf) );
                mId = (pListEventID+i)->Id;
                pFun( mId, mai8GetDataBuf );
                mu16Len = MakeTagId( mai8DataBuff, mu16Len, mId );
                mu16Len = MakeTagData( mai8DataBuff, mu16Len, mai8GetDataBuf );
            }
        }
    }
    
    SetWifiTxStatus(STATUS_TX_INFO_SIZE, mu8DataInfoDataCount);
    
    /* Delete Comma */
    mu16Len = DeleteComma( mai8DataBuff, mu16Len );
    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) != TRUE ) 
    {
        SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    }
    /* Send Protocol ( Target / ReId Usage Status / DataBuf / AT Cmd / API ) */
    WifiSendProtocol( mu8Target, mu8RequestId, mai8DataBuff, mai8AtCmd, mai8Api );
}

/* Make Tag Id (4Byte + 1Byte) */
static U16 MakeTagId ( I8* pBuf, U16 mLen, U16 mId )
{
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 12) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 8) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 4) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId) & 0x000F));
    pBuf[mLen++] = ASCII_COLON; // 0x3A :
    return mLen;
}

/* Make Tag Data (Max7Byte + 1Byte) */
static U16 MakeTagData ( I8 *pBuf, U16 mLen, I8 *pData )
{
    mLen += far_sprintf_s( &pBuf[mLen], mLen, pData );
    pBuf[mLen++] = ASCII_COMMA; // 0x2C ,
    return mLen;
}

/* Delete Comma */
static U16 DeleteComma ( I8 *pBuf, U16 mLen )
{
    if ( mLen == 0 )
    {
        return 0;
    }
    if ( pBuf[mLen-1] == ASCII_COMMA )
    {
        mLen = mLen-1;
        pBuf[mLen] = ASCII_NULL; // 'null'
    }
    return mLen;
}

static void* GetCmdList ( E_WIFI_TX_TYPE_T mType )
{
    WifiTxFuncList_T *pList = NULL;
    WifiTxFuncList_Monitor_T *pList_monitor = NULL;
    if ( (mType == TYPE_FUNC_CMD) || (mType == TYPE_FUNC_EVENT_CMD) || (mType == TYPE_FUNC_CONTROL_CMD))
    {
    	pList = (WifiTxFuncList_T *)GetWifiFuncEventList();
    }
    else if ( mType == TYPE_SENSOR_CMD )
    {
    	pList = (WifiTxFuncList_T *)GetWifiSensorEventList();
    }
    else if ( mType == TYPE_ERROR_CMD )
    {
    	pList = (WifiTxFuncList_T *)GetWifiErrorEventList();
    }
    else if ( mType == TYPE_PART_CMD )
    {
    	pList = (WifiTxFuncList_T *)GetWifiPartEventList();
    }
    else if ( mType == TYPE_ACCUMULATE_CMD )
    {
        pList = (WifiTxFuncList_T *)GetWifiAccumulateEventList();
    }
    else if ( (mType == TYPE_INFO_RQST_CMD) || (mType == TYPE_INFO_RQST_USER_CMD) 
            || (mType == TYPE_INFO_RQST_DATA_CMD) || (mType == TYPE_INFO_RQST_DRR_CMD) 
            )
    {
        pList = (WifiTxFuncList_T *)GetWifiInformationEventList();
    }
    // else if ( mType == TYPE_INFO_RQST_USER_CMD )
    // {
    //     pList = (WifiTxFuncList_T *)GetWifiInfoUserEventList();
    // }
    // else if ( mType == TYPE_INFO_RQST_DATA_CMD )
    // {
    //     pList = (WifiTxFuncList_T *)GetWifiInfoDataEventList();
    // }
    // else if ( mType == TYPE_INFO_RQST_DRR_CMD )
    // {
    //     pList = (WifiTxFuncList_T *)GetWifiInfoDRREventList();
    // }
    else if ( mType == TYPE_INFO_RQST_JSON_CMD )
    {
        pList = (WifiTxFuncList_T *)GetWifiInformationJsonEventList();
    }
    else if ( mType == TYPE_MONITOR_DATA_CMD )
    {
        pList_monitor = (WifiTxFuncList_Monitor_T *)GetWifiMonitorEventList();
        return pList_monitor;
    }
    else
    {
        pList = NULL;
        //mu16Size = 0;
    }
    return pList;
}


static void* GetCmdListEventID ( E_WIFI_TX_TYPE_T mType )
{
    WifiTxFuncEventList_T *pList = NULL;
    if ( (mType == TYPE_FUNC_CMD) || (mType == TYPE_FUNC_EVENT_CMD) || (mType == TYPE_FUNC_CONTROL_CMD))
    {
    	pList = (WifiTxFuncEventList_T *)GetWifiFuncDataEventList();
    }

    else if ( mType == TYPE_ERROR_CMD )
    {
    	pList = (WifiTxFuncEventList_T *)GetWifiErrDataEventList();
    }

    else
    {
        pList = NULL;
    }
    return pList;
}
static void* GetCmdListNormalID ( E_WIFI_TX_TYPE_T mType )
{
    WifiNormalList_T *pList = NULL;

    if ( mType == TYPE_SENSOR_CMD )
    {
    	pList = (WifiNormalList_T *)GetWifiSensorDataEventList();
    }
    else if ( mType == TYPE_PART_CMD )
    {
    	pList = (WifiNormalList_T *)GetWifiPartDataEventList();
    }
    else if ( (mType == TYPE_INFO_RQST_CMD) || (mType == TYPE_INFO_RQST_USER_CMD) 
            || (mType == TYPE_INFO_RQST_DATA_CMD) || (mType == TYPE_INFO_RQST_DRR_CMD) 
            || (mType == TYPE_INFO_RQST_JSON_CMD)
            )
    {
    	pList = (WifiNormalList_T *)GetWifiInfoDataEventList();
    }
    else if ( mType == TYPE_ACCUMULATE_CMD )
    {
    	pList = (WifiNormalList_T *)GetWifiAccumulateDataEventList();
    }

    else
    {
        pList = NULL;
    }
    return pList;
}

static U16 GetCmdSize ( E_WIFI_TX_TYPE_T mType )
{
    U16 mSize = 0U;
    if ( (mType == TYPE_FUNC_CMD) ||(mType == TYPE_FUNC_EVENT_CMD) || (mType == TYPE_FUNC_CONTROL_CMD) )
    {
        // mSize = GetWifiFuncListSize();
        mSize = GetWifiFuncEventListSize();
    }
    else if ( mType == TYPE_SENSOR_CMD )
    {
        // mSize = GetWifiSensorListSize();
        mSize = GetWifiSensorEventListSize();
    }
    else if ( mType == TYPE_ERROR_CMD )
    {
        // mSize = GetWifiErrorListSize();
        mSize = GetWifiErrEventListSize();
    }
    else if ( mType == TYPE_PART_CMD )
    {
        // mSize = GetWifiPartListSize();
        mSize = GetWifiPartEventListSize();
    }
    else if ( (mType == TYPE_INFO_RQST_CMD) || (mType == TYPE_INFO_RQST_USER_CMD) 
            || (mType == TYPE_INFO_RQST_DATA_CMD) || (mType == TYPE_INFO_RQST_DRR_CMD) 
            || (mType == TYPE_INFO_RQST_JSON_CMD)
            )
    {
        // mSize = GetWifiInformationListSize();
        mSize = GetWifiInfoEventListSize();
    }
    else if ( mType == TYPE_ACCUMULATE_CMD )
    {
        // mSize = GetWifiAccumulateListSize();
        mSize = GetWifiAccumulateEventListSize();
    }
    // else if ( mType == TYPE_INFO_RQST_CMD )
    // {
    //     mSize = GetWifiInformationListSize();
    // }
    // else if ( mType == TYPE_INFO_RQST_USER_CMD )
    // {
    //     mSize = GetWifiInformationListSize();
    // }
    // else if ( mType == TYPE_INFO_RQST_DATA_CMD )
    // {
    //     mSize = GetWifiInformationListSize();
    // }  
    // else if ( mType == TYPE_INFO_RQST_DRR_CMD )
    // {
    //     mSize = GetWifiInformationListSize();
    // }  
    // else if ( mType == TYPE_INFO_RQST_JSON_CMD )
    // {
    //     mSize = GetWifiInformationJsonListSize();
    // }  
    else if ( mType == TYPE_MONITOR_DATA_CMD )
    {
        mSize = gu16_RAM_Monitor_RESP_SIZE;
    }
    else
    {
        mSize = 0U;
    }
    return mSize;
}


