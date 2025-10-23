/**
 * File : comp.h
 * 
 * Comp Module
*/
#ifndef __COMP_BLDC_H__
#define __COMP_BLDC_H__

#include "prj_type.h"

#define CONFIG_BLDC_COMP_LG         (1)
#define CONFIG_BLDC_COMP_SAMSUNG    (0)

/// BLDC COMP ERROR CODE 
#if( CONFIG_BLDC_COMP_LG == 1 )
typedef enum _comp_bldc_error_id_
{
    COMP_BLDC_ERR_ID_NONE = 0,          /// 0 : 
    
    COMP_BLDC_ERR_ID_AD_OFFSET,         /// 1 : Ad Offset Error     
    COMP_BLDC_ERR_ID_TEMP_SENSING = 5,  /// 5 : Temp Sensing Error
    COMP_BLDC_ERR_ID_OVER_CURRENT,      /// 6 : Over Current Error
    COMP_BLDC_ERR_ID_IPM_FAULT,         /// 7 : IPM Fault
    COMP_BLDC_ERR_ID_COMM,              /// 8 : Communication Error
    COMP_BLDC_ERR_ID_OVERLOAD_PROT,     /// 9 : Overload Protection
    COMP_BLDC_ERR_ID_OVERTEMP,          /// 10 : OverTemp Error
    COMP_BLDC_ERR_ID_LOW_VOLTAGE,       /// 11 : Low Voltage Error
    COMP_BLDC_ERR_ID_HIGH_VOLTAGE,      /// 12 : HIGH Voltage Error

    COMP_BLDC_ERR_ID_OUT_OF_RANGE,      /// 13 : 이후 추가되는 에러

    COMP_BLDC_ERR_ID_MAX                /// 14 :    
} ECompBldcErrorId_T;
#endif  /// CONFIG_BLDC_COMP_LG

#if( CONFIG_BLDC_COMP_SAMSUNG == 1 )
typedef enum _comp_bldc_error_id_
{
    COMP_BLDC_ERR_ID_NONE = 0,                  /// 0 : None
    
    COMP_BLDC_ERR_ID_STARTING_FAIL,             /// 1 : After three attempts to start ( EC1 )
    COMP_BLDC_ERR_ID_CURRENT_SENSING_ERROR,     /// 2 : disconnection of comp wire, Or poor current control ( EC2 )
    COMP_BLDC_ERR_ID_OVER_CURRENT,              /// 3 : Cureent exceeds 2.5A peak ( EC3 )
    
    COMP_BLDC_ERR_ID_ABNORMAL_VOLTAGE,          /// 4 : DC link voltge is out of the min 80 V, max 450 V ( EC4 )
    COMP_BLDC_ERR_ID_OVERHEAT_CONTROLLER,       /// 5 : IPM temperture exceeds 110'c ( EC5 )
    COMP_BLDC_ERR_ID_DISCONNECTION_COMM,        /// 6 : After 2 min of receiving the last data ( EC6 )

    COMP_BLDC_ERR_ID_OUT_OF_RANGE,              /// 7 : 이후 추가되는 에러 ( EC7 )

    COMP_BLDC_ERR_ID_MAX                        /// 8 : Max
} ECompBldcErrorId_T;
#endif  /// CONFIG_BLDC_COMP_SAMSUNG

typedef enum _comp_bldc_error_code_
{
    COMP_BLDC_ERR_CODE_NONE = 0,           /// 0 : 
    COMP_BLDC_ERR_CODE_1,                  /// 0 : 
    COMP_BLDC_ERR_CODE_2,                  /// 0 : 
    COMP_BLDC_ERR_CODE_3,                  /// 0 : 
    COMP_BLDC_ERR_CODE_4,                  /// 0 : 
    COMP_BLDC_ERR_CODE_5,                  /// 0 : 
    COMP_BLDC_ERR_CODE_6,                  /// 0 : 
    COMP_BLDC_ERR_CODE_7,                  /// 0 : 
    COMP_BLDC_ERR_CODE_8,                  /// 0 : 
    COMP_BLDC_ERR_CODE_9,                  /// 0 : 
    COMP_BLDC_ERR_CODE_10,                 /// 0 : 
} ECompBldcErrorCode_T;

/// BLDC MIN/MAX RPS
#define MIN_RPS     (21)
#define MAX_RPS     (75)
typedef unsigned char RPS_T;

typedef struct _comp_bldc_
{
    RPS_T target;
    RPS_T current;
    RPS_T min;
    RPS_T max;
    U8 temp;

    ECompBldcErrorId_T error;            // COMP ERR - 에러 코드
    ECompBldcErrorId_T lastedError;      // 에러 코드
    U8 errorComm;        // COMP ERR - 통신 에러
    U8 protect;          // COMP PROTECT 
    U8 testComm;         // 통신 테스트 (FCT)

    U16 onStableTime;     // ON 동작시 대기 시간
    U16 offMaxTime;       // COMP 동작시 대기 시간
} SCompBldc_T;

void InitCompBldc(void);
void GetCompBldcData(SCompBldc_T *pData);

void SetCompBldcTargetRps(RPS_T rps);
RPS_T GetCompBldcTargetRps(void);

void SetCompBldcCurrentRps(RPS_T rps);
RPS_T GetCompBldcCurrentRps(void);

void SetCompBldcTemp(U8 temp);
U8 GetCompBldcTemp(void);

#define COMP_BLDC_ERR_COMM_TIMEOUT    (30)    /// @1sec
void SetCompBldcError(ECompBldcErrorId_T id);
ECompBldcErrorId_T GetCompBldcError(void);

void SetCompBldcLastedError(ECompBldcErrorId_T id);
ECompBldcErrorId_T GetCompBldcLastedError(void);

ECompBldcErrorCode_T GetCompBldcErrorCode(ECompBldcErrorId_T id);

void SetCompBldcErrorComm(U8 error);
U8 GetCompBldcErrorComm(void);

void SetCompBldcProtect(U8 protect);
U8 GetCompBldcProtect(void);
U8 IsCompBldcProtectMode(void);

void SetCompBldcCommTest(U8 test);
U8 GetCompBldcCommTest(void);

U8 IsTurnOnCompBldc(void);

void ControlCompBldc(void);

#endif /* __COMP_BLDC_H__ */
