#include "ice_door.h"
#include "api_step_motor.h"
#include "relay.h"
#include "ice_full.h"
#include "ice_low.h"
#include "ice.h"
#include "ice_water_out.h"
#include "valve.h"
#if( CONFIG_USE_CONTROL_ICE_FEEDER == 1 )
#include "ice_feeder.h"
#endif

#define  ICE_DOOR_ID       SM_ID_1
#define  ICE_DOOR_SPEED    SM_SPEED_2

#define  ICE_DOOR_OPEN_STEP_VAL    (-400)
#define  ICE_DOOR_CLOSE_STEP_VAL   (400)

/* MODE */
#define  ICE_DOOR_MODE_NONE             0x00
#define  ICE_DOOR_MODE_INIT             0x01
#define  ICE_DOOR_MODE_CLOSE            0x02
#define  ICE_DOOR_MODE_OPEN             0x04
#define  ICE_DOOR_MODE_WATER_OUT_OPEN   0x08
#define  ICE_DOOR_MODE_WATER_OUT_CLOSE  0x10

//#define  ICE_DOOR_MODE_TEST    0x08


typedef struct _ice_door_
{
    /* STATUS */
    U8 Status;       // Open or Close
    U8 WaterStatus;  // Start or Stop
    
    /* MODE */
    U8 Mode;         // Init / Close / Open  / Water out open / Water out close
    U8 InitStep;
    U8 OpenStep;
    U8 CloseStep;
    //U8 TestStep;

    /* Forced Close Door Timer */
    U32 CloseTime;

    U16 OpeningTimeIot;     /* @Sec */
    U16 OpeningTime;        /* @Sec */
} IceDoor_T;

IceDoor_T  Door;

void InitIceDoor(void)
{
    Door.Status      = ICE_DOOR_CLOSE;
    Door.WaterStatus = ICE_DOOR_WATER_STOP;

    Door.Mode        = ICE_DOOR_MODE_INIT;
    Door.InitStep    = 0;
    Door.OpenStep    = 0;
    Door.CloseStep   = 0;
    //Door.TestStep    = 0;

    Door.CloseTime   = ICE_CLOSE_TIME;
    Door.OpeningTimeIot = 0;
    Door.OpeningTime    = 0;

    InitStepVal( ICE_DOOR_ID );
    SetStepSpeed( ICE_DOOR_ID, ICE_DOOR_SPEED );
}

void ClearIceDoorOpeningTime(void)
{
    Door.OpeningTime = 0;
}

U16 GetIceDoorOpeningTime(void)
{
    return Door.OpeningTime;
}

void SetIceDoorOpeningTime(U16 mu16Time)
{
    Door.OpeningTime = mu16Time;
}

U16 GetIceDoorOpeningTimeIot(void)
{
    return Door.OpeningTimeIot;
}

void SetIceDoorOpeningTimeIot(U16 mu16Time)
{
    Door.OpeningTimeIot = mu16Time;
}

void CountingIceDoorOpeningTime(void)
{
    if( IsOpenIceDoor() == TRUE )
    {
        if( Door.OpeningTime< 65535U )
        {
            Door.OpeningTime++;
        }

        if( Door.OpeningTimeIot< 65535U )
        {
            Door.OpeningTimeIot++;
        }
    }
}

void SetIceDoorConfClosedTime(U32 mu16Time)
{
    Door.CloseTime   = mu16Time;
}

U32 GetIceDoorConfClosedTime(void)
{
    return Door.CloseTime;
}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );

U8 GetIceDoorStatus(void)
{
    return Door.Status;
}

U8 GetIceDoorWaterStatus(void)
{
    return Door.Status;
}

// ���̽� ���� ���� ����
// RETURN
// TRUE - ���� ��
// FALSE - ���� ����
U8 IsOpenIceDoor(void)
{
    if( Door.Status == ICE_DOOR_OPEN 
            || Door.Status == ICE_DOOR_OPENING )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsStartIceDoorWater(void)
{
    if( Door.WaterStatus == ICE_DOOR_WATER_START 
            || Door.WaterStatus == ICE_DOOR_WATER_STATING )
    {
        return TRUE;
    }

    return FALSE;
}

static void SetIceDoorStatus(U8 mu8Status)
{
    Door.Status = mu8Status;
    if( mu8Status == ICE_DOOR_CLOSE )
    {
        Door.CloseTime = ICE_CLOSE_TIME;
    }
}

static void SetIceDoorWaterStatus(U8 mu8Status)
{
    Door.WaterStatus = mu8Status;
}

void OpenIceDoor(void)
{
    SetMode( ICE_DOOR_MODE_OPEN );
}

void CloseIceDoor(void)
{
    if( (Door.Mode & ICE_DOOR_MODE_WATER_OUT_CLOSE)  == ICE_DOOR_MODE_WATER_OUT_CLOSE )
    {
        return;
    }

    SetMode( ICE_DOOR_MODE_CLOSE );
}

void OpenIceDoorWaterOut(void)
{
    SetMode( ICE_DOOR_MODE_WATER_OUT_OPEN );
}

void CloseIceDoorWaterOut(void)
{
    ClearMode(ICE_DOOR_MODE_CLOSE);
    SetMode( ICE_DOOR_MODE_WATER_OUT_CLOSE );
}


#if 0
void TestStartIceDoor(void)
{
    SetMode( ICE_DOOR_MODE_TEST );
}

void TestStopIceDoor(void)
{
    ClearMode( ICE_DOOR_MODE_TEST );
    SetMode( ICE_DOOR_MODE_CLOSE );
}
#endif

// ���� CLOSE
void ForcedCloseIceDoor(void)
{
    // ���̽� ����� ���� ���� ������, �� ���� �ݾ��ش�.
    // ���̽� ��� open�Ǿ� �ִٴ� ���� ���� ����ڰ� ��� ���� �����̰�,
    // ���� �ݱ� ������ ����ڰ� ��� ���� ������ �����ؼ��� �ȵȴ�.
    if( IsOpenIceDoor() == FALSE )
    {
        SetMode( ICE_DOOR_MODE_INIT );
    }
}

static void SetMode(U8 mu8Mode )
{
    Door.Mode |= mu8Mode;
    if( mu8Mode == ICE_DOOR_MODE_OPEN )
    {
        ClearMode( ICE_DOOR_MODE_CLOSE );
        ClearMode( ICE_DOOR_MODE_WATER_OUT_CLOSE );
    }
    else if( mu8Mode == ICE_DOOR_MODE_CLOSE )
    {
        ClearMode( ICE_DOOR_MODE_OPEN );
        ClearMode( ICE_DOOR_MODE_WATER_OUT_OPEN );
    }
    else if( mu8Mode == ICE_DOOR_MODE_WATER_OUT_OPEN )
    {
        ClearMode( ICE_DOOR_MODE_CLOSE );
        ClearMode( ICE_DOOR_MODE_WATER_OUT_CLOSE );
    }
    else if( mu8Mode == ICE_DOOR_MODE_WATER_OUT_CLOSE )
    {
        ClearMode( ICE_DOOR_MODE_OPEN );
        ClearMode( ICE_DOOR_MODE_WATER_OUT_OPEN );
    }
    //else if( mu8Mode == ICE_DOOR_MODE_TEST )
    //{
    //    ClearMode( ICE_DOOR_MODE_OPEN );
    //    ClearMode( ICE_DOOR_MODE_CLOSE );
    //}
}

static void ClearMode(U8 mu8Mode )
{
    Door.Mode &= ~mu8Mode;

    if( mu8Mode == ICE_DOOR_MODE_INIT )
    {
        Door.InitStep = 0;
    }
    else if( (mu8Mode == ICE_DOOR_MODE_OPEN)
          || (mu8Mode == ICE_DOOR_MODE_WATER_OUT_OPEN) )
    {
        Door.CloseStep = 0;
    }
    else if( (mu8Mode == ICE_DOOR_MODE_CLOSE)
          || (mu8Mode == ICE_DOOR_MODE_WATER_OUT_CLOSE) )
    {
        Door.OpenStep = 0;
    }
    //else if( mu8Mode == ICE_DOOR_MODE_TEST )
    //{
    //    Door.TestStep = 0;
    //}
}


// 24�ð� ���� ���� �ݱ� ����
void ControlForcedCloseIceDoor(void)
{
    // ���� OFF�̸� �������� ����.
    if( GetIceConfigMake() == FALSE )
    {
        Door.CloseTime = ICE_CLOSE_TIME;
        return ;
    }


    if( Door.CloseTime > 0 )
    {
        Door.CloseTime--;
        return ;
    }

    ForcedCloseIceDoor();
    Door.CloseTime = ICE_CLOSE_TIME;
}

static void HandleDoorOpening(U8 mu8Mode, U8 (*open_func)(void))
{
    U8 mu8Ret = open_func();

    if (mu8Ret == TRUE) // ���� ���� �Ϸ� �Ǹ�
    {
        ClearMode(mu8Mode);
        // ���� ���¸� '����'���� ����
        SetIceDoorStatus(ICE_DOOR_OPEN);
    }
    else // ���� ���� ������ ���� ���� ���̸�
    {
        // ���� ���¸� '������ ��'���� ����
        SetIceDoorStatus(ICE_DOOR_OPENING);
    }
}

static void HandleDoorWaterOpening(U8 mu8Mode, U8 (*open_func)(void))
{
    U8 mu8Ret = open_func();

    if (mu8Ret == TRUE) // ���� ���� �Ϸ� �Ǹ�
    {
        ClearMode(mu8Mode);
        // ���� ���¸� '����'���� ����
        SetIceDoorStatus(ICE_DOOR_OPEN);
        // ������ ���¸� '����'���� ����
        SetIceDoorWaterStatus(ICE_DOOR_WATER_START);
    }
    else // ���� ���� ������ ���� ���� ���̸�
    {
        // ���� ���¸� '������ ��'���� ����
        SetIceDoorStatus(ICE_DOOR_OPENING);
        // ������ ���¸� '������'���� ����
        SetIceDoorWaterStatus(ICE_DOOR_WATER_STATING);
    }
}

static void HandleDoorClosing(U8 mu8Mode, U8 (*close_func)(void))
{
    U8 mu8Ret = close_func();

    if (mu8Ret == TRUE) // ���� ���� �Ϸ�Ǹ�
    {
        ClearMode(mu8Mode);
        // ���� ���¸� '����'���� ����
        SetIceDoorStatus(ICE_DOOR_CLOSE);

        StartCheckIceFull();
        StartCheckIceLow();
    }
    else // ���� ���� ������ ���� ���� ���̸�
    {
        // ���� ���¸� '������ ��'���� ����
        SetIceDoorStatus(ICE_DOOR_CLOSING);
    }
}

static void HandleDoorWaterClosing(U8 mu8Mode, U8 (*close_func)(void))
{
    U8 mu8Ret = close_func();

    if (mu8Ret == TRUE) // ���� ���� �Ϸ�Ǹ�
    {
        ClearMode(mu8Mode);
        // ���� ���¸� '����'���� ����
        SetIceDoorStatus(ICE_DOOR_CLOSE);
        // ������ ���¸� 'STOP'���� ����
        SetIceDoorWaterStatus(ICE_DOOR_WATER_STOP);

        StartCheckIceFull();
        StartCheckIceLow();
    }
    else // ���� ���� ������ ���� ���� ���̸�
    {
        // ���� ���¸� '������ ��'���� ����
        SetIceDoorStatus(ICE_DOOR_CLOSING);
        // ������ ���¸� 'STOPING'���� ����
        SetIceDoorWaterStatus(ICE_DOOR_WATER_STOPING);
    }
}

static void ClearMode(U8 mu8Mode );
static U8 InitDoor(void);
static U8 OpenDoor(void);
static U8 CloseDoor(void);
static U8 OpenDoorWaterOut(void);
static U8 CloseDoorWaterOut(void);
#if 0
static U8 TestDoor(void);
#endif
#if 0
void ControlIceDoor(void)
{
    U8 mu8Ret = FALSE;

#if 0
    /* TEST */
    if( (Door.Mode & ICE_DOOR_MODE_TEST ) == ICE_DOOR_MODE_TEST )
    {
        mu8Ret = TestDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_TEST );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }
#endif

    /* Init */
    if( (Door.Mode & ICE_DOOR_MODE_INIT ) == ICE_DOOR_MODE_INIT )
    {
        mu8Ret = InitDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_INIT );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }

    /* Open - ICE */
    if( (Door.Mode & ICE_DOOR_MODE_OPEN ) == ICE_DOOR_MODE_OPEN )
    {
        mu8Ret = OpenDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_OPEN );
            SetIceDoorStatus( ICE_DOOR_OPEN );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_OPENING );
        }
        return ;
    }

    /* Close - ICE */
    if( (Door.Mode & ICE_DOOR_MODE_CLOSE ) == ICE_DOOR_MODE_CLOSE )
    {
        mu8Ret = CloseDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_CLOSE );
            SetIceDoorStatus( ICE_DOOR_CLOSE );

            StartCheckIceFull();
            StartCheckIceLow();
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }

    /* Open - ICE + WATER OUT */
    if( (Door.Mode & ICE_DOOR_MODE_WATER_OUT_OPEN ) == ICE_DOOR_MODE_WATER_OUT_OPEN )
    {
        mu8Ret = OpenDoorWaterOut();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_WATER_OUT_OPEN );
            SetIceDoorStatus( ICE_DOOR_OPEN );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_OPENING );
        }
        return ;
    }

    /* Close - ICE + WATER OUT */
    if( (Door.Mode & ICE_DOOR_MODE_WATER_OUT_CLOSE ) == ICE_DOOR_MODE_WATER_OUT_CLOSE )
    {
        mu8Ret = CloseDoorWaterOut();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_WATER_OUT_CLOSE );
            SetIceDoorStatus( ICE_DOOR_CLOSE );

            StartCheckIceFull();
            StartCheckIceLow();
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }    
}
#endif

void ControlIceDoor(void)
{
    U8 mu8Ret = FALSE;

#if 0
    /* TEST */
    if( (Door.Mode & ICE_DOOR_MODE_TEST ) == ICE_DOOR_MODE_TEST )
    {
        mu8Ret = TestDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_TEST );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }
#endif

    /* Init */
    if( (Door.Mode & ICE_DOOR_MODE_INIT ) == ICE_DOOR_MODE_INIT )
    {
        mu8Ret = InitDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_INIT );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }

    /* Open - ICE */
    if( (Door.Mode & ICE_DOOR_MODE_OPEN ) == ICE_DOOR_MODE_OPEN )
    {
        HandleDoorOpening(ICE_DOOR_MODE_OPEN, OpenDoor);
        return ;
    }

    /* Close - ICE */
    if ((Door.Mode & ICE_DOOR_MODE_CLOSE) == ICE_DOOR_MODE_CLOSE)
    {
        HandleDoorClosing(ICE_DOOR_MODE_CLOSE, CloseDoor);
        return;
    }

    /* Open - ICE + WATER OUT */
    if ((Door.Mode & ICE_DOOR_MODE_WATER_OUT_OPEN) == ICE_DOOR_MODE_WATER_OUT_OPEN)
    {
        HandleDoorWaterOpening(ICE_DOOR_MODE_WATER_OUT_OPEN, OpenDoorWaterOut);
        return;
    }
    
    /* Close - ICE + WATER OUT */
    if ((Door.Mode & ICE_DOOR_MODE_WATER_OUT_CLOSE) == ICE_DOOR_MODE_WATER_OUT_CLOSE)
    {
        // ICE_DOOR_MODE_WATER_OUT_CLOSE ���� CloseDoorWaterOut �Լ��� ���ڷ� �����Ͽ� ���� �Լ� ȣ��
        HandleDoorWaterClosing(ICE_DOOR_MODE_WATER_OUT_CLOSE, CloseDoorWaterOut);
        return;
    }
}

/*
 * ���̽� ���� ���� ���� ���� ( close )
 */
static void CloseStepMotor(void)
{
    // ���� ���� ���� ��ġ �ʱ�ȭ�� �ϰ� ICE_DOOR_CLOSE_STEP_VAL �̵��Ѵ�.
    SetTargetStep( ICE_DOOR_ID, ICE_DOOR_CLOSE_STEP_VAL );
}

/* ���̽� ���� ���� ���� ���� ( open )
 */
static void OpenStepMotor(void)
{
    // ���� ���� ���� ��ġ �ʱ�ȭ�� �ϰ� ICE_DOOR_OPEN_STEP_VAL �̵��Ѵ�.
    SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );
}

// ���� �ʱ�ȭ ����
// ������ �ݰ� ���� �ʱ�ȭ
static U8 InitDoor(void)
{
    switch( Door.InitStep )
    {
        case 0:
            InitStepVal( ICE_DOOR_ID ); // ��ġ �ʱ�ȭ
            SetTargetStep( ICE_DOOR_ID, ICE_DOOR_CLOSE_STEP_VAL );
            Door.InitStep++;
            break;

        case 1:
            if( IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                InitStepVal( ICE_DOOR_ID ); // ��ġ �ʱ�ȭ
                Door.InitStep++;
            }
            break;

        case 2:
            return TRUE;

    }

    return FALSE;
}

// ���� OPEN ����
static U8 OpenDoor(void)
{
    static U16 delayTime = 0;   /* @100ms */

    switch( Door.OpenStep )
    {
        case 0:
            ClearIceDoorOpeningTime();
            
            delayTime = 5;  // 0.5sec..-> 0.3sec -> 0.5sec

            StopMove( ICE_DOOR_ID );
            SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );

            if( Door.Status == ICE_DOOR_CLOSING )
            {
                delayTime = 20;
                TurnOffRelay(RELAY_FEEDER_CW);
                TurnOnRelay(RELAY_FEEDER_CCW);
                Door.OpenStep++;
            }
            else
            {
                Door.OpenStep = 2;
            }
            break;

        case 1:
            if( delayTime == 0 )
            {
                TurnOffRelay(RELAY_FEEDER_CW);
                TurnOffRelay(RELAY_FEEDER_CCW);
                delayTime = 5;  // 0.5sec..-> 0.3sec -> 0.5sec
                Door.OpenStep++;
            }
            else
            {
                delayTime--;
            }
            break;

        case 2:
            if( IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.OpenStep++;
            }
            break;

        case 3:
            if( delayTime == 0 )
            {
                Door.OpenStep++;
            }
            else
            {
                delayTime--;
            }
            break;

        case 4:
#if( CONFIG_USE_CONTROL_ICE_FEEDER == 1 )
            StartIceFeeder();
#else
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
#endif
            Door.OpenStep++;

            break;

        case 5:
            return TRUE;

    }

    return FALSE;

}

// ���� CLOSE ����
static U8 CloseDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Door.CloseStep )
    {
        case 0:
            mu16DelayTime = 10; 
#if( CONFIG_USE_CONTROL_ICE_FEEDER == 1 )
            StopIceFeeder();
#endif
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            StopMove( ICE_DOOR_ID );

            Door.CloseStep++;
            break;

        case 1:
            if( mu16DelayTime == 0 )
            {
                Door.CloseStep++;
            }
            else
            {
                mu16DelayTime--;
            }
            break;

        case 2:
            StopMove( ICE_DOOR_ID );
            SetTargetStep( ICE_DOOR_ID, 0 );

            if( GetIceDoorOpeningTime() >= 3U )  // 3sec @1sec
            {   // ���� ���� 3�� �̻��� �� ��ȸ�� �ǽ�
                mu16DelayTime = 20;
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOnRelay( RELAY_FEEDER_CCW );
            }
            
            Door.CloseStep++;
            break;

        case 3:
            if( mu16DelayTime == 0U )
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
            }
  
            if( (mu16DelayTime == 0U)
                 && (IsDoneMoveStep( ICE_DOOR_ID ) == TRUE) )
            {
                Door.CloseStep++;
            }
        
            if( mu16DelayTime != 0U )
            {
                mu16DelayTime--;
            }
            break;

        case 4:
            return TRUE;

    }

    return FALSE;
}

// ���� OPEN + Water Out Open ����
static U8 OpenDoorWaterOut(void)
{
    static U16 delayTime = 0;   /* @100ms */


    switch( Door.OpenStep )
    {
        case 0:
            ClearIceDoorOpeningTime();
        
            delayTime = 5;  // 0.5sec..-> 0.3sec -> 0.5sec
            StopMove( ICE_DOOR_ID );
            SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );

            if( Door.Status == ICE_DOOR_CLOSING )
            {
                delayTime = 20;
                TurnOffRelay(RELAY_FEEDER_CW);
                TurnOnRelay(RELAY_FEEDER_CCW);
                Door.OpenStep++;
            }
            else
            {
                Door.OpenStep = 2;
            }
            break;

        case 1:
            if( delayTime == 0 )
            {
                TurnOffRelay(RELAY_FEEDER_CW);
                TurnOffRelay(RELAY_FEEDER_CCW);
                delayTime = 5;  // 0.5sec..-> 0.3sec -> 0.5sec
                Door.OpenStep++;
            }
            else
            {
                delayTime--;
            }
            break;

        case 2:
            if( IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.OpenStep++;
            }
            break;

        case 3:
            if( delayTime == 0 )
            {
                Door.OpenStep++;
            }
            else
            {
                delayTime--;
            }
            break;

        case 4:
#if( CONFIG_USE_CONTROL_ICE_FEEDER == 1 )
            StartIceFeeder();
#else
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
#endif
            CloseValve(VALVE_NOS_FLOW);
            RegisterOutageIceWaterOut();
            StartIceOut();       // Start ���� ���� 
            StartIceWaterOut();  // Start ������(�ü�) ���� 
            Door.OpenStep++;
            break;

        case 5:
            return TRUE;

    }

    return FALSE;

}

// ���� CLOSE + Water Out Close ����
static U8 CloseDoorWaterOut(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Door.CloseStep )
    {
        case 0:
            mu16DelayTime = 10; 
#if( CONFIG_USE_CONTROL_ICE_FEEDER == 1 )
            StopIceFeeder();
#endif            
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            StopMove( ICE_DOOR_ID );
            OpenValve(VALVE_NOS_FLOW);
            UnRegisterOutageIceWaterOut();
            StopIceOut();       // Stop ���� ����
            StopIceWaterOut();  // Stop ������(�ü�) ����
            Door.CloseStep++;
            break;

        case 1:
            if( mu16DelayTime == 0 )
            {
                Door.CloseStep++;
            }
            else
            {
                mu16DelayTime--;
            }
            break;

        case 2:
            StopMove( ICE_DOOR_ID );
            SetTargetStep( ICE_DOOR_ID, 0 );

            if( GetIceDoorOpeningTime() >= 3U )  // 3sec @1sec
            {   // ���� ���� 3�� �̻��� �� ��ȸ�� �ǽ�
                mu16DelayTime = 20;
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOnRelay( RELAY_FEEDER_CCW );
            }
            
            Door.CloseStep++;
            break;

        case 3:
            if( mu16DelayTime == 0U )
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
            }
        
            if( (mu16DelayTime == 0U)
                 && (IsDoneMoveStep( ICE_DOOR_ID ) == TRUE) )
            {
                Door.CloseStep++;
            }

            if( mu16DelayTime != 0U )
            {
                mu16DelayTime--;
            }
            break;

        case 4:
            return TRUE;

    }

    return FALSE;
}

#if 0
static U8 TestDoor(void)
{
    switch( Door.TestStep )
    {
        case 0:
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            HAL_StopMove( ICE_DOOR_ID );

            Door.TestStep++;
            break;

        case 1:
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.TestStep++;
            break;

        case 2:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );
                Door.TestStep++;
            }
            break;
            
        case 3:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                HAL_SetTargetStep( ICE_DOOR_ID, 0 );
                Door.TestStep++;
            }
            break;

            // ���� �ܰ�.
        case 4:
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.TestStep++;
            break;

        case 5:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.TestStep++;
            }
            break;

        case 6:
            return TRUE;

    }

    return FALSE;
}
#endif



/*************************************************************************************************************************
 * TEST
 */
typedef struct _ice_door_repeat_test_
{
    U8 Run;
    U8 Delay;
    U32 RepeatCount;
} IceDoorTest_T;

IceDoorTest_T DoorTest;
void TEST_StartIceDoor(void)
{
    DoorTest.Run         = TRUE;
    DoorTest.Delay       = 0;
    DoorTest.RepeatCount = 0;
}

void TEST_StopIceDoor(void)
{
    DoorTest.Run         = FALSE;
}

#define TEST_DELAY  (5)
void TEST_RepeatIceDoor(void)
{
    static U8 mu8Toggle = 0;

    if( DoorTest.Run == FALSE )
    {
        Door.Mode = ICE_DOOR_MODE_NONE;
        return ;
    }

    if( mu8Toggle != 0 )
    {
        if( Door.Mode != ICE_DOOR_MODE_NONE )
        {
            DoorTest.Delay = TEST_DELAY;
            return;
        }

        if( DoorTest.Delay != 0 )
        {
            DoorTest.Delay--;
            return ;
        }
        
        OpenIceDoor();
        mu8Toggle = 0;
    }
    else
    {
        if( Door.Mode != ICE_DOOR_MODE_NONE )
        {
            DoorTest.Delay = TEST_DELAY;
            return;
        }

        if( DoorTest.Delay != 0 )
        {
            DoorTest.Delay--;
            return ;
        }
        
        CloseIceDoor();
        mu8Toggle = 1;
    }
}
