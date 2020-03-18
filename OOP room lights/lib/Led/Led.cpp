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
}

void Led::execute()
{
  analogWrite(3, rgb[0]);
  analogWrite(5, rgb[1]);
  analogWrite(6, rgb[2]);
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

AdrLed::AdrLed(String objName, Adafruit_NeoPixel &stripObj)
{
  strip = stripObj;
  name = objName;
}