# SmoothLCDProgressBars

Library for siplaying smooth progress bars on the character based LCD displays for Arduino

Relies on the LiquidCrystal_I2C.h or LiquidCrystal.h library (and should work on any platform that supports this library). Uses the user definable characters of the LCD display and can display up to 2 smoot gauges simultaneously.

![ProgressBar0](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/SmootProgressBar_0_16x2.png) ![ProgressBar1](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/SmootProgressBar_1_16x2.png)

![Battery6](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/BatteryGauge_6_16x2.png) ![Battery7](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/BatteryGauge_7_16x2.png)

![ProgressBar3](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/SmootProgressBar_1_20x4.png) ![Battery8](https://github.com/Gjorgjevikj/yaLCDProgressBar/blob/main/extras/BatteryGauge_7_20x4.png)

Two libraries 
- yaLCDProgressBar.h : The length of the progress bar can be configured as well as the position on the LCD display. The state of the progress bar can be set as a percentage or absolute value (pixels). Supports 3 differently looking progress bar types (configure in yaLCDpbConf.h).
  - 0 : Progress bar drawn using the whole height of the character (8 pixels) and rounded corners
  - 1 : Thinner progress bar that is only 6 pixels high with square edges

- yaLCDBatteryGauge.h : Progress bar for battery charge status

Known minorities:
- no checking of the parameters (so the gauges can act unexpectedly if odd values are passed, like negative or very big numbers) for performance reasons.
- the borders drawn with the gauge are considered as part of the gauge (except for the battery gauge that is drawn in a different way) 

So here it is, hopefully someone else can find a use of it (or make a comment).
For explanation on the usage see the comments before the functions and look at the examples.
You can also see the execution of the examples on the [wokwi](https://wokwi.com/) emulator on-line ([TwoSmoothGauges.ino](https://wokwi.com/projects/334758694868746834) and [TwoSmoothGauges20x4.ino](https://wokwi.com/projects/334841453200015954) or see extras).

Usage:

Configuring is performed by editing the yaLCDpbConf.h file.
Please include (uncomment) the appropriate library (either LiquidCrystal_I2C.h or LiquidCrystal.h) in the yaLCDpbConf.h
It should also work with other libraries for the character LCD displays based on the Hitachi HD44780 or compatible controllers and support write(uint8_t) and createChar(uint8_t, uint8_t[]) with minor changes in yaLCDpbConf.h (define LCD_OBJ to be the name of the LCD class). The type of the Progress bar is also configured here.

