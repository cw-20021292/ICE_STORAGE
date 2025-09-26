/// @file     Hal_Debug.h
/// @date     2025/04/20
/// @author   SHM
/// @brief    ����� ���(UART2)�� ���� HAL ��� ����

#ifndef _HAL_DEBUG_H_
#define _HAL_DEBUG_H_

#define DEBUG_UART_USE                                  USE

/*  Debug ��� Function ����
    => Hal_Debug_Initialize();                               // Debug ��� �ʱ�ȭ
    => Hal_Debug_Control();                                  // Debug ���� ���� (While������ ȣ��)
    => Hal_Debug_OnCommand("��ɾ�");                        // ��ɾ� ���� ����
    => printf("�޽���");                                      // ����� �޽��� ���

    ��� ��)
    Hal_Debug_Initialize();                                  // �ý��� �ʱ�ȭ �� ȣ��
    
    while(1) {
        Hal_Debug_Control();                                 // ���� �������� ȣ��
        
        // ����� �޽��� ���
        printf("Temperature: %d\n", temperature_value);
        
        // ���� ��ɾ� ����
        Hal_Debug_OnCommand("help");                         // ���� ���
        Hal_Debug_OnCommand("motor 1000");                   // ���� RPM ����
        Hal_Debug_OnCommand("buzzer on");                    // ���� ON
    }

    ���ǻ���)
    - DEBUG_UART_USE�� USE�� �����Ǿ�� ��
    - UART2�� ���� ����ϹǷ� UART2 ������ �Ϸ�Ǿ�� ��
*/

#if (DEBUG_UART_USE == USE)

/// @brief    ����� �޽��� �ִ� ����
#define DEBUG_MAX_MESSAGE_LENGTH    256

/// @brief    ����� �޽��� Ÿ�� ����
typedef enum {
    DEBUG_INFO = 0,    /// @brief �Ϲ� ���� �޽���
    DEBUG_WARN,        /// @brief ��� �޽���
    DEBUG_ERROR,       /// @brief ���� �޽���
    DEBUG_DEBUG        /// @brief ����� �޽���
} E_DEBUG_TYPE;

#define MAX_INDEX    100
#define DEBUG_PORT   UART2

typedef struct _sCmd SHELL_CMD;

struct _sCmd {
    char* Cmd;                   ///< Ŀ�ǵ� ���ڿ�
    U8 (*CmdFunc)(U8 argc, void** argv);  ///< Ŀ�ǵ� ���� �Լ�
    char* CmdHelp;              ///< Ŀ�ǵ� ����
};

/// @brief    ǥ�� ��� �Լ� �������̵� (printf ��� �Լ�)
int putchar(int c);

/// @brief    ����� ��� �ʱ�ȭ
void Hal_Debug_Initialize(void);

/// @brief    ����� ��� ���� ����
void Hal_Debug_Control(void);

/// @brief    Ŀ�ǵ� ó��
/// @param    cmd_str - ó���� ��ɾ� ���ڿ�
void Hal_Debug_OnCommand(const char* cmd_str);

// UART2 ó�� �Լ���
void hal_debug_uart2_init(void);
void hal_debug_uart2_process_received_byte(void);
void hal_debug_uart2_handle_overrun(uint16_t rx_data);

// �⺻ Ŀ�ǵ� �Լ���
U8 Hal_Debug_TestCMD(U8 argc, void** argv);
U8 Hal_Debug_CmdHello(U8 argc, void** argv);
U8 Hal_Debug_CmdHelp(U8 argc, void** argv);
U8 Hal_Debug_CmdReset(U8 argc, void** argv);
U8 Hal_Debug_CmdVersion(U8 argc, void** argv);
U8 Hal_Debug_MotorCMD(U8 argc, void** argv);
U8 Hal_Debug_CmdTemp(U8 argc, void** argv);
U8 Hal_Debug_CmdData(U8 argc, void** argv);
U8 Hal_Debug_StepCMD(U8 argc, void** argv);
U8 Hal_Debug_BuzzerCMD(U8 argc, void** argv);
U8 Hal_Debug_EEPROMCMD(U8 argc, void** argv);
U8 Hal_Debug_UARTCMD(U8 argc, void** argv);


#endif  // DEBUG_UART_USE == USE
#endif  // _HAL_DEBUG_H_

