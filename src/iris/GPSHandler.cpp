/*
 * File.......: GPSHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 28, 2014
 * Description: Library for handling GPS functionality.
 */
#include <Arduino.h>
#include "GPSHandler.h"

/*
 * Creates a new GPSHandler object.
 */
GPSHandler::GPSHandler() : _GPS(&GPS_SERIAL)
{
  // Only set variable to 0 if GPS hasn't been configured yet
  if (_configured != 1) 
  {
    _configured = 0;
  }
}

/*
 * To figure out if the GPS got fix or not.
 * Return 1 if GPS has fix, else 0.
 */
int GPSHandler::hasFix()
{
  int fix = getFix();

  if (fix > 0) {
    _irisCore.debug("GPS has achieved fix...");
    return 1;
  }

  _irisCore.debug("GPS has not achieved fix yet...");
  return 0;
}

/*
 * To get number of fixes the GPS got.
 * Return number of fixes.
 */
int GPSHandler::getFix()
{
  configureGPS();
  int fix = _GPS.fix;
  _irisCore.debug("Number of fixes: " + String(fix));

  return fix;
}

/*
 * To configure the GPS. It's kind of important to run this method before any GPS.* call.
 */
void GPSHandler::configureGPS()
{
  if (_configured == 0)
  {
    // Set correct baud rates to the gps and gps's USART connection
    _GPS.begin(GPS_BAUD_RATE);
    GPS_SERIAL.begin(GPS_BAUD_RATE);
    // Use RMC(recommended minimum) and GGA(fix data) including altitude
    _GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set update rate of GPS to 1 Hz
    _GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

    // Request updates on antenna status, this could be commented out
    _GPS.sendCommand(PGCMD_ANTENNA);
    
    _irisCore.debug("GPS has been successfully configured...");
    
    _configured = 1;
  }
}
