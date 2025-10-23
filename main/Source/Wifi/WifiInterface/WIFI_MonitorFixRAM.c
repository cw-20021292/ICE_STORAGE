/*******************************************************************************
&Description :
  제품 모니터링 WIFI 함수 File
  
&History : ssg@coway.com

  SSG 2023.05.30 update
  - FIXRAM 기능 추가
*******************************************************************************/

#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_SetCommon.h"
#include "WIFI_UserInterface.h"
#include "WIFI_MonitorFixRAM.h"

/* G23WIFI???? 
extern U8 A_NULL=0;


static const WifiFixRamList_T WifiRamList[] =
{
    {A_U8_A301_COLD_WATER_LEV_LOW,          (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U8_A303_COLD_WATER_LEV_HIGH,         (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    // {A_U8_A603_VALVE_PURE_OUT,              (U8*)(&gu8_Pure_Valve_CMD),                     OneByte,        Read_Only     }, 
    // {A_U8_A605_VALVE_HOT_OUT,               (U8*)(&gu8_Hot_OutValve_CMD),                   OneByte,        Read_Only     }, 
    // {A_U8_A609_VALVE_COLD_OUT,              (U8*)(&gu8_Cold_OutValve_CMD),                  OneByte,        Read_Only     }, 
    {A_U16_A701_COLD_TEMP,                  (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_A701_COLD_TEMP,                  (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U16_A705_HOT_TEMP,                   (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_A708_OUT_TEMP,                   (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U8_AA01_FAN_STATUS,                  (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U8_AF04_COMP_OPERATION_STATUS,       (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U8_B201_HOT_OPERATION_STATUS,        (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U8_A901_ERROR_CODE,                  (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_FF_HOT_Heater_ControlNum,            (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U16_A709_EVA_TEMP,                   (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_AC02_ICE_FULL_SENSOR,            (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U8_B001_ICEMAKE_STATUS,              (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    {A_U16_A707_PURE_WATER_TEMP,            (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_FF_MIX_OUT_TEMP,                 (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_FF_TRAY_IN_TEMP,                 (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    {A_U16_FF_OUT2_TEMP,                    (U8*)(&A_NULL),                     TwoByte,        Read_Only     }, 
    // {A_U8_FF_SMART_SAMRT_SAVING_STATE,      (U8*)(&A_NULL),                     OneByte,        Read_Only     }, 
    // {A_FF_HOT_AddOperation,                 (U8*)(&gu16_Timer_Hot_AddOperation_for_Wifi),   TwoByte,        *SetRamData   }, 
    // {A_FF_COLD_AddOperation,                (U8*)(&gu16_Timer_Comp_AddOperation_for_Wifi),  TwoByte,        *SetRamData   },
    // {A_AF01_COLD_STATUS,                    (U8*)(&gu8_CoolingStep),                        OneByte,        Read_Only     }, 
    // {A_FF_HOT_STATUS,                       (U8*)(&gu8_HeatingStep),                        OneByte,        Read_Only     }, 
    // {A_FF_OPERATION,                        (U8*)(&gu8_OperationStep),                      OneByte,        Read_Only     },


};
#define SZ_RAM_ADR_LIST ( sizeof(WifiRamList) / sizeof(WifiFixRamList_T) )
*/
/*--------------------------------------------------*/
/*	Parameter : write Ram Address (server -> wifi)  */
/*	return : U8* write value(start address) (wifi -> mcu)           */
/*--------------------------------------------------*/
U8 *SetRamData (U16 mu16address)
{
	/* G23WIFI???? 
    static U16 mu16Data = 0U;
    U8 *buf;
    switch ( mu16address )
    {    
        buf = (U8*)mu16address;     // fix ram value

        // case A_FF_HOT_AddOperation : 
        //     mu16Data = *buf;
        // break;
        // case A_FF_COLD_AddOperation : 
        //     mu16Data = *buf;
        // break;
 
        default:
            mu16Data = 0U;
        break;
    }
    return (U8*)(&mu16Data);
    */
}

void GetRamData (void)
{
	/* G23WIFI???? 
    __near U8 *pFixRamAddress ;
    // U8 *TempPtrFix ;
    U8 *pTempPtr ;
    U8 *pTempPtrReturn ;
    U16 RamCheckAddress =0;
    U8 i=0;
    WifiWriteFun_T pWriteFunc;

    for ( i = 0 ; i < SZ_RAM_ADR_LIST; i++ )
    {
        pFixRamAddress = ((__near U8*)WifiRamList[i].AddressList);
        pTempPtr = WifiRamList[i].TargetVariable;
        RamCheckAddress = (U16)(GetUserSystemRamWriteStatus((U16)(WifiRamList[i].AddressList)));
        if (pTempPtr == &A_NULL) 
        {
            RamCheckAddress = CLEAR_ADR;
            SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
        }
        else {
            if (RamCheckAddress != CLEAR_ADR) // write check
            {
                pWriteFunc =  WifiRamList[i].WriteFunc;
                if ( pWriteFunc != Read_Only ) // write check
                {
                    pTempPtrReturn = (U8*)pWriteFunc((U16)(RamCheckAddress));
                    *(pTempPtr) = *(pTempPtrReturn); // 1byte write

                    if (WifiRamList[i].VarSize == TwoByte ) // 2byte write
                    {
                        pTempPtr++;
                        RamCheckAddress++;
                        pTempPtrReturn = (U8*)RamCheckAddress;
                        *(pTempPtr) = *(pTempPtrReturn);
                    }
                    RamCheckAddress = CLEAR_ADR;
                    SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
                    break;
                }
                else 
                {
                    RamCheckAddress = CLEAR_ADR;
                    SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
                }
            }
            else  // read
            { //1byte read
                *pFixRamAddress = *(WifiRamList[i].TargetVariable);

                if (WifiRamList[i].VarSize == TwoByte ) 
                { //2byte variable
                    pTempPtr = WifiRamList[i].TargetVariable;
                    pFixRamAddress++;
                    pTempPtr++;
                    *pFixRamAddress = *pTempPtr;
                }
            }
        }

    }
    */
}
