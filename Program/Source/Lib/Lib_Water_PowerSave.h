/// @file     Lib_Water_PowerSave.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    절전 동작 제어 file

#ifndef _LIB_WATER_POWER_SAVE_H_
#define _LIB_WATER_POWER_SAVE_H_


/*  Lib Function 사용법
    => Set_NoUsePowerSaveFuncSet(mu8OnOff);             // 미사용 절전 기능 설정 상태 설정 - 0(OFF), 1(ON)
    => Get_NoUsePowerSaveFuncSet();                     // 미사용 절전 기능 설정 상태 확인 - 0(OFF), 1(ON)

    => Set_PowerSaveFuncSet(mu8OnOff);                  // 일반 절전 기능 설정 상태 설정 - 0(OFF), 1(ON)
    => Get_PowerSaveFuncSet();                          // 일반 절전 기능 설정 상태 확인 - 0(OFF), 1(ON)

    => Set_PowerSaveMode(mu8Mode);                      // 현재 동작 중인 절전 모드 설정 - 0(절전 미동작), 1(일반 절전), 2(미사용 절전)
    => Get_PowerSaveMode();                             // 현재 동작 중인 절전 모드 확인 - 0(절전 미동작), 1(일반 절전), 2(미사용 절전)

    => Set_BrightnessData(mu8Data);                     // 현재 밝기 상태 설정 - 0(밝음), 1(어두움)
    => Get_BrightnessData();                            // 현재 밝기 상태 확인 - 0(밝음), 1(어두움)
*/


#define SMART_NO_USE_POWER_SAVE_USE                 USE     // 스마트 미사용 절전 기능 사용 여부
#define POWER_SAVE_USE                              USE     // 절전 기능 사용 여부

#define PHOTO_SENSOR_USE                            NO_USE     // 조도 센서 사용 여부 : 0(NO_USE), 1(USE)


#if ( (SMART_NO_USE_POWER_SAVE_USE == USE) || (POWER_SAVE_USE == USE) )

#define POWER_SAVE_CONTROL_TIME_PERIOD          1000    // 온수 탱크 제어 시간 주기 : 1Sec(@1ms)

#if (PHOTO_SENSOR_USE == USE)
#define BRIGHT_DETECT_TIME                      600     // 밝기 변화 감지 확정 시간 10min(@1sec)

// 조도 센서 테스트 결과(5mm 서포터)
// 1.5 Lux - 354  440  400
// 3.5 Lux - 693  770  730
#define SLEEP_IN_LUX_DETECT_AD                  360     // 절전 모드 진입 기준 밝기 1.5 Lux AD값
#define SLEEP_OUT_LUX_DETECT_AD                 700     // 절전 모드 해제 기준 밝기 3.5 Lux AD값

#define BRIGHTNESS_DETECT_BRIGHT                0       // 밝음 감지
#define BRIGHTNESS_DETECT_DARK                  1       // 어두움 감지
#endif


/// @brief      절전모드 동작 리스트
/// @details    절전모드 동작을 리스트로 정리한다
typedef enum
{
    POWERSAVE_ID_NONE = 0,                  // 0 : 절전 미동작
    POWERSAVE_ID_NORMAL,                    // 1 : 일반 절전 모드 동작
    POWERSAVE_ID_SMART,                     // 2 : 스마트 미사용 절전 모드 동작
    POWERSAVE_ID_MAX                        // Max
} EPowerSaveID_T;


// 스마트 미사용 절전 제어
#define NO_USE_TIME_OUT                         9       // 9시간, 미사용 절전 모드 동작 시작 체크 시간

#define	SMART_POWER_SAVE_TIME_PERIOD            168     // 7days = 168hour, 1주일 주기 시간
#define SMART_POWER_SAVE_TIME_NO_USE            16      // 16hr, 제품 미사용 체크 시간
#define SMART_POWER_SAVE_TIME_NO_USE_LONG       84      // 84hr, 제품 장기 미사용 체크 시간

#define SMART_POWER_SAVE_TYPE_NONE              0
#define SMART_POWER_SAVE_TYPE_GREEN_WEEK        1
#define SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK    2

void Lib_PowerSave_Initialize(void);
void PowerSaveControlTimer(void);
void Set_NoUsePowerSaveFuncSet(U8 mu8OnOff);
U8 Get_NoUsePowerSaveFuncSet(void);
void Set_PowerSaveFuncSet(U8 mu8OnOff);
U8 Get_PowerSaveFuncSet(void);
void Set_PowerSaveMode(U8 mu8Mode);
U8 Get_PowerSaveMode(void);

#if (PHOTO_SENSOR_USE == USE)
void Set_BrightnessData(U8 mu8Data);
U8 Get_BrightnessData(void);
#endif

void Lib_PowerSaveControl(void);

#if (PHOTO_SENSOR_USE == USE)
void BrightnessCheckControl(void);
#endif

void PowerSaveControl(void);
void SmartNoUsePowerSaveControl(void);
void NoUseTimeClear(void);
void NoUseTimeCounter(void);
void Set_SmartNoUsePowerSaveButtonInput(U8 mu8Data);
void DecisionSmartPowerSaveType(void);
void SmartNoUseTimeCounter(void);
void SmartNoUseCheckClear(void);
void PowerSaveActEnd(void);

#endif      // 스마트 미사용 절전 사용 또는 절전 사용 END

void Lib_PowerSave_Module_Initialize(void);
void Lib_PowerSave_Module_1ms_Control(void);
void Lib_PowerSave_Module_Control(void);

#endif

