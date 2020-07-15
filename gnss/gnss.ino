#include "ros.h"
#include<std_msgs/Int64.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <Wire.h>

//************************************************
//               SYSTEM VARIABLES
//************************************************
long lastTime = 0;  // simple local timer. Limits amount of I2C traffic to Ublox module.
long longitude = 0; // variable to store longitude
long latitude = 0;  // variable to store latitude

//************************************************
//              OBJECT INSTANCES
//************************************************
SFE_UBLOX_GPS baseStation;                    // create rtk gnss object
ros::NodeHandle nh;                           // ROS node handler. Allows us to create publishers and subscribers.
std_msgs::Int64 intLat;                       // ROS message type to store latitude
std_msgs::Int64 intLon;                       // ROS message type to store longitude
ros::Publisher latPub("latitude", &intLat);   // ROS publisher for latitude
ros::Publisher lonPub("longitude", &intLon);  // ROS publisher for longitude

//************************************************
//                   SETUP
//************************************************
void setup() {
  Wire.begin();           // initiate I2C comms bus
  nh.initNode();          // initialize ROS node handle
  nh.advertise(latPub);   // advertise latitude publisher
  nh.advertise(lonPub);   // advertise longitude publisher
  setupGNSS(baseStation); // setup GNSS 
  waitForSurvey();        // wait for Survey-In to finish
  enableRTCM();           // enable RTCM messages
}

void loop() {
  baseStation.checkUblox();                 // see if new data is available. Process bytes as they come in. 
  if (millis() - lastTime > 1000){
    lastTime = millis(); // Update timer
    latitude = baseStation.getLatitude();
    longitude = baseStation.getLongitude();
    Serial.print(F("Latitude: "));
    Serial.print(latitude);
    Serial.print(F("\tLongitude: "));
    Serial.println(longitude);

    intLat.data = latitude;
    intLon.data = longitude;
    latPub.publish( &intLat );
    lonPub.publish( &intLon );
    nh.spinOnce();
  }
}
