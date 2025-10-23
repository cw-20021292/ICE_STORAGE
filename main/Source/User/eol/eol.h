/**
 * File : eol.h
 * 
 * Eol
*/
#ifndef __EOL_H__
#define __EOL_H__

#include "prj_type.h"

#define MODEL_CODE_CHPI_5820L   (0x50)    /// Ice Stand 3Kg
#define MODEL_CODE_CHPI_5830L   (0x51)    /// Ice Stand 5Kg

#define MODEL_CODE              MODEL_CODE_CHPI_5830L

#define CONFIG_EOL_TEST_WATER       (1)       /// 물검사 여부 ( 사용1, 미사용 0 ) 
#define CONFIG_EOL_TEST_NITROGEN    (1)       /// 질소검사 여부 ( 사용:1, 미사용 0 )

#define  DEFAULT_TEST_TIME_VALUE    (54000U)  /// 90min, @100msec

/// EOL INPUT TEST MASK BIT
#define MK_EOL_LAST_STEP                  (0x80000000UL)
#define MK_EOL_KEY_HOT_DP                 (0x40000000UL)
#define MK_EOL_KEY_ROOM_DP                (0x20000000UL)
#define MK_EOL_KEY_COLD_DP                (0x10000000UL)

#define MK_EOL_KEY_HOT_OUT_END            (0x08000000UL)
#define MK_EOL_KEY_ROOM_OUT_END           (0x04000000UL)
#define MK_EOL_KEY_COLD_OUT_END           (0x02000000UL)
#define MK_EOL_KEY_ICE_DONE               (0x01000000UL)

#define MK_EOL_KEY_ICE_TRAY_FLOW          (0x00800000UL)
#define MK_EOL_KEY_ICE_WATER_FLOW         (0x00400000UL)
#define MK_EOL_KEY_ICE_WATER_AMOUNT_DONE  (0x00200000UL)
#define MK_EOL_KEY_ICE_WATER_DONE         (0x00100000UL)

#define MK_EOL_KEY_ICE                    (0x00000001UL)
#define MK_EOL_KEY_HOT                    (0x00000002UL)
#define MK_EOL_KEY_ROOM                   (0x00000004UL)
#define MK_EOL_KEY_COLD                   (0x00000008UL)
#define MK_EOL_KEY_AMOUNT                 (0x00000010UL)

#define MK_EOL_KEY_ICE_WATER              (0x00000020UL)
#define MK_EOL_KEY_ICE_WATER_AMOUNT       (0x00000040UL)
#define MK_EOL_KEY_ICE_LOCK               (0x00000080UL)
#define MK_EOL_KEY_ICE_WATER_LOCK         (0x00000100UL)
#define MK_EOL_KEY_HOT_LOCK               (0x00000200UL)

#define MK_EOL_LEVER_ICE                  (0x00000400UL)
#define MK_EOL_LEVER_WATER                (0x00000800UL)
#define MK_EOL_TANK_COVER                 (0x00001000UL)
#define MK_EOL_TANK_BRACKET               (0x00002000UL)

#define MK_EOL_ALL                        (0xFFF03FFFUL)
#define MK_EOL_RBK_FRONT_LAST             (0x00000FFFUL)
#define MK_EOL_LOAD_FRONT_LAST            (0x00003FFFUL)  // 정상 마지막 조건
#define MK_EOL_LOAD_SUB_FRONT_LAST        (0x00000FFFUL)  // 정상 마지막 조건
#define MK_EOL_LOAD_MAIN_FRONT_LAST       (0x00000FFFUL)  // 정상 마지막 조건

#define MK_EOL_SUB_FRONT_STEP1            (0x00000000UL)
#define MK_EOL_SUB_FRONT_STEP2            (0x00000800UL)
#define MK_EOL_SUB_FRONT_STEP3            (0x00000801UL)
#define MK_EOL_SUB_FRONT_STEP4            (0x00000821UL)
#define MK_EOL_SUB_FRONT_STEP5            (0x00000861UL)
#define MK_EOL_SUB_FRONT_STEP6            (0x00000863UL)
#define MK_EOL_SUB_FRONT_STEP7            (0x00000867UL)
#define MK_EOL_SUB_FRONT_STEP8            (0x0000086FUL)
#define MK_EOL_SUB_FRONT_STEP9            (0x0000087FUL)
#define MK_EOL_SUB_FRONT_STEP10           (0x000008FFUL)
#define MK_EOL_SUB_FRONT_STEP11           (0x000009FFUL)
#define MK_EOL_SUB_FRONT_STEP12           (0x00000BFFUL)

typedef enum _eol_type_
{
    EOL_TYPE_NONE = 0,    // 0 :
    EOL_TYPE_LOAD,        // 0 :
    EOL_TYPE_FRONT,       // 0 :
    EOL_TYPE_MAIN_FRONT,  // 0 :
    EOL_TYPE_SUB_FRONT,   // 0 : 
    EOL_TYPE_MAX,         // 0 :
} EEolType_T;

typedef enum _eol_mode_
{
    EOL_MODE_READY = 0,       // 0 :
    EOL_MODE_INIT,            // 1 :
    EOL_MODE_CHECK_SENSOR,    // 2 :
    EOL_MODE_CHECK_LOAD,      // 3 :
    EOL_MODE_DONE,            // 4 :
    EOL_MODE_SEALED_INIT,     // 5 :
    EOL_MODE_SEALED,          // 6 :
    EOL_MODE_MAX,             // 7 :
} EEolMode_T;

typedef enum _eol_test_mode_
{
    EOL_TEST_MODE_0 = 0,           // 0 : 테스트 미진행
    EOL_TEST_MODE_2,               // 1 : 테스트 검사모드2 - 자동검사
    EOL_TEST_MODE_1,               // 2 : 테스트 검사모드1 - 수동검사
} EEolTestMode_T;

typedef enum _eol_key_id
{
    EOL_ID_KEY_ICE = 0,            // 0 :
    EOL_ID_KEY_AMOUNT,             // 1 :
    EOL_ID_KEY_MAX                 // 2 :
} EEolKeyId_T;

typedef enum _eol_check_id_
{
    EOL_CHECK_ID_DRAIN_PUMP = 0,         /// 0 :
    EOL_CHECK_ID_DRAIN_PUMP_OFF,         /// 1 :
    EOL_CHECK_ID_ICE_DRAIN_PUMP,         /// 2 :
    EOL_CHECK_ID_ICE_DRAIN_PUMP_OFF,     /// 3 :
    EOL_CHECK_ID_CIRCULATE_PUMP,         /// 4 :
    EOL_CHECK_ID_CIRCULATE_PUMP_OFF,     /// 5 :
    EOL_CHECK_ID_EEPROM,                 /// 6 :
    EOL_CHECK_ID_UV_ICE_DOOR,            /// 7 :
    EOL_CHECK_ID_UV_ICE_DOOR_OFF,        /// 8 :
    EOL_CHECK_ID_UV_WATER_OUT,           /// 9 :
    EOL_CHECK_ID_UV_WATER_OUT_OFF,       /// 10 :
    EOL_CHECK_ID_UV_ICE_TANK,            /// 11 :
    EOL_CHECK_ID_UV_ICE_TANK_OFF,        /// 12 :
    EOL_CHECK_ID_UV_ICE_TRAY,            /// 13 :
    EOL_CHECK_ID_UV_ICE_TRAY_OFF,        /// 14 :
    EOL_CHECK_ID_UV_DRAIN_TANK,          /// 15 :
    EOL_CHECK_ID_UV_DRAIN_TANK_OFF,      /// 16 :
    EOL_CHECK_ID_ICE_FULL_HIGH,          /// 17 :
    EOL_CHECK_ID_ICE_FULL_HIGH_OFF,      /// 18 :
    EOL_CHECK_ID_ICE_FULL_LOW,           /// 19 :
    EOL_CHECK_ID_ICE_FULL_LOW_OFF,       /// 20 :
    EOL_CHECK_ID_FAN,                    /// 21 :
    EOL_CHECK_ID_FAN_OFF,                /// 22 :
    EOL_CHECK_ID_ICE_TRAY_DEICING,       /// 23 :
    EOL_CHECK_ID_ICE_TRAY_ICING,         /// 24 :
    EOL_CHECK_ID_FLOW_METER,             /// 25 :
    EOL_CHECK_ID_FLOW_METER_CUR,         /// 26:
    EOL_CHECK_ID_MAX,                    /// 27 :
} EEolCheckId_T;

void InitEol(void);

void StartEol(EEolType_T type);
void StopEol(void);

U8 GetEolStatus(void);
EEolType_T GetEolType(void);
U8 IsExpiredEolLimitTimer(void);

// Return true : start eol
// Return false : do not start eol
U8 StartEolLoad(void);

void SetEolMode(EEolMode_T mode);
EEolMode_T GetEolMode(void);

void SetEolTestMode(EEolTestMode_T mode);
EEolTestMode_T GetEolTestMode(void);

void SetEolTestInputBit(U32 maskBit);
void ClearEolTestInputBit(U32 maskBit);
void SetEolTestInputVal(U32 maksBit);
U32 GetEolTestInputVal(void);

void SetEolTestInputCnt(U8 id, U8 maxCnt);
U8 GetEolTestInputCnt(U8 id);
void ClearEolTestInputCnt(void);

void SetEolTestTimer(U16 time);
U16 GetEolTestTimer(void);

// EOL - DRAIN WATER 
void StartEolDrainWater(void);
void StopEolDrainWater(void);
U8 GetEolDrainWater(void);

// Cold Water Circulate
void StartEolCirculate(void);
void StopEolCirculate(void);
U8 GetEolCirculate(void);

// Error
void SetEolErrorComm(U8 error);
U8 GetEolErrorComm(void);

void UpdateEolTimer(void); // @1sec

void SetEolCheckStatus(EEolCheckId_T id, U32 val);
U32 GetEolCheckStatus(EEolCheckId_T id);

U8 EOL_Drain(void);

#endif /* __EOL_H__ */
