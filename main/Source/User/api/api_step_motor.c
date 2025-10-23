/**
 * File : api_step_motor.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "hw.h"

#include "hal_timer.h"

#include "api_step_motor.h"

#define SM_SPEED_1_COUNT  (29)  /// @1ms * (29 +1) = 30ms
#define SM_SPEED_2_COUNT  (2)   /// @1ms * (2 +1) = 3ms

#define	DEFAULT_HOLD_STEP	 (20)	 /// about 0.5 sec

typedef struct _sm_
{
    sm_step_t	target;
    sm_step_t	current;

    /* SPEED */
    sm_speed_t	speed;
    U8	speed_cnt;

    /* DLETA STEP */
    sm_step_t	move_step;

    /* HOLD STEP */
    sm_step_t	hold_start_step;
    sm_step_t	hold_stop_step;

    /* Step Motor Phase  */
    I8	current_mode;
    I8	prev_mode;

    /* TEST MODE */
    U8 in_active;   // FCT Test Mode
} sm_info_t;

LOCAL sm_info_t sm_info[SM_ID_MAX];

#define	PHASE_STEP_NUM		(8)	
static U8	gu8PhaseTable[SM_ID_MAX][PHASE_STEP_NUM] 	= 
{
    { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 },
    { 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04 },
    { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }
};

void InitStepMotor(void)
{
    U8 i = 0U;
    
    for( i=0; i<SM_ID_MAX; i++ )
    {
        sm_info[i].target = 0;
        sm_info[i].current = 0;

        sm_info[i].speed	= 0;
        sm_info[i].speed_cnt	= 0;

        sm_info[i].move_step = 0;

        sm_info[i].hold_start_step = 0;
        sm_info[i].hold_stop_step	= 0;

        sm_info[i].current_mode = 0;

        sm_info[i].in_active = 0;
    }

    HAL_TIMER03_START();   // Start Timer-03
}

void InitStepVal(U8 _id)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    sm_info[_id].target = 0;
    sm_info[_id].current = 0;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

void SetInActive(U8 _id, U8 _in_active)
{
    sm_info[_id].in_active = _in_active;
}

U8 GetInActive(U8 _id)
{
    return sm_info[_id].in_active;
}

void SetTargetStep(U8 _id, sm_step_t step)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    sm_info[_id].target = step;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

sm_step_t GetTargetStep(U8 _id)
{
    return sm_info[_id].target;
}

void SetCurrentStep(U8 _id, sm_step_t step)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    sm_info[_id].current = step;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

sm_step_t GetCurrentStep(U8 _id)
{
    sm_step_t	step;
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    step = sm_info[_id].current;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    return step;
}

void SetHoldStep(U8 _id, sm_step_t step)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    sm_info[_id].hold_start_step = DEFAULT_HOLD_STEP;	/* about 0.5sec */
    sm_info[_id].hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */

    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    /* ENTER CRITICAL SECTION */
}

static void SetDeltaStep(U8 _id, sm_step_t _step)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    sm_info[_id].move_step = _step;
    sm_info[_id].hold_start_step = DEFAULT_HOLD_STEP;	/* about 0.5sec */
    sm_info[_id].hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */

    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    /* ENTER CRITICAL SECTION */
}

static sm_step_t GetDeltaStep(U8 _id)
{
    INTS_BYTE_STEP_MOTOR_TIMER();
    sm_step_t	step;


    /* ENTER CRITICAL SECTION */
    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    step = sm_info[_id].move_step;

    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    /* ENTER CRITICAL SECTION */

    return step;
}

void	SetStepSpeed(U8 _id, sm_speed_t	speed)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    sm_info[_id].speed = speed;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

sm_speed_t GetStepSpeed(U8 _id)
{
    return sm_info[_id].speed;
}

void SetStepSpeedCount(U8 _id, U8 _cnt)
{
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    sm_info[_id].speed_cnt = _cnt;
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

U8 GetStepSpeedCount(U8 _id)
{
    return sm_info[_id].speed_cnt;
}

void DecStepSpeedCount(U8 _id)
{
    if( sm_info[_id].speed_cnt != 0 )
    {
        sm_info[_id].speed_cnt--;
    }
}

U8 IsDoneMoveStep(U8 _id)
{
    sm_step_t	target;
    sm_step_t	current;

    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();
    target = GetTargetStep(_id);
    current = GetCurrentStep(_id);
    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    if( target == current )
    {
        return TRUE;
    }

    return FALSE;
}

void StopMove(U8 id)
{
    sm_step_t	current;
    INTS_BYTE_STEP_MOTOR_TIMER();

    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    current = GetCurrentStep(id);
    SetTargetStep(id, current);
    SetDeltaStep(id, 0);

    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();
}

U8 SetTargetStepDone(U8 id, sm_step_t step)
{
    SetTargetStep(id, step);

    return IsDoneMoveStep(id);
}

void ControlStepMotor(U8 _id)
{
    sm_step_t val;
    sm_step_t target;
    sm_step_t current;
    INTS_BYTE_STEP_MOTOR_TIMER();


    ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    val 	= GetDeltaStep(_id);
    target 	= GetTargetStep(_id);
    current = GetCurrentStep(_id);

    EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER();

    /* CALC STEP, TARGET - CURRENT */
    if( val == 0 )
    {
        if( target != current )
        {
            val = target - current;
            SetDeltaStep(_id, val);
        }
    }

    /* CONTROL STEP */
    if( GetStepSpeed(_id) == SM_SPEED_1 )
    {
        if( GetStepSpeedCount(_id) != 0 )
        {
            DecStepSpeedCount(_id);
            return ;
        }
        else
        {
            SetStepSpeedCount(_id, SM_SPEED_1_COUNT);
        }
    }
    else if( GetStepSpeed(_id) == SM_SPEED_2 )
    {
        if( GetStepSpeedCount(_id) != 0 )
        {
            DecStepSpeedCount(_id);
            return ;
        }
        else
        {
            SetStepSpeedCount(_id, SM_SPEED_2_COUNT);
        }
    }

    MoveStep(_id);
}

static void ControlGasValveMotor(U8 mode);
static void ControlHotStepMotor(U8 mode);
static void ControlIceDoorMotor(U8 mode);
void 	MoveStep(U8 _id)
{
    sm_info_t *p_info	= NULL;
    U8 val = 0;

    p_info = &sm_info[_id];

    /* START HOLD */
    if( p_info->hold_start_step > 0 )
    {
        p_info->hold_start_step--;
        val = gu8PhaseTable[_id][ p_info->current_mode ];
    }
    else if( p_info->move_step == 0 )
    {
        /* STOP HOLD */
        if( p_info->hold_stop_step > 0 )
        {
            p_info->hold_stop_step--;
            val = gu8PhaseTable[_id][ p_info->current_mode ];
        }
        else
        {
            /* STOP */
            val = 0x00;
        }
    }
    /* MOVE FORWARD */
    else if( p_info->move_step > 0 )
    {
        /* OPEN */
        p_info->move_step--;
        p_info->current++;

        p_info->prev_mode = p_info->current_mode;
        p_info->current_mode++;
        if( p_info->current_mode >= PHASE_STEP_NUM )
        {
            p_info->current_mode 	= 0;
        }

        val = gu8PhaseTable[_id][p_info->current_mode];
    }
    /* MOVE BACKWARD */
    else if( p_info->move_step < 0 )
    {
        /* CLOSE */
        p_info->move_step++;
        p_info->current--;

        p_info->prev_mode = p_info->current_mode;
        p_info->current_mode--;
        if( p_info->current_mode < 0 )
        {
            p_info->current_mode = PHASE_STEP_NUM - 1;
        }

        val = gu8PhaseTable[_id][p_info->current_mode];
    }

    if( _id == SM_ID_0 )
    {
        ControlGasValveMotor( val );
    }
    else if( _id == SM_ID_1 )
    {
        ControlIceDoorMotor( val );
    }
    else if( _id == SM_ID_2 )
    {
        ControlHotStepMotor( val );
    }
}

/**
 * «÷ ∞°Ω∫ ≈ª∫˘ Ω∫≈‹ ∏≈Õ 
 */
static void ControlHotStepMotor(U8 mode)
{
    switch( mode )
    {
        case 0:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_HOT_1  = 1;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 1;
            break;
        case 2:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 1;
            break;
        case 3:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 1;
            P_STEP_MOTOR_HOT_4  = 1;
            break;
        case 4:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 1;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 5:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 1;
            P_STEP_MOTOR_HOT_3  = 1;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 6:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 1;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 7:
            P_STEP_MOTOR_HOT_1  = 1;
            P_STEP_MOTOR_HOT_2  = 1;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 8:
            P_STEP_MOTOR_HOT_1  = 1;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        default:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 1;
            P_STEP_MOTOR_HOT_4  = 1;
            break;
    }
}

/**
 * ≥√∏≈ ¿Ø∑Œ ¿¸»Ø Ω∫≈‹∏≈Õ 
 */
static void ControlGasValveMotor(U8 mode)
{
    switch( mode )
    {
        case 0:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_GAS_1  = 1;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 1;
            break;
        case 2:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 1;
            break;
        case 3:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 1;
            P_STEP_MOTOR_GAS_4  = 1;
            break;
        case 4:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 1;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 5:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 1;
            P_STEP_MOTOR_GAS_3  = 1;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 6:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 1;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 7:
            P_STEP_MOTOR_GAS_1  = 1;
            P_STEP_MOTOR_GAS_2  = 1;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 8:
            P_STEP_MOTOR_GAS_1  = 1;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;

        default:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 1;
            P_STEP_MOTOR_GAS_4  = 1;
            break;
    }
}

/**
 * æ∆¿ÃΩ∫ µµæÓ √ﬂ√‚
 */
static void ControlIceDoorMotor(U8 mode)
{
    switch( mode )
    {
        case 0:
            P_STEP_MOTOR_ICE_DOOR_1  = 0;
            P_STEP_MOTOR_ICE_DOOR_2  = 0;
            P_STEP_MOTOR_ICE_DOOR_3  = 0;
            P_STEP_MOTOR_ICE_DOOR_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_ICE_DOOR_1  = 0;
            P_STEP_MOTOR_ICE_DOOR_2  = 0;
            P_STEP_MOTOR_ICE_DOOR_3  = 1;
            P_STEP_MOTOR_ICE_DOOR_4  = 1;
            break;
        case 2:
            P_STEP_MOTOR_ICE_DOOR_1  = 1;
            P_STEP_MOTOR_ICE_DOOR_2  = 0;
            P_STEP_MOTOR_ICE_DOOR_3  = 0;
            P_STEP_MOTOR_ICE_DOOR_4  = 1;
            break;
        case 3:
            P_STEP_MOTOR_ICE_DOOR_1  = 1;
            P_STEP_MOTOR_ICE_DOOR_2  = 1;
            P_STEP_MOTOR_ICE_DOOR_3  = 0;
            P_STEP_MOTOR_ICE_DOOR_4  = 0;
            break;
        case 4:
            P_STEP_MOTOR_ICE_DOOR_1  = 0;
            P_STEP_MOTOR_ICE_DOOR_2  = 1;
            P_STEP_MOTOR_ICE_DOOR_3  = 1;
            P_STEP_MOTOR_ICE_DOOR_4  = 0;
            break;
        default:
            P_STEP_MOTOR_ICE_DOOR_1  = 0;
            P_STEP_MOTOR_ICE_DOOR_2  = 0;
            P_STEP_MOTOR_ICE_DOOR_3  = 0;
            P_STEP_MOTOR_ICE_DOOR_4  = 0;
            break;
    }
}
