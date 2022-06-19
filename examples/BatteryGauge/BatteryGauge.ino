/*
    Name:       BatteryGauge.ino
    Created:	18.6.2022 11:31:51
    Author:     DESKTOP-I9\Dejan
*/


#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
#include <yaLCDBatteryGauge.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3f,16,2);    // set the LCD address to 0x3f for a 16 chars and 2 line display
// if you don't know the I2C address of the display, use I2C scanner first (https://playground.arduino.cc/Main/I2cScanner/)

LCDGauge bat(lcd, 6, 0, 0);

//unsigned int gauge = 0;
char buffer[16];         // helper buffer to store C-style strings (generated with sprintf function)

void setup()
{
    lcd.init();                       // initialize the 16x2 lcd module
    lcd.backlight();                  // enable backlight for the LCD module
    lcd.print("Battery gauge...");
    delay(1000);
    bat.init();
    lcd.clear();
    
}

void chargingAnimation(int state, int r = 3)
{
    bat.showGaugePct(state);
    sprintf(buffer, "Charging: %3d%%", state);
    lcd.setCursor(0, 0);
    lcd.print(buffer);                            // print the string on the display

    while (r)
    {
        int cs = state;
        // animate
        while (cs <= 100 && r > 0)
        {
            cs += 2;
            bat.showGaugePct(cs);
            delay(1);
        }
        r--;
        delay(200);
    }
    bat.showGaugePct(state);
}

void loop()
{
    lcd.clear();
    bat.setWidth(7);
    bat.setPosition(0, 0);
    for (int gauge = 0; gauge <= 100; gauge++)
    {
        bat.showGaugePct(gauge);
        sprintf(buffer, "%3d%%", gauge);
        lcd.setCursor(12, 1);
        lcd.print(buffer);                            // print the string on the display
        delay(10);  // wait for a while 
    }
    //bat.showGauge(gauge);
    //sprintf(buffer, "%3d", gauge);
    delay(500);  // wait for a while 
    lcd.clear();

    bat.setWidth(6);
    bat.setPosition(8, 1);
    for (int gauge = 20; gauge <= 80; gauge+=10)
    {
        chargingAnimation(gauge);
        //sprintf(buffer, "%3d%%", gauge);
        //lcd.setCursor(12, 1);
        //lcd.print(buffer);                            // print the string on the display
        delay(100);  // wait for a while 
    }

    //gauge++;
    //if (gauge > bat.size()+2) { gauge = 0; }
    //if (gauge > 100) { gauge = 0; }

    delay(1000);  // wait for a while 
}
