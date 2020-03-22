#include "Arduino.h"
#include "Animations.h"
#include "Adafruit_NeoPixel.h"

Rainbow::Rainbow(int startDelayTime, byte numLeds, Adafruit_NeoPixel *stripObj)
{
    name = "Rainbow";
    _delayTime = startDelayTime;
    _numLeds = numLeds;
    _rainbowCounter = 0;

    _strip = stripObj;
    _strip->clear(); //notīra iepriekšējās krāsas no krāsu lentas
    _strip->show();  //apdeito krāsu lentu, lai iepriekšējās krāsas tiktu notīrītas
}

void Rainbow::execute(byte rgbArr[3])
{
    if (millis() - _oldTimeInt >= _delayTime)
    {
        _oldTimeInt = millis();
        byte *c;
        uint16_t i, j;

        for (i = 0; i < _numLeds; i++)
        {
            c = rainbowWheel(((i * 256 /_numLeds) + _rainbowCounter) & 255);
            _strip->setPixelColor(i, *c, *(c + 1), *(c + 2));
        }
        _rainbowCounter++;
        _strip->show();
    }
}

byte *Rainbow::rainbowWheel(byte WheelPos)
{
    static byte c[3];
    if (WheelPos < 85)
    {
        c[0] = WheelPos * 3;
        c[1] = 255 - WheelPos * 3;
        c[2] = 0;
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        c[0] = 255 - WheelPos * 3;
        c[1] = 0;
        c[2] = WheelPos * 3;
    }
    else
    {
        WheelPos -= 170;
        c[0] = 0;
        c[1] = WheelPos * 3;
        c[2] = 255 - WheelPos * 3;
    }
    return c;
}

void Rainbow::firstTime(byte part[3])
{
}
