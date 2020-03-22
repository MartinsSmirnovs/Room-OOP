#ifndef buttonIn
#define buttonIn
#include "Arduino.h"
class Input{
    public:
        Input(byte topButtonPin, byte botButtonPin, byte potPin, byte powerPin, int inputTime);
        Input(){};
        void listen();//reads data from all pins
        int getPot();//returns pot value
        bool powerOn();//returns power button value

        byte potValue;//reads pot value
        bool topClick;//stores top button state
        bool botClick;//stores bot button state
        bool doubleClick;
        bool doubleHold;//if double click has been pressed more than 1 sec
    private:
        byte _topPin;
        byte _botPin;
        byte _potPin;
        byte _powerPin;
        int _inputTime;
        unsigned long _startTopTime;//for checking passed time since user has pressed the button 
        unsigned long _startBotTime;
        unsigned long _startDoubleTime;
        int _prevPot[10];
};
#endif