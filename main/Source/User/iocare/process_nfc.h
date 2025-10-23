#ifndef __PROCESS_NFC_H__
#define __PROCESS_NFC_H__

#include "prj_type.h"
#include "nfc.h"

/* NFC MODE */
#define NFC_INIT                    0x0001
#define NFC_ERASE_ALL               0x0002
#define NFC_SERVICE_CHECK_DAY       0x0004
#define NFC_ERROR                   0x0008
#define NFC_UPDATE_SETTING          0x0010
#define NFC_DAILY_DATA              0x0020
#define NFC_USE_WATER_DATA          0x0040
#define NFC_STER_PASS_DAY           0x0080
#define NFC_STER_HISTORY            0x0100



typedef struct _nfc_control_
{
    U16 Mode;     // NFC Control Mode
    U8 Error;     // NFC Comm. error..
    U8 ErrorCount;     // NFC Comm. error.. count..

    U8 ErrorHistoryCount; // NFC ERROR HISTORY COUNTER VALUE
    U8 UserHistoryCount;   
    U8 SterHistoryCount;
    U8 WaterHistoryCount;

    // To check updated information, compare tag time..
    NFC_TagTime_T TagTime; 
} NFC_Control_T;

void InitNfc(void);

void SetNfcMode(U16 mu16Mode);

// 통신 에러 상태 반환
// Return : TRUE - 에러, FALSE : 정상
U8 GetNfcError(void);

// Get/Set Error History Count
U8 GetNfcErrorHisCount(void);
void SetNfcErrorHisCount(U8 mu8Count);

// Get/Set User History Count
U8 GetNfcUserHisCount(void);
void SetNfcUserHisCount(U8 mu8Count);

// Get/Set Ster History Count
U8 GetNfcSterHisCount(void);
void SetNfcSterHisCount(U8 mu8Count);

// Get/Set Water History Count
U8 GetNfcWaterHisCount(void);
void SetNfcWaterHisCount(U8 mu8Count);

// Get/Set Tag Time
void GetNfcTagTime( NFC_TagTime_T *pTime);
void SetNfcTagTime( NFC_TagTime_T *pTime);

void ProcessNfc(void);

#endif /* __PROCESS_NFC_H__ */
