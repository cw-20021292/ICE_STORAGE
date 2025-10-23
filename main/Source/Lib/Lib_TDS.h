/// @file     Lib_TDS.h
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    TDS 동작 제어 헤더 file

#ifndef _LIB_TDS_H_
#define _LIB_TDS_H_


/*  Lib TDS Function 사용법
    => Set_TDS_Check_Start(mu8OnOff);                   // TDS 센싱 시작 여부 설정 - 0(미체크), 1(체크)
    => Get_TDS_CheckStatus();                           // TDS 센싱 동작 여부 확인 - 0(미체크), 1(체크)

    => Get_TDS_In_Value();                              // TDS IN 센싱값 확인
    => Get_TDS_Out_Value();                             // TDS OUT 센싱값 확인

    사용 예)
    u16Return = Get_TDS_In_Value();                     // TDS IN 센싱값 확인
    u16Return = Get_TDS_Out_Value();                    // TDS OUT 센싱값 확인
*/


#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )
void Set_TDS_Check_Start(U8 mu8OnOff);
U16 Get_TDS_CheckStatus(void);
#endif

#if (TDS_IN_USE == USE)
U16 Get_TDS_In_Value(void);
#endif

#if (TDS_OUT_USE == USE)
U16 Get_TDS_Out_Value(void);
#endif


#endif

