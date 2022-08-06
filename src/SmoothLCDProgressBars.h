/*
SmoothLCDProgressBars library v 1.0

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

(c) Dejan Gjorgjevikj, 8/2022
*/

#ifndef SMOOTH_LCD_PROGRESS_BARS_H
#define SMOOTH_LCD_PROGRESS_BARS_H

#include <Arduino.h>
//#include <avr/pgmspace.h>

#if defined(LiquidCrystal_I2C_h) 
#define LCD_OBJ LiquidCrystal_I2C
// init();
#elif defined(FDB_LIQUID_CRYSTAL_I2C_H)
#define LCD_OBJ LiquidCrystal_I2C
// begin();
#elif defined LiquidCrystal_h 
#define LCD_OBJ LiquidCrystal
#elif
#define LCD_OBJ UNDEFINED
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
/*
template <unsigned int N>
struct Mask
{
    static unsigned int const value = (1 << (N + 1)) - 1;
};

template <>
struct Mask<0>
{
    static unsigned int const value = 0;
};
*/

template <const unsigned char N> const unsigned char Mask()
{
    return ((1 << N) - 1);
}

enum MemoryType { RAM = 0, FLASH = 1, DATA = 0, PROG = 1 };
enum BarOrientation : byte { Horizontal, Vertical };
//enum ByteMasks : byte { =3, =4, =5, =6, =7, =8};

struct barstyle
{
    enum dotMatrixDim : byte { CharPatRows = 8, CharPatCols = 5 };
//    enum bitMasks : byte { AllRows = Mask<CharPatRows>::value, AllCols = Mask<CharPatCols>::value };
//    enum bitMasks : byte { AllRows = Mask<CharPatRows>(), AllCols = Mask<CharPatCols>()};
    enum bitMasks : byte { AllRows = 0b11111111, AllCols = 0b00011111};
    enum andOrMaskIndex : byte { ANDmask, ORmask };
    uint8_t startMask[2][CharPatRows]; // Left/Bottom-AND+OR
    uint8_t endMask[2][CharPatRows]; // Right/Top-AND+OR
    uint8_t middleMask[2][CharPatRows]; // Middle-AND+OR 
    struct BarOffsets // where the filling of the prograss bar actually starts/ends considerng that some pixels are taken for the frame
    {
        unsigned char startOffset : 3;
        unsigned char endOffset : 3;
        unsigned char dir : 2;
    } offset;
};

template <class DISP = LCD_OBJ, MemoryType MT = FLASH>
class LCDDisplay;

// specializations
template <class DISP>
class LCDDisplay< DISP, RAM > 
{
public:
    enum UDCharCode: byte { ProgBarCharBlank = 0, ProgBarCharFull, ProgBarCharBegin, ProgBarCharEnd, ProgBarPartial };
    LCDDisplay(DISP& disp, const barstyle& bs) : display(&disp), barStyle(&bs) { }

    DISP* getDisplay() const
    {
        return display;
    }

    const barstyle& getStyle() const
    {
        return *barStyle;
    }

    int getStartOffset() const
    {
        return(barStyle->offset.startOffset);
    }

    int getEndOffset() const
    {
        return(barStyle->offset.endOffset);
    }
 
    void init() const
    {
        // create the blank piece in the middle 
        display->createChar(ProgBarCharBlank, const_cast<uint8_t *>(barStyle->middleMask[barstyle::ORmask]));
        // create the completely filled piece in the middle 
        makeChar(ProgBarCharFull, barStyle->middleMask);
        // create the full piece at the begining (left edge of the frame when filled, bottom on vertical progress bar)
        makeChar(ProgBarCharBegin, barStyle->startMask);
        // create the blank piece at the end (right edge of the frame when blank, top on vertical progress bar)
        display->createChar(ProgBarCharEnd, const_cast<uint8_t*>(barStyle->endMask[barstyle::ORmask])); // .getBitMask<barstyle::MiddleOR>())); //barStyle.bs->mORmask);
    }

    void makeChar(byte udChr, const uint8_t xMask[2][barstyle::CharPatRows], uint8_t mask = barstyle::AllCols) const
    {
        uint8_t t[barstyle::CharPatRows];
        for (byte i = 0; i < barstyle::CharPatRows; i++)
            t[i] = (mask & xMask[0][i]) | xMask[1][i];
        display->createChar(udChr, t);
    }

protected:
    DISP* display; // pointer to the display object
    const barstyle * barStyle; // pointer to the barstyle struct
};

template <class DISP>
class LCDDisplay< DISP, FLASH > 
{
public:
    enum CustomChar : byte { ProgBarCharBlank = 0, ProgBarCharFull, ProgBarCharBegin, ProgBarCharEnd, ProgBarPartial };
    LCDDisplay(DISP& disp, const barstyle& _hbs) : display(&disp), barStyle(&_hbs) { }

    DISP* getDisplay() const
    {
        return display;
    }

    const barstyle& getStyle() const
    {
        return *barStyle;
    }

    uint8_t getStartOffset() const
    {
        uint8_t m = pgm_read_byte(&(barStyle->offset)); 
        return (uint8_t)((reinterpret_cast<struct barstyle::BarOffsets*>(&m))->startOffset);
    }

    uint8_t getEndOffset() const
    {
        uint8_t m = pgm_read_byte(&(barStyle->offset)); 
        return (uint8_t)((reinterpret_cast<struct barstyle::BarOffsets*>(&m))->endOffset);
    }

    void init() const
    {
        // create the blank piece in the middle 
        createChar(ProgBarCharBlank, reinterpret_cast<PGM_VOID_P>(barStyle->middleMask[barstyle::ORmask]));
        // create the completely filled piece in the middle 
        makeChar(ProgBarCharFull, reinterpret_cast<PGM_VOID_P>(barStyle->middleMask));
        // create the full piece at the begining (left edge of the frame when filled, bottom on vertical progress bar)
        makeChar(ProgBarCharBegin, reinterpret_cast<PGM_VOID_P>(barStyle->startMask));
        // create the blank piece at the end (right edge of the frame when blank, top on vertical progress bar)
        createChar(ProgBarCharEnd, reinterpret_cast<PGM_VOID_P>(barStyle->endMask[barstyle::ORmask])); // .getBitMask<barstyle::MiddleOR>())); //barStyle.bs->mORmask);
    }

    void makeChar(byte udChr, PGM_VOID_P andorMask, uint8_t mask = barstyle::AllCols) const
    {
        uint8_t tao[2][barstyle::CharPatRows];
        memcpy_P(tao, andorMask, barstyle::CharPatRows<<1);
        for (byte i = 0; i < barstyle::CharPatRows; i++)
            tao[0][i] = (tao[0][i] & mask) | tao[1][i];
        display->createChar(udChr, tao[0]);
    }

    void makeCharV(byte udChr, PGM_VOID_P andorMask, uint8_t mask = barstyle::AllRows) const
    {
        uint8_t tao[2][barstyle::CharPatRows];
        memcpy_P(tao, andorMask, barstyle::CharPatRows << 1);
        for (byte i = 0; i < barstyle::CharPatRows; i++, mask>>=1)
        {
            if(mask & 1)
                tao[0][i] = tao[0][i] | tao[1][i];
            else
                tao[0][i] = tao[1][i];
        }
        display->createChar(udChr, tao[0]);
    }

    // replacement for direct call to createChar(chr, mask)
    void createChar(byte udChr, PGM_VOID_P bitMask) const
    {
        uint8_t ta[barstyle::CharPatRows];
        memcpy_P(ta, bitMask, barstyle::CharPatRows);
        display->createChar(udChr, ta);
    }
    
protected:
    DISP* display; // pointer to the display object
    const barstyle* barStyle;
};

class BarPos
{
public:
    BarPos(byte _width, byte _row, byte _col, byte _id = 0) 
        : par(ProgBarPar{ _width, _row, _col, _id }) { }

    void setLength(int _len) // in characters 
    {
        par.len = _len;
    }

    void setPosition(byte _row, byte _col)
    {
        par.row = _row;
        par.col = _col;
    }

protected:
    struct ProgBarPar
    {
        byte len : 6; // progress bar length in characters
        byte row : 2; // row position of the progress bar
        byte col : 6; // column position of the progress bar
        byte id  : 2; // progress bar number 0-3 (should be const)
    } par;
};


template <class DISP, BarOrientation = Horizontal >
class ProgressBar : public BarPos {};

template <class DISP >
class ProgressBar < DISP, Horizontal > : public BarPos
{
private:
    ProgressBar(byte _width, byte _row, byte _col, byte _pbn = 0)
        : BarPos(_width, _row, _col, _pbn) { }

public: 
    /// <summary>
    /// Creates ProgressBar object
    /// </summary>
    /// <typeparam name="DISP">the display on which it is shown</typeparam>
    /// <typeparam name="len">the length of the prograss bar in characters</typeparam>
    /// <typeparam name="row">position: the row of the start end of the progressbar</typeparam>
    /// <typeparam name="col">position: the column of the start end of the progressbar</typeparam>
    /// <typeparam name="id">the id of the progressbar (4 supported: 0-3)</typeparam>
    ProgressBar(DISP& disp, byte _width, byte _row, byte _col, byte _pbn = 0)
        : BarPos(_width, _row, _col, _pbn), displ(disp) { }
 
    unsigned int size() const
    {
        return par.len * barstyle::CharPatCols - displ.getStartOffset() - displ.getEndOffset();
    }

    void showProgress(int val)
    {
        //ckecking limits: showProg(min(val + displ.getStartOffset(), size()-1), par.id);
        showProg(val + displ.getStartOffset(), par.id);
    }

    void showProgressPct(int val) { showProgress((val * size()) / 100); }

protected:

    void showProg(int val, byte n)
    {
        int full, blank, partial;
        full = min(max(0, val / barstyle::CharPatCols - 1), par.len - 2);
        partial = (val > barstyle::CharPatCols && val < (par.len - 1)* barstyle::CharPatCols);
        blank = max(0, par.len - full - 2 - partial);

        uint8_t mask = (barstyle::AllCols << (barstyle::CharPatCols - (val % barstyle::CharPatCols))) & barstyle::AllCols;

        if (val < barstyle::CharPatCols) // still filling the first block, create and use the middle char
            displ.makeChar(DISP::ProgBarPartial + n, displ.getStyle().startMask, mask);
        if (partial)
            displ.makeChar(DISP::ProgBarPartial + n, displ.getStyle().middleMask, mask);
        if (val > (par.len - 1) * barstyle::CharPatCols) // last block partially (or fully) filled, create and use the middle char
            displ.makeChar(DISP::ProgBarPartial + n, displ.getStyle().endMask, mask);

        displ.getDisplay()->setCursor(par.col, par.row);

        displ.getDisplay()->write((val < barstyle::CharPatCols) ? (DISP::ProgBarPartial + n) : DISP::ProgBarCharBegin);
        while (full--)
        {
            displ.getDisplay()->write(DISP::ProgBarCharFull);
        }
        if (partial)
        {
            displ.getDisplay()->write(DISP::ProgBarPartial + n);
        }
        while (blank--)
        {
            displ.getDisplay()->write(DISP::ProgBarCharBlank);
        }

        if (val > (par.len - 1) * barstyle::CharPatCols) // last block partially (or fully) filled, create and use the middle char
        {
            displ.getDisplay()->write(DISP::ProgBarPartial + n);
        }
        else
        {
            displ.getDisplay()->write(DISP::ProgBarCharEnd);
        }
    }
//data
    const DISP & displ; // pointer to the display wraper object
};

template <class DISP >
class ProgressBar < DISP, Vertical > : public BarPos
{
private:
    ProgressBar(byte _width, byte _row, byte _col, byte _pbn = 0)
        : BarPos(_width, _row, _col, _pbn) { }

public:
    ProgressBar(DISP& disp, byte _width, byte _row, byte _col, byte _pbn = 0)
        : BarPos(_width, _row, _col, _pbn), displ(disp) { }

    unsigned int size() const
    {
        return par.len * barstyle::CharPatRows - displ.getStartOffset() - displ.getEndOffset();
    }

    void showProgress(int val)
    {
        //ckecking limits: showProg(min(val + displ.getStartOffset(), size()-1), par.id);
        showProg(val + displ.getStartOffset(), par.id);
    }

    void showProgressPct(int val) { showProgress((val * size()) / 100); }

protected:

    void showProg(int val, byte n)
    {
        int full, blank, partial;
        full = min(max(0, val / barstyle::CharPatRows - 1), par.len - 2);
        partial = (val > barstyle::CharPatRows && val < (par.len - 1)* barstyle::CharPatRows);
        blank = max(0, par.len - full - 2 - partial);

        uint8_t mask = (barstyle::AllRows << (barstyle::CharPatRows - (val % barstyle::CharPatRows)));

        if (val < barstyle::CharPatRows) // still filling the first block, create and use the middle char
            displ.makeCharV(DISP::ProgBarPartial + n, displ.getStyle().startMask, mask);
        if (partial)
            displ.makeCharV(DISP::ProgBarPartial + n, displ.getStyle().middleMask, mask);
        if (val > (par.len - 1) * barstyle::CharPatRows) // last block partially (or fully) filled, create and use the middle char
            displ.makeCharV(DISP::ProgBarPartial + n, displ.getStyle().endMask, mask);

        displ.getDisplay()->setCursor(par.col, par.row);

        byte crow = par.row;
        displ.getDisplay()->write((val < barstyle::CharPatRows) ? (DISP::ProgBarPartial + n) : DISP::ProgBarCharBegin);
        while (full--)
        {
            displ.getDisplay()->setCursor(par.col, --crow);
            displ.getDisplay()->write(DISP::ProgBarCharFull);
        }
        if (partial)
        {
            displ.getDisplay()->setCursor(par.col, --crow);
            displ.getDisplay()->write(DISP::ProgBarPartial + n);
        }
        while (blank--)
        {
            displ.getDisplay()->setCursor(par.col, --crow);
            displ.getDisplay()->write(DISP::ProgBarCharBlank);
        }

        displ.getDisplay()->setCursor(par.col, --crow);
        if (val > (par.len - 1) * barstyle::CharPatRows) // last block partially (or fully) filled, create and use the middle char
        {
            displ.getDisplay()->write(DISP::ProgBarPartial + n);
        }
        else
        {
            displ.getDisplay()->write(DISP::ProgBarCharEnd);
        }
    }
    //data
    const DISP& displ; // pointer to the display wraper object
};

typedef LCDDisplay < LCD_OBJ > LCD;
typedef ProgressBar < LCDDisplay< LCD_OBJ > > SmoothProgressBar;


/*
void dbgPrintM(uint8_t x[8], char *s)
{
    Serial.println(s);
    for (int i = 0; i < 8; i++)
    {
        for(uint8_t m = 0b10000; m; m>>=1)
        {
            Serial.print((x[i] & m) ? '1' : '0');
        }
        Serial.println();
    }
};
*/


#endif // SMOOTH_LCD_PROGRESS_BARS_H