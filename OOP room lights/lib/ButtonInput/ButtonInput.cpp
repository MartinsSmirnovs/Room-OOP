#include "Arduino.h"
#include "ButtonInput.h"

Input::Input(byte topButtonPin, byte botButtonPin, byte potPin, byte powerPin, int inputTime)
{
    _topPin = topButtonPin;
    _botPin = botButtonPin;
    _potPin = potPin;
    _powerPin = powerPin;
    _startDoubleTime = 0;
    _startTopTime = 0;
    _startBotTime = 0;
    _inputTime = inputTime;
    botClick = false;
    topClick = false;
    doubleClick = false;
    pinMode(_topPin, INPUT);
    pinMode(_botPin, INPUT);
    pinMode(_potPin, INPUT);
    pinMode(_powerPin, INPUT);

    for (int i = 0; i < 10; i++)
        _prevPot[i] = 0;
}

void Input::listen()
{

    if (!digitalRead(_botPin))
    {
        _startBotTime = millis();
        botClick = false;
    }
    else if (digitalRead(_botPin) && millis() - _startBotTime > _inputTime)
    { //waits _inputTime seconds until button value gets saved
        botClick = true;
    }

    if (!digitalRead(_topPin))
    {
        _startTopTime = millis();
        topClick = false;
    }
    else if (digitalRead(_topPin) && millis() - _startTopTime > _inputTime)
    {
        topClick = true;
    }

    if (!(digitalRead(_topPin) && digitalRead(_botPin)))
    {
        _startDoubleTime = millis();
        doubleClick = false;
        doubleHold = false;
    }
    else if (digitalRead(_topPin) && digitalRead(_botPin) && millis() - _startDoubleTime > 1000)
    {
        doubleHold = true;
        doubleClick = false;
        topClick = false;
        botClick = false;
    }
    else if (digitalRead(_topPin) && digitalRead(_botPin) && millis() - _startDoubleTime < 1000)
    {
        doubleHold = false;
        doubleClick = true;
        topClick = false;
        botClick = false;
    }
}

bool Input::powerOn()
{
    return digitalRead(_powerPin);
}

int Input::getPot()//reads average of 10 pot values
{
    for (int i = 0; i < 10; i++)
        if (i < 9)
            _prevPot[i] = _prevPot[i + 1];
    _prevPot[9] = analogRead(_potPin);
    int avg = 0;
    for (int i = 0; i < 10; i++)
        avg += _prevPot[i];
    avg /= 10;
    return avg;
}