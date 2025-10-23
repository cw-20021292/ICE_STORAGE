
/*******************************************************************************
&Description :
    Uart File

&History :
    ShinHM 2021.04.21 update
    -. Edit the interrupt process
*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_UartInterrupt.h"
#include "WIFI_Status.h"
#include "WIFI_RxProcess.h"


/***** Define *****************************************************************/
#define COUNT_UART_CHECK 30


/***** Variable ***************************************************************/
typedef struct _wifiuart_
{
    U8  u8TxTimer;
    U8  u8RxTimer;
    U16 u16TxLength;
    U16 u16RxLength;
    U16 u16TxStep;
    U8  u8RxFinished;
} WifiUart_T;
WifiUart_T WifiUart;

I8 ai8WifiUartTxBuff[LENGTH_WIFI_ALL_MAX_N];
// I8 ai8WifiUartRxBuff[LENGTH_WIFI_ALL_MAX_N];
I8 ai8WifiUartRxBuff[LENGTH_WIFI_ALL_RX_MAX];
// I8 ai8WifiUartTxBuff[LENGTH_WIFI_ALL_MAX];
// I8 ai8WifiUartRxBuff[LENGTH_WIFI_ALL_MAX];


/***** Constant ***************************************************************/


/***** Static Function ********************************************************/


/***** Table ******************************************************************/


/***** Global Function ********************************************************/
void InitializeWifiUart ( void )
{
    _MEMSET_( (void __far*) &WifiUart, 0, sizeof(WifiUart_T) );
    _MEMSET_( (void __far*) ai8WifiUartRxBuff, '\0', sizeof(ai8WifiUartRxBuff) );
    _MEMSET_( (void __far*) ai8WifiUartTxBuff, '\0', sizeof(ai8WifiUartTxBuff) );
}

void IntWifiUartRx ( void ) //U8 mu8RxData )
{
    U8 mu8RxData = R_RXD;
    if ( GetWifiControlStatus( STATUS_CON_POWER ) != ON ) //serial ?????????
    {   /* WIFI ON ���� Rxó�� */
        return;
    }
    if ( ( WifiUart.u16RxLength == 0 ) && ( mu8RxData != ASCII_STAR ) )
    {   /* ù��° ���� �����Ͱ� *�� �ƴ� ��� */
        return;
    }

    ai8WifiUartRxBuff[WifiUart.u16RxLength] = mu8RxData;

    if ( ( ai8WifiUartRxBuff[WifiUart.u16RxLength] == ASCII_ATDONE )
        && ( ai8WifiUartRxBuff[WifiUart.u16RxLength-1] == ASCII_DONE ) )
    {
        WifiUart.u8RxFinished = SET;
    }
    else
    {
        WifiUart.u16RxLength++;
        WifiUart.u8RxTimer = COUNT_UART_CHECK;
    }
}

void IntWifiUartTx ( void )
{
    if ( WifiUart.u16TxLength == 0 )
    {
        return;
    }

    R_TXD = ai8WifiUartTxBuff[ WifiUart.u16TxStep ];
    WifiUart.u16TxStep++;
    if( WifiUart.u16TxStep >= WifiUart.u16TxLength )
    {
    	WifiUart.u16TxLength = 0U;
    	WifiUart.u16TxStep = 0U;
    	_MEMSET_( (void __far*) ai8WifiUartTxBuff, '\0', sizeof(ai8WifiUartTxBuff) );
    }
}

void SendWifiUart ( U16 mu16PacketLength, I8* mai8WifiTxFinalData )
{
    _MEMSET_( (void __far*) ai8WifiUartTxBuff, '\0', sizeof(ai8WifiUartTxBuff) );
    far_sprintf_s( ai8WifiUartTxBuff, 0, mai8WifiTxFinalData );
    WifiUart.u16TxLength = mu16PacketLength;
    WifiUart.u8TxTimer = COUNT_UART_CHECK;
    WifiUart.u16TxStep = 0U;
    R_STIF = SET;
}

void ReceiveWifiUart ( void )
{
    if ( WifiUart.u8RxFinished == SET )
    {
        GetWifiUartRxData( ai8WifiUartRxBuff );
        _MEMSET_( (void __far*) ai8WifiUartRxBuff, '\0', sizeof(ai8WifiUartRxBuff) );
        WifiUart.u16RxLength = 0U;
        WifiUart.u8RxTimer = 0U;
        WifiUart.u8RxFinished = CLEAR;
    }
    else
    {
        // if ( WifiUart.u16RxLength >= LENGTH_WIFI_ALL_MAX_N )
        if ( WifiUart.u16RxLength >= LENGTH_WIFI_ALL_RX_MAX )
        {
            WifiUart.u16RxLength = 0U;
            WifiUart.u8RxTimer = 0U;
            _MEMSET_( (void __far*) ai8WifiUartRxBuff, '\0', sizeof(ai8WifiUartRxBuff) );
        }
    }
}

void WifiUartRxTimer ( void )
{
    if ( WifiUart.u16RxLength != 0 )
    {
        if ( WifiUart.u8RxTimer != 0 )
        {
            WifiUart.u8RxTimer--;

            if ( WifiUart.u8RxTimer == 0 )    // Rx Time Out �߻��� Buffer �ʱ�ȭ
            {
                _MEMSET_( (void __far*) ai8WifiUartRxBuff, '\0', sizeof(ai8WifiUartRxBuff) );
                WifiUart.u16RxLength = 0U;
                WifiUart.u8RxTimer = 0U;
            }
        }
    }
    else
    {
        WifiUart.u8RxTimer = 0U;
    }
}

void WifiUartTxTimer ( void )
{
    if ( WifiUart.u16TxLength != 0 )
    {
        if ( WifiUart.u8TxTimer != 0 )
        {
            WifiUart.u8TxTimer--;
        }
        if ( WifiUart.u8TxTimer == 0 )
        {
            _MEMSET_( (void __far*) ai8WifiUartTxBuff, '\0', sizeof(ai8WifiUartTxBuff) );
            SetWifiTxStatus( STATUS_TX_SEQ, WIFI_TX_SEQ_NULL );
        }
    }
    if ( GetWifiTxStatus(STATUS_TX_SEQ) == WIFI_TX_SEQ_NULL )
    {
        WifiUart.u8TxTimer = 0U;
    }
}


/***** Local Function *********************************************************/


