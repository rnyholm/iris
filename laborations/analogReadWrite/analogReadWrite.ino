const int NUM_READINGS = 10;

// Pin constants
const int STICK_PIN = A0;
const int MOTOR_CONTROLLER_PIN = DAC0;

// DAC - ADC resolutions
const int RESOLUTION_READ = 12;
const int RESOLUTION_WRITE = 12;

int stickReadings[NUM_READINGS];
int stickReadingsIndex = 0;
int stickReadingsTotal = 0;
int stickReadingsAverage = 0;

void setup()
{
  Serial.begin(9600);

  // Set ADC - DAC resolutions
  analogReadResolution(RESOLUTION_READ);
  analogWriteResolution(RESOLUTION_READ);

  for (int i = 0; i < NUM_READINGS; i++)
  {
    stickReadings[i] = 0;
  }
}

void loop()
{
	readStickInput();
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
	
	Serial.print("12-bit PWM value: ");
	Serial.println(stickReadingsAverage); 
  
	// Increase the index and if needed reset it
	stickReadingsIndex += 1;
	if (stickReadingsIndex >= NUM_READINGS)
	{
		stickReadingsIndex = 0;
	}
}

void readGPSInput()
{
	// TODO: Complete this method
}