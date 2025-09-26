/// @file     Lib_ADConvert.h
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    AD Convert function control file

#ifndef _LIB_AD_CONVERT_H_
#define _LIB_AD_CONVERT_H_


#if (ADC_USE == USE)

// Channel별 사용 여부를 설정 : USE = 1, NO USE = 0
#define AD_CH_0_USE                                 USE
#define AD_CH_1_USE                                 USE
#define AD_CH_2_USE                                 USE
#define AD_CH_3_USE                                 USE
#define AD_CH_4_USE                                 USE
#define AD_CH_5_USE                                 USE
#define AD_CH_6_USE                                 USE
#define AD_CH_7_USE                                 USE
#define AD_CH_8_USE                                 USE
#define AD_CH_9_USE                                 USE
#define AD_CH_10_USE                                USE
#define AD_CH_11_USE                                USE
#define AD_CH_12_USE                                USE
#define AD_CH_13_USE                                NO_USE
#define AD_CH_14_USE                                NO_USE
#define AD_CH_15_USE                                NO_USE
#define AD_CH_16_USE                                NO_USE
#define AD_CH_17_USE                                NO_USE      // Wi-Fi Uart Port 사용금지
#define AD_CH_18_USE                                NO_USE
#define AD_CH_19_USE                                USE
#define	AD_CH_20_USE                                NO_USE
#define	AD_CH_21_USE                                NO_USE
#define	AD_CH_22_USE                                NO_USE
#define	AD_CH_23_USE                                NO_USE
#define	AD_CH_24_USE                                NO_USE
#define	AD_CH_25_USE                                NO_USE


// ADC Channel 순서별 AD 측정하는 값 ID 배정, 미사용은 DUMMY_(숫자)로 자리만 배치
typedef enum
{
    ADC_ID_V_FB = 0,                                // 0 :
    ADC_ID_TEM_C_FB,                                // 1 :
    ADC_ID_TH_AIR,                                  // 2 :
    ADC_ID_TH_HOT_IN,                               // 3 :
    ADC_ID_TH_HOT_OUT,                              // 4 :
    ADC_ID_TH_HEATSINK,                             // 5 :
    ADC_ID_TH_COOL,                                 // 6 :
    ADC_ID_TH_HOT_BODY,                             // 7 :
    ADC_ID_FAN,                                     // 8 :
    ADC_ID_TH_ROOM,                                 // 9 :
    ADC_ID_LEAK_SENSOR,                             // 10 :
    ADC_ID_DUMMY_2,                                 // 11 :
    ADC_ID_DUMMY_3,                                 // 12 :
    ADC_ID_DUMMY_4,                                 // 13 :
    ADC_ID_DUMMY_5,                                 // 14 :
    ADC_ID_DUMMY_6,                                 // 15 :
    ADC_ID_DUMMY_7,                                 // 16 :
    ADC_ID_DUMMY_8,                                 // 17 :
    ADC_ID_DUMMY_9,                                 // 18 :
    ADC_ID_HEATER_V,                                // 19 :
    ADC_ID_DUMMY_10,                                // 20 :
    ADC_ID_DUMMY_11,                                // 21 :
    ADC_ID_DUMMY_12,                                // 22 :
    ADC_ID_DUMMY_13,                                // 23 :
    ADC_ID_DUMMY_14,                                // 24 :
    ADC_ID_DUMMY_15,                                // 25 :
    ADC_ID_MAX                                      // 26 Max, 26이 넘으면 안됨
} EADC_DataID_T;


// Channel별 평균을 계산하는 함수를 함수 포인터 배정. 미사용은 0으로 세팅
// define 하는 함수의 형태는 "exFunction(U8 mu8ID, U16 mu16ExParam)" 이다.
// #define AD_CH_0_AVERAGE_FUCTION                  exFunction
#define AD_AVERAGE_FUNCTION                         Average_ADC

#define ADC_Start()                                 R_Config_ADC_Start();
#define ADC_Stop()                                  R_Config_ADC_Stop();

#define AD_CONV_8BIT                                0
#define AD_CONV_10BIT                               1
#define AD_CONV_12BIT                               2
#define AD_CH_CONV_BIT                              AD_CONV_10BIT   // AD Convert의 데이터 형 : 0(8bit Convert), 1(10bit Convert), 2(12bit Convert)


// Channel Num - 변경하지 말 것
#define AD_CH_0                                     0
#define AD_CH_1                                     1
#define AD_CH_2                                     2
#define AD_CH_3                                     3
#define AD_CH_4                                     4
#define AD_CH_5                                     5
#define AD_CH_6                                     6
#define AD_CH_7                                     7
#define AD_CH_8                                     8
#define AD_CH_9                                     9
#define AD_CH_10                                    10
#define AD_CH_11                                    11
#define AD_CH_12                                    12
#define AD_CH_13                                    13
#define AD_CH_14                                    14
#define AD_CH_15                                    15
#define AD_CH_16                                    16
#define AD_CH_17                                    17        // Wi-Fi Uart Port 사용금지
#define AD_CH_18                                    18
#define AD_CH_19                                    19
#define	AD_CH_20                                    20
#define	AD_CH_21                                    21
#define	AD_CH_22                                    22
#define	AD_CH_23                                    23
#define	AD_CH_24                                    24
#define	AD_CH_25                                    25
#define AD_CH_MAX                                   26

// ADC bit 설정
#define AD_CH_CONV_12BIT                            4
#define AD_CH_CONV_10BIT                            6
#define AD_CH_CONV_8BIT                             8

#if (AD_CH_CONV_BIT == AD_CONV_12BIT)    // 12bit Convert
#define AD_CONV_MAX                                 4095
#elif (AD_CH_CONV_BIT == AD_CONV_10BIT)  // 10bit Convert
#define AD_CONV_MAX                                 1023
#else                           // 8bit Convert
#define AD_CONV_MAX                                 255
#endif

#define AD_CONVERT_TIME_PERIOD                      1           // 1ms@1ms

void AD_Convert_Initialize(void);
void ADC_ControlTimer(void);
void Set_ADC_EndStatus(U8 mu8Value);
U8 Get_ADC_EndStatus(void);
void ADC_CheckStart(U8 mu8AD_Channel);
U16 Read_ADC_Operation(U8 mu8Resolution);
void Read_ADC_Channel(U8 mu8Channel);
void ADC_ChannelChangeStart(void);
void AD_Convert_Control(void);

#endif


void Lib_ADC_Module_Initialize(void);
void Lib_ADC_Module_1ms_Control(void);
void Lib_ADC_Module_Control(void);

#endif

