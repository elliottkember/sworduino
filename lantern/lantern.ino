#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"

#define LED_DATA 7
#define LED_CLOCK 11

#define COLOR_ORDER BGR
#define STARTING_BRIGHTNESS 255
#define NUM_LEDS 600
#define SPI true

#define apa102 true
#define NUM_LEDS 600

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
