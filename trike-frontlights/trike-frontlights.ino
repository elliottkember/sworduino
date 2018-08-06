#import <FastLED.h>

#define NUM_LEDS 16
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, 3, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUM_LEDS);
}

uint8_t hue = 0;

void loop() {
  fill_gradient(leds, NUM_LEDS, CHSV(hue, 255, 255), CHSV(hue+128, 255, 255));
  hue++;
  Serial.println("hi");
  FastLED.show();
}
