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

#define LCDPROGRESSBAR_VER 1.0.0

#include <Arduino.h>
#include "yaLCDProgressBar.h"     

/// <summary>
/// LCDProgressBar Class (2 independent progress bars can be created)
/// </summary>
/// <param name="_display">refference to the LiquidCrystal_I2C object</param>
/// <param name="_width">width of the progress bar in characters</param>
/// <param name="_row">row position of the progress bar</param>
/// <param name="_col">column position of the progress bar</param>
/// <param name="_alt">first (false or omitted) or second (true)</param>
LCDProgressBar::LCDProgressBar(LCD_OBJ & _display, byte _width, byte _row, byte _col, bool _alt)
        : display(_display), width(_width), row(_row), col(_col), alt(_alt)
{
}

/// <summary>
/// Initializes the object
/// </summary>
void LCDProgressBar::init()
{
    uint8_t t[charRows] = { CUSTCHAR_PB_BLANK };    // empty middle piece
    display.createChar(pbBlank, t);
#if PB_TYPE == 1 || PB_TYPE == 2
    for (byte j = 2; j < charRows - 2; j++)
        t[j] = 0b11111;
    display.createChar(pbFull, t);
#endif
}

/// <summary>
/// Sets the position of the progress bar
/// </summary>
/// <param name="r">row position</param>
/// <param name="c">column position</param>
void LCDProgressBar::setPosition(byte r, byte c)
{
    row = r;
    col = c;
}

/// <summary>
/// The size of the pogress bar in pixels
/// </summary>
/// <returns>the width of the progress bar in pixels</returns>
int LCDProgressBar::size() const
{
    return width * charCols;
}

/// <summary>
/// Sets the width of the progress bar in characters
/// </summary>
/// <param name="w">the width of the progress bar in characters</param>
void LCDProgressBar::setWidth(int w) // in characters 
{
    width = w;
}

/// <summary>
/// Draws the progress bar for the given value 
/// </summary>
/// <param name="val">the filled pard of the progress bar in pixels (0-size())</param>
void LCDProgressBar::showProgress(int val)
{
    showProgressAlt(val, alt);
}

/// <summary>
/// Draws the progress bar for the given value 
/// </summary>
/// <param name="val">the filled part of the progress bar in percent (0-100)</param>
void LCDProgressBar::showProgressPct(int val)
{
    showProgressAlt(val * (width * charCols) / 100, alt);
}

/// <summary>
/// Draws the progress bar
/// </summary>
/// <param name="val">the filled part of the progress bar in pixels</param>
/// <param name="alt">the first or the second bar</param>
void LCDProgressBar::showProgressAlt(int val, bool alt)
{
    CreateCustomChars(val, alt);
    uint8_t full, blank, partial;
    full = min(max(0, val / charCols - 1), width - 2);
    partial = (val > charCols && val < (width - 1)* charCols);
    blank = max(0, width - full - 2 - partial);

    display.setCursor(col, row);
    display.write(alt ? pbLeft2 : pbLeft);
    while (full--)
        display.write(pbFull);
    if (partial)
        display.write(alt ? pbMiddle2 : pbMiddle);
    while (blank--)
        display.write(byte(pbBlank));
    display.write(alt ? pbRight2 : pbRight);
}

/// <summary>
/// Creates the bitmask representing how much the character is filled up left to right
/// </summary>
/// <param name="val">the value of the progress bar in pixels</param>
/// <returns>the bitmask with ones on the left and zeroes on the left representing how much it is supposed to be filled</returns>
uint8_t LCDProgressBar::createFillMask(uint8_t val)
{
    uint8_t bitmask = 1 << charCols; // 0b100000;
    uint8_t mask = 0;

    while (val--)
        mask |= (bitmask >>= 1);
    return mask;
}

/// <summary>
/// Creates the user definable character that is at the top of the progress bar
/// </summary>
/// <param name="chpos">which user definable character (0-7) to be used</param>
/// <param name="mask">the bitmask to fill all rows of the character</param>
/// <param name="vframe">the bitmask to be OR-ed with all the rows of the character (for drawing the frame)</param>
/// <param name="hframe">the bitmask for the top and bottom row of the character (for drawing the frame)</param>
void LCDProgressBar::CreateCustomCharacter(uint8_t chpos, uint8_t mask, uint8_t vframe, uint8_t hframe)
{
    byte t[charRows];
    for (byte j = 1; j < charRows - 1; j++)
        t[j] = mask | vframe;
#if PB_TYPE == 0
    t[0] = t[charRows - 1] = hframe;
#elif PB_TYPE == 1 
    t[0] = t[charRows - 1] = 0;
    t[1] = t[charRows - 2] = hframe;
#elif PB_TYPE == 2
    t[0] = t[charRows - 1] = hframe;
    t[1] = t[charRows - 2] = 0;
#endif
    display.createChar(chpos, t);
}

/// <summary>
/// Creates the user definable characters
/// </summary>
/// <param name="val">the filled part of the progress bar in pixels</param>
/// <param name="alt">the first or the second bar</param>
void LCDProgressBar::CreateCustomChars(int val, bool alt)
{
    uint8_t mask;
    mask = (val < charCols) ? createFillMask(val) : 0xff;
    CreateCustomCharacter(alt ? pbLeft2 : pbLeft, mask, VFRAME_L, HFRAME_L);
    mask = createFillMask(val % charCols);
    CreateCustomCharacter(alt ? pbMiddle2 : pbMiddle, mask, 0, 0b11111);
    mask = (val > (width - 1) * charCols) ? createFillMask(val % charCols) : 0;
    CreateCustomCharacter(alt ? pbRight2 : pbRight, mask, VFRAME_R, HFRAME_R);
}

