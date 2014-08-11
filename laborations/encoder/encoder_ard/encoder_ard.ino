int encoderSignalA = 0;
int encoderSignalB = 1;

float pulses = 0.5;

void setup()
{
  Serial.begin(115200);
  attachInterrupt(11, encoderRiseA, RISING);
  attachInterrupt(12, encoderRiseB, RISING);
}

void loop()
{
  // DO NOTHING, ONLY USE INTERRUPTS!
}

void encoderRiseA()
{
  // Detach the interrupt, set another later
  detachInterrupt(11);
  encoderSignalA = 1;

  // Moving forward
  if (encoderSignalB == 0)
  {
    pulses = pulses + 0.5;
  }
  // Moving reverse
  if (encoderSignalB == 1)
  {
    pulses = pulses - 0.5;
  }

  Serial.println(pulses);
  // Remember to attach interrupt with a new ISR
  attachInterrupt(11, encoderFallA, FALLING);
}

void encoderFallA() {
  detachInterrupt(11);
  encoderSignalA = 0;

  if (encoderSignalB == 1)
  {
    pulses = pulses + 0.5;
  }
  if (encoderSignalB == 0)
  {
    pulses = pulses - 0.5;
  }

  Serial.println(pulses);
  attachInterrupt(11, encoderRiseA, RISING);
}

void encoderRiseB() {
  detachInterrupt(12);
  encoderSignalB = 1;

  if (encoderSignalA == 1)
  {
    pulses = pulses + 0.5;
  }
  if (encoderSignalA == 0)
  {
    pulses = pulses - 0.5;
  }

  Serial.println(pulses);
  attachInterrupt(12, encoderFallB, FALLING);
}

void encoderFallB()
{
  detachInterrupt(12);
  encoderSignalB = 0;

  if (encoderSignalA == 0)
  {
    pulses = pulses + 0.5;
  }
  if (encoderSignalA == 1)
  {
    pulses = pulses - 0.5;
  }

  Serial.println(pulses);
  attachInterrupt(12, encoderRiseB, RISING);
}
