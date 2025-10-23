#include "ice_tray_test.h"
#include "ice_tray.h"
#include "room_water.h"
#include "cold_water.h"
#include "service.h"
#include "ice.h"
#include "ice_maker.h"

typedef enum
{
    STEP_MOVE_1,
    STEP_WAIT_1,

    STEP_MOVE_2,
    STEP_WAIT_2,

    STEP_MOVE_3,
    STEP_WAIT_3,

    STEP_TEST_ICING_DONE,
    STEP_TEST_DEICING_DONE

} TrayTestStep_T;

// TEST POSITION
#define POSITION_ICING    0
#define POSITION_DEICING  1

#define DEFAULT_WAIT    30      // 3sec.. @100ms..
typedef struct _ice_tray_test_
{
    U8 Status;              /* 0: STOP, 1: RUN */
    TrayTestStep_T Step;    /* CONTROL STEP */

    U8 Position;            
    U16 Wait;               // @100ms

    U8 CheckIcing;          /* FALSE: NONE-CHECK, TRUE:CHECK */
    U8 CheckDeIcing;        /* FALSE: NONE-CHECK, TRUE:CHECK */
} IceTrayTest_T;

IceTrayTest_T  TrayTest;

void InitIceTrayTest(void)
{
    TrayTest.Status  = FALSE;
    TrayTest.Step    = 0;
    TrayTest.Position = POSITION_ICING;

    TrayTest.CheckIcing = FALSE;
    TrayTest.CheckDeIcing = FALSE;
}

/* Ʈ���� �׽�Ʈ */
void StartIceTrayTest(void)
{
    TrayTest.Status  = TRUE;
    TrayTest.Step    = 0;
    TrayTest.CheckIcing     = FALSE;
    TrayTest.CheckDeIcing   = FALSE;
}

/* Ʈ���� �׽�Ʈ ���� */
void StopIceTrayTest(void)
{
    if( TrayTest.Position == POSITION_DEICING )
    {
        if( TrayTest.Step <= STEP_MOVE_2 )
        {
            TrayTest.Step = STEP_TEST_ICING_DONE;
        }
    }
    else
    {
        if( TrayTest.Step < STEP_MOVE_2 )
        {
            TrayTest.Step = STEP_TEST_DEICING_DONE;
        }
        else if( TrayTest.Step < STEP_MOVE_3 )
        {
            TrayTest.Step = STEP_TEST_ICING_DONE;
        }
    }
}

U8 GetIceTrayTestStatus(void)
{
    return TrayTest.Status;
}

U8 GetIceTrayTestCheckIcing(void)
{
    return TrayTest.CheckIcing;
}

U8 GetIceTrayTestCheckDeIcing(void)
{
    return TrayTest.CheckDeIcing;
}

U8 IsIceTrayTest(void)
{
#if( CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST == 1 )
    if( TrayTest.Status == FALSE )
#else
    if( GetRoomWaterLevel() == LEVEL_LOW 
            && GetColdWaterLevel() == LEVEL_LOW 
            && GetServiceCheckTankOpen() == TRUE 
            && TrayTest.Status == FALSE )
#endif
    {
        return TRUE; 
    }

    return FALSE;
}

static void CheckPosition(void)
{
    if( GetIceTrayStatus() == ICE_TRAY_DEICING )
    {
        TrayTest.Position = POSITION_DEICING;    // deicing..
    }
    else
    {
        TrayTest.Position = POSITION_ICING;    // icing...
    }
}

static void TestIcing(void)
{
    switch( TrayTest.Step )
    {
        // Go deicing..
        case STEP_MOVE_1:
            MoveIceTrayDeIcing();

            TrayTest.Wait = DEFAULT_WAIT;
            TrayTest.Step++;
            break;

        case STEP_WAIT_1:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                if( TrayTest.Wait != 0 )
                {
                    TrayTest.Wait--;
                }
                else
                {
                    TrayTest.Step++;
                }
            }
            break;


            // Go icing
        case STEP_MOVE_2:
            MoveIceTrayIcing();

            TrayTest.Wait = DEFAULT_WAIT;
            TrayTest.Step++;
            break;

        case STEP_WAIT_2:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                TrayTest.CheckIcing = TRUE;
                if( TrayTest.Wait != 0 )
                {
                    TrayTest.Wait--;
                }
                else
                {
                    TrayTest.Step++;
                }
            }
            break;


            // Go deicing...
        case STEP_MOVE_3:
            MoveIceTrayDeIcing();

            TrayTest.Wait = DEFAULT_WAIT;
            TrayTest.Step++;
            break;

        case STEP_WAIT_3:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TrayTest.CheckDeIcing = TRUE;
                if( TrayTest.Wait != 0 )
                {
                    TrayTest.Wait--;
                }
                else
                {
                    TrayTest.Step = STEP_TEST_DEICING_DONE;
                }
            }
            break;


            // ���� ��ġ���� ����
        case STEP_TEST_ICING_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                TrayTest.Step = 0;
                TrayTest.Status = FALSE;
                TrayTest.Position = POSITION_ICING;
            }
            break;

            // Ż��  ��ġ���� ����
        case STEP_TEST_DEICING_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TrayTest.Step = 0;
                TrayTest.Status = FALSE;
                TrayTest.Position = POSITION_DEICING;
            }
            break;

        default:
            TrayTest.Step = 0;
            TrayTest.Status = FALSE;
            TrayTest.Position = POSITION_DEICING;
            break;
    }
}

// Ʈ���� �׽�Ʈ - ���� ��ġ�� ������ ���,
// ���� -> Ż�� 1ȸ �̵�
static void TestDeIcing(void)
{
    switch( TrayTest.Step )
    {
        // go icing..
        case STEP_MOVE_1:
            MoveIceTrayIcing();

            TrayTest.Wait = DEFAULT_WAIT;
            TrayTest.Step++;
            break;

        case STEP_WAIT_1:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                TrayTest.CheckIcing = TRUE;
                if( TrayTest.Wait != 0 )
                {
                    TrayTest.Wait--;
                }
                else
                {
                    TrayTest.Step++;
                }
            }
            break;

            // go deicing..
        case STEP_MOVE_2:
            MoveIceTrayDeIcing();

            TrayTest.Wait = DEFAULT_WAIT;
            TrayTest.Step++;
            break;

        case STEP_WAIT_2:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TrayTest.CheckDeIcing = TRUE;
                if( TrayTest.Wait != 0 )
                {
                    TrayTest.Wait--;
                }
                else
                {
                    TrayTest.Step = STEP_TEST_DEICING_DONE;
                }
            }
            break;

            // ���� ��ġ���� ����
        case STEP_TEST_ICING_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                TrayTest.Step = 0;
                TrayTest.Status = FALSE;
                TrayTest.Position = POSITION_ICING;
            }
            break;

            // Ż��  ��ġ���� ����
        case STEP_TEST_DEICING_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TrayTest.Step = 0;
                TrayTest.Status = FALSE;
                TrayTest.Position = POSITION_DEICING;
            }
            break;

        default:
            TrayTest.Step = 0;
            TrayTest.Status = FALSE;
            TrayTest.Position = POSITION_ICING;
            break;
    }
}

void ProcessIceTrayTest(void)
{
   if( TrayTest.Status == FALSE )
    {
        // �׽�Ʈ Position�� �˻��Ѥ�.
        CheckPosition();
        return;
    }

    // ����Ż�� ���̸�, ���
    // ���� ���̸� ���� ���
#if 0
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE )
    {
        StopDummyDeIcing();
    }
    else 
    {
        StopIceMake();
    }
#endif

    // Ʈ���� �׽�Ʈ
    if( TrayTest.Position == POSITION_ICING )
    {
        // ���� ��ġ�� ���� ��ġ�� ���
        TestIcing();
    }
    else
    {
        // ���� ��ġ�� Ż�� ��ġ�� ���
        TestDeIcing();
    }
}

