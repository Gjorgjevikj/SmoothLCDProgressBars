/*
SmoothLCDProgressBars library v 0.9

Smooth LCD progress bar (gauge) for the character based LCD displays for Arduino
Library for displaying progress bar on a character based LCD like Hitachi HD44780 16x2 or 20x4
Relies on the LiquidCrystal_I2C.h library (and should work on any platform that supports this library).
Uses the user definable characters of the LCD display and can dispalay up to 4 smoot gauges simultaneously.

Known minorities:
- no checking of the parameters (so the gauges can act unexpectedly if odd values are passed, like negative or very big numbers) for performance reasons.
- the borders drawn with the gauge are considered as part of the gauge (todo - offsets)
- at most 4 independent progress bars can be controled (restricted by the limitation of only 8 user definable characters on these displays)

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
//#include <avr/pgmspace.h>

#ifdef LiquidCrystal_I2C_h
#define LCD_OBJ LiquidCrystal_I2C
#elif defined LiquidCrystal_h 
#define LCD_OBJ LiquidCrystal
#endif

// utility functions ...
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

template <typename T1, typename T2>
struct offset_of_impl {
    static T2 object;
    static constexpr size_t offset(T1 T2::* member) {
        return size_t(&(offset_of_impl<T1, T2>::object.*member)) -
            size_t(&offset_of_impl<T1, T2>::object);
    }
};
template <typename T1, typename T2>
T2 offset_of_impl<T1, T2>::object;

template <typename T1, typename T2>
inline constexpr size_t offset_of(T1 T2::* member) {
    return offset_of_impl<T1, T2>::offset(member);
}

/// <summary>
/// LCDProgressBar class
/// </summary>
class LCDProgressBar
{
public:
    enum PixelMatrix : byte { charRows = 8, charCols = 5 };
    enum CustomChar : byte { pbBlank = 0, pbLeft, pbRight, pbFull, pbMiddle };
    struct BarStyle
    {
        uint8_t lANDmask[8];
        uint8_t lORmask[8];
        uint8_t rANDmask[8];
        uint8_t rORmask[8];
        uint8_t mANDmask[8];
        uint8_t mORmask[8];
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
//        : width(_width), row(_row), col(_col), pbn(_pbn)
    {
        par.width = _width;
        par.row = _row;
        par.col = _col;
        par.pbn = _pbn;
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
    
    static void setStyle(BarStyle & _bs) // no const & because createChar is declared as void createChar(uint8_t, uint8_t[]) and not as void createChar(uint8_t, const uint8_t[]) as it should
    {
        bs.bsPtr = &_bs;
        masksInFlash = false;
    }

    static void setStyle(PGM_VOID_P _bsp)
    {
        bs.bspPtr = _bsp;
        masksInFlash = true;
    }

    /// <summary>
    /// The size of the pogress bar in pixels
    /// </summary>
    /// <returns>the width of the progress bar in pixels</returns>
    int size() const
    {
        return par.width * charCols;
    }

    /// <summary>
    /// Sets the width of the progress bar in characters
    /// </summary>
    /// <param name="w">the width of the progress bar in characters</param>
    void setWidth(int w) // in characters 
    {
        par.width = w;
    }

    /// <summary>
    /// Sets the position of the progress bar
    /// </summary>
    /// <param name="r">row position</param>
    /// <param name="c">column position</param>
    void setPosition(byte r, byte c)
    {
        par.row = r;
        par.col = c;
    }

    // to be moved in protected
    void prepChar(byte udChr, const uint8_t* orMask, const uint8_t* andMask, uint8_t mask = 0b11111)
    {
        uint8_t t[charRows];
        for (byte i = 0; i < charRows; i++)
            t[i] = (mask & andMask[i]) | orMask[i];
        display->createChar(udChr, t);
    }

    // for masks in PROGMEM
    void prepChar(byte udChr, const char * orMask, const char * andMask, uint8_t mask = 0b11111)
    {
        uint8_t ta[charRows];
        uint8_t to[charRows];
        memcpy_P(ta, andMask, charRows);
        memcpy_P(to, orMask, charRows);
        for (byte i = 0; i < charRows; i++)
            ta[i] = (ta[i] & mask) | to[i];
        display->createChar(udChr, ta);
    }

    /// <summary>
    /// Initializes the progress bar object
    /// </summary>
    void init()
    {
        if (masksInFlash)
        {
            uint8_t t[charRows];
            // create the empty piece in the middle 
            memcpy_P(t, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::mORmask), charRows);
            display->createChar(pbBlank, t);
            // create the completely filled piece in the middle 
            prepChar(pbFull, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::mORmask), ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::mANDmask));
            // create the full piece on the left (left edge of the frame when filled)
            prepChar(pbLeft, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::lORmask), ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::lANDmask));
            // create the blank piece on the right (right edge of the frame when blank)
            memcpy_P(t, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::rORmask), charRows);
            display->createChar(pbRight, t);
        }
        else
        {
            // create the empty piece in the middle 
            display->createChar(pbBlank, bs.bsPtr->mORmask);
            // create the completely filled piece in the middle 
            prepChar(pbFull, bs.bsPtr->mORmask, bs.bsPtr->mANDmask);
            // create the full piece on the left (left edge of the frame when filled)
            prepChar(pbLeft, bs.bsPtr->lORmask, bs.bsPtr->lANDmask);
            // create the blank piece on the right (right edge of the frame when blank)
            display->createChar(pbRight, bs.bsPtr->rORmask);
        }
    }

    /// <summary>
    /// Draws the progress bar for the given value 
    /// </summary>
    /// <param name="val">the filled pard of the progress bar in pixels (0-size())</param>
    void showProgress(int val)
    {
        showProg(val, par.pbn);
    }

    /// <summary>
    /// Draws the progress bar for the given value 
    /// </summary>
    /// <param name="val">the filled part of the progress bar in percent (0-100)</param>
    void showProgressPct(int val)
    {
        showProg(val * (par.width * charCols) / 100, par.pbn);
    }

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
        full = min(max(0, val / charCols - 1), par.width - 2);
        partial = (val > charCols && val < (par.width - 1)* charCols);
        blank = max(0, par.width - full - 2 - partial);

        uint8_t mask = (0b11111 << (charCols - (val % charCols))) & 0b11111;
        
        if (masksInFlash)
        {
            if (val < charCols) // still filling the first block, create and use the middle char
                prepChar(pbMiddle + n, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::lORmask), ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::lANDmask), mask);
            if (partial)
                prepChar(pbMiddle + n, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::mORmask), ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::mANDmask), mask);
            if (val > (par.width - 1) * charCols) // last block partially (or fully) filled, create and use the middle char
                prepChar(pbMiddle + n, ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::rORmask), ((const char*)bs.bspPtr) + offset_of(&LCDProgressBar::BarStyle::rANDmask), mask);
        }
        else
        {
            if (val < charCols) // still filling the first block, create and use the middle char
                prepChar(pbMiddle + n, bs.bsPtr->lORmask, bs.bsPtr->lANDmask, mask);
            if (partial)
                prepChar(pbMiddle + n, bs.bsPtr->mORmask, bs.bsPtr->mANDmask, mask);
            if (val > (par.width - 1) * charCols) // last block partially (or fully) filled, create and use the middle char
                prepChar(pbMiddle + n, bs.bsPtr->rORmask, bs.bsPtr->rANDmask, mask);
        }

        display->setCursor(par.col, par.row);

        display->write((val < charCols) ? (pbMiddle + n) : pbLeft);
        while (full--)
        {
            display->write(pbFull);
        }
        if (partial)
        {
            display->write(pbMiddle + n);
        }
        while (blank--)
        {
            display->write(pbBlank);
        }
        
        if (val > (par.width - 1) * charCols) // last block partially (or fully) filled, create and use the middle char
        {
            display->write(pbMiddle + n);
        }
        else
        {
            display->write(pbRight);
        }
 
    }

    // Data    
    static LCD_OBJ * display; // refference to the display object
    static union barStylePtr {
        BarStyle* bsPtr;
        PGM_VOID_P bspPtr;
    } bs;
    static bool masksInFlash;
    struct ProgBarPar
    {
        byte width : 5; // progress bar width in characters
        byte row : 3; // row position of the progress bar
        byte col : 5; // column position of the progress bar
        byte pbn : 2; // progress bar number 0-4 (should be const)
    } par;
};

LCDProgressBar::barStylePtr LCDProgressBar::bs = { NULL };
LCD_OBJ* LCDProgressBar::display = NULL;
bool LCDProgressBar::masksInFlash = false;

#endif