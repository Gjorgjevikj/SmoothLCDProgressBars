// iclude the LiquidCrystal library here
// either LiquidCrystal_I2C.h
// or LiquidCrystal.h
// It should also work with other LCD libraries that inherit from Arduino Print library 
// and support write(uint8_t) and createChar(uint8_t, uint8_t[]) with minor changes 
// just define LCD_OBJ to be the name of the LCD class

#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
//#include <LiquidCrystal.h>

#ifdef LiquidCrystal_I2C_h
#define LCD_OBJ LiquidCrystal_I2C
#elif defined LiquidCrystal_h 
#define LCD_OBJ LiquidCrystal
#endif

// Progress bar type
// 0 - slightly rounded edges, full height (8 pixels)
// 1 - square edges, reduced hight (6 pixels, frame @6 pixels)
// 2 - experimental... filled part separated from frame

#define PB_TYPE 1

