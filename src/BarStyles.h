// Defining the bitmasks for different bar styles

#ifndef BARSTYLES_H
#define BARSTYLES_H

struct BarStyle
{
    byte lANDmask[8];
    byte lORmask[8];
    byte rANDmask[8];
    byte rORmask[8];
    byte mANDmask[8];
    byte mORmask[8];
    unsigned char lOff : 4;
    unsigned char rOff : 4;
};

#endif
