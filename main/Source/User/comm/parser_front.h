/**
 * File : parser_front.h
 * 
 * Front Data Parsing
*/
#ifndef __PARSER_FRONT_H__
#define __PARSER_FRONT_H__

#include "prj_type.h"
#include "comm.h"

#define PKT_REQ_FRONT       (0x01)

#define PKT_ACK_FRONT       (0x81)

I16 IsValidPktFront(U8 *buf, I16 len);
I16 ParserPktFront(U8 *buf, I16 len);
I16 Crc16Front(U8 *buf, I16 len);
I16 MakePktFront(CommHeader_T *pComm, U8 *buf);

#endif /* __PARSER_FRONT_H__ */
