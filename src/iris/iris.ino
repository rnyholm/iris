#include <Adafruit_GPS.h>
#include "IrisCore.h"
#include "OverrideHandler.h"
#include "ServoHandler.h"
#include "GPSHandler.h"

OverrideHandler override(A0, 2);
ServoHandler servo(DAC0, A1);
GPSHandler gps;

IrisCore iris;

void setup()
{
  iris.debug("");
  iris.debug("****************************");
  iris.debug("Staring Iris v0.9 beta...");
  iris.debug("****************************");
  iris.debug("Microcontroller: AT91SAM3X8E");
  iris.debug("Clock speed: 84 MHz");
  iris.debug("SRAM: 96 KB");
  iris.debug("Flash Memory: 512 KB");
  iris.debug("****************************"); 
  iris.debug("");
}

void loop()
{
//  int val = override.getOverrideValue();
//  servo.move(val);
//  servo.getFeedback();
  iris.debug("Fix: " + (String)gps.getFix() + ", quality: " + (String)gps.getFixQuality());
  iris.debug("Date: " + gps.getDate());
  iris.debug("Time(UTC): " + gps.getTimeUTC());
  iris.debug("Time(EEST): " + gps.getTimeEEST());
  iris.debug("Longitude: " + gps.getLongitude());
  iris.debug("Latitude: " + gps.getLatitude());
  
  delay(2000);
}             
