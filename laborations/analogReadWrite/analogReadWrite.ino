const int NUM_READINGS = 10;

const int FEEDBACK_ABSOLUTE_MAX = 3300;
const int FEEDBACK_MAX = 2330;

const int ANALOG_5V = 4095;
const int ANALOG_0V = 0;

// Pin constants
const int STICK_PIN = A0;
const int SERVO_FEEDBACK_PIN = A1;
const int MOTOR_CONTROLLER_PIN = DAC0;

// DAC - ADC resolutions
const int RESOLUTION_READ = 12;
const int RESOLUTION_WRITE = 12;

int stickReadings[NUM_READINGS];
int stickReadingsIndex = 0;
int stickReadingsTotal = 0;
int stickReadingsAverage = 0;

int feedbackReadings[NUM_READINGS];
int feedbackReadingsIndex = 0;
int feedbackReadingsTotal = 0;
int feedbackReadingsAverage = 0;

void setup()
{
  Serial.begin(9600);

  // Set ADC - DAC resolutions
  analogReadResolution(RESOLUTION_READ);
  analogWriteResolution(RESOLUTION_READ);

  for (int i = 0; i < NUM_READINGS; i++)
  {
    stickReadings[i] = 0;
    feedbackReadings[i] = 0;
  }
}

void loop()
{
  readStickInput();
  readServoFeedback();
}

void readStickInput()
{
  // Remove the last reading
  stickReadingsTotal -= stickReadings[stickReadingsIndex];
  // Read from sensor and add readings to joysticks total readings
  stickReadings[stickReadingsIndex] = analogRead(STICK_PIN);
  stickReadingsTotal += stickReadings[stickReadingsIndex];

  // Calculate an average value for the readings and write it to ppin
  stickReadingsAverage = stickReadingsTotal / NUM_READINGS;
  analogWrite(MOTOR_CONTROLLER_PIN, stickReadingsAverage);

  //Serial.print("12-bit PWM value: ");
  //Serial.println(stickReadingsAverage);

  // Increase the index and if needed reset it
  stickReadingsIndex += 1;
  if (stickReadingsIndex >= NUM_READINGS)
  {
    stickReadingsIndex = 0;
  }
}

void readServoFeedback()
{
  // Remove the last reading
  feedbackReadingsTotal -= feedbackReadings[feedbackReadingsIndex];
  // Read from servo feedback and add readings to feedback total readings
  feedbackReadings[feedbackReadingsIndex] = analogRead(SERVO_FEEDBACK_PIN);
  feedbackReadingsTotal += feedbackReadings[feedbackReadingsIndex];

  // Calculate an average value for the readings and write it to ppin
  feedbackReadingsAverage = feedbackReadingsTotal / NUM_READINGS;

  Serial.print("Servo feedback: ");
  Serial.println(scaleFeedback(feedbackReadingsAverage));

  // Increase the index and if needed reset it
  feedbackReadingsIndex += 1;
  if (feedbackReadingsIndex >= NUM_READINGS)
  {
    feedbackReadingsIndex = 0;
  }
}

void readGPSInput()
{
  // TODO: Complete this method
}

int scaleFeedback(int rawFeedback)
{
  double feedbackScale = ANALOG_5V / (FEEDBACK_ABSOLUTE_MAX - FEEDBACK_MAX);
  int scaledFeedback = (rawFeedback - FEEDBACK_MAX) * feedbackScale;

  if (ANALOG_0V <= scaledFeedback && scaledFeedback <= ANALOG_5V)
  {
    return scaledFeedback;
  }
  
  if (scaledFeedback < ANALOG_0V)
  {
    return ANALOG_0V;
  }
  
  return ANALOG_5V;
}
