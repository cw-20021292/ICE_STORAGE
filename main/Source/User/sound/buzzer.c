/**
 * File : buzzer.c
 * 
 * Buzzer
*/
#include "hw.h"

#include <string.h>
#include "prj_type.h"
#include "buzzer.h"

#define D_BUZZER_END (0xFFFF)

/* Variable *******************************************************************/
typedef struct _buzzer_
{
    U16 Power; // ON/OFF
    U16 TDR;   // TDR PWM
    U16 Time;  // 출력 시간
    U8 Order;  // 출력 순서
    U8 Output; // 출력음 선택
    U8 Silent; // 무음모드
    U8 CriticalSection;
} Buzzer_T;

Buzzer_T Buzzer;

typedef struct _buzzer_list_
{
    EBuzzId_T Id;
    U16 *pType;
    U8 Size;
    U8 SilentSet;
} SBuzzList_t;

/* Buzzer List ****************************************************************/
// 0 : OFF
const static U16 buzzOff[3][1]= 
{
    {0},
    {0},
    {D_BUZZER_END}
};
// 1 : 전원 ON
const static U16 buzzPowerOn[3][11]=
{
    {ON,  OFF, ON, OFF,  ON,   OFF,  ON,   OFF,  ON,  OFF, OFF},
    {Si5, Si5, Re6, Re6, Sol6, Sol6, Pas6, Pas6, Re7, Re7, 0},
    {4,   19,  4,   19,  4,    19,   5,    65,   5,   112, D_BUZZER_END}
};

// 2 : 추출시작
const static U16 buzzEffStart[3][5]=
{
    {ON, OFF, ON,  OFF, OFF},
    {Re6,Re6, Re7, Re7, 0},
    {3,  21,  3,   21,  D_BUZZER_END}
};
// 3 : 추출종료
const static U16 buzzEffStop[3][7]=
{
    {ON,  OFF, ON,   OFF,  ON,  OFF, OFF},
    {La5, La5, Pas6, Pas6, Re6, Re6, 0},
    {3,   21,  3,    21,   3,   30,  D_BUZZER_END}
};
// 4 : 냉각 ON
const static U16 buzzCoolOn[3][5]=
{
    {ON,   OFF,  ON,  OFF, OFF},
    {Pas6, Pas6, Re7, Re7, 0},
    {5,    19,   10,  38,  D_BUZZER_END}
};
// 5 : 냉각 OFF
const static U16 buzzCoolOff[3][7]=
{
    {ON,    OFF,   ON,  OFF, ON,  OFF, OFF},
    {Sols5, Sols5, Si5, Si5, Mi5, Mi5, 0},
    {2,     21,    2,   21,  10,  38,  D_BUZZER_END}
};
// 선택 / 페어링 / 메멘토1
const static U16 buzzSelect[3][3]=
{
    {ON,  OFF, OFF},
    {La6, La6, 0},
    {8,   28,  D_BUZZER_END}
};
// 에러(불가)
const static U16 buzzError[3][9]=
{
    {ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {Re6, Re6, Re7, Re7, Re6, Re6, Re7, Re7, 0},
    {2,   10,  8,   28,  2,   10,  8,   28,  D_BUZZER_END}
};
// 기능설정
const static U16 buzzFuncSet[3][5]=
{
    {ON,   OFF,  ON,  OFF, OFF},
    {Sol6, Sol6, Re7, Re7, 0},
    {5,    19,   10,  38,  D_BUZZER_END}
};
// 기능해제
const static U16 buzzFuncClr[3][5]=
{
    {ON,  OFF, ON,   OFF,  OFF},
    {Re7, Re7, Sol6, Sol6, 0},
    {5,   19,  10,   38,   D_BUZZER_END}
};
// 10 : 연속추출
const static U16 buzzEffConti[3][17]=
{
    {ON,  OFF, ON,  OFF, ON,   OFF,  ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {Re6, Re6, Re7, Re7, Dos7, Dos7, La6, La6, Mi6, Mi6, La5, La5, Re6, Re6, La6, La6, 0},
    {8,   20,  8,   20,  8,    20,   8,   20,  8,   20,  8,   20,  8,   20,  8,   20,  D_BUZZER_END}
};
// 살균시작
const static U16 buzzSterilizeStart[3][7]=
{
    {ON,  OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {Mi6, Mi6,  Dos6, Dos6, La6,  La6,  0},
    {3,   21,  3,     21,   10,   38,   D_BUZZER_END}
};
// 살균종료
const static U16 buzzSterilizeEnd[3][11]=
{
    {ON,  OFF,  ON,   OFF,  ON,   OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {La6, La6,  Dos6, Dos6, Dos6, Dos6, Pas6, Pas6, Mi6,  Mi6,  0},
    {8,   28,   8,    28,   3,    21,   10,   38,   14,   50,   D_BUZZER_END}
};
// 메멘토2
const static U16 buzzMemento1[3][3]=
{
    {ON,   OFF,   OFF},
    {Sol6, Sol6,  0},
    {6,    50,    D_BUZZER_END}
};
// 메멘토2
const static U16 buzzMemento2[3][5]=
{
    {ON,    OFF,   ON,    OFF,    OFF},
    {Sol6,  Sol6,  Sol6,  Sol6,   0},
    {6,     10,    6,     50,     D_BUZZER_END}
};
// 메멘토3
const static U16 buzzMemento3[3][7]=
{
    {ON,    OFF,    ON,   OFF,  ON,   OFF,    OFF},
    {Sol6,  Sol6,   Sol6, Sol6, Sol6, Sol6,   0},
    {6,     10,     6,    10,   6,    50,     D_BUZZER_END}
};
// 메멘토4
const static U16 buzzMemento4[3][9]=
{
    {ON,    OFF,    ON,     OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {Sol6,  Sol6,   Sol6,   Sol6, Sol6, Sol6, Sol6, Sol6, 0},
    {6,     10,     6,      10,   6,    10,   6,    50,   D_BUZZER_END}
};
// 메멘토5
const static U16 buzzMemento5[3][11]=
{
    {ON,   OFF,   ON,   OFF,  ON,   OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {Sol6, Sol6,  Sol6, Sol6, Sol6, Sol6, Sol6, Sol6, Sol6, Sol6, 0},
    {6,    10,    6,    10,   6,    10,   6,    10,   8,    50,   D_BUZZER_END}
};
// WIFI 서버연결
const static U16 buzzServerConnect[3][7]=
{
    {ON,  OFF,  ON,  OFF, ON,  OFF,  OFF},
    {Do7, Do7,  Re7, Re7, Mi7, Mi7,  0},
    {6,   10,   6,   10,  6,   50,   D_BUZZER_END}
};
// AP 접속
const static U16 buzzAPConnect[3][7]=
{
    {ON,  OFF, ON,  OFF, ON,  OFF,  OFF},
    {Do6, Do6, Re6, Re6, Mi6, Mi6,  0},
    {6,   10,  6,   10,  6,   50,   D_BUZZER_END}
};

/* Buzzer List Table **********************************************************/
SBuzzList_t BuzzList[BUZZER_MAX] = 
{ /* Id                       Table                              Size   SilentMode */
    { BUZZER_OFF,                 &buzzOff[0][0],               1U,    OFF },
    { BUZZER_POWER_ON,            &buzzPowerOn[0][0],           11U,   OFF },
    { BUZZER_EFFLUENT,            &buzzEffStart[0][0],          5U,    OFF },
    { BUZZER_EFFLUENT_END,        &buzzEffStop[0][0],           7U,    OFF },
    { BUZZER_COOL_SETUP,          &buzzCoolOn[0][0],            5U,    OFF },
    { BUZZER_COOL_CANCEL,         &buzzCoolOff[0][0],           7U,    OFF },
    { BUZZER_SELECT,              &buzzSelect[0][0],            3U,    OFF },
    { BUZZER_ERROR,               &buzzError[0][0],             9U,    OFF },
    { BUZZER_SETUP,               &buzzFuncSet[0][0],           5U,    OFF },
    { BUZZER_CANCEL,              &buzzFuncClr[0][0],           5U,    OFF },
    { BUZZER_EFFLUENT_CONTINUE,   &buzzEffConti[0][0],          17U,   OFF },
    { BUZZER_STERILIZE_START,     &buzzSterilizeStart[0][0],    7U,    OFF },
    { BUZZER_STERILIZE_END,       &buzzSterilizeEnd[0][0],      11U,   OFF },
    { BUZZER_MEMENTO_1,           &buzzMemento1[0][0],          3U,    OFF },
    { BUZZER_MEMENTO_2,           &buzzMemento2[0][0],          5U,    OFF },
    { BUZZER_MEMENTO_3,           &buzzMemento3[0][0],          7U,    OFF },
    { BUZZER_MEMENTO_4,           &buzzMemento4[0][0],          9U,    OFF },
    { BUZZER_MEMENTO_5,           &buzzMemento5[0][0],          11U,   OFF },
    { BUZZER_WIFI_SELECT,         &buzzSelect[0][0],            3U,    ON  },
    { BUZZER_WIFI_SETUP,          &buzzFuncSet[0][0],           5U,    ON  },
    { BUZZER_WIFI_ERROR,          &buzzError[0][0],             9U,    ON  },
    { BUZZER_WIFI_CONNECTING,     &buzzAPConnect[0][0],         7U,    ON  },
    { BUZZER_WIFI_CONNECTED,      &buzzServerConnect[0][0],     7u,    ON  },
    { BUZZER_WIFI_FAIL_CONNECTING,&buzzFuncClr[0][0],           5u,    ON  },
    { BUZZER_WIFI_DISCONNECTED,   &buzzFuncClr[0][0],           5u,    ON  },
    { BUZZER_BLE_FAIL,            &buzzFuncClr[0][0],           5u,    ON  },
    { BUZZER_MUTE,                &buzzFuncSet[0][0],           5u,    ON  },
};
#define SZ_BZ_LIST  ( sizeof(BuzzList) / sizeof(SBuzzList_t) )

/* Function *******************************************************************/
static void BuzzStart(void);
static void BuzzStop(void);
static void BuzzerCount(void);
static void BuzzerCheck(void);

void InitializeBuzzer(void)
{
    MEMSET( (void __FAR *)&Buzzer, 0, sizeof(Buzzer_T));
    TURN_OFF_TDR();
}

void SetBuzzOut(EBuzzId_T id)
{
    if( Buzzer.Silent == ON
            && BuzzList[id].SilentSet == OFF )
    {
        return;
    }
    Buzzer.CriticalSection = ON;
    Buzzer.Output = id;
    Buzzer.Order = 0;
    Buzzer.Time = 0;
    Buzzer.CriticalSection = OFF;
}

U8 IsSetSilent(void)
{
    return Buzzer.Silent;
}

void SetOnOffSilent(U8 silent)
{
    if(silent == ON)
    {
        Buzzer.Silent = ON;
    }
    else
    {
        Buzzer.Silent = OFF;
    }
}

static void Buzzing(U16 time)
{
    if( time == 0 )
    {
        BuzzStart();
        BuzzerCheck();
    }
}

void BuzzerControl(void)
{
    //-1- Local Variable Definition
    static U8 countBuzzCheckTime = 0;

    if(Buzzer.CriticalSection == ON)
    {
        return;
    }

    //-2- Buzzer Timer 10ms
    BuzzerCount();

    //-3- Buzzer Check & Output Setting
    if( Buzzer.Output == BUZZER_OFF ) // Not Choice
    {
        BuzzStop();
        if( Buzzer.Silent == ON )
        {
            InitializeBuzzer();
            Buzzer.Silent = ON;
        }
        else
        {
            InitializeBuzzer();
        }
    }
    else // Choice
    {
        if( Buzzer.Silent == OFF )
        {
            Buzzing(Buzzer.Time);
        }
        else if( BuzzList[Buzzer.Output].SilentSet==ON )
        {
            Buzzing(Buzzer.Time);
        }
    }

    //-4- TDR, PORT ON/OFF
    if( Buzzer.Power == ON )
    {
        TURN_ON_BUZZER();
        TURN_ON_TDR();
    }
    else
    {
        TURN_OFF_BUZZER();
    }
}

/* Local Function Information ==================================================
   ※ Description : 부저 TDR 레지스터 Start / Stop
   ==============================================================================*/
static void BuzzStart(void)
{
    SoundStart();
}

static void BuzzStop(void)
{
    SoundStop();
}

/* Local Function Information ==================================================
   ※ Description : 부저 시간 카운터
   ==============================================================================*/
static void BuzzerCount(void)
{
    static U8 countBuzzCheckTime = 0;

    countBuzzCheckTime++;
    if( countBuzzCheckTime >= 10 )
    {
        countBuzzCheckTime = 0;
        if( Buzzer.Time != 0 )
        {
            Buzzer.Time--;
        }
    }
}

/* Local Function Information ==================================================
   ※ Description : 해당부저 Power, TDR, Time 설정
   ==============================================================================*/
static void BuzzerCheck(void)
{
    SBuzzList_t *pBuz = NULL;
    U8 buzzCount = 0;
    U16 idxTDR = 0;
    U16 idxTime = 0;

    for( buzzCount=0 ; buzzCount<SZ_BZ_LIST; buzzCount++ )
    {
        pBuz = &BuzzList[ buzzCount ];
        idxTDR = pBuz->Size + Buzzer.Order;
        idxTime = (pBuz->Size*2) + Buzzer.Order;

        if( pBuz->Id == Buzzer.Output )
        {
            Buzzer.Power = pBuz->pType[ Buzzer.Order ];
            Buzzer.TDR = pBuz->pType[ idxTDR ];
            Buzzer.Time = pBuz->pType[ idxTime ];
            Buzzer.Order++;

            if( Buzzer.Time == D_BUZZER_END)
            {
                Buzzer.Output = BUZZER_OFF;
            }
            break;
        }
    }
}
