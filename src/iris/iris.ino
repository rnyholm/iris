#include <TinyGPS++.h>
#include "IrisCore.h"
#include "OverrideHandler.h"
#include "ServoHandler.h"

OverrideHandler override(A0, 2);
ServoHandler servo(DAC0, A1);;
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
}

void loop()
{
  printGPSData();
  smartDelay(2000);
}

/*
 * A clever little delay function, which will feed
 * our GPS object with data from GPS device while
 * delaying, nice;-)
 */
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SERIAL.available() > 0)
      gps.encode(GPS_SERIAL.read());
  } while (millis() - start < ms);
}

/*
 * To print all GPS data thats interesting for this
 * application to the serial. Mostly this function is
 * used for debugging.
 */
void printGPSData()
{
  Serial.print(F("\nTime: ")); Serial.println(getGPSTime());
  Serial.print(F("Date: ")); Serial.println(getGPSDate());
  Serial.print(F("Location: ")); Serial.println(getGPSLocation(true));
  Serial.print(F("Speed: ")); Serial.println(getGPSSpeed());
  Serial.print(F("Course: ")); Serial.println(getGPSCourse());
  Serial.print(F("Altitude: ")); Serial.println(getGPSAltitude());
  Serial.print(F("Satellites: ")); Serial.println(getGPSSatellites());
}

/*
 * To get current time from GPS as a String in
 * format hh:mm:ss, the timezone is UTC.
 */
String getGPSTime()
{
  String time;
  // Check that the current time is valid
  if (gps.time.isValid())
  {
    // Resolving hours...
    if (gps.time.hour() < 10)
    {
      time += "0";
    }
    time += (String) gps.time.hour() + ":";

    // ...minutes...
    if (gps.time.minute() < 10)
    {
      time += "0";
    }
    time += (String) gps.time.minute() + ":";

    // ...and seconds
    if (gps.time.second() < 10)
    {
      time += "0";
    }
    time += (String) gps.time.minute();
    
    return time;
  }
  
  return "Invalid time...";
}

/*
 * To get current date from GPS as a String in
 * format dd/mm/yyyy.
 */
String getGPSDate()
{
  if (gps.date.isValid())
  {
    return (String)gps.date.day() + "/" + (String)gps.date.month() + "/" + (String)gps.date.year();
  }
  return "Invalid date...";
}

/*
 * To get current location from GPS in Decimal Degree format.
 * Cardinal is also added to the string depending on argument passed in is true.
 * Ex. 60.1574572N,19.94766E
 */
String getGPSLocation(boolean showCardinals)
{
  return getGPSLatitude(showCardinals) + "," + getGPSLongitude(showCardinals);
}

/*
 * To get latitude in Decimal Degree format as a String, with a 
 * precision of six decimals. Cardinal is also added
 * to the string depending on argument passed in is true.
 * Ex. 60.1574572N
 */
String getGPSLatitude(boolean showCardinal)
{  
  if (gps.location.isValid())
  {
    char latitude [10];
    ftoa(latitude, gps.location.lat(), 6);
    if (showCardinal)
    {
      return (String) latitude + "N";
    }
    return latitude;
  }
  return "Invalid latitude...";
}

/*
 * To get longitude in Decimal Degree format as a String, with a 
 * precision of six decimals. Cardinal is also added
 * to the string depending on argument passed in is true.
 * Ex. 19.94766E
 */
String getGPSLongitude(boolean showCardinal)
{
  String longitude;
  if (gps.location.isValid())
  {
    char longitude [10];
    ftoa(longitude, gps.location.lng(), 6);
    if (showCardinal)
    {
      return (String) longitude + "E";
    }
    return longitude;
  }
  return "Invalid longitude...";
}

/*
 * To get speed from gps in knots as a String.
 * Unit will be added to the String.
 * Ex. 5.6 knots
 */
String getGPSSpeed()
{
  if (gps.speed.isValid())
  {
    return (String) gps.speed.knots() + " knots";
  }
  return "Invalid speed...";
}

/*
 * To get current course from GPS as a String.
 * Course is given in degrees. Unit will be added to the String.
 * Ex. 231.34deg
 */
String getGPSCourse()
{
  if (gps.speed.isValid())
  {
    return (String) gps.course.deg() + "deg";
  }
  return "Invalid course...";
}

/*
 * To get altitude in meters from GPS as a String.
 * Unit will be added to the String.
 * Ex. 50.2m
 */
String getGPSAltitude()
{
  if (gps.speed.isValid())
  {
    return (String) gps.altitude.meters() + "m";
  }
  return "Invalid altitude...";
}

/*
 * To get the number of satellites the GPS
 * has contact with as a String.
 */
String getGPSSatellites()
{
  if (gps.speed.isValid())
  {
    return (String) gps.satellites.value();
  }
  return "Invalid satellites...";
}

/*
 * Homebrew function to convert a given double to a char
 * array with given precision.
 * Snatched from the Arduino forum:
 *    http://forum.arduino.cc/index.php/topic,44262.0.html
 */
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};  
  char *ret = a;
  long l = (long)f;
  
  itoa(l, a, 10);
  while (*a != '\0') 
  {
    a++;
  }
  
  *a++ = '.';
  long d = abs((long)((f - l) * p[precision]));
  itoa(d, a, 10);
  
  return ret;
}
