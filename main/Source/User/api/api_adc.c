/**
 * File : api_adc.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "hw.h"

#include "hal_adc.h"
#include "api_adc.h"

#define TURN_ON_DELAY   (1000UL)    /// 1000ms @1ms

typedef struct _adc_data_
{
    U16 Sum;
    U8  SumCount;

    U16 Average;

    U16 Min;
    U16 Max;
} SAdcData_T;

SAdcData_T  AdcData[ADC_ID_MAX];

typedef void (*PFControl_T)(void);
typedef struct _adc_config_
{
    U8          Id;               // adc id
    U8          Channel;          // adc channel
    U8          SumCount;         // adc sum count

    U16         Min;              // filtering min value
    U16         Max;              // filtering max value

    U8          OnOff;            // Adc Channel On / Off
    U16         TurnOnDelay;      // Adc Channel Turn On Delay
    U16         TurnOnDelaySet;   // Adc Channel Turn On Delay
    PFControl_T pEnable;           // Adc Channel On Function
    PFControl_T pDisable;          // Adc Channel Off Function
} SAdcConfig_T;

static void EnableTempCold(void);
static void DisableTempCold(void);
static void EnableTempRoom(void);
static void DisableTempRoom(void);

SAdcConfig_T AdcConfigList[] =
{
    /* Adc Id                     Adc Ch,  SumCount,  Initial Min  Inital Max,  OnOff,   Delay,             Set_Delay  Enable,  Disable */
    {ADC_ID_PUMP_DRAIN_IAD,       0,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_UV_DRAIN_TANK_IAD,    1,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_TEMP_HOT_WATER,       2,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_UV_ICE_DOOR_IAD,      3,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_UV_WATER_OUT_IAD,     4,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_UV_ICE_TRAY_IAD,      5,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_UV_ICE_TANK_IAD,      6,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
#if( CONFIG_TEST_TEMP_READ == 1)
    {ADC_ID_TEMP_COLD_WATER,      7,       52U,       1024U,       0U,          ON,      TURN_ON_DELAY,     TURN_ON_DELAY,  EnableTempCold,    EnableTempCold },
    {ADC_ID_TEMP_ROOM_WATER,      14,      52U,       1024U,       0U,          ON,      TURN_ON_DELAY,     TURN_ON_DELAY,  EnableTempRoom,    EnableTempRoom },
#else
    {ADC_ID_TEMP_COLD_WATER,      7,       52U,       1024U,       0U,          ON,      TURN_ON_DELAY,     TURN_ON_DELAY,  EnableTempCold,    DisableTempCold },
    {ADC_ID_TEMP_ROOM_WATER,      14,      52U,       1024U,       0U,          ON,      TURN_ON_DELAY,     TURN_ON_DELAY,  EnableTempRoom,    DisableTempRoom },
#endif
    {ADC_ID_SENSOR_INFRARED_HIGH, 8,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_TEMP_AMBIENT,         9,       52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_TEMP_EVA_1,           10,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_TEMP_EVA_2,           11,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_SENSOR_INFRARED_LOW,  12,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_FAN_IAD,              13,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_PUMP_COLD_VAD,        16,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_PUMP_COLD_IAD,        17,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_PUMP_ICE_DRAIN_IAD,   18,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },
    {ADC_ID_TEMP_AMBIENT_2,       20,      52U,       1024U,       0U,          ON,      0U,                0U,        NULL,    NULL    },    
};

#define SZ_ADC_CONF_LIST    ( sizeof(AdcConfigList)/sizeof(SAdcConfig_T) )

static void EnableTempCold(void)
{
    TURN_ON_TEMP_COLD();
}

static void DisableTempCold(void)
{
    TURN_OFF_TEMP_COLD();
}

static void EnableTempRoom(void)
{
    TURN_ON_TEMP_ROOM();
}

static void DisableTempRoom(void)
{
    TURN_OFF_TEMP_ROOM();
}

/**
 * @ brief     Function For Initialize Adc
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void InitAdc(void)
{
    U8 i = 0U;
    U8 channel = 0U;

    // Setup inital value...
    for( i=0; i<SZ_ADC_CONF_LIST; i++ )
    {
        if( AdcConfigList[i].pEnable != NULL )
        {
            AdcConfigList[i].pEnable();
        }

        channel = AdcConfigList[i].Channel;
        AdcData[i].Average = HalSampleAdc(channel);
    }
}

/**
 * @ brief          Function For Set Adc Data
 * @ detail         none
 * @ param   id     Adc Id
 * @ param   adc    Adc Average Data
 * @ return         none
**/
void SetAdcData(U8 id, U16 adc)
{
    if( id < ADC_ID_MAX )
    {
        AdcData[id].Average = adc;
    }
}

/**
 * @ brief          Function For Get Adc Data
 * @ detail         none
 * @ param   id     Adc Id
 * @ return         Adc Average Data
**/
U16 GetAdcData(U8 id)
{
    if( id >= ADC_ID_MAX )
    {
        return 0;
    }

    return AdcData[id].Average;
}

/**
 * @ brief             Function For Find Adc Id
 * @ detail            none
 * @ param   id        Adc Id
 * @ param   *index    List Index Address
 * @ return            Adc Average Data
**/
static U8 GetIndexConfList(U8 id, U8 *index)
{
    U8 i = 0U;

    for( i = 0; i < SZ_ADC_CONF_LIST; i++ )
    {
        if( AdcConfigList[i].Id == id )
        {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @ brief               Function For On Adc Channel
 * @ detail              none
 * @ param   id          Adc Id
 * @ return              none
**/
void TurnOnSensingAdc(U8 id)
{
    U8 index = 0U;

    if( GetIndexConfList(id, &index) == TRUE )
    {
        AdcConfigList[index].OnOff = ON;
    }
}

/**
 * @ brief               Function For Off Adc Channel
 * @ detail              none
 * @ param   id          Adc Id
 * @ return              none
**/
void TurnOffSensingAdc(U8 id)
{
    U8 index = 0U;

    if( GetIndexConfList(id, &index) == TRUE )
    {
        AdcConfigList[index].OnOff = OFF;
    }
}

I8 IsOnOffSensingAdc(U8 id)
{
    U8 index = 0U;
    I8 onOff = -1;

    if( GetIndexConfList(id, &index) == TRUE )
    {
        onOff = AdcConfigList[index].OnOff;
    }

    return onOff;
}

/**
 * @ brief               Function For Initialize Adc Data
 * @ detail              none
 * @ param   id          Adc Id
 * @ return              none
**/
static void InitAdcData(U8 id)
{
    U8 index = 0U;

    if( GetIndexConfList(id, &index) == TRUE )
    {
        AdcData[id].Sum = 0;
        AdcData[id].SumCount = 0;
        AdcData[id].Min = AdcConfigList[index].Min;
        AdcData[id].Max = AdcConfigList[index].Max;
    }
}

/**
 * @ brief       Function For Sum Adc Data
 * @ detail      none
 * @ param       none
 * @ return      none
**/
static void SumAdc(void)
{
    U8  index = 0U;
    U8  id = 0U;
    U8  channel = 0U;
    U8  sumCount = 0U;
    U16 data = 0U;

    for( index=0; index<SZ_ADC_CONF_LIST; index++ )
    {
        id = AdcConfigList[index].Id;
#if( CONFIG_TEST_TEMP_READ == 0 )
        if( AdcConfigList[index].OnOff == OFF )
        {
            if( AdcConfigList[index].pDisable != NULL )
            {
                AdcConfigList[index].pDisable();
                AdcConfigList[index].TurnOnDelay = AdcConfigList[index].TurnOnDelaySet;
            }

            InitAdcData(id);
        }
        else
#endif
        {       
            if( AdcConfigList[index].pEnable != NULL )
            {
                AdcConfigList[index].pEnable();

#if (CONFIG_TEST_TEMP_READ == 0)
                if( AdcConfigList[index].TurnOnDelay != 0 )
                {
                    AdcConfigList[index].TurnOnDelay--;
                    continue ;
                }
#endif
            }

            sumCount = AdcData[id].SumCount;
            if( sumCount < AdcConfigList[index].SumCount )
            {
                channel = AdcConfigList[index].Channel;
                data = HalSampleAdc(channel); 

                AdcData[id].Sum += data;
                AdcData[id].SumCount++;

                // Sampling min/max value
                if( data < AdcData[id].Min )
                {
                    AdcData[id].Min = data;
                }

                if( data > AdcData[id].Max )
                {
                    AdcData[id].Max = data;
                }
            }
        }
    }
}

/**
 * @ brief       Function For Average Adc Sum Data
 * @ detail      none
 * @ param       none
 * @ return      none
**/
static void AverageAdc(void)
{
    U8 i = 0U;
    U8 id = 0U;
    U8 sumCount = 0U;
    SAdcData_T *pAdcData = NULL;

    for( i=0; i<SZ_ADC_CONF_LIST; i++ )
    {
        id = AdcConfigList[i].Id;
        pAdcData = &AdcData[id];

        sumCount = pAdcData->SumCount;
        if(  AdcConfigList[i].SumCount == sumCount )
        {
            pAdcData->Sum -= (pAdcData->Min + pAdcData->Max);
            pAdcData->Average = pAdcData->Sum / (sumCount - 2);

            InitAdcData(id);
        }
    }
}

/**
 * @ brief       Function For Control Adc
 * @ detail      none
 * @ param       none
 * @ return      none
**/
static void ControlAdc(void)
{
    SumAdc();
    AverageAdc();
}

/**
 * @ brief       Function For Process Adc (External)
 * @ detail      none
 * @ param       none
 * @ return      none
**/
void ProcessAdc(void)
{
    ControlAdc();
}
