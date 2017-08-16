#include "Circular_Gauge.h"

#define afrMin 9
#define afrMax 19

Circular_Gauge gauge(afrMin, afrMax, true);

void setup()   {                
  Serial.begin(19200);
  gauge.begin();
}

void loop() {
  float value = getTestValue();
  gauge.drawGaugeData(value);
}

float testVal = afrMin;
float inc = 0.2;
boolean increase = true;
float getTestValue() {
  if (testVal >= afrMax) increase = false;
  if (testVal <= afrMin) increase = true;
  if (increase) testVal+=inc;
  else testVal-=inc;
  return testVal;
}

