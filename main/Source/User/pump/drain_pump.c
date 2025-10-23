/**
 * File : drain_pump.c
 * 
 * Drain Pump For Empty Drain Tank
*/
#include "drain_pump.h"

#include <string.h>

#include "api_pump.h"
#include "api_adc.h"

#define RETRY_COUNT         (44)
#define DEFAULT_LOAD_TIME   (5)
#define DEFAULT_DEAD_TIME   (5)

/// drain pump 무부하 조건 port 입력 전압 : 0.44 ~ 0.66[v] == 90 ~ 135[adc]
/// 계산치는 : 135 [adc]
/// 측정치는 : 140 [adc]
/// 적용수치는 :  175[adc]  계산치에 + 30% 가산
///
/// [펌프 ADC 데이터 측정]
/// 구분	                    ADC
/// 무부하	                110
/// 부하 - 짧은 유로	        265 ~ 271
/// 부하 - 긴 유로	        300 ~330
/// 보호동작 걸리는 시점	    390 ~ 400
/// 보호 동작 발생 후	    20

#define PROTECT_DEAD_ADC    (40)
#define NO_LOAD_ADC         (175)

SDrainPump_T DrainPump;

void InitDrainPump(void)
{
    DrainPump.Cmd = OFF;
    DrainPump.RetryCmd = OFF;
    DrainPump.RetryCount = 0;
    DrainPump.OnOffTime = 0;
    DrainPump.Error = FALSE;

    // 펌프 부하 검사
    DrainPump.LoadTime = DEFAULT_LOAD_TIME;
    DrainPump.Load = FALSE;

    DrainPump.DeadTime = DEFAULT_DEAD_TIME;
    DrainPump.Dead = FALSE;
}

void GetDrainData(SDrainPump_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DrainPump, sizeof(SDrainPump_T) );
}

U8 GetDrainPumpStatus(void)
{
    return DrainPump.Cmd;
}

void ClearDrainPumpRetryCount(void)
{
    DrainPump.RetryCount = 0;
}

U8 GetDrainPumpError(void)
{
    return DrainPump.Error;
}

void TurnOnDrainPump(void)
{
    DrainPump.Cmd = ON;
}

void TurnOffDrainPump(void)
{
    DrainPump.Cmd = OFF;

    DrainPump.RetryCmd     = 0;
    DrainPump.RetryCount   = 0;
    DrainPump.OnOffTime    = 0;
}

static const U8 OnOffCmdList[] = 
{
    1,    0,     
    1,    0,     1,    0,    1,    0,    
    1,    0,     1,    0,    1,    0,     
    1,    0,     1,    0,    1,    0,   
    1,    0,     1,    0,    1,    0,    
    1,    0,     1,    0,    1,    0,     
    1,    0,     1,    0,    1,    0,    
    1,    0,     1,    0,    1,    0,     
    1                  
};

static const U16 OnOffTimeList[] = 
{ 
    900,    60,    
    120,    60,     120,     60,    120,   60,     
    120,    60,     120,     60,    120,   60,     
    120,    60,     120,     60,    120,   60,      
    120,    60,     120,     60,    120,   60,     
    120,    60,     120,     60,    120,   60,    
    120,    60,     120,     60,    120,   60,     
    120,    60,     120,     60,    120,   60,     
    120   
};

// 배수 펌프 부하 확인 ( 물이 없는 경우 )
// RETURN
//  - TRUE - 부하가 있음.
//  - FALSE - 부하가 없음
U8 GetDrainPumpLoadStatus(void)
{
    return DrainPump.Load;
}

static void InitDrainPumpLoad(void)
{
  DrainPump.Load      = TRUE;
  DrainPump.LoadTime  = DEFAULT_LOAD_TIME;

  DrainPump.Dead      = TRUE;
  DrainPump.DeadTime  = DEFAULT_DEAD_TIME;
}

static void CheckDrainPumpLoad(void)
{
    U16 adc;
  
    adc = GetAdcData(ADC_ID_PUMP_DRAIN_IAD);

    // Count load time
    if( adc < PROTECT_DEAD_ADC )
    {
        if( DrainPump.DeadTime != 0 )
        {
            DrainPump.DeadTime--;
        }
        else
        {
            DrainPump.Dead = TRUE;
        }
    }
    else
    {
        DrainPump.Dead = FALSE;
        DrainPump.DeadTime = DEFAULT_DEAD_TIME;
    }

    if( adc < NO_LOAD_ADC )
    {
        if( DrainPump.LoadTime != 0 )
        {
            DrainPump.LoadTime--;
        }

        if( DrainPump.LoadTime == 0 )
        {
            DrainPump.Load = FALSE;
        }

        return ;
    }

    // Reload load time
    DrainPump.Load = TRUE;
    DrainPump.LoadTime = DEFAULT_LOAD_TIME;
}

// 최초 기동시 24V 제어
// 3초후 19V로 제어
// 2번째 부터 다시 24V로 제어
#define DRAIN_PUMP_DUTY     (100U)    // Max : 24V
void ControlDrainPump(void)
{
    U8 mu8OnOff;

    if( DrainPump.Cmd == OFF 
            || DrainPump.Error == TRUE )
    {
        TurnOffPump(PUMP_ID_DRAIN);
        InitDrainPumpLoad();
        return;
    }


    if( DrainPump.OnOffTime == 0 )
    {
        DrainPump.RetryCmd  = OnOffCmdList[ DrainPump.RetryCount ];
        DrainPump.OnOffTime = OnOffTimeList[ DrainPump.RetryCount ];

        DrainPump.RetryCount++;
        if( DrainPump.RetryCount > RETRY_COUNT )
        {
            DrainPump.Error = TRUE;
        }

        return ;
    }


    if( DrainPump.OnOffTime != 0 )
    {
        DrainPump.OnOffTime--;
    }

    if( DrainPump.RetryCmd == 1 )
    {
        if( DrainPump.Dead == TRUE )
        {
            // 펌프가 protection으로 off되면, 펌프 전원을 off해서 reset 시킨다.
            DrainPump.Dead      = FALSE;
            DrainPump.DeadTime  = DEFAULT_DEAD_TIME;
            TurnOffPump(PUMP_ID_DRAIN);
        }
        else
        {
            TurnOnPump(PUMP_ID_DRAIN, DRAIN_PUMP_DUTY);
        }

        CheckDrainPumpLoad();
    }
    else
    {
        TurnOffPump(PUMP_ID_DRAIN);
        InitDrainPumpLoad();
    }
}
