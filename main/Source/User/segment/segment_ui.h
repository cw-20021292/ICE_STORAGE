/**
 * File : segment_ui.h
 * 
 * Segment Ui
*/
#ifndef __SEGMENT_UI_H__
#define __SEGMENT_UI_H__

#include "prj_type.h"

/**
   a
   天
 f太 g l b
   天
 e太  l c
   天
   d
*/
#define SPACE     ' '

#define RING_TOP     (0b01100011)
#define RING_BOTTOM  (0b01011100)

#define SEG_OFF      (0b00000000) 
#define SEG_A        (0b00000001)
#define SEG_B        (0b00000010)
#define SEG_C        (0b00000100)
#define SEG_D        (0b00001000)
#define SEG_E        (0b00010000)
#define SEG_F        (0b00100000)
#define SEG_G        (0b01000000)

#define SEG_AG       (SEG_A|SEG_G)
#define SEG_AGD      (SEG_A|SEG_G|SEG_D)
#define SEG_BC       (SEG_B|SEG_C)
#define SEG_AD       (SEG_A|SEG_D)
#define SEG_EF       (SEG_E|SEG_F)
#define SEG_EFA      (SEG_E|SEG_F|SEG_A)
#define SEG_EFAD     (SEG_E|SEG_F|SEG_A|SEG_D)
#define SEG_FAD      (SEG_F|SEG_A|SEG_D)
#define SEG_AB       (SEG_A|SEG_B)
#define SEG_ABC      (SEG_A|SEG_B|SEG_C)
#define SEG_ABCD     (SEG_A|SEG_B|SEG_C|SEG_D)
#define SEG_BCD      (SEG_B|SEG_C|SEG_D)
#define SEG_CD       (SEG_C|SEG_D)

void InitSegmentUi(void);

U8* GetSegmentPatterns(void);

#endif /* __SEGMENT_UI_H__ */