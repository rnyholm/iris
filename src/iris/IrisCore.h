/* 
 * File.......: IrisCore.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 17, 2014
 * Description: Library holding definitions, classes and other
 *              functionality shared within the components of application Iris.
 */
#ifndef IRIS_CORE_H_
#define IRIS_CORE_H_

#include <Arduino.h>

#define DEBUG                // Comment this out to disable debug printings throughout the application
#define BAUD_RATE 115200     // Desired baud rate for serial debug communication
#define GPS_SERIAL Serial1   // DUE has 3 hardware USART so we can use one hardware serial for the GPS
#define GPS_BAUD_RATE 9600   // Seems to be the standard for Adafruits MTK GPS's, could use 4800 also

class IrisCore {
  public:
    IrisCore();
    void debug(int i);
    void debug(double d);
    void debug(const char* charSeq);
    void debug(StringSumHelper& charSeq);
  private:
    int _serialInitialized;
    void initSerial();
};

#endif
