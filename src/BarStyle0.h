#ifndef BAR_STYLE_0
#define BAR_STYLE_0

// PLAIN smooth progressbar
// No frame, full height, square edges, plain

#include <SmoothLCDProgressBars.h>     

#define USE_PROGMEM

#ifdef USE_PROGMEM
static const PROGMEM struct barstyle 
#else
static const struct barstyle
#endif

barStyle0 = {
    {
        { // lANDmask  
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111},
        { // leftORmask  
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000}
    },
    {
        { // rightANDmask
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111},
        { // rightORmask
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000}
    },
    {
        { // middleANDmask
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111},
        { // middleORmask
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000}
    },
    0, // LeftOffset
    0, // RightOffset
    0  // direction...
};

#endif // !BAR_STYLE_1
