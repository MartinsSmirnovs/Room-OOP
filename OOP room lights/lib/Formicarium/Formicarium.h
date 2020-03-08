/*
Modes AntVent:
    0 - staticRun
Modes AntVent:
    0 - staticRun
    1 - smoothStart
    2 - fade
*/
#ifndef antvent
#define antvent
#include "Arduino.h"
class AntVent
{
public:
    AntVent(byte pin, String name, int mode);//initializes object
    AntVent(){};
    void staticRun();//runs pwm
    void updateVal(int pwmToCheck);//updates pwm value on pin
    void stop(bool type);//stops pwm
    String getName();//returns name of this object
    byte getPwm();//returns saved pwm value
    int getMode();
protected:
    bool _run;//checks if staticRun has to run
    String _name;//name of object
    byte _pin;//pin number
    byte _pwm;//pwm value
    byte _mode;//checks which function mode to execute
};

class AntLed : public AntVent
{
public:
    AntLed(byte pin, String name, int mode);//initializes object
    void smoothStart(bool type);//starts smoothly
    void run();//runs through functions
    void fade();//fades
    void updateMode(int mode);//update function type to execute
    void setSpeed(int speed);
    unsigned long getTimeInt();
private:
    int _fadeCount;
    int _speed;//for timing operations in functions
    unsigned long _timeInt;//for timing operations
};

#endif