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
}

void Input::listen()
{
    
    if(!digitalRead(_botPin)){
        _startBotTime = millis();
        botClick = false;
    } else if(digitalRead(_botPin) && millis()-_startBotTime > _inputTime){//waits _inputTime seconds until button value gets saved
        botClick = true;
    }

    
    if(!digitalRead(_topPin)){
        _startTopTime = millis();
        topClick = false;
    } else if(digitalRead(_topPin) && millis()-_startTopTime > _inputTime){
        topClick = true;
    }


    if(!(digitalRead(_topPin) && digitalRead(_botPin))){
        _startDoubleTime = millis();
        doubleClick = false;
        doubleHold = false;
    } else if(digitalRead(_topPin) && digitalRead(_botPin) && millis()-_startDoubleTime > 1000){
        doubleHold = true;
        doubleClick = false;
        topClick = false;
        botClick = false;
    } else if (digitalRead(_topPin) && digitalRead(_botPin) && millis()-_startDoubleTime < 1000){
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

int Input::getPot()
{
    return analogRead(_potPin);
}