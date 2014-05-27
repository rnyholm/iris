/*
 * File.......: ServoHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 26, 2014
 * Description: Library for handling movements of linear actuator(servo),
 * 		and feedback from it.
 */
#ifndef SERVO_HANDLER_H_
#define SERVO_HANDLER_H_

#include <Arduino.h>
#include "IrisCore.h"

#define RESOLUTION_WRITE 12 // Number of bits we want to use as resolution reading analog input

// Servo positions (analog values)
#define MAX_POSITION 4040    // Maximum position
#define CENTER_POSITION 2020 // Center position
#define MIN_POSITION 0	     // Minimum position

/*
 * Servo feedback values.
 * Min and max values are specific when feedback is derived with usage of 5V.
 */
#define MAX_FEEDBACK 3340     // Analog value of maximum feedback
#define CENTER_FEEDBACK 2890  // Analog value of center feedback
#define MIN_FEEDBACK 2440     // Analog value of minimum feedback

class ServoHandler {
  public:
    ServoHandler(int pinServo, int pinFeedback);
    void move(int position);
    void moveToMax();
    void moveToCenter();
    void moveToMin();
    void stop();
    int getFeedback();
  private:
    IrisCore _irisCore;
    int _pinServo;
    int _pinFeedback;
    int _lastValidPosition;
};

#endif
