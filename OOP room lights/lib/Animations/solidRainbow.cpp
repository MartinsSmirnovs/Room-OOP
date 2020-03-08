#include "Arduino.h"
#include "Animations.h"

SolidRainbow::SolidRainbow(int startDelayTime)
{
    _delayTime = startDelayTime;
    name = "Solid Rainbow";
    byte _decColor;
    byte _incColor;
    byte _rainbowCounter;
    unsigned long _oldTimeInt;
}

void SolidRainbow::firstTime(byte rgbArr[3])
{
    rgbArr[0], rgbArr[2], _decColor, _rainbowCounter = 0;
    rgbArr[1] = 255;
}

void SolidRainbow::execute(byte rgbArr[3])
{
    if (millis() - _oldTimeInt >= _delayTime)
    {
        if (_rainbowCounter == 0)
        {
            if (_decColor < 3)
            {
                _decColor++;
            }
            if (_decColor == 3)
            {
                _decColor = 0;
                _incColor = 1;
            }
            if (_decColor == 2)
            {
                _incColor = 0;
            }
            if (_decColor == 1)
            {
                _incColor = 2;
            }
        }

        _rainbowCounter++;

        if (_rainbowCounter != 0)
        {
            rgbArr[_decColor] -= 1;
            rgbArr[_incColor] += 1;
        }
        _oldTimeInt = millis();
    }
}
