/**
 * 
 */
#ifndef IRIS_CORE_H_
#define IRIS_CORE_H_

#include <Arduino.h>

//#define DEBUG
#define BAUD_RATE 9600

class IrisCore {
  public:
    IrisCore();
    ~IrisCore();
    void debug(int i);
    void debug(double d);
    void debug(char c); 
    void debug(char cArr[]);
  private:
    int _serialInitialized;
    void initSerial();
};

#endif
