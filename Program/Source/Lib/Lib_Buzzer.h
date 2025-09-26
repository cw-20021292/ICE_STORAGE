/// @file   Lib_Buzzer.h
/// @date   2025/04/10
/// @author Jaejin Ham
/// @brief  Buzzer ���� ���� ���� ��� file

#ifndef _LIB_BUZZER_H_
#define _LIB_BUZZER_H_

#if (BUZZER_USE == USE)


/*  Buzzer ���� Function ����
    => Set_BuzzerMode(mu8Mode);             // Buzzer �� ��� ��� ���� : mu8Mode - 0(�������), 1(�� ��� ���)
    => Set_BuzzerSelect(mu8Type);           // Buzzer �� ��� : mu8Type - ����� �� ����

    ��� ��)
    Set_BuzzerSelect(BUZZER_POWER_ON);      // ���� �ΰ��� ���
    Set_BuzzerSelect(BUZZER_OFF);           // �� ��� ����
*/

#define BUZZER_WATER                        0               // ������ Buzzer�� ���
#define BUZZER_AIR                          1               // û���� Buzzer�� ���

#define BUZZER_PRODUCT                      BUZZER_WATER

// ����� ����
#if (BUZZER_PRODUCT == BUZZER_WATER)

// ������ ��� Buzzer
#define BUZZER_OFF                          0
#define BUZZER_POWER_ON                     1
#define BUZZER_EFFLUENT                     2
#define BUZZER_EFFLUENT_END                 3
#define BUZZER_COOL_SETUP                   4
#define BUZZER_COOL_CANCEL                  5
#define BUZZER_SELECT                       6
#define BUZZER_ERROR                        7
#define BUZZER_SETUP                        8
#define BUZZER_CANCEL                       9
#define BUZZER_EFFLUENT_CONTINUE            10
#define BUZZER_STERILIZE_START              11
#define BUZZER_STERILIZE_END                12
#define BUZZER_AP_CONNECT                   13
#define BUZZER_SERVER_CONNECT               14
#define BUZZER_EXTRA_HEAT_END               15          // ��� ���� �Ϸ���
#define BUZZER_MEMENTO_1                    16
#define BUZZER_MEMENTO_2                    17
#define BUZZER_MEMENTO_3                    18
#define BUZZER_MEMENTO_4                    19
#define BUZZER_MEMENTO_5                    20

#elif (BUZZER_PRODUCT == BUZZER_AIR)

// û���� ��� ����
#define BUZZER_OFF                          0
#define BUZZER_POWER_ON                     1
#define BUZZER_POWER_OFF                    2
#define BUZZER_SET                          3
#define BUZZER_CLEAR                        4
#define BUZZER_KEY_IN                       5
#define BUZZER_ERROR                        6
#define BUZZER_SERVER_OK                    7
#define BUZZER_AP_OK                        8
#define BUZZER_SERVER_START                 9
#define BUZZER_MEMENTO_1                    10
#define BUZZER_MEMENTO_2                    11
#define BUZZER_MEMENTO_3                    12
#define BUZZER_MEMENTO_4                    13
#define BUZZER_MEMENTO_5                    14
#define BUZZER_AC_ON                        15

#endif

#define BUZZER_CONTROL_TIME_PERIOD          10          // 10ms@1ms

void BuzzerInitialize(void);
void Set_BuzzerMode(U8 mu8Mode);
void Set_BuzzerSelect(U8 mu8Type);
void BuzzerTimeCounter(void);
void BuzzerControl(void);

#endif

#endif

