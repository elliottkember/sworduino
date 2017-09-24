#include "FastLED.h"

struct CRGB leds[300];

void setup() {
  // put your setup code here, to run once:
  LEDS.addLeds<APA102, 13, 11, GRB>(leds, 300);
  set_max_power_in_volts_and_milliamps(5, 100);
}

void loop() {
  // put your main code here, to run repeatedly:
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
}
