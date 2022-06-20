/*
    Name:       TwoSmoothGauges.ino
    Created:	17.6.2022 23:03:07
    Author:     DESKTOP-I9\Dejan
*/

#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
#include <SmoothLCDProgressBars.h>     

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3f,16,2);    // set the LCD address to 0x3f for a 16 chars and 2 line display
// if you don't know the I2C address of the display, use I2C scanner first (https://playground.arduino.cc/Main/I2cScanner/)

LCDProgressBar pb1(12, 1, 0);
LCDProgressBar pb2(8, 0, 8, 1);

unsigned int gauge = 0;       
char buffer[16];         // helper buffer to store C-style strings (generated with sprintf function)

void setup()
{
    Serial.begin(9600);
    lcd.init();                       // initialize the 16x2 lcd module
    lcd.backlight();                  // enable backlight for the LCD module
    lcd.print("Two smoth gauges");
    delay(1000);
    LCDProgressBar::setDisplay(&lcd);
    pb1.init();
    pb2.init();
    lcd.clear();

    
    //pb2.showBlank();
    //pb2.showAllchars();
    //byte cc[] = { 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111 };
    //pb2.defShow(cc);

    //lcd.createChar(4, cc);
    //lcd.init();
    //lcd.setCursor(0, 0);
    //lcd.write(byte(4));

    delay(500);

}

void loop()
{
    lcd.setCursor(0, 0);                           // move cursor to top left
    sprintf(buffer, "val:%3d ", gauge % (8 * 5));  // set a string as val: XX% 
    lcd.print(buffer);                             // print the string on the display

    pb1.showProgressPct(gauge % 100);
    
    pb2.showProgress(gauge % pb2.size());

    sprintf(buffer, "%3d%% ", gauge % 100);       // set a string as XX%, with the number always taking at least 3 character
    lcd.setCursor(12, 1);
    lcd.print(buffer);                            // print the string on the display
    
    
    gauge++;
    if (gauge < 0) { gauge = 0; }

    delay(200);  // wait for a while 
    //lcd.clear();
}
