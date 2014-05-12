#include <Arduino.h>
#include "JOYSTICK_READ_A0.h"

const int NUM_READINGS = 10;

// Pin and resolution constants
const int STICK_PIN = A0;
const int RESOLUTION_READ = 12;

int stickReadings[NUM_READINGS];
int stickReadingsIndex = 0;
int stickReadingsTotal = 0;
int stickReadingsAverage = 0;

/*
 * Constructs a new joystick reading object.
 */
JOYSTICK_READ_A0::JOYSTICK_READ_A0() {
  analogReadResolution(RESOLUTION_READ);

  for (int i = 0; i < NUM_READINGS; i++) {
    stickReadings[i] = 0;
  }
}

JOYSTICK_READ_A0::~JOYSTICK_READ_A0() {
  // DO NOTHING!
}

int JOYSTICK_READ_A0::getReading() {
  // Remove the last reading
  stickReadingsTotal -= stickReadings[stickReadingsIndex];
  // Read from sensor and add readings to joysticks total readings
  stickReadings[stickReadingsIndex] = analogRead(STICK_PIN);
  stickReadingsTotal += stickReadings[stickReadingsIndex];

  // Calculate an average value for the readings and write it to ppin
  stickReadingsAverage = stickReadingsTotal / NUM_READINGS;

  // Increase the index and if needed reset it
  stickReadingsIndex += 1;
  if (stickReadingsIndex >= NUM_READINGS) {
    stickReadingsIndex = 0;
  }

  return stickReadingsAverage;
}
