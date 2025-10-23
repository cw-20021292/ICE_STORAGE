/// @file     Lib_TDS.h
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    TDS ���� ���� ��� file

#ifndef _LIB_TDS_H_
#define _LIB_TDS_H_


/*  Lib TDS Function ����
    => Set_TDS_Check_Start(mu8OnOff);                   // TDS ���� ���� ���� ���� - 0(��üũ), 1(üũ)
    => Get_TDS_CheckStatus();                           // TDS ���� ���� ���� Ȯ�� - 0(��üũ), 1(üũ)

    => Get_TDS_In_Value();                              // TDS IN ���̰� Ȯ��
    => Get_TDS_Out_Value();                             // TDS OUT ���̰� Ȯ��

    ��� ��)
    u16Return = Get_TDS_In_Value();                     // TDS IN ���̰� Ȯ��
    u16Return = Get_TDS_Out_Value();                    // TDS OUT ���̰� Ȯ��
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

