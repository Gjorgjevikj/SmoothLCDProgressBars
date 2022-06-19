
/*
yaLCDBatteryGauge library v 1.0.0

Yet another LCD battery gauge for the character based LCD displays for Arduino
Library for displaying battery status gauge on a character based LCD like Hitachi HD44780 16x2 or 20x4
Relies on the LiquidCrystal_I2C.h library (and should work on any platform that supports this library).
Uses the user definable characters of the LCD display.

This library is distributed in the hope that it will be useful, but
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE either express or implied.
Released into the public domain.
Distributed under: MIT License

https://github.com/Gjorgjevikj/yaLCDProgressBar.git

(c) Dejan Gjorgjevikj, 2022
*/

#include <Arduino.h>
#include "yaLCDBatteryGauge.h"     

LCDGauge::LCDGauge(LCD_OBJ & _display, byte _width, byte _row, byte _col)
        : display(_display), width(_width), row(_row), col(_col)
{
}

void LCDGauge::init()
{
    uint8_t t[charRows] = { 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111 };    // empty middle piece
    display.createChar(batBlank, t);
    for (byte i = 2; i < charRows - 2; i++)
        t[i] = 0b11111;
    display.createChar(batFull, t);

    for (byte i = 0; i < charRows; i++)
        t[i] = 0b00001;
    display.createChar(batLeft, t);
    for (byte i = 0; i < charRows; i++)
        t[i] = 0b10000;
    for (byte i = 2; i < charRows-2; i++)
        t[i] = 0b11100;
    display.createChar(batRight, t);
}

void LCDGauge::setPosition(byte r, byte c)
{
    row = r;
    col = c;
}

int LCDGauge::size() const // in pixel columns
{
    return (width-2) * charCols;
}

void LCDGauge::setWidth(int w) // in characters (2 of which are used for the left/right border fo the gauge stylizing a battery) 
{
    width = w;
}

void LCDGauge::showGauge(int val)
{
    CreateCustomChars(val);
    uint8_t full, blank, partial;
    full = min(max(0, val / charCols), width - 2);
    partial = (val < (width - 2)* charCols);
    blank = max(0, width - full - 2 - partial);

    display.setCursor(col, row);
    display.write(batLeft);
    while (full--)
        display.write(batFull);
    if (partial)
        display.write(batMid);
    while (blank--)
        display.write(byte(batBlank));
    display.write(batRight);
}

void LCDGauge::showGaugePct(int val)
{
    showGauge(min(val,100) * (width-2) * charCols / 100);
}

uint8_t LCDGauge::createFillMask(uint8_t val)
{
    uint8_t bitmask = 1 << charCols; // 0b100000;
    uint8_t mask = 0;

    while (val--)
        mask |= (bitmask >>= 1);
    return mask;
}

void LCDGauge::CreateCustomCharacter(uint8_t chpos, uint8_t mask, uint8_t vframe, uint8_t hframe)
{
    byte t[charRows];
    for (byte j = 2; j < charRows - 2; j++)
        t[j] = mask | vframe;
    t[0] = t[charRows - 1] = hframe;
    t[1] = t[charRows - 2] = 0;
    display.createChar(chpos, t);
}

void LCDGauge::CreateCustomChars(int val)
{
    uint8_t mask;
    mask = createFillMask(val % charCols);
    CreateCustomCharacter(batMid, mask, 0, 0b11111);
}

