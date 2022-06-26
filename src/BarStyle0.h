#ifndef BAR_STYLE_1
#define BAR_STYLE_1

// No frame, full height, square edges, plain

#include <SmoothLCDProgressBars.h>     

#ifdef USE_PROGMEM
static const PROGMEM struct LCDProgressBar::BarStyle 
#else
static struct LCDProgressBar::BarStyle 
#endif

barStyle = {
//struct BarStyle barStyle = {
    { // lANDmask  
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
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
    0b11111,
    0b11111,
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
    0b11111,
    0b11111,
    0b11111,
    0b11111,
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





#endif // !BAR_STYLE_1
