
/* Confetti

    By: Mark Kriegsman

  Modified By: Andrew Tuline

  Date: July 2015

  Confetti flashes colours within a limited hue. It's been modified from Mark's original to support a few variables. It's a simple, but great looking routine.

*/


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 7                                             // Data pin to connect to the strip.
//#define LED_CK 11                                             // Clock pin for the strip.
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B                                       // Don't forget to change LEDS.addLeds
#define NUM_LEDS 1500                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 255;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

unsigned long previousMillis;                                 // Store last time the strip was updated.

// Define variables used by the sequences.
uint8_t  thisfade = 200;                                      // How quickly does it fade? Lower = slower fade rate.
int       thishue = 50;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 240;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t thisdelay = 0;                                        // We don't need much delay (if any)

bool   rainbow = false;
bool   rainbow2 = false;

const int ledPin = 13;
const int led = 13;

void setup() {
  delay(3000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 10000);
}

int numberOfSparkles = 1;
int increasing = true;

void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  if (numberOfSparkles < 40 && increasing) {
    numberOfSparkles += 1;
  } else if (numberOfSparkles > 5) {
    numberOfSparkles -= 1;
    increasing = false;
  } else {
    increasing = true;
    numberOfSparkles += 1;
  }

  int pos;
  for (int i = 0; i < numberOfSparkles * 8; i++) {
    pos = random16(NUM_LEDS);
    if (rainbow) {
      leds[pos] += CHSV((thishue + random16(huediff)) / 4 , thissat, thisbri);
    } else if(rainbow2) {
      leds[pos] = CHSV(thishue + (pos / 10), thissat, thisbri);
    } else {
      leds[pos] = CHSV(thishue, thissat, thisbri);
    }
  }
  thishue = thishue + thisinc;
}

void loop () {
  confetti();
  show_at_max_brightness_for_power();
}


