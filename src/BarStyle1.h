#ifndef BAR_STYLE_1
#define BAR_STYLE_1

// smooth progressbar in square frame
// 1 pixel frame, 1 pixel margine to frame, square edges

#include <SmoothLCDProgressBars.h>     

#define USE_PROGMEM

#ifdef USE_PROGMEM
static const PROGMEM struct barstyle
#else
static const struct barstyle
#endif

barStyle1 = {
    {
        { // lANDmask  
        0b00000,
        0b00000,
        0b00111,
        0b00111,
        0b00111,
        0b00111,
        0b00000,
        0b00000},
        { // leftORmask  
        0b11111,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b11111}},
    {
        { // rightANDmask
        0b00000,
        0b00000,
        0b11100,
        0b11100,
        0b11100,
        0b11100,
        0b00000,
        0b00000},
        { // rORmask
        0b11111,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b11111}},
    {{ // mANDmask
        0b00000,
        0b00000,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b00000,
        0b00000},
        { // mORmask
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111}},
    2, // LeftOffset
    2, // RightOffset
    0 // direction...
};


#endif // BAR_STYLE_1
