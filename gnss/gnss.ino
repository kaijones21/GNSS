#include <SFE_MicroOLED.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <Wire.h>

//************************************************
//               PIN DEFINITIONS
//************************************************
#define PIN_RESET 9
#define DC_JUMPER 1

//************************************************
//              OBJECT INSTANCES
//************************************************
MicroOLED oled(PIN_RESET, DC_JUMPER);  // create oled object
SFE_UBLOX_GPS baseStation;             // create rtk gnss object

//************************************************
//                   SETUP
//************************************************
void setup() {
  Wire.begin();           // initiate I2C comms bus
  setupOLED(oled);        // setup the oled
  setupGNSS(baseStation); // setup basestation GNSS
}

void loop() {

}
