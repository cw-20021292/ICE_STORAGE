
/*******************************************************************************
&Description :
    FOTA Process
  
&History : ShinHM
    2020.08.12 update

    ssg@coway.com 2023.12.06 
    - FOTA SIZE 처리 추가
*******************************************************************************/

/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_Fota.h"
#include "WIFI_Status.h"
#include "WIFI_UserInterface.h"


/***** Define *****************************************************************/
#if ((USER_FOTA_SIZE == 128) || (USER_FOTA_SIZE == 192))
#define LENGTH_SIZE_CHECK   3   // 513 (100MG/100PG) / 769 (100PH/100SH)
#define LENGTH_FOTA_SIZE    25  // include of 0x0D 0x0A (0~24)
#endif

#if (USER_FOTA_SIZE == 256)
#define LENGTH_SIZE_CHECK   4   // 1025 (100PJ) 
#define LENGTH_FOTA_SIZE    26  // include of 0x0D 0x0A (0~25)
#endif

#define LENGTH_TYPE_CHECK   6   // Product Code (xxxxxx)
#define POSITION_FOTA_SIZE  20  // *ICT*OTA_SEND_START=513
#define POSITION_FOTA_TYPE  33  // *ICT*OTA_SEND_DATA=<index> <size> <data> <crc> 0000 256 0F05 112874 V1.0.0.1 Aquamega200c
#define W_FOTA_ERROR_CNT    5

/***** Variable ***************************************************************/
/* WifiRx Value */
typedef struct _wififota_
{
    U8  u8RetryCount;
    U8  u8ErrorCount;
} WifiFota_T;
WifiFota_T WifiFota;

I8 ai8ProductCode[LENGTH_MODEL_TYPE+1]; // Product Code

/***** Constant ***************************************************************/

/***** Static Function ********************************************************/
static void DoRsps ( void );
static void DoReady ( void );
static void DoRun ( void );

static U8 IsFotaSizeOk ( U8 mLen, I8* pSize );
static void DoSizeOk ( void );
static void SamplingType ( void );
static U8 IsFotaTypeOk ( I8* pType );
static void DoTypeOk ( void );


/***** Table ******************************************************************/
typedef void (*WifiFotaFun_T)( void );
typedef struct _wifi_fota_list_
{
    U8 Id;
    U32 Tx;
    WifiFotaFun_T Func;
    U8 Step;
} WifiFotaList_T;

static void RunFotaData ( WifiFotaList_T *pList );

static const WifiFotaList_T WifiFotaList[] =
{   /*  ID                  AT Cmd                  Function        NextStep        */            
    {   FOTA_NULL,          WIFI_TX_NULL,           NULL,           FOTA_NULL       },
    {   FOTA_RESPONSE,      WIFI_AT_FOTA_CHECK,     DoRsps,         FOTA_NULL       },
    {   FOTA_CHECK,         WIFI_TX_NULL,           DoReady,        FOTA_DISCONNECT },
    {   FOTA_DISCONNECT,    WIFI_AT_AUTO_OFF,       DoReady,        FOTA_WAIT       },
    {   FOTA_WAIT,          WIFI_AT_AUTO_OFF,       NULL,           FOTA_NULL       },
    {   FOTA_START,         WIFI_TX_NULL,           NULL,           FOTA_RUN        },
    {   FOTA_RUN,           WIFI_TX_NULL,           DoRun,          FOTA_NULL       }
};

#define SZ_FOTA_LIST ( sizeof(WifiFotaList) / sizeof(WifiFotaList_T) )


/***** Global Function ********************************************************/
void InitializeWifiFota ( void )
{
    _MEMSET_( (void __far*) &WifiFota, 0, sizeof(WifiFota_T) );
    _MEMSET_( (void __far*) ai8ProductCode, '\0', sizeof(ai8ProductCode) );
}

void WifiFotaProcess ( void )
{
    WifiFotaList_T *pList = NULL;
    U8 i = 0U;
    U8 mFota = 0U;

    mFota = GetWifiFotaStatus( STATUS_FOTA_SEQ );
    pList = WifiFotaList;
    for ( i = 0 ; i < SZ_FOTA_LIST; i++ )
    {
        if ( mFota == (pList+i)->Id )
        {
            RunFotaData( pList+i );
            break;
        }
    }
}

void WifiCheckFotaSize ( I8* mi8FotaData )
{
    static I8 mData[LENGTH_SIZE_CHECK+1];
    U8 i = 0U;
    U8 mSize = 0U;
    
    _MEMSET_( (void __far*) mData, '\0', sizeof(mData) );

    /* Sampling */
    mSize = (U8)( _STRLEN_( (const char __far*) mi8FotaData ) );
    for ( i = 0; i < LENGTH_SIZE_CHECK; i++ )
    {
        mData[i] = mi8FotaData[POSITION_FOTA_SIZE+i];
    }
    
    /* Validity Check */
    if ( IsFotaSizeOk( mSize, mData ) != TRUE )
    {
        return;
    }
    
    /* Run */
    DoSizeOk();
}

void WifiCheckFotaType ( I8* mi8FotaData )
{
    static I8 mData[LENGTH_TYPE_CHECK+1];
    U8 i = 0U;
    
    _MEMSET_( (void __far*)mData, '\0', sizeof(mData) );

    /* Sampling */
    for ( i = 0; i < LENGTH_TYPE_CHECK; i++)
    {   /* *ICT*OTA_SEND_DATA=<index> <size> <data> <crc> 0000 256 0F05 112874 V1.0.0.1 Aquamega200c */
        mData[i] = mi8FotaData[POSITION_FOTA_TYPE+i];
    }
    SamplingType();
    
    /* Validity Check */
    if ( IsFotaTypeOk( mData ) != TRUE )
    {
        return;
    }
    
    /* Run */
    DoTypeOk();
}


/***** Local Function *********************************************************/
static void RunFotaData ( WifiFotaList_T *pList )
{
    WifiFotaFun_T pFun;
    U32 mTx = 0UL;
    U8 mStep = 0U;
            
    pFun = pList->Func;
    mTx = pList->Tx;
    mStep = pList->Step;

    if ( mTx != WIFI_TX_NULL )
    {
        SetWifiSendStatus( TX_ATCMD, mTx );
    }
        		
    if ( pFun != NULL )
    {
        pFun();
    }

    if ( mStep != FOTA_NULL )
    {
        SetWifiFotaStatus( STATUS_FOTA_SEQ, mStep );
    }
}

static void DoRsps ( void )
{
    WifiFota.u8ErrorCount++;
    if ( WifiFota.u8ErrorCount >= W_FOTA_ERROR_CNT )
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_TYPE_ERR );
        SetWifiFotaStatus( STATUS_FOTA_SEQ, FOTA_NULL );
        WifiFota.u8ErrorCount = CLEAR;
    }
}

static void DoReady ( void )
{
    SetUserDisplay( USER_DISPLAY_FOTA );
}

static void DoRun ( void )
{
    StartFotaOperation();
}

static U8 IsFotaSizeOk ( U8 mLen, I8* pSize )
{
    if ( mLen != LENGTH_FOTA_SIZE )
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_SIZE_ERR ); // 3 digits(513) must be received, but if 3 digits(513/769) are received
        return FALSE;
    }
    // if ( GetUserInterface( USER_FOTA_POSSIBLE_STATUS ) != TRUE )
    if ( GetUserInterface( USER_FOTA_POSSIBLE_STATUS ) == TRUE ) // IMPOSSIBLE
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_DEVICE_RUN );
        return FALSE;
    }
    if ( _STRNCMP_( (const char __far*) FOTA_SIZE, (const char __far*) pSize, LENGTH_SIZE_CHECK ) != N_TRUE )
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_SIZE_ERR );
        return FALSE;
    }
    return TRUE;
}

static void DoSizeOk ( void )
{
    SetWifiSendStatus( TX_ATCMD, WIFI_AT_FOTA_CHECK );
    SetWifiFotaStatus( STATUS_FOTA_SEQ, FOTA_RESPONSE );
}

static void SamplingType ( void )
{
    if ( GetUserInterface( USER_MODEL_TYPE ) == USER_MODEL_MODEL0 )
    {
        _STRCPY_( (char __far*) ai8ProductCode, (const char __far*) FOTA_TYPE_CHP );
    }
    else if ( GetUserInterface( USER_MODEL_TYPE ) == USER_MODEL_MODEL1 )
    {
        _STRCPY_( (char __far*) ai8ProductCode, (const char __far*) FOTA_TYPE_CP );
    }
    else
    {

    }
}

static U8 IsFotaTypeOk ( I8* pType )
{
    if( _STRNCMP_( (const char __far*) ai8ProductCode, (const char __far*) pType, LENGTH_TYPE_CHECK ) == N_TRUE )
    {
        return TRUE;
    }

    WifiFota.u8RetryCount++;
    if ( WifiFota.u8RetryCount >= W_FOTA_ERROR_CNT )
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_TYPE_ERR );
        SetWifiFotaStatus( STATUS_FOTA_SEQ, FOTA_NULL );
        WifiFota.u8RetryCount = CLEAR;
    }
    else
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_FOTA_CHECK );
    }
    return FALSE;    
}

static void DoTypeOk ( void )
{
    SetWifiSendStatus( TX_DATA, WIFI_TX_SEND_STOP );
    SetWifiFotaStatus( STATUS_FOTA_SEQ, FOTA_CHECK );
    WifiFota.u8RetryCount = CLEAR;
    WifiFota.u8ErrorCount = CLEAR;
}



