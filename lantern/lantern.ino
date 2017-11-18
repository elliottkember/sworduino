//#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"

#define LED_DT 7
#define COLOR_ORDER GRB
#define LED_TYPE
#define STARTING_BRIGHTNESS = 255;
#define NUM_LEDS 300
struct CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  set_max_power_in_volts_and_milliamps(5, 3000);
  randomSeed(analogRead(0));
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs - SPI?

  // APA102:
  LEDS.addLeds<APA102, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalSMD5050);
  // FastLED.addLeds<APA102, 7, 11, RGB>(leds, NUM_LEDS);
  // WS2812B:
  // FastLED.addLeds<WS2812B, 7, RGB>(leds, NUM_LEDS);

  FastLED.setDither(0);
  FastLED.delay(50);
  FastLED.setBrightness(STARTING_BRIGHTNESS);
}

void loop() {
  lanternPattern();
  FastLED.show();
}
