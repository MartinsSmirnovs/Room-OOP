#include "Arduino.h"
#include "Animations.h"

SolidFade::SolidFade(int startDelayTime)
{
    _delayTime = startDelayTime * 100;
    name = "Solid Fade";
    unsigned long _oldTimeInt;
    byte _rgbPart[3];
}

void SolidFade::execute(byte rgbArr[3])
{
    unsigned long time = millis();
    for (byte i = 0; i < 3; i++)
    {
        rgbArr[i] = (_rgbPart[i] / 2) + (_rgbPart[i] / 2) * sin(2 * PI / (_delayTime * 100) * time);
    }
}

void SolidFade::firstTime(byte part[3])
{
    for (byte i = 0; i < 3; i++)
    {
        _rgbPart[i] = part[i];
    }
}