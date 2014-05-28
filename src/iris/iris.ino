#include <Adafruit_GPS.h>
#include "IrisCore.h"
#include "OverrideHandler.h"
#include "ServoHandler.h"
#include "GPSHandler.h"

OverrideHandler overrideHandler(A0, 2);
ServoHandler servoHandler(DAC0, A1);
GPSHandler gpsHandler;

IrisCore irisCore;

void setup()
{
  irisCore.debug("");
  irisCore.debug("****************************");
  irisCore.debug("Staring Iris v0.9 beta...");
  irisCore.debug("****************************");
  irisCore.debug("Microcontroller: AT91SAM3X8E");
  irisCore.debug("Clock speed: 84 MHz");
  irisCore.debug("SRAM: 96 KB");
  irisCore.debug("Flash Memory: 512 KB");
  irisCore.debug("****************************"); 
  irisCore.debug("");
}

void loop()
{
//  int val = overrideHandler.getOverrideValue();
//  servoHandler.move(val);
//  servoHandler.getFeedback();
  gpsHandler.hasFix();
}             
