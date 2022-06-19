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

#ifndef LCDGAUGE_H
#define LCDGAUGE_H

#include <Arduino.h>
#include "yaLCDpbConf.h"

class LCDGauge
{
public:
    enum PixelMatrix : byte { charRows = 8, charCols = 5 };
    enum CustomChar : byte { batBlank, batLeft, batMid, batRight, batFull, pbBlank, bpMiddle, pbFull };

    LCDGauge(LCD_OBJ & _display, byte _width, byte _row = 0, byte _col = 0);
    void init();
    int size() const;
    void setPosition(byte, byte);
    void setWidth(int w);
    void showGauge(int val);
    void showGaugePct(int pct);

protected:
    //void showGaugeAlt(int val, bool alt = false);
    static uint8_t createFillMask(uint8_t mask);
    void CreateCustomCharacter(uint8_t chpos, uint8_t mask, uint8_t vframe, uint8_t hframe);
    void CreateCustomChars(int val);

    LCD_OBJ & display; // refference to the display object
    byte width; // progress bar width in characters
    byte row; // row position of the progress bar
    byte col; // column position of the progress bar
};

#endif