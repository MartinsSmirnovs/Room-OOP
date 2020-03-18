#ifndef animations
#define animations
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

class Animation
{
public:
    Animation();                              //class constructor
    String name;                              //name of objecy
    void setDelay(int setDelayTime);          //sets delay for object
    unsigned long getTime();                  //returns oldTimeInt of object
    int getDelay();                           //returns current delayTime of object
    virtual void execute(byte rgbArr[3]) = 0; //executes
    virtual void firstTime(byte part[3]) = 0; //prepares for execution
    virtual ~Animation(){};                   //class destructor
protected:
    int _delayTime;
    unsigned long _oldTimeInt;
};

class SolidFade : public Animation
{ //inherited solid fade class
public:
    SolidFade(int startDelayTime);
    SolidFade(){};
    void execute(byte rgbArr[3]); //executes solid fade function
    void firstTime(byte part[3]); //prepares solid fade for execution
private:
    byte _rgbPart[3]; //stores values to fade
};

class SolidRainbow : public Animation
{ //inherited solid rainbow class
public:
    SolidRainbow(int startDelayTime);
    // SolidRainbow(){};
    void execute(byte rgbArr[3]);
    void firstTime(byte part[3]); //if there were an animation other than solid rainbow running before, then this should be executed to start new cycle
private:
    byte _decColor;       //decreasing color of rainbow cycle
    byte _incColor;       //increasing color of rainbow cycle
    byte _rainbowCounter; //rainbow cycle counter
};

class SolidBlink : public Animation
{
public:
    SolidBlink(int startDelayTime);
    void execute(byte rgbArr[3]);
    void firstTime(byte part[3]);

private:
    byte _rgbPart[3];
    bool _onOffState; //saves last state of blink
};

class Fire : public Animation
{
public:
    Fire(int startDelayTime, byte numLeds, Adafruit_NeoPixel &stripObj);
    void execute(byte rgbArr[3]);
    void firstTime(byte part[3]);

private:
    byte _numLeds;
    Adafruit_NeoPixel _strip;
};

class Rainbow : public Animation
{
public:
    Rainbow(int startDelayTime, byte numLeds, Adafruit_NeoPixel &stripObj);
    void execute(byte rgbArr[3]);
    void firstTime(byte part[3]);

private:
    byte * rainbowWheel(byte WheelPos);
    byte _numLeds;
    Adafruit_NeoPixel _strip;
    int _rainbowCounter;
};

#endif