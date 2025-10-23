/// @file     Hal_TDS.c
/// @date     2025/03/13
/// @author   Jaejin Ham
/// @brief    TDS 동작 제어 file

#include "Global_Header.h"

#if (TDS_IN_USE == USE)
U8 gu8TDS_In_ControlTimer = 0;              /// @brief  TDS In 센서 제어 주기 타이머
U8 gu8TDS_In_Check_Start = 0;               /// @brief  TDS In 센서 감지 시작 상태값 - 0(미체크), 1(체크)

U16 gu16TDS_In_Pulse = 0;                   /// @brief  TDS In 센서 감지 신호 pulse 개수
U16 gu16Cnt100ms_TDS_In = 0;                /// @brief  TDS In 센서 100ms 감지 시간 타이머
U16 gu16TDS_In_Data = 0;                    /// @brief  TDS In 센서 감지값
#endif

#if (TDS_OUT_USE == USE)
U8 gu8TDS_Out_ControlTimer = 0;             /// @brief  TDS Out 센서 제어 주기 타이머
U8 gu8TDS_Out_CheckControlTimer = 0;        /// @brief  TDS Out 체크 동작 제어 시간 타이머
U16 gu16TDS_Out_CheckTimer = 0;             /// @brief  TDS Out 상태값을 체크 시간 타이머

U8 gu8TDS_Out_Check_Start = 0;              /// @brief  TDS Out 감지 시작 상태값 - 0(미체크), 1(체크)

U16 gu16TDS_Out_TempResult = 0;             /// @brief  TDS Out 온도 상태값
U16 gu16TDS_Out_Data = 0;                   /// @brief  TDS Out 센서 감지값
#endif


#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )

/// @brief      TDS 관련값 관리 자료형
/// @details    TDS 관련 제어값들을 데이터 자료형으로 구성한다
typedef struct {
    U16 au16Average[TDS_ID_MAX_COUNT];
    U32 au32Sum[TDS_ID_MAX_COUNT];
    U8  au8Count[TDS_ID_MAX_COUNT];
    U16 au16Max[TDS_ID_MAX_COUNT];
    U16 au16Min[TDS_ID_MAX_COUNT];
}   STDS_Data_T;

STDS_Data_T   STDS_Data;


/// @brief      TDS Data Initialize
/// @details    TDS 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_TDS_Initialize(void)
{
    U8 mu8TDS_Buf = 0;

    MEMSET( (void __FAR *)&STDS_Data, 0x00, sizeof(STDS_Data_T) );

    // TDS 값 중 Min값 초기화
    for (mu8TDS_Buf = 1 ; mu8TDS_Buf < TDS_ID_MAX_COUNT ; mu8TDS_Buf++)
    {
        STDS_Data.au16Min[mu8TDS_Buf] = TDS_DATA_MAX;
    }

#if (TDS_IN_USE == USE)
    gu8TDS_In_ControlTimer = 0;
    gu8TDS_In_Check_Start = 0;

    gu16TDS_In_Pulse = 0;
    gu16Cnt100ms_TDS_In = 0;

    gu16TDS_In_Data = 0;
#endif

#if (TDS_OUT_USE == USE)
    gu8TDS_Out_ControlTimer = 0;
    gu8TDS_Out_CheckControlTimer = 0;
    gu16TDS_Out_CheckTimer = 0;          // TDS를 체크하는 Timer

    gu8TDS_Out_Check_Start = 0;

    gu16TDS_Out_TempResult = 0;
    gu16TDS_Out_Data = 0;
#endif
}

#endif


#if (TDS_IN_USE == USE)
/// @brief      TDS In control Timer(@1ms interrupt)
/// @details    TDS In 체크 컨트롤 시간 카운트를 한다
/// @param      void
/// @return     void
void TDS_In_ControlTimer(void)
{
    if (gu8TDS_In_ControlTimer < TDS_IN_CONTROL_TIME_PERIOD)
    {
        gu8TDS_In_ControlTimer++;
    }
}


/// @brief      TDS In Check 시작 처리 함수
/// @details    TDS In 체크 시작 여부를 설정한다
/// @param      mu8OnOff : TDS IN 제어 시작 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_TDS_In_Check_Start(U8 mu8OnOff)
{
    gu8TDS_In_Check_Start = mu8OnOff;

    if (mu8OnOff)
    {
        TURN_ON_TDS_IN();
        TDS_IN_INTERRUPT_START();
    }
    else
    {
        TURN_OFF_TDS_IN();
        TDS_IN_INTERRUPT_STOP();
    }
}


/// @brief      TDS In Check 동작 중 확인 함수
/// @details    TDS In Check 동작 중인지 확인하여 상태를 반환한다
/// @param      void
/// @return     return : TDS In 체크 제어 상태 - 0(OFF), 1(ON)
U8 Get_TDS_In_Check_Start(void)
{
    if (gu8TDS_In_Check_Start)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      TDS In Interrupt(@External interrupt)
/// @details    TDS In 센싱한 인터럽트 펄스를 카운트 한다
/// @param      void
/// @return     void
void TDS_In_Interrupt(void)
{
    if (gu8TDS_In_Check_Start)
    {
        if (gu16TDS_In_Pulse < 6000)
        {
            ++gu16TDS_In_Pulse;
        }
    }
}


/// @brief      TDS In Control Function(@while)
/// @details    TDS In 체크를 제어한다
/// @param      void
/// @return     void
void TDS_In_Control(void)
{
    if (gu8TDS_In_ControlTimer >= TDS_IN_CONTROL_TIME_PERIOD)
    {
        gu8TDS_In_ControlTimer = 0;

        if(gu8TDS_In_Check_Start)
        {
            if (gu16Cnt100ms_TDS_In < 100)
            {
                gu16Cnt100ms_TDS_In++;
            }
        }
        else
        {
            gu16Cnt100ms_TDS_In = 0;
            gu16TDS_In_Pulse = 0;
        }

        // 입력 주파수 계산
        if (gu16Cnt100ms_TDS_In >= 100)
        {
            gu16Cnt100ms_TDS_In = 0;

            TDS_In_FACTOR();

            gu16TDS_In_Pulse = 0;
        }
    }
}


/// @brief      TDS Factor Function
/// @details    TDS In 데이터를 산출한다
/// @param      void
/// @return     void
void TDS_In_FACTOR(void)
{
    U16 mu16Pulse1sec = 0U;
    D64 md64SquareX = 0.0f;
    D64 md64SquareX2 = 0.0f;
    D64 md64SquareX3 = 0.0f;

    U16 mu16TDS_In_Temp_AD = 0;

    U8 mu8CountBuf = 0;

    mu16Pulse1sec = gu16TDS_In_Pulse * (10U);     // 100ms동안 입력된 Signal로 1초 데이터 추정 연산

    md64SquareX = (D64)(mu16Pulse1sec);
    md64SquareX2 = ((D64)(mu16Pulse1sec) * (D64)(mu16Pulse1sec));
    md64SquareX3 = ((D64)(mu16Pulse1sec) * (D64)(mu16Pulse1sec) * (D64)(mu16Pulse1sec));

    mu16TDS_In_Temp_AD = Get_ADC_Data(TDS_IN_THERMISTOR_ADC_ID);

    if (mu16TDS_In_Temp_AD < TDS_IN_TEMP6_0)
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.0001f * md64SquareX2) + (0.4091f * md64SquareX) + 0.2205f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP6_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP9_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.0001f * md64SquareX2) + (0.3832f * md64SquareX) + 0.1762f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP9_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP11_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00008f * md64SquareX2) + (0.3462f * md64SquareX) + 0.1615f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP11_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP14_0))
    {
        gu16TDS_In_Data = (U16)((0.00000003f * md64SquareX3) + (0.00006f * md64SquareX2) + (0.3284f * md64SquareX) - 0.6361f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP14_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP16_0))
    {
        gu16TDS_In_Data = (U16)((0.00000004f * md64SquareX3) + (0.00003f * md64SquareX2) + (0.3131f * md64SquareX) - 1.4181f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP16_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP19_0))
    {
        gu16TDS_In_Data = (U16)((0.00000003f * md64SquareX3) + (0.00005f * md64SquareX2) + (0.2899f * md64SquareX) - 0.8228f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP19_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP21_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00006f * md64SquareX2) + (0.2683f * md64SquareX) - 0.155f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP21_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP24_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00005f * md64SquareX2) + (0.2532f * md64SquareX) - 0.1544f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP24_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP26_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00005f * md64SquareX2) + (0.2398f * md64SquareX) - 0.1511f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP26_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP29_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00004f * md64SquareX2) + (0.2235f * md64SquareX) + 0.0222f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP29_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP31_0))
    {
        gu16TDS_In_Data = (U16)((0.00000002f * md64SquareX3) + (0.00004f * md64SquareX2) + (0.2095f * md64SquareX) + 0.1505f);
    }
    else if ((mu16TDS_In_Temp_AD >= TDS_IN_TEMP31_0) && (mu16TDS_In_Temp_AD < TDS_IN_TEMP34_0))
    {
        gu16TDS_In_Data = (U16)((0.00000003f * md64SquareX3) + (0.00001f * md64SquareX2) + (0.2118f * md64SquareX) - 3.0446f);
    }
    else if (mu16TDS_In_Temp_AD >= TDS_IN_TEMP34_0)
    {
        gu16TDS_In_Data = (U16)((0.00000003f * md64SquareX3) - (0.000009f * md64SquareX2) + (0.2217f * md64SquareX) - 6.3495f);
    }

    // 온도에 따른 TDS값 보정
    if (mu16TDS_In_Temp_AD < TDS_IN_TEMP7_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.83);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP8_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.82);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP9_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.80);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP10_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.87);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP11_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP12_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP13_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP14_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.80);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP15_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP16_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.82);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP17_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP18_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.82);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP19_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.80);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP20_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP21_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP22_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP23_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP24_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.82);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP25_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP26_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.82);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP27_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP28_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP29_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.83);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP30_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP31_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP32_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.90);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP33_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.85);
    }
    else if (mu16TDS_In_Temp_AD < TDS_IN_TEMP34_0)
    {
        gu16TDS_In_Data = (U16)(gu16TDS_In_Data * 0.83);
    }
    else
    {
    }

    if (!gu16TDS_In_Pulse)
    {
        gu16TDS_In_Data = 0;
    }

    STDS_Data.au32Sum[TDS_IN_SENSOR_ID] += gu16TDS_In_Data;

    if (gu16TDS_In_Data > STDS_Data.au16Max[TDS_IN_SENSOR_ID])
    {
        STDS_Data.au16Max[TDS_IN_SENSOR_ID] = gu16TDS_In_Data;      // 최대값 저장
    }
    if (gu16TDS_In_Data < STDS_Data.au16Min[TDS_IN_SENSOR_ID])
    {
        STDS_Data.au16Min[TDS_IN_SENSOR_ID] = gu16TDS_In_Data;      // 최소값 저장
    }

    mu8CountBuf = TDS_IN_AVERAGE_COUNT + 2;

    STDS_Data.au8Count[TDS_IN_SENSOR_ID]++;

    if (STDS_Data.au8Count[TDS_IN_SENSOR_ID] >= mu8CountBuf)
    {
        STDS_Data.au16Average[TDS_IN_SENSOR_ID] = (U16)( ( STDS_Data.au32Sum[TDS_IN_SENSOR_ID] -
                                                  ((U32)(STDS_Data.au16Max[TDS_IN_SENSOR_ID]) + (U32)(STDS_Data.au16Min[TDS_IN_SENSOR_ID])) ) / TDS_IN_AVERAGE_COUNT );

        STDS_Data.au8Count[TDS_IN_SENSOR_ID] = 0;
        STDS_Data.au32Sum[TDS_IN_SENSOR_ID] = 0;
        STDS_Data.au16Max[TDS_IN_SENSOR_ID] = 0;
        STDS_Data.au16Min[TDS_IN_SENSOR_ID] = TDS_DATA_MAX;
    }
}


/// @brief      TDS In 값 확인 함수
/// @details    TDS In 센싱값을 확인하여 반환한다
/// @param      void
/// @return     mu16Return : 입수부 TDS 센싱값
U16 Get_TDS_In_Data(void)
{
    U16 mu16Return = 0;

    mu16Return = STDS_Data.au16Average[TDS_IN_SENSOR_ID];

    return  mu16Return;
}

#endif


#if (TDS_OUT_USE == USE)

/// @brief      TDS Out Power control Timer(@100us interrupt)
/// @details    TDS Out 센서의 전원 제어를 한다
/// @param      void
/// @return     void
void TDS_Out_ControlTimer(void)
{
    if (gu8TDS_Out_ControlTimer < TDS_OUT_CONTROL_TIME_PERIOD)
    {
        gu8TDS_Out_ControlTimer++;
    }

    TDS_Out_PowerControl();
}


/// @brief      TDS Out Check control Timer(@1ms interrupt)
/// @details    TDS Out 체크 제어 시간을 카운트 한다
/// @param      void
/// @return     void
void TDS_Out_CheckControlTimer(void)
{
    if (gu8TDS_Out_CheckControlTimer < TDS_OUT_CHECK_CONTROL_TIME_PERIOD)
    {
        gu8TDS_Out_CheckControlTimer++;
    }
}


/// @brief      TDS Out Check 시작 처리 함수
/// @details    TDS Out 체크 시작 여부를 설정한다
/// @param      mu8OnOff : TDS OUT 제어 시작 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_TDS_Out_Check_Start(U8 mu8OnOff)
{
    gu8TDS_Out_Check_Start = mu8OnOff;
    gu16TDS_Out_CheckTimer = 50;
}


/// @brief      TDS Out Check 동작 중 확인 함수
/// @details    TDS Out Check 동작 중인지 확인하여 상태를 반환한다
/// @param      void
/// @return     return : TDS Out 체크 제어 상태 - 0(OFF), 1(ON)
U8 Get_TDS_Out_Check_Start(void)
{
    if (gu8TDS_Out_Check_Start)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      TDS Out 센서 전원 제어 함수
/// @brief      TDS Out 센서 전원 제어를 한다
/// @param      void
/// @return     void
void TDS_Out_PowerControl(void)
{
    if (gu8TDS_Out_ControlTimer >= TDS_OUT_CONTROL_TIME_PERIOD)
    {
        gu8TDS_Out_ControlTimer = 0;

        if (gu8TDS_Out_Check_Start)
        {
            gu16TDS_Out_CheckTimer--;

            if (gu16TDS_Out_CheckTimer == 0)
            {
                gu16TDS_Out_CheckTimer = 50;
            }

            if (gu16TDS_Out_CheckTimer == 20 || gu16TDS_Out_CheckTimer == 19 ||
                gu16TDS_Out_CheckTimer == 18 || gu16TDS_Out_CheckTimer == 17 || gu16TDS_Out_CheckTimer == 16)
            {
                TURN_ON_TDS_OUT_1();
                TURN_OFF_TDS_OUT_2();
            }
            else if (gu16TDS_Out_CheckTimer == 15 || gu16TDS_Out_CheckTimer == 14 ||
                    gu16TDS_Out_CheckTimer == 13 || gu16TDS_Out_CheckTimer == 12 || gu16TDS_Out_CheckTimer == 11)
            {
                TURN_OFF_TDS_OUT_1();
                TURN_ON_TDS_OUT_2();
            }
            else
            {
                TURN_OFF_TDS_OUT_1();
                TURN_OFF_TDS_OUT_2();
            }
        }
        else
        {
            gu16TDS_Out_CheckTimer = 50;
            TURN_OFF_TDS_OUT_1();
            TURN_OFF_TDS_OUT_2();
        }

        TDS_Out_ADC_CheckStartControl();
    }
}


/// @brief      TDS Out Sensor AD Check 제어 함수
/// @details    TDS Out 센서 AD값을 체크 제어한다
/// @param      void
/// @return     void
void TDS_Out_ADC_CheckStartControl(void)
{
    if(gu16TDS_Out_CheckTimer == 14 || gu16TDS_Out_CheckTimer == 13 || gu16TDS_Out_CheckTimer == 12)
    {
        Set_Hal_ADC_Check(TDS_OUT_SENSOR_ADC_ID, 1);
    }
    else
    {
        Set_Hal_ADC_Check(TDS_OUT_SENSOR_ADC_ID, 0);
    }
}


/// @brief      TDS Out 계산 루틴(@While)
/// @details    TDS Out 센싱값을 산출한다
/// @param      void
/// @return     void
void TDS_Out_Calculation(void)
{
    D64 md64TDSOutTemp1_Voltage = 0;
    D64 md64TDSOutTemp2_Voltage = 0;
    D64 md64TDSOutTemp_Resistance = 0;

    U16 mu16AD_TDS_Out_Temp = 0;
    U16 mu16AD_TDS_Out = 0;
    D64 md64TDSOut1_Voltage = 0;
    D64 md64TDSOut2_Voltage = 0;
    D64 md64TDSOut_Resistance = 0;

    U8 mu8CountBuf = 0;

    if (gu8TDS_Out_CheckControlTimer >= TDS_OUT_CHECK_CONTROL_TIME_PERIOD)
    {
        gu8TDS_Out_CheckControlTimer = 0;

        mu16AD_TDS_Out_Temp = Get_ADC_Data(TDS_OUT_THERMISTOR_ADC_ID);

        md64TDSOutTemp1_Voltage = (1024 - mu16AD_TDS_Out_Temp) * 0.0048828125f;
        md64TDSOutTemp2_Voltage = mu16AD_TDS_Out_Temp * 0.0048828125f;
        md64TDSOutTemp_Resistance = (md64TDSOutTemp1_Voltage / (md64TDSOutTemp2_Voltage / 10000));
        gu16TDS_Out_TempResult = (U16)(fabs((((1 / (pow((md64TDSOutTemp_Resistance / 1000.0f), 0.09305f))) * 369.8154f) - 273.15f) * 10.0f));

        if (mu16AD_TDS_Out_Temp < 5)
        {
            gu16TDS_Out_TempResult = 0;
        }


        if (gu8TDS_Out_Check_Start)
        {
            mu16AD_TDS_Out = Get_ADC_Data(TDS_OUT_SENSOR_ADC_ID);

            md64TDSOut1_Voltage = mu16AD_TDS_Out * 0.0048828125f;
            md64TDSOut2_Voltage = (1024 - mu16AD_TDS_Out) * 0.0048828125f;
            md64TDSOut_Resistance = (md64TDSOut2_Voltage / (md64TDSOut1_Voltage / 1088));
            gu16TDS_Out_Data = (U16)(fabs((((((1 / ((-16.3984575f + md64TDSOut_Resistance) * (-0.0000150134f + 0.0000000592714f * ((gu16TDS_Out_TempResult / 10.0f) + 273.15f)))) + 0.694408181818182f) * 0.798594f) / 1.36198f))));  // Plate Type

            if (mu16AD_TDS_Out < 8)
            {
                gu16TDS_Out_Data = 0;
            }

            STDS_Data.au32Sum[TDS_OUT_SENSOR_ID] += gu16TDS_Out_Data;

            if (gu16TDS_Out_Data > STDS_Data.au16Max[TDS_OUT_SENSOR_ID])
            {
                STDS_Data.au16Max[TDS_OUT_SENSOR_ID] = gu16TDS_Out_Data;      // 최대값 저장
            }

            if (gu16TDS_Out_Data < STDS_Data.au16Min[TDS_OUT_SENSOR_ID])
            {
                STDS_Data.au16Min[TDS_OUT_SENSOR_ID] = gu16TDS_Out_Data;      // 최소값 저장
            }

            mu8CountBuf = TDS_OUT_AVERAGE_COUNT + 2;

            STDS_Data.au8Count[TDS_OUT_SENSOR_ID]++;

            if (STDS_Data.au8Count[TDS_OUT_SENSOR_ID] >= mu8CountBuf)
            {
                STDS_Data.au16Average[TDS_OUT_SENSOR_ID] = (U16)( ( STDS_Data.au32Sum[TDS_OUT_SENSOR_ID] -
                                                           ((U32)(STDS_Data.au16Max[TDS_OUT_SENSOR_ID]) + (U32)(STDS_Data.au16Min[TDS_OUT_SENSOR_ID])) ) / TDS_OUT_AVERAGE_COUNT);

                STDS_Data.au8Count[TDS_OUT_SENSOR_ID] = 0;
                STDS_Data.au32Sum[TDS_OUT_SENSOR_ID] = 0;
                STDS_Data.au16Max[TDS_OUT_SENSOR_ID] = 0;
                STDS_Data.au16Min[TDS_OUT_SENSOR_ID] = TDS_DATA_MAX;
            }
        }
    }
}


/// @brief      TDS Out 값 확인 함수
/// @details    TDS Out 센싱값을 확인하여 반환한다
/// @param      void
/// @return     mu16Return : 출수부 TDS 센싱값
U16 Get_TDS_Out_Data(void)
{
    U16 mu16Return = 0;

    mu16Return = STDS_Data.au16Average[TDS_OUT_SENSOR_ID];

    return  mu16Return;
}

#endif



// Hal TDS Module ********************************************************************************************

/// @brief      Hal TDS Initilaize Module
/// @details    TDS 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_TDS_Module_Initialize(void)
{
#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )
    Hal_TDS_Initialize();                   // TDS 관련 초기화
#endif
}


/// @brief      Hal TDS Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 TDS 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_TDS_Module_1ms_Control(void)
{
#if (TDS_IN_USE == USE)
    TDS_In_ControlTimer();                  // TDS In 센서 제어 시간 Counter
#endif

#if (TDS_OUT_USE == USE)
    TDS_Out_CheckControlTimer();            // TDS Out 센서 체크 제어  시간 Counter
#endif
}


/// @brief      Hal TDS Module in 100us Interrupt
/// @details    100us Interrupt 안에서 TDS 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_TDS_Module_100us_Control(void)
{
#if (TDS_OUT_USE == USE)
    TDS_Out_ControlTimer();                 // TDS Out 센서 제어 시간 Counter
#endif
}


/// @brief      Hal TDS Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 TDS 센서들을 제어한다
/// @param      void
/// @return     void
void Hal_TDS_Module_Control(void)
{
#if (TDS_IN_USE == USE)
    TDS_In_Control();                       // TDS IN Control 함수
#endif

#if (TDS_OUT_USE == USE)
    TDS_Out_Calculation();                  // TDS OUT 계산 함수
#endif
}


