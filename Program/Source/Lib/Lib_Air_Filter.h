/// @file     Lib_Air_Filter.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    ����û���� ���� ���� ���̺귯�� ��� ����
/// @details  SDK ǥ���� �ؼ��ϴ� ���� ���� ���̺귯�� (Noble30 ��� ����)

#ifndef _LIB_AIR_FILTER_H_
#define _LIB_AIR_FILTER_H_

// ���� ��� ����
#define AIR_FILTER_USE                  USE    // ���� ��� ��� ���� - 0(No Use), 1(Use)

/*  ���� ���� ���� Function ����
    => Lib_Air_Filter_Module_Initialize();                   // ���� ���� ��� �ʱ�ȭ
    => Lib_Air_Filter_UpdateUsage(����RPM);                  // ���� ���� �� ���� ��뷮 ������Ʈ
    => Lib_Air_Filter_GetStatus(����Ÿ��);                   // ���� ���� Ȯ�� - FILTER_TYPE_PRE, FILTER_TYPE_MAX2
    => Lib_Air_Filter_GetLifePercent(����Ÿ��);              // ���� ���� ����� Ȯ�� (0~100%)
    => Lib_Air_Filter_Reset(����Ÿ��);                       // ���� ��ü �� ���� ����
    => Lib_Air_Filter_SetLifeSpan(����Ÿ��, �ּ�);           // ���� ���� ���� (�� ����)

    ��� ��)
    Lib_Air_Filter_Module_Initialize();                      // �ý��� �ʱ�ȭ �� ȣ��
    
    while(1) {
        Lib_Air_Filter_Module_Control();                     // ���� �������� ȣ��
        
        // ���� ���� �� ���� ��뷮 ������Ʈ
        U16 current_rpm = 1000;                              // ���� ���� RPM
        Lib_Air_Filter_UpdateUsage(current_rpm);
        
        // ���� ���� Ȯ��
        EFilterStatus pre_status = Lib_Air_Filter_GetStatus(FILTER_TYPE_PRE);
        EFilterStatus max2_status = Lib_Air_Filter_GetStatus(FILTER_TYPE_MAX2);
        
        // ���� ���� ����� Ȯ��
        U8 pre_percent = Lib_Air_Filter_GetLifePercent(FILTER_TYPE_PRE);     // PRE ���� ���� %
        U8 max2_percent = Lib_Air_Filter_GetLifePercent(FILTER_TYPE_MAX2);   // MAX2 ���� ���� %
        
        // ���� ��ü �Ϸ� �� ����
        if(filter_replaced_flag) {
            Lib_Air_Filter_Reset(FILTER_TYPE_PRE);           // PRE ���� ���� ����
            Lib_Air_Filter_Reset(FILTER_TYPE_MAX2);          // MAX2 ���� ���� ����
        }
        
        // ���� ���� ���� (���û���)
        Lib_Air_Filter_SetLifeSpan(FILTER_TYPE_PRE, 2);     // PRE ���� 2�� ����
        Lib_Air_Filter_SetLifeSpan(FILTER_TYPE_MAX2, 52);   // MAX2 ���� 52��(1��) ����
    }

    ���ǻ���)
    - AIR_FILTER_USE�� USE�� �����Ǿ�� ��
    - ���� ����: GOOD(����), WARNING(��ü�ӹ�), REPLACE(��ü�ʿ�), ERROR(����)
    - ���� �˶� ������: 90% ��� ����
    - EEPROM�� 20�и��� �ڵ� �����
    - CMM ���� ���� RPM�� ���� �ڵ� ����
*/

// ���� ���� ���
#define USER_LIFE_SPAN_FILTER1    14     /// @brief PRE ���� ����: 14��
#define USER_LIFE_SPAN_FILTER2    365    /// @brief MAX2 ���� ����: 365��
#define USER_FILTER1_ALARM        90     /// @brief PRE ���� �˶� ������: 90%
#define USER_FILTER2_ALARM        90     /// @brief MAX2 ���� �˶� ������: 90%

// �ð� ���� ���
#define TIME_20M                  20     /// @brief EEPROM ���� �ֱ�: 20��
#define TIME_60M                  60     /// @brief 1�ð�: 60��
#define TIME_A_DAY               8      /// @brief �Ϸ� ��� �ð�: 8�ð�

// CMM(Cubic Meter per Minute) �� ���� (Noble30 Ȯ��)
#define CMM_SPEED0               0      /// @brief ���� ���� CMM
#define CMM_SPEED1               10     /// @brief 1�� CMM
#define CMM_SPEED2               20     /// @brief 2�� CMM
#define CMM_SPEED3               30     /// @brief 3�� CMM
#define CMM_SPEED4               50     /// @brief 4�� CMM

// EEPROM �ּ� ���� (������Ʈ�� �°� ���� �ʿ�)
#ifndef EEP_ADD_FILTER1_1
#define EEP_ADD_FILTER1_1        50     /// @brief PRE ���� ������ �ּ� 1
#define EEP_ADD_FILTER1_2        51     /// @brief PRE ���� ������ �ּ� 2
#define EEP_ADD_FILTER1_3        52     /// @brief PRE ���� ������ �ּ� 3
#define EEP_ADD_FILTER1_4        53     /// @brief PRE ���� ������ �ּ� 4
#define EEP_ADD_FILTER2_1        54     /// @brief MAX2 ���� ������ �ּ� 1
#define EEP_ADD_FILTER2_2        55     /// @brief MAX2 ���� ������ �ּ� 2
#define EEP_ADD_FILTER2_3        56     /// @brief MAX2 ���� ������ �ּ� 3
#define EEP_ADD_FILTER2_4        57     /// @brief MAX2 ���� ������ �ּ� 4
#define EEP_ADD_FILTER_LIFESPAN  58     /// @brief ���� ���� ���� �ּ�
#endif

// ����� ����
// #define _DEBUG_FILTER_           // ���� ����� ��� Ȱ��ȭ

/// @brief ���� ���� ����
typedef enum {
    FILTER_STATUS_GOOD = 0,      /// @brief ���� ����
    FILTER_STATUS_WARNING,       /// @brief ��ü �ñ� �ӹ�
    FILTER_STATUS_REPLACE,       /// @brief ��ü �ʿ�
    FILTER_STATUS_ERROR         /// @brief ���� ����
} EFilterStatus;

/// @brief ���� ���� ����
typedef enum {
    FILTER_TYPE_PRE = 0,        /// @brief PRE ����
    FILTER_TYPE_MAX2,           /// @brief MAX2 ����
    FILTER_TYPE_MAX             /// @brief ���� ����
} EFilterType;

/// @brief ���� ������ ����ü
typedef struct {
    U32 gu32LifeSpan;          /// @brief ���� ��ü ����
    U32 gu32Usage;             /// @brief ���� ��뷮
    U16 gu16PercentUsage;      /// @brief ��뷮 �����(%)
    U32 gu32Temp;              /// @brief �ӽ� ���� ����
    U8  au8Filter[4];          /// @brief EEPROM ����� ����Ʈ �迭
} SFilterData_T;

// �ʱ�ȭ �� ���� �Լ�
void Lib_Air_Filter_Initialize(void);
void Lib_Air_Filter_Control(void);
void Lib_Air_Filter_Module_1ms_Control(void);  // ȣȯ�� ������ (�� �Լ�)

// ���� ���� ���� �Լ�
EFilterStatus Lib_Air_Filter_GetStatus(EFilterType type);
U8 Lib_Air_Filter_GetLifePercent(EFilterType type);
void Lib_Air_Filter_Reset(EFilterType type);

// � �ð� ������Ʈ �Լ�
void Lib_Air_Filter_UpdateUsage(U16 motor_rpm);

// ���� ���� �Լ� (�ٸ� ��⿡���� ��� ����)
void Lib_Air_Filter_EEPROM_SaveData(void);
void Lib_Air_Filter_EEPROM_LoadData(void);
void Lib_Air_Filter_UpdatePercent(EFilterType type);
void Lib_Air_Filter_AddUsage(EFilterType type, U16 cmm_value);

// Noble30 ���� Ȯ�� �Լ�
void Lib_Air_Filter_SetAlarm(EFilterType type);
void Lib_Air_Filter_SetLifeSpan(EFilterType type, U8 weeks);

// ����� ���� �Լ� (���Ǻ� ������)
void Lib_Air_Filter_DebugStatus(EFilterType type);
void Lib_Air_Filter_DebugAllStatus(void);

// ǥ�� ��� �������̽� �Լ� (SDK ǥ��)
void Lib_Air_Filter_Module_Initialize(void);
void Lib_Air_Filter_Module_Control(void);

#endif // _LIB_AIR_FILTER_H_
