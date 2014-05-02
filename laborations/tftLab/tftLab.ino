#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#define _cs 10
#define _dc 9
#define _rst 8

// Use hardware SPI
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

void setup() {
  tft.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
