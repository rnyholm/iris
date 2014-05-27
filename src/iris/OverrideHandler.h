/* 
 * File.......: OverrideHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 17, 2014
 * Description: Library for handling the reading from specified input pin.
 *              This value is then supposed to be used for manual override
 *              of the linear actuator, if possible.
 */
#ifndef OVERRIDE_HANDLER_H_
#define OVERRIDE_HANDLER_H_

#include <Arduino.h>
#include "IrisCore.h"

#define RESOLUTION_READ 12       // Number of bits we want to use as resolution reading analog input

class OverrideHandler {
  public:
    OverrideHandler(int pinInput, int pinEnable);
    int getOverrideValue();
    int isOverrideable();
  private:
    IrisCore _irisCore;
    int _pinInput;
    int _pinOverrideable; 
};

#endif
