#include "IrisCore.h"
#include "OverrideHandler.h"

OverrideHandler overrideHandler(A0, 2);
IrisCore irisCore;

void setup()
{
}

void loop()
{
  overrideHandler.getOverrideValue();
}
