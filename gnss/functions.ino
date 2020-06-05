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
  gnss.begin(Wire);   // Setup GNSS for Serial communication at default value

  //Check to see if GNSS is connected
  if(gnss.isConnected() == false){
    printOLED("Initial GNSS setup failed. Freezing.");   // Print error message to oled
    while(1);                                            // Freeze program (infinite loop)
  }
  else if(gnss.isConnected() == true){
    printOLED("Setting up GNSS module...");              // Print message 
  }

  //Check to see if Survey is in Progress before initiating one
  boolean surveyStatus;
  surveyStatus = checkSurveyStatus(gnss);
  if(surveyStatus == false){
    printOLED("Failed to get Survey In status. Freezing.");  // Print error message to oled
    while (1);                                               // Freeze program (infinite loop)
  }
  else if(surveyStatus == true){
    printOLED("Survey is already in progress.");
  }
  else{
    boolean response = gnss.enableSurveyMode(300, 2.000); // Enable Survey In. Minimum time is 300 seconds and minimum accuracy of 2.000 meters accuracy achieved.  
    if(response == false){
      printOLED("Survey failed. Freezing.");
      while(1);
    }
    printOLED("Survey has begun."); 
  }
  gnss.setI2COutput(COM_TYPE_UBX);
  gnss.saveConfiguration();
}

void printOLED(String msg){
  oled.clear(PAGE); // Clear the buffer
  oled.print(msg);  // Save message to buffer
  oled.display();   // Display buffer
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
