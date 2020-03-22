#include "Arduino.h"
#include "Animations.h"

SolidBlink::SolidBlink(int startDelayTime)
{
    _delayTime = startDelayTime;
    name = "Solid Blink";
    _oldTimeInt = 0;
    for(byte i = 0; i<3; i++)
        _rgbPart[i] = 0;
    _onOffState = false;
}

void SolidBlink::firstTime(byte part[3])
{
    for (byte i = 0; i < 3; i++)
    {
        _rgbPart[i] = part[i];
    }
}

void SolidBlink::execute(byte rgbArr[3])
{
    if (millis() - _oldTimeInt >= _delayTime*10)
    {
        _oldTimeInt = millis();
        if (_onOffState)
        {
            for (byte i = 0; i < 3; i++)
            {
                rgbArr[i] = _rgbPart[i];
            }
        }
        else
        {
            for (byte i = 0; i < 3; i++)
            {
                rgbArr[i] = 0;
            }
        }
        _onOffState = !_onOffState;
    }
}