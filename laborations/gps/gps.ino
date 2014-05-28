#include <string.h>

// variable for NMEA sentence
String sentence = "";

void setup() {
  // For output to serial monitor
  Serial.begin(9600);
  // Connection to GPS
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    // Check if end of NMEA sentence
    if (c == '\n') {
      // Check if $GPRMC NMEA sentence
      if (sentence.startsWith("$GPRMC")) {
        char data[150];
        char *dataPtr = data;
        char *value;
        sentence.toCharArray(data, sentence.length());
        int i = 0;
        // Tokenize each line into values using comma delimiter.
        // Returns NULL when no more tokens.
        // strtok_r is part of C standard library.
        while ((value = strtok_r(dataPtr, ",", &dataPtr)) != NULL) {
          // 4th value is north latitude, 6th is west longitude
          if (i == 3 || i == 5) {
            int degMin = atoi(value);
            int degree = (int) degMin / 100;
            int minutes = degMin - (degree * 100);
            float seconds = (float) (atof(value) - (float) degMin) * 60.0;
            String label;
            if (i == 3)
              label = " N";
            else
              label = " W";
            char secBuffer[6];
            dtostrf(seconds, 6, 3, secBuffer);
            char location[100];
            sprintf(location, "%02d\xB0 %02d' %s\"", degree, minutes, secBuffer);
            Serial.println(location + label);
          }
          i++;
        }
        Serial.println();
      }
      sentence = "";
    }
    else {
      // Append character if not end of sentence
      sentence += c;
    }
  }
}

// Arduino IDE libraries do not include dtostrf()
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
