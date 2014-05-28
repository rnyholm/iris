/*
 * File.......: GPSHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 28, 2014
 * Description: Library for handling GPS functionality.
 */
#ifndef GPS_HANDLER_H_
#define GPS_HANDLER_H_

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "IrisCore.h"

#define GPS_SERIAL Serial1 // DUE has 3 hardware USART so we can use one hardware serial for the GPS
#define GPS_BAUD_RATE 9600 // Seems to be the standard for Adafruits MTK GPS's, could use 4800 also

class GPSHandler {
  public:
    GPSHandler();
    int hasFix();
    int getFix();
    int getFixQuality();
    String getTime();
    String getDate();
    String getPosition();
    String getLongitude();
    String getLatitude();
    String getSpeed();
    String getHeading();
    String getAltitude();
    String getSatellites();
    void toSerial();
  private:
    IrisCore _irisCore;
    Adafruit_GPS _GPS;
    int _configured;
    void configureGPS();
};

#endif
