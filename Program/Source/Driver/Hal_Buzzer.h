/// @file   Hal_Buzzer.h
/// @date   2025/04/09
/// @author Jaejin Ham
/// @brief  Buzzer ���� ���� ���� ��� file

#ifndef _HAL_BUZZER_H_
#define _HAL_BUZZER_H_


/*  Buzzer ���� Function ����(���� : Squarewave �����ϰ� Clock source�� 32MHz��, Square width�� 100us�� ������ ��)
    => Set_Buzzer_Out(mu16Value);           // Buzzer ������ PWM ���� : mu8Value - �������Ͱ�

    ��� ��)
    Set_Buzzer_Out(LA5);                     // 5��Ÿ�� �� �� ���
    Set_Buzzer_Out(0);                       // �� ��� ����(stop)
*/

#define BUZZER_USE                          USE             // Buzzer ��� ���� - 0(No USE), 1(USE)

#if (BUZZER_USE == USE)
// Buzzer Port �Ҵ�
// �̻�� port�� 0���� ó��
#define REGISTER_DEFINE_BUZZER_PWM          TDR14
#define BUZZER_PWM_START()                  R_Config_TAU1_4_Start();    // Code Generate�� PWM ���� Start�Լ� ��ġ
#define BUZZER_PWM_STOP()                   R_Config_TAU1_4_Stop();     // Code Generate�� PWM ���� Stop�Լ� ��ġ
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

// 4��Ÿ��
#define DO4         30591               // �� : 261.6256Hz = 3.822ms
#define RE4         27263               // �� : 293.6648Hz = 3.405ms
#define MI4         24271               // �� : 329.6276Hz = 3.034ms
#define PA4         22927               // �� : 349.2282Hz = 2.863ms
#define PAS4        21615               // ��#: 369.9944Hz = 2.703ms
#define SOL4        20415               // �� : 391.9954Hz = 2.551ms
#define LA4         18175               // �� : 440.0000Hz = 2.273ms
#define SI4         16191               // �� : 493.8833Hz = 2.025ms
// 5��Ÿ��
#define DO5         15295               // �� : 523.2511Hz = 1.912ms
#define DOS5        14432               // ��#: 554.3653Hz = 1.804ms
#define RE5         13631               // �� : 587.3295Hz = 1.704ms
#define MI5         12135               // �� : 659.2551Hz = 1.517ms
#define PA5         11463               // �� : 698.4565Hz = 1.433ms
#define PAS5        10807               // ��#: 739.9888Hz = 1.351ms
#define SOL5        10207               // �� : 783.9909Hz = 1.276ms
#define SOLS5       9631                // ��#: 830.6094Hz = 1.204ms
#define LA5         9087                // �� : 880.0000Hz = 1.136ms
#define SI5         8095                // �� : 987.7666Hz = 1.012ms
// 6��Ÿ��
#define DO6         7639                // �� : 1046.502Hz = 0.955ms
#define DOS6        7215                // ��#: 1108.731Hz = 0.902ms
#define RE6         6807                // �� : 1174.659Hz = 0.851ms
#define MI6         6063                // �� : 1318.510Hz = 0.758ms
#define PA6         5727                // �� : 1396.913Hz = 0.716ms
#define PAS6        5407                // ��#: 1479.978Hz = 0.676ms
#define SOL6        5103                // �� : 1567.982Hz = 0.638ms
#define SOLS6       4815                // ��#: 1661.219Hz = 0.602ms
#define LA6         4543                // �� : 1760.000Hz = 0.568ms
#define SI6         4047                // �� : 1975.533Hz = 0.506ms
// 7��Ÿ��
#define DO7         3823                // �� : 2093.005Hz = 0.478ms
#define DOS7        3607                // ��#: 2217.461Hz = 0.451ms
#define RE7         3407                // �� : 2349.318Hz = 0.426ms
#define MI7         3031                // �� : 2637.020Hz = 0.379ms
#define PA7         2863                // �� : 2793.826Hz = 0.357ms
#define PAS7        2703                // ��#: 2959.955Hz = 0.338ms
#define SOL7        2551                // �� : 3135.963Hz = 0.319ms
#define SOLS7       2407                // ��#: 3322.438Hz = 0.301ms
#define LA7         2271                // �� : 3520.000Hz = 0.284ms
#define SI7         2023                // �� : 3951.066Hz = 0.253ms


#else

// 4��Ÿ��
#define DO4         (30591 * 2)         // �� : 261.6256Hz = 3.822ms
#define RE4         (27263 * 2)         // �� : 293.6648Hz = 3.405ms
#define MI4         (24271 * 2)         // �� : 329.6276Hz = 3.034ms
#define PA4         (22927 * 2)         // �� : 349.2282Hz = 2.863ms
#define PAS4        (21615 * 2)         // ��#: 369.9944Hz = 2.703ms
#define SOL4        (20415 * 2)         // �� : 391.9954Hz = 2.551ms
#define LA4         (18175 * 2)         // �� : 440.0000Hz = 2.273ms
#define SI4         (16191 * 2)         // �� : 493.8833Hz = 2.025ms
// 5��Ÿ��
#define DO5         (15295 * 2)         // �� : 523.2511Hz = 1.912ms
#define DOS5        (14432 * 2)         // ��#: 554.3653Hz = 1.804ms
#define RE5         (13631 * 2)         // �� : 587.3295Hz = 1.704ms
#define MI5         (12135 * 2)         // �� : 659.2551Hz = 1.517ms
#define PA5         (11463 * 2)         // �� : 698.4565Hz = 1.433ms
#define PAS5        (10807 * 2)         // ��#: 739.9888Hz = 1.351ms
#define SOL5        (10207 * 2)         // �� : 783.9909Hz = 1.276ms
#define SOLS5       (9631  * 2)         // ��#: 830.6094Hz = 1.204ms
#define LA5         (9087  * 2)         // �� : 880.0000Hz = 1.136ms
#define SI5         (8095  * 2)         // �� : 987.7666Hz = 1.012ms
// 6��Ÿ��
#define DO6         (7639  * 2)         // �� : 1046.502Hz = 0.955ms
#define DOS6        (7215  * 2)         // ��#: 1108.731Hz = 0.902ms
#define RE6         (6807  * 2)         // �� : 1174.659Hz = 0.851ms
#define MI6         (6063  * 2)         // �� : 1318.510Hz = 0.758ms
#define PA6         (5727  * 2)         // �� : 1396.913Hz = 0.716ms
#define PAS6        (5407  * 2)         // ��#: 1479.978Hz = 0.676ms
#define SOL6        (5103  * 2)         // �� : 1567.982Hz = 0.638ms
#define SOLS6       (4815  * 2)         // ��#: 1661.219Hz = 0.602ms
#define LA6         (4543  * 2)         // �� : 1760.000Hz = 0.568ms
#define SI6         (4047  * 2)         // �� : 1975.533Hz = 0.506ms
// 7��Ÿ��
#define DO7         (3823  * 2)         // �� : 2093.005Hz = 0.478ms
#define DOS7        (3607  * 2)         // ��#: 2217.461Hz = 0.451ms
#define RE7         (3407  * 2)         // �� : 2349.318Hz = 0.426ms
#define MI7         (3031  * 2)         // �� : 2637.020Hz = 0.379ms
#define PA7         (2863  * 2)         // �� : 2793.826Hz = 0.357ms
#define PAS7        (2703  * 2)         // ��#: 2959.955Hz = 0.338ms
#define SOL7        (2551  * 2)         // �� : 3135.963Hz = 0.319ms
#define SOLS7       (2407  * 2)         // ��#: 3322.438Hz = 0.301ms
#define LA7         (2271  * 2)         // �� : 3520.000Hz = 0.284ms
#define SI7         (2023  * 2)         // �� : 3951.066Hz = 0.253ms

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

