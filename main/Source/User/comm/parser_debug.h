/**
 * File : parser_debug.h
 * 
 * Eol (Line Auto Test ) Uart Debug Data Parsing
*/
#ifndef __PARSER_DEBUG_H__
#define __PARSER_DEBUG_H__

#include "prj_type.h"

void ParserDebugControl(U8 *buf, I16 len);
I16 MakePkt_Debug_1(U8 *buf, U16 type);
I16 MakePkt_Debug_2(U8 *buf, U16 type);
I16 MakePkt_Debug_3(U8 *buf, U16 type);
I16 MakePkt_Debug_4(U8 *buf, U16 type);
I16 MakePkt_Debug_5(U8 *buf, U16 type);
I16 MakePkt_Debug_6(U8 *buf, U16 type);
I16 MakePkt_Debug_7(U8 *buf, U16 type);

#endif  /* __PARSER_DEBUG_H__ */
