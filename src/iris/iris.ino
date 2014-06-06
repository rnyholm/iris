#include <TinyGPS++.h>

/*
 * It's desired to fire the different tasks with specific intervals.
 * Ex. Fire GPS task with 2000ms intervals and so on...
 */
#define GPS_TASK_INTERVAL 2000  
#define SERVO_TASK_INTERVAL 50

// Servo positions (approx analog values)
#define MAX_POSITION 4040    // Maximum position
#define CENTER_POSITION 2020 // Center position
#define MIN_POSITION 0	     // Minimum position

// Different A/D resolutions
#define RESOLUTION_READ 12  // Number of bits we want to use as resolution reading analog input
#define RESOLUTION_WRITE 12 // Number of bits we want to use as resolution writing analog output

// Different pins
#define JOYSTICK_PIN A0
#define SERVO_PIN DAC0
#define SERVO_FEEDBACK_PIN A1
#define MODE_SWITCH_PIN 2

// Different baud rates and serial stuff
#define BAUD_RATE 115200   // Desired baud rate for serial debug communication
#define GPS_BAUD_RATE 9600 // Seems to be the standard for Adafruits MTK GPS's, could use 4800 also
#define GPS_SERIAL Serial1 // DUE has 3 hardware USART so we can use one hardware serial for the GPS

// Different timers for the different tasks
unsigned long timerServo = 0;
unsigned long timerGPS = 0;

// Must store last valid given servo position
int lastValidPosition;

TinyGPSPlus gps;

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.println(F("\n**************************************************"));
  Serial.println(F("Starting Iris v0.9beta...."));
  
  Serial.println("Beginning serial communication on: " + (String)GPS_SERIAL + " at: " + (String)GPS_BAUD_RATE + "baud...");
  GPS_SERIAL.begin(GPS_BAUD_RATE);
  Serial.println(F("Serial communication successfully started..."));
  
  Serial.println("Setting A/D r/w resolutions, read: " + (String)RESOLUTION_READ + " bits, write: " + (String)RESOLUTION_WRITE + " bits...");
  // Set A/D r/w resolutions
  analogReadResolution(RESOLUTION_READ);
  analogWriteResolution(RESOLUTION_WRITE);  
  Serial.println(F("Resolutions successfully set..."));
  
  Serial.println(F("Setting pins and their directions..."));
  // Setting pins and directions
  pinMode(MODE_SWITCH_PIN, INPUT);
  Serial.println(F("Pins and directions successfully set..."));
  
  Serial.println(F("Setting other parameters..."));
  // Set las valid position to center
  lastValidPosition = CENTER_POSITION;
  Serial.println(F("Parameters successfully set..."));
  
  Serial.println(F("Iris successfully started, happy autopiloting ;-)"));
  Serial.println(F("**************************************************\n"));
}

void loop()
{
  resetTimersIfNeeded();

  // GPS task...
  if (millis() - timerGPS >= GPS_TASK_INTERVAL)
  {
    timerGPS = millis();
    taskGPS();
  }

  // Servo task...
  if (millis() - timerServo >= SERVO_TASK_INTERVAL)
  {
    timerServo = millis();
    taskServo();
  }
  
  // It's desired to have as fresh data as possible in our gps object
  refreshGPSData();
}

/*************************************************
 *             The different tasks               *
 *************************************************/

/*
 * Task responsible for all GPS handling,
 * except refreshing of GPS data.
 */
void taskGPS()
{
  printGPSData();
}

/*
 * Task responsible for all interactions with
 * the systems servo, or more correctly, linear actuator.
 */
void taskServo()
{
  if (isManualMode())
  {
    Serial.println(getJoystickValue());
    moveServo(getJoystickValue());
  }
}

/*************************************************
 *               GPS functionality               *
 *************************************************/

/*
 * To refresh GPS data.
 */
void refreshGPSData()
{
  while (GPS_SERIAL.available() > 0)
    gps.encode(GPS_SERIAL.read());
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

/*************************************************
 *       Functionalities for manual input        *
 *************************************************/

/*
 * To get the A/D value from JOYSTICK_PIN,
 * in other words the joysticks position.
 * This value will only be returned if the
 * system is in manual mode, else -1 will
 * be returned
 */
int getJoystickValue()
{
  if (isManualMode() == 1) {
    return analogRead(JOYSTICK_PIN);
  }
  return -1;
}

/*
 * To figure out if the system is in manual
 * operation mode. If so true will be returned
 * else false.
 */
boolean isManualMode()
{
  return digitalRead(MODE_SWITCH_PIN) == HIGH ? true : false;
}

/*************************************************
 *              Servo functionality              *
 *************************************************/

/*
 * Moves servo to given position, if it's valid.
 */
void moveServo(int position)
{
  if (MIN_POSITION <= position && position <= MAX_POSITION) 
  {
    Serial.println("Moving servo to position: " + position);
    lastValidPosition = position;
    analogWrite(SERVO_PIN, position);
  } 
  else 
  {
    Serial.println("Wanted servo position is invalid, moving servo to last valid position: " + lastValidPosition);
    analogWrite(SERVO_PIN, lastValidPosition);
  }
}

/*
 * Moves servo to it's maximum position.
 */
void moveServoToMax()
{
  Serial.println("Moving servo to max position...");
  moveServo(MAX_POSITION);
}

/*
 * Moves servo to it's center position.
 */
void moveServoToCenter()
{
  Serial.println("Moving servo to center position...");
  moveServo(CENTER_POSITION);
}

/*
 * Moves servo to it's minimum position.
 */
void moveServoToMin()
{
  Serial.println("Moving servo to min position...");
  moveServo(MIN_POSITION);
}

/*
 * To geet feedback from servo.
 */
int getServoFeedback()
{
  int feedback = analogRead(SERVO_FEEDBACK_PIN);
  Serial.println("Feedback: " + feedback);
  return feedback;
}

/*************************************************
 *       Utilities and other functionality       *
 *************************************************/

/*
 * To reset the different timers used by
 * the different tasks. Reset is only needed
 * the timers has wrapped around.
 */
void resetTimersIfNeeded()
{
  // If our timers wrap around we reset them
  if (timerGPS > millis()) {
    timerGPS = millis();
  }
  if (timerServo > millis()) {
    timerServo = millis();
  }
}

/*
 * Homebrew function to convert a given double to a char
 * array with given precision.
 * Snatched from the Arduino forum:
 *    http://forum.arduino.cc/index.php/topic,44262.0.html
 */
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
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
