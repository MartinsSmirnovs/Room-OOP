#include "Arduino.h"
#include "Formicarium.h"

AntLed::AntLed(byte pin, String name, int mode)
{
    _pin = pin;
    _name = name;
    _pwm = 0;
    _mode = mode;
    _timeInt = 0;
    _fadeCount = 0;
    pinMode(pin, OUTPUT);
}

void AntLed::setSpeed(int speed)
{
    _speed = speed;
}

void AntLed::run()
{
    switch (_mode)
    {
    case 0:
        // staticRun();
        break;
    case 1:
        smoothStart(true);
        break;
    case 2:
        smoothStart(false);
        break;
    case 3:
        fade();
        break;
    default:
        break;
    }
    staticRun();
}

void AntLed::smoothStart(bool type)
{
    if (millis() - _timeInt >= 10)
    {
        if (type)
        {
            updateVal(_pwm + 10);
        }
        else
        {
            updateVal(_pwm - 10);
        }
        _timeInt = millis();
    }
}

void AntLed::updateMode(int mode)
{
    _mode = mode;
}

void AntLed::fade()
{
    if (_fadeCount == 360)
    {
        _fadeCount = 0;
    }

    if (millis() - _timeInt >= _speed)
    {
        _timeInt = millis();
        float angle = radians(_fadeCount);                   // Converts degrees to radians.
        int brightness = (255 / 2) + (255 / 2) * sin(angle); // Generates points on a sign wave.
        updateVal(brightness);
        _fadeCount++;
    }
}

unsigned long AntLed::getTimeInt()
{
    return _timeInt;
}