/// @file   Lib_Water_Effluent.c
/// @date   2025/06/26
/// @author Jaejin Ham
/// @brief  ���� ���� ��� �ְ� ����

#include "Global_header.h"


/// @brief      ���� ���� ���� ������
/// @details    ���� ���� ��� �ϱ� ���� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U8 gu8WaterSelect;                                  /// @brief  �� ���� ����
    U8 gu8AmountSelect;                                 /// @brief  �� ���� ����
    U8 au8Quantitative[EFFLUENT_ID_MAX];                /// @brief  ���� ���� ���� - 0(������), 1(����)
    U16 au16Time[EFFLUENT_ID_MAX];                      /// @brief  ���� ���� �ð�
    U16 au16Hz[EFFLUENT_ID_MAX];                        /// @brief  ���� ���� Hz
    U8 au8Status[EFFLUENT_ID_MAX];                      /// @brief  ���� �߻� Ȯ�� ���� - 0(�̹߻�), 1(�߻�)
}   SEffluentData_T;

SEffluentData_T    SEffluentData;

U8 gu8EffluentPeriodTimer = 0;                          /// @brief  ���� ���� �ֱ� �ð� ī����
U16 au16EffluentTimeCount[EFFLUENT_ID_MAX];             /// @brief  ���� ���� �ð� ī����

U8 gu8RoomEffluentStep = 0;                             /// @brief  ���� ���� �ܰ�
U16 gu16RoomEffluentStepDelay = 0;                      /// @brief  ���� ���� �ܰ� ���� �� ���� ���� �ð�
U8 gu8ColdEffluentStep = 0;                             /// @brief  �ü� ���� �ܰ�
U16 gu16ColdEffluentStepDelay = 0;                      /// @brief  �ü� ���� �ܰ� ���� �� ���� ���� �ð�
U8 gu8HotEffluentStep = 0;                              /// @brief  �¼� ���� �ܰ�
U16 gu16HotEffluentStepDelay = 0;                       /// @brief  �¼� ���� �ܰ� ���� �� ���� ���� �ð�

U8 gu8DefaultWaterSelect = 0;                           /// @brief  �ü�, ���� �� Default ���� ����
U8 gu8DefaultCupSize = 0;                               /// @brief  �� ������ default��

U16 gu16MyCupAmount = 0;                                /// @brief  ������ �뷮(ml)



/// @brief      ���� ���� ���� ���� �ʱ�ȭ �Լ�
/// @details    ���� ���� ���� ���� �����͵��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_Effluent_Initialize(void)
{
    U8 mu8i = 0;

    MEMSET( (void __FAR *)&SEffluentData, 0x00, sizeof(SEffluentData_T) );

    gu8EffluentPeriodTimer = 0;

    for (mu8i = 0 ; mu8i < EFFLUENT_ID_MAX ; mu8i++)
    {
        au16EffluentTimeCount[mu8i] = 0;
    }

    SEffluentData.gu8WaterSelect = EFFLUENT_ID_ROOM;
    SEffluentData.gu8AmountSelect = AMOUNT_ID_HALF_CUP;

    gu8RoomEffluentStep = 0;
    gu16RoomEffluentStepDelay = 0;
    gu8ColdEffluentStep = 0;
    gu16ColdEffluentStepDelay = 0;
    gu8HotEffluentStep = 0;
    gu16HotEffluentStepDelay = 0;

    gu8DefaultWaterSelect = EFFLUENT_ID_ROOM;
    gu8DefaultCupSize = AMOUNT_ID_HALF_CUP;
    gu16MyCupAmount = 130;
}


/// @brief      ���õ� �� ���� ���� �Լ�
/// @details    �� ������ �����Ͽ� �����Ѵ�.
/// @param      mu8ID : �� ���� ID
/// @return     void
void Set_WaterSelect(U8 mu8ID)
{
    SEffluentData.gu8WaterSelect = mu8ID;
}


/// @brief      ���õ� �� ���� Ȯ�� �Լ�
/// @details    ���õǾ� �ִ� �� ������ Ȯ���Ͽ� ��ȯ�Ѵ�.
/// @param      void
/// @return     return : �� ���� ID
U8 Get_WaterSelect(void)
{
    return  SEffluentData.gu8WaterSelect;
}


/// @brief      �⺻ ���� �� ���� ���� �Լ�
/// @details    �⺻ ���� �� ������ �����Ͽ� �����Ѵ�.
/// @param      mu8ID : �� ���� ID
/// @return     void
void Set_DefaultWaterSelect(U8 mu8ID)
{
    gu8DefaultWaterSelect = mu8ID;
}


/// @brief      �⺻ ���� �� ���� Ȯ�� �Լ�
/// @details    �⺻ ���� �� ������ Ȯ���Ͽ� ��ȯ�Ѵ�.
/// @param      void
/// @return     return : �� ���� ID
U8 Get_DefaultWaterSelect(void)
{
    return  gu8DefaultWaterSelect;
}



/// @brief      ���õ� ���� ���� �Լ�
/// @details    ������ �����Ͽ� �����Ѵ�.
/// @param      mu8ID : ���� ID
/// @return     void
void Set_WaterAmount(U8 mu8ID)
{
    SEffluentData.gu8AmountSelect = mu8ID;
}


/// @brief      ���õ� ���� Ȯ�� �Լ�
/// @details    ���õǾ� �ִ� ������ Ȯ���Ͽ� ��ȯ�Ѵ�.
/// @param      void
/// @return     return : ���� ID
U8 Get_WaterAmount(void)
{
    return  SEffluentData.gu8AmountSelect;
}


/// @brief      �⺻ ���� ���� ���� �Լ�
/// @details    �⺻ ���� ������ �����Ѵ�.
/// @param      mu8ID : ���� ID
/// @return     void
void Set_DefaultWaterAmount(U8 mu8ID)
{
    gu8DefaultCupSize = mu8ID;
}


/// @brief      �⺻ ���� ���� Ȯ�� �Լ�
/// @details    �⺻ ���� ������ Ȯ���Ͽ� ��ȯ�Ѵ�.
/// @param      void
/// @return     return : ���� ID
U8 Get_DefaultWaterAmount(void)
{
    return  gu8DefaultCupSize;
}


/// @brief      ������ �뷮 ���� �Լ�
/// @details    ������ �뷮�� �����Ѵ�
/// @param      mu16Data : ������ ���� �뷮 - 130ml ~ 1000ml
/// @return     void
void Set_MyCupAmount(U16 mu16Data)
{
    gu16MyCupAmount = mu16Data;
}


/// @brief      ������ �뷮 ���� ���� Ȯ�� �Լ�
/// @details    ������ ������ �뷮�� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ ���� �뷮 - 130ml ~ 1000ml
U16 Get_MyCupAmount(void)
{
    return  gu16MyCupAmount;
}


/// @brief      ���� ���� ���� Timer(@1ms interrupt)
/// @details    ���� ���� ��� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void EffluentControlTimer(void)
{
    if (gu8EffluentPeriodTimer < EFFLUENT_CONTROL_PERIOD_TIME)
    {
        gu8EffluentPeriodTimer++;
    }

    EffluentTimeCountdown();    // ���� �ð� ī��Ʈ �ٿ� ����
}


/// @brief      ���� ���� Time ī��Ʈ �ٿ�(@1ms interrupt)
/// @details    ���� ���� ���� �ð��� ī��Ʈ �ٿ� �Ѵ�
/// @param      void
/// @return     void
void EffluentTimeCountdown(void)
{
    U8 mu8i = 0;

    static U8 mu8Timer = 0;

    if (++mu8Timer >= 100)
    {   // 100ms ���� ����
        mu8Timer = 0;

        for (mu8i = 0 ; mu8i < EFFLUENT_ID_MAX ; mu8i++)
        {
            if (au16EffluentTimeCount[mu8i])
            {
                au16EffluentTimeCount[mu8i]--;
            }
        }
    }
}


/// @brief      ���� ���� �ð� ī��Ʈ �ٿ� �Ϸ� üũ �Լ�
/// @details    ���� ���� �ð� ī��Ʈ �ٿ��� �Ϸ�Ǿ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� ���� �ð� ī��Ʈ �ٿ� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_EffluentTimeCountdownEnd(U8 mu8ID)
{
    if (au16EffluentTimeCount[mu8ID])
    {   // ���� �ܿ� �ð��� ������
        return  FALSE;
    }
    else
    {   // ���� �ܿ� �ð��� ������
        return  TRUE;
    }
}


/// @brief      ���� ������ ���� �������� ���� �Լ�
/// @details    ���� ������ ���� �������� ���θ� �����Ѵ�
/// @param      mu8ID : �� ���� ID
///             mu8Data : ���� ���� ���� ������ - 0(������), 1(����)
/// @return     void
void Set_QuantitativeEffluent(U8 mu8ID, U8 mu8Data)
{
    if (mu8Data)
    {
        SEffluentData.au8Quantitative[mu8ID] = SET;
    }
    else
    {
        SEffluentData.au8Quantitative[mu8ID] = CLEAR;
    }
}


/// @brief      ���� ������ ���� �������� Ȯ�� �Լ�
/// @details    ���� ������ ���� �������� ���θ� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� ���� ������ - 0(������), 1(����)
U8 Get_QuantitativeEffluent(U8 mu8ID)
{
    if (SEffluentData.au8Quantitative[mu8ID])
    {   // ���� ������ ���
        return  TRUE;
    }
    else
    {   // ���� ������ �ƴ� ���
        return  FALSE;
    }
}


/// @brief      ���� ���� �ð� ���� �Լ�
/// @details    ���� ������ ������ �ð��� �����Ѵ�
/// @param      mu8ID : �� ���� ID
///             mu8Data : ���� ���� �ð���(100ms����)
/// @return     void
void Set_EffluentTime(U8 mu8ID, U16 mu16Data)
{
    SEffluentData.au16Time[mu8ID] = mu16Data;   // ������ ���� ���� �ð�

    au16EffluentTimeCount[mu8ID] = mu16Data;    // ���� ī��Ʈ �ٿ��� ���� �ð�
}


/// @brief      ���� ���� ���� �ð� Ȯ�� �Լ�
/// @details    ���� ���� ���� �ð��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� �ð���(100ms����)
U16 Get_EffluentTime(U8 mu8ID)
{
    return  SEffluentData.au16Time[mu8ID];
}


/// @brief      ���� ���� �ܿ� �ð� Ȯ�� �Լ�
/// @details    ���� ���� �ܿ� �ð��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� �ð���(100ms����)
U16 Get_EffluentRemainTime(U8 mu8ID)
{
    return  au16EffluentTimeCount[mu8ID];
}


/// @brief      ���� ���� Hz ���� �Լ�
/// @details    ���� ������ ������ Hz�� �����Ѵ�
/// @param      mu8ID : �� ���� ID
///             mu8Data : ���� ���� Pulse��
/// @return     void
void Set_EffluentHz(U8 mu8ID, U16 mu16Data)
{
    SEffluentData.au16Hz[mu8ID] = mu16Data;     // ������ ���� ���� Hz

#if (EFFLUENT_FLOWMETER_USE == USE)
    Set_FlowmeterCountdownPulse(FLOWMETER_1_ID, mu16Data);      // ���� ī��Ʈ �ٿ��� �������� ���� Hz ����
#endif
}


/// @brief      ���� ���� ���� Hz Ȯ�� �Լ�
/// @details    ���� ���� ������ ������ Hz�� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� Pulse��
U16 Get_EffluentHz(U8 mu8ID)
{
    return  SEffluentData.au16Hz[mu8ID];
}


/// @brief      ���� ���� �ܿ� Hz Ȯ�� �Լ�
/// @details    ���� ���� �����ϴ� ���� �ܿ� Hz�� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      mu8FlowmeterID : �������� ID
/// @return     return : ���� ���� Pulse��
U16 Get_EffluentRemainHz(U8 mu8FlowmeterID)
{
#if (EFFLUENT_FLOWMETER_USE == USE)
    return  Get_FlowmeterCountdownPulse(mu8FlowmeterID);
#else
    return 0;
#endif
}


/// @brief      ���� ���� Pulse ī��Ʈ �ٿ� �Ϸ� üũ �Լ�
/// @details    ���� ���� Pulse ī��Ʈ �ٿ��� �Ϸ�Ǿ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� ���� �ð� ī��Ʈ �ٿ� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_EffluentHzCountdownEnd(U8 mu8ID)
{
#if (EFFLUENT_FLOWMETER_USE == USE)
    U8 mu8FlowmeterCountdownEnd = 0;
    U8 mu8ReturnData = 0;

    if (mu8ID == EFFLUENT_ID_ROOM)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_1_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // ī��Ʈ �ٿ��� �Ϸ�� ���
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }

    if (mu8ID == EFFLUENT_ID_COLD)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_2_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // ī��Ʈ �ٿ��� �Ϸ�� ���
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }

    if (mu8ID == EFFLUENT_ID_HOT)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_3_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // ī��Ʈ �ٿ��� �Ϸ�� ���
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }
    return  mu8ReturnData;
#else
    return  TRUE;
#endif
}


/// @brief      ���� ���� ���� �Լ�
/// @details    ���� ������ ���� ������ ���θ� �����Ѵ�
/// @param      mu8ID : �� ���� ID
///             mu8Data : ���� ���� ���� ������ - 0(������), 1(����)
/// @return     void
void Set_EffluentStatus(U8 mu8ID, U8 mu8Data)
{
    if (mu8Data)
    {
        SEffluentData.au8Status[mu8ID] = SET;
    }
    else
    {
        SEffluentData.au8Status[mu8ID] = CLEAR;
    }
}


/// @brief      ���� ������ Ȯ�� �Լ�
/// @details    ���� ������ ���������� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� ���� ������ - 0(������), 1(����)
U8 Get_EffluentStatus(U8 mu8ID)
{
    if (SEffluentData.au8Status[mu8ID])
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ������ �ʱ�ȭ �Լ�
/// @details    ID�� �ش��ϴ� �� ������ ���� ���� ���� �����͸� ��� �ʱ�ȭ ��Ų��
/// @param      mu8ID : �� ���� ID
/// @return     void
void Set_ClearEffluentData(U8 mu8ID)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_COLD] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_COLD] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_COLD] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_COLD] = CLEAR;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_HOT] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_HOT] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_HOT] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_HOT] = CLEAR;
    }
    else
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_ROOM] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_ROOM] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_ROOM] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_ROOM] = CLEAR;
    }
}


/// @brief      ���� ������ �Ϸ�Ǿ����� Ȯ�� �Լ�
/// @details    ���� ������ �Ϸ�Ǿ����� Ȯ���Ͽ� �� ���°��� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� �Ϸ� ������ - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_EffluentEndCheck(U8 mu8ID)
{
    U8 mu8EffluentTimeEnd = 0;
    U8 mu8EffluentHzEnd = 0;

    mu8EffluentTimeEnd = Get_EffluentTimeCountdownEnd(mu8ID);
    mu8EffluentHzEnd = Get_EffluentHzCountdownEnd(mu8ID);

    if ( (mu8EffluentTimeEnd == TRUE) && (mu8EffluentHzEnd == TRUE) )
    {   // ���� �ð�, ���� Hz ��� 0�� �Ǿ��� ��
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Effluent ���� �Լ�(100ms)
/// @details    ���� ������ ��Ȳ�� ���� �����ϴ� ������ �Ѵ�
/// @param      void
/// @return     void
void Lib_Effluent_Control(void)
{
    U8 mu8EffluentStatus = 0;

    if (gu8EffluentPeriodTimer >= EFFLUENT_CONTROL_PERIOD_TIME)
    {   // 100ms ���� ����
        gu8EffluentPeriodTimer = 0;

        mu8EffluentStatus = Get_OperatingStatus();

        if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
        {   // ���� ���� ���̸�
            if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
            {   // ���� ���� ������ ���
                RoomEffluentControl();
            }
            else
            {   // ���� ���� ���� �ƴϸ� ���� ���� ���� ������ �ʱ�ȭ
                Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
            {   // �ü� ���� ������ ���
                ColdEffluentControl();
            }
            else
            {   // �ü� ���� ���� �ƴϸ� �ü� ���� ���� ������ �ʱ�ȭ
                Set_ClearEffluentData(EFFLUENT_ID_COLD);
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
            {   // �¼� ���� ������ ���
                HotEffluentControl();
            }
            else
            {   // �¼� ���� ���°� �ƴϸ� �¼� ���� ���� ������ �ʱ�ȭ
                Set_ClearEffluentData(EFFLUENT_ID_HOT);
            }
        }
        else
        {   // ���� ���� ���� �ƴϸ�
            if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
            {   // ���� ���� ������ ��� ���� ����
                RoomEffluentStop();
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
            {   // �ü� ���� ������ ��� ���� ����
                ColdEffluentStop();
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
            {   // �¼� ���� ������ ��� ���� ����
                HotEffluentStop();
            }
        }
    }
}


/// @brief      Effluent ���� ���� �Լ�
/// @details    ���� ������ ������ �����Ų��
/// @param      void
/// @return     void
void Lib_Effluent_Stop(void)
{
    U8 mu8EffluentStatus = 0;

    mu8EffluentStatus = Get_OperatingStatus();

    if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
    {   // ���� ���¿����� ��� ���·� ��ȯ
        Set_OperatingStatus(OPERATION_ID_STAND_BY);
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
    {   // ���� ���� ������ ��� ���� ����
        RoomEffluentStop();
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
    {   // �ü� ���� ������ ��� ���� ����
        ColdEffluentStop();
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
    {   // �¼� ���� ������ ��� ���� ����
        HotEffluentStop();
    }
}



/// @brief      ���� ���� ���� �Լ�
/// @details    ���� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void RoomEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8RoomEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // ���� ����
            Set_FlowMeterErrorCheckStart(SET);      // �������� ���� ���� ����
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // ���� ����
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_ROOM);

            if (mu8EffluentEnd == TRUE)
            {
                gu16RoomEffluentStepDelay = 0;
                gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����
                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            }
            break;

        default :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;
            Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����

            Set_EffluentTime(EFFLUENT_ID_ROOM, 0);
            Set_EffluentHz(EFFLUENT_ID_ROOM, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            break;
    }
}


/// @brief      ���� ���� ���� �Լ�
/// @details    ���� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void RoomEffluentStop(void)
{
    gu16RoomEffluentStepDelay = 0;
    gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;

    Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����
    Set_EffluentTime(EFFLUENT_ID_ROOM, 0);
    Set_EffluentHz(EFFLUENT_ID_ROOM, 0);

    Set_ClearEffluentData(EFFLUENT_ID_ROOM);
}



/// @brief      �ü� ���� ���� �Լ�
/// @details    �ü� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void ColdEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8ColdEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // ���� ����
            Set_FlowMeterErrorCheckStart(SET);      // �������� ���� ���� ����
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // ���� ����
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_COLD);

            if (mu8EffluentEnd == TRUE)
            {
                gu16ColdEffluentStepDelay = 0;
                gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����
                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_COLD);
            }
            break;

        default :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;
            Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����

            Set_EffluentTime(EFFLUENT_ID_COLD, 0);
            Set_EffluentHz(EFFLUENT_ID_COLD, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_COLD);
            break;
    }
}


/// @brief      �ü� ���� ���� �Լ�
/// @details    �ü� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void ColdEffluentStop(void)
{
    gu16ColdEffluentStepDelay = 0;
    gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;
    Set_FlowMeterErrorCheckStart(CLEAR);      // �������� ���� ���� ����

    Set_EffluentTime(EFFLUENT_ID_COLD, 0);
    Set_EffluentHz(EFFLUENT_ID_COLD, 0);

    Set_ClearEffluentData(EFFLUENT_ID_COLD);
}


/// @brief      �¼� ���� ���� �Լ�
/// @details    �¼� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void HotEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8HotEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // ���� ����
            Set_HotCutOffCheckStart(SET);      // �������� ���� ���� ����
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // ���� ����
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_HOT);

            if (mu8EffluentEnd == TRUE)
            {
                gu16HotEffluentStepDelay = 0;
                gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_HotCutOffCheckStart(CLEAR);      // �������� ���� ���� ����

                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_HOT);
            }
            break;

        default :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

            Set_HotCutOffCheckStart(CLEAR);      // �������� ���� ���� ����

            Set_EffluentTime(EFFLUENT_ID_HOT, 0);
            Set_EffluentHz(EFFLUENT_ID_HOT, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_HOT);
            break;
    }
}


/// @brief      �¼� ���� ���� �Լ�
/// @details    �¼� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void HotEffluentStop(void)
{
    gu16HotEffluentStepDelay = 0;
    gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

    Set_HotCutOffCheckStart(CLEAR);      // �������� ���� ���� ����

    Set_EffluentTime(EFFLUENT_ID_HOT, 0);
    Set_EffluentHz(EFFLUENT_ID_HOT, 0);

    Set_ClearEffluentData(EFFLUENT_ID_HOT);
}


/// @brief      ���� ������ ���� �ܰ� ���� �Լ�
/// @details    ID�� �ش��ϴ� �� ������ ���� ���� �߿� ���� ���� �ܰ踦 �����ؼ� ���۽�Ű���� �� �� ����Ѵ�
/// @param      mu8ID : �� ���� ID
///             mu8Step : �����Ϸ��� �ϴ� ���� ���� �ܰ�(0~10�ܰ�)
/// @return     void
void Set_EffluentStep(U8 mu8ID, U8 mu8Step)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        gu16ColdEffluentStepDelay = 0;
        gu8ColdEffluentStep = mu8Step;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        gu16HotEffluentStepDelay = 0;
        gu8HotEffluentStep = mu8Step;
    }
    else if (mu8ID == EFFLUENT_ID_ROOM)
    {
        gu16RoomEffluentStepDelay = 0;
        gu8RoomEffluentStep = mu8Step;
    }
}


/// @brief      ���� ������ �ܰ� Ȯ�� �Լ�
/// @details    ���� ������ ���� �ܰ踦 Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : �� ���� ID
/// @return     return : ���� ���� �ܰ�
U8 Get_EffluentStep(U8 mu8ID)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        return  gu8ColdEffluentStep;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        return  gu8HotEffluentStep;
    }
    else if (mu8ID == EFFLUENT_ID_ROOM)
    {
        return  gu8RoomEffluentStep;
    }
    else
    {
        return  0;
    }
}



// Lib Effluent Module *********************************************************************************

/// @brief      Lib Effluent Module Initilaize
/// @details    ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_Effluent_Module_Initialize(void)
{
    Lib_Effluent_Initialize();
}


/// @brief      Lib Effluent Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Effluent ���� ��� Library
/// @param      void
/// @return     void
void Lib_Effluent_Module_1ms_Control(void)
{
    EffluentControlTimer();
}


/// @brief      Lib BoEffluent Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Effluent ���� ��� Library
/// @param      void
/// @return     void
void Lib_Effluent_Module_Control(void)
{
    U8 mu8EffluentStatus = 0;

    mu8EffluentStatus = Get_OperatingStatus();

    if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
    {
        Lib_Effluent_Control();
    }
}


