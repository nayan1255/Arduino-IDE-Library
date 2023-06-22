// ====================================================================================
// Title  :  Location Library
// Author :  Eng. Mohamed Sayed Yousef  -  Mansoura, Egypt
//            https://electronics010.blogspot.com.eg/
// Date   :  25/02/2018
// ver    :  1.0

// Based on Post from http://m0xpd.blogspot.com.eg/2016/08/esp8266-geolocation.html

// Copyright (c) 2018 Mohamed Sayed Yousef.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice, author and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ====================================================================================

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "location.h" 

DynamicJsonBuffer jsonBuffer(320);
// ===========================================================================================

Location::Location(){
  
}

// ===========================================================================================

void Location::begin(String geolocationKey, String geocodingKey, int MCC, int MNC, String carrier){

  _geolocationKey = geolocationKey;
  _geocodingKey = geocodingKey;
  _MCC = MCC;    // Country Code
  _MNC = MNC;    // Network Code
  _carrier = carrier;
    
  _GEO_LOCATION_HOST = "www.googleapis.com";
  _GEO_LOCATION_PAGE = "/geolocation/v1/geolocate?key=";

  _GEO_CODING_HOST = "maps.googleapis.com";
  _GEO_CODING_PAGE = "/maps/api/geocode/json?latlng=";

  latitude  = 0.0;
  longitude = 0.0;
  accuracy  = 0;
  address   = " ";
  
    
}

// ===========================================================================================

void Location::getLocation(){
  geolocation();
  geocoding();
}

// ===========================================================================================

void Location::geolocation(){
  
int networks = 0;
String request_body = "";
String response = "";

  // return the number of networks found
  networks = WiFi.scanNetworks();
  Serial.print(networks);
  Serial.println(" networks found.");  
  Serial.println(" ");

  //--------------------------------------------------------------------
  // build the jsonString
  request_body = "{\n";
  request_body += "\"homeMobileCountryCode\": ";
  request_body += String(_MCC);
  request_body += ",\n";

  request_body += "\"homeMobileNetworkCode\": ";
  request_body += String(_MNC);
  request_body += ",\n";

  request_body += "\"radioType\": \"gsm\",\n";
  request_body += "\"carrier\": \"";    
  request_body += _carrier;
  request_body += "\",\n";
  
  request_body +="\"wifiAccessPoints\": [\n";
  
    for (int i = 0; i < networks; ++i){
      
      request_body += "{\n";
      request_body += "\"macAddress\": \"";    
      request_body += (WiFi.BSSIDstr(i));
      request_body += "\",\n";
      request_body += "\"signalStrength\": ";     
      request_body += WiFi.RSSI(i);
      request_body += "\n";
      
      if(i<networks-1){
        request_body +="},\n";
      } else {
        request_body +="}\n";  
      }
    }
    
    request_body += ("]\n");
    request_body += ("}\n"); 
  //--------------------------------------------------------------------  
  
  // Connect to the host serve  
  Serial.println("Please wait about 30 seconds to get the data...");

  // Creates a client that can connect to to a specified server and port
  WiFiClientSecure client;
  if (client.connect(_GEO_LOCATION_HOST, 443)) {
    // Create the URL that will be used 
    String url = "";
           url = url + String(_GEO_LOCATION_PAGE) + _geolocationKey ;
               
   // Make a HTTP request
   String http = "POST ";
          http = http + url + " HTTP/1.1\r\n";
          http = http + "Host: " + String(_GEO_LOCATION_HOST) + "\r\n";
          http = http + "Connection: close\r\n";
          http = http + "Content-Type: application/json\r\n";
          http = http + "Content-Length: " + request_body.length() + "\r\n";
          http = http + "\r\n";

    // Send HTTP request to the server
    client.print(http);
    client.print(request_body);  
    delay(500);
  }

  // Read and parse all the lines of the reply from server        
  while (client.available()) {
    response = client.readStringUntil('\r');
    JsonObject& root = jsonBuffer.parseObject(response); 
    if(root.success()){
      latitude  = root["location"]["lat"];
      longitude = root["location"]["lng"];
      accuracy  = root["accuracy"];
    }
    
  }

  // Disconnect from the server
  client.stop();
  
}

// ===========================================================================================

void Location::geocoding(){
String response = "";  
String _status = "";
  
  // Connect to the host serve  
  // Creates a client that can connect to to a specified server and port
  WiFiClientSecure client;
  if (client.connect(_GEO_CODING_HOST, 443)) {   
    // Create the URL that will be used 
    String url = "";
           url = url + String(_GEO_CODING_PAGE) +String(latitude) + "," + String(longitude) + "&key=" + _geocodingKey ;
               
   // Make a HTTP request
   String http = "GET ";
          http = http + url + " HTTP/1.1\r\n";
          http = http + "Host: " + String(_GEO_CODING_HOST) + "\r\n";
          http = http + "Connection: close\r\n";
          http = http + "Content-Type: application/json\r\n";
          http = http + "\r\n";

    // Send HTTP request to the server
    client.print(http);
    delay(500);
  }

  // Read and parse all the lines of the reply from server          
  while (client.available()) {
    response = client.readStringUntil('\r');
    JsonObject& root = jsonBuffer.parseObject(response); 
    if(root.success()){
      _status = (const char*) root["status"];

      if(_status == "OK"){
          address = (const char*) root["results"][0]["formatted_address"]; 
      }
    }
    
  }

  // Disconnect from the server
  client.stop();
}

// ===========================================================================================



