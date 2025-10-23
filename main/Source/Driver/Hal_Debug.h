/// @file     Hal_Debug.h
/// @date     2025/04/20
/// @author   SHM
/// @brief    디버그 통신(UART2)을 위한 HAL 헤더 파일

#ifndef _HAL_DEBUG_H_
#define _HAL_DEBUG_H_

#define DEBUG_UART_USE                                  USE

/*  Debug 통신 Function 사용법
    => Hal_Debug_Initialize();                               // Debug 모듈 초기화
    => Hal_Debug_Control();                                  // Debug 메인 제어 (While문에서 호출)
    => Hal_Debug_OnCommand("명령어");                        // 명령어 직접 실행
    => printf("메시지");                                      // 디버그 메시지 출력

    사용 예)
    Hal_Debug_Initialize();                                  // 시스템 초기화 시 호출
    
    while(1) {
        Hal_Debug_Control();                                 // 메인 루프에서 호출
        
        // 디버그 메시지 출력
        printf("Temperature: %d\n", temperature_value);
        
        // 직접 명령어 실행
        Hal_Debug_OnCommand("help");                         // 도움말 출력
        Hal_Debug_OnCommand("motor 1000");                   // 모터 RPM 설정
        Hal_Debug_OnCommand("buzzer on");                    // 부저 ON
    }

    주의사항)
    - DEBUG_UART_USE가 USE로 설정되어야 함
    - UART2를 통해 통신하므로 UART2 설정이 완료되어야 함
*/

#if (DEBUG_UART_USE == USE)

/// @brief    디버그 메시지 최대 길이
#define DEBUG_MAX_MESSAGE_LENGTH    256

/// @brief    디버그 메시지 타입 정의
typedef enum {
    DEBUG_INFO = 0,    /// @brief 일반 정보 메시지
    DEBUG_WARN,        /// @brief 경고 메시지
    DEBUG_ERROR,       /// @brief 에러 메시지
    DEBUG_DEBUG        /// @brief 디버그 메시지
} E_DEBUG_TYPE;

#define MAX_INDEX    100
#define DEBUG_PORT   UART2

typedef struct _sCmd SHELL_CMD;

struct _sCmd {
    char* Cmd;                   ///< 커맨드 문자열
    U8 (*CmdFunc)(U8 argc, void** argv);  ///< 커맨드 실행 함수
    char* CmdHelp;              ///< 커맨드 도움말
};

/// @brief    표준 출력 함수 오버라이드 (printf 사용 함수)
int putchar(int c);

/// @brief    디버그 모듈 초기화
void Hal_Debug_Initialize(void);

/// @brief    디버그 모듈 메인 제어
void Hal_Debug_Control(void);

/// @brief    커맨드 처리
/// @param    cmd_str - 처리할 명령어 문자열
void Hal_Debug_OnCommand(const char* cmd_str);

// UART2 처리 함수들
void hal_debug_uart2_init(void);
void hal_debug_uart2_process_received_byte(void);
void hal_debug_uart2_handle_overrun(uint16_t rx_data);

// 기본 커맨드 함수들
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

