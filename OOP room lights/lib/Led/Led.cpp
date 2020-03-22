#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "Led.h"

Led::Led(byte redPin, byte greenPin, byte bluePin, String objName)
{
  name = objName;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  for (byte i = 0; i < 3; i++)
    rgb[0] = 0;
  _run = false;
}

void Led::execute()
{
  analogWrite(5, rgb[0]);
  analogWrite(6, rgb[1]);
  analogWrite(3, rgb[2]);
}

String Led::state()
{
  for (int i = 0; i < 3; i++)
  {
    if (rgb[i] > 0)
      return "ON";
    else if (i == 2 && rgb[i] == 0)
      return "OFF";
  }
}

String Led::getState()
{
  if (_run)
    return "Turn OFF";
  else
    return "Turn ON";
}

void Led::stop()
{
  _run = !_run;
  if (_run)
    for (byte i = 0; i < 3; i++)
      rgb[i] = 255;
  else
    for (byte i = 0; i < 3; i++)
      rgb[i] = 0;
}

bool Led::getRun()
{
  return _run;
}

AdrLed::AdrLed(String objName)
{
  name = objName;
}