#define qsubd(x, b)  ((x>b)?wavebright:0)                     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
// Fixed definitions cannot change on the fly.
#define LED_DT     7
#define COLOR_ORDER GRB
#define LED_TYPE     WS2812B
#define NUM_LEDS    1500

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.
uint16_t frame = 0;

void setup() {
  Serial.begin(57600);
  delay(2000);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5.5, 7000);               // FastLED Power management set at 5V, 500mA
  frame = 0;
}

void loop () {


  EVERY_N_MILLISECONDS(0) {                           // FastLED based non-blocking delay to update/display the sequence.
    frame += 5;
    if (frame > NUM_LEDS) {
      frame = 0;
    }
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
    uint8_t minimumLength = 80;
    int length = (sin(frame / 10) * 50) + minimumLength;
    int start = 0;
    start = frame - length / 2;
    if (start < 0) {
      start = 0;
    }
    if ((start + length) > NUM_LEDS) {
      length = NUM_LEDS - start;
    }
    fill_solid(leds + start, length, CRGB( 255, 68, 221));
  }
  show_at_max_brightness_for_power();
}
