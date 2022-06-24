#ifndef BAR_STYLE_SS
#define BAR_STYLE_SS

#include <SmoothLCDProgressBars.h>     

#ifdef USE_PROGMEM
static const PROGMEM struct LCDProgressBar::BarStyle
#else
static struct LCDProgressBar::BarStyle
#endif

barStyle = {
{ // lANDmask  
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00011,
    0b11111,
    0b11111},
{ // lORmask  
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000},
{ // rANDmask
    0b00011,
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111},
{ // rORmask
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000},
{ // mANDmask
    0b00000,
    0b00000,
    0b00001,
    0b00111,
    0b11111,
    0b11111,
    0b11111,
    0b11111},
{ // mORmask
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000},
    0, 0
};

#endif // !BAR_STYLE_SS
