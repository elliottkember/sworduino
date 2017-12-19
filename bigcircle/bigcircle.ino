#import <FastLED.h>

#define N_LEDS 150
CRGBArray<N_LEDS> leds;

void setup() {
  FastLED.addLeds<WS2812B, 7, GRB>(leds, N_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
}

int rings[3] = { 60, 48, 42 };
int ring = 10;
int hue = 0;

void loop() {
  EVERY_N_MILLISECONDS(80) {
    int pixels = rings[ring];
    if (ring < 3) {
      int offset = 0;
      for (int i = 0; i < ring; i++) {
        offset += rings[i];
      }
      for(CRGB & pixel : leds(offset, offset + pixels - 1)) { 
        pixel = CHSV(hue, 255, 255);
      }
    }
    ring -= 1;
    hue += 5;
    if (ring == -1) { ring = 10; }
  }
  fadeToBlackBy(leds, N_LEDS, 10);
  show_at_max_brightness_for_power();
}

