#ifndef BAR_STYLE_BAT
#define BAR_STYLE_BAT

// smooth progressbar rounded style no frame
// 1 pixel frame, no margin (full height), more rounded edges

#include <SmoothLCDProgressBars.h>     

#define USE_PROGMEM

#ifdef USE_PROGMEM
static const PROGMEM struct barstyle
#else
static const struct barstyle
#endif

barStyleBat = {
    {
        { // lANDmask  
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001},
        { // leftORmask  
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001,
            0b00001},
    },
    {
        { // rightANDmask
            0b10000,
            0b10000,
            0b11100,
            0b11100,
            0b11100,
            0b11100,
            0b10000,
            0b10000},
        { // rightORmask
            0b10000,
            0b10000,
            0b11100,
            0b11100,
            0b11100,
            0b11100,
            0b10000,
            0b10000},
    },
    {
        { // middleANDmask
            0b00000,
            0b00000,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b00000,
            0b00000},
        { // middleORmask
            0b11111,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b11111},
    },
    5, // LeftOffset
    5, // RightOffset
    0  // direction...
};


#endif // !BAR_STYLE_BAT
