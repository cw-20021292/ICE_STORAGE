/// @file     Hal_TDS.h
/// @date     2025/03/13
/// @author   Jaejin Ham
/// @brief    TDS 동작 제어 헤더 file

#ifndef _HAL_TDS_H_
#define _HAL_TDS_H_


/*  TDS 센싱 제어 Function 사용법
    1. TDS IN
      => TDS_In_Interrupt();                                    // TDS IN 체크를 위한 외부 Interrupt 루틴 안에 직접 써야 하는 함수(Basic_Loop.c 파일에 필요시 사용할 수 있도록 처리되어 있음)
      => Set_TDS_In_Check_Start(mu8OnOff);                      // TDS IN 체크 시작 On/Off 설정 - 0(OFF), 1(ON)
      => Get_TDS_In_Data();                                     // 입수부 TDS 센싱값  평균값 확인, mu8Return : 평균 데이터

    2. TDS OUT
      => Set_TDS_Out_Check_Start(mu8OnOff);                     // TDS OUT 체크 시작 On/Off 설정  - 0(OFF), 1(ON)
      => Get_TDS_Out_Data();                                    // 출수부 TDS 센싱값  평균값 확인, mu8Return : 평균 데이터

    사용 예)
    u8Return = Get_TDS_In_Data();           // TDS In 센싱값 확인
    u8Return = Get_TDS_Out_Data();          // TDS Out 센싱값 확인
*/


// TDS 센싱 관련 AD 체크값 ID
typedef enum
{
    TDS_ID_NONE = 0,                                    // 0 :
    TDS_ID_IN,                                          // 1 : 입수부 TDS 온도 센서 AD
    TDS_ID_OUT,                                         // 2 : 출수부 TDS 센서 AD
    TDS_ID_MAX                                          // 3 Max, 3이 넘으면 안됨. - TDS IN, OUT 1개씩만 설정
} ETDS_ID_T;

#define TDS_IN_USE                                      NO_USE  // 입수부 TDS 센서 사용 여부 - 0(No Use), 1(Use)
#define TDS_OUT_USE                                     NO_USE  // 입수부 TDS 센서 사용 여부 - 0(No Use), 1(Use)

#if (TDS_IN_USE == USE)
#define TDS_IN_THERMISTOR_ADC_ID                        ADC_ID_TH_TDS_IN    // TDS IN 온도 센서의 ADC ID
#define TDS_IN_SENSOR_ID                                TDS_ID_IN           // TDS IN 센서의 ID
#define TDS_IN_AVERAGE_COUNT                            10                  // TDS IN 값 평균 산출 데이터 수
#define VALUE_DEFINE_TDS_IN_DATA_MAX                    1024                // TDS IN 값 최대값
#define P_TDS_IN_ON                                     P14_bit.no0         // TDS IN 전원 공급 Port
//#define TDS_IN_INTERRUPT_START()                      exStart();          // Code Generate된 TDS In 외부 인터럽트 Start함수 배치
//#define TDS_IN_INTERRUPT_STOP(                        exStop();           // Code Generate된 TDS In 외부 인터럽트 Stop함수 배치
#define TDS_IN_INTERRUPT_START()                        0
#define TDS_IN_INTERRUPT_STOP()                         0

#define TURN_ON_TDS_IN()                                { P_TDS_IN_ON = 1; }
#define TURN_OFF_TDS_IN()                               { P_TDS_IN_ON = 0; }
#define GET_STATUS_TDS_IN()                             ( P_TDS_IN_ON )

#define TDS_IN_CONTROL_TIME_PERIOD                      1           // 1ms@1ms
#endif

#if (TDS_OUT_USE == USE)
#define TDS_OUT_THERMISTOR_ADC_ID                       ADC_ID_TH_TDS_OUT   // TDS OUT 온도 센서의 ADC ID
#define TDS_OUT_SENSOR_ADC_ID                           ADC_ID_TDS_OUT      // TDS OUT 센서의 ADC ID
#define TDS_OUT_SENSOR_ID                               TDS_ID_OUT          // TDS OUT 센서의 ID
#define TDS_OUT_AVERAGE_COUNT                           10                  // TDS OUT 값 평균 산출 데이터 수
#define VALUE_DEFINE_TDS_OUT_DATA_MAX                   1024                // TDS OUT 값 최대값
#define P_TDS_OUT_ON_1                                  0                   // TDS OUT 전원 공급 Port 1
#define P_TDS_OUT_ON_2                                  0                   // TDS OUT 전원 공급 Port 2

#define TURN_ON_TDS_OUT_1()                             { P_TDS_OUT_ON_1 = 1; }
#define TURN_OFF_TDS_OUT_1()                            { P_TDS_OUT_ON_1 = 0; }
#define GET_STATUS_TDS_OUT_1()                          ( P_TDS_OUT_ON_1 )

#define TURN_ON_TDS_OUT_2()                             { P_TDS_OUT_ON_2 = 1; }
#define TURN_OFF_TDS_OUT_2()                            { P_TDS_OUT_ON_2 = 0; }
#define GET_STATUS_TDS_OUT_2()                          ( P_TDS_OUT_ON_2 )

#define TDS_OUT_CONTROL_TIME_PERIOD                     5           // 500us@100us
#define TDS_OUT_CHECK_CONTROL_TIME_PERIOD               100         // 100ms@1ms
#endif

#define TDS_ID_MAX_COUNT          TDS_ID_MAX


#if (TDS_IN_USE == USE)
// TDS 입수 입수 온도 AD값
#define TDS_IN_TEMP0_0             (1024 - 768)    // 0도       256
#define TDS_IN_TEMP1_0             (1024 - 759)    // 1도       265
#define TDS_IN_TEMP2_0             (1024 - 751)    // 2도       273
#define TDS_IN_TEMP3_0             (1024 - 742)    // 3도       282
#define TDS_IN_TEMP4_0             (1024 - 733)    // 4도       291
#define TDS_IN_TEMP5_0             (1024 - 724)    // 5도       300
#define TDS_IN_TEMP6_0             (1024 - 715)    // 6도       309
#define TDS_IN_TEMP7_0             (1024 - 706)    // 7도       318
#define TDS_IN_TEMP7_5             (1024 - 701)    // 7.5도     323
#define TDS_IN_TEMP8_0             (1024 - 696)    // 8도       328
#define TDS_IN_TEMP9_0             (1024 - 687)    // 9도       337
#define TDS_IN_TEMP10_0            (1024 - 677)    // 10도      347
#define TDS_IN_TEMP11_0            (1024 - 668)    // 11도      356
#define TDS_IN_TEMP12_0            (1024 - 658)    // 12도      366
#define TDS_IN_TEMP12_5            (1024 - 653)    // 12.5도    371
#define TDS_IN_TEMP13_0            (1024 - 648)    // 13도      376
#define TDS_IN_TEMP14_0            (1024 - 639)    // 14도      385
#define TDS_IN_TEMP15_0            (1024 - 629)    // 15도      395
#define TDS_IN_TEMP16_0            (1024 - 619)    // 16도      405
#define TDS_IN_TEMP17_0            (1024 - 609)    // 17도      415
#define TDS_IN_TEMP17_5            (1024 - 604)    // 17.5도    420
#define TDS_IN_TEMP18_0            (1024 - 599)    // 18도      425
#define TDS_IN_TEMP19_0            (1024 - 589)    // 19도      435
#define TDS_IN_TEMP20_0            (1024 - 580)    // 20도      444
#define TDS_IN_TEMP21_0            (1024 - 570)    // 21도      454
#define TDS_IN_TEMP22_0            (1024 - 560)    // 22도      464
#define TDS_IN_TEMP22_5            (1024 - 555)    // 22.5도    469
#define TDS_IN_TEMP23_0            (1024 - 550)    // 23도      474
#define TDS_IN_TEMP24_0            (1024 - 540)    // 24도      484
#define TDS_IN_TEMP25_0            (1024 - 530)    // 25도      494
#define TDS_IN_TEMP26_0            (1024 - 521)    // 26도      503
#define TDS_IN_TEMP27_0            (1024 - 511)    // 27도      513
#define TDS_IN_TEMP27_5            (1024 - 506)    // 27.5도    518
#define TDS_IN_TEMP28_0            (1024 - 501)    // 28도      523
#define TDS_IN_TEMP29_0            (1024 - 492)    // 29도      532
#define TDS_IN_TEMP30_0            (1024 - 482)    // 30도      542
#define TDS_IN_TEMP31_0            (1024 - 473)    // 31도      551
#define TDS_IN_TEMP31_5            (1024 - 477)    // 31.5도    547
#define TDS_IN_TEMP32_0            (1024 - 463)    // 32도      561
#define TDS_IN_TEMP32_5            (1024 - 459)    // 32.5도    565
#define TDS_IN_TEMP33_0            (1024 - 454)    // 33도      570
#define TDS_IN_TEMP34_0            (1024 - 445)    // 34도      579
#define TDS_IN_TEMP35_0            (1024 - 436)    // 35도      588
#define TDS_IN_TEMP36_0            (1024 - 427)    // 36도      597
#define TDS_IN_TEMP37_0            (1024 - 418)    // 37도      606
#define TDS_IN_TEMP38_0            (1024 - 409)    // 38도      615
#define TDS_IN_TEMP39_0            (1024 - 401)    // 39도      623
#define TDS_IN_TEMP40_0            (1024 - 392)    // 40도      632
#define TDS_IN_TEMP41_0            (1024 - 384)    // 41도      640
#define TDS_IN_TEMP42_0            (1024 - 375)    // 42도      649
#define TDS_IN_TEMP43_0            (1024 - 367)    // 43도      657
#define TDS_IN_TEMP44_0            (1024 - 359)    // 44도      665
#define TDS_IN_TEMP45_0            (1024 - 351)    // 45도      673
#define TDS_IN_TEMP46_0            (1024 - 343)    // 46도      681
#define TDS_IN_TEMP47_0            (1024 - 335)    // 47도      689
#define TDS_IN_TEMP48_0            (1024 - 328)    // 48도      696
#define TDS_IN_TEMP49_0            (1024 - 320)    // 49도      704
#define TDS_IN_TEMP50_0            (1024 - 313)    // 50도      711
#define TDS_IN_TEMP51_0            (1024 - 306)    // 51도      718
#define TDS_IN_TEMP52_0            (1024 - 299)    // 52도      725
#define TDS_IN_TEMP53_0            (1024 - 292)    // 53도      732
#define TDS_IN_TEMP54_0            (1024 - 285)    // 54도      739
#define TDS_IN_TEMP55_0            (1024 - 279)    // 55도      745
#define TDS_IN_TEMP56_0            (1024 - 272)    // 56도      752
#define TDS_IN_TEMP57_0            (1024 - 266)    // 57도      758
#define TDS_IN_TEMP58_0            (1024 - 260)    // 58도      764
#define TDS_IN_TEMP59_0            (1024 - 253)    // 59도      771
#define TDS_IN_TEMP60_0            (1024 - 247)    // 60도      777
#define TDS_IN_TEMP61_0            (1024 - 242)    // 61도      782
#define TDS_IN_TEMP62_0            (1024 - 236)    // 62도      788
#define TDS_IN_TEMP63_0            (1024 - 230)    // 63도      794
#define TDS_IN_TEMP64_0            (1024 - 225)    // 64도      799
#define TDS_IN_TEMP65_0            (1024 - 219)    // 65도      805
#define TDS_IN_TEMP66_0            (1024 - 214)    // 66도      810
#define TDS_IN_TEMP67_0            (1024 - 209)    // 67도      815
#define TDS_IN_TEMP68_0            (1024 - 204)    // 68도      820
#define TDS_IN_TEMP69_0            (1024 - 199)    // 69도      825
#define TDS_IN_TEMP70_0            (1024 - 194)    // 70도      830
#define TDS_IN_TEMP71_0            (1024 - 190)    // 71도      834
#define TDS_IN_TEMP72_0            (1024 - 185)    // 72도      839
#define TDS_IN_TEMP73_0            (1024 - 181)    // 73도      843
#define TDS_IN_TEMP74_0            (1024 - 176)    // 74도      848
#define TDS_IN_TEMP75_0            (1024 - 172)    // 75도      852
#define TDS_IN_TEMP76_0            (1024 - 168)    // 76도      856
#define TDS_IN_TEMP77_0            (1024 - 164)    // 77도      860
#define TDS_IN_TEMP78_0            (1024 - 160)    // 78도      864
#define TDS_IN_TEMP79_0            (1024 - 156)    // 79도      868
#define TDS_IN_TEMP80_0            (1024 - 152)    // 80도      872
#define TDS_IN_TEMP81_0            (1024 - 149)    // 81도      875
#define TDS_IN_TEMP82_0            (1024 - 145)    // 82도      879
#define TDS_IN_TEMP83_0            (1024 - 142)    // 83도      882
#define TDS_IN_TEMP84_0            (1024 - 138)    // 84도      886
#define TDS_IN_TEMP85_0            (1024 - 135)    // 85도      889
#define TDS_IN_TEMP86_0            (1024 - 132)    // 86도      892
#define TDS_IN_TEMP87_0            (1024 - 129)    // 87도      895
#define TDS_IN_TEMP88_0            (1024 - 126)    // 88도      898
#define TDS_IN_TEMP89_0            (1024 - 123)    // 89도      901
#define TDS_IN_TEMP90_0            (1024 - 120)    // 90도      904
#define TDS_IN_TEMP91_0            (1024 - 117)    // 91도      907
#define TDS_IN_TEMP92_0            (1024 - 114)    // 92도      910
#define TDS_IN_TEMP93_0            (1024 - 111)    // 93도      913
#define TDS_IN_TEMP94_0            (1024 - 109)    // 94도      915
#define TDS_IN_TEMP95_0            (1024 - 106)    // 95도      918
#define TDS_IN_TEMP96_0            (1024 - 104)    // 96도      920
#define TDS_IN_TEMP97_0            (1024 - 101)    // 97도      923
#define TDS_IN_TEMP98_0            (1024 - 99)     // 98도      925
#define TDS_IN_TEMP99_0            (1024 - 97)     // 99도      927
#define TDS_IN_TEMP100_0           (1024 - 94)     // 100도     930

#endif


#if ( (TDS_IN_USE == USE) || (TDS_OUT_USE == USE) )
#define TDS_DATA_MAX                            1024        // TDS 센싱 데이터의 최대값

void Hal_TDS_Initialize(void);
#endif

#if (TDS_IN_USE == USE)
void TDS_In_ControlTimer(void);
void Set_TDS_In_Check_Start(U8 mu8OnOff);
U8 Get_TDS_In_Check_Start(void);
void TDS_In_Interrupt(void);
void TDS_In_Control(void);
void TDS_In_FACTOR(void);
U16 Get_TDS_In_Data(void);
#endif

#if (TDS_OUT_USE == USE)
void TDS_Out_ControlTimer(void);
void TDS_Out_CheckControlTimer(void);
void Set_TDS_Out_Check_Start(U8 mu8OnOff);
U8 Get_TDS_Out_Check_Start(void);
void TDS_Out_PowerControl(void);
void TDS_Out_ADC_CheckStartControl(void);
void TDS_Out_Calculation(void);
U16 Get_TDS_Out_Data(void);
#endif


void Hal_TDS_Module_Initialize(void);
void Hal_TDS_Module_1ms_Control(void);
void Hal_TDS_Module_100us_Control(void);
void Hal_TDS_Module_Control(void);

#endif

