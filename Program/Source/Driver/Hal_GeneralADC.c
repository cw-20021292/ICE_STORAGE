/// @file     Hal_GeneralADC.c
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    통합 ADC Control file


#include "Global_Header.h"


#if (ADC_USE == USE)                            // ADC 부하가 1개라도 있는 경우

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
U8 gu8CoolTH_ControlTimer = 0;                  /// @brief  냉수 온도 센서 전원 제어 타이머 카운터
U16 gu16Cool1TH_PowerControlTimer = 0;          /// @brief  냉수 온도 센서 1 전원 제어 타이머
#if (COOL_THERMISTOR_COUNT == 2)
U16 gu16Cool2TH_PowerControlTimer = 0;          /// @brief  냉수 온도 센서 2 전원 제어 타이머
#endif
U16 gu16BootCoolTempCheckTimer = 0;             /// @brief  냉수 온도 센서 부팅 동작 전원 제어 타이머
U8 gu8CoolTH_PowerControlMode = 0;              /// @brief  냉수 온도 센서 전원 제어 모드
#endif

/// @brief      ADC 평균 산출 산정 데이터 수 테이블
/// @details    ADC의 평균을 계산하기 위한 AD값의 개수를 정하여 놓는다
U8 au8ADCAverageCountSet[26] = { ADC_AVERAGE_COUNT_0,
                                 ADC_AVERAGE_COUNT_1,
                                 ADC_AVERAGE_COUNT_2,
                                 ADC_AVERAGE_COUNT_3,
                                 ADC_AVERAGE_COUNT_4,
                                 ADC_AVERAGE_COUNT_5,
                                 ADC_AVERAGE_COUNT_6,
                                 ADC_AVERAGE_COUNT_7,
                                 ADC_AVERAGE_COUNT_8,
                                 ADC_AVERAGE_COUNT_9,
                                 ADC_AVERAGE_COUNT_10,
                                 ADC_AVERAGE_COUNT_11,
                                 ADC_AVERAGE_COUNT_12,
                                 ADC_AVERAGE_COUNT_13,
                                 ADC_AVERAGE_COUNT_14,
                                 ADC_AVERAGE_COUNT_15,
                                 ADC_AVERAGE_COUNT_16,
                                 ADC_AVERAGE_COUNT_17,
                                 ADC_AVERAGE_COUNT_18,
                                 ADC_AVERAGE_COUNT_19,
                                 ADC_AVERAGE_COUNT_20,
                                 ADC_AVERAGE_COUNT_21,
                                 ADC_AVERAGE_COUNT_22,
                                 ADC_AVERAGE_COUNT_23,
                                 ADC_AVERAGE_COUNT_24,
                                 ADC_AVERAGE_COUNT_25
};


/// @brief      ADC 평균 산출 산정 데이터 수 테이블
/// @details    ADC의 평균을 계산하기 위한 배열 열거형
typedef struct {
    U16 au16Average[AD_CH_MAX];
    U16 au16Sum[AD_CH_MAX];
    U8  au8Count[AD_CH_MAX];
    U16 au16Max[AD_CH_MAX];
    U16 au16Min[AD_CH_MAX];
    U8 au8CheckStart[AD_CH_MAX];
}   SADC_Data_T;

SADC_Data_T   SADC_Data;


/// @brief      ADC Data Initialize
/// @details    ADC와 관련된 모든 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_ADC_Initialize(void)
{
    U8 mu8ADC_Buf = 0;

    MEMSET( (void __FAR *)&SADC_Data, 0x00, sizeof(SADC_Data_T) );

    // AD min값 초기화
    for (mu8ADC_Buf = 0 ; mu8ADC_Buf < AD_CH_MAX ; mu8ADC_Buf++)
    {
        SADC_Data.au8CheckStart[mu8ADC_Buf] = SET;        // ADC Start 변수 1로 설정 - 0(Stop), 1(Start)
        SADC_Data.au16Min[mu8ADC_Buf] = ADC_DATA_MAX;
    }
}


/// @brief      AD값 평균 확인 함수
/// @details    ID에 해당하는 ADC 데이터의 평균값을 반환한다
/// @param      mu8Channel : 체크 요청하는 ADC Channel값
/// @return     return : 요청한 Thermistor ID의 AD값 평균값
U16 Get_ADC_Data(U8 mu8Channel)
{
    U16 mu16Return = 0;

    mu16Return = SADC_Data.au16Average[mu8Channel];

    return  mu16Return;
}


/// @brief      ADC Check Start Set function
/// @details    채널의 AD Convert 평균 계산 동작 시작을 설정한다
/// @param      mu8Channel : 체크하는 ADC channel값
///             mu8Start : 1(Check Start), 0(Check Stop)
/// @return     void
void Set_Hal_ADC_Check(U8 mu8Channel, U8 mu8Start)
{
    SADC_Data.au8CheckStart[mu8Channel] = mu8Start;
}


/// @brief      ADC All Check Start Set function
/// @details    모든 채널의 AD Convert 평균 계산 동작 시작을 설정한다
/// @param      void
/// @return     void
void Set_Hal_All_ADC_CheckStart(void)
{
    U8 mu8ADC_Buf = 0;

    for (mu8ADC_Buf = 0 ; mu8ADC_Buf < AD_CH_MAX ; mu8ADC_Buf++)
    {
        SADC_Data.au8CheckStart[mu8ADC_Buf] = SET;        // ADC Start 변수 1로 설정 - 0(Stop), 1(Start)
    }
}


/// @brief      AD Convert 평균 계산 함수
/// @details    AD값들의 평균을 계산한다
/// @param      mu8Channel : 평균 계산할 AD Channel
///             mu16AD_Value : AD 값
/// @return     void
void Average_ADC(U8 mu8Channel, U16 mu16AD_Value)
{
    U8 mu8CountBuf = 0;

    if (SADC_Data.au8CheckStart[mu8Channel] == 1)
    {       // ADC 체크 동작 중인 경우에만 평균 계산
        if (mu16AD_Value > SADC_Data.au16Max[mu8Channel])
        {
            SADC_Data.au16Max[mu8Channel] = mu16AD_Value;      // 최대값 저장
        }

        if (mu16AD_Value < SADC_Data.au16Min[mu8Channel])
        {
            SADC_Data.au16Min[mu8Channel] = mu16AD_Value;      // 최소값 저장
        }

        SADC_Data.au16Sum[mu8Channel] += mu16AD_Value;

        mu8CountBuf = au8ADCAverageCountSet[mu8Channel] + 2;

        SADC_Data.au8Count[mu8Channel]++;

        if (SADC_Data.au8Count[mu8Channel] >= mu8CountBuf)
        {     // 최대, 최소값을 제외한 10회 평균값 계산
            SADC_Data.au16Average[mu8Channel] = (U16)( ( SADC_Data.au16Sum[mu8Channel] - (SADC_Data.au16Max[mu8Channel] + SADC_Data.au16Min[mu8Channel]) ) / au8ADCAverageCountSet[mu8Channel] );

            SADC_Data.au16Sum[mu8Channel] = 0;
            SADC_Data.au8Count[mu8Channel] = 0;
            SADC_Data.au16Max[mu8Channel] = 0;
            SADC_Data.au16Min[mu8Channel] = ADC_DATA_MAX;
        }
    }
    else
    {       // ADC 체크 동작 중이 아닌 경우
        SADC_Data.au16Sum[mu8Channel] = 0;
        SADC_Data.au8Count[mu8Channel] = 0;
        SADC_Data.au16Max[mu8Channel] = 0;
        SADC_Data.au16Min[mu8Channel] = ADC_DATA_MAX;
    }
}


#if (COOL_THERMISTOR_POWER_CONTROL == USE)        // 냉수 온도 센서 전원 동작 제어시

/// @brief      Cool Thermistor Power Control Initialize
/// @details    냉수 온도 센서 전원 제어와 관련된 변수들을 초기화 한다
/// @param      void
/// @return     void
void CoolTH_ControlInitialize(void)
{
        gu8CoolTH_ControlTimer = 0;
        gu16Cool1TH_PowerControlTimer = 0;
#if (COOL_THERMISTOR_COUNT == 2)
        gu16Cool2TH_PowerControlTimer = 0;
#endif
        gu16BootCoolTempCheckTimer = 6000;                      // 전원 인가 후 온도 센서 측정 타이머 10분@100ms
        gu8CoolTH_PowerControlMode = COOL_TH_NORMAL_MODE;       // 냉수 온도 센서 전원 제어 일반 모드로 부팅
}


/// @brief      Cool Thermistor Power Control Timer(@1ms interrupt)
/// @details    1ms 인터럽트 루틴 안에서 냉수 온도 센서 전원 제어 타이머를 카운터한다
/// @param      void
/// @return     void
void CoolTH_ControlTimer(void)
{
    if (gu8CoolTH_ControlTimer < COOL_TH_POWER_CONTROL_TIME_PERIOD)
    {
        gu8CoolTH_ControlTimer++;
    }
}


/// @brief      Cool Thermistor Power Control 모드 설정 함수
/// @details    냉수 온도 센서 전원 제어와 관련된 동작 모드를 설정한다
/// @param      mu8Mode : 0(Normal), 1(Test Mode)
/// @return     void
void Set_CoolTH_PowerControl(U8 mu8Mode)
{
    gu8CoolTH_PowerControlMode = mu8Mode;
}


/// @brief      Cool Thermistor 1 Control 함수(100ms@1ms)
/// @details    냉수 온도 센서 1 전원을 모드에 따라 100ms 주기로 전원을 제어한다
/// @param      mu8ControlMode : 전원 제어할 모드
/// @return     void
void Cool1TH_Control(U8 mu8ControlMode)
{
    U16 mu16TempADBuff = 0;

    if (mu8ControlMode == TH_OFF_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 0;
        P_COOL1_TH_ON = OFF;
    }
    else if (mu8ControlMode == TH_TEST_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 10;      // AD값 Read 전 delay 없이 바로 측정되도록
        P_COOL1_TH_ON = ON;
    }
    else if (mu8ControlMode == TH_ERROR_MODE)
    {
        gu16Cool1TH_PowerControlTimer = 10;      // AD값 Read 전 delay 없이 바로 측정되도록
        P_COOL1_TH_ON = ON;
    }
    else
    {
        mu16TempADBuff = Get_Temp(ADC_ID_TH_COOL);

        if (mu16TempADBuff <= COOL1_TARGET_TEMP)
        {   // 냉수 온도가 목표 온도  이하가 된 경우 전원 제어 항시 ON
            if (gu16Cool1TH_PowerControlTimer <= 10)
            {   // 온도 측정 전 1초 Delay
                gu16Cool1TH_PowerControlTimer++;
            }

            P_COOL1_TH_ON = ON;
        }
        else
        {  // 냉수 온도가 목표 온도 보다 높은 경우
            gu16Cool1TH_PowerControlTimer++;

            if (gu16Cool1TH_PowerControlTimer >= 400)
            {   // 40초 이상시(40초 주기) Thermistor 전원 제어 초기화
                gu16Cool1TH_PowerControlTimer = 0;
                P_COOL1_TH_ON = OFF;
            }
            else if (gu16Cool1TH_PowerControlTimer >= 100)
            {   // 10초 ~ 40초 사이(30초간)는 Thermistor 전원 해제
                P_COOL1_TH_ON = OFF;
            }
            else
            {   // 10초간 Thermistor 전원 인가
                P_COOL1_TH_ON = ON;
            }
        }
    }
}


#if (COOL_THERMISTOR_COUNT == 2)
/// @brief      Cool Thermistor 2 Control 함수(100ms@1ms)
/// @details    냉수 온도 센서 2 전원을 모드에 따라 100ms 주기로 전원을 제어한다
/// @param      mu8ControlMode : 전원 제어할 모드
/// @return     void
void Cool2TH_Control(U8 mu8ControlMode)
{
    U16 mu16TempADBuff = 0;

    if (mu8ControlMode == TH_OFF_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 0;
        P_COOL2_TH_ON = OFF;
    }
    else if (mu8ControlMode == TH_TEST_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 10;      // AD값 Read 전 delay 없이 바로 측정되도록
        P_COOL2_TH_ON = ON;
    }
    else if (mu8ControlMode == TH_ERROR_MODE)
    {
        gu16Cool2TH_PowerControlTimer = 10;      // AD값 Read 전 delay 없이 바로 측정되도록
        P_COOL2_TH_ON = ON;
    }
    else
    {
        mu16TempADBuff = Get_Temp(ADC_ID_TH_COOL);

        if (mu16TempADBuff <= COOL2_TARGET_TEMP)
        {   // 냉수 온도가 목표 온도  이하가 된 경우 전원 제어 항시 ON
            if (gu16Cool2TH_PowerControlTimer <= 10)
            {   // 온도 측정 전 1초 Delay
                gu16Cool2TH_PowerControlTimer++;
            }

            P_COOL2_TH_ON = ON;
        }
        else
        {  // 냉수 온도가 목표 온도 보다 높은 경우
            gu16Cool2TH_PowerControlTimer++;

            if (gu16Cool2TH_PowerControlTimer >= 400)
            {   // 40초 이상시(40초 주기) Thermistor 전원 제어 초기화
                gu16Cool2TH_PowerControlTimer = 0;
                P_COOL2_TH_ON = OFF;
            }
            else if (gu16Cool2TH_PowerControlTimer >= 100)
            {   // 10초 ~ 40초 사이(30초간)는 Thermistor 전원 해제
                P_COOL2_TH_ON = OFF;
            }
            else
            {   // 10초간 Thermistor 전원 인가
                P_COOL2_TH_ON = ON;
            }
        }
    }
}

#endif


/// @brief      Cool Thermistor ADC 체크 동작 여부 제어 함수(@100ms)
/// @details    냉수 온도 센서의 전원 인가 중 ADC Read를 할 타이밍을 설정한다
/// @param      void
/// @return     void
void CoolTH_ADC_CheckStartControl(void)
{
    if (P_COOL1_TH_ON)
    {   // 냉수 온도 센서에 전원이 인가된 경우
        if ( (gu16BootCoolTempCheckTimer > 0) ||
             ( (gu16Cool1TH_PowerControlTimer >= 10) && (gu16Cool1TH_PowerControlTimer < 100) ) )
        {   // 부팅후 일정 시간 동안, 그 이후에는 온도 센서에 전원이 인가되고 1초가 경과한 뒤부터 측정
            Set_Hal_ADC_Check(ADC_ID_TH_COOL, 1);
        }
        else
        {
            Set_Hal_ADC_Check(ADC_ID_TH_COOL, 0);
        }
    }
    else
    {
        Set_Hal_ADC_Check(ADC_ID_TH_COOL, 0);
    }
#if (COOL_THERMISTOR_COUNT == 2)
    if (P_COOL2_TH_ON)
    {   // 냉수 온도 센서에 전원이 인가된 경우
        if ( (gu16BootCoolTempCheckTimer > 0) ||
             ( (gu16Cool1TH_PowerControlTimer >= 10) && (gu16Cool1TH_PowerControlTimer < 100) ) )
        {   // 부팅후 일정 시간 동안, 그 이후에는 온도 센서에 전원이 인가되고 1초가 경과한 뒤부터 측정
            Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 1);
        }
        else
        {
            Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 0);
        }
    }
    else
    {
        Set_Hal_ADC_Check(COOL_THERMISTOR_2_ID, 0);
    }
#endif
}


/// @brief      Cool Thermistor Power Control 함수(100ms@1ms)
/// @details    100ms 주기로 설정된 냉수 온도 센서 전원 제어 모드에 따라 전원 제어를 한다
/// @param      void
/// @return     void
void CoolTH_PowerControl(void)
{
    if (gu8CoolTH_ControlTimer >= COOL_TH_POWER_CONTROL_TIME_PERIOD)
    {
        gu8CoolTH_ControlTimer = 0;

        if (gu8CoolTH_PowerControlMode == COOL_TH_OFF_MODE)
        {   // 냉수 온도 센서 전원 제어 OFF시
            Cool1TH_Control(TH_OFF_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_OFF_MODE);
#endif
        }
        if (gu8CoolTH_PowerControlMode == COOL_TH_TEST_MODE)
        {   // 테스트 모드 동작시
            Cool1TH_Control(TH_TEST_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_TEST_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH_ALL_ERROR_CHECK_MODE)
        {   // 냉수 온도 센서 에러 모두 발생시
            Cool1TH_Control(TH_ERROR_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_ERROR_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH1_ERROR_CHECK_MODE)
        {   // 냉수 온도 센서 1 관련 에러 동작 중
            Cool1TH_Control(TH_ERROR_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_NORMAL_MODE);
#endif
        }
        else if (gu8CoolTH_PowerControlMode == COOL_TH2_ERROR_CHECK_MODE)
        {   // 냉수 온도 센서 1 관련 에러 동작 중
            Cool1TH_Control(TH_NORMAL_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
            Cool2TH_Control(TH_ERROR_MODE);
#endif
        }
        else
        {   // 일반 모드 제어
            if (gu16BootCoolTempCheckTimer)
            {   // 부팅 후 일정 시간 동안은 무조건 냉수 온도 센서 전원 인가
                gu16BootCoolTempCheckTimer--;

                gu16Cool1TH_PowerControlTimer = 0;
                P_COOL1_TH_ON = ON;

#if (COOL_THERMISTOR_COUNT == 2)
                gu16Cool2TH_PowerControlTimer = 0;
                P_COOL2_TH_ON = ON;
#endif
            }
            else
            {   // 부팅 후 일정 시간 경과 이후 온도 센서 전원 인가 제어
                Cool1TH_Control(TH_NORMAL_MODE);
#if (COOL_THERMISTOR_COUNT == 2)
                Cool2TH_Control(TH_NORMAL_MODE);
#endif
            }
        }
    }

    CoolTH_ADC_CheckStartControl();
}

#endif      // if - 냉수 온도 센서 전원 제어 동작시


#endif


// Hal ADC Module ********************************************************************************************

/// @brief      Hal ADC Initilaize Module
/// @details    Hal ADC 관련 데이터를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_ADC_Module_Initialize(void)
{
#if (ADC_USE == USE)
    Hal_ADC_Initialize();                   // ADC 데이터 모두 초기화
#endif
}



// Hal Cold Temp Sensor Module *******************************************************************************

/// @brief      Hal Cold Thermistor Initilaize Module
/// @details    냉수 온도 센서 전원 제어와 관련된 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_ColdTH_Module_Initialize(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
    CoolTH_ControlInitialize();             // 냉수 온도 센서 전원 제어 관련 초기화
#endif

#endif
}


/// @brief      Hal Cold Thermistor Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 냉수 온도 센서 전원 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_ColdTH_Module_1ms_Control(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
    CoolTH_ControlTimer();                  // 냉수 온도 센서 전원 제어 시간 Counter
#endif

#endif
}


/// @brief      Hal Cold Thermistor Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 냉수 온도 센서 전원 제어를 한다
/// @param      void
/// @return     void
void Hal_ColdTH_Module_Control(void)
{
#if (ADC_USE == USE)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
        CoolTH_PowerControl();                  // 냉수 온도 센서 전원 제어 함수
#endif

#endif
}

