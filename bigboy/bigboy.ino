#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"

#define LED_DATA 11 // 7
#define LED_CLOCK 13 // 11

#define COLOR_ORDER BGR
#define NUM_LEDS 1100
#define spi false
#define apa102 true

struct CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<SK9822, LED_DATA, LED_CLOCK, COLOR_ORDER, DATA_RATE_MHZ(6)>(leds, NUM_LEDS);
//  if (apa102) {
//    if (spi) {
//      // enable access to LEDs via SPI
//      pinMode(7, OUTPUT);
//      digitalWrite(7, HIGH);
//      FastLED.addLeds<SK9822, COLOR_ORDER>(leds, NUM_LEDS);
//    } else {
//      FastLED.addLeds<SK9822, LED_DATA, LED_CLOCK, COLOR_ORDER, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
//    }
//  } else {
//    FastLED.addLeds<WS2812B, LED_DATA, COLOR_ORDER>(leds, NUM_LEDS);
//  }
  
  Serial.begin(57600);
  set_max_power_in_volts_and_milliamps(5, 1000);
  randomSeed(analogRead(0));
  FastLED.setDither(0);
  FastLED.delay(50);
  setupLanternPattern();
}

void loop() {
  lanternPattern();
  FastLED.show();
}
