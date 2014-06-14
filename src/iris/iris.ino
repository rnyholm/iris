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
#define TFT_TASK_INTERVAL 500

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

#define BUFFPIXEL 20

// Different timers for the different tasks
unsigned long timerServo = 0;
unsigned long timerGPS = 0;
unsigned long timerTFT = 0;

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
  if (!tft.begin(RA8875_800x480))
  {
    Serial.println(F("TFT Not Found!"));
    while (1);
  }
  initTFT();
  Serial.println(F("TFT successfully initialized..."));

  Serial.println(F("Initializing SD card..."));
  if (!SD.begin(SD_CS))
  {
    Serial.println(F("Failed to initialize SD card!"));
    while (1);
  }
  Serial.println(F("SD card successfully initialized..."));

  Serial.println(F("Setting other parameters..."));
  // Set las valid position to center
  lastValidPosition = CENTER_POSITION;
  Serial.println(F("Parameters successfully set..."));

  bmpDraw("iris.bmp", 234, 40);
  bmpDraw("desc.bmp", 201, 169);
  
//  tft.textMode();
//  tft.textSetCursor(235, 40);
//  tft.textEnlarge(3);
//  tft.textTransparent(RA8875_WHITE);
//  tft.textWrite("Iris");

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
  if (isManualMode())
  {
    moveServo(getJoystickValue());
  }
}

void taskTFT()
{

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
    time += (String)gps.time.minute();

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
      return (String)latitude + "N";
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
      return (String)longitude + "E";
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
    return (String)gps.speed.knots() + " knots";
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
    return (String)gps.course.deg() + "deg";
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
    return (String)gps.altitude.meters() + "m";
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
    return (String)gps.satellites.value();
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

void initTFT()
{
  // Enable tft
  tft.displayOn(true);
  tft.GPIOX(true); // Enable TFT - display enable tied to GPIOX

  // Config pwm, mainly backlight is controlled with pwm
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  // background color to white
  tft.fillScreen(ARDUINO_BLUE);
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

void bmpDraw(char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        //tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            //tft.pushColor(tft.Color565(r,g,b));
            tft.drawPixel(x+col, y+row, color565(r, g, b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File & f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File & f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
