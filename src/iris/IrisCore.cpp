#include <Arduino.h>
#include "IrisCore.h"

IrisCore::IrisCore() 
{
#ifdef DEBUG
  _serialInitialized = 0;
#endif
}

IrisCore::~IrisCore()
{
  // No need to deconstruct anything, an object of this is never instantiated
}

void IrisCore::debug(int i)
{
#ifdef DEBUG
  initSerial();
  Serial.println(i);
#endif	
}

void IrisCore::debug(double d)
{
#ifdef DEBUG
  initSerial();
  Serial.println(d);
#endif
}

void IrisCore::debug(char c)
{
#ifdef DEBUG
  initSerial();
  Serial.println(c);
#endif
}

void IrisCore::debug(char cArr[])
{
#ifdef DEBUG
  initSerial();
  Serial.println(cArr);
#endif
}

void IrisCore::initSerial()
{
  if(_serialInitialized == 0) 
  {
    Serial.begin(BAUD_RATE);
    _serialInitialized = 1;
  }
}
