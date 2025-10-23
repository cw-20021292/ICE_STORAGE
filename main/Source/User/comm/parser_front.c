/**
 * File : parser_front.h
 * 
 * Front Data Parsing
*/
#include <stdio.h>
#include <stdlib.h>

#include "parser_front.h"
#include "util.h"
#include "timer.h"
#include "purifier.h"

#include "led.h"
#include "api_key.h"

/***********************************************************************************************
 * DEFINITION 
 */
#define STX                 (0xAA)
#define ETX                 (0x55)

#define MIN_PKT_SZ          (4)

U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
{
    U8 i = 0;
    U16 wCRCin = 0x0000;
    U16 wCPoly = 0x1021;
    U8 wChar = 0;

    while(usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= ((U16)wChar << 8);
        for(i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }

    return (wCRCin);
}

static U8 Check_Crc(U8 *buf, I16 len)
{
    U16 crc16 = 0;

    crc16 = ( ( (U16)buf[ len - 3 ] ) << 8 ) & 0xFF00;
    crc16 |=    (U16)( buf[ len - 2 ] );

    if( crc16 != Rx_CRC_CCITT(buf, (U16)(len - 3)) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPktFront(U8 *buf, I16 len)
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( Check_Crc(buf, len) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


static I16 ParserAckFront(U8 *buf);
typedef I16 (*FPAction_t)(U8 *buf);
typedef struct _parser_list_
{
    U8 Type;
    FPAction_t pParserPkt;
} SParserList_T;
const static SParserList_T parserList[] = 
{
    { PKT_ACK_FRONT,     ParserAckFront },
};

#define SZ_PARSER_LIST ( sizeof(parserList) / sizeof(SParserList_T) )

I16 ParserPktFront(U8 *buf, I16 len)
{
    U8 type = 0U;
    FPAction_t pParserPkt = NULL;
    U8 i = 0U;

    for( i=0; i<SZ_PARSER_LIST; i++ )
    {
        type = parserList[i].Type;
        pParserPkt = parserList[i].pParserPkt;

        if( type == buf[1] )
        {
            if( pParserPkt != NULL )
            {
                len = pParserPkt(&buf[2]);
            }
            break;
        }
    }

    return len;
}

I16 Crc16Front(U8 *buf, I16 len)
{
    U16 checksum = 0;
    
    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    checksum = Rx_CRC_CCITT(buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_16_HIGH_BYTE(checksum);
    buf[ len - 2 ] = GET_16_LOW_BYTE(checksum);

    return len;
}

static I16 ParserAckFront(U8 *buf)
{
    U32 key = 0UL;
    U32 val_32 = 0UL;
    U16 val_16;

    /// Front Version
    SetFrontVerMajor(buf[0]);
    SetFrontVerEvent(buf[1]);
    SetFrontVerPatch(buf[2]);
    SetFrontVerMiner(buf[3]);

    /// Front Key
    val_32 = GET_32_UINT_WORD(buf[4], buf[5], buf[6], buf[7]);
    key = (val_32 & KEY_ALL);
    key = (key & (~OUT_KEY));
    SetFrontKey(key);

    /// Front Lever
    if( (val_32 & KEY_LEVER_ICE) != 0UL )
    {
        SetLeverIce(TRUE);
    }
    else
    {
        SetLeverIce(FALSE);
    }

    if( (val_32 & KEY_LEVER_WATER) != 0UL )
    {
        SetLeverWater(TRUE);
    }
    else
    {
        SetLeverWater(FALSE);
    }
    
    return TRUE;
}

typedef struct _make_list_
{
    U8 Type;
    FPAction_t pMakePkt;
} SMakeList_t;

static I16 MakePktReqFront(U8 *buf);
const static SMakeList_t makeList[] = 
{
    { PKT_REQ_FRONT,          MakePktReqFront   },
};
#define SZ_MAKE_LIST    ( sizeof(makeList) / sizeof(SMakeList_t) )

I16 MakePktFront(CommHeader_T *pComm, U8 *buf)
{
    U8 type = 0U;
    FPAction_t pMakePkt = NULL;
    U8 i = 0U;
    I16	len	= -1;

    for( i=0; i<SZ_MAKE_LIST; i++ )
    {
        type = makeList[i].Type;
        pMakePkt = makeList[i].pMakePkt;

        if( type == (U8)pComm )
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

// STX(1) + PKT_REQ_FRONT(1) + LED(69) +  CRC(2) + EXT(1)
// SIZE : 74bytes
static I16 MakePktReqFront(U8 *buf)
{
    U16 len = 0U;
    U8 i = 0U;

    /// STX 
    buf[len++] = STX;

    /// MESSAGE TYPE
    buf[len++] = PKT_REQ_FRONT;
    
    /// LED
    for( i=0; i<LED_ID_MAX; i++ )
    {
        buf[len++] = ConvertDuty2Protocol(i);
    }

    /// CRC-16
    buf[len++] = 0;
    buf[len++] = 0;

    buf[len++] = ETX;
    return len;
}
