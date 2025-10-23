/// @file   Hal_Buzzer.h
/// @date   2025/04/09
/// @author Jaejin Ham
/// @brief  Buzzer 동작 제어 관련 헤더 file

#ifndef _HAL_BUZZER_H_
#define _HAL_BUZZER_H_


/*  Buzzer 제어 Function 사용법(참고 : Squarewave 세팅하고 Clock source를 32MHz로, Square width를 100us로 세팅할 것)
    => Set_Buzzer_Out(mu16Value);           // Buzzer 제어할 PWM 제어 : mu8Value - 레지스터값

    사용 예)
    Set_Buzzer_Out(LA5);                     // 5옥타브 라 음 출력
    Set_Buzzer_Out(0);                       // 음 출력 정지(stop)
*/

#define BUZZER_USE                          USE             // Buzzer 사용 여부 - 0(No USE), 1(USE)

#if (BUZZER_USE == USE)
// Buzzer Port 할당
// 미사용 port는 0으로 처리
#define REGISTER_DEFINE_BUZZER_PWM          TDR14
#define BUZZER_PWM_START()                  R_Config_TAU1_4_Start();    // Code Generate된 PWM 제어 Start함수 배치
#define BUZZER_PWM_STOP()                   R_Config_TAU1_4_Stop();     // Code Generate된 PWM 제어 Stop함수 배치
#define PORT_DEFINE_BUZZER_ON               P4_bit.no5      // Buzzer On Port


// 16MHz OSE - TDRxx = 0x063F = 1599 => 200us High / 200us Low => 400us period
// 32MHz OSE - TDRxx = 0x0C7F = 3199 => 200us High / 200us Low => 400us period
#define BUZZER_REGISTER                     REGISTER_DEFINE_BUZZER_PWM

#define P_BUZZER_ON                         PORT_DEFINE_BUZZER_ON
#define TURN_ON_BUZZER()                    { P_BUZZER_ON = 1; }
#define TURN_OFF_BUZZER()                   { P_BUZZER_ON = 0; }
#define GET_STATUS_BUZZER()                 ( P_BUZZER_ON )

#define OSC_32MHZ                           0
#define OSC_16MHZ                           1
#define OSC_FREQUENCY                       OSC_32MHZ

#if (OSC_FREQUENCY == OSC_16MHZ)

// 4옥타브
#define DO4         30591               // 도 : 261.6256Hz = 3.822ms
#define RE4         27263               // 레 : 293.6648Hz = 3.405ms
#define MI4         24271               // 미 : 329.6276Hz = 3.034ms
#define PA4         22927               // 파 : 349.2282Hz = 2.863ms
#define PAS4        21615               // 파#: 369.9944Hz = 2.703ms
#define SOL4        20415               // 솔 : 391.9954Hz = 2.551ms
#define LA4         18175               // 라 : 440.0000Hz = 2.273ms
#define SI4         16191               // 시 : 493.8833Hz = 2.025ms
// 5옥타브
#define DO5         15295               // 도 : 523.2511Hz = 1.912ms
#define DOS5        14432               // 도#: 554.3653Hz = 1.804ms
#define RE5         13631               // 레 : 587.3295Hz = 1.704ms
#define MI5         12135               // 미 : 659.2551Hz = 1.517ms
#define PA5         11463               // 파 : 698.4565Hz = 1.433ms
#define PAS5        10807               // 파#: 739.9888Hz = 1.351ms
#define SOL5        10207               // 솔 : 783.9909Hz = 1.276ms
#define SOLS5       9631                // 솔#: 830.6094Hz = 1.204ms
#define LA5         9087                // 라 : 880.0000Hz = 1.136ms
#define SI5         8095                // 시 : 987.7666Hz = 1.012ms
// 6옥타브
#define DO6         7639                // 도 : 1046.502Hz = 0.955ms
#define DOS6        7215                // 도#: 1108.731Hz = 0.902ms
#define RE6         6807                // 레 : 1174.659Hz = 0.851ms
#define MI6         6063                // 미 : 1318.510Hz = 0.758ms
#define PA6         5727                // 파 : 1396.913Hz = 0.716ms
#define PAS6        5407                // 파#: 1479.978Hz = 0.676ms
#define SOL6        5103                // 솔 : 1567.982Hz = 0.638ms
#define SOLS6       4815                // 솔#: 1661.219Hz = 0.602ms
#define LA6         4543                // 라 : 1760.000Hz = 0.568ms
#define SI6         4047                // 시 : 1975.533Hz = 0.506ms
// 7옥타브
#define DO7         3823                // 도 : 2093.005Hz = 0.478ms
#define DOS7        3607                // 도#: 2217.461Hz = 0.451ms
#define RE7         3407                // 레 : 2349.318Hz = 0.426ms
#define MI7         3031                // 미 : 2637.020Hz = 0.379ms
#define PA7         2863                // 파 : 2793.826Hz = 0.357ms
#define PAS7        2703                // 파#: 2959.955Hz = 0.338ms
#define SOL7        2551                // 솔 : 3135.963Hz = 0.319ms
#define SOLS7       2407                // 솔#: 3322.438Hz = 0.301ms
#define LA7         2271                // 라 : 3520.000Hz = 0.284ms
#define SI7         2023                // 시 : 3951.066Hz = 0.253ms


#else

// 4옥타브
#define DO4         (30591 * 2)         // 도 : 261.6256Hz = 3.822ms
#define RE4         (27263 * 2)         // 레 : 293.6648Hz = 3.405ms
#define MI4         (24271 * 2)         // 미 : 329.6276Hz = 3.034ms
#define PA4         (22927 * 2)         // 파 : 349.2282Hz = 2.863ms
#define PAS4        (21615 * 2)         // 파#: 369.9944Hz = 2.703ms
#define SOL4        (20415 * 2)         // 솔 : 391.9954Hz = 2.551ms
#define LA4         (18175 * 2)         // 라 : 440.0000Hz = 2.273ms
#define SI4         (16191 * 2)         // 시 : 493.8833Hz = 2.025ms
// 5옥타브
#define DO5         (15295 * 2)         // 도 : 523.2511Hz = 1.912ms
#define DOS5        (14432 * 2)         // 도#: 554.3653Hz = 1.804ms
#define RE5         (13631 * 2)         // 레 : 587.3295Hz = 1.704ms
#define MI5         (12135 * 2)         // 미 : 659.2551Hz = 1.517ms
#define PA5         (11463 * 2)         // 파 : 698.4565Hz = 1.433ms
#define PAS5        (10807 * 2)         // 파#: 739.9888Hz = 1.351ms
#define SOL5        (10207 * 2)         // 솔 : 783.9909Hz = 1.276ms
#define SOLS5       (9631  * 2)         // 솔#: 830.6094Hz = 1.204ms
#define LA5         (9087  * 2)         // 라 : 880.0000Hz = 1.136ms
#define SI5         (8095  * 2)         // 시 : 987.7666Hz = 1.012ms
// 6옥타브
#define DO6         (7639  * 2)         // 도 : 1046.502Hz = 0.955ms
#define DOS6        (7215  * 2)         // 도#: 1108.731Hz = 0.902ms
#define RE6         (6807  * 2)         // 레 : 1174.659Hz = 0.851ms
#define MI6         (6063  * 2)         // 미 : 1318.510Hz = 0.758ms
#define PA6         (5727  * 2)         // 파 : 1396.913Hz = 0.716ms
#define PAS6        (5407  * 2)         // 파#: 1479.978Hz = 0.676ms
#define SOL6        (5103  * 2)         // 솔 : 1567.982Hz = 0.638ms
#define SOLS6       (4815  * 2)         // 솔#: 1661.219Hz = 0.602ms
#define LA6         (4543  * 2)         // 라 : 1760.000Hz = 0.568ms
#define SI6         (4047  * 2)         // 시 : 1975.533Hz = 0.506ms
// 7옥타브
#define DO7         (3823  * 2)         // 도 : 2093.005Hz = 0.478ms
#define DOS7        (3607  * 2)         // 도#: 2217.461Hz = 0.451ms
#define RE7         (3407  * 2)         // 레 : 2349.318Hz = 0.426ms
#define MI7         (3031  * 2)         // 미 : 2637.020Hz = 0.379ms
#define PA7         (2863  * 2)         // 파 : 2793.826Hz = 0.357ms
#define PAS7        (2703  * 2)         // 파#: 2959.955Hz = 0.338ms
#define SOL7        (2551  * 2)         // 솔 : 3135.963Hz = 0.319ms
#define SOLS7       (2407  * 2)         // 솔#: 3322.438Hz = 0.301ms
#define LA7         (2271  * 2)         // 라 : 3520.000Hz = 0.284ms
#define SI7         (2023  * 2)         // 시 : 3951.066Hz = 0.253ms

#endif

#define BUZZER_START                BUZZER_PWM_START
#define BUZZER_STOP                 BUZZER_PWM_STOP



void Hal_Buzzer_Initialize(void);
void Set_Buzzer_Out(U16 mu16Value);
void Set_Buzzer_PWM(U16 mu16Value);
void Set_Buzzer_ON(U8 mu8OnOff);


#endif

void Hal_Buzzer_Module_Initialize(void);
void Hal_Buzzer_Module_1ms_Control(void);

#endif

