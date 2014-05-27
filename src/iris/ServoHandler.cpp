/*
 * File.......: ServoHandler.h
 * Author.....: Robert Nyholm <robert.nyholm@aland.net>
 * Date.......: May 26, 2014
 * Description: Library for handling movements of linear actuator(servo),
 * 		and feedback from it.
 */
#include <Arduino.h>
#include "ServoHandler.h"

/*
 * Creates a new ServoHandler object.
 * Param: pinServo - pin to control linear actuator from.
 * Param: pinFeedback - pin to get linear actuators position from(feedback).
 */
ServoHandler::ServoHandler(int pinServo, int pinFeedback)
{
  // Store pins to be used as linear actuator control and feedback pins
  _pinServo = pinServo;
  _pinFeedback = pinFeedback;

  // Set correct write resolution
  analogWriteResolution(RESOLUTION_WRITE);
  
  // Set las valid position to center
  _lastValidPosition = CENTER_POSITION;
}

/*
 * Moves servo to given position, if it's valid.
 * Param: position - desired position to move servo to.
 */
void ServoHandler::move(int position)
{
  if (MIN_POSITION <= position && position <= MAX_POSITION) 
  {
    _irisCore.debug(String("Moving servo to position: ") + String(position));
    _lastValidPosition = position;
    analogWrite(_pinServo, position);
  } else {
    _irisCore.debug(String("Wanted servo position is invalid, moving servo to last valid position: ") + _lastValidPosition);
    analogWrite(_pinServo, _lastValidPosition);
  }
}

/*
 * Moves servo to it's maximum position.
 */
void ServoHandler::moveToMax()
{
  _irisCore.debug("Moving servo to max position");
  move(MAX_POSITION);
}

/*
 * Moves servo to it's center position.
 */
void ServoHandler::moveToCenter()
{
  _irisCore.debug("Moving servo to center position");
  move(CENTER_POSITION);
}

/*
 * Moves servo to it's minimum position.
 */
void ServoHandler::moveToMin()
{
  _irisCore.debug("Moving servo to min position");
  move(MIN_POSITION);
}

/*
 * To geet feedback from servo.
 * Return feedback from servo.
 */
int ServoHandler::getFeedback()
{
  int feedback = analogRead(_pinFeedback);
  _irisCore.debug(String("Feedback: ") + feedback);
  return feedback;
}
