#ifndef led
#define led
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
class Led{
    public:
        Led(byte redPin, byte greenPin, byte bluePin, String objName);
        Led(){};
        String name;
        void execute();
        byte rgb[3]; //stores 12v led strip rgb values
        String state();
};

class AdrLed : public Led {
    public:
        AdrLed(String objName, Adafruit_NeoPixel & stripObj);
        Adafruit_NeoPixel strip;
};
#endif