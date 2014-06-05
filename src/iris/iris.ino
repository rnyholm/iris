#include <Adafruit_GPS.h>
#include <TinyGPS++.h>
#include "IrisCore.h"
#include "OverrideHandler.h"
#include "ServoHandler.h"
//#include "GPSHandler.h"

OverrideHandler override(A0, 2);
ServoHandler servo(DAC0, A1);;
//Adafruit_GPS gps(GPS_SERIAL);
TinyGPSPlus gps;

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

  GPS_SERIAL.begin(GPS_BAUD_RATE);
  // configureGPS();
}

void loop()
{
  //updateGPSData();
  displayInfo();
  //  int val = override.getOverrideValue();
  //  servo.move(val);
  //  servo.getFeedback();
  smartDelay(250);
}

void updateGPSData()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
  {
    gps.encode(Serial1.read());
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available()>0)
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

//void configureGPS()
//{
//    // Set correct baud rates to the gps and gps's USART connection
//    _gps.begin(GPS_BAUD_RATE);
//    GPS_SERIAL.begin(GPS_BAUD_RATE);
//    // Use RMC(recommended minimum) and GGA(fix data) including altitude
//    _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
//
//    // Set update rate of GPS to 1 Hz
//    _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
//
//    // Request updates on antenna status, this could be commented out
//    _gps.sendCommand(PGCMD_ANTENNA);
//
//    _iris.debug("GPS has been successfully configured...");
//
//    _configured = 1;
//}
//
//void checkGPSData()
//{
//  // If a sentence is received, we can check the checksum, parse it...
//  if (GPS.newNMEAreceived()) {
//    // If parsing
//    if (!GPS.parse(GPS.lastNMEA()))
//    {
//        return;  // we can fail to parse a sentence in which case we should just wait for another
//    }
//}

