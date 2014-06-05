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
GPSHandler::GPSHandler() : _gps(&GPS_SERIAL)
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
    _iris.debug("GPS has achieved fix...");
    return 1;
  }

  _iris.debug("GPS has not achieved fix yet...");
  return 0;
}

/*
 * To get number of fixes the GPS got.
 * Return number of fixes.
 */
int GPSHandler::getFix()
{
  configureGPS();
  int fix = _gps.fix;
  _iris.debug("Number of fixes: " + String(fix));

  return fix;
}

/*
 * To get the fix quality.
 * Return quality of fix(as integer), if GPS has got any fix, else -1 is returned.
 */
int GPSHandler::getFixQuality()
{
  configureGPS();
  int fixQuality = -1;

  if (hasFix())
  {
    fixQuality = (int)_gps.fixquality;
    _iris.debug("Quality of fix: " + String(fixQuality));
  }
  else
  {
    _iris.debug("No quality of fix can be achieved");
  }

  return fixQuality;
}

/*
 * To get the current time in UTC.
 * Return current time in format hh:mm:ss, if GPS got fix, else an empty string is returned.
 */
String GPSHandler::getTimeUTC()
{
  configureGPS();
  String time = "";

  if (hasFix())
  {
    time = (String)_gps.hour + ":" + (String)_gps.minute + ":" + (String)_gps.seconds;
    _iris.debug("Current time(UTC): " + time);
  }
  else
  {
    _iris.debug("Current time cannot be resolved");
  }

  return time;
}

/*
 * To get the current time in EEST(UTC+3).
 * Return current time in format hh:mm:ss, if GPS got fix, else an empty string is returned.
 */
String GPSHandler::getTimeEEST()
{
  configureGPS();
  String time = "";

  if (hasFix())
  {
    time = (String)(_gps.hour + 3) + ":" + (String)_gps.minute + ":" + (String)_gps.seconds;
    _iris.debug("Current time(EEST): " + time);
  }
  else
  {
    _iris.debug("Current time cannot be resolved");
  }

  return time;
}

/*
 * To get the days date.
 * Return days date in format dd.mm-yyyy, if GPS got fix, else an empty string is returned.
 */
String GPSHandler::getDate()
{
  configureGPS();
  String date = "";

  if (hasFix())
  {
    date = (String)(_gps.day) + "." + (String)_gps.month + "-20" + (String)_gps.year;
    _iris.debug("Date: " + date);
  }
  else
  {
    _iris.debug("Date cannot be resolved");
  }

  return date;
}

/*
 * To get longitude of current position.
 * Return longitude, if GPS got fix, else -1.
 */
String GPSHandler::getLongitude()
{
  configureGPS();
  String longitude = "";

  if (hasFix())
  {
    longitude = (String)_gps.lon;
    _iris.debug("Longitude: " + longitude);
  }
  else
  {
    _iris.debug("Longitude cannot be resolved");
  }

  return longitude;
}

/*
 * To get latitude of current position.
 * Return latitude, if GPS got fix, else -1.
 */
String GPSHandler::getLatitude()
{
  configureGPS();
  String latitude = "";

  if (hasFix())
  {
    latitude = (String)_gps.lat;
    _iris.debug("Latitude: " + latitude);
  }
  else
  {
    _iris.debug("Latitude cannot be resolved");
  }

  return latitude;
}

/*
 * To configure the GPS. It's kind of important to run this method before any GPS.* call.
 */
void GPSHandler::configureGPS()
{
  _gps.parse(_gps.lastNMEA());
  if (_configured == 0)
  {
    // Set correct baud rates to the gps and gps's USART connection
    _gps.begin(GPS_BAUD_RATE);
    GPS_SERIAL.begin(GPS_BAUD_RATE);
    // Use RMC(recommended minimum) and GGA(fix data) including altitude
    _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set update rate of GPS to 1 Hz
    _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

    // Request updates on antenna status, this could be commented out
    _gps.sendCommand(PGCMD_ANTENNA);

    _iris.debug("GPS has been successfully configured...");

    _configured = 1;
  }
}
