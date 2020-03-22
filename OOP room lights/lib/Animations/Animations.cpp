#include "Arduino.h"
#include "Animations.h"

Animation::Animation()
{
    name = "";
    _delayTime = 0;
    _oldTimeInt = 0;
}

void Animation::setDelay(int setDelayTime)
{
    if (setDelayTime >= 0)
    {
        _delayTime = setDelayTime;
    }
}

unsigned long Animation::getTime()
{
    return _oldTimeInt;
}

int Animation::getDelay()
{
    return _delayTime;
}

