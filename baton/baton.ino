#include "FastLED.h"

// Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsubd(x, b)  ((x>b)?255:0)
// Analog Unsigned subtraction macro. if result <0, then => 0
#define qsuba(x, b)  ((x>b)?x-b:0)

#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define VOLTS 5
#define MAX_CURRENT_IN_MA 200
#define NUM_LEDS 300
#define CIRCUMFERENCE 8
#define ROTATION_IN_SECONDS 1
#define fps 30

namespace Global {
  struct CRGB leds[NUM_LEDS];
  bool firstTimeRunningThroughPattern = true;
  uint8_t max_bright = 250;
  uint16_t frameDelay = 1; // not actually used yet
  int patternId = 0;

  void nextPattern(int maxPatternId) {
    Global::firstTimeRunningThroughPattern = true;
    if (++Global::patternId == maxPatternId) {
      Global::patternId = 0;
    }
  }
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(VOLTS, MAX_CURRENT_IN_MA);
  delay(100);
}

void (*patterns[])() = {
  dave,
  lanternPattern,
  beautifulSparkles,
  shootingGradients,
  discoTwirl,
  discoTwirl2,
  rain
};

int patternId = 0;
int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);

void loop () {
  FastLED.setBrightness(Global::max_bright);
  patterns[Global::patternId]();
  Global::firstTimeRunningThroughPattern = false;
  show_at_max_brightness_for_power();
  delay(1000 / fps);
  EVERY_N_SECONDS(ROTATION_IN_SECONDS) {
    Global::nextPattern(maxPatternId);
  }
}
