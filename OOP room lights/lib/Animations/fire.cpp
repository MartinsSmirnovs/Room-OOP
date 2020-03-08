#include "Arduino.h"
#include "Animations.h"
#include "Adafruit_NeoPixel.h"

Fire::Fire(int startDelayTime, byte numLeds, Adafruit_NeoPixel *stripPointer)
{
  name = "Fire";
  _delayTime = startDelayTime;
  byte _numLeds = numLeds;
  Adafruit_NeoPixel *_strip = stripPointer;
}

void Fire::execute(byte rgbArr[3])
{
  if (millis() - _oldTimeInt >= _delayTime)
  {
    _oldTimeInt = millis();
    byte Cooling = 55;
    byte Sparking = 120;
    byte heat[_numLeds];
    int cooldown;

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < _numLeds; i++)
    {
      cooldown = random(0, ((Cooling * 10) / _numLeds) + 2);

      if (cooldown > heat[i])
      {
        heat[i] = 0;
      }
      else
      {
        heat[i] = heat[i] - cooldown;
      }
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = _numLeds - 1; k >= 2; k--)
    {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < Sparking)
    {
      int y = random(7);
      heat[y] = heat[y] + random(160, 255);
      //heat[y] = random(160,255);
    }
    // Step 4.  Convert heat to LED colors
    for (int j = 0; j < _numLeds; j++)
    {
      Serial.println(j);
      // Scale 'heat' down from 0-255 to 0-191
      byte t192 = round((heat[j] / 255.0) * 191);

      // calculate ramp up from
      byte heatramp = t192 & 0x3F; // 0..63
      heatramp <<= 2;              // scale up to 0..252

      // figure out which third of the spectrum we're in:
      if (t192 > 0x80)
      { // hottest
        _strip->setPixelColor(j, 255, heatramp, 255);
      }
      else if (t192 > 0x40)
      { // middle
        _strip->setPixelColor(j, heatramp, 0, 255);
      }
      else
      { // coolest
        _strip->setPixelColor(j, 0, 0, heatramp);
      }
    }
  }
}

void Fire::firstTime(byte part[3]){
  
}