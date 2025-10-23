/// @file     Hal_DC_Pump.h
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    DC Pump Control file


#ifndef _HAL_DC_PUMP_H_
#define _HAL_DC_PUMP_H_


/*  DC Pump 제어 Function 사용법
    => Set_DC_PumpControl(Pump ID, On/Off data, Delay Time);
    => Set_PumpPWMControl(Pump ID, mu16RegValue)   // Pump ID에 해당하는 PWM 제어 - mu16RegValue값은 레지스터에 직접 쓰는 값

    사용 예)
    Set_DC_PumpControl(DC_PUMP_COLD_OUT, ON, 0);    // DC_PUMP_COLD_OUT ID의 Pump를 0초 딜레이 후 ON
    Set_DC_PumpControl(DC_PUMP_COLD_OUT, OFF,0);    // DC_PUMP_COLD_OUT ID의 Pump를 0초 딜레이 후 OFF
    Set_DC_PumpControl(DC_PUMP_COLD_OUT, ON, 5);    // DC_PUMP_COLD_OUT ID의 Pump를 500ms 딜레이 후 ON
    Set_DC_PumpControl(DC_PUMP_COLD_OUT, OFF,5);    // DC_PUMP_COLD_OUT ID의 Pump를 500ms 딜레이 후 OFF

    Set_PumpPWMControl(DC_PUMP_COLD_OUT, 0x3777);   // DC_PUMP_COLD_OUT ID의 Pump의 전압가변 PWM 제어 레지스터에 0x3777을 씀

    Hal_All_DC_Pump_Off();                          // 모든 DC Pump OFF
*/

// DC PUMP 최대 10개 사용 가능
typedef enum
{
    DC_PUMP_ID_NONE = 0,                                // 0 :
    DC_PUMP_ID_1,                                       // 1 :
    DC_PUMP_ID_DRAIN,                                   // 2 :
    DC_PUMP_ID_MAX                                      // 11 Max, 11이 넘으면 안됨
} EDC_PumpID_T;

#define DC_PUMP_COUNT                                   1           // DC Pump 개수 - MAX 10

// DC Pump port 설정
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
//#define DC_PUMP_1_PWM_START()                         exPwm_Start();    // Code Generate된 Pump1의 PWM 제어 Start함수 배치
//#define DC_PUMP_1_PWM_STOP()                          exPwm_Stop();     // Code Generate된 Pump1의 PWM 제어 Stop함수 배치

#define PUMP_PWM_NO_USE                                 0
#define PUMP_PWM_REGISTER_USE                           1
#define PUMP_PWM_TRIG_USE                               2

// Pump 1
#define PUMP_1_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 1 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_1_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_1_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_1_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_1_PWM_ON_DELAY                 0           // PWM 제어 와 밸브 ON 사이의 Delay @100ms
#define TIME_DEFINE_PUMP_1_PWM_OFF_DELAY                0           // PWM 제어 후 밸브 OFF 사이의 Delay @100ms
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_1_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_1_TRIG_ON_DELAY                0           // TRIG Port 제어 와 밸브 ON 사이의 Delay @100ms
#define TIME_DEFINE_PUMP_1_TRIG_OFF_DELAY               0           // TRIG Port 제어 와 밸브 OFF 사이의 Delay @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_1                           P7_bit.no7

// Pump 2
#define PUMP_2_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 2 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_2_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_2_PWM_START()                           0           // Code Generate된 Pump2의 PWM 제어 Start함수 배치
#define DC_PUMP_2_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_2_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_2_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_2_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_2_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_2_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_2                           0

// Pump 3
#define PUMP_3_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 3 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_3_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_3_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_3_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_3_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_3_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_3_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_3_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_3_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_3                           0

// Pump 4
#define PUMP_4_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 4 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_4_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_4_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_4_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_4_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_4_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_4_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_4_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_4_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_4                           0

// Pump 5
#define PUMP_5_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 5 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_5_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_5_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_5_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_5_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_5_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_5_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_5_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_5_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_5                           0

// Pump 6
#define PUMP_6_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 6 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_6_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_6_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_6_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_6_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_6_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_6_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_6_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_6_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_6                           0

// Pump 7
#define PUMP_7_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 7 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_7_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_7_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_7_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_7_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_7_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_7_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_7_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_7_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_7                           0

// Pump 8
#define PUMP_8_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 8 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_8_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_8_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_8_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_8_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_8_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_8_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_8_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_8_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_8                           0

// Pump 9
#define PUMP_9_PWM_USE                                  PUMP_PWM_NO_USE     // PUMP 9 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_9_PWM                   0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_9_PWM_START()                           0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_9_PWM_STOP()                            0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_9_PWM_ON_DELAY                 0           // @100ms
#define TIME_DEFINE_PUMP_9_PWM_OFF_DELAY                0           // @100ms
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_9_TRIG                      0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_9_TRIG_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_9_TRIG_OFF_DELAY               0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_9                           0

// Pump 10
#define PUMP_10_PWM_USE                                 PUMP_PWM_NO_USE     // PUMP 10 PWM 제어 여부 - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
#define REGISTER_DEFINE_DC_PUMP_10_PWM                  0           // PWM 제어 레지스터 세팅, 미사용시 0으로 처리
#define DC_PUMP_10_PWM_START()                          0           // Code Generate된 Pump1의 PWM 제어 Start함수 배치
#define DC_PUMP_10_PWM_STOP()                           0           // Code Generate된 Pump1의 PWM 제어 Stop함수 배치
#define TIME_DEFINE_PUMP_10_PWM_ON_DELAY                0           // @100ms
#define TIME_DEFINE_PUMP_10_PWM_OFF_DELAY               0           // @100ms
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
#define PORT_DEFINE_DC_PUMP_10_TRIG                     0           // TRIG Port 세팅, 미사용시 0으로 처리
#define TIME_DEFINE_PUMP_10_TRIG_ON_DELAY               0           // @100ms
#define TIME_DEFINE_PUMP_10_TRIG_OFF_DELAY              0           // @100ms
#else
#endif
#define PORT_DEFINE_DC_PUMP_10                          0


#if (DC_PUMP_COUNT > 0)

#define DC_PUMP_ID_MAX_COUNT            DC_PUMP_ID_MAX

// DC Pump Port Define
#if (PUMP_1_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_1_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_1_PWM
#define PUMP_1_PWM_ON_DELAY             TIME_DEFINE_PUMP_1_PWM_ON_DELAY     // @100ms
#define PUMP_1_PWM_OFF_DELAY            TIME_DEFINE_PUMP_1_PWM_OFF_DELAY    // @100ms
#elif (PUMP_1_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_1_TRIG                   PORT_DEFINE_DC_PUMP_1_TRIG
#define TURN_ON_PUMP_1_TRIG()           { P_PUMP_1_TRIG = 1; }
#define TURN_OFF_PUMP_1_TRIG()          { P_PUMP_1_TRIG = 0; }
#define GET_STATUS_PUMP_1_TRIG()        ( P_PUMP_1_TRIG )

#define PUMP_1_TRIG_ON_DELAY            TIME_DEFINE_PUMP_1_TRIG_ON_DELAY    // @100ms
#define PUMP_1_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_1_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_1                        PORT_DEFINE_DC_PUMP_1
#define TURN_ON_PUMP_1()                { P_PUMP_1 = 1; }
#define TURN_OFF_PUMP_1()               { P_PUMP_1 = 0; }
#define GET_STATUS_PUMP_1()             ( P_PUMP_1 )


#if (PUMP_2_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_2_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_2_PWM
#define PUMP_2_PWM_ON_DELAY             TIME_DEFINE_PUMP_2_PWM_ON_DELAY     // @100ms
#define PUMP_2_PWM_OFF_DELAY            TIME_DEFINE_PUMP_2_PWM_OFF_DELAY    // @100ms
#elif (PUMP_2_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_2_TRIG                   PORT_DEFINE_DC_PUMP_2_TRIG
#define TURN_ON_PUMP_2_TRIG()           { P_PUMP_2_TRIG = 1; }
#define TURN_OFF_PUMP_2_TRIG()          { P_PUMP_2_TRIG = 0; }
#define GET_STATUS_PUMP_2_TRIG()        ( P_PUMP_2_TRIG )

#define PUMP_2_TRIG_ON_DELAY            TIME_DEFINE_PUMP_2_TRIG_ON_DELAY    // @100ms
#define PUMP_2_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_2_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_2                        PORT_DEFINE_DC_PUMP_2
#define TURN_ON_PUMP_2()                { P_PUMP_2 = 1; }
#define TURN_OFF_PUMP_2()               { P_PUMP_2 = 0; }
#define GET_STATUS_PUMP_2()             ( P_PUMP_2 )


#if (PUMP_3_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_3_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_3_PWM
#define PUMP_3_PWM_ON_DELAY             TIME_DEFINE_PUMP_3_PWM_ON_DELAY     // @100ms
#define PUMP_3_PWM_OFF_DELAY            TIME_DEFINE_PUMP_3_PWM_OFF_DELAY    // @100ms
#elif (PUMP_3_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_3_TRIG                   PORT_DEFINE_DC_PUMP_3_TRIG
#define TURN_ON_PUMP_3_TRIG()           { P_PUMP_3_TRIG = 1; }
#define TURN_OFF_PUMP_3_TRIG()          { P_PUMP_3_TRIG = 0; }
#define GET_STATUS_PUMP_3_TRIG()        ( P_PUMP_3_TRIG )

#define PUMP_3_TRIG_ON_DELAY            TIME_DEFINE_PUMP_3_TRIG_ON_DELAY    // @100ms
#define PUMP_3_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_3_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_3                        PORT_DEFINE_DC_PUMP_3
#define TURN_ON_PUMP_3()                { P_PUMP_3 = 1; }
#define TURN_OFF_PUMP_3()               { P_PUMP_3 = 0; }
#define GET_STATUS_PUMP_3()             ( P_PUMP_3 )


#if (PUMP_4_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_4_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_4_PWM
#define PUMP_4_PWM_ON_DELAY             TIME_DEFINE_PUMP_4_PWM_ON_DELAY     // @100ms
#define PUMP_4_PWM_OFF_DELAY            TIME_DEFINE_PUMP_4_PWM_OFF_DELAY    // @100ms
#elif (PUMP_4_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_4_TRIG                   PORT_DEFINE_DC_PUMP_4_TRIG
#define TURN_ON_PUMP_4_TRIG()           { P_PUMP_4_TRIG = 1; }
#define TURN_OFF_PUMP_4_TRIG()          { P_PUMP_4_TRIG = 0; }
#define GET_STATUS_PUMP_4_TRIG()        ( P_PUMP_4_TRIG )

#define PUMP_4_TRIG_ON_DELAY            TIME_DEFINE_PUMP_4_TRIG_ON_DELAY    // @100ms
#define PUMP_4_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_4_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_4                        PORT_DEFINE_DC_PUMP_4
#define TURN_ON_PUMP_4()                { P_PUMP_4 = 1; }
#define TURN_OFF_PUMP_4()               { P_PUMP_4 = 0; }
#define GET_STATUS_PUMP_4()             ( P_PUMP_4 )


#if (PUMP_5_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_5_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_5_PWM
#define PUMP_5_PWM_ON_DELAY             TIME_DEFINE_PUMP_5_PWM_ON_DELAY     // @100ms
#define PUMP_5_PWM_OFF_DELAY            TIME_DEFINE_PUMP_5_PWM_OFF_DELAY    // @100ms
#elif (PUMP_5_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_5_TRIG                   PORT_DEFINE_DC_PUMP_5_TRIG
#define TURN_ON_PUMP_5_TRIG()           { P_PUMP_5_TRIG = 1; }
#define TURN_OFF_PUMP_5_TRIG()          { P_PUMP_5_TRIG = 0; }
#define GET_STATUS_PUMP_5_TRIG()        ( P_PUMP_5_TRIG )

#define PUMP_5_TRIG_ON_DELAY            TIME_DEFINE_PUMP_5_TRIG_ON_DELAY    // @100ms
#define PUMP_5_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_5_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_5                        PORT_DEFINE_DC_PUMP_5
#define TURN_ON_PUMP_5()                { P_PUMP_5 = 1; }
#define TURN_OFF_PUMP_5()               { P_PUMP_5 = 0; }
#define GET_STATUS_PUMP_5()             ( P_PUMP_5 )


#if (PUMP_6_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_6_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_6_PWM
#define PUMP_6_PWM_ON_DELAY             TIME_DEFINE_PUMP_6_PWM_ON_DELAY     // @100ms
#define PUMP_6_PWM_OFF_DELAY            TIME_DEFINE_PUMP_6_PWM_OFF_DELAY    // @100ms
#elif (PUMP_6_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_6_TRIG                   PORT_DEFINE_DC_PUMP_6_TRIG
#define TURN_ON_PUMP_6_TRIG()           { P_PUMP_6_TRIG = 1; }
#define TURN_OFF_PUMP_6_TRIG()          { P_PUMP_6_TRIG = 0; }
#define GET_STATUS_PUMP_6_TRIG()        ( P_PUMP_6_TRIG )

#define PUMP_6_TRIG_ON_DELAY            TIME_DEFINE_PUMP_6_TRIG_ON_DELAY    // @100ms
#define PUMP_6_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_6_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_6                        PORT_DEFINE_DC_PUMP_6
#define TURN_ON_PUMP_6()                { P_PUMP_6 = 1; }
#define TURN_OFF_PUMP_6()               { P_PUMP_6 = 0; }
#define GET_STATUS_PUMP_6()             ( P_PUMP_6 )


#if (PUMP_7_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_7_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_7_PWM
#define PUMP_7_PWM_ON_DELAY             TIME_DEFINE_PUMP_7_PWM_ON_DELAY     // @100ms
#define PUMP_7_PWM_OFF_DELAY            TIME_DEFINE_PUMP_7_PWM_OFF_DELAY    // @100ms
#elif (PUMP_7_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_7_TRIG                   PORT_DEFINE_DC_PUMP_7_TRIG
#define TURN_ON_PUMP_7_TRIG()           { P_PUMP_7_TRIG = 1; }
#define TURN_OFF_PUMP_7_TRIG()          { P_PUMP_7_TRIG = 0; }
#define GET_STATUS_PUMP_7_TRIG()        ( P_PUMP_7_TRIG )

#define PUMP_7_TRIG_ON_DELAY            TIME_DEFINE_PUMP_7_TRIG_ON_DELAY    // @100ms
#define PUMP_7_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_7_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_7                        PORT_DEFINE_DC_PUMP_7
#define TURN_ON_PUMP_7()                { P_PUMP_7 = 1; }
#define TURN_OFF_PUMP_7()               { P_PUMP_7 = 0; }
#define GET_STATUS_PUMP_7()             ( P_PUMP_7 )


#if (PUMP_8_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_8_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_8_PWM
#define PUMP_8_PWM_ON_DELAY             TIME_DEFINE_PUMP_8_PWM_ON_DELAY     // @100ms
#define PUMP_8_PWM_OFF_DELAY            TIME_DEFINE_PUMP_8_PWM_OFF_DELAY    // @100ms
#elif (PUMP_8_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_8_TRIG                   PORT_DEFINE_DC_PUMP_8_TRIG
#define TURN_ON_PUMP_8_TRIG()           { P_PUMP_8_TRIG = 1; }
#define TURN_OFF_PUMP_8_TRIG()          { P_PUMP_8_TRIG = 0; }
#define GET_STATUS_PUMP_8_TRIG()        ( P_PUMP_8_TRIG )

#define PUMP_8_TRIG_ON_DELAY            TIME_DEFINE_PUMP_8_TRIG_ON_DELAY    // @100ms
#define PUMP_8_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_8_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_8                        PORT_DEFINE_DC_PUMP_8
#define TURN_ON_PUMP_8()                { P_PUMP_8 = 1; }
#define TURN_OFF_PUMP_8()               { P_PUMP_8 = 0; }
#define GET_STATUS_PUMP_8()             ( P_PUMP_8 )


#if (PUMP_9_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_9_PWM_REGISTER             REGISTER_DEFINE_DC_PUMP_9_PWM
#define PUMP_9_PWM_ON_DELAY             TIME_DEFINE_PUMP_9_PWM_ON_DELAY     // @100ms
#define PUMP_9_PWM_OFF_DELAY            TIME_DEFINE_PUMP_9_PWM_OFF_DELAY    // @100ms
#elif (PUMP_9_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_9_TRIG                   PORT_DEFINE_DC_PUMP_9_TRIG
#define TURN_ON_PUMP_9_TRIG()           { P_PUMP_9_TRIG = 1; }
#define TURN_OFF_PUMP_9_TRIG()          { P_PUMP_9_TRIG = 0; }
#define GET_STATUS_PUMP_9_TRIG()        ( P_PUMP_9_TRIG )

#define PUMP_9_TRIG_ON_DELAY            TIME_DEFINE_PUMP_9_TRIG_ON_DELAY    // @100ms
#define PUMP_9_TRIG_OFF_DELAY           TIME_DEFINE_PUMP_9_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_PUMP_9                        PORT_DEFINE_DC_PUMP_9
#define TURN_ON_PUMP_9()                { P_PUMP_9 = 1; }
#define TURN_OFF_PUMP_9()               { P_PUMP_9 = 0; }
#define GET_STATUS_PUMP_9()             ( P_PUMP_9 )


#if (PUMP_10_PWM_USE == PUMP_PWM_REGISTER_USE)
#define PUMP_10_PWM_REGISTER            REGISTER_DEFINE_DC_PUMP_10_PWM
#define PUMP_10_PWM_ON_DELAY            TIME_DEFINE_PUMP_10_PWM_ON_DELAY    // @100ms
#define PUMP_10_PWM_OFF_DELAY           TIME_DEFINE_PUMP_10_PWM_OFF_DELAY   // @100ms
#elif (PUMP_10_PWM_USE == PUMP_PWM_TRIG_USE)
#define P_PUMP_10_TRIG                  PORT_DEFINE_DC_PUMP_10_TRIG
#define TURN_ON_PUMP_10_TRIG()          { P_PUMP_10_TRIG = 1; }
#define TURN_OFF_PUMP_10_TRIG()         { P_PUMP_10_TRIG = 0; }
#define GET_STATUS_PUMP_10_TRIG()       ( P_PUMP_10_TRIG )

#define PUMP_10_TRIG_ON_DELAY           TIME_DEFINE_PUMP_10_TRIG_ON_DELAY   // @100ms
#define PUMP_10_TRIG_OFF_DELAY          TIME_DEFINE_PUMP_10_TRIG_OFF_DELAY  // @100ms
#else
#endif
#define P_PUMP_10                       PORT_DEFINE_DC_PUMP_10
#define TURN_ON_PUMP_10()               { P_PUMP_10 = 1; }
#define TURN_OFF_PUMP_10()              { P_PUMP_10 = 0; }
#define GET_STATUS_PUMP_10()            ( P_PUMP_10 )


#define DC_PUMP_CONTROL_TIME_PERIOD     100         // 100ms@1ms

void Hal_DC_Pump_Initialize(void);
void DC_PumpControlTimer(void);
void DC_PumpControl(void);
void Set_DC_PumpControl(U8 mu8PumpID, U8 mu8Status, U16 mu16Delay);
U8 Get_DC_PumpStatus(U8 mu8PumpID);
void Hal_All_DC_Pump_Off(void);
void Hal_DC_PumpControl(U8 mu8PumpID);
U8 Get_DC_PumpStatus_HW(U8 mu8PumpID);
void Set_PumpPWMControl(U8 mu8PumpID, U16 mu16RegValue);

#if (DC_PUMP_COUNT >= 1)
void Hal_DC_Pump_1_On(void);
void Hal_DC_Pump_1_Off(void);
#endif

#if (DC_PUMP_COUNT >= 2)
void Hal_DC_Pump_2_On(void);
void Hal_DC_Pump_2_Off(void);
#endif

#if (DC_PUMP_COUNT >= 3)
void Hal_DC_Pump_3_On(void);
void Hal_DC_Pump_3_Off(void);
#endif

#if (DC_PUMP_COUNT >= 4)
void Hal_DC_Pump_4_On(void);
void Hal_DC_Pump_4_Off(void);
#endif

#if (DC_PUMP_COUNT >= 5)
void Hal_DC_Pump_5_On(void);
void Hal_DC_Pump_5_Off(void);
#endif

#if (DC_PUMP_COUNT >= 6)
void Hal_DC_Pump_6_On(void);
void Hal_DC_Pump_6_Off(void);
#endif

#if (DC_PUMP_COUNT >= 7)
void Hal_DC_Pump_7_On(void);
void Hal_DC_Pump_7_Off(void);
#endif

#if (DC_PUMP_COUNT >= 8)
void Hal_DC_Pump_8_On(void);
void Hal_DC_Pump_8_Off(void);
#endif

#if (DC_PUMP_COUNT >= 9)
void Hal_DC_Pump_9_On(void);
void Hal_DC_Pump_9_Off(void);
#endif

#if (DC_PUMP_COUNT >= 10)
void Hal_DC_Pump_10_On(void);
void Hal_DC_Pump_10_Off(void);
#endif


#endif


void Hal_DC_Pump_Module_Initialize(void);
void Hal_DC_Pump_Module_1ms_Control(void);
void Hal_DC_Pump_Module_Control(void);

#endif

