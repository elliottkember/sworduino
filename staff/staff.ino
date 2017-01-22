#include "FastLED.h"

// Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsubd(x, b)  ((x>b)?255:0)
// Analog Unsigned subtraction macro. if result <0, then => 0
#define qsuba(x, b)  ((x>b)?x-b:0)

#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LEDS 1500
#define CIRCUMFERENCE 8

namespace Global {
  struct CRGB leds[NUM_LEDS];
  bool firstTimeRunningThroughPattern = false;
  int hue = 50;
  uint8_t max_bright = 80;
}

// Set up LEDs, fade them all to black.
void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS);
  FastLED.setBrightness(Global::max_bright);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
  show_at_max_brightness_for_power();
  delay(100);
}
