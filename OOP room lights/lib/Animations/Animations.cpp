#include "Arduino.h"
#include "Animations.h"

Animation::Animation()
{
    String name;
    int _delayTime;
    unsigned long _oldTimeInt;
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

