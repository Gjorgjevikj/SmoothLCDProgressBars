/*
    Name:       TwoSmoothGauges.ino
    Created:	17.6.2022 23:03:07
    Author:     DESKTOP-I9\Dejan
*/

#include <LiquidCrystal_I2C.h>     // if you don't have I2C version of the display, use LiquidCrystal.h library instead
#include <SmoothLCDProgressBars.h>  

#include <BarStyleBAT.h>
#include <BarStyle1.h>

#define BAR_STYLE_V_0_IN_PROGMEM
#include <BarStyleV0.h>
#define BAR_STYLE_BAT_V_IN_PROGMEM
#include <BarStyleBatV.h>

LiquidCrystal_I2C lcd(0x26, 16, 2);  // set the LCD address to 0x26 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x27, 20, 4);  // set the LCD address to 0x27 for a 20 chars and 4 line display
//LiquidCrystal_I2C lcd2(0x3f,16,2);    // set the LCD address to 0x3f for a 16 chars and 2 line display
// if you don't know the I2C address of the display, use I2C scanner first (https://playground.arduino.cc/Main/I2cScanner/)

#define PLACE1(x) (x)
#define PLACE2(x) inPROGMEM(x)

//LCD dispA(lcd, barStyle1);
//BarDisplay < LiquidCrystal_I2C, S1 > dispA(lcd, barStyle0);
//BarDisplay < LiquidCrystal_I2C, S2 > dispB(lcd2, barStyleV4);
//BarDisplay < LiquidCrystal_I2C, FLASH > dispA(lcd, barStyle0);
//BarDisplay < LiquidCrystal_I2C, RAM > dispB(lcd2, barStyleV4);

BarDisplay < LiquidCrystal_I2C > dispA(lcd, PLACE1(barStyleBat));
BarDisplay < LiquidCrystal_I2C > dispB(lcd2, PLACE2(barStyleBatV));

ProgressBar< BarDisplay< LiquidCrystal_I2C > > pb1(dispA, 10, 1, 0);
ProgressBar< BarDisplay< LiquidCrystal_I2C > > pb2(dispA, 6, 0, 8, 1);
ProgressBar< BarDisplay< LiquidCrystal_I2C > > pb3(dispB, 2, 2, 2, 0);
ProgressBar< BarDisplay< LiquidCrystal_I2C > > pb4(dispB, 4, 3, 18, 1);


//SmoothProgressBar pb1(dispA, 10, 1, 0);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, S1 > > pb1(dispA, 10, 1, 0);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, S1 > > pb2(dispA, 6, 0, 8, 1);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, S2 > > pb3(dispB, 2, 2, 2, 0);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, S2 > > pb4(dispB, 4, 3, 18, 1);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, FLASH > > pb1(dispA, 10, 1, 0);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, FLASH > > pb2(dispA, 6, 0, 8, 1);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, RAM > > pb3(dispB, 2, 2, 2, 0);
//ProgressBar< BarDisplay< LiquidCrystal_I2C, RAM > > pb4(dispB, 4, 3, 18, 1);

unsigned int gauge1 = 0;       
unsigned int gauge2 = 0;
unsigned int gauge3 = 0;
unsigned int gauge4 = 0;
char buffer[16];         // helper buffer to store C-style strings (generated with sprintf function)

/*
class __FlashBarStyleHelper;
#define FS(style) (reinterpret_cast<const __FlashBarStyleHelper *>(style))
//#define FSO(style) (static_cast<const __FlashBarStyleHelper>(style))

void identifyStyle(const barstyle* bs)
{
    Serial.println("barstyle * bs");
    Serial.println(bs->stylePar.startOffset);
}

void identifyStyle(const __FlashBarStyleHelper * bs)
{
    Serial.println("__FlashBarStyleHelper * bs");
    //(reinterpret_cast<const barstyle*>bs)
    uint8_t m = pgm_read_byte(&((reinterpret_cast<const barstyle*>(bs))->stylePar));
    int so = (uint8_t)((reinterpret_cast<struct barstyle::bspar*>(&m))->startOffset);
    Serial.println(so);
}

void idSt(const barstyle & bs)
{
    Serial.println("barstyle & bs");
    Serial.println(bs.stylePar.startOffset);
}

void idSt(const inPROGMEM & wraped)
{
    Serial.println("inPROGMEM & wraped");
    const barstyle* bs = wraped.unwrap();
    uint8_t m = pgm_read_byte(&(bs->stylePar));
    int so = (uint8_t)((reinterpret_cast<struct barstyle::bspar*>(&m))->startOffset);
    Serial.println(so);
}
*/

void showProgress1(int val)
{
    lcd.setCursor(0, 0);                           // move cursor to top left
    sprintf(buffer, "val:%3d ", val);  // set a string as val: XX% 
    lcd.print(buffer);                             // print the string on the display

    pb2.showProgress(val);
}

void setup()
{
    Serial.begin(9600);
    delay(100);
    Serial.println("LCD smooth gauge DEMO begin");

#if defined(LiquidCrystal_I2C_h) 
    lcd.init();                       // initialize the 16x2 lcd module
    lcd2.init();                       // initialize the 16x2 lcd module

    Serial.println("lcd.begin()");

#elif defined(FDB_LIQUID_CRYSTAL_I2C_H)
    lcd.begin();                       // initialize the 16x2 lcd module
    lcd2.begin();                       // initialize the 16x2 lcd module
#endif

    Serial.println("Text on lcd...");
    lcd.backlight();                  // enable backlight for the LCD module
    lcd.print("2 smooth gauges");

    lcd2.backlight();                  // enable backlight for the LCD module
    lcd2.print("2 smooth gauges");
    lcd2.print("  ...  on the second display");

    delay(1000);

    lcd.clear();
    lcd2.clear();

    dispA.begin();
    dispB.begin();

    Serial.print("dispA.lOff=");
    Serial.println(dispA.styleStartOffset());
    Serial.print("dispA.rOff=");
    Serial.println(dispA.styleEndOffset());
    Serial.print("dispA...dir=");
    Serial.println(dispA.styleOrientation());

    Serial.print("dispB.lOff=");
    Serial.println(dispB.styleStartOffset());
    Serial.print("dispB.rOff=");
    Serial.println(dispB.styleEndOffset());
    Serial.print("dispB...dir=");
    Serial.println(dispB.styleOrientation());

//    identifyStyle(&barStyleV1);
//    identifyStyle(FS(&barStyle0));

//    idSt(barStyleV1);
//    idSt(inPROGMEM(barStyle0));
    
//    Serial.println(pb1.size());
//    Serial.println(pb2.size());
//    Serial.println(pb3.size());

    /*
    pb1.showProgressPct(0);
    pb2.showProgress(0);
    pb3.showProgress(0);
    pb4.showProgress(0);
    delay(1000);

    pb1.showProgressPct(100);
    pb2.showProgress(pb2.size());
    pb3.showProgress(pb3.size());
    pb4.showProgress(pb4.size());
    delay(1500);
    */


    /*
    lcd.clear();
    lcd2.clear();
    lcd.print("Timing...");
    lcd2.print("Timing...");
    unsigned long t0,t1,t2;

    t0 = micros();
    for (int i = 0, e = pb1.size(); i < e; i++)
        pb1.showProgress(i);
    t1 = micros();

    Serial.print("th=");
    Serial.println(t1-t0);

    t0 = micros();
    for (int i = 0, e = pb4.size(); i < e; i++)
        pb4.showProgress(i);
    t1 = micros();

    Serial.print("tv=");
    Serial.println(t1 - t0);

    delay(500);
    
    lcd.clear();

   
    Serial.print("size=");
    Serial.println(pb2.size());
    showProgress1(0);
    delay(2500);
    showProgress1(1);
    delay(2500);
    showProgress1(2);
    delay(2500);
    showProgress1(5);
    delay(2500);
    showProgress1(6);
    delay(2500);
    showProgress1(7);
    delay(2500); 
    showProgress1(22);
    delay(2500);
    showProgress1(23);
    delay(2500);
    showProgress1(pb4.size() - 3);
    delay(2500);
    showProgress1(pb4.size() - 2);
    delay(2500);
    showProgress1(pb4.size() - 1);
    delay(2500);
    showProgress1(pb4.size());
    delay(2500);

    

    for (int i = pb2.size() - 7; i <= pb2.size(); i++)
    {
        showProgress1(i);
        delay(500);
    }

    showProgress1(pb2.size() + 1);
    delay(2500);
    

    delay(500);

    while (1)     ;

    */
    lcd.clear();
    lcd2.clear();
    delay(500);


}

void loop()
{
    lcd.setCursor(0, 0);                           // move cursor to top left
    sprintf(buffer, "val:%3d ", gauge2);  // set a string as val: XX% 
    lcd.print(buffer);                             // print the string on the display

    pb1.showProgressPct(gauge1);
    pb2.showProgress(gauge2);
    pb3.showProgress(gauge3);
    pb4.showProgress(gauge4);

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

    if (gauge1 > 100)
        gauge1 = 0;
    gauge2++;
    if (gauge2 > pb2.size())
        gauge2 = 0;
    gauge3++;
    if (gauge3 > pb3.size())
        gauge3 = 0;
    gauge4++;
    if (gauge4 > pb4.size())
        gauge4 = 0;

    if (gauge1 == 30)
    {
        dispA.setStyle(PLACE1(barStyleBat));
        /*Serial.print("dispA.lOff=");
        Serial.println(dispA.styleStartOffset());
        Serial.print("dispA.rOff=");
        Serial.println(dispA.styleEndOffset());
        Serial.print("dispA...dir=");
        Serial.println(dispA.styleOrientation());*/
    }
    if (gauge1 == 60)
    {
        dispA.setStyle(PLACE1(barStyle1));
        /*Serial.print("dispA.lOff=");
        Serial.println(dispA.styleStartOffset());
        Serial.print("dispA.rOff=");
        Serial.println(dispA.styleEndOffset());
        Serial.print("dispA...dir=");
        Serial.println(dispA.styleOrientation());*/
    }

    
    if (gauge4 == 12)
    {
        dispB.setStyle(PLACE2(barStyleV0));
    }
    if (gauge4 == 25)
    {
        dispB.setStyle(PLACE2(barStyleBatV));
    }
    
    //    if (gauge < 0) { gauge = 0; }

    delay(100);  // wait for a while 
    //lcd.clear();
}
