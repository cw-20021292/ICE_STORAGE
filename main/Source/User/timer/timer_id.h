/**
 * File : timer_id.h
 * 
 * Process Sys Timer Id
*/
#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

/* TIMER - MS */
typedef enum _timer_id_
{
    TIMER_ID_COMM_DEBUG_RX = 0,         // 0 :
    TIMER_ID_COMM_DEBUG_TX,             // 1 :

    TIMER_ID_1MS,                       // 2 :
    TIMER_ID_10MS,                      // 3 :
    TIMER_ID_100MS,                     // 4 :
    TIMER_ID_1SEC,                      // 5 :
    TIMER_ID_10SEC,                     // 6 :
    TIMER_ID_40SEC,                     // 7 :
    TIMER_ID_1MIN,                      // 8 :
    
    TIMER_ID_DEBUG,                     // 9 :
    TIMER_ID_FRONT,                     // 10 :
    TIMER_ID_COMM_FRONT_RX_ERR,         // 11 :

    TIMER_ID_DRAIN_PUMP_24H,            // 12 :

    TIMER_ID_COMM_COMP_REQUEST,         // 13 :
    TIMER_ID_COMM_COMP_RX_ERR,          // 14 :

    TIMER_ID_TEST_8585,                 // 15 :
    TIMER_ID_TEST_8585_2,               // 16 :
    TIMER_ID_TEST_8585_ICE,             // 17 :
    TIMER_ID_TEST_8585_ICE_WATER,       // 18 :
    TIMER_ID_TEST_8585_FEEDER,          // 19 :
    TIMER_ID_TEST_8585_BACK_FEEDER,     // 20 :
    TIMER_ID_TEST_8585_HOT_REPEAT,      // 21 :
    TIMER_ID_TEST_100MS,                // 22 :
    TIMER_ID_TEST_1SEC,                 // 23 :

    TIMER_ID_TEST_STER_CONTY,           // 24 :

    TIMER_ID_MAX                        // 25 :
} ETimerId_T;

typedef enum _timer_user_id_
{
    TIMER_USER_ID_COMM_COMP_RX = 0,     // 0 :
    TIMER_USER_ID_COMM_COMP_TX,         // 1 :
   
    TIMER_USER_ID_COMM_FRONT_RX,        // 2 :
    TIMER_USER_ID_COMM_FRONT_TX,        // 3 :

    TIMER_USER_ID_COMM_EOL_RX,          // 4 :
    TIMER_USER_ID_COMM_EOL_TX,          // 5 :
    TIMER_USER_ID_MAX,                  // 6 :
} ETimerUserId_T;

#endif /* __TIMER_ID_H__ */
