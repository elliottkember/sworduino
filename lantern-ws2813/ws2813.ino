#define FASTLED_FORCE_SOFTWARE_SPI

#include "FastLED.h"

#define LED_DATA 11
#define LED_CLOCK 13
#define NUM_LEDS 300
int hue = 50;
struct CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  FastLED.addLeds<WS2813, LED_DATA, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs
  FastLED.setBrightness(255);
  set_max_power_in_volts_and_milliamps(5, 3000);
  delay(50);
}

void loop() {
  lanternPattern();
  FastLED.show();
}

