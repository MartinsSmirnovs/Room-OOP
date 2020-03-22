# Room Lighting control

# Table of contents
- [Usage](https://github.com/Onii-Chaan/SGLS-Web-controller#usage)
- [Libraries used](https://github.com/Onii-Chaan/SGLS-Web-controller#libraries-used)
  - [Animations.h](https://github.com/Onii-Chaan/SGLS-Web-controller#animationsh)
  - [ButtonInput.h](https://github.com/Onii-Chaan/SGLS-Web-controller#buttoninputh)
  - [Formicarium.h](https://github.com/Onii-Chaan/SGLS-Web-controller#formicariumh)
  - [Led.h](https://github.com/Onii-Chaan/SGLS-Web-controller#ledh)
  - [Other libraries](https://github.com/Onii-Chaan/SGLS-Web-controller#other-libraries)
- [Main.cpp](https://github.com/Onii-Chaan/SGLS-Web-controller#maincpp)
- [Setup](https://github.com/Onii-Chaan/SGLS-Web-controller#setup)

# Usage
This code was made for Aduino UNO shield, that controls lighting in room. You can switch between colors and animations using OLED display, two buttons and potentiometer. Current code controls four main parts in the room, that are displayed on screen, they are:
- 12V RGB led strip
- 5V addressable led strip
- 12V white led strip
- 5V fan

Each of these parts have their submenus and submenu submenus. Each accessable using two input buttons

# Libraries used:
I've made 4 libraries for driving leds and fan. They all with a little to none modification can be used for other purposes.

## Animations.h
Animation is base class for all other animations. Each animation is different object with its own values.
- ```Default``` empty Animation class
- ```void firstTime()``` must be run once object is created, sets default values
- ```void execute()``` main animation method, makes all the color calculations
- ```int getDelay()``` returns current delay time
- ```unsigned long getTime();``` gets previous animation execution time in millis()
- ```void setDelay()``` sets delay for animation

Some animations can be run on addressable led strip only. In this case, Adafruit_NeoPixel object pointer location must be passed to object

## Input.h
For inputs from 3 buttons and potentiometer Input.h is used. ```PowerPin``` is not used. 
- ```void listen()``` main input method, must be placed in loop(), listens for all user inputs
- ```int getPot()``` returns potentiometer value 0 - 1023
- ```bool powerOn()``` returns if powerPin is low or high (not used in code)

There are 4 button input types
- ```topClick``` left button click
- ```botClick``` right button click
- ```doubleClick``` left and right pressed together (must be faster than left click or right click)
- ```doubleHold``` doubleClick hold for 1 sec

## Formicarium.h
I keep ants as a pet. Antkeeping nests are called formicariums. I made a library that controls 12V white led strip and 5V fan. Of course this library can be used for purposes, that are not ant keeping. This library runs PWM values on pins, either with or without fade animation. For Led strip, PWM turns ON and turns OFF slowly
- ```AntVent``` main object for controlling fan
- ```AntLed``` inherited object for controlling single color led strip
- ```void staticRun()``` digitalWrite on pit you have set
- ```void updateVal()``` sets new PWM value
- ```void stop()``` sets run bool to !run, so static run will stop or start
- ```String getName()``` returns name of object
- ```byte getPwm()``` returns current PWM value
- ```int getMode()``` returns current mode
- ```String getState()``` returns "turn on" or "turn off" string
- ```bool getRun()``` returns turn on or turn off bool value

AntLed has some additional method
- ```void smoothStart()``` slowly starts or stops PWM
- ```void run()``` main method, must be placed in loop(), runs user set modes
- ```void fade()``` runs fade
- ```updateMode(int mode)``` sets new mode
- ```void setSpeed()``` sets delay for animations
- ```unsigned long getTimeInt()``` returns last animation execution time in millis

## Led.h
For basic manipulations with Adressable led steip and basic RGB led strip, Led.h is needed.
- ```void execute()``` main method that drives led strips
- ```String state()``` returns "OFF" if is turned off or "ON" othervise
- ```stop()``` turns on or turns off strip, stops execution
- ```String getState()``` returns "Turn OFF" if is turned off or "Turn ON" othervise

## Other libraries
Apart from my libraries, other libraries were used. They are:
- ```Arduino.h``` if you are not using Arduino IDE
- ```Adafruit_NeoPixel.h```Adressable led strip driver
- ```MemoryFree.h``` Used this for checking used RAM, optional
- ```SSD1306Ascii.h``` Really good library for driving OLED displays and using the least RAM possible by displaying simple ASCII characters.
- ```SSD1306AsciiAvrI2c.h``` Second library used for driving OLED display

# Main.cpp
Controls OLED display and puts all libraries together. Makes 4 main menus, which you can access by clicking two buttons. Each menu has its own submenu. And most of submenus have their own submenus for setting parameters. Use potentiometer to scroll through parameter values. 
Each strip, fan, button, OLED work independently of each other, made as asynchronous as possible. 
After 300000 milliseconds, OLED displey goes dark to "sleep mode", you can wake it up by pressing one or another button.

# Setup
Here is the circuit I made for this code. 
- For stable readings from potentiometer, a small 100nF optional capacitor may be placed. Transistor values may vary depending on current flow through them. 
- I used two external power supplies, 5V and 12V
- For uploading and developing code I used PlatformIO
![Image of Yaktocat](https://i.ibb.co/jrKcgDb/Schematic-Room-light-v3-Sheet-1-20200322223808.png)



    
