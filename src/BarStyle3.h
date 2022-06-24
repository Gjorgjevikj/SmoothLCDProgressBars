#ifndef BAR_STYLE_3
#define BAR_STYLE_3

#include <SmoothLCDProgressBars.h>     

#ifdef USE_PROGMEM
static const PROGMEM struct LCDProgressBar::BarStyle
#else
static struct LCDProgressBar::BarStyle
#endif

barStyle = {
    { // lANDmask  
    0b00111,
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01111,
    0b00111},
    { // lORmask  
    0b00111,
    0b01000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b01000,
    0b00111},
    { // rANDmask
    0b11100,
    0b11110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11110,
    0b11100},
    { // rORmask
    0b11100,
    0b00010,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00010,
    0b11100},
    { // mANDmask
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111},
    { // mORmask
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111},
    1, 1
};

#endif // !BAR_STYLE_3
