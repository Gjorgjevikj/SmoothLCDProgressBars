#ifndef BAR_STYLE_BAT
#define BAR_STYLE_BAT

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
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001},
    { // lORmask  
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001},
    { // rANDmask
    0b10000,
    0b10000,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b10000,
    0b10000},
    { // rORmask
    0b10000,
    0b10000,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b10000,
    0b10000},
    { // mANDmask
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
    0b11111},
    5, 5
};

#endif // !BAR_STYLE_BAT
