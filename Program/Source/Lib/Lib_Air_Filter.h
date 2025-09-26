/// @file     Lib_Air_Filter.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    공기청정기 필터 관리 라이브러리 헤더 파일
/// @details  SDK 표준을 준수하는 필터 관리 라이브러리 (Noble30 기능 통합)

#ifndef _LIB_AIR_FILTER_H_
#define _LIB_AIR_FILTER_H_

// 필터 사용 설정
#define AIR_FILTER_USE                  USE    // 필터 기능 사용 여부 - 0(No Use), 1(Use)

/*  공기 필터 제어 Function 사용법
    => Lib_Air_Filter_Module_Initialize();                   // 공기 필터 모듈 초기화
    => Lib_Air_Filter_UpdateUsage(모터RPM);                  // 모터 동작 시 필터 사용량 업데이트
    => Lib_Air_Filter_GetStatus(필터타입);                   // 필터 상태 확인 - FILTER_TYPE_PRE, FILTER_TYPE_MAX2
    => Lib_Air_Filter_GetLifePercent(필터타입);              // 필터 수명 백분율 확인 (0~100%)
    => Lib_Air_Filter_Reset(필터타입);                       // 필터 교체 후 수명 리셋
    => Lib_Air_Filter_SetLifeSpan(필터타입, 주수);           // 필터 수명 설정 (주 단위)

    사용 예)
    Lib_Air_Filter_Module_Initialize();                      // 시스템 초기화 시 호출
    
    while(1) {
        Lib_Air_Filter_Module_Control();                     // 메인 루프에서 호출
        
        // 모터 동작 중 필터 사용량 업데이트
        U16 current_rpm = 1000;                              // 현재 모터 RPM
        Lib_Air_Filter_UpdateUsage(current_rpm);
        
        // 필터 상태 확인
        EFilterStatus pre_status = Lib_Air_Filter_GetStatus(FILTER_TYPE_PRE);
        EFilterStatus max2_status = Lib_Air_Filter_GetStatus(FILTER_TYPE_MAX2);
        
        // 필터 수명 백분율 확인
        U8 pre_percent = Lib_Air_Filter_GetLifePercent(FILTER_TYPE_PRE);     // PRE 필터 수명 %
        U8 max2_percent = Lib_Air_Filter_GetLifePercent(FILTER_TYPE_MAX2);   // MAX2 필터 수명 %
        
        // 필터 교체 완료 후 리셋
        if(filter_replaced_flag) {
            Lib_Air_Filter_Reset(FILTER_TYPE_PRE);           // PRE 필터 수명 리셋
            Lib_Air_Filter_Reset(FILTER_TYPE_MAX2);          // MAX2 필터 수명 리셋
        }
        
        // 필터 수명 설정 (선택사항)
        Lib_Air_Filter_SetLifeSpan(FILTER_TYPE_PRE, 2);     // PRE 필터 2주 수명
        Lib_Air_Filter_SetLifeSpan(FILTER_TYPE_MAX2, 52);   // MAX2 필터 52주(1년) 수명
    }

    주의사항)
    - AIR_FILTER_USE가 USE로 설정되어야 함
    - 필터 상태: GOOD(정상), WARNING(교체임박), REPLACE(교체필요), ERROR(에러)
    - 필터 알람 시작점: 90% 사용 시점
    - EEPROM에 20분마다 자동 저장됨
    - CMM 값은 모터 RPM에 따라 자동 계산됨
*/

// 필터 관련 상수
#define USER_LIFE_SPAN_FILTER1    14     /// @brief PRE 필터 수명: 14일
#define USER_LIFE_SPAN_FILTER2    365    /// @brief MAX2 필터 수명: 365일
#define USER_FILTER1_ALARM        90     /// @brief PRE 필터 알람 시작점: 90%
#define USER_FILTER2_ALARM        90     /// @brief MAX2 필터 알람 시작점: 90%

// 시간 관련 상수
#define TIME_20M                  20     /// @brief EEPROM 저장 주기: 20분
#define TIME_60M                  60     /// @brief 1시간: 60분
#define TIME_A_DAY               8      /// @brief 하루 사용 시간: 8시간

// CMM(Cubic Meter per Minute) 값 정의 (Noble30 확장)
#define CMM_SPEED0               0      /// @brief 정지 상태 CMM
#define CMM_SPEED1               10     /// @brief 1단 CMM
#define CMM_SPEED2               20     /// @brief 2단 CMM
#define CMM_SPEED3               30     /// @brief 3단 CMM
#define CMM_SPEED4               50     /// @brief 4단 CMM

// EEPROM 주소 정의 (프로젝트에 맞게 조정 필요)
#ifndef EEP_ADD_FILTER1_1
#define EEP_ADD_FILTER1_1        50     /// @brief PRE 필터 데이터 주소 1
#define EEP_ADD_FILTER1_2        51     /// @brief PRE 필터 데이터 주소 2
#define EEP_ADD_FILTER1_3        52     /// @brief PRE 필터 데이터 주소 3
#define EEP_ADD_FILTER1_4        53     /// @brief PRE 필터 데이터 주소 4
#define EEP_ADD_FILTER2_1        54     /// @brief MAX2 필터 데이터 주소 1
#define EEP_ADD_FILTER2_2        55     /// @brief MAX2 필터 데이터 주소 2
#define EEP_ADD_FILTER2_3        56     /// @brief MAX2 필터 데이터 주소 3
#define EEP_ADD_FILTER2_4        57     /// @brief MAX2 필터 데이터 주소 4
#define EEP_ADD_FILTER_LIFESPAN  58     /// @brief 필터 수명 설정 주소
#endif

// 디버그 설정
// #define _DEBUG_FILTER_           // 필터 디버그 모드 활성화

/// @brief 필터 상태 정의
typedef enum {
    FILTER_STATUS_GOOD = 0,      /// @brief 정상 상태
    FILTER_STATUS_WARNING,       /// @brief 교체 시기 임박
    FILTER_STATUS_REPLACE,       /// @brief 교체 필요
    FILTER_STATUS_ERROR         /// @brief 에러 상태
} EFilterStatus;

/// @brief 필터 종류 정의
typedef enum {
    FILTER_TYPE_PRE = 0,        /// @brief PRE 필터
    FILTER_TYPE_MAX2,           /// @brief MAX2 필터
    FILTER_TYPE_MAX             /// @brief 필터 개수
} EFilterType;

/// @brief 필터 데이터 구조체
typedef struct {
    U32 gu32LifeSpan;          /// @brief 필터 전체 수명
    U32 gu32Usage;             /// @brief 현재 사용량
    U16 gu16PercentUsage;      /// @brief 사용량 백분율(%)
    U32 gu32Temp;              /// @brief 임시 계산용 변수
    U8  au8Filter[4];          /// @brief EEPROM 저장용 바이트 배열
} SFilterData_T;

// 초기화 및 제어 함수
void Lib_Air_Filter_Initialize(void);
void Lib_Air_Filter_Control(void);
void Lib_Air_Filter_Module_1ms_Control(void);  // 호환성 유지용 (빈 함수)

// 필터 상태 관리 함수
EFilterStatus Lib_Air_Filter_GetStatus(EFilterType type);
U8 Lib_Air_Filter_GetLifePercent(EFilterType type);
void Lib_Air_Filter_Reset(EFilterType type);

// 운영 시간 업데이트 함수
void Lib_Air_Filter_UpdateUsage(U16 motor_rpm);

// 내부 구현 함수 (다른 모듈에서도 사용 가능)
void Lib_Air_Filter_EEPROM_SaveData(void);
void Lib_Air_Filter_EEPROM_LoadData(void);
void Lib_Air_Filter_UpdatePercent(EFilterType type);
void Lib_Air_Filter_AddUsage(EFilterType type, U16 cmm_value);

// Noble30 통합 확장 함수
void Lib_Air_Filter_SetAlarm(EFilterType type);
void Lib_Air_Filter_SetLifeSpan(EFilterType type, U8 weeks);

// 디버그 지원 함수 (조건부 컴파일)
void Lib_Air_Filter_DebugStatus(EFilterType type);
void Lib_Air_Filter_DebugAllStatus(void);

// 표준 모듈 인터페이스 함수 (SDK 표준)
void Lib_Air_Filter_Module_Initialize(void);
void Lib_Air_Filter_Module_Control(void);

#endif // _LIB_AIR_FILTER_H_
