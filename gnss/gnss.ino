#include <SFE_MicroOLED.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <Wire.h>

//************************************************
//               PIN DEFINITIONS
//************************************************
#define PIN_RESET 9
#define DC_JUMPER 1

//************************************************
//               SYSTEM VARIABLES
//************************************************
boolean response;            // variable used for gnss setup
float minSurveyTime = 300;   // minimum amount of time [sec] for Survey-In to do
float minSurveyAcc = 1.000;  // minimum positional accuracy [m] Survey-In must achieve
long lastTime = 0;           // simple local timer. Limits amount of I2C traffic to Ublox module.

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
  
  //-----------------------------
  // Wait for Survey-In to finish
  //-----------------------------
  while(baseStation.svin.valid == false){
    response = baseStation.getSurveyStatus(2000); //Query module for SVIN status with 2000 ms timeout.
    if (response == true){
      oled.clear(PAGE);
      oled.setCursor(0,0);
      oled.print(F("Time: "));
      oled.print((String)baseStation.svin.observationTime);
      oled.print(F("\nAccuracy: "));
      oled.print((String)baseStation.svin.meanAccuracy);
      oled.display();
    }
    delay(1000);
  }

  printOLED("Survey finished!\nEnabling RTCM msgs");
  delay(1000);
  
  //---------------------
  // Enable RTCM Messages
  //---------------------
  enableRTCM(baseStation);

}

void loop() {
  baseStation.checkUblox(); // See if new data is available. Process bytes as they come in. 
  /*if ((millis() - lastTime > 1000) && (flag == true)){
    lastTime = millis(); // Update timer
    
    long latitude = baseStation.getLatitude();
    long longitude = baseStation.getLongitude();
    oled.clear(PAGE);
    oled.setCursor(0,0); 
    oled.print("Lat:");
    oled.print((String)latitude);
    oled.print("\nLon:");
    oled.print((String)longitude);
    oled.display();
  }*/
}
