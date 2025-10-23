/// @file     Hal_Debug.c
/// @date     2025/04/20
/// @author   SHM
/// @brief    디버그 통신(UART2)을 위한 HAL 소스 파일

#include "Global_Header.h"
#include "Config_UART2.h"

#if (DEBUG_UART_USE == USE)

// 디버그 매크로 정의
#define DEBUG_COMPANY_NAME    "COWAY"
#define DEBUG_VERSION_STR     "1.0.0"

// UART2 외부 변수들 참조
extern volatile uint8_t * gp_uart2_tx_address;
extern volatile uint16_t g_uart2_tx_count;
extern volatile uint8_t * gp_uart2_rx_address;
extern volatile uint16_t g_uart2_rx_count;
extern uint16_t g_uart2_rx_length;

// static U16 index = 0;
// static U8 RevData[MAX_INDEX];

// 전역 변수로 명령어 인자 관리
static char g_cmd_buffer[MAX_INDEX];
static char* g_cmd_argv[10];
static U8 g_cmd_argc = 0;

// UART2 관련 전역 변수들
// static uint8_t g_uart2_tx_state = 0;
// static uint8_t test_cnt = 0;
#define RX_BUFFER_SIZE 64
static uint8_t g_rx_buffer[RX_BUFFER_SIZE];
static uint16_t g_rx_index = 0;

// 커맨드 테이블 정의
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
@ Description   : printf 사용 함수 
@ Return Value  : 0~0xFF
--------------------------------------------------------------*/
int putchar(int c) 
{ 
    U16 timeout;
    // 전송 완료 대기 (타임아웃 포함)
    for (timeout = 0; timeout < 65530; timeout++) {
        // SSR10의 6번 비트(TEMP 플래그)를 확인하여 이전 전송이 완료되었는지 확인
        if ((SSR10 & (1 << 6)) != 0) {
            NOP();
        } 
        else {
            break;
        }
    }
    // UART2로 한 문자 전송
    TXD2 = (U8)c;
    return c; 
}

// UART2 초기화 함수
void hal_debug_uart2_init(void) {
    // 초기 UART2 수신 버퍼 설정
    static uint8_t rx_buf[64];  // 수신 버퍼
    gp_uart2_rx_address = rx_buf;
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // 버퍼 크기
}

// UART2 수신 완료 처리 함수
void hal_debug_uart2_process_received_byte(void) {
    uint8_t rx_char = *(gp_uart2_rx_address - 1);  // 마지막 수신된 바이트
    
    // 백스페이스 처리 (ASCII 8)
    if (rx_char == 0x08) {
        if (g_rx_index > 0) {
            // 버퍼에서 마지막 문자 제거
            g_rx_index--;
            g_rx_buffer[g_rx_index] = '\0';
            
            // 터미널에 백스페이스 효과 적용 (백스페이스 + 공백 + 백스페이스)
            static const uint8_t bs_seq[] = {0x08, 0x20, 0x08}; // 백스페이스, 공백, 백스페이스
            R_Config_UART2_Send((uint8_t*)bs_seq, sizeof(bs_seq));
        }
    } 
    // 정상 문자 처리
    else {
        // 수신된 문자를 즉시 에코
        R_Config_UART2_Send((uint8_t*)(gp_uart2_rx_address - 1), 1);
        
        // 줄바꿈 문자가 오면 완전한 명령어로 처리
        if (rx_char == '\r' || rx_char == '\n') {
            // 줄바꿈 처리: \r\n을 에코하여 터미널에 새 줄 표시
            static const uint8_t newline[] = "\r\n";
            R_Config_UART2_Send((uint8_t*)newline, 2);
            
            // 여기서 전체 명령어 처리 (g_rx_buffer에 저장된 문자열)
            Hal_Debug_OnCommand((const char*)g_rx_buffer);
            // 명령어 처리 후 버퍼 초기화
            g_rx_index = 0;
            g_rx_buffer[g_rx_index] = '\0';
            
        }
        // 일반 문자는 버퍼에 저장
        else if (g_rx_index < RX_BUFFER_SIZE - 1) {
            g_rx_buffer[g_rx_index++] = rx_char;
            g_rx_buffer[g_rx_index] = '\0';  // 항상 널 종료 문자 추가
        }
    }
    
    // 다음 수신을 위해 버퍼 초기화 (인덱스는 유지)
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // 1바이트씩 수신
}

// UART2 오버런 처리 함수
void hal_debug_uart2_handle_overrun(uint16_t rx_data) {
    // 오버런 발생 시 버퍼 다시 초기화
    g_uart2_rx_count = 0;
    g_uart2_rx_length = 1;  // 다시 수신 준비
}

/// @brief    디버그 모듈 초기화
void Hal_Debug_Initialize(void)
{
    
    
}

/// @brief    커맨드 처리
void Hal_Debug_OnCommand(const char* cmd_str)
{
    U8 cnt = 0;
    U16 i;
    
    // 모든 전역 변수 초기화
    g_cmd_argc = 0;
    for (i = 0; i < 10; i++) {
        g_cmd_argv[i] = NULL;
    }
    
    // cmd_str 복사
    for (i = 0; i < MAX_INDEX - 1 && cmd_str[i] != '\0'; i++) {
        g_cmd_buffer[i] = cmd_str[i];
    }
    g_cmd_buffer[i] = '\0';
    
    printf("Command: %s\r\n", g_cmd_buffer);
    
    // 첫 번째 토큰 (명령어) 추출
    char* ptr = (char*)strtok(g_cmd_buffer, " ");
    
    // 토큰을 전역 배열에 저장
    while (ptr != NULL && g_cmd_argc < 10) {
        g_cmd_argv[g_cmd_argc] = ptr;
        g_cmd_argc++;
        ptr = (char*)strtok(NULL, " ");
    }
        
    // 명령어 찾기
    U8 cmd_found = 0;
    for (cnt = 0; CmdShell[cnt].Cmd != 0; cnt++) {
        if (strcmp((const char*)g_cmd_argv[0], (const char*)CmdShell[cnt].Cmd) == 0) {
            // 함수 호출 시 인자 수와 전역 배열 주소만 전달
            CmdShell[cnt].CmdFunc(g_cmd_argc, (void**)g_cmd_argv);
            cmd_found = 1;
            break;
        }
    }
    
    // 명령어를 찾지 못한 경우
    if (!cmd_found) {
        printf("Unknown command: %s\r\n", g_cmd_argv[0]);
        printf("Type 'help' for available commands\r\n");
    }
}

// 기본 커맨드 함수들
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
    while (1);  // 시스템 리셋
    // return TRUE; // 도달 불가능한 코드
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
    
    // 전역 배열에서 직접 데이터 참조
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
        // CRC는 송신 시 계산하여 추가
    };


    printf("UART Command\r\n");
    R_Config_UART0_Send(g_version_check_response, 22);

    return TRUE;
}

U8 Hal_Debug_MotorCMD(U8 argc, void** argv)
{
    U16 Motor_RPM = 0;
    U16 Motor_PWM = 0;
    
    // 인자 개수 체크 추가
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
        // PWM 명령어는 추가 인자가 필요
        if (argc < 3) {
            printf("Usage: motor pwm <value>\r\n");
            return FALSE;
        }
        // strtoul을 사용하고 U16으로 명시적 캐스팅
        Motor_PWM = (U16)strtoul(g_cmd_argv[2], NULL, 10);
        printf("Motor PWM : %d \r\n", Motor_PWM);
        Set_BLDCMotor_PWM(Motor_PWM);
    }
    else{
        // strtoul을 사용하고 U16으로 명시적 캐스팅
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
    
    // 인자 개수 체크 추가
    if (argc < 2) {
        printf("Usage: eep [r|w|load|check] [args...]\r\n");
        return FALSE;
    }
    
    if(strcmp(g_cmd_argv[1],(const char *)"r")==0){
        // 읽기 명령어는 추가 인자가 필요
        if (argc < 3) {
            printf("Usage: eep r <id>\r\n");
            return FALSE;
        }
        Id = (U8)strtoul(g_cmd_argv[2], NULL, 10);
        EI(); 
        // 실제 I2C 통신을 통해 EEPROM에서 데이터 읽기
        EepromByteRead(Id);
        // 방금 읽어온 데이터 가져오기
        Data = Get_EEPROM_Data(Id);
        printf("EEPROM Read (I2C): ID[%d] = %d\r\n", Id, Data);
    }
    // else if(strcmp(g_cmd_argv[1],(const char *)"rd")==0){  // 직접 읽기 추가
    //     Id = (U8)strtoul(g_cmd_argv[2], NULL, 10);
    //     EepromByteRead(Id);  // 직접 EEPROM에서 읽기
    //     Data = SEEPROM_Data.au8Data[Id];
    //     printf("EEPROM Read (Direct): ID[%d] = %d\r\n", Id, Data);
    // }
    else if(strcmp(g_cmd_argv[1],(const char *)"w")==0){
        // 쓰기 명령어는 ID와 Data 인자가 필요
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
    else if(strcmp(g_cmd_argv[1],(const char *)"check")==0){  // 상태 체크 추가
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

    // 방향 문자열 체크 (대소문자 모두 허용)
    if (strcmp(g_cmd_argv[2], "cw") == 0 || strcmp(g_cmd_argv[2], "CW") == 0) {
        Direction = STEPMOTOR_CW;
    }
    else if (strcmp(g_cmd_argv[2], "ccw") == 0 || strcmp(g_cmd_argv[2], "CCW") == 0) {
        Direction = STEPMOTOR_CCW;
    }
    else {
        printf("잘못된 방향입니다. 'cw' 또는 'ccw'를 입력하세요.\r\n");
        return FALSE;
    }

    // strtoul을 사용하고 U16으로 명시적 캐스팅
    Step = (U16)strtoul(g_cmd_argv[3], NULL, 10);
    printf("Direction: %s, Step Count: %d\r\n", (Direction == STEPMOTOR_CW) ? "CW" : "CCW", Step);
    
    Set_Stepmotor_Move(StepMotorID, Direction, Step);

    return TRUE;
}

#endif  // DEBUG_UART_USE == USE






