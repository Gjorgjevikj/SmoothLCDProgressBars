/*
    Name:       TwoSmoothGauges.ino
    Created:	17.6.2022 23:03:07
    Author:     DESKTOP-I9\Dejan
*/

#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
#include <SmoothLCDProgressBars.h>  
//#define USE_PROGMEM
#include <BarStyle3.h>
#include <BarStyle1.h>

LiquidCrystal_I2C lcd(0x26, 16, 2);  // set the LCD address to 0x26 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x27, 20, 4);  // set the LCD address to 0x27 for a 20 chars and 4 line display
//LiquidCrystal_I2C lcd2(0x3f,16,2);    // set the LCD address to 0x3f for a 16 chars and 2 line display
// if you don't know the I2C address of the display, use I2C scanner first (https://playground.arduino.cc/Main/I2cScanner/)

LCDDisplay < LiquidCrystal_I2C > dispA(lcd, barStyle1);
LCDDisplay < LiquidCrystal_I2C, FLASH > dispB(lcd2, barStyle3);

ProgressBar< LCDDisplay< LiquidCrystal_I2C > > pb1(dispA, 10, 1, 0);
ProgressBar< LCDDisplay< LiquidCrystal_I2C > > pb2(dispA, 6, 0, 8, 1);
ProgressBar< LCDDisplay< LiquidCrystal_I2C, FLASH > > pb3(dispB, 16, 2, 2, 0);

unsigned int gauge1 = 0;       
unsigned int gauge2 = 0;
unsigned int gauge3 = 0;
char buffer[16];         // helper buffer to store C-style strings (generated with sprintf function)

void setup()
{
    Serial.begin(9600);
    delay(100);
    Serial.println("LCD smooth gauge DEMO begin");

#if defined(LiquidCrystal_I2C_h) 
    lcd.init();                       // initialize the 16x2 lcd module
    lcd2.init();                       // initialize the 16x2 lcd module

    Serial.println("lcd.init()");

#elif defined(FDB_LIQUID_CRYSTAL_I2C_H)
    lcd.begin();                       // initialize the 16x2 lcd module
    lcd2.begin();                       // initialize the 16x2 lcd module
#endif

    Serial.println("Text on lcd...");
    lcd.backlight();                  // enable backlight for the LCD module
    lcd.print("Two smooth gauges");

    lcd2.backlight();                  // enable backlight for the LCD module
    lcd2.print("Two smooth gauges");
    lcd2.print("  ...  on the second display");

    delay(2000);

    lcd.clear();
    lcd2.clear();

    dispA.init();
    dispB.init();

    Serial.print("dispA.lOff=");
    Serial.println(dispA.getStartOffset());
    Serial.print("dispA.rOff=");
    Serial.println(dispA.getEndOffset());
    Serial.print("dispB.lOff=");
    Serial.println(dispB.getStartOffset());
    Serial.print("dispB.rOff=");
    Serial.println(dispB.getEndOffset());

//    Serial.println(pb1.size());
//    Serial.println(pb2.size());
//    Serial.println(pb3.size());

    pb1.showProgressPct(100);
    pb2.showProgress(pb2.size());
    pb3.showProgress(pb3.size());

    delay(1500);
    lcd.clear();
}

void loop()
{
    lcd.setCursor(0, 0);                           // move cursor to top left
    sprintf(buffer, "val:%3d ", gauge2);  // set a string as val: XX% 
    lcd.print(buffer);                             // print the string on the display

    pb1.showProgressPct(gauge1);
    pb2.showProgress(gauge2);
    pb3.showProgress(gauge3);

    sprintf(buffer, "%3d%% ", gauge1);       // set a string as XX%, with the number always taking at least 3 character
    lcd.setCursor(12, 1);
    lcd.print(buffer);                            // print the string on the display

    sprintf(buffer, "val:%3d ", gauge3);  // set a string as val: XX% 
    lcd2.setCursor(1, 0);
    lcd2.print(buffer);                            // print the string on the display

    
    //pb3.showProgressPct(gauge % 100);
    //pb4.showProgress(gauge % pb4.size());

    if (
        gauge2 == 0
        || gauge2 == 1 
        || gauge2 == pb2.size() 
        || gauge2 == pb2.size() - 1
        || gauge2 == pb2.size() - 2
        || gauge1 < 5
        || gauge1 > 95)
        delay(500);  // wait for a while 

    gauge1++;
    gauge2++;
    gauge3++;

    if (gauge1 > 100)
        gauge1 = 0;
    if (gauge2 > pb2.size())
        gauge2 = 0;
    if (gauge3 > pb3.size())
        gauge3 = 0;

     //    if (gauge < 0) { gauge = 0; }

    delay(100);  // wait for a while 
    //lcd.clear();
}
