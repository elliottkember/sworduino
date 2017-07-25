#include "FastLED.h"

#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
uint8_t MAX_BRIGHT = 255;     // Overall brightness definition. It can be changed on the fly.
uint8_t hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;
#define NUM_LEDS 300         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

void setup() {
  Serial.begin(57600);
  // LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs
  FastLED.setBrightness(MAX_BRIGHT);
  set_max_power_in_volts_and_milliamps(5, 3000);
  randomSeed(analogRead(0));
  delay(50);
}

void loop () {
  lanternPattern();
  show_at_max_brightness_for_power();
}


