#ifndef __PARSER_SUBFRONT_H__
#define __PARSER_SUBFRONT_H__

#include "prj_type.h"
#include "comm.h"

I16 IsValidPktSubFront(U8 *buf, I16 len);
I16 ParserPktSubFront(U8 *buf, I16 len);
I16 Crc16Main(U8 *buf, I16 len);
I16 MakePktSubFront(CommHeader_T *p_comm, U8 *buf);

I16 IsValidPktSubFront(U8 *buf, I16 len);

#endif /* __PARSER_MAIN_H__ */
