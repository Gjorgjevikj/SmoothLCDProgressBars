#ifndef BAR_STYLE_2
#define BAR_STYLE_2

#include "BarStyles.h"

struct BarStyle barStyle = {
    { // lANDmask  
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01111},
    { // lORmask  
    0b01111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b01111},
    { // rANDmask
    0b11110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11110},
    { // rORmask
    0b11110,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11110},
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

#endif // !BAR_STYLE_2
