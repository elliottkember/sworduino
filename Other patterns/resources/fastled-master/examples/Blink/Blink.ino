#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 300

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(13, OUTPUT);
}

void loop() { 
  // Turn the LED on, then pause
  leds[1] = CRGB::Red;
  FastLED.show();
  digitalWrite(13, HIGH);
  delay(100);
  // Now turn the LED off, then pause
  leds[1] = CRGB::Black;
  FastLED.show();
  digitalWrite(13, LOW);
  delay(100);
}
