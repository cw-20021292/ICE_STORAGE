/// @file     Lib_TDS.c
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    TDS 동작 제어 file

#include "Global_Header.h"


#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )

/// @brief      TDS Check 시작 처리 함수
/// @details    TDS 체크 시작 여부를 설정한다
/// @param      mu8OnOff : TDS 체크 제어 시작 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_TDS_Check_Start(U8 mu8OnOff)
{
#if (TDS_IN_USE == USE)     // TDS In 센서 사용시
    Set_TDS_In_Check_Start(mu8OnOff);
#endif

#if (TDS_OUT_USE == USE)    // TDS Out 센서 사용시
    Set_TDS_Out_Check_Start(mu8OnOff);
#endif
}



/// @brief      TDS 체크 동작 상태 확인 함수
/// @details    TDS 체크 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : TDS 체크 동작 여부 - 0(미동작), 1(동작)
U8 Get_TDS_CheckStatus(void)
{
#if (TDS_IN_USE == USE)     // TDS In 센서 사용시
    U8 mu8Status = 0;

    // TDS In과 Out은 항상 동시 측정하므로 TDS In 체크 동작 여부로 TDS 측정중인지 판단
    mu8Status = Get_TDS_In_Check_Start();

    if (mu8Status)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }

#else                       // TDS In 센서 미사용시

#if (TDS_OUT_USE == USE)    // TDS Out 센서 사용시
    U8 mu8Status = 0;

    mu8Status = Get_TDS_Out_Check_Start();

    if (mu8Status)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }

#endif

#endif
}

#endif

#if (TDS_IN_USE == USE)

/// @brief      TDS 체크 동작 상태 확인 함수
/// @details    TDS 체크 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : TDS 체크 동작 여부 - 0(미동작), 1(동작)
U16 Get_TDS_In_Value(void)
{
    return  Get_TDS_In_Data();
}

#endif

#if (TDS_OUT_USE == USE)
/// @brief      TDS 체크 동작 상태 확인 함수
/// @details    TDS 체크 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : TDS 체크 동작 여부 - 0(미동작), 1(동작)
U16 Get_TDS_Out_Value(void)
{
    return  Get_TDS_Out_Data();
}
#endif
