/**
 * File : comm.c
 * 
 * Uart Commnuication
*/
#include "comm.h"

CommHeader_T commHeader[COMM_ID_MAX];

#define COMM_COMP_RX_BUF_SZ     (30)
#define COMM_COMP_TX_BUF_SZ     (30)

#define COMM_FRONT_RX_BUF_SZ    (150)
#define COMM_FRONT_TX_BUF_SZ    (150)

#define COMM_EOL_RX_BUF_SZ      (128)
#define COMM_EOL_TX_BUF_SZ      (256)

U8 compRecvBuf[COMM_COMP_RX_BUF_SZ];
U8 compSendBuf[COMM_COMP_TX_BUF_SZ];

U8 frontRecvBuf[COMM_FRONT_RX_BUF_SZ];
U8 frontSendBuf[COMM_FRONT_TX_BUF_SZ];

U8 eolRecvBuf[COMM_EOL_RX_BUF_SZ];
U8 eolSendBuf[COMM_EOL_TX_BUF_SZ];

typedef struct _comm_
{
    U8 *recvBuf;
    U8 *sendBuf;
    I16 recvLen;
    I16 sendLen;
    I16 txLen;
} SComm_T;

SComm_T comm[COMM_ID_MAX];

void InitComm(void)
{
    MEMSET( (void __FAR *)&comm, 0x00, sizeof(SComm_T) );

    /// COMP
    comm[COMM_ID_COMP].recvBuf = &compRecvBuf[0];
    comm[COMM_ID_COMP].sendBuf = &compSendBuf[0];
    HalUart2Start();

    /// FRONT
    comm[COMM_ID_FRONT].recvBuf = &frontRecvBuf[0];
    comm[COMM_ID_FRONT].sendBuf = &frontSendBuf[0];
    HalUart0Start();

    /// EOL
    comm[COMM_ID_EOL].recvBuf = &eolRecvBuf[0];
    comm[COMM_ID_EOL].sendBuf = &eolSendBuf[0];
    HalUart3Start();   
}

void InitCommId(ECommId id)
{
    SComm_T *pComm = NULL;

    pComm = &comm[id];

    if( id == COMM_ID_COMP )
    {
        MEMSET( (void __FAR *)&pComm->recvBuf[0], 0x00, COMM_COMP_RX_BUF_SZ );
        MEMSET( (void __FAR *)&pComm->sendBuf[0], 0x00, COMM_COMP_TX_BUF_SZ );
    }
    else if( id == COMM_ID_FRONT )
    {
        MEMSET( (void __FAR *)&pComm->recvBuf[0], 0x00, COMM_FRONT_RX_BUF_SZ );
        MEMSET( (void __FAR *)&pComm->sendBuf[0], 0x00, COMM_FRONT_TX_BUF_SZ );
    }
    else if( id == COMM_ID_EOL )
    {
        MEMSET( (void __FAR *)&pComm->recvBuf[0], 0x00, COMM_EOL_RX_BUF_SZ );
        MEMSET( (void __FAR *)&pComm->sendBuf[0], 0x00, COMM_EOL_TX_BUF_SZ );
    }
    
    pComm->recvLen = 0;
    pComm->sendLen = 0;
    pComm->txLen = 0;
}

static I16 GetRecvBufferLength(ECommId id)
{
    I16 size = 0;

    if( id == COMM_ID_FRONT )
    {
        size = COMM_FRONT_RX_BUF_SZ;
    }
    else if( id == COMM_ID_COMP )
    {
        size = COMM_COMP_RX_BUF_SZ;
    }
    else if( id == COMM_ID_EOL )
    {
        size = COMM_EOL_RX_BUF_SZ;
    }

    return size;
}

U8 IsFullRecvBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->recvLen >= GetRecvBufferLength(id) )
    {
        return TRUE;
    }

    return FALSE;
}

static IsEmptyRecvBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->recvLen > 0 )
    {
        return FALSE;
    }

    return TRUE;
}

static I16 GetSendBufferLength(ECommId id)
{
    I16 size = 0;

    if( id == COMM_ID_FRONT )
    {
        size = COMM_FRONT_TX_BUF_SZ;
    }
    else if( id == COMM_ID_COMP )
    {
        size = COMM_COMP_TX_BUF_SZ;
    }
    else if( id == COMM_ID_EOL )
    {
        size = COMM_EOL_TX_BUF_SZ;
    }

    return size;
}

static IsFullSendBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->sendLen >= GetSendBufferLength(id) )
    {
        return TRUE;
    }

    return FALSE;
}

void InitRecvLength(ECommId id)
{
    comm[id].recvLen = 0;
}

static I16 GetRecvLength(ECommId id)
{
    return comm[id].recvLen;
}

void InitSendLength(ECommId id)
{
    comm[id].sendLen = 0;
}

static I16 GetSendLength(ECommId id)
{
    return comm[id].sendLen;
}

void SetRecvBuffer(ECommId id, U8 data)
{
    SComm_T *communication = NULL;

    communication = &comm[id];
    communication->recvBuf[communication->recvLen] = data;
    communication->recvLen++;
}

static U8 GetRecvBuffer(ECommId id, U16 index)
{
    SComm_T *communicaiton = NULL;

    communicaiton = &comm[id];

    return communicaiton->recvBuf[index];
}

static void SetSendBuffer(ECommId id, U8 data)
{
    SComm_T *communicaiton = NULL;

    if( IsFullSendBuffer(id) == FALSE )
    {
        communicaiton = &comm[id];

        communicaiton->sendBuf[communicaiton->sendLen] = data;
        communicaiton->sendLen++;
    }   
}

/**
 * @ brief           Function For Get Raw Data In Send Buffer
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ param    index  Send Buffer Index
 * @ return          none
**/
static U8 GetSendBuffer(ECommId id, U16 index)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    return communication->sendBuf[index];
}

/**
 * @ brief           Function For Send Data From Send Buffer
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ return          none
**/
void SendByte(ECommId id)
{
    U8 txData = 0U;
    SComm_T *communication = NULL;

    communication = &comm[id];

    txData = communication->sendBuf[communication->txLen];
    communication->txLen++;

    if( id == COMM_ID_FRONT )
    {
        HalUart0SendByte(txData);
    }
    else if( id == COMM_ID_COMP )
    {
        HalUart2SendByte(txData);
    }
    else if( id == COMM_ID_EOL )
    {
        HalUart3SendByte(txData);
    }
}

/**
 * @ brief           Function For Check if Transmit is complete
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ return          TRUE : Complete, FALSE : NO
**/
U8 IsCompleteTx(ECommId id)
{
    if( comm[id].txLen < comm[id].sendLen )
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @ brief          Function For Set Protocol Command
 * @ detail         none
 * @ param   id     User Commnication Id
 * @ param   cmd    Protocol Command
 * @ return         none
**/
void SetCommHeader(ECommId id, CommHeader_T cmd)
{
    // Set address of data model 
    commHeader[id] = cmd;
}

/**
 * @ brief          Function For Get Protocol Command
 * @ detail         none
 * @ param   id     User Commnication Id
 * @ return         Protocol Command
**/
CommHeader_T GetCommHeader(ECommId id)
{
    // Get address of data model 
    return commHeader[id];
}

/***********************************************************************************************
 *  SEND PACKET 
 */
I16 SendCommPacket(ECommId id, U8 *sendBuf, I16 len)
{
    U8 i = 0U;

    if( IsFullSendBuffer(id) != TRUE )
    {
        for( i=0; i<len; i++ )
        {
            SetSendBuffer(id, sendBuf[i]);
        }

        SendByte(id);
        return len;     /// send buf size
    }

    return -1; /// error - there is full data
}

/***********************************************************************************************
 *  RECV PACKET 
 */
I8 RecvCommPacket(ECommId id, U8 *recvBuf)
{
    U16 i = 0U;
    I16 len;

    if( IsEmptyRecvBuffer(id) != TRUE )
    {
        len = GetRecvLength(id);
        for( i=0; i<len; i++ )
        {
            recvBuf[i] = GetRecvBuffer(id, i);
        }

        return len; /* received buf size */
    }

    return -1;  /* error - there is no data */
}
