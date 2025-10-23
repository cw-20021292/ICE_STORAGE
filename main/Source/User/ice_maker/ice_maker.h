#ifndef __ICE_MAKER_H__
#define __ICE_MAKER_H__

#include "prj_type.h"

typedef struct _ice_maker_temp_t
{
    TEMP_T tCold;
    TEMP_T tAmbi;
} IcingTemp_T;

/* INIT */
typedef enum
{
    STEP_INIT_TRAY_MOVE,
    STEP_INIT_TRAY_DEICING,
    STEP_INIT_TRAY_DONE
} InitStep_T;

typedef struct _do_init_
{
    InitStep_T Step;
} DoInit_T;

/* PREHEAT */
typedef enum
{
    STEP_PREHEAT_CHECK,
    STEP_PREHEAT_CHECK_COMP,
    STEP_PREHEAT_INIT,
    STEP_PREHEAT_HOTGAS_SWITCH_CHECK,
    STEP_PREHEAT_GAS_CLOSE_SWITCH_WAIT,
    STEP_PREHEAT_START,
    STEP_PREHEAT_WAIT,
    STEP_PREHEAT_GAS_ICING_SWITCH_CHECK,
    STEP_PREHEAT_GAS_ICING_SWITCH_WAIT,
    STEP_PREHEAT_DONE_CONTROL,
    STEP_PREHEAT_DONE
} EPreheatStep_T;

typedef struct _do_preheat_
{
    EPreheatStep_T Step;

    U8 Region;
    U8 Count;
    U8 RepeatNum;

    U8 OnOffCmd;
    U16 PreHeatTime;        // Preheat Time
    U8 CompRps;            // Rps of Comp On

    U16 GasSwitchWaitTime; 
} SDoPreheat_T;

/* READY */
typedef enum
{
    STEP_READY_CHECK_CONDITION,

    STEP_READY_TRAY_MOVE,
    STEP_READY_TRAY_DONE,

    STEP_READY_CHECK_CONTY_ICING,
    STEP_READY_CHECK_CONTY_ICING_DONE,

    STEP_READY_EMPTY_DRAIN_2,
    STEP_READY_TRAY_FEED_START,
    STEP_READY_TRAY_FEED_DONE,

    STEP_READY_GAS_SWITCH,
    STEP_READY_GAS_SWITCH_DONE,

    STEP_READY_HOT_GAS_SWITCH,
    //STEP_READY_HOT_GAS_SWITCH_DONE,

    STEP_READY_COMP_ON,

    STEP_READY_DONE

} ReadyStep_T;

typedef struct _do_ready_
{
    ReadyStep_T Step;

    U32 TrayFeedAmount;      // Time of Ice tray feeding
    U16 RoomOutTimeOut;     // 정수 추출 시간 초과

    //U16 TrayWaitTime;   // 트레이 이동 후, 떨어지는 물에 대한 안정화 시간
    U16 TrayFeedTime;   // Time of Ice tray feeding
    U16 TimeOut;
} DoReady_T;


/* ICING */
typedef enum
{
    STEP_ICING_CHECK_COMP,
    STEP_ICING_FIGUARE_OUT_TIME,
    STEP_ICING,

    STEP_ICING_DONE
} IcingStep_T;

typedef struct _do_icing_
{
    IcingStep_T Step;

    U16 IcingTime;      // Time of Comp On
    U8  CompRps;        // Rps of Comp On
    U8  DeicingCompRps; // Rps of Comp On
    U8  FanOnOff;
} DoIcing_T;


/* DE-ICING */
typedef enum
{
    STEP_DEICING_TRAY_MOVE,             // 0
    STEP_DEICING_TRAY_DONE,             // 1

    STEP_DEICING_FIGUARE_OUT_REGION,    // 2
    STEP_DEICING_COMMAND,               // 3
    STEP_DEICING_CONTROL,               // 4

    STEP_DEICING_HOTGAS_SWITCH_CHECK,     // 5
    STEP_DEICING_GAS_CLOSE_SWITCH_WAIT,   // 6
    STEP_DEICING_20DEGREE_UNDER_COMMAND,  // 7
    STEP_DEICING_20DEGREE_UNDER_CONTROL,  // 8
    STEP_DEICING_GAS_ICING_SWITCH_CHECK,  // 9
    STEP_DEICING_GAS_ICING_SWITCH_WAIT,   // 10

    STEP_DEICING_DONE_CONTROL,          // 11      

#if CONFIG_BACK_FEEDER
    STEP_DEICING_BACK_FEEDER_START,     // 12
    STEP_DEICING_BACK_FEEDER_DONE,      // 13
#endif

    STEP_DEICING_CHECK_ICE_FULL,        // 14 or 12
    STEP_DEICING_WAIT_CHECK_ICE_FULL,   // 15 or 13

    STEP_DEICING_DONE                   // 16 or 14
} EDeIcingStep_T;

typedef struct _do_deicing_
{
    EDeIcingStep_T Step;

    /* DEICING HEATER REPEAT ON/OFF COMMAND & TIME */
    U8 Region;
    U8 Count;
    U8 RepeatNum;

    /* DEICING HEATER or COMP */
    U8 OnOffCmd;
    U16 OnOffTime;
    U8 CompRps;      // Rps of Comp On
    U8 FanOnOff;

    U16 GasSwitchWaitTime;

#if( CONFIG_BACK_FEEDER == 1 )
    U16 BackFeederTime;
#endif
} SDoDeIcing_T;



/* DUMMY DE-ICING */
typedef enum
{
    STEP_DUMMY_DEICING_GAS_SWITCH,
    STEP_DUMMY_DEICING_TRAY_MOVE,
    STEP_DUMMY_DEICING_TRAY_DONE,

    STEP_DUMMY_DEICING_COMP_PROTECT_TIME,
    STEP_DUMMY_DEICING_FIGUARE_OUT_REGION,
    STEP_DUMMY_DEICING_GAS_SWITCH_VALVE_CHECK,
    STEP_DUMMY_DEICING_COMP_ON,

    STEP_DUMMY_DEICING_COMMAND,

    STEP_DUMMY_DEICING_HOT_GAS_SWITCH_CHECK,
    STEP_DUMMY_DEICING_CONTROL,

    STEP_DUMMY_DEICING_20DEGREE_UNDER_HOT_GAS_SWITCH_CHECK,
    STEP_DUMMY_DEICING_GAS_CLOSE_SWITCH_WAIT,
    STEP_DUMMY_DEICING_GAS_CLOSE_SWITCH,
    STEP_DUMMY_DEICING_20DEGREE_UNDER_CONTROL,
    STEP_DUMMY_DEICING_GAS_ICING_SWITCH_CHECK,
    STEP_DUMMY_DEICING_GAS_ICING_SWITCH_WAIT,
    
    STEP_DUMMY_DEICING_CONTROL_CHECK,
    STEP_DUMMY_DEICING_DONE_CONTROL,

    STEP_DUMMY_DEICING_DONE
} EDummyDeIcingStep_T;

typedef struct _do_init_deicing_
{
    EDummyDeIcingStep_T Step;

    /* DEICING HEATER REPEAT ON/OFF COMMAND & TIME */
    U8 Region;
    U8 Count;
    U8 RepeatNum;

    /* COMP TIME */
    U8 CompOnCmd;
    U16 CompOnTime;
    U8 CompOnRps;

    /* DEICING HEATER */
    U8 OnOffCmd;
    U16 OnOffTime;
    U8 OnOffRps;       // Rps of Comp On

    U8 FanOnOff;

    TEMP_T Ambient;
    U16 GasSwitchWaitTime;
} SDoDummyDeIcing_T;


/* DUMMY DE-ICING */
typedef enum
{
    STEP_ICING_MOVE_DEICING_INIT,

    STEP_ICING_MOVE_DEICING_COMP_PROTECT_TIME,
    STEP_ICING_MOVE_DEICING_HOTGAS_SWITCH,
    STEP_ICING_MOVE_DEICING_HOTGAS_SWITCH_CHECK,
    STEP_ICING_MOVE_DEICING_GAS_CLOSE_SWITCH_WAIT,
    STEP_ICING_MOVE_DEICING_GAS_CLOSE_SWITCH_CHECK,
    
    STEP_ICING_MOVE_DEICING_COMMAND,
    STEP_ICING_MOVE_DEICING_CONTROL,
    STEP_ICING_MOVE_DEICING_GAS_ICING_SWITCH_CHECK,
    STEP_ICING_MOVE_DEICING_DONE_CONTROL,
    STEP_ICING_MOVE_DEICING_DONE_CONTROL_CHECK,

    STEP_ICING_MOVE_DEICING_COMP_OFF_WAIT,
    
    STEP_ICING_MOVE_DEICING_DONE
} IcingMoveDeicingStep_T;

typedef struct _do_iceing_move_deicing_
{
    IcingMoveDeicingStep_T Step;

    U8  TargetRps;
    U16 TargetTime;
    U16 GasSwitchWaitTime;
    U16 CompOffWaitTime;
}DoIcingMoveDeIcing_T;


void GetIceMakerInitData( DoInit_T *pData );

void GetIceMakerPreheatData(SDoPreheat_T *pData);

void GetIceMakerReadyData( DoReady_T *pData );

void GetIceMakerIcingData( DoIcing_T *pData );

void GetIceMakerDeIcingData(SDoDeIcing_T *pData);

void GetIceMakerInitDeIcingData(SDoDummyDeIcing_T *pData);

void GetIceMakerIcingMoveDeIcingData( DoIcingMoveDeIcing_T *pData );

void SetIcingTime(U16 mu16Val );
void SetDeIcingTime(U16 mu16Val );

// 더미 탈빙 중지
void StopDummyDeIcing(void);

void ProcessIceMaker(void);

// 타이머 업데이트 ( @100ms )
void UpdateIceMakerTimer(void);

#endif /* __ICE_MAKER_H__ */
