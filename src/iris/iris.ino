#include "JOYSTICK_READ_A0.h"

JOYSTICK_READ_A0 joystick;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(joystick.getReading());
}
