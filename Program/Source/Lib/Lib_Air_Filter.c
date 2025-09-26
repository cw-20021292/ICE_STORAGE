/// @file     Lib_Air_Filter.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    ����û���� ���� ���� ���̺귯�� ���� ����
/// @details  SDK ǥ���� �ؼ��ϴ� ���� ���� ���̺귯��

#include "Global_Header.h"

#if (AIR_FILTER_USE == USE)

// ���� ���� (SDK ǥ�� ��� ��Ģ ����)
static U8 gu8FilterSaveTimer = TIME_20M;        /// @brief EEPROM ���� Ÿ�̸� (�� ����)
static U8 gu8FilterInitialized = FALSE;        /// @brief �ʱ�ȭ �Ϸ� �÷���
static SFilterData_T gu8FilterData[FILTER_TYPE_MAX];  /// @brief ���� ������ �迭

// CMM ���̺� (ǳ���� �д� �Թ����)
static const U16 gau16CMMTable[] = {
    CMM_SPEED0,  // ����
    CMM_SPEED1,  // 1��
    CMM_SPEED2,  // 2��
    CMM_SPEED3,  // 3��
    CMM_SPEED4   // 4��
};

/// @brief    ���� ���̺귯�� �ʱ�ȭ �Լ�
/// @details  SDK ǥ�ؿ� ���� TimeScheduler�� ����Ͽ� �����ٸ� ����
/// @param    void
/// @return   void
void Lib_Air_Filter_Initialize(void)
{
    U32 u32DayMinutes;
    U32 u32Filter1Life;
    U32 u32Filter2Life;
    
    // �ʱ�ȭ ���� Ȯ��
    if (gu8FilterInitialized == TRUE) {
        return;
    }
    
    // �Ϸ� ��뷮 ��� (�� ����)
    u32DayMinutes = TIME_60M * TIME_A_DAY;
    
    // PRE ���� ���� ���
    u32Filter1Life = u32DayMinutes * USER_LIFE_SPAN_FILTER1 * CMM_SPEED4;
    
    // MAX2 ���� ���� ���
    u32Filter2Life = u32DayMinutes * USER_LIFE_SPAN_FILTER2 * CMM_SPEED4;
    
    // ���� ������ �ʱ�ȭ
    gu8FilterData[FILTER_TYPE_PRE].gu32LifeSpan = u32Filter1Life;
    gu8FilterData[FILTER_TYPE_MAX2].gu32LifeSpan = u32Filter2Life;
    
    // EEPROM���� ���� ������ �ε�
    Lib_Air_Filter_EEPROM_LoadData();
    
    // TimeScheduler ���� (1�� �ֱ�� ���� ���� �Լ� ����)
#ifdef _FUNC_TEST_
    SetupTimeScheduler(Lib_Air_Filter_Control, 10, ACT_COUNT_INFINITE);  // �׽�Ʈ ���: 10ms �ֱ�
#else
    SetupTimeScheduler(Lib_Air_Filter_Control, 60000, ACT_COUNT_INFINITE);  // ���� ���: 1�� �ֱ�
#endif
    
    // �ʱ�ȭ �Ϸ� �÷��� ����
    gu8FilterSaveTimer = TIME_20M;
    gu8FilterInitialized = TRUE;
}

/// @brief    ���� ��� 1ms ���� �Լ� (���ŵ� - TimeScheduler ���)
/// @details  ���� ��ü Ÿ�̸� �ý����� �����ϰ� TimeScheduler�� ��ü
/// @note     �� �Լ��� �� �̻� ������ ����
void Lib_Air_Filter_Module_1ms_Control(void)
{
    // TimeScheduler ������� ���� �� �Լ��� �� �̻� �ʿ����� ����
    // ȣȯ���� ���� �� �Լ��� ����
}

/// @brief    ���� ���� ���� �Լ� (TimeScheduler�� ȣ��)
/// @details  1�� �ֱ�� TimeScheduler�� ���� �ڵ� ȣ���
/// @param    void
/// @return   void
void Lib_Air_Filter_Control(void)
{
    // �ʱ�ȭ üũ
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // EEPROM ���� Ÿ�̸� ����
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

/// @brief    ���� ���� ��ȸ �Լ�
/// @details  ���� ������ ���� ���� ��ȯ (����/���/��ü)
/// @param    type ���� ����
/// @return   EFilterStatus ���� ����
EFilterStatus Lib_Air_Filter_GetStatus(EFilterType type)
{
    // �ʱ�ȭ �� Ÿ�� ��ȿ�� �˻�
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

/// @brief    ���� ���� ����� ��ȸ �Լ�
/// @details  ���� ������ ������� ��ȯ (0-100%)
/// @param    type ���� ����
/// @return   U8 ���� ���� ����� (0-100%)
U8 Lib_Air_Filter_GetLifePercent(EFilterType type)
{
    // ��谪 �˻� ��ȭ
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX)
    {
        return 0;
    }
    
    return (U8)gu8FilterData[type].gu16PercentUsage;
}

/// @brief    ���� ���� �Լ�
/// @details  ���� ��뷮�� �ʱ�ȭ�ϰ� EEPROM�� ����
/// @param    type ���� ����
/// @return   void
void Lib_Air_Filter_Reset(EFilterType type)
{
    // ��谪 �˻� ��ȭ
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX)
    {
        return;
    }
    
    // ���� ������ �ʱ�ȭ
    gu8FilterData[type].gu32Usage = 0;
    gu8FilterData[type].gu16PercentUsage = 0;
    gu8FilterData[type].gu32Temp = 0;
    MEMSET(gu8FilterData[type].au8Filter, 0, 4);
    
    // EEPROM�� ����
    Lib_Air_Filter_EEPROM_SaveData();
}

/// @brief    ���� ��뷮 ������Ʈ �Լ�
/// @details  ���� RPM�� ������� ���� ��뷮�� ����ϰ� ������Ʈ
/// @param    motor_rpm ���� ���� RPM
/// @return   void
void Lib_Air_Filter_UpdateUsage(U16 motor_rpm)
{
    U8 u8Speed;
    U16 u16CMMValue;
    
    // �ʱ�ȭ üũ
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // RPM�� �ӵ� ������ ��ȯ (Ȯ��� ����)
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
    
    // CMM �� �������� (�迭 ��� �˻�)
    if (u8Speed < (sizeof(gau16CMMTable) / sizeof(gau16CMMTable[0]))) {
        u16CMMValue = gau16CMMTable[u8Speed];
    } else {
        u16CMMValue = CMM_SPEED4;  // �⺻��
    }
    
    // �� ������ ��뷮 ������Ʈ
    Lib_Air_Filter_AddUsage(FILTER_TYPE_PRE, u16CMMValue);
    Lib_Air_Filter_AddUsage(FILTER_TYPE_MAX2, u16CMMValue);
}

/// @brief    ���� ������ EEPROM ���� �Լ�
/// @details  ���� ��뷮 �����͸� EEPROM�� ���� (SDK ǥ�� �Լ� ���)
/// @param    void
/// @return   void
void Lib_Air_Filter_EEPROM_SaveData(void)
{
    U8 au8Data[8];
    U8 i;
    
    // �ʱ�ȭ üũ
    if (gu8FilterInitialized == FALSE) {
        return;
    }
    
    // PRE ���� ������ ��ȯ
    gu8FilterData[FILTER_TYPE_PRE].gu32Temp = gu8FilterData[FILTER_TYPE_PRE].gu32Usage;
    for (i = 0; i < 4; i++)
    {
        gu8FilterData[FILTER_TYPE_PRE].au8Filter[3-i] = (U8)(gu8FilterData[FILTER_TYPE_PRE].gu32Temp & 0xFF);
        gu8FilterData[FILTER_TYPE_PRE].gu32Temp >>= 8;
    }
    
    // MAX2 ���� ������ ��ȯ
    gu8FilterData[FILTER_TYPE_MAX2].gu32Temp = gu8FilterData[FILTER_TYPE_MAX2].gu32Usage;
    for (i = 0; i < 4; i++)
    {
        gu8FilterData[FILTER_TYPE_MAX2].au8Filter[3-i] = (U8)(gu8FilterData[FILTER_TYPE_MAX2].gu32Temp & 0xFF);
        gu8FilterData[FILTER_TYPE_MAX2].gu32Temp >>= 8;
    }
    
    // EEPROM ���� ������ �غ�
    MEMCPY(&au8Data[0], gu8FilterData[FILTER_TYPE_PRE].au8Filter, 4);
    MEMCPY(&au8Data[4], gu8FilterData[FILTER_TYPE_MAX2].au8Filter, 4);
    
    // SDK ǥ�� EEPROM �Լ� ���
#if (EEPROM_USE == USE)
    EepromPageWrite(EEP_ADD_FILTER1_1, au8Data, 8);
#endif
}

/// @brief    ���� ������ EEPROM �ε� �Լ�
/// @details  EEPROM���� ���� ��뷮 �����͸� �ε��ϰ� ����� ��� (SDK ǥ�� �Լ� ���)
/// @param    void
/// @return   void
void Lib_Air_Filter_EEPROM_LoadData(void)
{
#if (EEPROM_USE == USE)
    // PRE ���� ������ �ε� (SDK ǥ�� �Լ� ���)
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[0] = Get_EEPROM_Data(EEP_ADD_FILTER1_1);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[1] = Get_EEPROM_Data(EEP_ADD_FILTER1_2);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[2] = Get_EEPROM_Data(EEP_ADD_FILTER1_3);
    gu8FilterData[FILTER_TYPE_PRE].au8Filter[3] = Get_EEPROM_Data(EEP_ADD_FILTER1_4);
    
    // MAX2 ���� ������ �ε�
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[0] = Get_EEPROM_Data(EEP_ADD_FILTER2_1);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[1] = Get_EEPROM_Data(EEP_ADD_FILTER2_2);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[2] = Get_EEPROM_Data(EEP_ADD_FILTER2_3);
    gu8FilterData[FILTER_TYPE_MAX2].au8Filter[3] = Get_EEPROM_Data(EEP_ADD_FILTER2_4);
#else
    // EEPROM �̻�� �� �⺻������ �ʱ�ȭ
    MEMSET(gu8FilterData[FILTER_TYPE_PRE].au8Filter, 0, 4);
    MEMSET(gu8FilterData[FILTER_TYPE_MAX2].au8Filter, 0, 4);
#endif
    
    // ������ ��ȯ �� ����� ���
    Lib_Air_Filter_UpdatePercent(FILTER_TYPE_PRE);
    Lib_Air_Filter_UpdatePercent(FILTER_TYPE_MAX2);
}

/// @brief    ���� ��뷮 ����� ������Ʈ �Լ�
/// @details  ����Ʈ �迭 �����͸� U32�� ��ȯ�ϰ� ���� ����� ���
/// @param    type ���� ����
/// @return   void
void Lib_Air_Filter_UpdatePercent(EFilterType type)
{
    U32 u32Usage = 0;
    U8 i;
    
    // ��谪 �˻�
    if (type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // ����Ʈ �迭�� U32�� ��ȯ
    for (i = 0; i < 4; i++)
    {
        u32Usage = (u32Usage << 8) | gu8FilterData[type].au8Filter[i];
    }
    
    // ��뷮 ����
    gu8FilterData[type].gu32Usage = u32Usage;
    
    // ����� ��� (0���� ������ ����)
    if (gu8FilterData[type].gu32LifeSpan > 0) {
        gu8FilterData[type].gu16PercentUsage = (U16)((gu8FilterData[type].gu32Usage * 100) / gu8FilterData[type].gu32LifeSpan);
    } else {
        gu8FilterData[type].gu16PercentUsage = 0;
    }
    
    // �ִ밪 ����
    if (gu8FilterData[type].gu16PercentUsage > 100)
    {
        gu8FilterData[type].gu16PercentUsage = 100;
    }
}

/// @brief    ���� ��뷮 ���� �Լ�
/// @details  ���� CMM ����ŭ ���� ��뷮�� ������Ű�� ����� ������Ʈ
/// @param    type ���� ����
/// @param    cmm_value ���� CMM ��
/// @return   void
void Lib_Air_Filter_AddUsage(EFilterType type, U16 cmm_value)
{
    // ��谪 �˻�
    if (type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // �ִ� ��뷮 üũ
    if (gu8FilterData[type].gu32Usage >= gu8FilterData[type].gu32LifeSpan)
    {
        return;
    }
    
    // ��뷮 ����
    gu8FilterData[type].gu32Usage += cmm_value;
    
    // �ִ밪 ����
    if (gu8FilterData[type].gu32Usage > gu8FilterData[type].gu32LifeSpan)
    {
        gu8FilterData[type].gu32Usage = gu8FilterData[type].gu32LifeSpan;
    }
    
    // ����� ������Ʈ (0���� ������ ����)
    if (gu8FilterData[type].gu32LifeSpan > 0) {
        gu8FilterData[type].gu16PercentUsage = (U16)((gu8FilterData[type].gu32Usage * 100) / gu8FilterData[type].gu32LifeSpan);
    }
}

/// @brief    ���� �˶� ���� �Լ�
/// @details  ���͸� ��ü �ʿ� ���·� ���� ���� (�˶� ����)
/// @param    type ���� ����
/// @return   void
void Lib_Air_Filter_SetAlarm(EFilterType type)
{
    U8 au8AlarmData[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    
    // ��谪 �˻�
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // ���� �����͸� 0xFF�� ���� (��ü �ʿ� ����)
    MEMCPY(gu8FilterData[type].au8Filter, au8AlarmData, 4);
    
    // �ִ� ��뷮���� ����
    gu8FilterData[type].gu32Usage = gu8FilterData[type].gu32LifeSpan;
    gu8FilterData[type].gu16PercentUsage = 100;
    
    // EEPROM�� ��� ����
    Lib_Air_Filter_EEPROM_SaveData();
}

/// @brief    ���� ���� ���� ���� �Լ�
/// @details  WiFi�� �ܺ� ������� ���� ��ü �ֱ⸦ �������� ����
/// @param    type ���� ����
/// @param    weeks ���� ���� (�� ����)
/// @return   void
void Lib_Air_Filter_SetLifeSpan(EFilterType type, U8 weeks)
{
    U32 u32DayMinutes;
    U32 u32NewLifeSpan;
    
    // ��谪 �˻�
    if (gu8FilterInitialized == FALSE || type >= FILTER_TYPE_MAX) {
        return;
    }
    
    // ��ȿ ���� üũ (2~4��)
    if (weeks < 2 || weeks > 4) {
        weeks = 3;  // �⺻��
    }
    
    // ���ο� ���� ���
    u32DayMinutes = TIME_60M * TIME_A_DAY;
    u32NewLifeSpan = u32DayMinutes * (weeks * 7) * CMM_SPEED4;
    
    // ���� ���� ������Ʈ
    gu8FilterData[type].gu32LifeSpan = u32NewLifeSpan;
    
    // ����� ����
    Lib_Air_Filter_UpdatePercent(type);
    
#if (EEPROM_USE == USE)
    // ���� �������� EEPROM�� ����
    Set_EEPROM_Data(EEP_ADD_FILTER_LIFESPAN, weeks);
#endif
}

/// @brief    ���� ����� ���� ��� �Լ�
/// @details  ����� ��忡�� ���� ���¸� ���� ���
/// @param    type ���� ����
/// @return   void
void Lib_Air_Filter_DebugStatus(EFilterType type)
{
#ifdef _DEBUG_FILTER_
    const char* filterName[] = {"PRE", "MAX2"};
    
    // ��谪 �˻�
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

/// @brief    ��� ���� ���� ��� �Լ�
/// @details  ��� ������ ���¸� �� ���� ���
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
/// SDK ǥ�� ��� �������̽� �Լ���
///****************************************************************************

/// @brief    ���� ��� �ʱ�ȭ �Լ� (SDK ǥ��)
/// @details  SDK ǥ�� ��� ���Ͽ� ���� �ʱ�ȭ �Լ�
/// @param    void
/// @return   void
void Lib_Air_Filter_Module_Initialize(void)
{
#if (AIR_FILTER_USE == USE)
    Lib_Air_Filter_Initialize();
#endif
}

/// @brief    ���� ��� ���� �Լ� (SDK ǥ��)
/// @details  SDK ǥ�� ��� ���Ͽ� ���� ���� ���� �Լ�
/// @param    void
/// @return   void
void Lib_Air_Filter_Module_Control(void)
{
#if (AIR_FILTER_USE == USE)
    // TimeScheduler�� �ڵ����� Lib_Air_Filter_Control() ȣ���ϹǷ�
    // ���⼭�� �߰����� ��� ����
    
    // �ʿ�� �߰� ���� ���� ����
#endif
}

#endif // AIR_FILTER_USE == USE
