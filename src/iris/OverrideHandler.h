/**
 *
 */
#ifndef OVERRIDE_HANDLER_H_
#define OVERRIDE_HANDLER_H_

#include <Arduino.h>
#include "IrisCore.h"

#define RESOLUTION_READ 12
#define NUM_READINGS 10

class OverrideHandler {
  public:
    OverrideHandler(int pinInput, int pinEnable);
    int getOverrideValue();
    int isOverrideable();
  private:
    IrisCore _irisCore;
  
    int _pinInput;
    int _pinEnable;
    
    int _readings[NUM_READINGS];
    int _readingsIndex;
    int _readingsTotal;
    int _readingsAverage;    
};

#endif
