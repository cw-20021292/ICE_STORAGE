#include "hal_adc.h"

#include "power_saving.h"
#include "eeprom.h"


/* 0.1V @ 200 ADC */
#define  OFFSET_ADC  (-1)
//#define  DEFAULT_ADC_LIMIT_SLEEP    (510 + OFFSET_ADC)
//#define  DEFAULT_ADC_LIMIT_WAKE_UP  (880 + OFFSET_ADC)
#define  DEFAULT_ADC_LIMIT_SLEEP    (327 + OFFSET_ADC)
#define  DEFAULT_ADC_LIMIT_WAKE_UP  (632 + OFFSET_ADC)

PowerSaving_T   Saving;

static void CountTimerSleep(void);
static void CountTimerWakeUp(void);
static void ResetTimer(void);

void InitSaving(void)
{
    Saving.Config = TRUE;

    Saving.PrevStatus = STATUS_WAKE_UP;       
    Saving.Status     = STATUS_WAKE_UP;    
    Saving.Dark2Light = FALSE; 
    Saving.Light2Dark = FALSE;

    Saving.IsDark          = FALSE;  

    Saving.AdcLimitSleep   = DEFAULT_ADC_LIMIT_SLEEP; 
    Saving.AdcLimitWakeUp  = DEFAULT_ADC_LIMIT_WAKE_UP; 
    Saving.TimerSleep      = DEFAULT_SLEEP_TIMER;      
    Saving.TimerWakeUp     = DEFAULT_WAKE_UP_TIMER;   
}

void GetSavingData(PowerSaving_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Saving, sizeof( PowerSaving_T ));
}

void SetSavingConfig(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }
    Saving.Config = mu8Val;

    SaveEepromId( EEP_ID_CONF_POWER_SAVING );
}

U8 GetSavingConfig(void)
{
    return Saving.Config;
}

U8 GetSavingStatus(void)
{
    // ������ �����̸�, WAKE_UP ���¸� ��ȯ�Ѵ�.
    if( Saving.Config == FALSE )
    {
        return STATUS_WAKE_UP;
    }

    return Saving.Status;
}

// Get/Set Sleep Timer 
U16 GetSavingConfTimerSleep(void)
{
    return Saving.TimerSleep;
}

void SetSavingConfTimerSleep(U16 mu16Timer)
{
    Saving.TimerSleep = mu16Timer;
}


// Get/Set Wakeup Timer 
U16 GetSavingConfTimerWakeUp(void)
{
    return Saving.TimerWakeUp;
}

void SetSavingConfTimerWakeUp(U16 mu16Timer)
{
    Saving.TimerWakeUp = mu16Timer;
}

void WakeUpSavingMode(void)
{
    Saving.Status       = STATUS_WAKE_UP;

    ResetTimer();
}

U8 GetSavingStatusDark2Light(void)
{
    U8 mu8Ret;

    mu8Ret = Saving.Dark2Light;
    Saving.Dark2Light = FALSE;

    return mu8Ret; 
}

U8 GetSavingStatusLight2Dark(void)
{
    U8 mu8Ret;

    mu8Ret = Saving.Light2Dark;
    Saving.Light2Dark = FALSE;

    return mu8Ret; 
}

static void CountTimerSleep(void)
{
    if( Saving.TimerSleep != 0 )
    {
        Saving.TimerSleep--;
    }
    Saving.TimerWakeUp = DEFAULT_WAKE_UP_TIMER;
}

static void CountTimerWakeUp(void)
{
    Saving.TimerSleep = DEFAULT_SLEEP_TIMER;
    if( Saving.TimerWakeUp != 0 )
    {
        Saving.TimerWakeUp--;
    }
}

static void ResetTimer(void)
{
    Saving.TimerSleep   = DEFAULT_SLEEP_TIMER;
    Saving.TimerWakeUp  = DEFAULT_WAKE_UP_TIMER;
}

/**
 * ���� ������ ���ؼ� SLEEP ��� ���� ���� ����.
 * �¼� ��ɿ��� �̻�� ���� ���� ���� �߿� ���� ������ ���� �� ���� ������ ����.
 * �̸� ���ؼ� Dart2Light�� Light2Dark ���� ������ �߰��� 
 */
void ProcessSaving(void)
{
    U16 mu16Adc;


    /* ���� ADC ������ �ֺ� ��� ����  */
    //mu16Adc = GetAdcData(ANI_SAVING_PHOTO );
    if( mu16Adc < Saving.AdcLimitSleep )
    {
        Saving.IsDark = TRUE;
    }
    else if( mu16Adc > Saving.AdcLimitWakeUp )
    {
        Saving.IsDark = FALSE;
    }
    else
    {
        ResetTimer();
    }

    /* ���� Ÿ�̸� ���� */
    if( Saving.IsDark == TRUE )
    {
        CountTimerSleep();
    }
    else
    {
        CountTimerWakeUp();
    }

    /* ���� */
//    if( Saving.Config == FALSE )
//    {
//        Saving.Status = STATUS_WAKE_UP;
//    }
//    else
    {
        if( Saving.TimerSleep == 0 )
        {
            Saving.Status = STATUS_SLEEP;
        }
        else if( Saving.TimerWakeUp == 0 )
        {
            Saving.Status = STATUS_WAKE_UP;
        }
    }

    /* ���� ��ȭ ���� ������Ʈ */
    if( Saving.PrevStatus != Saving.Status )
    {
        Saving.PrevStatus = Saving.Status;
        if( Saving.Status == STATUS_WAKE_UP )
        {
            Saving.Dark2Light = TRUE;
            Saving.Light2Dark = FALSE;
        }
        else if( Saving.Status == STATUS_SLEEP )
        {
            Saving.Dark2Light = FALSE;
            Saving.Light2Dark = TRUE;
        }
    }
}

