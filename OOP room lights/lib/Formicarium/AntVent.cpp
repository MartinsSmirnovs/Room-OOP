#include "Arduino.h"
#include "Formicarium.h"

AntVent::AntVent(byte pin, String name, int mode)
{
    _pin = pin;
    _name = name;
    _pwm = 255;
    _mode = 0;
    pinMode(pin, OUTPUT);
}

void AntVent::staticRun()
{
    if (_run == true)
    {
        analogWrite(_pin, _pwm);
    }
    else
    {
        analogWrite(_pin, 0);
    }
}

void AntVent::updateVal(int pwmToCheck)
{
    if (pwmToCheck > 255)
    {
        _pwm = 255;
    }
    else if (pwmToCheck < 0)
    {
        _pwm = 0;
    }
    else
    {
        _pwm = pwmToCheck;
    }
}

void AntVent::stop(bool type)
{
    _run = !type;
}

String AntVent::getName()
{
    return _name;
}

byte AntVent::getPwm()
{
    return _pwm;
}

int AntVent::getMode()
{
    return _mode;
}