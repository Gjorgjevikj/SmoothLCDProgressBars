#ifndef BAR_STYLE_1
#define BAR_STYLE_1

//#include "BarStyles.h"
#include <SmoothLCDProgressBars.h>     

#ifdef USE_PROGMEM
static const PROGMEM struct LCDProgressBar::BarStyle 
#else
static struct LCDProgressBar::BarStyle 
#endif

barStyle = {
//struct BarStyle barStyle = {
    { // lANDmask  
    0b00000,
    0b00000,
    0b00111,
    0b00111,
    0b00111,
    0b00111,
    0b00000,
    0b00000},
    { // lORmask  
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111},
    { // rANDmask
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
    0b11111},
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
    2, 2
};





#endif // !BAR_STYLE_1
