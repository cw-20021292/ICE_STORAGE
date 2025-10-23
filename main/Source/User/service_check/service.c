#include "hw.h"
#include "service.h"
#include "eeprom.h"
#include "ice.h"
#include "ice_maker.h"
#include "hot_water.h"
#include "drain_pump.h"
#include "input.h"

#define	ONE_DAY_MIN ( 1440U )	/* 60min x 24hour = 1440 min  */


Service_T   Svc;

void InitServiceCheck(void)
{
    Svc.TankOpen = 0;               /* Tank Open, FALSE:CLOSED, TRUE: OPEN */

    Svc.Status = FALSE;             /* Service Need, FALSE : NOT YET, TRUE : Need Service */
    Svc.Time   = SERVICE_TIME_MIN;
    Svc.SaveTime = ONE_DAY_MIN;     // EEPROM�� ������Ʈ �ֱ� 

    Svc.OpenTime = 0U;              /* ���� ���� TANK OPEN �ð� @sec */
}

void GetServiceCheckData(Service_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Svc, sizeof(Service_T));
}

U8 GetServiceCheckTankOpen(void)
{
    return Svc.TankOpen;
}

U8 GetServiceCheckStatus(void)
{
    return Svc.Status;
}

void SetServiceTime(U32 mu32Time)
{
    Svc.Time = mu32Time;
}

U32 GetServiceTime(void)
{
    return Svc.Time;
}

void SetServiceCheckDay(U8 mu8Days)
{
    /*  DAYS * 24 HOUR * 60 MIN */
    Svc.Time = (U32)mu8Days * 24UL * 60UL;
}

U8 GetServiceCheckDay(void)
{
    U32 mu32Time;

    mu32Time = Svc.Time * 10UL;     
    mu32Time = mu32Time / 24UL / 60UL;
    mu32Time = mu32Time + 5UL;              /* ROUND UP */
    mu32Time = mu32Time / 10UL;

    return (U8)mu32Time;
}

// TANK OPENING TIME.. @sec
void SetServiceOpenTime(U16 mu16Time)
{
    Svc.OpenTime = mu16Time;
}

U16 GetServiceOpenTime(void)
{
    return Svc.OpenTime;
}

static void ResetServiceCheckTimer(void)
{
    Svc.Status = FALSE;
    Svc.Time   = SERVICE_TIME_MIN;
}


static U8 gu8Sec = 10U;
static void CountingServiceOpenTime(void)
{
    if( gu8Sec != 0 )
    {
        gu8Sec--;
    }
    else
    {
        gu8Sec = 10UL;

        if( Svc.OpenTime < 65535 )
        {
            Svc.OpenTime++;
        }
    }
}

void ResetServiceOpenTime(void)
{
    gu8Sec = 10U;
    Svc.OpenTime = 0U;
}

void ProcessCheckTankOpen(void)
{
    U8 mu8Val;

    mu8Val = GetCurrentInputVal(INPUT_ID_TANK_OPEN);

    /* Tank is open, Clear timer */
    if( Svc.TankOpen != mu8Val )
    {
        Svc.TankOpen = mu8Val;

        if( Svc.TankOpen == TRUE )
        {
            SaveEepromId(EEP_ID_SERVICE_CHECK_DAY);

            // When the tank is opened, the ice making stops.
            StopDummyDeIcing();
            StopIceMake();

#if 0

  // IGLOO

            SetIoCareMode( IOC_ID_SERVICE_CHECK_DAY );
#endif
        }
    }

    if( Svc.TankOpen == TRUE )
    {
        ResetServiceCheckTimer();
        CountingServiceOpenTime();
        ClearDrainPumpRetryCount();
    }
    else
    {
        ResetServiceOpenTime();
        ClearHotMakeDelayTankOpen();
    }
}

void ProcessServiceCheck(void)
{
    if( Svc.TankOpen == TRUE )
    {
        return ;  // Tank Open!!
    }

    /* Tank is closed, count timer */
    if( Svc.Time != 0 )
    {
        Svc.Time--;
    }

    /* Tank is closed, timeout !!! */
    if( Svc.Time == 0 && Svc.Status == FALSE )
    {
        Svc.Status = TRUE;
        SaveEepromId( EEP_ID_SERVICE_CHECK_DAY );
#if 0
        
  // IGLOO

        SetIoCareMode( IOC_ID_SERVICE_CHECK_DAY );
#endif

        return ;
    }


    /* Save Eeprom 1-day cycle */
    if( Svc.Time != 0 && Svc.TankOpen == FALSE && Svc.Status == FALSE )
    {
        // �Ϸ翡 �� ��(24�ð�) �������� eeprom�� ����
        Svc.SaveTime--;
        if( Svc.SaveTime == 0 )
        {
            Svc.SaveTime = ONE_DAY_MIN;
            SaveEepromId( EEP_ID_SERVICE_CHECK_DAY );
#if 0

  // IGLOO

            SetIoCareMode( IOC_ID_SERVICE_CHECK_DAY );
#endif
        }
    }
}

