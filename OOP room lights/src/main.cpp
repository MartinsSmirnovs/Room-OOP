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
#define potPin 17
#define switchPin 2

Adafruit_NeoPixel strip(NUM_LEDS, ledPin, NEO_GRB + NEO_KHZ800);

Led bed(redPin, greenPin, bluePin, "Bed");
AdrLed table("Table");

AntVent antVent(ventPin, "Ant Vent", 0);
AntLed antLed(outLedPin, "Ant LED", 1);
Input input(topButtonPin, botButtonPin, potPin, switchPin, 100);

Animation *animation[2];

SSD1306AsciiAvrI2c oled;

const byte menuNum = 4;
const char chSym = '>';
String divider = "---------------------";

unsigned long screenTimeOut = 300000; //time out of 5 minutes
unsigned long millisOut = 0;          //holds last click millis() value

String animOptions[2] = {
    "Back",
    "Speed: "};

String colorOptions[4] = {
    "Back",
    "Red:   ",
    "Green: ",
    "Blue:  "};

String menuStrings[menuNum] = { //main menu text options
    "Table",
    "Bed",
    "AntLed",
    "AntVent"};

String antVentMenu[2] = {
    "Back",
    antVent.getState()};

String antLedMenu[3] = {
    "Back",
    antVent.getState(),
    "Fade"};

String bedMenu[6] = {
    "Back",
    bed.getState(),
    "Color",
    "Solid Rainbow",
    "Solid Fade",
    "Solid Blink"};

String tableMenu[8] = {
    "Back",
    table.getState(),
    "Color",
    "Fire",
    "Rainbow",
    "Solid Rainbow",
    "Solid Fade",
    "Solid Blink"};

byte menuStartEnd[2] = {0, 0}; //for displaying menus that are longer than screen

bool lastTop = false; //for last button press states
bool lastBot = false;
bool lastDouble = false;
bool lastHold = false;

bool readPot = false; //if user changes parameters in options

byte menuState = 0; //for arrow position in menu
byte subMenuLevel = 0;

byte maxMenuLen = 4; //how many rows are in current menu

String currentMenu = "Main"; //stores name of current menu
String oldCurrentMenu;
String subMenuName = ""; //options menu name

unsigned long oldTimeInt = 0;
int adrSpeed = 100;

int lastPotVal = 0; //stores last potentiometer value

int subMenuState = 0; //saves menuState before going to options

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
  if (subMenuLevel == 1)
    oled.println(currentMenu); //prints name of menu
  else
    oled.println(subMenuName);

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
    oled.println(F("  --------"));
  }
}

void chooseFunction() //chooses color/animation and sets pot value
{
  if (currentMenu == "AntVent")
  {
    switch (menuState)
    { // all switch cases start from 1, because 0 is for back
    case 1:
      antVent.stop(); //turns on/off vent
      antVentMenu[1] = antVent.getState();
      antVent.staticRun();
      break;
    default:
      break;
    }
  }
  else if (currentMenu == "AntLed")
  {
    if (menuState != 1 && !antLed.getRun()) //for turning on when fade is turned on
    {
      antLed.stop();
      antLedMenu[1] = antLed.getState();
    }
    switch (menuState)
    {
    case 1: //turns on ant led
      antLed.stop();
      antLedMenu[1] = antLed.getState();
      antLed.updateMode(1);
      break;
    case 2: //turns on ant led fade
      antLed.setSpeed(10);
      antLed.updateMode(2);
      break;
    default:
      break;
    }
  }
  else if (currentMenu == "Bed") //bed menu
  {
    delete animation[0];                 //deletes previous object
    if (menuState != 1 && !bed.getRun()) //for turning on when animation is turned on
    {
      bed.stop();
      bedMenu[1] = bed.getState();
    }
    switch (menuState)
    {
    case 1:
      animation[0] = new Default();
      bed.stop();
      bedMenu[1] = bed.getState();
      break;
    case 2:
      animation[0] = new Default();
      break;
    case 3:
      animation[0] = new SolidRainbow(10);
      break;
    case 4:
      animation[0] = new SolidFade(10);
      break;
    case 5:
      animation[0] = new SolidBlink(100);
      break;
    default:
      break;
    }
    animation[0]->firstTime(bed.rgb);
  }
  else if (currentMenu == "Table") //table menu
  {
    delete animation[1];                   //deletes previous object
    if (menuState != 1 && !table.getRun()) //for turning on when animation is turned on
    {
      table.stop();
      tableMenu[1] = table.getState();
    }
    switch (menuState)
    {
    case 1:
      animation[1] = new Default();
      table.stop();
      tableMenu[1] = table.getState();

      strip.fill(strip.Color(table.rgb[0], table.rgb[1], table.rgb[2]), 0, NUM_LEDS);
      strip.show();
      break;
    case 2:
      animation[1] = new Default();
      break;
    case 3:
      animation[1] = new Fire(15, NUM_LEDS, &strip);
      break;
    case 4:
      animation[1] = new Rainbow(15, NUM_LEDS, &strip);
      break;
    case 5:
      adrSpeed = 50;
      animation[1] = new SolidRainbow(adrSpeed);
      break;
    case 6:
      adrSpeed = 50;
      animation[1] = new SolidFade(adrSpeed);
      break;
    case 7:
      adrSpeed = 100;
      animation[1] = new SolidBlink(adrSpeed);
      break;
    default:
      break;
    }
    animation[1]->firstTime(table.rgb);
  }
}

void buildOptions(String menuData[8], int newSpeed = 0, int red = 0, int green = 0, int blue = 0) //build color or animation options menu
{//newSpeed IS NOT new speed
  if (subMenuName.substring(subMenuName.length() - menuData[subMenuState].length()) != menuData[subMenuState]) //runs only once, to set options data
  {
    subMenuName = currentMenu + " " + menuData[subMenuState];
    menuState = 0;
    if (menuData[subMenuState] == "Color") //sets displayed menu length
    {
      menuStartEnd[0] = 0;
      menuStartEnd[1] = 2;
      maxMenuLen = 4;
    }
    else
    {
      menuStartEnd[0] = 0;
      menuStartEnd[1] = 1;
      maxMenuLen = 2;
    }
    if (menuData[subMenuState] != "Color") //sets last animation or color value
    {
      animOptions[1] = "Speed: <" + String(newSpeed) + ">";
    }
    else
    {
      colorOptions[1] = "Red:   <" + String(red) + ">";
      colorOptions[2] = "Green: <" + String(green) + ">";
      colorOptions[3] = "Blue:  <" + String(blue) + ">";
    }
  }

  if (readPot)
  {
    if (menuData[subMenuState] != "Color")
    {
      int speed = lastPotVal;
      animOptions[1] = "Speed: <" + String(speed) + ">";
      if (currentMenu == "AntLed")
      {
        antLed.setSpeed(speed);
      }
      else if (currentMenu == "Bed")
      {
        animation[0]->setDelay(speed);
      }
      else if (currentMenu == "Table")
      {
        animation[1]->setDelay(speed);
      }
    }
    else
    {
      if (red != lastPotVal - 1 && menuState == 1 && abs(red - lastPotVal < 15))//sets new color values. -1 because values is maped from 1-256 but we need 0-255 for pwm
        red = lastPotVal - 1;
      else if (green != lastPotVal - 1 && menuState == 2 && abs(green - lastPotVal < 15))
        green = lastPotVal - 1;
      else if (blue != lastPotVal - 1 && menuState == 3 && abs(blue - lastPotVal < 15))
        blue = lastPotVal - 1;

      colorOptions[1] = "Red:   <" + String(red) + ">"; //sets displayed data in array
      colorOptions[2] = "Green: <" + String(green) + ">";
      colorOptions[3] = "Blue:  <" + String(blue) + ">";
      if (currentMenu == "Table") //sets new color values to bed and table
      {
        table.rgb[0] = red;
        table.rgb[1] = green;
        table.rgb[2] = blue;
        strip.fill(strip.Color(table.rgb[0],table.rgb[1],table.rgb[2]), 0, NUM_LEDS);
        strip.show();
      }
      else
      {
        bed.rgb[0] = red;
        bed.rgb[1] = green;
        bed.rgb[2] = blue;
      }
    }
  }

  if (menuData[subMenuState] == "Color") //color or animation options
  {
    if (menuStartEnd[0] > menuState) //if menuState goes up too high
    {
      menuStartEnd[0] = menuState;
      menuStartEnd[1] = menuStartEnd[0] + 2;
    }
    else if (menuStartEnd[1] < menuState) //if menuState goes down too low
    {
      menuStartEnd[1] = menuState;
      menuStartEnd[0] = menuStartEnd[1] - 2;
    }
    buildRows(colorOptions, maxMenuLen); //build color options
  }
  else
  {
    buildRows(animOptions, maxMenuLen); //build animation options
  }
}

void buildMenu() //builds all menus
{
  if (subMenuLevel == 0) //builds main manu
  {
    maxMenuLen = 4;
    mainMenu(menuState);
  }
  else if (subMenuLevel == 1) //build main menu submenu
  {
    if (menuStartEnd[0] > menuState) //if menuState goes up too high
    {
      menuStartEnd[0] = menuState;
      if (currentMenu == "AntVent")
        menuStartEnd[1] = menuStartEnd[0] + 1;
      else
        menuStartEnd[1] = menuStartEnd[0] + 2;
    }
    else if (menuStartEnd[1] < menuState) //if menuState goes down too low
    {
      menuStartEnd[1] = menuState;
      if (currentMenu == "AntVent")
        menuStartEnd[0] = menuStartEnd[1] - 1;
      else
        menuStartEnd[0] = menuStartEnd[1] - 2;
    }

    if (currentMenu == "AntVent") //opens antvent menu
    {
      maxMenuLen = 2; //max array length of antvent
      buildRows(antVentMenu, maxMenuLen);
    }
    else if (currentMenu == "AntLed") //opens antLed menu
    {
      maxMenuLen = 3; //max array length of antLed
      buildRows(antLedMenu, maxMenuLen);
    }
    else if (currentMenu == "Bed") //opens bed menu
    {
      maxMenuLen = 6; //max array length of bed
      buildRows(bedMenu, maxMenuLen);
    }
    else if (currentMenu == "Table") //opens table menu
    {
      maxMenuLen = 8; //max array length of table
      buildRows(tableMenu, maxMenuLen);
    }
  }
  else //submenu == 2
  {
    if (currentMenu == "AntVent")
      buildOptions(antVentMenu);
    else if (currentMenu == "AntLed")
      buildOptions(antLedMenu, antLed.getTimeInt(), antLed.getTimeInt());
    else if (currentMenu == "Bed")
      buildOptions(bedMenu, animation[0]->getDelay(), bed.rgb[0], bed.rgb[1], bed.rgb[2]);
    else
      buildOptions(tableMenu, animation[1]->getDelay(), table.rgb[0], table.rgb[1], table.rgb[2]);
  }
}

void openMenu() //sets variables to corresponding values
{
  if (subMenuLevel == 0)
  {
    currentMenu = menuStrings[menuState]; //current submenu name
    subMenuLevel++;
    menuState = 0; //for starting at first position in menu
    menuStartEnd[0] = 0;
    if (currentMenu != "AntVent")
      menuStartEnd[1] = 2;
    else
      menuStartEnd[1] = 1;
  }
  else if ((subMenuLevel == 2 || subMenuLevel == 1) && menuState == 0) //going one lever higher towards main menu
  {
    if (subMenuLevel == 2)
    {
      menuStartEnd[0] = 0;
      if (currentMenu != "AntVent")
        menuStartEnd[1] = 2;
      else
        menuStartEnd[1] = 1;
    }
    subMenuLevel--;
  }
  else if (subMenuLevel == 1 && !input.doubleHold) //if user presses one of submenu functions
  {
    chooseFunction();
  }
  else if (input.doubleHold) //animations/color options
  {
    if (subMenuLevel == 1)
    {
      subMenuName = "";
      subMenuState = menuState;
      subMenuLevel++;
    }
  }
}
bool turnOff = false;

void menuExe() //Controls menu inputs
{
  if (input.botClick || input.topClick || input.doubleClick)
  {
    millisOut = millis();
    turnOff = false;
  }
  if (lastBot != input.botClick && input.botClick) //checks for bot click
  {
    menuState++;
    if (menuState == maxMenuLen) //if menuState has exceeded current length of current array
      menuState = 0;
  }
  if (lastTop != input.topClick && input.topClick) //checks for top click
  {

    if (menuState == 0) //if menuState is zero and has to reset
      menuState = maxMenuLen - 1;
    else
      menuState--;
  }

  if ((lastDouble != input.doubleClick && input.doubleClick) || (lastHold != input.doubleHold && input.doubleHold)) //checks for double click
  {
    openMenu();
    buildMenu();
  }

  if ((lastTop != input.topClick && input.topClick) || (lastBot != input.botClick && input.botClick))
  {
    buildMenu();
  }

  lastHold = input.doubleHold;
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

  strip.begin();
  strip.clear(); //notīra iepriekšējās krāsas no krāsu lentas
  strip.show();  //apdeito krāsu lentu, lai iepriekšējās krāsas tiktu notīrītas

  animation[0] = new Default();
  // animation[0]->firstTime(bed.rgb);

  animation[1] = new Default();
  // animation[1] = new Rainbow(15, NUM_LEDS, strip);
  // animation[1]->firstTime(table.rgb);

  mainMenu(0);
  Serial.println(F("Start"));
  Serial.println(freeMemory());
}


void loop()
{
  animation[0]->execute(bed.rgb);
  animation[1]->execute(table.rgb);

  input.listen();
  menuExe();
  antLed.run();
  bed.execute();

  //For adr led when static animation runs
  if (millis() - oldTimeInt >= adrSpeed)
  {
    if (animation[1]->name == "Solid Rainbow" || animation[1]->name == "Solid Blink" || animation[1]->name == "Solid Fade")
    {
      strip.fill(strip.Color(table.rgb[0], table.rgb[1], table.rgb[2]), 0, NUM_LEDS);
      oldTimeInt = millis();
      strip.show();
    }
  }

  if (millis() - millisOut >= screenTimeOut && turnOff == false) //sleep mode after screenTimeOut milliseconds
  {
    turnOff = true;
    oled.clear();
  }

  if (subMenuLevel == 2 && menuState != 0) //AND last pot value != currentpotvalue
  {
    if (input.getPot() != lastPotVal)
    {
      lastPotVal = map(input.getPot(), 0, 1023, 1, 256);
      readPot = true;
      buildMenu();
    }
  }
  else
    readPot = false;
}

// Serial.println(map(input.getPot(), 0, 1023, 1, 255));

//  if (readPot){
//    antLed.setSpeed(map(input.getPot(), 0, 1023, 1, 1000));
//    animSpee
//  }