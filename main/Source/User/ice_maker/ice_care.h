#ifndef __ICE_CARE_H__
#define __ICE_CARE_H__

#include "prj_type.h"


#define ICE_CARE_MODE_NONE        (0x0000)
#define ICE_CARE_MODE_OFF         (0x0001)
#define ICE_CARE_MODE_BREAK       (0x0002)
#define ICE_CARE_MODE_POSITION    (0x0004)
#define ICE_CARE_MODE_ALL       \
    ( ICE_CARE_MODE_BREAK |     \
      ICE_CARE_MODE_POSITION )

void InitIceCare(void);
void SetIceCareMode(U16 mu16Mode);
void ClearIceCareMode(U16 mu16Mode);
U16 IsSetIceCareMode(U16 mu16Mode );
U16 GetIceCareMode(void);

void SetConfigCareBreak(U8 mu8Config);
U8 GetConfigCareBreak(void);

void SetConfigCarePosition(U8 mu8Config);
U16 GetConfigCarePosition(void);

U8 IsValidIceCare(void);

#endif /* __ICE_CARE_H__ */

