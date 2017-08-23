#include "FastLED.h"

// Digital unsigned subtraction macros. if result <0, then => 0. Otherwise, take on fixed value.
#define qsubd(x, b)  ((x>b)?255:0)
#define qsuba(x, b)  ((x>b)?x-b:0)
#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define VOLTS 5
#define NUM_LEDS 300
#define CIRCUMFERENCE 8
#define ROTATION_IN_SECONDS 30
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
  FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(Global::leds, NUM_LEDS)
    .setCorrection(TypicalSMD5050);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 600);
  delay(1000);
}

void (*patterns[])() = {
  dave,
  lantern,
  beautifulSparkles,
  shootingGradients,
  discoTwirl2,
  rain
};

int maxPatternId = sizeof( patterns ) / sizeof(patterns[0]);

void loop () {
  patterns[Global::patternId]();
  Global::firstTimeRunningThroughPattern = false;
  FastLED.show();
  delay(1000 / fps);
  EVERY_N_SECONDS(ROTATION_IN_SECONDS) {
    Global::nextPattern(maxPatternId);
  }
}
