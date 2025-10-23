/**
 * File : segment.c
 * 
 * Segment
*/
#include "segment.h"
#include "segment_ui.h"

#include "led.h"

#include "string.h"

#define SEGMENT_LED_MAX   (8U) /// a, b, c, d, e, f, g, dot  

typedef enum _segment_group_
{
    GROUP_A = 0,
    GROUP_B,
} ESegmentGroup_T;

//U8 dispBuf[SEG_MAX] = {'\0', '\0', '\0', '\0'};

void InitSegment(void)
{
    InitSegmentUi();
}

typedef void (*FPOnOff)(U8 onOff);
typedef struct _segment_led_
{
    FPOnOff SegLeds[SEGMENT_LED_MAX];
} SSegmentLed_T;

typedef struct _segment_
{
    SSegmentLed_T Seg[SEG_MAX];       /// 1000의 자리, 100의 자리, 10의 자리, 1의 자리
} SSegment_T;

/// Group_A
static void GA_SEG1_B(U8 onOff)
{
    OnOffLed(LED_ID_SEG_1A, onOff);
}

static void GA_SEG1_C(U8 onOff)
{
    OnOffLed(LED_ID_SEG_1B, onOff);
}

static void GA_SEG2_A(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2A, onOff);
}

static void GA_SEG2_B(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2B, onOff);
}

static void GA_SEG2_C(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2C, onOff);
}

static void GA_SEG2_D(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2D, onOff);
}

static void GA_SEG2_E(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2E, onOff);
}

static void GA_SEG2_F(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2F, onOff);
}

static void GA_SEG2_G(U8 onOff)
{
    OnOffLed(LED_ID_SEG_2G, onOff);
}

static void GA_SEG3_A(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3A, onOff);
}

static void GA_SEG3_B(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3B, onOff);
}

static void GA_SEG3_C(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3C, onOff);
}

static void GA_SEG3_D(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3D, onOff);
}

static void GA_SEG3_E(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3E, onOff);
}

static void GA_SEG3_F(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3F, onOff);
}

static void GA_SEG3_G(U8 onOff)
{
    OnOffLed(LED_ID_SEG_3G, onOff);
}

static void GA_SEG4_A(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4A, onOff);
}

static void GA_SEG4_B(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4B, onOff);
}

static void GA_SEG4_C(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4C, onOff);
}

static void GA_SEG4_D(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4D, onOff);
}

static void GA_SEG4_E(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4E, onOff);
}

static void GA_SEG4_F(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4F, onOff);
}

static void GA_SEG4_G(U8 onOff)
{
    OnOffLed(LED_ID_SEG_4G, onOff);
}


/// Group_B






const static SSegment_T SegmentList[] = 
{
/// GROUPA : seg_1000 seg100 seg10 seg1
    {
        {
            { NULL,      GA_SEG1_B, GA_SEG1_C, NULL,      NULL,      NULL,      NULL,      NULL },
            { GA_SEG2_A, GA_SEG2_B, GA_SEG2_C, GA_SEG2_D, GA_SEG2_E, GA_SEG2_F, GA_SEG2_G, NULL }, 
            { GA_SEG3_A, GA_SEG3_B, GA_SEG3_C, GA_SEG3_D, GA_SEG3_E, GA_SEG3_F, GA_SEG3_G, NULL },
            { GA_SEG4_A, GA_SEG4_B, GA_SEG4_C, GA_SEG4_D, GA_SEG4_E, GA_SEG4_F, GA_SEG4_G, NULL }
        }
    },

/// GROUPB : seg_1000 seg100 seg10 seg1
    {
        {
            { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
            { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }, 
            { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
            { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
        }
    }
};

/// Group A Segment
void OnOffGroupASeg(ESegmentNum_T segNum, U8 ui)
{
    U8 i = 0U;
    U8 dispUi = 0U;
    SSegmentLed_T *list = NULL;

    list = &SegmentList[GROUP_A].Seg[segNum];
    
    for( i=0; i<SEGMENT_LED_MAX; i++)
    {
        dispUi = ( (ui >> i) & 0x01 );

        if( list->SegLeds[i] != NULL )
        { 
            list->SegLeds[i](dispUi);
        }
    }
}

/// Group A Segment
void OnOffGroupBSeg(ESegmentNum_T segNum, U8 ui)
{
    U8 i = 0U;
    U8 dispUi = 0U;
    SSegmentLed_T *list = NULL;

    list = &SegmentList[GROUP_A].Seg[segNum];
    
    for( i=0; i<SEGMENT_LED_MAX; i++)
    {
        dispUi = ( (ui >> i) & 0x01 );

        if( list->SegLeds[i] != NULL )
        { 
            list->SegLeds[i](dispUi);
        }
    }
}

void AllOnGroupASeg(void)
{
    U8 *ui = NULL;

    ui = GetSegmentPatterns();

    OnOffGroupASeg(SEG_1, ui['1']);
    OnOffGroupASeg(SEG_2, ui['8']);
    OnOffGroupASeg(SEG_3, ui['8']);
    OnOffGroupASeg(SEG_4, ui['8']);
}

void AllOffGroupASeg(void)
{
    U8 *ui = NULL;

    ui = GetSegmentPatterns();

    OnOffGroupASeg(SEG_1, ui[SPACE]);
    OnOffGroupASeg(SEG_2, ui[SPACE]);
    OnOffGroupASeg(SEG_3, ui[SPACE]);
    OnOffGroupASeg(SEG_4, ui[SPACE]);
}

void AllOnGroupBSeg(void)
{
    U8 *ui = NULL;

    ui = GetSegmentPatterns();

    OnOffGroupBSeg(SEG_1, ui['1']);
    OnOffGroupBSeg(SEG_2, ui['8']);
    OnOffGroupBSeg(SEG_3, ui['8']);
    OnOffGroupBSeg(SEG_4, ui['8']);
}

void AllOffGroupBSeg(void)
{
    U8 *ui = NULL;

    ui = GetSegmentPatterns();

    OnOffGroupBSeg(SEG_1, ui[SPACE]);
    OnOffGroupBSeg(SEG_2, ui[SPACE]);
    OnOffGroupBSeg(SEG_3, ui[SPACE]);
    OnOffGroupBSeg(SEG_4, ui[SPACE]);
}

/*
"E01"
str[0] str[1] str[2] 
0    1    2    3
SEG1 SEG2 SEG3 SEG4
4 -> 0
3 -> 1
2 -> 2
1 -> 3
*/
void DispGaSegmentStr(const char *str)
{
    U8 i = 0;
    U8 *ui = NULL;
    U8 strIndex = 0;
    U8 strSize = (U8)strlen(str);  // 3

    if( strSize > SEG_MAX )
    {
        return;
    }
        
    ui = GetSegmentPatterns();

    for( i=(SEG_MAX-strSize); i<SEG_MAX; i++ )
    {
        OnOffGroupASeg(i, ui[str[strIndex]]);
        strIndex++;
    }
}

#define DIGITS_4SEG_MAX_NUM             (1999)
#define DIGITS_4SEG_NEGATIVE_MAX_NUM    (-99)
#define DIGITS_2SEG_MAX_NUM             (99)
U8 CountDigits(U16 num)
{
    U8 count = 0;

    if( num == 0 )
    {
        return 1;
    }

    while( num != 0 )
    {
        num /= 10;
        count++;
    }

    return count;
}

void DispGaSegmentDigits(U16 digits)
{
    I8 i = 0;
    U8 *ui = NULL;
    U8 data = 0;
    U8 count = 0;

    count = CountDigits(digits);
    if( count > SEG_MAX )
    {
        return;
    }

    if( digits > DIGITS_4SEG_MAX_NUM )
    {
        return;
    }
            
    ui = GetSegmentPatterns();

    for( i=0; i<count; i++ )
    {
        data = (U8)((digits % 10) + '0');
        digits /= 10;
        OnOffGroupASeg((SEG_MAX-1)-i, ui[data]);
    }
}

void DispGaSegmentNegativeDigits(I16 digits)
{
    I8 i = 0;
    U8 *ui = NULL;
    U8 data = 0;
    U8 count = 0;
    U16 positiveData = 0U;

    positiveData = (U16)(ABS(digits));

    count = CountDigits(positiveData);
    if( count > SEG_MAX )
    {
        return;
    }

    if( digits > DIGITS_4SEG_MAX_NUM )
    {
        return;
    }

    if( digits < DIGITS_4SEG_NEGATIVE_MAX_NUM )
    {
        return;
    }
            
    ui = GetSegmentPatterns();

    OnOffGroupASeg(((SEG_MAX-1)-count), ui['-']);

    for( i=0; i<count; i++ )
    {
        data = (U8)((positiveData % 10) + '0');
        positiveData /= 10;
        OnOffGroupASeg((SEG_MAX-1)-i, ui[data]);
    }
}

void DispGaSegmentDigitsVer(U16 digits)
{
    I8 i = 0;
    U8 *ui = NULL;
    U8 data = 0;
    U8 count = 0;

    count = CountDigits(digits);
    if( count > SEG_MAX )
    {
        return;
    }

    if( digits > DIGITS_4SEG_MAX_NUM )
    {
        return;
    }
            
    ui = GetSegmentPatterns();

    OnOffGroupASeg(SEG_2, ui['0']);
    OnOffGroupASeg(SEG_3, ui['0']);
    OnOffGroupASeg(SEG_4, ui['0']);

    for( i=0; i<count; i++ )
    {
        data = (U8)((digits % 10) + '0');
        digits /= 10;
        OnOffGroupASeg((SEG_MAX-1)-i, ui[data]);
    }
}

void DispGaSegmentStrDigit(const char *str, U8 digits)
{
    I8 i = 0;
    U8 *ui = NULL;
    U8 data = 0;
    U8 count = 0;

    ui = GetSegmentPatterns();
    
    OnOffGroupASeg(SEG_2, ui[str[0]]);

    count = CountDigits(digits);
    if( count > 2 )
    {
        return;
    }

    if( digits > DIGITS_2SEG_MAX_NUM )
    {
        return;
    }

    for( i=0; i<count; i++ )
    {
        data = (U8)((digits % 10) + '0');
        digits /= 10;
        OnOffGroupASeg((SEG_MAX-1)-i, ui[data]);
    }
}

void DispGaSegment(U8 seg1Data, U8 seg2Data, U8 seg3Data, U8 seg4Data)
{
    OnOffGroupASeg(SEG_1, seg1Data);
    OnOffGroupASeg(SEG_2, seg2Data);
    OnOffGroupASeg(SEG_3, seg3Data);
    OnOffGroupASeg(SEG_4, seg4Data);    
}

