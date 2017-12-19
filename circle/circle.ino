#import <FastLED.h>
struct CRGB leds[N_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, 7, GRB>(leds, 93);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,3000);
}

int rings[6] = { 32, 24, 16, 12, 8, 1 };
int frame = 0;

void loop() {
  int offset = 0;
  for(int ring = 0; ring < 6; ring++) {
    int pixels = rings[ring];
    // frame steps the ring around, ring * 3 off-sets each ring 3 pixels
    uint8_t rainbowStart = frame + ring * 3;
    int rainbowDelta = 255 / pixels;
    fill_rainbow(&(leds[offset]), pixels, rainbowStart, rainbowDelta);
    offset += pixels;
  }
  EVERY_N_MILLISECONDS(5) { frame++; }
  FastLED.show();
}

