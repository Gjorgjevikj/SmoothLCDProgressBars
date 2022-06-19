/*
yaLCDProgressBar library v 1.0.0

Yet another LCD progress bar (gauge) for the character based LCD displays for Arduino
Library for displaying progress bar on a character based LCD like Hitachi HD44780 16x2 or 20x4
Relies on the LiquidCrystal_I2C.h library (and should work on any platform that supports this library).
Uses the user definable characters of the LCD display and can dispalay up to 2 smoot gauges simultaneously.

Known minorities:
- no checking of the parameters (so the gauges can act unexpectedly if odd values are passed, like negative or very big numbers) for performance reasons.
- the borders drawn with the gauge are considered as part of the gauge
- at most 2 independent progress bars can be controled (restricted by the limitation of only 8 user definable characters on these displays)

This library is distributed in the hope that it will be useful, but
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE either express or implied.
Released into the public domain.
Distributed under: MIT License

https://github.com/Gjorgjevikj/yaLCDProgressBar.git

(c) Dejan Gjorgjevikj, 2022
*/

#ifndef LCDPROGRESSBAR_H
#define LCDPROGRESSBAR_H

#include <Arduino.h>
#include "yaLCDpbConf.h"

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

//#pragma message(VAR_NAME_VALUE(PB_TYPE))

#if PB_TYPE == 0

#define CUSTCHAR_PB_FULL_C = 255
#define CUSTCHAR_PB_BLANK 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111
#define VFRAME_L 0b10000
#define HFRAME_L 0b01111
#define VFRAME_R 0b00001
#define HFRAME_R 0b11110

#elif PB_TYPE == 1

#define CUSTCHAR_PB_FULL_C 
#define CUSTCHAR_PB_BLANK 0b00000, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b00000
#define VFRAME_L 0b10000
#define HFRAME_L 0b11111
#define VFRAME_R 0b00001
#define HFRAME_R 0b11111

#elif PB_TYPE == 2

#define CUSTCHAR_PB_FULL_C 
#define CUSTCHAR_PB_BLANK 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111
#define VFRAME_L 0b10000
#define HFRAME_L 0b11111
#define VFRAME_R 0b00001
#define HFRAME_R 0b11111

#else
#error "Not a valid value for PB_TYPE"
#endif


class LCDProgressBar
{
public:
    enum PixelMatrix : byte { charRows = 8, charCols = 5 };
    enum CustomChar : byte { pbBlank, pbLeft, pbMiddle, pbRight, pbLeft2, pbMiddle2, pbRight2, pbFull CUSTCHAR_PB_FULL_C };

    LCDProgressBar(LCD_OBJ & _display, byte _width, byte _row = 0, byte _col = 0, bool _alt = false);
    void init();
    int size() const;
    void setWidth(int w);
    void setPosition(byte, byte);
    void showProgress(int val);
    void showProgressPct(int pct);

protected:
    void showProgressAlt(int val, bool alt = false);
    static uint8_t createFillMask(uint8_t mask);
    void CreateCustomCharacter(uint8_t chpos, uint8_t mask, uint8_t vframe, uint8_t hframe);
    void CreateCustomChars(int val, bool alt = false);

    LCD_OBJ & display; // refference to the display object
    byte width; // progress bar width in characters
    byte row; // row position of the progress bar
    byte col; // column position of the progress bar
    bool alt; // first or second progress bar
};

#endif