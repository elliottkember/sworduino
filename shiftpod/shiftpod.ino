#include "FastLED.h"
#define NUM_LEDS 300
struct CRGB leds[NUM_LEDS];
int hue = 0;
float deltaHue = 0.8;

void setup() {
  LEDS.addLeds<WS2812B, 7, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 5000);
}

void loop() {
  fill_rainbow(leds, NUM_LEDS, hue, deltaHue);
  EVERY_N_MILLISECONDS(400) {
    hue += 1;
  }
  FastLED.show();
  Serial.println('a');
}
