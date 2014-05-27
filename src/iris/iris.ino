#include "IrisCore.h"
#include "OverrideHandler.h"
#include "ServoHandler.h" 

OverrideHandler overrideHandler(A0, 2);
ServoHandler servoHandler(DAC0, A1);

IrisCore irisCore;

void setup()
{
  irisCore.debug("");
  irisCore.debug("System:");
  irisCore.debug("Microcontroller: AT91SAM3X8E");
  irisCore.debug("Clock speed: 84 MHz");
  irisCore.debug("SRAM: 96 KB");
  irisCore.debug("Flash Memory: 512 KB");
  irisCore.debug("****************************");
  irisCore.debug("Staring Iris v0.9 beta");
  irisCore.debug("Iris v0.9 beta started");
  irisCore.debug("****************************");
  irisCore.debug("");
  servoHandler.moveToMax();
}

void loop()
{
  //int val = overrideHandler.getOverrideValue();
  //servoHandler.move(val);
  //servoHandler.getFeedback();
  
  if (servoHandler.getFeedback() == MAX_FEEDBACK)
  {
    servoHandler.moveToMin();
  }
  
  if (servoHandler.getFeedback() == MIN_FEEDBACK)
  {
    servoHandler.moveToMax();
  } 
}
