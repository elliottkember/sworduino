// #define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"

#define LED_DATA 7 // false for SPI
#define LED 11 // false for SPI
#define COLOR_ORDER GRB
#define LED_TYPE
#define STARTING_BRIGHTNESS = 255;
#define NUM_LEDS = 300
#define SPI = true

struct CRGB leds[NUM_LEDS];

void setup() {
  if (LED_TYPE == APA102) {
    if (!LED_DATA && !LED_CLOCK) {
      // enable access to LEDs via SPI
      pinMode(7, OUTPUT);
      digitalWrite(7, HIGH);
      FastLED.addLeds<APA102, COLOR_ORDER>(leds, NUM_LEDS)
    } else {
      FastLED.addLeds<APA102, LED_DATA, LED_CLOCK, RGB>(leds, NUM_LEDS);
    }
  } else {
    FastLED.addLeds<WS2812B, LED_DATA, RGB>(leds, NUM_LEDS);
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
