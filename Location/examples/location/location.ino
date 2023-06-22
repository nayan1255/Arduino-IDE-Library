// ====================================================================================
// Title  :  location 
// Author :  Eng. Mohamed Sayed Yousef  -  Mansoura, Egypt
//            https://electronics010.blogspot.com.eg/
// Date   :  25/02/2018
// ver    :  1.0
// Board  :  NodeMCU 1.0 
// ====================================================================================

// Libraries
#include <ESP8266WiFi.h>
#include "location.h"

char ssid[] ="xxxxxxxx";    // your network SSID (name) 
char pass[] = "xxxxxxxx";   // your network password

// Google API key
String geolocationKey = "xxxxxxxx";
String geocodingKey   = "xxxxxxxx";

// From http://www.mcc-mnc.com/
int MCC = 602;  // Egypt Country Code
int MNC = 2;    // Egypt Network Code
String carrier = "Vodafone";

Location myLocation;

// ===========================================================================================

void setup() {
  Serial.begin(115200);  

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");        
  }
        
  Serial.println("."); 
  Serial.println("WiFi connected");
  Serial.print("local IP: ");
  Serial.println(WiFi.localIP());

  // initialize the myLocation
  myLocation.begin(geolocationKey, geocodingKey, MCC, MNC, carrier);
    
}

// ===========================================================================================

void loop() {  
  
  myLocation.getLocation();

  Serial.print("Latitude = ");     Serial.println(myLocation.latitude,7);
  Serial.print("Longitude = ");    Serial.println(myLocation.longitude,7);
  Serial.print("Accuracy = ");     Serial.println(myLocation.accuracy);
  Serial.print("Address: ");       Serial.println(myLocation.address); 
  Serial.println("-------------------------");

  
  // Wait for 1 Min.
  delay(60000);      
  
}

// ===========================================================================================


