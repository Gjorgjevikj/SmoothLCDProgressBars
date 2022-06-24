/*
SmoothLCDProgressBars library v 1.0.0

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

https://github.com/Gjorgjevikj/SmoothLCDProgressBars.git

(c) Dejan Gjorgjevikj, 2022
*/

#ifndef SmoothLCDProgressBars_H
#define SmoothLCDProgressBars_H

#include <Arduino.h>
#include <avr/pgmspace.h>


//#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
//#define LCD_OBJ LiquidCrystal_I2C

#ifdef LiquidCrystal_I2C_h
#define LCD_OBJ LiquidCrystal_I2C
#elif defined LiquidCrystal_h 
#define LCD_OBJ LiquidCrystal
#endif

//#include "BarStyles.h"


template <typename T> void PROGMEM_readAnything(const T* sce, T& dest)
{
    memcpy_P(&dest, sce, sizeof(T));
}

template <typename T> T PROGMEM_getAnything(const T* sce)
{
    static T temp;
    memcpy_P(&temp, sce, sizeof(T));
    return temp;
}

// number of items in an array
template< typename T, size_t N > size_t ArraySize(T(&)[N]) { return N; }


//template < class LCDLIB >
class LCDProgressBar
{
public:
    enum PixelMatrix : byte { charRows = 8, charCols = 5 };
    enum CustomChar : byte { pbBlank = 0, pbLeft, pbRight, pbFull, pbMiddle };
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


    /// <summary>
    /// LCDProgressBar Class (2 independent progress bars can be created)
    /// </summary>
    /// <param name="_width">width of the progress bar in characters</param>
    /// <param name="_row">row position of the progress bar</param>
    /// <param name="_col">column position of the progress bar</param>
    /// <param name="_pbn">progress bar number 0-3</param>
    LCDProgressBar(byte _width, byte _row, byte _col, byte _pbn = 0)
        : width(_width), row(_row), col(_col), pbn(_pbn)
    {
    }    

    /// <summary>
    /// set the LiquidCrystal object
    /// </summary>
    /// <param name="disp">the LCD created by LiquidCrystal</param>
    static void setDisplay(LCD_OBJ & disp)
//    static void setDisplay(LCD_OBJ* disp)
    {
        display = &disp;
    }
    
    static void setStyle(BarStyle & _bs)
    {
        bs = &_bs;
    }

    static void setStyleP(PGM_VOID_P _bsp)
    {
        bspPtr = _bsp;
    }

    /// <summary>
    /// Initializes the progress bar object
    /// </summary>
    void init()
    {
        // create the empty piece in the middle 
        uint8_t t[charRows] = { 0 };   
        for (byte i = 0; i < charRows; i++) // add the mask for the frame
            t[i] = bs->mORmask[i];
        display->createChar(byte(pbBlank), t);

        // create the complitely filled piece in the middle 
        for (byte i = 0; i < charRows; i++)
            t[i] = bs->mANDmask[i] | bs->mORmask[i];
        display->createChar(pbFull, t);

        // create the full piece on the left (left edge of the frame when filled)
        for (byte i = 0; i < charRows; i++)
            t[i] = bs->lANDmask[i] | bs->lORmask[i];
        display->createChar(pbLeft, t);

        // create the blank piece on the right (right edge of the frame when blank)
        for (byte i = 0; i < charRows; i++)
            t[i] = bs->rORmask[i];
        display->createChar(pbRight, t);
    }

    void initP()
    {
        // create the empty piece in the middle 
        uint8_t t[charRows] = { 0 };
        uint8_t t2[charRows] = { 0 };

        memcpy_P(t, ((const char *)bspPtr)+40, charRows);
//        for (byte i = 0; i < charRows; i++) // add the mask for the frame
//            t[i] = bs->mORmask[i];
        display->createChar(byte(pbBlank), t);

        // create the complitely filled piece in the middle 
        memcpy_P(t2, ((const char*)bspPtr) + 32, charRows);
        for (byte i = 0; i < charRows; i++)
            t[i] |= t2[i];
            //t[i] = bs->mANDmask[i] | bs->mORmask[i];
        display->createChar(pbFull, t);

        // create the full piece on the left (left edge of the frame when filled)
        memcpy_P(t, ((const char*)bspPtr), charRows);
        memcpy_P(t2, ((const char*)bspPtr) + 8, charRows);
        for (byte i = 0; i < charRows; i++)
            t[i] |= t2[i];
//            t[i] = bs->lANDmask[i] | bs->lORmask[i];
        display->createChar(pbLeft, t);

        // create the blank piece on the right (right edge of the frame when blank)
        memcpy_P(t, ((const char*)bspPtr + 24), charRows);
//        for (byte i = 0; i < charRows; i++)
//            t[i] = bs->rORmask[i];
        display->createChar(pbRight, t);
    }


    /// <summary>
    /// The size of the pogress bar in pixels
    /// </summary>
    /// <returns>the width of the progress bar in pixels</returns>
    int size() const
    {
        return width * charCols;
    }

    /// <summary>
    /// Sets the width of the progress bar in characters
    /// </summary>
    /// <param name="w">the width of the progress bar in characters</param>
    void setWidth(int w) // in characters 
    {
        width = w;
    }

    /// <summary>
    /// Sets the position of the progress bar
    /// </summary>
    /// <param name="r">row position</param>
    /// <param name="c">column position</param>
    void setPosition(byte r, byte c)
    {
        row = r;
        col = c;
    }

    /// <summary>
    /// Draws the progress bar for the given value 
    /// </summary>
    /// <param name="val">the filled pard of the progress bar in pixels (0-size())</param>
    void showProgress(int val)
    {
        showProg(val, pbn);
    }

    /// <summary>
    /// Draws the progress bar for the given value 
    /// </summary>
    /// <param name="val">the filled part of the progress bar in percent (0-100)</param>
    void showProgressPct(int val)
    {
        showProg(val * (width * charCols) / 100, pbn);
    }
    /*
    void showBlank(void)
    {
        int n = width;
        byte t[8];
        display->setCursor(col, row);
//        display->write(pbLeft);
        for (byte i = 0; i < charRows; i++)
            t[i] = bs->lORmask[i];
        display->createChar(pbMiddle, t);
        display->write(pbMiddle);

        n-=2;
        while(n--)
            display->write(pbBlank);
        display->write(pbRight);
    }

    void showAllchars(void)
    {
        display->setCursor(col, row);
        display->write(byte(pbBlank));
        display->write(pbFull);
        display->write(pbLeft);
        display->write(pbRight);
        //display->write(pbMiddle);
    }

    void defShow(byte t[])
    {
        ////Serial.print("pbMiddle=");
        ////Serial.println(pbMiddle);
        display->createChar(pbMiddle, t);
        display->setCursor(col, row);
        display->write(byte(pbMiddle));
    }
    */
protected:
    /// <summary>
    /// Draws the progress bar
    /// </summary>
    /// <param name="val">the filled part of the progress bar in pixels</param>
    /// <param name="n">0-3 which one of the 4 possible progress bars</param>
    void showProg(int val, byte n)
    {
        //CreateCustomChars(val, alt);
        int full, blank, partial;
        full = min(max(0, val / charCols - 1), width - 2);
        partial = (val > charCols && val < (width - 1)* charCols);
        blank = max(0, width - full - 2 - partial);

        //char buffer[16];
        //display->setCursor(0, 1);                           
        //sprintf(buffer, "%2d:%d+%d+%d w%2d", val, full, partial, blank, width);
        //display->setCursor(0, 1);
        //display->print(buffer);                            */
        //Serial.print(buffer);


        uint8_t mask; // = 0b11111;
        //mask <<= charCols - (val % charCols);
        mask = createFillMask(val % charCols);
        //Serial.print(mask,HEX);
        uint8_t t[charRows];

        if (val < charCols) // still filling the first block, create and use the middle char
        {
            for (byte i = 0; i < charRows; i++)
                t[i] = (mask & bs->lANDmask[i]) | bs->lORmask[i];
            display->createChar(uint8_t(pbMiddle + n), t);
        }
        if (partial)
        {
            for (byte i = 0; i < charRows; i++)
                t[i] = (mask & bs->mANDmask[i]) | bs->mORmask[i];
            display->createChar(pbMiddle + n, t);
        }
        if (val > (width - 1) * charCols) // last block partially (or fully) filled, create and use the middle char
        {
            for (byte i = 0; i < charRows; i++)
                t[i] = (mask & bs->rANDmask[i]) | bs->rORmask[i];
            display->createChar(pbMiddle + n, t);
        }

        display->setCursor(col, row);

        if (val < charCols) // still filling the first block, create and use the middle char
        {
            display->write(uint8_t(pbMiddle + n));
            //printUDchar(t);
            //Serial.print(" <");
        }
        else
        {
            display->write(pbLeft);
            //Serial.print(" [");
        }

        while (full--)
        {
            display->write(pbFull);
            //Serial.print("#");
        }
        if (partial)
        {
            display->write(uint8_t(pbMiddle + n));
            //Serial.print("|");
            //printUDchar(t);
        }
        while (blank--)
        {
            display->write(uint8_t(pbBlank));
            //Serial.print("_");
        }
        
        if (val > (width - 1) * charCols) // last block partially (or fully) filled, create and use the middle char
        {
            display->write(byte(pbMiddle + n));
            //Serial.print(">");
        }
        else
        {
            display->write(pbRight);
            //Serial.print("]");
        }
        //Serial.println();
    }

    
    /// <summary>
    /// Creates the bitmask representing how much the character is filled up left to right
    /// </summary>
    /// <param name="val">the value of the progress bar in pixels</param>
    /// <returns>the bitmask with ones on the left and zeroes on the left representing how much it is supposed to be filled</returns>
    uint8_t createFillMask(uint8_t val)
    {
        uint8_t bitmask = 1 << charCols; // 0b100000;
        uint8_t mask = 0;

        while (val--)
            mask |= (bitmask >>= 1);
        return mask;
    }

    void printUDchar(byte t[])
    {
        //Serial.println();
        for (byte i = 0; i < charRows; i++)
        {
            uint8_t mask = 0b10000;
            while (mask)
            {
                //Serial.print((t[i] & mask) ? '1' : '0');
                mask >>= 1;
            }
            //Serial.println();
        }
    }

    /*
    /// <summary>
    /// Creates the user definable character that is at the top of the progress bar
    /// </summary>
    /// <param name="chpos">which user definable character (0-7) to be used</param>
    /// <param name="mask">the bitmask to fill all rows of the character</param>
    /// <param name="vframe">the bitmask to be OR-ed with all the rows of the character (for drawing the frame)</param>
    /// <param name="hframe">the bitmask for the top and bottom row of the character (for drawing the frame)</param>
    void CreateCustomCharacter(uint8_t chpos, uint8_t mask, uint8_t vframe, uint8_t hframe)
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
        display->createChar(chpos, t);
    }

    /// <summary>
    /// Creates the user definable characters
    /// </summary>
    /// <param name="val">the filled part of the progress bar in pixels</param>
    /// <param name="alt">the first=0 or the second=1 bar</param>
    void CreateCustomChars(int val, byte alt)
    {
        uint8_t mask;
        mask = (val < charCols) ? createFillMask(val) : 0xff;
        CreateCustomCharacter(alt ? pbLeft2 : pbLeft, mask, VFRAME_L, HFRAME_L);
        mask = createFillMask(val % charCols);
        CreateCustomCharacter(alt ? pbMiddle2 : pbMiddle, mask, 0, 0b11111);
        mask = (val > (width - 1) * charCols) ? createFillMask(val % charCols) : 0;
        CreateCustomCharacter(alt ? pbRight2 : pbRight, mask, VFRAME_R, HFRAME_R);
    }
    */

    static LCD_OBJ * display; // refference to the display object
//    static union barStylePtr {
        static BarStyle* bs;
        static PGM_VOID_P bspPtr;
//    } bs;
    byte width; // progress bar width in characters
    byte row; // row position of the progress bar
    byte col; // column position of the progress bar
    byte pbn; // progress bar number 0-4
};

LCDProgressBar::BarStyle* LCDProgressBar::bs = NULL;
PGM_VOID_P LCDProgressBar::bspPtr = NULL;
LCD_OBJ* LCDProgressBar::display = NULL;

#endif