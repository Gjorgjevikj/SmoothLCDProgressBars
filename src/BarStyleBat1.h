#ifndef BAR_STYLE_BAT1
#define BAR_STYLE_BAT1

// Contains the bit masks defining a style for a smooth progress bar 
// to be used with SmoothLCDProgressBars library
// https://github.com/Gjorgjevikj/SmoothLCDProgressBars.git
// 
// Defines a smooth horizontal progress bar representing a battery
// 1 pixel frame, 1 pixel margin, 2 pixel offsets 

#include <barstyle.h>

#ifdef BAR_STYLE_BAT1_IN_PROGMEM
static const PROGMEM struct barstyle
#else
static const struct barstyle
#endif

barStyleBat1 = {
    {
        { // lANDmask  
            0b00000,
            0b01111,
            0b01111,
            0b01111,
            0b01111,
            0b01111,
            0b01111,
            0b00000},
        { // leftORmask  
            0b11111,
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b10000,
            0b11111},
    },
    {
        { // rightANDmask
            0b00000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b11000,
            0b00000},
        { // rightORmask
            0b11100,
            0b00100,
            0b00111,
            0b00111,
            0b00111,
            0b00111,
            0b00100,
            0b11100},
    },
    {
        { // middleANDmask
            0b00000,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
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
    1, // LeftOffset
    3, // RightOffset
    0  // direction...
};


#endif // !BAR_STYLE_BAT1
