/**
 * File : ice.c
 * 
 * Ice Control
*/
#include "ice.h"
#include "ice_full.h"
#include "ice_low.h"
#include "eeprom.h"
#include "error.h"
#include "health.h"
#include "power_saving.h"
#include "room_water.h"
#include "cold_water.h"
#include "comp.h"
#include "front.h"
#include "drain_water.h"
#include "tank_clean.h"
#include "temp.h"
#include "service.h"
#include "flow_meter.h"
#include "input.h"

#include <string.h>

/* ICE FULL */
#define  ICE_CHECK_TIME      (900)   /// @1sec, 15min

#define  ICE_STOP_TIME       (14400) /// @1sec, 4Hour

/* DE-ICING ���� �ð� */
#define MIN_DEICING_TIME     (180U)  ///@1sec, 3min

#define DEFAULT_RETURN_TIME  (700)   /// 7sec @10ms

#define ICING_MAX_COUNT      (10)    /// 10ȸ �ƽ� ī��Ʈ ( 10ȸ������ �ü�ä�� ���� )

SIce_T Ice;

void InitIce(void)
{
    Ice.Turbo = FALSE;

    Ice.ConfigMake = TRUE;
    Ice.Make = FALSE;

    Ice.Mode = ICE_MODE_DUMMY_DEICING;

    Ice.FullCheckTimer = 0;
    Ice.StopMakeTimer = 0;

    Ice.DummyDeIcing = FALSE;

    Ice.DeIcingCompTime = MIN_DEICING_TIME;

    Ice.IcingCount = ICING_MAX_COUNT;
}

void GetIceData(SIce_T *pIce)
{
    MEMCPY( (void __FAR *)pIce, (const void __FAR *)&Ice, sizeof(SIce_T) );
}

void SetIceTurbo(U8 val)
{
    if( val != TRUE && val != FALSE )
    {
        return ;
    }
    Ice.Turbo = val;

    SaveEepromId( EEP_ID_CONF_TURBO );
}

U8  GetIceTurbo(void)
{
    return Ice.Turbo;
}

void SetIceConfigMake(U8 val)
{
    if( val != TRUE && val != FALSE )
    {
        return ;
    }

    Ice.ConfigMake = val;

    SaveEepromId(EEP_ID_CONF_MAKE_ICE);
}

U8 GetIceConfigMake(void)
{
    return Ice.ConfigMake;
}

void  SetIceMake(U8 val)
{
   Ice.Make = val;
}

U8 GetIceMake(void)
{
    return Ice.Make;
}


// ���� ���� �µ� �˻� 
U8 IsValidDoIcingAmbiTempRange(void)
{
    TEMP_T  tAmbi;

    tAmbi = GetTemp( TEMP_ID_AMBIENT_2 );
    if( tAmbi < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

U8 IsValidDoIcingColdTempRange(void)
{
    TEMP_T tCold;

    tCold = GetTemp(TEMP_ID_COLD_WATER);
    if( tCold < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

void SetIceMakeMode(U16 mode)
{
    Ice.Mode |= mode;
}

void ClearIceMakeMode(U16 mode)
{
    Ice.Mode &= ~mode;
}

U16 IsSetIceMakeMode(U16 mode )
{
    if( (Ice.Mode & mode ) == mode)
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetIceMakeMode(void )
{
    return Ice.Mode;
}

void SetIceFullCheckTimer(U16 val)
{
    Ice.FullCheckTimer = val;
}

// ���� Ż�� ���� ���� ����/��ȯ
U8 GetIceDummyDeIcing( void )
{
    return Ice.DummyDeIcing;
}

void SetIceDummyDeIcing(U8 val)
{
    Ice.DummyDeIcing = val;
}

// ���� Ż���� ���� �ּ� ����� ���� �ð�
U16 GetDeIcingCompOnTime(void)
{
    return Ice.DeIcingCompTime;
}

void ResetDeIcingCompOnTime(void)
{
     Ice.DeIcingCompTime = MIN_DEICING_TIME;
}

void UpdateDeIcingCompOnTime(void)
{
    if( Ice.DeIcingCompTime != 0 )
    {
        Ice.DeIcingCompTime--;
    }
}

void UpdateIcingCount(void)
{
    if( Ice.Make == FALSE )
    {
        return;
    }

    if( Ice.IcingCount < ICING_MAX_COUNT )
    {
        Ice.IcingCount++;
    }
    else
    {
        Ice.IcingCount = 0U;
    }
}

void ClearIcingCount(void)
{
    Ice.IcingCount = ICING_MAX_COUNT;
}

U8 IsMaxIcingCount(void)
{
    if( Ice.IcingCount == ICING_MAX_COUNT )
    {
        return TRUE;
    }

    return FALSE;
}

/* ���� ��� �ð� �ʰ��ϸ� ��õ� �����ð� */
#define ICE_OUTAGE_TIMER        600U    //10min // @100ms
void StartIceOutageTimer(void)
{
    Ice.OutageTimer = ICE_OUTAGE_TIMER;
}

static void CheckIceOutage(void)
{
    if( Ice.OutageTimer != 0 )
    {
        Ice.OutageTimer--;
    }

    if( GetFlowMeterLPM() > 0 )
    {
        Ice.OutageTimer = 0;
    }
}

void SetIceTrayMovingRetry(void)
{
    SetIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
}

void StopIceMakeLoad(void)
{
    // ���� ������ ����
    ClearIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
    SetIceMakeMode(ICE_MODE_OFF_LOAD);
}

// �̵� Ż�� Ŭ����(void)
void ClearIcingMoveDeicing(void)
{
    ClearIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
}

// ���� Ż�� 
void SetDummyDeicing(void)
{
    // ���� ������ ����
    ClearIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);

    // ���� Ż��
    SetIceMakeMode(ICE_MODE_DUMMY_DEICING);
}

// ���� ����
U8 StopIceMake(void)
{
    if( IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE 
            || IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE
            || IsSetIceMakeMode(ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE
            || IsSetIceMakeMode(ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE )
    {
        // �ʱ�ȭ, ����, �غ� �ܰ��̸� ���� ���� ����ϰ� Ʈ���� Ż�� ��ġ�� �̵�
        ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
        SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_OFF_LOAD );

        return TRUE;
    }
    else if( IsSetIceMakeMode(ICE_MODE_ICING | ICE_MODE_DEICING) == TRUE )
    {
        if( GetDeIcingCompOnTime() == 0 )
        {
            // ���� ���̸�, Ż�� �ܰ�� �̵�
            // ��, ���� �ּ� �ð� ���� ��...
            ClearIceMakeMode(ICE_MODE_ICING);
        }
        else
        {
            // ���� �ð��� ª�� ���, �׳� ���...
            ClearIceMakeMode(ICE_MODE_ICING | ICE_MODE_DEICING);
            SetIceMakeMode(ICE_MODE_INIT | ICE_MODE_OFF_LOAD);
        }

        return TRUE;
    }
    else if( IsSetIceMakeMode(ICE_MODE_ICING_MOVE_DEICING) == TRUE )
    {
        ClearIceMakeMode(ICE_MODE_ICING_MOVE_DEICING);
        SetIceMakeMode(ICE_MODE_INIT | ICE_MODE_OFF_LOAD);
    }

    // Ż�� ���̸�, Ż���� �Ϸ��Ѵ�.
    return FALSE;
}

/* �����˻� */
void  MakeIceFull(void)
{
    static U8 prevIceFull = FALSE;
    U8 iceFull;

    // ���� �߿� ������ �Ǹ�, StopMakeTimer ��ŭ ������ �����Ѵ�.
    iceFull = GetIceFullStatus();
    if( prevIceFull != iceFull )
    {
        prevIceFull = iceFull;
        if( iceFull == TRUE && GetSavingStatus() == TRUE )
        {
            Ice.StopMakeTimer = ICE_STOP_TIME;
        }
    }
    // ��, ������ �����Ǹ� ���� ���� ���¸� �����Ѵ�.
    if( GetSavingStatus() == FALSE )
    {
        Ice.StopMakeTimer = 0;
    }

    if( Ice.StopMakeTimer != 0 )
    {
        Ice.StopMakeTimer--;
    }

    // ���� �˻� ���̸�, ���� �˻� �ֱ� Ÿ�̸� ������Ʈ
    if( IsDoneCheckIceFull() == FALSE )
    {
        SetIceFullCheckTimer(ICE_CHECK_TIME);
    }

    // ���� �˻� Ÿ�̸� ����...
    if( Ice.FullCheckTimer != 0 )
    {
        Ice.FullCheckTimer--;
        return ;
    }

    // ���� �˻� ����
    // �׸��� Ÿ�̸� reload
    StartCheckIceFull();
    StartCheckIceLow();
}

static U8 IsDetctTankLevel(void)
{
    U8 roomLevel;
    U8 coldlevel;

    roomLevel = GetRoomWaterLevel();

    if( roomLevel == LEVEL_LOW 
        || roomLevel == LEVEL_ERR_LOW
        || roomLevel == LEVEL_ERR_COMPLEX
        || roomLevel == LEVEL_ERR_OVF_LOW )
    {
        return FALSE;
    }

    /**
     * �ü� ������ �̰����̸� ���� ����
     * �ü��� �����ϸ鼭 ����Ǵ� ��
     * �ü��� ���� ���� �� Ʈ���� �Լ� �� ��ȯ������ ��� ���۵�
    */
    coldlevel = GetColdWaterLevel();

    if( coldlevel == LEVEL_LOW )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 IsValidMake(void)
{
    // ������ ���� ����
    if( Ice.ConfigMake == FALSE )
    {
        return FALSE ;
    }

    /* �ܱ�, ���� �µ� ���� ���� */
    if( IsValidDoIcingColdTempRange() == FALSE 
            || IsValidDoIcingAmbiTempRange() == FALSE )
    {
        return FALSE;
    }

//    /* ���� ��ȯ ��� */
//    if( IsGoingOnDrainWaterFull() == TRUE )
//    {
//        return FALSE;
//    }

    /* ����� ���� �ð� */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    // ����� ��ȣ �ð� �ʰ� 
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }

    // ���� ���� �˻簡 �Ϸ���� ���� ���, ���� �������� �ʴ´�.
    if( GetIceFullPowerOnCheck() == FALSE )
    {
        return  FALSE;
    }

    // ����, �ü� ���� ������ ���� �˻�
    if( GetRoomWaterInitFull() == FALSE 
            || GetColdWaterInitFull() == FALSE )
    {
        return FALSE;
    }

    // ���� / �ü� ������ ����
    if( IsDetctTankLevel() == FALSE )
    {
        return FALSE;
    }

    // ���� Ÿ�̸� ( ���� �� ������ �� ��� )
    if( Ice.StopMakeTimer != 0 )
    {
        return FALSE;
    }

    // ��ũ ž Ŀ�� OPEN ��, ���� ����
    if( GetServiceCheckTankOpen() == TRUE )
    {
        return FALSE;
    }

    // Tray Braket �̰��� �� , ���� ����
    if( GetCurrentInputVal(INPUT_ID_TRAY_BRACKET) == TRUE )
    {
        return FALSE;
    }
    

    // ������ ���� ����
    if( IsErrorType(ERROR_TYPE_ICE_MAKE) == TRUE )
    {
        return FALSE;   // ���� ����~
    }

    // �ܼ��� ���� ���...
    if( Ice.OutageTimer != 0 )
    {
        return FALSE;
    }

    // HEALTH ���� �� ( ���/��� )
    if( IsStartHealth() == TRUE )
    {
        return FALSE;
    }

    // �ü� û�� ���
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    return TRUE;  // ���� go~ go~ 
}

void MakeIce(void)
{
    U8 make = FALSE;
    static U8 prevMake = FALSE;

    CheckIceOutage();
  
    if( GetIceFullStatus() == FALSE )
    {
        make = TRUE;
    }
    else
    {
        make = FALSE;
    }

    if( IsValidMake() == FALSE )
    {
        make = FALSE;
    }

    if( prevMake != make )
    {
        prevMake = make;
    
        if( make == TRUE )
        {
            SetIceFullCheckCount();
        }
        else
        {
            ClearIcingCount();
        }
    }

    // Set new make status
    SetIceMake(make);
}
