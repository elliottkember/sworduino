#include "FastLED.h"
#include "constants.cpp"

namespace Global {
  struct CRGB leds[NUM_LEDS];
  bool firstTimeRunningThroughPattern = false;
  int hue = 50;
  uint8_t max_bright = 250;
  uint16_t frameDelay = 1; // not actually used yet
  int patternId = 0;

  void printPixels(CRGB (*calculatePixel)(int)) {
    for (int k = 0; k < NUM_LEDS - 1; k++) {
      leds[k] = calculatePixel(k);
    }
  }
}

