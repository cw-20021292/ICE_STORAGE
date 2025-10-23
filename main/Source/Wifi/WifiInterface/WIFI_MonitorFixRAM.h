/***** Type Define *****************************************************************/ 
// typedef unsigned char       U8;
// typedef signed char         I8;
// typedef unsigned int        U16; 
// typedef signed int          I16; 

/******** Define Func *********/
void GetRamData (void);
U8 *SetRamData (U16);

/******** typedef *********/
typedef U8 *(*WifiWriteFun_T)(U16); 
typedef struct 
{ 
    U16 AddressList;  
    U8 *TargetVariable;  
    U8 VarSize; 
    WifiWriteFun_T WriteFunc; 
} WifiFixRamList_T; 


/***** Define Address *****************************************************************/ 
#define     A_FAR_RANGE                         (0xF0000)
#define     A_RAM_PREFIX                        (0xFC00)

// #define USE_PRESET
#ifndef USE_PRESET

#define     A_U8_A301_COLD_WATER_LEV_LOW        (0xFC00)
#define     A_U8_A303_COLD_WATER_LEV_HIGH       (0xFC01)
#define     A_U8_A603_VALVE_PURE_OUT            (0xFC02)
#define     A_U8_A605_VALVE_HOT_OUT             (0xFC03)
#define     A_U8_A609_VALVE_COLD_OUT            (0xFC04)
#define     A_U16_A701_COLD_TEMP                (0xFC05)
#define     A_U16_A705_HOT_TEMP                 (0xFC07)
#define     A_U16_A708_OUT_TEMP                 (0xFC09)
#define     A_U8_AA01_FAN_STATUS                (0xFC0B)
#define     A_U8_AF04_COMP_OPERATION_STATUS     (0xFC0F)
#define     A_U8_B201_HOT_OPERATION_STATUS      (0xFC10)
#define     A_U8_A901_ERROR_CODE                (0xFC11)
#define     A_FF_HOT_Heater_ControlNum          (0xFC12)
#define     A_FF_HOT_AddOperation               (0xFC13)
#define     A_FF_COLD_AddOperation              (0xFC15)
#define     A_AF01_COLD_STATUS                  (0xFC0C)
#define     A_FF_HOT_STATUS                     (0xFC0D)
#define     A_FF_OPERATION                      (0xFC0E)
#define     A_U16_A709_EVA_TEMP                 (0xFC17)      // ���߱�µ�����
#define     A_U16_AC02_ICE_FULL_SENSOR          (0xFC19)      // ��������
#define     A_U8_B001_ICEMAKE_STATUS            (0xFC1B)      // ��������
#define     A_U16_A707_PURE_WATER_TEMP          (0xFC1C)      // �����µ�����
#define     A_U16_FF_MIX_OUT_TEMP               (0xFC1E)      // ����µ�����
#define     A_U16_FF_TRAY_IN_TEMP               (0xFC20)      // Ʈ�����Լ��µ�����
#define     A_U16_FF_OUT2_TEMP                  (0xFC22)      // �ܱ�µ�����2
#define     A_U8_FF_SMART_SAMRT_SAVING_STATE    (0xFC24)      // ����Ʈ��������

/***** Directmap *****************************************************************/ 
/* G23WIFI???? 
__directmap     U8     gu8_A301_COLD_WATER_LEV_LOW      = {A_FAR_RANGE | A_U8_A301_COLD_WATER_LEV_LOW       };
__directmap     U8     gu8_A303_COLD_WATER_LEV_HIGH     = {A_FAR_RANGE | A_U8_A303_COLD_WATER_LEV_HIGH      };
__directmap     U8     gu8_A603_VALVE_PURE_OUT          = {A_FAR_RANGE | A_U8_A603_VALVE_PURE_OUT           };
__directmap     U8     gu8_A605_VALVE_HOT_OUT           = {A_FAR_RANGE | A_U8_A605_VALVE_HOT_OUT            };
__directmap     U8     gu8_A609_VALVE_COLD_OUT          = {A_FAR_RANGE | A_U8_A609_VALVE_COLD_OUT           };
__directmap     U16    gu16_A701_COLD_TEMP              = {A_FAR_RANGE | A_U16_A701_COLD_TEMP               };
__directmap     U16    gu16_A705_HOT_TEMP               = {A_FAR_RANGE | A_U16_A705_HOT_TEMP                };
__directmap     U16    gu16_A708_OUT_TEMP               = {A_FAR_RANGE | A_U16_A708_OUT_TEMP                };
__directmap     U8     gu8_AA01_FAN_STATUS              = {A_FAR_RANGE | A_U8_AA01_FAN_STATUS               };
__directmap     U8     gu8_AF01_COLD_STATUS             = {A_FAR_RANGE | A_AF01_COLD_STATUS                 };  // �ð���������
__directmap     U8     gu8_FF_HOT_STATUS                = {A_FAR_RANGE | A_FF_HOT_STATUS                    };  // �¼���������
__directmap     U8     gu8_FF_OPERATION                 = {A_FAR_RANGE | A_FF_OPERATION                     };  // ��������
__directmap     U8     gu8_AF04_COMP_OPERATION_STATUS   = {A_FAR_RANGE | A_U8_AF04_COMP_OPERATION_STATUS    };
__directmap     U8     gu8_B201_HOT_OPERATION_STATUS    = {A_FAR_RANGE | A_U8_B201_HOT_OPERATION_STATUS     };
__directmap     U8     gu8_A901_ERROR_CODE              = {A_FAR_RANGE | A_U8_A901_ERROR_CODE               };  // �����ڵ�
__directmap     U8     gu8_FF_HOT_Heater_ControlNum     = {A_FAR_RANGE | A_FF_HOT_Heater_ControlNum         };  // �����
__directmap     U16    gu16_FF_HOT_AddOperation         = {A_FAR_RANGE | A_FF_HOT_AddOperation              };  // �¼��߰��ð�
__directmap     U16    gu16_FF_COLD_AddOperation        = {A_FAR_RANGE | A_FF_COLD_AddOperation             };  // �ð��߰��ð�
__directmap     U16    gu16_A709_EVA_TEMP               = {A_FAR_RANGE | A_U16_A709_EVA_TEMP                };  // ���߱�µ�����
__directmap     U16    gu16_AC02_ICE_FULL_SENSOR        = {A_FAR_RANGE | A_U16_AC02_ICE_FULL_SENSOR         };  // ��������
__directmap     U8     gu8_B001_ICEMAKE_STATUS          = {A_FAR_RANGE | A_U8_B001_ICEMAKE_STATUS           };  // ��������
__directmap     U16    gu16_A707_PURE_WATER_TEMP        = {A_FAR_RANGE | A_U16_A707_PURE_WATER_TEMP         };  // �����µ�����
__directmap     U16    gu16_FF_MIX_OUT_TEMP             = {A_FAR_RANGE | A_U16_FF_MIX_OUT_TEMP              };  // ����µ�����
__directmap     U16    gu16_FF_TRAY_IN_TEMP             = {A_FAR_RANGE | A_U16_FF_TRAY_IN_TEMP              };  // Ʈ�����Լ��µ�����
__directmap     U16    gu16_FF_OUT2_TEMP                = {A_FAR_RANGE | A_U16_FF_OUT2_TEMP                 };  // �ܱ�µ�����2
__directmap     U8     gu8_FF_SMART_SAMRT_SAVING_STATE  = {A_FAR_RANGE | A_U8_FF_SMART_SAMRT_SAVING_STATE   };  // ����Ʈ��������
*/

#endif


