#include "FastLED.h"                                          // FastLED library. 
// Fixed definitions cannot change on the fly.
#define LED_DT 7                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE WS2812B                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 590                                           // Number of LED's.
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.
uint8_t thisdelay = 1;                                        // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
int8_t thisrot = 10;                                           // Hue rotation speed. Includes direction.
uint8_t deltahue = 10;                                         // Hue change between pixels.
bool thisdir = 0;                                             // I use a direction variable, so I can plug into inputs in a standar fashion.
bool spin = 0;

void setup() {
  delay(2000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 3000);                // FastLED 2.1 Power management set at 5V, 500mA
}

void loop () {
  uint8_t secondHand = (millis() / 50) % 60;
  int stripeLength = 40;
  for(int i = 0; i < NUM_LEDS; i++) {
    CRGB color;
    int offset = 0;
    if (secondHand % 2 == 0) {
      offset = 1;
    }
    if (((i / 40) % 2) == offset) {
      color = 0xFF0000;
    } else {
      color = 0xFFFFFF;
    }
    leds[i] = color;
  }
  show_at_max_brightness_for_power();
}
