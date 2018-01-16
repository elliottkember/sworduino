// This is a test of Lantern patterns with WS2812B LEDs on the big light.

// #define FASTLED_ALLOW_INTERRUPTS 0 // https://github.com/FastLED/FastLED/wiki/Platform-limitations
#include "FastLED.h"

#define LED_DATA 7
#define LED_CLOCK 11

#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

// APA102: #define COLOR_ORDER BGR

#define apa102 false
#define STARTING_BRIGHTNESS 255
#define NUM_LEDS 1500
#define SPI false

struct CRGB leds[NUM_LEDS];

void setup() {
  if (apa102) {
    if (SPI) {
      // enable access to LEDs via SPI
      pinMode(7, OUTPUT);
      digitalWrite(7, HIGH);
      FastLED.addLeds<APA102, COLOR_ORDER>(leds, NUM_LEDS);
    } else {
      FastLED.addLeds<APA102, LED_DATA, LED_CLOCK, COLOR_ORDER>(leds, NUM_LEDS);
    }
  } else {
    FastLED.addLeds<WS2812B, LED_DATA, COLOR_ORDER>(leds, NUM_LEDS);
  }

  Serial.begin(57600);
  set_max_power_in_volts_and_milliamps(5, 3000);
  randomSeed(analogRead(0));
  FastLED.setDither(0);
  FastLED.delay(50);
  FastLED.setBrightness(STARTING_BRIGHTNESS);
  setupLanternPattern();
}

void loop() {
  lanternPattern();
  FastLED.show();
}
