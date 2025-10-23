/**
 * File : circulate_pump.c
 * 
 * Circulate Pump
*/
#include "api_pump.h"
#include "api_adc.h"
#include "circulate_pump.h"
#include "valve.h"
#include "comp.h"
#include "temp.h"

#define CONFIG_REPEAT_PUMP         (0)     /// ��ȯ ���� �ݺ� ���� 
#define CONFIG_USE_PURGE           (1)     /// ��ȯ ���� PURGE ���� ���
#define RETRY_COUNT                (22)

// CHECK ERROR TIMER
#define CHECK_ERROR_COUNT          (5U)
#define CHECK_ERROR_OFF_TIME       (500U)   //@10ms, 5sec
#define CHECK_ERROR_DELAY_TIME     (300U)   //@10ms, 3sec
#define CHECK_NORMAL_TIME          (300U)   //@10ms, 3sec
#define CHECK_ERROR_TIME           (1000U)  //@10ms, 10sec
#define CHECK_ERROR_ADC            (60U)   // 0.3V

SCirculatePump_T CirculatePump;

#if( CONFIG_USE_PURGE == 1 )
#define PURGE_CYCLE_MAX (3U)
#define PURGE_OFF_TIME  (50U)    /// 0.5sec @10ms
#define PURGE_ON_TIME   (50U)    /// 0.5sec @10ms
#endif

static void InitCheckError(void);
static void CheckErrorCirculatePump(void);

void InitCirculatePump(void)
{
    CirculatePump.Cmd = OFF;
    CirculatePump.Cmd_MakeCold = OFF;
    CirculatePump.Cmd_MakeColdForced = OFF;
    CirculatePump.Cmd_WaterOut = OFF;
    CirculatePump.Cmd_TrayIn = OFF;
    CirculatePump.Cmd_IceWaterOut = OFF;
    
    CirculatePump.RetryCmd = OFF;
    CirculatePump.RetryCount = 0;
    CirculatePump.OnOffTime = 0;

    // Error
    CirculatePump.Error = FALSE;
    CirculatePump.ErrorCount = 0;
    CirculatePump.CheckOffTime = 0;
    CirculatePump.CheckDelayTime = CHECK_ERROR_DELAY_TIME;
    CirculatePump.CheckNormalTime = CHECK_NORMAL_TIME;
    CirculatePump.CheckErrorTime  = CHECK_ERROR_TIME;

#if( CONFIG_USE_PURGE == 1 )
    CirculatePump.Purge = FALSE;
    CirculatePump.Cycle = PURGE_CYCLE_MAX;
    CirculatePump.PurgeOffTime = PURGE_OFF_TIME;
    CirculatePump.PurgeOnTime = PURGE_ON_TIME;
#endif
}

void GetCirculatePumpData(SCirculatePump_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&CirculatePump, sizeof(SCirculatePump_T) );
}

U8 GetCirculatePumpStatus(void)
{
    return CirculatePump.Cmd;
}

U8 GetCirculatePumpError(void)
{
    return CirculatePump.Error;
}

// �ð� ���� ���� ON/OFF
void TurnOnCirculatePumpMakeCold(void)
{
    CirculatePump.Cmd_MakeCold = ON;
}

void TurnOffCirculatePumpMakeCold(void)
{
    CirculatePump.Cmd_MakeCold = OFF;
}

// �ð� ���� ���� ���� ON/OFF
void TurnOnCirculatePumpMakeColdForced(void)
{
    CirculatePump.Cmd_MakeColdForced = ON;
}

void TurnOffCirculatePumpMakeColdForced(void)
{
    CirculatePump.Cmd_MakeColdForced = OFF;
}

// �ü� ���� ���� ���� ON/OFF
void TurnOnCirculatePumpWaterOut(void)
{
    CirculatePump.Cmd_WaterOut = ON;
}

void TurnOffCirculatePumpWaterOut(void)
{
    CirculatePump.Cmd_WaterOut = OFF;
}

// ������ �Լ� ���� ���� ON/OFF
void TurnOnCirculatePumpTrayIn(void)
{
    CirculatePump.Cmd_TrayIn = ON;
}

void TurnOffCirculatePumpTrayIn(void)
{
    CirculatePump.Cmd_TrayIn = OFF;
}

// ������  ���� ���� ���� ON/OFF
void TurnOnCirculatePumpIceWaterOut(void)
{
    CirculatePump.Cmd_IceWaterOut = ON;
}

void TurnOffCirculatePumpIceWaterOut(void)
{
    CirculatePump.Cmd_IceWaterOut = OFF;
}

void TurnOnCirculatePump(void)
{
    CirculatePump.Cmd = ON;
}

void TurnOffCirculatePump(void)
{
    CirculatePump.Cmd = OFF;

    CirculatePump.RetryCmd = 0;
    CirculatePump.RetryCount = 0;
    CirculatePump.OnOffTime = 0;
}

// �����׸��ý��� ������ �µ� ������ ��ȯ ���� ���� ���
static U8 GetCmdCheckRegion(TEMP_T tCold, U8 cmd)
{
    if( cmd == ON )
    {
        if( tCold >= 5.5f )
        {
            return OFF;
        }
    }
    else 
    {
        if( tCold <= 5.0f )
        {
            return ON;
        }
    }
    
    return cmd;
}

#if( CONFIG_REPEAT_PUMP == 1 )
static const U8 OnOffCmdList[] = 
{
    1,    0,     1,    0,     1,    /* 0 ~ 4 */
    0,    1,     0,    1,     0,    /* 5 ~ 9 */
    1,    0,     1,    0,     1,    /* 10 ~ 14 */
    0,    1,     0,    1,     0,    /* 15 ~ 19 */
    1,    0,     1                  /* 20 ~ 23 */
};

static const U16 OnOffTimeList[] =
{ 
    6000,      600,    1200,    600,     1200,    /* 0 ~ 4 */
     600,     1200,     600,   1200,      600,     /* 5 ~ 9 */
    1200,      600,    1200,    600,     1200,   /* 10 ~ 14 */
     600,     1200,     600,   1200,      600,    /* 15 ~ 19 */
    1200,      600,    1200                     /* 20 ~ 23 */
};
#endif

#if( CONFIG_USE_PURGE == 1 )

static void ClearPurgeData(void)
{   
    CirculatePump.Cycle = PURGE_CYCLE_MAX;
    CirculatePump.PurgeOffTime = PURGE_OFF_TIME;
    CirculatePump.PurgeOnTime = PURGE_ON_TIME;
}

static U8 GetCmdForPurge(void)
{
    if( CirculatePump.Cycle == 0U )
    {
        return CirculatePump.Cmd;
    }

    if( CirculatePump.PurgeOnTime != 0U )
    {
        CirculatePump.PurgeOnTime--;
        return ON;
    }

    if( CirculatePump.PurgeOffTime != 0U )
    {
        CirculatePump.PurgeOffTime--;
        return OFF;
    }
    
    CirculatePump.PurgeOffTime = PURGE_OFF_TIME;
    CirculatePump.PurgeOnTime = PURGE_ON_TIME;

    CirculatePump.Cycle--;

    return OFF;
}

#endif

#if 1
void ControlCirculatePump(void)
{
    /* CHECK PUMP ON/OFF COMMAND */

    /* ù°, �ð� ��� */
    CirculatePump.Cmd = OFF;
#if( CONFIG_USE_PURGE == 1 )   
    CirculatePump.Purge = FALSE;
#endif
    if( CirculatePump.Cmd_MakeCold == ON )
    {
        // �ð� ��忡�� ��ȯ ���� ������ ����Ⱑ ON�� ��,
        //if( GetCompOnOff() == ON )
        {
            CirculatePump.Cmd = ON;
#if( CONFIG_USE_PURGE == 1 )
            CirculatePump.Purge = TRUE;
#endif
        }

        if( CirculatePump.CheckOffTime != 0 )
        {
            CirculatePump.CheckOffTime--;
            CirculatePump.Cmd = OFF;
        }
    }

    /* �ð� ��� ���� ���� */
    if( CirculatePump.Cmd_MakeColdForced == ON )
    {
        CirculatePump.Cmd = ON;
#if( CONFIG_USE_PURGE == 1 )   
        CirculatePump.Purge = FALSE;
#endif

    }

    /* ��°, �ü� ���� */
    if( CirculatePump.Cmd_WaterOut == ON )
    {
        CirculatePump.Cmd = ON;
#if( CONFIG_USE_PURGE == 1 )   
        CirculatePump.Purge = FALSE;
#endif

        OpenValve(VALVE_COLD_CIRCULATE);
    }
    else
    {
        CloseValve(VALVE_COLD_CIRCULATE);
    }

    /* ��°, Ʈ���� �Լ�   */
    if( CirculatePump.Cmd_TrayIn == ON )
    {
        CirculatePump.Cmd = ON;
#if( CONFIG_USE_PURGE == 1 )   
        CirculatePump.Purge = FALSE;
#endif

    }

    /* ��°, ������ ����   */
    if( CirculatePump.Cmd_IceWaterOut == ON )
    {
        CirculatePump.Cmd = ON;
#if( CONFIG_USE_PURGE == 1 )   
        CirculatePump.Purge = FALSE;
#endif
        OpenValve(VALVE_ICE_WATER_OUT);
    }
    else
    {
        CloseValve(VALVE_ICE_WATER_OUT);
    }

#if( CONFIG_USE_PURGE == 1 )
    if( CirculatePump.Purge == TRUE )
    {
        CirculatePump.Cmd = GetCmdForPurge();
    }
    else
    {
        ClearPurgeData();
    }
#endif

    /* TURN OFF PUMP */
    if( CirculatePump.Cmd == OFF )
    {
        TurnOffPump(PUMP_ID_CIRCULATE);
        InitCheckError();
        return;
    }

    /* TURN ON PUMP */
#if (CONFIG_REPEAT_PUMP == 0)
    TurnOnPump(PUMP_ID_CIRCULATE, 0);
#else
    if( CirculatePump.OnOffTime == 0 )
    {
        CirculatePump.RetryCmd = OnOffCmdList[ CirculatePump.RetryCount ];
        CirculatePump.OnOffTime = OnOffTimeList[ CirculatePump.RetryCount ];

        CirculatePump.RetryCount++;
        if( CirculatePump.RetryCount > RETRY_COUNT )
        {
            CirculatePump.Error = TRUE;
        }

        return;
    }

    if( CirculatePump.OnOffTime != 0 )
    {
        CirculatePump.OnOffTime--;
    }

    if( CirculatePump.RetryCmd == 1 )
    {
        TurnOnPump(PUMP_ID_CIRCULATE, 0);
    }
    else
    {
        TurnOffPump(PUMP_ID_CIRCULATE);
    }
#endif

    /* CHECK ERROR */
    CheckErrorCirculatePump();
}

void ControlCirculatePumpEol(void)
{
    /* CHECK PUMP ON/OFF COMMAND */

    /* ù°, �ð� ��� */
    CirculatePump.Cmd = OFF;
    if( CirculatePump.Cmd_MakeCold == ON )
    {
        // �ð� ��忡�� ��ȯ ���� ������ ����Ⱑ ON�� ��,
        //if( GetCompOnOff() == ON )
        {
            CirculatePump.Cmd = ON;
        }

        if( CirculatePump.CheckOffTime != 0 )
        {
            CirculatePump.CheckOffTime--;
            CirculatePump.Cmd = OFF;
        }
    }

    /* �ð� ��� ���� ���� */
    if( CirculatePump.Cmd_MakeColdForced == ON )
    {
        CirculatePump.Cmd = ON;
    }

    /* TURN OFF PUMP */
    if( CirculatePump.Cmd == OFF )
    {
        TurnOffPump(PUMP_ID_CIRCULATE);
        InitCheckError();
        return ;
    }

    /* TURN ON PUMP */
#if( CONFIG_REPEAT_PUMP == 0 )
    TurnOnPump(PUMP_ID_CIRCULATE, 0);
#else
    if( CirculatePump.OnOffTime == 0 )
    {
        CirculatePump.RetryCmd  = OnOffCmdList[ CirculatePump.RetryCount ];
        CirculatePump.OnOffTime = OnOffTimeList[ CirculatePump.RetryCount ];

        CirculatePump.RetryCount++;
        if( CirculatePump.RetryCount > RETRY_COUNT )
        {
            CirculatePump.Error = TRUE;
        }

        return ;
    }

    if( CirculatePump.OnOffTime != 0 )
    {
        CirculatePump.OnOffTime--;
    }

    if( CirculatePump.RetryCmd == 1 )
    {
        TurnOnPump(PUMP_ID_CIRCULATE, 0);
    }
    else
    {
        TurnOffPump(PUMP_ID_CIRCULATE);
    }
#endif

    /* CHECK ERROR */
    CheckErrorCirculatePump();
}


#else
void ControlCirculatePump(void)
{
    U8 mu8ColdCmd = OFF;   


    // �ð� ��ȯ ���� ���� ��?
    // ��ȯ ���� CLOSE, ������ ON ���̸� �ð���ȯ ���� ������
    //if( IsOpenValve( VALVE_COLD_CIRCULATE ) == FALSE && CirculatePump.Cmd == ON )
    //{
    //    mu8ColdCmd = ON;
    //}

    /* ù°, �ð� ��� */
    if( CirculatePump.Cmd_MakeCold == ON )
    {

        // �ð� ��忡�� ��ȯ ���� ������ ����Ⱑ ON�� ��,
        if( GetCompOnOff() == OFF )
        {
            mu8ColdCmd = OFF;
        }
        else
        {
            // �ü� �µ��� ���� ��ȯ ���� ���� ��� ����
            mu8ColdCmd = GetCmdCheckRegion( GetTemp( TEMP_ID_COLD_WATER ), mu8ColdCmd );
        }

        if( CirculatePump.CheckOffTime != 0 )
        {
            CirculatePump.CheckOffTime--;
            mu8ColdCmd = OFF;
        }
    }
    CirculatePump.Cmd = mu8ColdCmd;


    /* ��°, �ü� ���� */
    if( CirculatePump.Cmd_WaterOut == ON )
    {
        CirculatePump.Cmd = ON;
        OpenValve( VALVE_COLD_CIRCULATE );
    }
    else
    {
        CloseValve( VALVE_COLD_CIRCULATE );
    }


    /* TURN OFF PUMP */
    if( CirculatePump.Cmd == OFF )
    {
        HAL_TurnOffPump( HAL_PUMP_CIRCULATE );
        InitCheckError();
        return ;
    }

    /* TURN ON PUMP */
#if (CONFIG_REPEAT_PUMP == 0)
    HAL_TurnOnPump( HAL_PUMP_CIRCULATE, 24 );
#else
    if( CirculatePump.OnOffTime == 0 )
    {
        CirculatePump.RetryCmd  = OnOffCmdList[ CirculatePump.RetryCount ];
        CirculatePump.OnOffTime = OnOffTimeList[ CirculatePump.RetryCount ];

        CirculatePump.RetryCount++;
        if( CirculatePump.RetryCount > RETRY_COUNT )
        {
            CirculatePump.Error = TRUE;
        }

        return ;
    }

    if( CirculatePump.OnOffTime != 0 )
    {
        CirculatePump.OnOffTime--;
    }

    if( CirculatePump.RetryCmd == 1 )
    {
        HAL_TurnOnPump( HAL_PUMP_CIRCULATE, 24 );
    }
    else
    {
        HAL_TurnOffPump( HAL_PUMP_CIRCULATE );
    }
#endif

    /* CHECK ERROR */
    CheckErrorCirculatePump();
}
#endif

static void InitCheckError(void)
{
    CirculatePump.CheckDelayTime = CHECK_ERROR_DELAY_TIME;
    CirculatePump.CheckNormalTime = CHECK_NORMAL_TIME;
    CirculatePump.CheckErrorTime = CHECK_ERROR_TIME;
}

// ���� ����
// �ü� ��ȯ ������ ������ �����Ѵ�.
// �ü� ���� ���ǿ����� �������� ����.
// Ʈ���� �Լ� ���ǿ����� �������� ����.
// ������ ���� ���ǿ����� �������� ����.
static void CheckErrorCirculatePump(void)
{
#if( CONFIG_CIRCULATE_PUMP == 1 )
    U16 adc = 0;

    // �ü� ���� ���ǿ����� ���� ���� ����
    if( CirculatePump.Cmd_WaterOut == ON
        || CirculatePump.Cmd_TrayIn == ON
        || CirculatePump.Cmd_IceWaterOut == ON )
    {
        InitCheckError();
        return;
    }

    // �˻� ���� �� ���� �ð�
    if( CirculatePump.CheckDelayTime != 0 )
    {
        CirculatePump.CheckDelayTime--;
        return;
    }

    // ���� ���۽� ���� ���� ADC ���� 350 ~ 370 (1.7v ~ 1.8v )
    // 0.3V �̻� �����Ǹ� ����ó��
    adc = GetAdcData(ADC_ID_PUMP_COLD_IAD);
    if( adc >= CHECK_ERROR_ADC ) 
    {
        // ���� ����
        if( CirculatePump.CheckNormalTime != 0 )
        {
            CirculatePump.CheckNormalTime--;
        }
        CirculatePump.CheckErrorTime = CHECK_ERROR_TIME;
    }
    else
    {
        // ���� ����
        CirculatePump.CheckNormalTime = CHECK_NORMAL_TIME;
        if( CirculatePump.CheckErrorTime != 0 )
        {
            CirculatePump.CheckErrorTime--;
        }
    }
            

    if( CirculatePump.CheckNormalTime == 0 )
    {
        // ����
        CirculatePump.ErrorCount    = 0;
        CirculatePump.CheckOffTime   = 0;
    }
    else if( CirculatePump.CheckErrorTime == 0 )
    {
        // ����
        CirculatePump.ErrorCount++;
        if( CirculatePump.ErrorCount > 5 )
        {
            CirculatePump.Error = TRUE;
        }
        else
        {
            CirculatePump.CheckOffTime = CHECK_ERROR_OFF_TIME;
        }
    }
#endif
}
