/**
 * File : segment.h
 * 
 * Segment
*/
#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include "prj_type.h"

typedef enum _segment_num_
{
    SEG_1 = 0,    /// 0 : Seg1000 
    SEG_2,        /// 1 : Seg100
    SEG_3,        /// 2 : Seg10
    SEG_4,        /// 3 : Seg1
    SEG_MAX,      /// 4 : Seg Max
} ESegmentNum_T;
    
void InitSegment(void);

void OnOffGroupASeg(ESegmentNum_T segNum, U8 ui);
void OnOffGroupBSeg(ESegmentNum_T segNum, U8 ui);
void AllOnGroupASeg(void);
void AllOffGroupASeg(void);
void AllOnGroupBSeg(void);
void AllOffGroupBSeg(void);

void DispGaSegmentStr(const char *str);
void DispGaSegmentDigits(U16 digits);
void DispGaSegmentNegativeDigits(I16 digits);
void DispGaSegmentDigitsVer(U16 digits);
void DispGaSegmentStrDigit(const char *str, U8 digits);
void DispGaSegment(U8 seg1Data, U8 seg2Data, U8 seg3Data, U8 seg4Data);

#endif /* __SEGMENT_H__  */
