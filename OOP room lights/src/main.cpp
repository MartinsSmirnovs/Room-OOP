#include <Arduino.h>
#include <Formicarium.h>
#include <ButtonInput.h>
#include <Animations.h>
#include <Adafruit_NeoPixel.h>
#include <MemoryFree.h>
#include <Led.h>

#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define NUM_LEDS 79

#define redPin 3
#define greenPin 5
#define bluePin 6

#define outLedPin 9
#define ventPin 10

#define ledPin 4

#define topButtonPin 7
#define botButtonPin 8
#define potPin 19
#define switchPin 2

Adafruit_NeoPixel strip(NUM_LEDS, ledPin, NEO_GRB + NEO_KHZ800);

Led bed(redPin, greenPin, bluePin, "Bed");
AdrLed table("Table", strip);

AntVent antVent(ventPin, "Ant Vent", 0);
AntLed antLed(outLedPin, "Ant LED", 1);
Input input(topButtonPin, botButtonPin, potPin, switchPin, 100);

Animation *animation[2];

SSD1306AsciiAvrI2c oled;

const byte menuNum = 4;
const char chSym = '>';
String divider = "---------------------";

String menuStrings[menuNum] = {
    "Table",
    "Bed",
    "AntLed",
    "AntVent"};

String antVentMenu[2] = {
    "Back",
    "Turn "};

String antLedMenu[3] = {
    "Back",
    "Turn ",
    "Fade"};

String bedMenu[6] = {
    "Back",
    "Turn ",
    "Color",
    "Solid Rainbow",
    "Solid Fade",
    "Solid Blink"};

String tableMenu[8] = {
    "Back",
    "Turn ",
    "Color",
    "Fire",
    "Rainbow",
    "Solid Rainbow",
    "Solid Fade",
    "Solid Blink"};

byte menuStartEnd[2] = {0, 0};

bool lastTop = false; //for last button press states
bool lastBot = false;
bool lastDouble = false;

byte menuState = 0; //for arrow position in menu
byte oldMenuState = 0;
byte subMenuLevel = 0;

byte maxMenuLen = 4; //how many rows are in current menu

String currentMenu = "Main"; //stores name of current menu
String oldCurrentMenu;

void mainMenu(byte position) //creates main menu and sets choose symbol at position place
{
  oled.clear();
  for (byte i = 0; i < menuNum; i++)
  {
    if (i == position)
    {
      oled.print(F("> "));
    }
    else
    {
      oled.print(F("  "));
    }
    oled.println(menuStrings[i]);
    oled.println(divider);
  }
}

void buildRows(String rowData[8], byte arrLen) //builds submenu based on input data
{
  oled.clear();
  oled.println(currentMenu); //prints name of menu
  oled.println(divider);

  for (byte i = menuStartEnd[0]; i <= menuStartEnd[1]; i++) //print out menu content
  {
    if (i == menuState)
    {
      oled.print(F("> "));
    }
    else
    {
      oled.print(F("  "));
    }
    oled.println(rowData[i]);
    Serial.print(i); Serial.print(" ");
    Serial.println(rowData[i]);
    oled.println(F("  --------"));
  }
}

void buildMenu() //builds all menus
{
  if (subMenuLevel == 0)
  {
    maxMenuLen = 4;
    mainMenu(menuState);
  }
  else if (subMenuLevel == 1)
  {
    if (menuStartEnd[0] > menuState)
    {
      menuStartEnd[0] = menuState;
      if (currentMenu == "AntVent")
        menuStartEnd[1] = menuStartEnd[0] + 1;
      else
        menuStartEnd[1] = menuStartEnd[0] + 2;
    }
    else if (menuStartEnd[1] < menuState)
    {
      menuStartEnd[1] = menuState;
      if (currentMenu == "AntVent")
        menuStartEnd[0] = menuStartEnd[1] - 1;
      else
        menuStartEnd[0] = menuStartEnd[1] - 2;
      Serial.println(menuState);
      Serial.println(menuStartEnd[1]);
    }


    Serial.print("start: ");Serial.println(menuStartEnd[0]);
    Serial.print("end: ");Serial.println(menuStartEnd[1]);

    if (currentMenu == "AntVent") //opens antvent menu
    {
      maxMenuLen = 2; //max array length of antvent
      buildRows(antVentMenu, maxMenuLen);
    }
    else if (currentMenu == "AntLed")
    {
      maxMenuLen = 3;
      buildRows(antLedMenu, maxMenuLen);
    }
    else if (currentMenu == "Bed")
    {
      maxMenuLen = 6;
      buildRows(bedMenu, maxMenuLen);
    }
    else if (currentMenu == "Table")
    {
      maxMenuLen = 8;
      buildRows(tableMenu, maxMenuLen);
    }
  }
}

void openMenu() //sets variables to corresponding values
{
  if (subMenuLevel == 0)
  {
    currentMenu = menuStrings[menuState];
    subMenuLevel++;
    menuState = 0;
    menuStartEnd[0] = 0;
    if(currentMenu != "AntVent")
    menuStartEnd[1] = 2;
    else menuStartEnd[1] = 1;
  }
  else if ((subMenuLevel == 2 || subMenuLevel == 1) && menuState == 0)
  { //going back from submenu
    subMenuLevel--;
  }
}

void menuExe() //Controls menu inputs
{
  if (lastBot != input.botClick && input.botClick)
  {
    oldMenuState = menuState;
    menuState++;
    if (menuState == maxMenuLen)
    {
      menuState = 0;
    }
    buildMenu();
  }
  if (lastTop != input.topClick && input.topClick)
  {
    oldMenuState = menuState;
    if (menuState == 0)
    {
      menuState = maxMenuLen - 1;
    }
    else
    {
      menuState--;
    }
    buildMenu();
  }

  if (lastDouble != input.doubleClick && input.doubleClick)
  {
    openMenu();
    buildMenu();
  }

  lastDouble = input.doubleClick;
  lastBot = input.botClick;
  lastTop = input.topClick;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(freeMemory());
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(Adafruit5x7);
  oled.clear();

  // strip.begin();
  // strip.clear(); //notīra iepriekšējās krāsas no krāsu lentas
  // strip.show();  //apdeito krāsu lentu, lai iepriekšējās krāsas tiktu notīrītas

  // animation[0] = new SolidRainbow(10);
  // animation[0]->firstTime(bed.rgb);

  // animation[1] = new Rainbow(15, NUM_LEDS, strip);
  // animation[1]->firstTime(table.rgb);

  mainMenu(0);
  Serial.println(F("Start"));
}

void loop()
{
  // animation[0]->execute(rgb);
  // analogWrite(3, rgb[0]);
  // analogWrite(5, rgb[1]);
  // analogWrite(6, rgb[2]);
  // animation[1]->execute(rgb);
  // strip.show();

  // if(millis()%1000 == 0){
  //   Serial.println(freeMemory());
  // }

  menuExe();
  input.listen();
}
