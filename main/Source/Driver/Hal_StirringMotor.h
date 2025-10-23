/// @file     Hal_StirringMotor.h
/// @date     2025/04/08
/// @author   Jaejin Ham
/// @brief    ���� ���� Control file


#ifndef _HAL_STIRRING_MOTOR_H_
#define _HAL_STIRRING_MOTOR__H_


/*  ���� ���� ���� Function ����
    => Set_StirringMotorControl(Motor ID, On/Off data, Delay Time);
    => Set_StirringMotorPWM(Motor ID, mu16RegValue);       // Motor ID�� �ش��ϴ� PWM ���� - mu16RegValue���� �������Ϳ� ���� ���� ��

    ��� ��)
    Set_StirringMotorControl(Motor ID, ON, 0);              // Motor ID�� ���� ���ͷ� 0�� ������ �� ON
    Set_StirringMotorControl(Motor ID, OFF,0);              // Motor ID�� ���� ���͸� 0�� ������ �� OFF
    Set_StirringMotorControl(Motor ID, ON, 5);              // Motor ID�� ���� ���͸� 500ms ������ �� ON
    Set_StirringMotorControl(Motor ID, OFF,5);              // Motor ID�� ���� ���͸� 500ms ������ �� OFF

    Set_StirringMotorPWM(Motor ID, 0x3777);                 // Motor ID�� Motor�� ���а��� PWM ���� �������Ϳ� 0x3777�� ��

    Hal_All_StirringMotor_Off();                            // ��� Motor OFF
*/


// ���� ���� �ִ� 5�� ��� ����
typedef enum
{
    STIRRING_MOTOR_ID_NONE = 0,                         // 0 :
    STIRRING_MOTOR_ID_1,                                // 1 :
    STIRRING_MOTOR_ID_MAX                               // 6 Max, 6�� ������ �ȵ�
} EStirringMotor_ID_T;

#define STIRRING_MOTOR_COUNT                            0           // ���� ���� ���� - MAX 5

// ���� ���� port ����
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
//#define REGISTER_DEFINE_STIRRING_MOTOR_1_PWM          TDR04           // PWM ���� �������� ����, �̻��� 0���� ó��
//#define STIRRING_MOTOR_1_PWM_START()                  exPwm_Start();  // Code Generate�� PWM ���� Start�Լ� ��ġ
//#define STIRRING_MOTOR_1_PWM_STOP()                   exPwm_Stop();   // Code Generate�� PWM ���� Stop�Լ� ��ġ

#if (STIRRING_MOTOR_COUNT >= 1)
#define STIRRING_MOTOR_PWM_NO_USE                       0
#define STIRRING_MOTOR_PWM_REGISTER_USE                 1
#define STIRRING_MOTOR_PWM_TRIG_USE                     2
// ���ݸ���1
#define STIRRING_MOTOR_1_PWM_USE                        STIRRING_MOTOR_PWM_NO_USE   // ���ݸ���1 PWM ���� ���� - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define REGISTER_DEFINE_STIRRING_MOTOR_1_PWM            0           // PWM ���� �������� ����, �̻��� 0���� ó��
#define STIRRING_MOTOR_1_PWM_START()                    0           // Code Generate�� PWM ���� Start�Լ� ��ġ
#define STIRRING_MOTOR_1_PWM_STOP()                     0           // Code Generate�� PWM ���� Stop�Լ� ��ġ
#define TIME_DEFINE_STIRRING_MOTOR_1_PWM_ON_DELAY       0           // PWM ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_1_PWM_OFF_DELAY      0           // PWM ���� �� ���� OFF ������ Delay @100ms
#define PORT_DEFINE_STIRRING_MOTOR_1_VOLTAGE_ON         0           // ���� ���� ���� ���� ON ���� Port
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define PORT_DEFINE_STIRRING_MOTOR_1_TRIG               0           // TRIG Port ����, �̻��� 0���� ó��
#define TIME_DEFINE_STIRRING_MOTOR_1_TRIG_ON_DELAY      0           // TRIG Port ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_1_TRIG_OFF_DELAY     0           // TRIG Port ���� �� ���� OFF ������ Delay @100ms
#else
#endif
#define PORT_DEFINE_STIRRING_MOTOR_1                    0           // ���� ���� ON ���� Port
#endif

#if (STIRRING_MOTOR_COUNT >= 2)
// ���ݸ���2
#define STIRRING_MOTOR_2_PWM_USE                        STIRRING_MOTOR_PWM_NO_USE   // ���ݸ���2 PWM ���� ���� - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define REGISTER_DEFINE_STIRRING_MOTOR_2_PWM            0           // PWM ���� �������� ����, �̻��� 0���� ó��
#define STIRRING_MOTOR_2_PWM_START()                    0           // Code Generate�� PWM ���� Start�Լ� ��ġ
#define STIRRING_MOTOR_2_PWM_STOP()                     0           // Code Generate�� PWM ���� Stop�Լ� ��ġ
#define TIME_DEFINE_STIRRING_MOTOR_2_PWM_ON_DELAY       0           // PWM ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_2_PWM_OFF_DELAY      0           // PWM ���� �� ���� OFF ������ Delay @100ms
#define PORT_DEFINE_STIRRING_MOTOR_2_VOLTAGE_ON         0           // ���� ���� ���� ���� ON ���� Port
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define PORT_DEFINE_STIRRING_MOTOR_2_TRIG               0           // TRIG Port ����, �̻��� 0���� ó��
#define TIME_DEFINE_STIRRING_MOTOR_2_TRIG_ON_DELAY      0           // TRIG Port ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_2_TRIG_OFF_DELAY     0           // TRIG Port ���� �� ���� OFF ������ Delay @100ms
#else
#endif
#define PORT_DEFINE_STIRRING_MOTOR_2                    0           // ���� ���� ON ���� Port
#endif

#if (STIRRING_MOTOR_COUNT >= 3)
// ���ݸ���3
#define STIRRING_MOTOR_3_PWM_USE                        STIRRING_MOTOR_PWM_NO_USE   // ���ݸ���3 PWM ���� ���� - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define REGISTER_DEFINE_STIRRING_MOTOR_3_PWM            0           // PWM ���� �������� ����, �̻��� 0���� ó��
#define STIRRING_MOTOR_3_PWM_START()                    0           // Code Generate�� PWM ���� Start�Լ� ��ġ
#define STIRRING_MOTOR_3_PWM_STOP()                     0           // Code Generate�� PWM ���� Stop�Լ� ��ġ
#define TIME_DEFINE_STIRRING_MOTOR_3_PWM_ON_DELAY       0           // PWM ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_3_PWM_OFF_DELAY      0           // PWM ���� �� ���� OFF ������ Delay @100ms
#define PORT_DEFINE_STIRRING_MOTOR_3_VOLTAGE_ON         0           // ���� ���� ���� ���� ON ���� Port
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define PORT_DEFINE_STIRRING_MOTOR_3_TRIG               0           // TRIG Port ����, �̻��� 0���� ó��
#define TIME_DEFINE_STIRRING_MOTOR_3_TRIG_ON_DELAY      0           // TRIG Port ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_3_TRIG_OFF_DELAY     0           // TRIG Port ���� �� ���� OFF ������ Delay @100ms
#else
#endif
#define PORT_DEFINE_STIRRING_MOTOR_3                    0           // ���� ���� ON ���� Port
#endif

#if (STIRRING_MOTOR_COUNT >= 4)
// ���ݸ���4
#define STIRRING_MOTOR_4_PWM_USE                        STIRRING_MOTOR_PWM_NO_USE   // ���ݸ���4 PWM ���� ���� - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define REGISTER_DEFINE_STIRRING_MOTOR_4_PWM            0           // PWM ���� �������� ����, �̻��� 0���� ó��
#define STIRRING_MOTOR_4_PWM_START()                    0           // Code Generate�� PWM ���� Start�Լ� ��ġ
#define STIRRING_MOTOR_4_PWM_STOP()                     0           // Code Generate�� PWM ���� Stop�Լ� ��ġ
#define TIME_DEFINE_STIRRING_MOTOR_4_PWM_ON_DELAY       0           // PWM ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_4_PWM_OFF_DELAY      0           // PWM ���� �� ���� OFF ������ Delay @100ms
#define PORT_DEFINE_STIRRING_MOTOR_4_VOLTAGE_ON         0           // ���� ���� ���� ���� ON ���� Port
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define PORT_DEFINE_STIRRING_MOTOR_4_TRIG               0           // TRIG Port ����, �̻��� 0���� ó��
#define TIME_DEFINE_STIRRING_MOTOR_4_TRIG_ON_DELAY      0           // TRIG Port ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_4_TRIG_OFF_DELAY     0           // TRIG Port ���� �� ���� OFF ������ Delay @100ms
#else
#endif
#define PORT_DEFINE_STIRRING_MOTOR_4                    0           // ���� ���� ON ���� Port
#endif

#if (STIRRING_MOTOR_COUNT >= 5)
// ���ݸ���5
#define STIRRING_MOTOR_5_PWM_USE                        STIRRING_MOTOR_PWM_NO_USE   // ���ݸ���5 PWM ���� ���� - 0(No Use), 1(Use PWM REGISTER), 2(Use TRIG Port)
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define REGISTER_DEFINE_STIRRING_MOTOR_5_PWM            0           // PWM ���� �������� ����, �̻��� 0���� ó��
#define STIRRING_MOTOR_5_PWM_START()                    0           // Code Generate�� PWM ���� Start�Լ� ��ġ
#define STIRRING_MOTOR_5_PWM_STOP()                     0           // Code Generate�� PWM ���� Stop�Լ� ��ġ
#define TIME_DEFINE_STIRRING_MOTOR_5_PWM_ON_DELAY       0           // PWM ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_5_PWM_OFF_DELAY      0           // PWM ���� �� ���� OFF ������ Delay @100ms
#define PORT_DEFINE_STIRRING_MOTOR_5_VOLTAGE_ON         0           // ���� ���� ���� ���� ON ���� Port
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define PORT_DEFINE_STIRRING_MOTOR_5_TRIG               0           // TRIG Port ����, �̻��� 0���� ó��
#define TIME_DEFINE_STIRRING_MOTOR_5_TRIG_ON_DELAY      0           // TRIG Port ���� �� ���� ON ������ Delay @100ms
#define TIME_DEFINE_STIRRING_MOTOR_5_TRIG_OFF_DELAY     0           // TRIG Port ���� �� ���� OFF ������ Delay @100ms
#else
#endif
#define PORT_DEFINE_STIRRING_MOTOR_5                    0           // ���� ���� ON ���� Port
#endif


#if (STIRRING_MOTOR_COUNT > 0)

#define STIRRING_MOTOR_ID_MAX_COUNT             STIRRING_MOTOR_ID_MAX

#if (STIRRING_MOTOR_COUNT >= 1)
// DC Pump Port Define
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define STIRRING_MOTOR_1_PWM_REGISTER           REGISTER_DEFINE_STIRRING_MOTOR_1_PWM
#define STIRRING_MOTOR_1_PWM_ON_DELAY           TIME_DEFINE_STIRRING_MOTOR_1_PWM_ON_DELAY     // @100ms
#define STIRRING_MOTOR_1_PWM_OFF_DELAY          TIME_DEFINE_STIRRING_MOTOR_1_PWM_OFF_DELAY    // @100ms
#define P_VARIABLE_VOLTAGE_1_ON                 PORT_DEFINE_STIRRING_MOTOR_1_VOLTAGE_ON
#define TURN_ON_VARIABLE_VOLTAGE_1()            { P_VARIABLE_VOLTAGE_1_ON = 1; }
#define TURN_OFF_VARIABLE_VOLTAGE_1()           { P_VARIABLE_VOLTAGE_1_ON = 0; }
#define GET_STATUS_VARIABLE_VOLTAGE_1()         ( P_VARIABLE_VOLTAGE_1_ON )
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define P_STIRRING_MOTOR_1_TRIG                 PORT_DEFINE_STIRRING_MOTOR_1_TRIG
#define TURN_ON_STIRRING_MOTOR_1_TRIG()         { P_STIRRING_MOTOR_1_TRIG = 1; }
#define TURN_OFF_STIRRING_MOTOR_1_TRIG()        { P_STIRRING_MOTOR_1_TRIG = 0; }
#define GET_STATUS_STIRRING_MOTOR_1_TRIG()      ( P_STIRRING_MOTOR_1_TRIG )
#define STIRRING_MOTOR_1_TRIG_ON_DELAY          TIME_DEFINE_STIRRING_MOTOR_1_TRIG_ON_DELAY    // @100ms
#define STIRRING_MOTOR_1_TRIG_OFF_DELAY         TIME_DEFINE_STIRRING_MOTOR_1_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_STIRRING_MOTOR_1                      PORT_DEFINE_STIRRING_MOTOR_1
#define TURN_ON_STIRRING_MOTOR_1()              { P_STIRRING_MOTOR_1 = 1; }
#define TURN_OFF_STIRRING_MOTOR_1()             { P_STIRRING_MOTOR_1 = 0; }
#define GET_STATUS_STIRRING_MOTOR_1()           ( P_STIRRING_MOTOR_1 )
#endif

#if (STIRRING_MOTOR_COUNT >= 2)
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define STIRRING_MOTOR_2_PWM_REGISTER           REGISTER_DEFINE_STIRRING_MOTOR_2_PWM
#define STIRRING_MOTOR_2_PWM_ON_DELAY           TIME_DEFINE_STIRRING_MOTOR_2_PWM_ON_DELAY     // @100ms
#define STIRRING_MOTOR_2_PWM_OFF_DELAY          TIME_DEFINE_STIRRING_MOTOR_2_PWM_OFF_DELAY    // @100ms
#define P_VARIABLE_VOLTAGE_2_ON                 PORT_DEFINE_STIRRING_MOTOR_2_VOLTAGE_ON
#define TURN_ON_VARIABLE_VOLTAGE_2()            { P_VARIABLE_VOLTAGE_2_ON = 1; }
#define TURN_OFF_VARIABLE_VOLTAGE_2()           { P_VARIABLE_VOLTAGE_2_ON = 0; }
#define GET_STATUS_VARIABLE_VOLTAGE_2()         ( P_VARIABLE_VOLTAGE_2_ON )
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define P_STIRRING_MOTOR_2_TRIG                 PORT_DEFINE_STIRRING_MOTOR_2_TRIG
#define TURN_ON_STIRRING_MOTOR_2_TRIG()         { P_STIRRING_MOTOR_2_TRIG = 1; }
#define TURN_OFF_STIRRING_MOTOR_2_TRIG()        { P_STIRRING_MOTOR_2_TRIG = 0; }
#define GET_STATUS_STIRRING_MOTOR_2_TRIG()      ( P_STIRRING_MOTOR_2_TRIG )
#define STIRRING_MOTOR_2_TRIG_ON_DELAY          TIME_DEFINE_STIRRING_MOTOR_2_TRIG_ON_DELAY    // @100ms
#define STIRRING_MOTOR_2_TRIG_OFF_DELAY         TIME_DEFINE_STIRRING_MOTOR_2_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_STIRRING_MOTOR_2                      PORT_DEFINE_STIRRING_MOTOR_2
#define TURN_ON_STIRRING_MOTOR_2()              { P_STIRRING_MOTOR_2 = 1; }
#define TURN_OFF_STIRRING_MOTOR_2()             { P_STIRRING_MOTOR_2 = 0; }
#define GET_STATUS_STIRRING_MOTOR_2()           ( P_STIRRING_MOTOR_2 )
#endif

#if (STIRRING_MOTOR_COUNT >= 3)
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define STIRRING_MOTOR_3_PWM_REGISTER           REGISTER_DEFINE_STIRRING_MOTOR_3_PWM
#define STIRRING_MOTOR_3_PWM_ON_DELAY           TIME_DEFINE_STIRRING_MOTOR_3_PWM_ON_DELAY     // @100ms
#define STIRRING_MOTOR_3_PWM_OFF_DELAY          TIME_DEFINE_STIRRING_MOTOR_3_PWM_OFF_DELAY    // @100ms
#define P_VARIABLE_VOLTAGE_3_ON                 PORT_DEFINE_STIRRING_MOTOR_3_VOLTAGE_ON
#define TURN_ON_VARIABLE_VOLTAGE_3()            { P_VARIABLE_VOLTAGE_3_ON = 1; }
#define TURN_OFF_VARIABLE_VOLTAGE_3()           { P_VARIABLE_VOLTAGE_3_ON = 0; }
#define GET_STATUS_VARIABLE_VOLTAGE_3()         ( P_VARIABLE_VOLTAGE_3_ON )
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define P_STIRRING_MOTOR_3_TRIG                 PORT_DEFINE_STIRRING_MOTOR_3_TRIG
#define TURN_ON_STIRRING_MOTOR_3_TRIG()         { P_STIRRING_MOTOR_3_TRIG = 1; }
#define TURN_OFF_STIRRING_MOTOR_3_TRIG()        { P_STIRRING_MOTOR_3_TRIG = 0; }
#define GET_STATUS_STIRRING_MOTOR_3_TRIG()      ( P_STIRRING_MOTOR_3_TRIG )
#define STIRRING_MOTOR_3_TRIG_ON_DELAY          TIME_DEFINE_STIRRING_MOTOR_3_TRIG_ON_DELAY    // @100ms
#define STIRRING_MOTOR_3_TRIG_OFF_DELAY         TIME_DEFINE_STIRRING_MOTOR_3_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_STIRRING_MOTOR_3                      PORT_DEFINE_STIRRING_MOTOR_3
#define TURN_ON_STIRRING_MOTOR_3()              { P_STIRRING_MOTOR_3 = 1; }
#define TURN_OFF_STIRRING_MOTOR_3()             { P_STIRRING_MOTOR_3 = 0; }
#define GET_STATUS_STIRRING_MOTOR_3()           ( P_STIRRING_MOTOR_3 )
#endif

#if (STIRRING_MOTOR_COUNT >= 4)
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define STIRRING_MOTOR_4_PWM_REGISTER           REGISTER_DEFINE_STIRRING_MOTOR_4_PWM
#define STIRRING_MOTOR_4_PWM_ON_DELAY           TIME_DEFINE_STIRRING_MOTOR_4_PWM_ON_DELAY     // @100ms
#define STIRRING_MOTOR_4_PWM_OFF_DELAY          TIME_DEFINE_STIRRING_MOTOR_4_PWM_OFF_DELAY    // @100ms
#define P_VARIABLE_VOLTAGE_4_ON                 PORT_DEFINE_STIRRING_MOTOR_4_VOLTAGE_ON
#define TURN_ON_VARIABLE_VOLTAGE_4()            { P_VARIABLE_VOLTAGE_4_ON = 1; }
#define TURN_OFF_VARIABLE_VOLTAGE_4()           { P_VARIABLE_VOLTAGE_4_ON = 0; }
#define GET_STATUS_VARIABLE_VOLTAGE_4()         ( P_VARIABLE_VOLTAGE_4_ON )
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define P_STIRRING_MOTOR_4_TRIG                 PORT_DEFINE_STIRRING_MOTOR_4_TRIG
#define TURN_ON_STIRRING_MOTOR_4_TRIG()         { P_STIRRING_MOTOR_4_TRIG = 1; }
#define TURN_OFF_STIRRING_MOTOR_4_TRIG()        { P_STIRRING_MOTOR_4_TRIG = 0; }
#define GET_STATUS_STIRRING_MOTOR_4_TRIG()      ( P_STIRRING_MOTOR_4_TRIG )
#define STIRRING_MOTOR_4_TRIG_ON_DELAY          TIME_DEFINE_STIRRING_MOTOR_4_TRIG_ON_DELAY    // @100ms
#define STIRRING_MOTOR_4_TRIG_OFF_DELAY         TIME_DEFINE_STIRRING_MOTOR_4_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_STIRRING_MOTOR_4                      PORT_DEFINE_STIRRING_MOTOR_4
#define TURN_ON_STIRRING_MOTOR_4()              { P_STIRRING_MOTOR_4 = 1; }
#define TURN_OFF_STIRRING_MOTOR_4()             { P_STIRRING_MOTOR_4 = 0; }
#define GET_STATUS_STIRRING_MOTOR_4()           ( P_STIRRING_MOTOR_4 )
#endif

#if (STIRRING_MOTOR_COUNT >= 5)
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
#define STIRRING_MOTOR_5_PWM_REGISTER           REGISTER_DEFINE_STIRRING_MOTOR_5_PWM
#define STIRRING_MOTOR_5_PWM_ON_DELAY           TIME_DEFINE_STIRRING_MOTOR_5_PWM_ON_DELAY     // @100ms
#define STIRRING_MOTOR_5_PWM_OFF_DELAY          TIME_DEFINE_STIRRING_MOTOR_5_PWM_OFF_DELAY    // @100ms
#define P_VARIABLE_VOLTAGE_5_ON                 PORT_DEFINE_STIRRING_MOTOR_5_VOLTAGE_ON
#define TURN_ON_VARIABLE_VOLTAGE_5()            { P_VARIABLE_VOLTAGE_5_ON = 1; }
#define TURN_OFF_VARIABLE_VOLTAGE_5()           { P_VARIABLE_VOLTAGE_5_ON = 0; }
#define GET_STATUS_VARIABLE_VOLTAGE_5()         ( P_VARIABLE_VOLTAGE_5_ON )
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
#define P_STIRRING_MOTOR_5_TRIG                 PORT_DEFINE_STIRRING_MOTOR_5_TRIG
#define TURN_ON_STIRRING_MOTOR_5_TRIG()         { P_STIRRING_MOTOR_5_TRIG = 1; }
#define TURN_OFF_STIRRING_MOTOR_5_TRIG()        { P_STIRRING_MOTOR_5_TRIG = 0; }
#define GET_STATUS_STIRRING_MOTOR_5_TRIG()      ( P_STIRRING_MOTOR_5_TRIG )
#define STIRRING_MOTOR_5_TRIG_ON_DELAY          TIME_DEFINE_STIRRING_MOTOR_5_TRIG_ON_DELAY    // @100ms
#define STIRRING_MOTOR_5_TRIG_OFF_DELAY         TIME_DEFINE_STIRRING_MOTOR_5_TRIG_OFF_DELAY   // @100ms
#else
#endif
#define P_STIRRING_MOTOR_5                      PORT_DEFINE_STIRRING_MOTOR_5
#define TURN_ON_STIRRING_MOTOR_5()              { P_STIRRING_MOTOR_5 = 1; }
#define TURN_OFF_STIRRING_MOTOR_5()             { P_STIRRING_MOTOR_5 = 0; }
#define GET_STATUS_STIRRING_MOTOR_5()           ( P_STIRRING_MOTOR_5 )
#endif

#define STIRRING_MOTOR_CONTROL_TIME_PERIOD      100         // 100ms@1ms

void Hal_StirringMotor_Initialize(void);
void StirringMotorControlTimer(void);
void StirringMotorControl(void);
void Set_StirringMotorControl(U8 mu8MotorID, U8 mu8Status, U16 mu16Delay);
void Hal_All_StirringMotor_Off(void);
void Hal_StirringMotorControl(U8 mu8MotorID);
U8 Get_StirringMotorStatus_HW(U8 mu8MotorID);
void Set_StirringMotorPWM(U8 mu8MotorID, U16 mu16RegValue);

#if (STIRRING_MOTOR_COUNT >= 1)
void Hal_StirringMotor_1_On(void);
void Hal_StirringMotor_1_Off(void);
#endif

#if (STIRRING_MOTOR_COUNT >= 2)
void Hal_StirringMotor_2_On(void);
void Hal_StirringMotor_2_Off(void);
#endif

#if (STIRRING_MOTOR_COUNT >= 3)
void Hal_StirringMotor_3_On(void);
void Hal_StirringMotor_3_Off(void);
#endif

#if (STIRRING_MOTOR_COUNT >= 4)
void Hal_StirringMotor_4_On(void);
void Hal_StirringMotor_4_Off(void);
#endif

#if (STIRRING_MOTOR_COUNT >= 5)
void Hal_StirringMotor_5_On(void);
void Hal_StirringMotor_5_Off(void);
#endif

#endif

void Hal_StirringMoter_Module_Initialize(void);
void Hal_StirringMotor_Module_1ms_Control(void);
void Hal_StirringMotor_Module_Control(void);

#endif

