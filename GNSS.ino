/* Arduino GNSS Sketch
 * 
 */

//****************************************************
//                    LIBRARIES
//****************************************************
#include <SFE_MicroOLED.h>                   // Micro OLED library
#include <SparkFun_Ublox_Arduino_Library.h>  // Ublox GNSS library
#include <Wire.h>                            // I2C library

//****************************************************
//                 PIN DEFINITIONS
//****************************************************
#define PIN_RESET 3  // reset pin for oled
#define DC_JUMPER 1  // jumper state 

//****************************************************
//               OBJECT DECLARATIONS
//****************************************************
MicroOLED oled(PIN_RESET, DC_JUMPER); 
 
void setup() {
  delay(100);
  Wire.begin();
  oled.begin();
  oled.clear(ALL);
  oled.display();
  delay(1000);  
  oled.clear(PAGE);
}

void loop() {
  printOLED("Hello World!\n Testing 1 2 3", 0);
}

void printOLED(String title, int font){
  oled.clear(PAGE);
  oled.setFontType(font);
  oled.setCursor(0,0);
  oled.print(title);
  oled.display();
  delay(1500);
  oled.clear(PAGE);
}
