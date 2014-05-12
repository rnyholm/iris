#ifndef JOYSTICK_READ_A0_H_
#define JOYSTICK_READ_A0_H_

#include <Arduino.h>

class JOYSTICK_READ_A0 {
  public:
    JOYSTICK_READ_A0();
    ~JOYSTICK_READ_A0();
    int getReading();
};

#endif
