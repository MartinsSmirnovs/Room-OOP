#include <Arduino.h>
#include <Formicarium.h>
#include <ButtonInput.h>
#include <Animations.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 79

const byte redPin = 3;
const byte greenPin = 5;
const byte bluePin = 6;

const byte outLedPin = 9;
const byte ventPin = 10;

const byte ledPin = 4;

const byte topButtonPin = 7;
const byte botButtonPin = 8;
const byte potPin = 19;
const byte switchPin = 2;

byte rgb[3];

// AntVent antVent(ventPin, "Ant Vent", 0);
// AntLed antLed(outLedPin, "Ant LED", 1);

// Input input(topButtonPin, botButtonPin, potPin, switchPin, 150);
Animation *animation[2];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, ledPin, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  strip.begin();
  strip.clear(); //notīra iepriekšējās krāsas no krāsu lentas
  strip.show();  //apdeito krāsu lentu, lai iepriekšējās krāsas tiktu notīrītas

  animation[0] = new SolidRainbow(10);
  animation[0]->firstTime(rgb);

  animation[1] = new Fire(15, NUM_LEDS, &strip);
  animation[1]->firstTime(rgb);
}

void loop()
{
  animation[0]->execute(rgb);
  analogWrite(3, rgb[0]);
  analogWrite(5, rgb[1]);
  analogWrite(6, rgb[2]);

  animation[1]->execute(rgb);
  strip.show();
}

//delete animation[0];

