/// @file     Hal_Debug.c
/// @date     2025/04/20
/// @author   SHM
/// @brief    ����� ���(UART2)�� ���� HAL �ҽ� ����

#include "Global_Header.h"
#include "Config_UART2.h"

#if (DEBUG_UART_USE == USE)

// ����� ��ũ�� ����
#define DEBUG_COMPANY_NAME    "COWAY"
#define DEBUG_VERSION_STR     "1.0.0"

// UART2 �ܺ� ������ ����
extern volatile uint8_t * gp_uart2_tx_address;
extern volatile uint16_t g_uart2_tx_count;
extern volatile uint8_t * gp_uart2_rx_address;
extern volatile uint16_t g_uart2_rx_count;
extern uint16_t g_uart2_rx_length;

// static U16 index = 0;
// static U8 RevData[MAX_INDEX];

// ���� ������ ��ɾ� ���� ����
static char g_cmd_buffer[MAX_INDEX];
static char* g_cmd_argv[10];
static U8 g_cmd_argc = 0;

// UART2 ���� ���� ������
// static uint8_t g_uart2_tx_state = 0;
// static uint8_t test_cnt = 0;
#define RX_BUFFER_SIZE 64
static uint8_t g_rx_buffer[RX_BUFFER_SIZE];
static uint16_t g_rx_index = 0;

// Ŀ�ǵ� ���̺� ����
const SHELL_CMD CmdShell[] = {
    {
        "hello", Hal_Debug_CmdHello, "Show Hello Message"
    },
    {
        "temp", Hal_Debug_CmdTemp, "Show Temp Message"
    },
    {
        "help", Hal_Debug_CmdHelp, "Show Help Message"
    },
    {
        "reset", Hal_Debug_CmdReset, "System Reset"
    },
    {
        "version", Hal_Debug_CmdVersion, "Show Version Info"
    },
    {
        "test", Hal_Debug_TestCMD, "Test Command"
    },
    {
        "motor", Hal_Debug_MotorCMD, "Motor Command"
    },
    {
        "data", Hal_Debug_CmdData, "Show Sensor Data (Usage: data [sensor_type])"
    },
    {
        "step", Hal_Debug_StepCMD, "Step Motor Command"
    },
    {
        "buzz", Hal_Debug_BuzzerCMD, "Buzzer Command"
    },
    {
        "eep", Hal_Debug_EEPROMCMD, "EEPROM Command"
    },
    {
        "uart", Hal_Debug_UARTCMD, "UART Command"
    },
    {
        (void*)0, (void*)0, (void*)0
    }
};

/*--------------------------------------------------------------
@ Function Name : putchar
@ Function ID   : F-003-001-007
@ Description   : printf ��� �Լ� 
@ Return Value  : 0~0xFF
--------------------------------------------------------------*/
int putchar(int c) 
{ 
    U16 timeout;
    // ���� �Ϸ� ��� (Ÿ�Ӿƿ� ����)
    for (timeout = 0; timeout < 65530; timeout++) {
        // SSR10�� 6�� ��Ʈ(TEMP �÷���)�� Ȯ���Ͽ� ���� ������ �Ϸ�Ǿ����� Ȯ��
        if ((SSR10 & (1 << 6)) != 0) {
            NOP();
        } 
        else {
            break;
        }
    }
    // UART2�� �� ���� ����
    TXD2 = (U8)c;
    return c; 
}

// UART2 �ʱ�ȭ �Լ�
void hal_debug_uart2_init(void) {
    // �ʱ� UART2 ���� ���� ����
    static uint8_t rx_buf[64];  // ���� ����
    gp_uart2_rx_address = rx_buf;
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // ���� ũ��
}

// UART2 ���� �Ϸ� ó�� �Լ�
void hal_debug_uart2_process_received_byte(void) {
    uint8_t rx_char = *(gp_uart2_rx_address - 1);  // ������ ���ŵ� ����Ʈ
    
    // �齺���̽� ó�� (ASCII 8)
    if (rx_char == 0x08) {
        if (g_rx_index > 0) {
            // ���ۿ��� ������ ���� ����
            g_rx_index--;
            g_rx_buffer[g_rx_index] = '\0';
            
            // �͹̳ο� �齺���̽� ȿ�� ���� (�齺���̽� + ���� + �齺���̽�)
            static const uint8_t bs_seq[] = {0x08, 0x20, 0x08}; // �齺���̽�, ����, �齺���̽�
            R_Config_UART2_Send((uint8_t*)bs_seq, sizeof(bs_seq));
        }
    } 
    // ���� ���� ó��
    else {
        // ���ŵ� ���ڸ� ��� ����
        R_Config_UART2_Send((uint8_t*)(gp_uart2_rx_address - 1), 1);
        
        // �ٹٲ� ���ڰ� ���� ������ ��ɾ�� ó��
        if (rx_char == '\r' || rx_char == '\n') {
            // �ٹٲ� ó��: \r\n�� �����Ͽ� �͹̳ο� �� �� ǥ��
            static const uint8_t newline[] = "\r\n";
            R_Config_UART2_Send((uint8_t*)newline, 2);
            
            // ���⼭ ��ü ��ɾ� ó�� (g_rx_buffer�� ����� ���ڿ�)
            Hal_Debug_OnCommand((const char*)g_rx_buffer);
            // ��ɾ� ó�� �� ���� �ʱ�ȭ
            g_rx_index = 0;
            g_rx_buffer[g_rx_index] = '\0';
            
        }
        // �Ϲ� ���ڴ� ���ۿ� ����
        else if (g_rx_index < RX_BUFFER_SIZE - 1) {
            g_rx_buffer[g_rx_index++] = rx_char;
            g_rx_buffer[g_rx_index] = '\0';  // �׻� �� ���� ���� �߰�
        }
    }
    
    // ���� ������ ���� ���� �ʱ�ȭ (�ε����� ����)
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // 1����Ʈ�� ����
}

// UART2 ������ ó�� �Լ�
void hal_debug_uart2_handle_overrun(uint16_t rx_data) {
    // ������ �߻� �� ���� �ٽ� �ʱ�ȭ
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // �ٽ� ���� �غ�
}

/// @brief    ����� ��� �ʱ�ȭ
void Hal_Debug_Initialize(void)
{
    
    
}

/// @brief    Ŀ�ǵ� ó��
void Hal_Debug_OnCommand(const char* cmd_str)
{
    U8 cnt = 0;
    U16 i;
    
    // ��� ���� ���� �ʱ�ȭ
    g_cmd_argc = 0;
    for (i = 0; i < 10; i++) {
        g_cmd_argv[i] = NULL;
    }
    
    // cmd_str ����
    for (i = 0; i < MAX_INDEX - 1 && cmd_str[i] != '\0'; i++) {
        g_cmd_buffer[i] = cmd_str[i];
    }
    g_cmd_buffer[i] = '\0';
    
    printf("Command: %s\r\n", g_cmd_buffer);
    
    // ù ��° ��ū (��ɾ�) ����
    char* ptr = (char*)strtok(g_cmd_buffer, " ");
    
    // ��ū�� ���� �迭�� ����
    while (ptr != NULL && g_cmd_argc < 10) {
        g_cmd_argv[g_cmd_argc] = ptr;
        g_cmd_argc++;
        ptr = (char*)strtok(NULL, " ");
    }
        
    // ��ɾ� ã��
    U8 cmd_found = 0;
    for (cnt = 0; CmdShell[cnt].Cmd != 0; cnt++) {
        if (strcmp((const char*)g_cmd_argv[0], (const char*)CmdShell[cnt].Cmd) == 0) {
            // �Լ� ȣ�� �� ���� ���� ���� �迭 �ּҸ� ����
            CmdShell[cnt].CmdFunc(g_cmd_argc, (void**)g_cmd_argv);
            cmd_found = 1;
            break;
        }
    }
    
    // ��ɾ ã�� ���� ���
    if (!cmd_found) {
        printf("Unknown command: %s\r\n", g_cmd_argv[0]);
        printf("Type 'help' for available commands\r\n");
    }
}

// �⺻ Ŀ�ǵ� �Լ���
U8 Hal_Debug_CmdHello(U8 argc, void** argv)
{
    printf("Hello %s\r\n", DEBUG_COMPANY_NAME);
    return TRUE;
}
U8 Hal_Debug_CmdTemp(U8 argc, void** argv)
{
    // printf("Temp : %d\r\n", getTemp());
    return TRUE;
}

U8 Hal_Debug_CmdHelp(U8 argc, void** argv)
{
    U8 cnt = 0;
    printf("Available Commands:\r\n");
    
    for (cnt = 0; CmdShell[cnt].Cmd != 0; cnt++) {
        printf("%s - %s\r\n", CmdShell[cnt].Cmd, CmdShell[cnt].CmdHelp);
    }
    return TRUE;
}

U8 Hal_Debug_CmdReset(U8 argc, void** argv)
{
    printf("System Reset\r\n");
    while (1);  // �ý��� ����
    // return TRUE; // ���� �Ұ����� �ڵ�
}

U8 Hal_Debug_CmdVersion(U8 argc, void** argv)
{
    printf("Version: %s\r\n", DEBUG_VERSION_STR);
    return TRUE;
}

U8 Hal_Debug_TestCMD(U8 argc, void** argv)
{
    printf("Test CMD\r\n");
    printf("argc = [%d]\r\n", g_cmd_argc);
    
    // ���� �迭���� ���� ������ ����
    U16 i;
    for(i = 0; i < g_cmd_argc; i++) {
        printf("[%d] ARGV : [%s]\r\n", i, g_cmd_argv[i]);
    }
    
    return TRUE;
}

U8 Hal_Debug_UARTCMD(U8 argc, void** argv)
{
    static uint8_t g_version_check_response[22] = {
        0x02,  // STX
        0x56,  // CMD_VERSION_CHECK
        0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x11, 0x10,
        0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        // CRC�� �۽� �� ����Ͽ� �߰�
    };


    printf("UART Command\r\n");
    R_Config_UART0_Send(g_version_check_response, 22);

    return TRUE;
}

U8 Hal_Debug_MotorCMD(U8 argc, void** argv)
{
    U16 Motor_RPM = 0;
    U16 Motor_PWM = 0;
    
    // ���� ���� üũ �߰�
    if (argc < 2) {
        printf("Usage: motor [on|off|pwm <value>|<rpm>]\r\n");
        return FALSE;
    }
    
    if(strcmp(g_cmd_argv[1],(const char *)"on")==0){
        printf("Motor ON \r\n");
        Set_BLDCMotor_Power(MOTOR_ON);
    }
    else if(strcmp((const  char*)g_cmd_argv[1],(const  char *)"off")==0){
        printf("Motor OFF \r\n");
        Set_BLDCMotor_Power(MOTOR_OFF);
    }
    else if(strcmp((const  char*)g_cmd_argv[1],(const  char *)"pwm")==0){
        // PWM ��ɾ�� �߰� ���ڰ� �ʿ�
        if (argc < 3) {
            printf("Usage: motor pwm <value>\r\n");
            return FALSE;
        }
        // strtoul�� ����ϰ� U16���� ����� ĳ����
        Motor_PWM = (U16)strtoul(g_cmd_argv[2], NULL, 10);
        printf("Motor PWM : %d \r\n", Motor_PWM);
        Set_BLDCMotor_PWM(Motor_PWM);
    }
    else{
        // strtoul�� ����ϰ� U16���� ����� ĳ����
        Motor_RPM = (U16)strtoul(g_cmd_argv[1], NULL, 10);
        printf("Motor RPM : %d \r\n", Motor_RPM);
        SetMotorRPM(Motor_RPM);
    }
    return TRUE;
}

U8 Hal_Debug_EEPROMCMD(U8 argc, void** argv)
{
    U8 Id = 0;
    U8 Data = 0;
    
    // ���� ���� üũ �߰�
    if (argc < 2) {
        printf("Usage: eep [r|w|load|check] [args...]\r\n");
        return FALSE;
    }
    
    if(strcmp(g_cmd_argv[1],(const char *)"r")==0){
        // �б� ��ɾ�� �߰� ���ڰ� �ʿ�
        if (argc < 3) {
            printf("Usage: eep r <id>\r\n");
            return FALSE;
        }
        Id = (U8)strtoul(g_cmd_argv[2], NULL, 10);
        EI(); 
        // ���� I2C ����� ���� EEPROM���� ������ �б�
        EepromByteRead(Id);
        // ��� �о�� ������ ��������
        Data = Get_EEPROM_Data(Id);
        printf("EEPROM Read (I2C): ID[%d] = %d\r\n", Id, Data);
    }
    // else if(strcmp(g_cmd_argv[1],(const char *)"rd")==0){  // ���� �б� �߰�
    //     Id = (U8)strtoul(g_cmd_argv[2], NULL, 10);
    //     EepromByteRead(Id);  // ���� EEPROM���� �б�
    //     Data = SEEPROM_Data.au8Data[Id];
    //     printf("EEPROM Read (Direct): ID[%d] = %d\r\n", Id, Data);
    // }
    else if(strcmp(g_cmd_argv[1],(const char *)"w")==0){
        // ���� ��ɾ�� ID�� Data ���ڰ� �ʿ�
        if (argc < 4) {
            printf("Usage: eep w <id> <data>\r\n");
            return FALSE;
        }
        // printf("EEPROM Write\r\n");
        Id = (U8)strtoul(g_cmd_argv[2], NULL, 10);
        Data = (U8)strtoul(g_cmd_argv[3], NULL, 10);
        // EepromByteWrite(Id, Data);
        EI();
        Set_EEPROM_Data(Id, Data);
        

        // printf("Written: ID[%d] = %d\r\n", Id, Data);
    }
    else if(strcmp(g_cmd_argv[1],(const char *)"load")==0) {
        printf("Loading EEPROM data...\r\n");
        EI();
        EEPROM_Data_Load();
    }
    else if(strcmp(g_cmd_argv[1],(const char *)"ei")==0) {
        printf("Enable Interrupt...\r\n");
        EI();
    }
    else if(strcmp(g_cmd_argv[1],(const char *)"di")==0) {
        printf("Disable Interrupt...\r\n");
        DI();
    }
    else if(strcmp(g_cmd_argv[1],(const char *)"check")==0){  // ���� üũ �߰�
        if (Get_EepromErrorCheck() == TRUE) {
            printf("EEPROM Status: ERROR\r\n");
        } else {
            printf("EEPROM Status: OK\r\n");
        }
    }
    else{
        printf("Usage:\r\n");
        printf("  eep r <id>     - Read from cache\r\n");
        printf("  eep w <id> <data> - Write data\r\n");
        printf("  eep load       - Load all data from EEPROM\r\n");
        printf("  eep check      - Check EEPROM status\r\n");
    }
    return TRUE;
}

U8 Hal_Debug_BuzzerCMD(U8 argc, void** argv)
{
    
    if(strcmp(g_cmd_argv[1],(const char *)"on")==0){
        printf("Power ON \r\n");
        TURN_ON_BUZZER();
        Set_BuzzerSelect(ON);
    }
    else if(strcmp((const  char*)g_cmd_argv[1],(const  char *)"off")==0){
        printf("Power OFF \r\n");
        TURN_OFF_BUZZER();
        Set_BuzzerSelect(OFF);
    }
    else if(strcmp(g_cmd_argv[1],(const char *)"a")==0){
        Set_BuzzerSelect(ON);
    }
    return TRUE;
}

U8 Hal_Debug_CmdData(U8 argc, void** argv)
{
    // Check if sensor type is specified
    if (argc < 2) {
        printf("Usage: data [sensor_type]\r\n");
        printf("Available sensors:\r\n");
        printf("  sen54 - SEN54 environmental sensor\r\n");
        return TRUE;
    }

    // Check sensor type
    if (strcmp((const char*)g_cmd_argv[1], "sen54") == 0) {
        printf("SEN54: PM1.0: %d PM2.5: %d PM10: %d VOC: %d Temp: %d Humid: %d\r\n",
               getSen54PM1_0(),
               getSen54PM2_5(),
               getSen54PM10(),
               getSen54Voc(),
               getSen54Temp(),
               getSen54Humid());
    } else {
        printf("Unknown sensor type: %s\r\n", g_cmd_argv[1]);
        printf("Type 'data' for available sensors\r\n");
    }
    
    return TRUE;
}

U8 Hal_Debug_StepCMD(U8 argc, void** argv)
{
    U16 Step;
    U8 Direction;
    U8 StepMotorID;

    printf("Step Motor Command\r\n");
    StepMotorID = (U8)strtoul(g_cmd_argv[1], NULL, 10);

    // ���� ���ڿ� üũ (��ҹ��� ��� ���)
    if (strcmp(g_cmd_argv[2], "cw") == 0 || strcmp(g_cmd_argv[2], "CW") == 0) {
        Direction = STEPMOTOR_CW;
    }
    else if (strcmp(g_cmd_argv[2], "ccw") == 0 || strcmp(g_cmd_argv[2], "CCW") == 0) {
        Direction = STEPMOTOR_CCW;
    }
    else {
        printf("�߸��� �����Դϴ�. 'cw' �Ǵ� 'ccw'�� �Է��ϼ���.\r\n");
        return FALSE;
    }

    // strtoul�� ����ϰ� U16���� ����� ĳ����
    Step = (U16)strtoul(g_cmd_argv[3], NULL, 10);
    printf("Direction: %s, Step Count: %d\r\n", (Direction == STEPMOTOR_CW) ? "CW" : "CCW", Step);
    
    Set_Stepmotor_Move(StepMotorID, Direction, Step);

    return TRUE;
}

#endif  // DEBUG_UART_USE == USE






