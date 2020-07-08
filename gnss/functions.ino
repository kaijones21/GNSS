void setupOLED(MicroOLED &oled){
  delay(100);            // Wait 100 ms
  oled.begin();          // Initialize the OLED
  oled.clear(ALL);       // Clear the display's internal memory
  oled.display();        // Display what's in buffer (should just be the SparkFun splashscreen)
  delay(1000);           // Delay 1000 ms
  oled.clear(PAGE);      // Clear the buffer
  oled.setFontType(0);
  oled.setCursor(0,0);
}

void setupGNSS(SFE_UBLOX_GPS &gnss){

  //-------------------------------------------------------------
  // Connect to the Ublox module using Wire port at default value
  // and check to see if gnss is connected. 
  //-------------------------------------------------------------  
  gnss.begin(Wire);                                      // Setup GNSS for Serial communication at default value
  if(gnss.isConnected() == false){
    //printOLED("Initial GNSS setup failed. Freezing.");   // Print error message to oled
    while(1);                                            // Freeze program (infinite loop)
  }
  else if(gnss.isConnected() == true){
    //printOLED("Setting up GNSS module...");              // Print message
    delay(2000);                                         // To allow LCD to have message for a brief moment 
  }

  //-----------------------------------------------------------
  //Check to see if Survey is in progress before initiating one
  //-----------------------------------------------------------
  response = gnss.getSurveyStatus(2000); // Query rtk module for SVIN status for time [ms] 
  
  if(response == false){
    //printOLED("Failed to get Survey In status. Freezing.");  // Print error message to oled
    while (1);                                               // Freeze program (infinite loop)
  }
  
  if(gnss.svin.active == true){
    //printOLED("Survey is already in progress.");
    delay(2000);
  }
  else{
    response = gnss.enableSurveyMode(minSurveyTime, minSurveyAcc); // Enable Survey In. Minimum time is 300 seconds and minimum accuracy of 1.000 meters accuracy achieved.  
    if(response == false){
      //printOLED("Survey failed to start. Freezing.");
      while(1);
    }
    oled.clear(PAGE); 
    oled.print(F("Survey has begun. Minimum time: "));
    oled.print(minSurveyTime);
    oled.print(F(". \nMinimum Acc: "));
    oled.print(minSurveyAcc);
    oled.display();
    delay(2000);
  }
  gnss.setI2COutput(COM_TYPE_UBX);
  gnss.saveConfiguration();
}

void printOLED(String msg){
  oled.clear(PAGE);     // Clear the buffer
  oled.setCursor(0,0);  // Set cursor in top left corner
  oled.print(msg);      // Save message to buffer
  oled.display();       // Display buffer
}

boolean checkSurveyStatus(SFE_UBLOX_GPS &gnss){
  boolean response; 
  response = gnss.getSurveyStatus(2000); // Query rtk module for SVIN status for time [ms] 
  if(response == false){
    return false;
  }
  if(gnss.svin.active == true){
    return true;
  }
}

void enableRTCM(SFE_UBLOX_GPS &gnss){
  response = true;
  response &= gnss.enableRTCMmessage(UBX_RTCM_1005, COM_PORT_I2C, 1); //Enable message 1005 to output through I2C port, message every second
  response &= gnss.enableRTCMmessage(UBX_RTCM_1077, COM_PORT_I2C, 1);
  response &= gnss.enableRTCMmessage(UBX_RTCM_1087, COM_PORT_I2C, 1);
  response &= gnss.enableRTCMmessage(UBX_RTCM_1230, COM_PORT_I2C, 10); //Enable message every 10 seconds

  if (response == true){
    //printOLED("RTCM messages enabled");
    delay(1000);
  }
  else{
    //printOLED("RTCM enable failed. Freezing.");
    while (1); //Freeze
  }
  
  //printOLED("Base survey complete! RTCM now broadcasting.");
}
