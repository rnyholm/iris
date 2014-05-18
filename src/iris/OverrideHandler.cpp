#include <Arduino.h>
#include "OverrideHandler.h"

OverrideHandler::OverrideHandler(int pinInput, int pinEnable)
{
  _pinInput = pinInput;
  _pinEnable = pinEnable; 

  analogReadResolution(RESOLUTION_READ);
  pinMode(_pinEnable, INPUT);

  _readingsIndex = 0;
  _readingsTotal = 0;
  _readingsAverage = 0;  

  for (int i = 0; i < NUM_READINGS; i++) {
    _readings[i] = 0;
  }
}

int OverrideHandler::getOverrideValue()
{
  if (isOverrideable() == 1) {
    // Remove the last reading
    _readingsTotal -= _readings[_readingsIndex];
    // Read from input and add readings to total readings
    _readings[_readingsIndex] = analogRead(_pinInput);
    _readingsTotal += _readings[_readingsIndex];

    // Calculate an average value for the readings and write it to ppin
    _readingsAverage = _readingsTotal / NUM_READINGS;

    // Increase the index and if needed reset it
    _readingsIndex += 1;
    if (_readingsIndex >= NUM_READINGS) {
      _readingsIndex = 0;
    }
    
    _irisCore.debug(String("Average override value: ") + String(_readingsAverage));

    return _readingsAverage;
  }

  _irisCore.debug("System not overrideable, returning -1");
  return -1;
}

int OverrideHandler::isOverrideable()
{
  return digitalRead(_pinEnable) == HIGH ? 1 : 0;
}
