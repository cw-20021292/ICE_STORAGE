/// @file     Lib_TDS.c
/// @date     2025/06/25
/// @author   Jaejin Ham
/// @brief    TDS ���� ���� file

#include "Global_Header.h"


#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )

/// @brief      TDS Check ���� ó�� �Լ�
/// @details    TDS üũ ���� ���θ� �����Ѵ�
/// @param      mu8OnOff : TDS üũ ���� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_TDS_Check_Start(U8 mu8OnOff)
{
#if (TDS_IN_USE == USE)     // TDS In ���� ����
    Set_TDS_In_Check_Start(mu8OnOff);
#endif

#if (TDS_OUT_USE == USE)    // TDS Out ���� ����
    Set_TDS_Out_Check_Start(mu8OnOff);
#endif
}



/// @brief      TDS üũ ���� ���� Ȯ�� �Լ�
/// @details    TDS üũ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TDS üũ ���� ���� - 0(�̵���), 1(����)
U8 Get_TDS_CheckStatus(void)
{
#if (TDS_IN_USE == USE)     // TDS In ���� ����
    U8 mu8Status = 0;

    // TDS In�� Out�� �׻� ���� �����ϹǷ� TDS In üũ ���� ���η� TDS ���������� �Ǵ�
    mu8Status = Get_TDS_In_Check_Start();

    if (mu8Status)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }

#else                       // TDS In ���� �̻���

#if (TDS_OUT_USE == USE)    // TDS Out ���� ����
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

/// @brief      TDS üũ ���� ���� Ȯ�� �Լ�
/// @details    TDS üũ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TDS üũ ���� ���� - 0(�̵���), 1(����)
U16 Get_TDS_In_Value(void)
{
    return  Get_TDS_In_Data();
}

#endif

#if (TDS_OUT_USE == USE)
/// @brief      TDS üũ ���� ���� Ȯ�� �Լ�
/// @details    TDS üũ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TDS üũ ���� ���� - 0(�̵���), 1(����)
U16 Get_TDS_Out_Value(void)
{
    return  Get_TDS_Out_Data();
}
#endif
