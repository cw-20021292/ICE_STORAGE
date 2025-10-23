/**
 * File : parser_comp.h
 * 
 * Bldc Comp Uart Data Parsing
**/
#ifndef __PARSER_COMP_H__
#define __PARSER_COMP_H__

#include "prj_type.h"
#include "comm.h"

#define CMD_COMP_REQ        (0x01)

I16 IsValidPktComp(U8 *buf, I16 len);
I16 ParserPktComp(U8 *buf, I16 len);
I16 Crc16Comp(U8 *buf, I16 len);
I16 MakePktComp(CommHeader_T *pComm, U8 *buf);

#endif  /* __PARSER_COMP_H__ */
