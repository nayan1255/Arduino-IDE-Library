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

#ifndef LOCATION_H
#define LOCATION_H
 
#include <Arduino.h>

class Location {

  private:
    String _GEO_LOCATION_HOST;
    String _GEO_LOCATION_PAGE;

    String _GEO_CODING_HOST;
    String _GEO_CODING_PAGE;

    String _geolocationKey;
    String _geocodingKey;

    int _MCC;    // Country Code
    int _MNC;    // Network Code
    String _carrier;
    
    void geolocation();
    void geocoding();
    
  
  public:
    Location();
    void begin(String geolocationKey, String geocodingKey, int _MCC, int _MNC, String _carrier);    
    void getLocation();

    float latitude;
    float longitude;
    int    accuracy;
    String address;     

};

#endif
