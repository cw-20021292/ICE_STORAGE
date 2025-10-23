/**
 * File : segment_ui.c
 * 
 * Segment Ui
*/
#include "segment_ui.h"

U8 Patterns[128] =
{
    0,

#if 0
    ['0'] = 0b00111111, ['1'] = 0b00000110, ['2'] = 0b01011011, ['3'] = 0b01001111, ['4'] = 0b01100110,
    ['5'] = 0b01101101, ['6'] = 0b01111101, ['7'] = 0b00000111, ['8'] = 0b01111111, ['9'] = 0b01101111,   
    ['A'] = 0b01110111, ['b'] = 0b01111100, ['C'] = 0b00111001, ['d'] = 0b00111110, ['E'] = 0b01111001,
    ['F'] = 0b01110001, ['H'] = 0b01110110, ['L'] = 0b00111000, ['P'] = 0b01110011, ['U'] = 0b00111110,
    ['-'] = 0b01000000, ['\0'] = 0b00000000
#endif
};

/**
   a
   天
 f太 g l b
   天
 e太  l c
   天
   d
*/
void InitSegmentUi(void)
{
    Patterns['0'] = 0b00111111;
    Patterns['1'] = 0b00000110;
    Patterns['2'] = 0b01011011;
    Patterns['3'] = 0b01001111;
    Patterns['4'] = 0b01100110;
    Patterns['5'] = 0b01101101;
    Patterns['6'] = 0b01111101;
    Patterns['7'] = 0b00000111;
    Patterns['8'] = 0b01111111;
    Patterns['9'] = 0b01101111;
    Patterns['A'] = 0b01110111;
    Patterns['b'] = 0b01111100;
    Patterns['C'] = 0b00111001;
    Patterns['c'] = 0b01011000;
    Patterns['d'] = 0b01011110;
    Patterns['E'] = 0b01111001;
    Patterns['F'] = 0b01110001;
    Patterns['H'] = 0b01110110;
    Patterns['L'] = 0b00111000;
    Patterns['n'] = 0b01010100;
    Patterns['O'] = 0b00111111;
    Patterns['o'] = 0b01011100;
    Patterns['P'] = 0b01110011;
    Patterns['R'] = 0b01110111;
    Patterns['S'] = 0b01101101;
    Patterns['T'] = 0b01111000;
    Patterns['U'] = 0b00111110;
    Patterns['-'] = 0b01000000;
    Patterns['['] = 0b00111001;
    Patterns[']'] = 0b00001111;
    Patterns[SPACE] = 0b00000000;
}

U8* GetSegmentPatterns(void)
{
    return &Patterns[0];
}
