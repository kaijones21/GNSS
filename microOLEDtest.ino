#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

//////////////////////////
// MicroOLED Definition //
//////////////////////////
//The library assumes a reset pin is necessary. The Qwiic OLED has RST hard-wired, so pick an arbitrarty IO pin that is not being used
#define PIN_RESET 9  
//The DC_JUMPER is the I2C Address Select jumper. Set to 1 if the jumper is open (Default), or set to 0 if it's closed.
#define DC_JUMPER 1 

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

void setup()
{
  delay(100);
  Wire.begin();
  oled.begin();
  oled.clear(ALL);
  oled.display();
  delay(1000);
  oled.clear(PAGE);
}

void loop(){
  printTest("Lat:-45.97760\nLong:42.91841", 0);
}

void printTest(String title, int font)
{
  oled.clear(PAGE);
  oled.setFontType(font);
  oled.setCursor(0,0);
  oled.print(title);
  oled.display();
  delay(1500);
  oled.clear(PAGE);
}

