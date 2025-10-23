/**
 * File : parser_bldc_comp.c
 * 
 * Bldc Comp Uart Data Parsing
*/
#include "parser_comp.h"

#include "timer.h"
#include "comp_bldc.h"
#include "err_comp_bldc_comm.h"

#define CHKSUM_MAGIC_CODE       (0x55)

#if 1

#define FIELD_ERR_CODE          (0)
#define FIELD_FEEDBACK_RPS      (1)
#define FIELD_CHECKSUM          (2)

#define MIN_RX_PKT_SZ           (3)
#define MIN_TX_PKT_SZ           (2)

#else

#define FIELD_ERR_CODE          (0)
#define FIELD_FEEDBACK_RPS      (1)
#define FIELD_FEEDBACK_TEMP     (2)
#define FIELD_CHECKSUM          (3)

#define MIN_RX_PKT_SZ           (4)
#define MIN_TX_PKT_SZ           (2)

#endif
static U8 Check_Crc(U8 *buf, I16 len)
{
    U8 uChecksum = 0;

#if 1
    uChecksum = (buf[FIELD_ERR_CODE] + buf[FIELD_FEEDBACK_RPS]) ^ CHKSUM_MAGIC_CODE;
    if( uChecksum != buf[FIELD_CHECKSUM] )
    {
        return FALSE;
    }
#else
    uChecksum = (buf[FIELD_ERR_CODE] + buf[FIELD_FEEDBACK_RPS] + buf[FIELD_FEEDBACK_TEMP]) ^ CHKSUM_MAGIC_CODE;
    if( uChecksum != buf[FIELD_CHECKSUM] )
    {
        return FALSE;
    }
#endif

    return TRUE;
}

I16 IsValidPktComp(U8 *buf, I16 len)
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_RX_PKT_SZ )
    {
        return FALSE;
    }

    if( Check_Crc(buf, len) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * Received Data Parsing
**/
U8 dbg_CompErr = 0U;
U8 dbg_CompErrType = 0U;
static I16 ParserAck(U8 *buf)
{
    if( dbg_CompErr == TRUE )
    {
        SetCompBldcError(dbg_CompErrType);
    }
    else
    {  
        SetCompBldcError(buf[FIELD_ERR_CODE]);
    }
    SetCompBldcCurrentRps(buf[FIELD_FEEDBACK_RPS]);
    //SetCompBldcTemp(buf[FIELD_FEEDBACK_TEMP]);

    return TRUE;
}

I16 ParserPktComp(U8 *buf, I16 len)
{
    ParserAck(&buf[0]);

    // 통신에러 판단 타이머 ( 30초 주기로 시작되며 Comp가 ON 되면 시작인데 2초마다 통신이 되어야 함 그래서 이 카운트 Expire 되면 rx가 안들어온거임 )
    StartTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_RX_ERR, SEC(COMP_BLDC_ERR_COMM_TIMEOUT));
    ResetCompBldcCommErrorCount();

    // FCT COMP 통신 테스트 용
    SetCompBldcCommTest(TRUE);

    return len;
}

/**
 * Packet Make
**/
I16 Crc16Comp(U8 *buf, I16 len)
{
    U16 checkSum = 0;

    if( len < MIN_TX_PKT_SZ )
    {
        return 0; // error..
    }

    buf[len - 1] = buf[0] ^ CHKSUM_MAGIC_CODE;

    return len;
}

typedef I16 (*FPActionMake_T)(U8 *buf);
typedef struct _make_list_
{
    U8 Type;
    FPActionMake_T pMakePkt;
} SMakeList;

static I16 MakePktReq(U8 *buf);
const static SMakeList makeList[] = 
{
    { CMD_COMP_REQ,      MakePktReq },
};
#define SZ_MAKE_LIST    ( sizeof(makeList) / sizeof(SMakeList) )

static I16 MakePktReq(U8 *buf)
{
    I16 len = 0;

    buf[ len++ ] = (U8)GetCompBldcTargetRps();   // TARGET RPS
    buf[ len++ ] = 0;                            // CHKSUM 

    return len;
}

I16 MakePktComp(CommHeader_T *pComm, U8 *buf)
{
    U8 cmd = 0;
    U8 i = 0;
    I16 len	= -1;
    FPActionMake_T pMakePkt = NULL;

    for( i=0; i<SZ_MAKE_LIST; i++ )
    {
        cmd = makeList[i].Type;
        pMakePkt = makeList[i].pMakePkt;

        if( cmd == (U8)pComm )
        {
            if( pMakePkt != NULL )
            {
                len = pMakePkt(buf);
            }
            break;
        }
    }

    return len;
}
