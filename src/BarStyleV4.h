#ifndef BAR_STYLE_V_1
#define BAR_STYLE_V_1

// vertical smooth progressbar in square frame
// 1 pixel frame, 1 pixel margine to frame, square edges

#include <SmoothLCDProgressBars.h>     

#define USE_PROGMEM

#ifdef USE_PROGMEM
static const PROGMEM struct barstyle
#else
static const struct barstyle
#endif

barStyleV4 = {
    {
        { // bottomANDmask  
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b00000},
        { // bottomORmask  
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b11111}},
    {
        { // topANDmask
        0b00000,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110},
        { // topORmask
        0b11111,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001}},
    {
        { // middleANDmask
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110,
        0b01110},
        { // middleORmask
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001}},
    1, // LeftOffset
    1, // RightOffset
    0 // direction...
};


#endif // BAR_STYLE_V_1
