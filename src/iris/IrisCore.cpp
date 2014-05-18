/* 
 File.......: IrisCore.cpp
 Author.....: Robert Nyholm <robert.nyholm@aland.net>
 Date.......: May 17, 2014
 Description: Library holding definitions, classes and other
              functionality shared within the components of application Iris.
*/
#include <Arduino.h>
#include <stdio.h>
#include "IrisCore.h"

IrisCore::IrisCore()
{
#ifdef DEBUG
  // Only set variable to 0 if serial connection hasn't been initialized already
  if (_serialInitialized != 1)
  {
    _serialInitialized = 0; 
  }
#endif
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

void IrisCore::debug(const char* charSeq)
{
#ifdef DEBUG
  initSerial();
  Serial.println(charSeq);
#endif
}

void IrisCore::debug(StringSumHelper& charSeq)
{
#ifdef DEBUG
  initSerial();
  Serial.println(charSeq);
#endif
}

/* To initialize(begin) Serial communication at defined baud rate*/
void IrisCore::initSerial()
{
  if (_serialInitialized == 0)
  {
    Serial.begin(BAUD_RATE);
    _serialInitialized = 1;
  }
}
