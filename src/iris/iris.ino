#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

/*
 * It's desired to fire the different tasks with specific intervals.
 * Ex. Fire GPS task with 2000ms intervals and so on...
 */
#define GPS_TASK_INTERVAL 2000
#define SERVO_TASK_INTERVAL 50
#define TFT_TASK_INTERVAL 1000

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
#define TFT_CS 10
#define TFT_RESET 9
#define SD_CS 4

// Different baud rates and serial stuff
#define BAUD_RATE 115200   // Desired baud rate for serial debug communication
#define GPS_BAUD_RATE 9600 // Seems to be the standard for Adafruits MTK GPS's, could use 4800 also
#define GPS_SERIAL Serial1 // DUE has 3 hardware USART so we can use one hardware serial for the GPS

// Colors (RGB565)
#define ARDUINO_BLUE 0x04B3
#define IRIS_GRAY 0xC618

// Text stuff
#define NORMAL_LINE_SPACE 16
#define LARGER_LINE_SPACE 32

#define BUFFPIXEL 20

// Different timers for the different tasks
unsigned long timerServo = 0;
unsigned long timerGPS = 0;
unsigned long timerTFT = 0;

// Controller(PID) parameters
double Kp = 0.1;
double Ki = 0.01;
double Kd = 0.005;

// It's nice to know if SD card has been initialized correctly
boolean sdInitialized = false;

// Must store last valid given servo position
int lastValidPosition;

TinyGPSPlus gps;
Adafruit_RA8875 tft = Adafruit_RA8875(TFT_CS, TFT_RESET);
Sd2Card sd;
File file;

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

  Serial.println(F("Initializing TFT with resolution 800x480..."));
  initTFT();
  Serial.println(F("TFT successfully initialized..."));

  Serial.println(F("Initializing SD card..."));
  if (!SD.begin(SD_CS))
  {
    Serial.println(F("Failed to initialize SD card!"));
  }
  else
  {
    sdInitialized = true;
    Serial.println(F("SD card successfully initialized..."));
  }

  Serial.println(F("Setting other parameters..."));
  // Set las valid position to center
  lastValidPosition = CENTER_POSITION;
  Serial.println(F("Parameters successfully set..."));

  Serial.println(F("Iris successfully started, happy autopiloting ;-)"));
  Serial.println(F("**************************************************\n"));

  // Now other stuff that's supposed to run only once executed
  showSplash();
  //drawText();
  drawGui();
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

  // TFT task
  if (millis() - timerTFT >= TFT_TASK_INTERVAL)
  {
    timerTFT = millis();
    taskTFT();
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
  // Ensure that nav mode is updated in gui
  drawNavMode();

  if (isManualMode())
  {
    moveServo(getJoystickValue());
  }
}

void taskTFT()
{
  drawNavMode();
  drawNavSource();
  drawGPSData();
  drawCompassData();
  drawControllerData();
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
    time += (String)gps.time.hour() + ":";

    // ...minutes...
    if (gps.time.minute() < 10)
    {
      time += "0";
    }
    time += (String)gps.time.minute() + ":";

    // ...and seconds
    if (gps.time.second() < 10)
    {
      time += "0";
    }
    time += (String)gps.time.second();

    return time;
  }

  return "-";
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
  return "-";
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
      return (String)latitude + "N";
    }
    return latitude;
  }
  return "-";
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
      return (String)longitude + "E";
    }
    return longitude;
  }
  return "-";
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
    return (String)gps.speed.knots() + " knots";
  }
  return "-";
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
    return (String)gps.course.deg() + " deg";
  }
  return "-";
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
    return (String)gps.altitude.meters() + " m";
  }
  return "-";
}

/*
 * To get the number of satellites the GPS
 * has contact with as a String.
 */
String getGPSSatellites()
{
  if (gps.speed.isValid())
  {
    return (String)gps.satellites.value();
  }
  return "-";
}

/*
 * To figure out if the GPS got enough satellites
 * in order to establish a good position, a 
 * minimum of 3 satellites are needed for this.
 * True will be returned if GPS got 3 or more satellites
 * else false is returned.
 */
boolean isGPSValidForNavigation()
{
  if (gps.speed.isValid())
  {
    if (gps.satellites.value() > 2)
    {
      return true;
    }
  }
  return false;
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
    Serial.println("Moving servo to position: " + (String)position);
    lastValidPosition = position;
    analogWrite(SERVO_PIN, position);
  }
  else
  {
    Serial.println("Wanted servo position is invalid, moving servo to last valid position: " + (String)lastValidPosition);
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
  Serial.println("Feedback: " + (String)feedback);
  return feedback;
}

/*************************************************
 *              TFT functionality              *
 *************************************************/

/*
 * To initialize the tft screeen. The screen will
 * be initialized at a resolution of 800x480. If
 * the screen cannot be initialzed the system will halt.
 */
void initTFT()
{
  // Initialise the display using 'RA8875_800x480'
  if (!tft.begin(RA8875_800x480))
  {
    Serial.println(F("TFT Not Found!"));
    while (1);
  }
  else
  {
    Serial.print(F("Successfully communicating with screen over SPI with CS pin: ")); Serial.println(TFT_CS);
    Serial.print(F("Using screen resolution: ")); Serial.println(RA8875_800x480);
  }
  // Enable tft
  tft.displayOn(true);
  tft.GPIOX(true); // Enable TFT - display enable tied to GPIOX

  Serial.println("Turning on screen...");

  // Config pwm, mainly backlight is controlled with pwm
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(0);
}

/*
 * To display a splash screen upon system boot.
 * The splash screen will be dimmed up from black, then
 * showed for about 3 seconds to be dimmed down to black
 * again. If SD card couldn't be found or some error
 * occurred with that, no splash screen will be shown.
 */
void showSplash()
{
  // Ensure screen is "black" using PWM
  tft.PWM1out(0);

  // If SD card has been initialized show splash
  if (sdInitialized)
  {
    Serial.println(F("SD card initialized, setting background color and loading splash images..."));
    // Set correct background color
    tft.fillScreen(ARDUINO_BLUE);

    // Load bmp's
    bmpDraw("iris.bmp", 234, 40);
    bmpDraw("desc.bmp", 201, 169);

    Serial.println(F("Images loaded, dimming in and out screen..."));

    // We want to fade up the screen using pwm, nice
    fadeInTFT(1, 10);

    // Show Splash for 3 seconds
    delay(3000);

    // And now we fade it down
    fadeOutTFT(1, 10);
  }
  Serial.println(F("Preparing for data rendering, setting background color to black..."));

  // This point the tft should be "black" and empty, in other words prepared for the next step
  tft.fillScreen(RA8875_BLACK);
}

/*
 * To draw the main and mostrly static user interface.
 * This is only needed to be done once, after that it should
 * remain the same.
 */
void drawGui()
{
  tft.graphicsMode();
  drawDoubleVLine(200, 5, 200, 475, ARDUINO_BLUE);
  drawDoubleHLine(5, 168, 195, 168, ARDUINO_BLUE);
  drawDoubleHLine(5, 277, 195, 277, ARDUINO_BLUE);

  tft.textMode();
  tft.textTransparent(RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textSetCursor(206, 0);
  tft.textWrite("Navigation");
  
  tft.textSetCursor(5, tft.height() - NORMAL_LINE_SPACE - 3);
  tft.textEnlarge(0);
  tft.textWrite("Iris - v0.8beta");

  tft.PWM1out(255);
}

void drawNavMode()
{
  tft.textMode();
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(206, LARGER_LINE_SPACE);
  tft.textEnlarge(0);

  if (isManualMode())
  {
    tft.textWrite("Mode: Manual");
  }
  else
  {
    tft.textWrite("Mode: Auto  ");
  }
}

void drawNavSource()
{
  tft.textMode();
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(400, LARGER_LINE_SPACE);
  tft.textEnlarge(0);

  if (isGPSValidForNavigation())
  {
    tft.textWrite("Source in use: GPS    ");
  }
  else
  {
    tft.textWrite("Source in use: Compass");
  }
}

void drawGPSData()
{
  tft.textMode();
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(5, 0);
  tft.textEnlarge(1);

  tft.textWrite("GPS Data");

  tft.textEnlarge(0);
  tft.textSetCursor(5, LARGER_LINE_SPACE);
  tft.textWrite(("Time(UTC):  " + getGPSTime()).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE);
  tft.textWrite(("Date:       " + getGPSDate()).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 2);
  tft.textWrite("Location(lat,lon):");
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 3);
  tft.textWrite(getGPSLocation(true).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 4);
  tft.textWrite(("Speed:      " + getGPSSpeed()).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 5);
  tft.textWrite(("Course:     " + getGPSCourse()).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 6);
  tft.textWrite(("Alitude:    " + getGPSAltitude()).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 7);
  tft.textWrite(("Satellites: " + getGPSSatellites() + "  ").c_str());
}

void drawCompassData()
{
  tft.textMode();
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(5, 171);
  tft.textEnlarge(1);

  tft.textWrite("Compass Data");
  tft.textEnlarge(0);
  tft.textSetCursor(5, LARGER_LINE_SPACE + 171);
  tft.textWrite("Course: xxx.xx deg");
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE + 171);
  tft.textWrite("X:      xxx.xx");
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 2 + 171);
  tft.textWrite("Y:      xxx.xx");
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 3 + 171);
  tft.textWrite("Z:      xxx.xx");
}

void drawControllerData()
{
  // Buffers to store the "String" version of the PID values
  char KpStr [5];
  char KiStr [5];
  char KdStr [5];

  // Casting the doubles to "strings" without loosing any precision
  dtostrf(Kp, 2, 3, KpStr);
  dtostrf(Ki, 2, 3, KiStr);
  dtostrf(Kd, 2, 3, KdStr);

  tft.textMode();
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(5, 280);
  tft.textEnlarge(1);

  tft.textWrite("Controller");
  tft.textEnlarge(0);
  tft.textSetCursor(5, LARGER_LINE_SPACE + 280);
  tft.textWrite(("P = " + (String)KpStr).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE + 280);
  tft.textWrite(("I = " + (String)KiStr).c_str());
  tft.textSetCursor(5, LARGER_LINE_SPACE + NORMAL_LINE_SPACE * 2 + 280);
  tft.textWrite(("D = " + (String)KdStr).c_str());
}

void drawText()
{
  tft.PWM1out(255);
  tft.textMode();

  tft.textTransparent(RA8875_WHITE);
  tft.textSetCursor(5, 0);
  tft.textEnlarge(0);
  tft.textWrite("Våöäåvdfkvnoavkpav*!--<><</)0987, consectetur adipisicing elit..."); // Standard
  tft.textSetCursor(5, 17);
  tft.fontArial();
  tft.textWrite("Lorem ipsum dolor sit amet, consectetur adipisicing elit...");
  tft.textSetCursor(5, 34);
  tft.fontRoman();
  tft.textWrite("Lorem ipsum dolor sit amet, consectetur adipisicing elit...");
  tft.textSetCursor(0, 51);
  tft.fontStandard();
  tft.textEnlarge(1);
  tft.textWrite("Lorem ipsum dolor sit amet, consectetur adipisicing elit..."); // Standard
  tft.textSetCursor(0, 85);
  tft.textEnlarge(2);
  tft.textWrite("Lorem ipsum dolor sit amet, consectetur adipisicing elit..."); // Standard
  tft.textSetCursor(0, 130);
  tft.textEnlarge(3);
  tft.textWrite("Lorem ipsum dolor sit amet, consectetur adipisicing elit..."); // Standard
}

/*
 * Turns TFT black with pwm and then fades it in again with
 * given steps as increment and delays each increment with
 * given delay.
 */
void fadeInTFT(int steps, int del)
{
  // Ensure screen is "black" using PWM
  tft.PWM1out(0);

  for (int i = 1; i < 256; i += steps)
  {
    tft.PWM1out(i);
    delay(del);
  }
}

/*
 * Turns TFT fully on with pwm and then fades it out again with
 * given steps as increment and delays each increment with
 * given delay.
 */
void fadeOutTFT(int steps, int del)
{
  // Ensure screen is fully on using PWM
  tft.PWM1out(255);

  for (int i = 255; i > -1; i -= steps)
  {
    tft.PWM1out(i);
    delay(del);
  }
}

/*
 * Convenience function that draws two lines side by side
 * by drawing two lines. One at the given x values and the
 * other at given x value + 1.
 */
void drawDoubleVLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  tft.drawLine(x0, y0, x1, y1, color);
  tft.drawLine(x0 + 1, y0, x1 + 1, y1, color);
}

/*
 * Convenience function that draws two lines side by side
 * by drawing two lines. One at the given y values and the
 * other at given y value + 1.
 */
void drawDoubleHLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  tft.drawLine(x0, y0, x1, y1, color);
  tft.drawLine(x0, y0 + 1, x1, y1 + 1, color);
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
  if (timerGPS > millis())
  {
    timerGPS = millis();
  }
  if (timerServo > millis())
  {
    timerServo = millis();
  }
  if (timerTFT > millis())
  {
    timerTFT = millis();
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

/*
 * Function dtostrf is only available on avr. This little function
 * emulates that one so it could be used on arm.
 */
char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

/*
 * To get bmp file from SD card named as given filename and then draw it
 * on TFT at given points x and y. If bmp file couldn't be found on the
 * SD card function will return without any further actions. The same happens
 * opened bmp file doesn't have a bit depth of 24bit, also if given
 * x and y points are larger than TFT's dimension this function will return.
 * Function snatched from Adafruits "spitftbitmap" example the Adafruit_ILI9340 library.
 */
void bmpDraw(char *filename, uint8_t x, uint8_t y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File '")); Serial.print(filename); Serial.println(F("' couldn't be found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: ")); Serial.print(bmpWidth); Serial.print(F("x")); Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())
        {
          w = tft.width()  - x;
        }
        if ((y + h - 1) >= tft.height())
        {
          h = tft.height() - y;
        }

        for (row = 0; row < h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.drawPixel(x + col, y + row, color565(r, g, b));
          }
        }
      }
    }
  }

  bmpFile.close();
  if (!goodBmp)
  {
    Serial.println(F("BMP format not recognized."));
  }
}

/*
 * To read out 16 bit from given file.
 * Function snatched from Adafruits "spitftbitmap" example
 * from the Adafruit_ILI9340 library.
 */
uint16_t read16(File & f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

/*
 * To read out 32 bit from given file.
 * Function snatched from Adafruits "spitftbitmap" example
 * from the Adafruit_ILI9340 library.
 */
uint32_t read32(File & f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

/*
 * To get 16 bit packed color from given 8 bit R,G,B values.
 * Function snatched from the Adafruit_ILI9340 library.
 */
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}