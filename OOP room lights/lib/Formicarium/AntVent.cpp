#include "Arduino.h"
#include "Formicarium.h"

AntVent::AntVent(byte pin, String name, int mode)
{
    _pin = pin;
    _name = name;
    _pwm = 255;
    pinMode(pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _run = false;
}

void AntVent::staticRun()
{
    digitalWrite(_pin, _run);
}

void AntVent::updateVal(int pwmToCheck)
{
    if (pwmToCheck > 255)
        _pwm = 255;
    else if (pwmToCheck < 0)
        _pwm = 0;
    else
        _pwm = pwmToCheck;
}

void AntVent::stop()
{
    _run = !_run;
}

String AntVent::getState(){
    if(_run) return "Turn OFF";
    else return "Turn ON";
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

bool AntVent::getRun(){
    return _run;
}