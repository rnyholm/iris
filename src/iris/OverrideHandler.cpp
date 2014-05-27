/* 
 * File.......: OverrideHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 17, 2014
 * Description: Library for handling the reading from specified input pin.
 *              This value is then supposed to be used for manual override
 *              of the linear actuator, if possible.
 */
#include <Arduino.h>
#include "OverrideHandler.h"

/* 
 * Creates a new OverridHandler object.
 * Param: pinInput - pin to read analog input from.
 * Param: pinOverrideable - pin from which if set HIGH this object is overrideable.
 */
OverrideHandler::OverrideHandler(int pinInput, int pinOverrideable)
{
  // Store pins to be used as overrideable and input pins
  _pinInput = pinInput;
  _pinOverrideable = pinOverrideable; 

  // Set pins and A/D resolution
  analogReadResolution(RESOLUTION_READ);
  pinMode(_pinOverrideable, INPUT);
}

/*
 * To get the averaged, override A/D value.
 * Return Averaged A/D value if this object is overrideable, else -1.
 */
int OverrideHandler::getOverrideValue()
{
  // Only get and calculate A/D reading if this object is overrideable, takes away some unnecessary from the microcontroller
  if (isOverrideable() == 1) {
    return analogRead(_pinInput);
  }
  
  _irisCore.debug("System not overrideable, returning -1");
  return -1;
}

/*
 * To figure out if this object is overrideable.
 * Return 1 if overrideable else 0.
 */
int OverrideHandler::isOverrideable()
{
  return digitalRead(_pinOverrideable) == HIGH ? 1 : 0;
}
