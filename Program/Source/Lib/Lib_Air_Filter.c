/// @file     Lib_Air_Filter.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    공기청정기 필터 관리 라이브러리 구현 파일
/// @details  SDK 표준을 준수하는 필터 관리 라이브러리

#include "Global_Header.h"

#if (AIR_FILTER_USE == USE)

// 전역 변수 (SDK 표준 명명 규칙 적용)
static U8 gu8FilterSaveTimer = TIME_20M;        /// @brief EEPROM 저장 타이머 (분 단위)
static U8 gu8FilterInitialized = FALSE;        /// @brief 초기화 완료 플래그
static SFilterData_T gu8FilterData[FILTER_TYPE_MAX];  /// @brief 필터 데이터 배열

// CMM 테이블 (풍량별 분당 입방미터)
static const U16 gau16CMMTable[] = {
    CMM_SPEED0,  // 정지
    CMM_SPEED1,  // 1단
    CMM_SPEED2,  // 2단
    CMM_SPEED3,  // 3단
    CMM_SPEED4   // 4단
};

/// @brief    필터 라이브러리 초기화 함수
/// @details  SDK 표준에 따라 TimeScheduler를 사용하여 스케줄링 설정
/// @param    void
/// @return   void
void Lib_Air_Filter_Initialize(void)
{
    U32 u32DayMinutes;
    U32 u32Filter1Life;
    U32 u32Filter2Life;
    
    // 초기화 상태 확인
    if (gu8FilterInitialized == TRUE) {
        return;
    }
    
    // 하루 사용량 계산 (분 단위)
    u32DayMinutes = TIME_60M * TIME_A_DAY;
    
    // PRE 필터 수명 계산
    u32Filter1Life = u32DayMinutes * USER_LIFE_SPAN_FILTER1 * CMM_SPEED4;
    
    // MAX2 필터 수명 계산
    u32Filter2Life = u32DayMinutes * USER_LIFE_SPAN_FILTER2 * CMM_SPEED4;
    
    // 필터 데이터 초기화
    gu8FilterData[FILTER_TYPE_PRE].gu32LifeSpan = u32Filter1Life;
    gu8FilterData[FILTER_TYPE_MAX2].gu32LifeSpan = u32Filter2Life;
    
    // EEPROM에서 필터 데이터 로드
    Lib_Air_Filter_EEPROM_LoadData();
    
    // TimeScheduler 설정 (1분 주기로 필터 제어 함수 실행)
#ifdef _FUNC_TEST_
    SetupTimeScheduler(Lib_Air_Filter_Control, 10, ACT_COUNT_INFINITE);  // 테스트 모드: 10ms 주기
#else
    SetupTimeScheduler(Lib_Air_Filter_Control, 60000, ACT_COUNT_INFINITE);  // 정상 모드: 1분 주기
#endif
    
    // 초기화 완료 플래그 설정
    gu8FilterSaveTimer = TIME_20M;
    gu8FilterInitialized = TRUE;
}

/// @brief    필터 모듈 1ms 제어 함수 (제거됨 - TimeScheduler 사용)
/// @details  기존 자체 타이머 시스템을 제거하고 TimeScheduler로 대체
/// @note     이 함수는 더 이상 사용되지 않음
void Lib_Air_Filter_Module_1ms_Control(void)
{
    // TimeScheduler 사용으로 인해 이 함수는 더 이상 필요하지 않음
    // 호환성을 위해 빈 함수로 유지
}

/// @brief    필터 제어 메인 함수 (TimeScheduler로 호출)
/// @details  1분 주기로 TimeScheduler에 의해 자동 호출됨
/// @param    void
/// @return   void
void Lib_Air_Filter_Control(void)
{
    // 초기화 체크
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // EEPROM 저장 타이머 감소
    if (gu8FilterSaveTimer > 0)
    {
        gu8FilterSaveTimer--;
        if (gu8FilterSaveTimer == 0)
        {
            Lib_Air_Filter_EEPROM_SaveData();
            gu8FilterSaveTimer = TIME_20M;
        }
    }
}

/// @brief    필터 상태 조회 함수
/// @details  필터 사용률에 따른 상태 반환 (정상/경고/교체)
/// @param    type 필터 종류
/// @return   EFilterStatus 필터 상태
EFilterStatus Lib_Air_Filter_GetStatus(EFilterType type)
{
    // 초기화 및 타입 유효성 검사
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return FILTER_STATUS_GOOD;
    }
    
    U8 u8Percent = Lib_Air_Filter_GetLifePercent(type);
    U8 u8AlarmPoint = (type == FILTER_TYPE_PRE) ? USER_FILTER1_ALARM : USER_FILTER2_ALARM;
    
    if (u8Percent >= 100)
    {
        return FILTER_STATUS_REPLACE;
    }
    else if (u8Percent >= u8AlarmPoint)
    {
        return FILTER_STATUS_WARNING;
    }
    
    return FILTER_STATUS_GOOD;
}

/// @brief    필터 수명 백분율 조회 함수
/// @details  필터 사용률을 백분율로 반환 (0-100%)
/// @param    type 필터 종류
/// @return   U8 필터 수명 백분율 (0-100%)
U8 Lib_Air_Filter_GetLifePercent(EFilterType type)
{
    // 경계값 검사 강화
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX)
    {
        return 0;
    }
    
    return (U8)gu8FilterData[type].gu16PercentUsage;
}

/// @brief    필터 리셋 함수
/// @details  필터 사용량을 초기화하고 EEPROM에 저장
/// @param    type 필터 종류
/// @return   void
void Lib_Air_Filter_Reset(EFilterType type)
{
    // 경계값 검사 강화
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX)
    {
        return;
    }
    
    // 필터 데이터 초기화
    gu8FilterData[type].gu32Usage = 0;
    gu8FilterData[type].gu16PercentUsage = 0;
    gu8FilterData[type].gu32Temp = 0;
    MEMSET(gu8FilterData[type].au8Filter, 0, 4);
    
    // EEPROM에 저장
    Lib_Air_Filter_EEPROM_SaveData();
}

/// @brief    필터 사용량 업데이트 함수
/// @details  모터 RPM을 기반으로 필터 사용량을 계산하고 업데이트
/// @param    motor_rpm 현재 모터 RPM
/// @return   void
void Lib_Air_Filter_UpdateUsage(U16 motor_rpm)
{
    U8 u8Speed;
    U16 u16CMMValue;
    
    // 초기화 체크
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // RPM을 속도 레벨로 변환 (확장된 매핑)
    if (motor_rpm == 0)
    {
        u8Speed = 0;
    }
    else if (motor_rpm <= 400)
    {
        u8Speed = 1;
    }
    else if (motor_rpm <= 800)
    {
        u8Speed = 2;
    }
    else if (motor_rpm <= 1200)
    {
        u8Speed = 3;
    }
    else
    {
        u8Speed = 4;
    }
    
    // CMM 값 가져오기 (배열 경계 검사)
    if (u8Speed < (sizeof(gau16CMMTable) / sizeof(gau16CMMTable[0]))) {
        u16CMMValue = gau16CMMTable[u8Speed];
    } else {
        u16CMMValue = CMM_SPEED4;  // 기본값
    }
    
    // 각 필터의 사용량 업데이트
    Lib_Air_Filter_AddUsage(FILTER_TYPE_PRE, u16CMMValue);
    Lib_Air_Filter_AddUsage(FILTER_TYPE_MAX2, u16CMMValue);
}

/// @brief    필터 데이터 EEPROM 저장 함수
/// @details  필터 사용량 데이터를 EEPROM에 저장 (SDK 표준 함수 사용)
/// @param    void
/// @return   void
void Lib_Air_Filter_EEPROM_SaveData(void)
{
    U8 au8Data[8];
    U8 i;
    
    // 초기화 체크
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // PRE 필터 데이터 변환
    gu8FilterData[FILTER_TYPE_PRE].gu32Temp = gu8FilterData[FILTER_TYPE_PRE].gu32Usage;
    for (i = 0; i < 4; i++)
    {
        gu8FilterData[FILTER_TYPE_PRE].au8Filter[3-i] = (U8)(gu8FilterData[FILTER_TYPE_PRE].gu32Temp & 0xFF);
        gu8FilterData[FILTER_TYPE_PRE].gu32Temp >>= 8;
    }
    
    // MAX2 필터 데이터 변환
    gu8FilterData[FILTER_TYPE_MAX2].gu32Temp = gu8FilterData[FILTER_TYPE_MAX2].gu32Usage;
    for (i = 0; i < 4; i++)
    {
        gu8FilterData[FILTER_TYPE_MAX2].au8Filter[3-i] = (U8)(gu8FilterData[FILTER_TYPE_MAX2].gu32Temp & 0xFF);
        gu8FilterData[FILTER_TYPE_MAX2].gu32Temp >>= 8;
    }
    
    // EEPROM 저장 데이터 준비
    MEMCPY(&au8Data[0], gu8FilterData[FILTER_TYPE_PRE].au8Filter, 4);
    MEMCPY(&au8Data[4], gu8FilterData[FILTER_TYPE_MAX2].au8Filter, 4);
    
    // SDK 표준 EEPROM 함수 사용
#if (EEPROM_USE == USE)
    EepromPageWrite(EEP_ADD_FILTER1_1, au8Data, 8);
#endif
}

/// @brief    필터 데이터 EEPROM 로드 함수
/// @details  EEPROM에서 필터 사용량 데이터를 로드하고 백분율 계산 (SDK 표준 함수 사용)
/// @param    void
/// @return   void
void Lib_Air_Filter_EEPROM_LoadData(void)
{
#if (EEPROM_USE == USE)
    // PRE 필터 데이터 로드 (SDK 표준 함수 사용)
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[0] = Get_EEPROM_Data(EEP_ADD_FILTER1_1);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[1] = Get_EEPROM_Data(EEP_ADD_FILTER1_2);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[2] = Get_EEPROM_Data(EEP_ADD_FILTER1_3);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[3] = Get_EEPROM_Data(EEP_ADD_FILTER1_4);
    
    // MAX2 필터 데이터 로드
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[0] = Get_EEPROM_Data(EEP_ADD_FILTER2_1);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[1] = Get_EEPROM_Data(EEP_ADD_FILTER2_2);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[2] = Get_EEPROM_Data(EEP_ADD_FILTER2_3);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[3] = Get_EEPROM_Data(EEP_ADD_FILTER2_4);
#else
    // EEPROM 미사용 시 기본값으로 초기화
    MEMSET(gu8FilterData[FILTER_TYPE_PRE].au8Filter, 0, 4);
    MEMSET(gu8FilterData[FILTER_TYPE_MAX2].au8Filter, 0, 4);
#endif
    
    // 데이터 변환 및 백분율 계산
    Lib_Air_Filter_UpdatePercent(FILTER_TYPE_PRE);
    Lib_Air_Filter_UpdatePercent(FILTER_TYPE_MAX2);
}

/// @brief    필터 사용량 백분율 업데이트 함수
/// @details  바이트 배열 데이터를 U32로 변환하고 사용률 백분율 계산
/// @param    type 필터 종류
/// @return   void
void Lib_Air_Filter_UpdatePercent(EFilterType type)
{
    U32 u32Usage = 0;
    U8 i;
    
    // 경계값 검사
    if (type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // 바이트 배열을 U32로 변환
    for (i = 0; i < 4; i++)
    {
        u32Usage = (u32Usage << 8) | gu8FilterData[type].au8Filter[i];
    }
    
    // 사용량 저장
    gu8FilterData[type].gu32Usage = u32Usage;
    
    // 백분율 계산 (0으로 나누기 방지)
    if (gu8FilterData[type].gu32LifeSpan > 0) {
        gu8FilterData[type].gu16PercentUsage = (U16)((gu8FilterData[type].gu32Usage * 100) / gu8FilterData[type].gu32LifeSpan);
    } else {
        gu8FilterData[type].gu16PercentUsage = 0;
    }
    
    // 최대값 제한
    if (gu8FilterData[type].gu16PercentUsage > 100)
    {
        gu8FilterData[type].gu16PercentUsage = 100;
    }
}

/// @brief    필터 사용량 증가 함수
/// @details  현재 CMM 값만큼 필터 사용량을 증가시키고 백분율 업데이트
/// @param    type 필터 종류
/// @param    cmm_value 현재 CMM 값
/// @return   void
void Lib_Air_Filter_AddUsage(EFilterType type, U16 cmm_value)
{
    // 경계값 검사
    if (type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // 최대 사용량 체크
    if (gu8FilterData[type].gu32Usage >= gu8FilterData[type].gu32LifeSpan)
    {
        return;
    }
    
    // 사용량 증가
    gu8FilterData[type].gu32Usage += cmm_value;
    
    // 최대값 제한
    if (gu8FilterData[type].gu32Usage > gu8FilterData[type].gu32LifeSpan)
    {
        gu8FilterData[type].gu32Usage = gu8FilterData[type].gu32LifeSpan;
    }
    
    // 백분율 업데이트 (0으로 나누기 방지)
    if (gu8FilterData[type].gu32LifeSpan > 0) {
        gu8FilterData[type].gu16PercentUsage = (U16)((gu8FilterData[type].gu32Usage * 100) / gu8FilterData[type].gu32LifeSpan);
    }
}

/// @brief    필터 알람 설정 함수
/// @details  필터를 교체 필요 상태로 강제 설정 (알람 목적)
/// @param    type 필터 종류
/// @return   void
void Lib_Air_Filter_SetAlarm(EFilterType type)
{
    U8 au8AlarmData[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    
    // 경계값 검사
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // 필터 데이터를 0xFF로 설정 (교체 필요 상태)
    MEMCPY(gu8FilterData[type].au8Filter, au8AlarmData, 4);
    
    // 최대 사용량으로 설정
    gu8FilterData[type].gu32Usage = gu8FilterData[type].gu32LifeSpan;
    gu8FilterData[type].gu16PercentUsage = 100;
    
    // EEPROM에 즉시 저장
    Lib_Air_Filter_EEPROM_SaveData();
}

/// @brief    필터 수명 동적 설정 함수
/// @details  WiFi나 외부 명령으로 필터 교체 주기를 동적으로 변경
/// @param    type 필터 종류
/// @param    weeks 필터 수명 (주 단위)
/// @return   void
void Lib_Air_Filter_SetLifeSpan(EFilterType type, U8 weeks)
{
    U32 u32DayMinutes;
    U32 u32NewLifeSpan;
    
    // 경계값 검사
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // 유효 범위 체크 (2~4주)
    if (weeks < 2 || weeks > 4) {
        weeks = 3;  // 기본값
    }
    
    // 새로운 수명 계산
    u32DayMinutes = TIME_60M * TIME_A_DAY;
    u32NewLifeSpan = u32DayMinutes * (weeks * 7) * CMM_SPEED4;
    
    // 필터 수명 업데이트
    gu8FilterData[type].gu32LifeSpan = u32NewLifeSpan;
    
    // 백분율 재계산
    Lib_Air_Filter_UpdatePercent(type);
    
#if (EEPROM_USE == USE)
    // 수명 설정값을 EEPROM에 저장
    Set_EEPROM_Data(EEP_ADD_FILTER_LIFESPAN, weeks);
#endif
}

/// @brief    필터 디버그 상태 출력 함수
/// @details  디버그 모드에서 필터 상태를 상세히 출력
/// @param    type 필터 종류
/// @return   void
void Lib_Air_Filter_DebugStatus(EFilterType type)
{
#ifdef _DEBUG_FILTER_
    const char* filterName[] = {"PRE", "MAX2"};
    
    // 경계값 검사
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return;
    }
    
    printf("=== Filter Debug Info [%s] ===\r\n", filterName[type]);
    printf("Usage: %lu / %lu\r\n", gu8FilterData[type].gu32Usage, gu8FilterData[type].gu32LifeSpan);
    printf("Percent: %d%%\r\n", gu8FilterData[type].gu16PercentUsage);
    printf("Status: %d\r\n", Lib_Air_Filter_GetStatus(type));
    printf("Raw Data: %02X %02X %02X %02X\r\n", 
           gu8FilterData[type].au8Filter[0], gu8FilterData[type].au8Filter[1],
           gu8FilterData[type].au8Filter[2], gu8FilterData[type].au8Filter[3]);
    printf("============================\r\n");
#endif
}

/// @brief    모든 필터 상태 출력 함수
/// @details  모든 필터의 상태를 한 번에 출력
/// @param    void
/// @return   void
void Lib_Air_Filter_DebugAllStatus(void)
{
#ifdef _DEBUG_FILTER_
    printf("\r\n=== All Filter Status ===\r\n");
    printf("Initialized: %s\r\n", gu8FilterInitialized ? "YES" : "NO");
    printf("Save Timer: %d min\r\n", gu8FilterSaveTimer);
    
    for (U8 i = 0; i < FILTER_TYPE_MAX; i++) {
        Lib_Air_Filter_DebugStatus((EFilterType)i);
    }
    printf("========================\r\n\r\n");
#endif
}

///****************************************************************************
/// SDK 표준 모듈 인터페이스 함수들
///****************************************************************************

/// @brief    필터 모듈 초기화 함수 (SDK 표준)
/// @details  SDK 표준 모듈 패턴에 따른 초기화 함수
/// @param    void
/// @return   void
void Lib_Air_Filter_Module_Initialize(void)
{
#if (AIR_FILTER_USE == USE)
    Lib_Air_Filter_Initialize();
#endif
}

/// @brief    필터 모듈 제어 함수 (SDK 표준)
/// @details  SDK 표준 모듈 패턴에 따른 메인 제어 함수
/// @param    void
/// @return   void
void Lib_Air_Filter_Module_Control(void)
{
#if (AIR_FILTER_USE == USE)
    // TimeScheduler가 자동으로 Lib_Air_Filter_Control() 호출하므로
    // 여기서는 추가적인 제어만 수행
    
    // 필요시 추가 제어 로직 구현
#endif
}

#endif // AIR_FILTER_USE == USE
