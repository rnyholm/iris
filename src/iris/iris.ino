#include "JOYSTICK_READ_A0.h"
#include "IrisCore.h"

JOYSTICK_READ_A0 joystick;
IrisCore irisCore;

void setup()
{
}

void loop()
{
  irisCore.debug(joystick.getReading());
}
