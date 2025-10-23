#include "err_over_hot_water.h"
#include "hot_water.h"
#include "relay.h"
#include "valve.h"

#define HEATER_ON_3HOURS    ( 3UL * 60UL * 600UL )  /* @100msec, 3hour x 60min x 60sec */

// �¼� ���� ���� �߻� ���� ī��Ʈ
U32 gu32HeaterOnTime = HEATER_ON_3HOURS;    // @100msec
U8 CheckErrOverHotWater(U8 mu8Error)
{
    // �¼� �̻���̸�, ���� ī��Ʈ �ʱ�ȭ
    if( GetHotWaterConfigMake() == FALSE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
        return FALSE; // no error
    }


    // ���Ͱ� OFF�̸�, ���� ī���� �ʱ�ȭ
    if( IsTurnOnRelay( RELAY_HEATER ) == FALSE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
    }

    // �¼� �����̸�, ���� ī��Ʈ �ʱ�ȭ
    if( IsOpenValve( VALVE_HOT_OUT ) == TRUE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
    }


    // ���� ī��Ʈ�� 0�̸�, ���� �߻�
    gu32HeaterOnTime--;
    if( gu32HeaterOnTime == 0UL )
    {
        return TRUE;    // error
    }

    return FALSE;   // no error
}
