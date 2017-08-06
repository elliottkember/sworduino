#include "FastLED.h"

#define qsubd(x, b)  ((x>b)?255:0) // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0) // Analog Unsigned subtraction macro. if result <0, then => 0
#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define VOLTS 5
#define MAX_CURRENT_IN_MA 1000
#define NUM_LEDS 300
#define CIRCUMFERENCE 8
#define ROTATION_IN_SECONDS 1
#define fps 30

namespace Global {
  int patternId = 0;
  bool firstTimeRunningThroughPattern = true;
  struct CRGB leds[NUM_LEDS];

  void nextPattern(int maxPatternId) {
    firstTimeRunningThroughPattern = true;
    if (++patternId == maxPatternId) patternId = 0;
  }
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(VOLTS, MAX_CURRENT_IN_MA);
  delay(1000);
}

void (*patterns[])() = {
  dave,
  lantern,
  beautifulSparkles,
  shootingGradients,
  discoTwirl,
  discoTwirl2,
  rain
};

void loop () {
  patterns[Global::patternId]();
  Global::firstTimeRunningThroughPattern = false;
  show_at_max_brightness_for_power();
  delay(1000 / fps);
  int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);
  EVERY_N_SECONDS(ROTATION_IN_SECONDS) {
    Global::nextPattern(maxPatternId);
  }
}
