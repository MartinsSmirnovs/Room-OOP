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
    digitalWrite(_pin, LOW);
    _run = false;
}

void AntLed::setSpeed(int speed)
{
    _speed = speed;
}

void AntLed::run()
{
    switch (_mode)
    {
    case 1:
        smoothStart();
        break;
    case 2:
        fade();
        break;
    default:
        break;
    }
    analogWrite(_pin, _pwm);
}

void AntLed::smoothStart()
{
    if (millis() - _timeInt >= 10)
    {
        if (_run)
        {
            updateVal(_pwm + 10);
        }
        else
        {
            updateVal(_pwm - 10);
        }
        _timeInt = millis();
    }
    analogWrite(_pin, _pwm);
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
        int brightness = (255 / 2) + (255 / 2) * cos(angle); // Generates points on a sign wave.
        updateVal(brightness);
        _fadeCount++;
    }
}

unsigned long AntLed::getTimeInt()
{
    return _timeInt;
}

