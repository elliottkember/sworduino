/* Juggle

By: Originally by Mark Kriegsman

Modified By: Andrew Tuline

Date: February 2015

Juggle just moves some balls back and forth. A single ball could be a Cylon effect. I've added several variables to this simple routine.

*/


#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT     7
#define COLOR_ORDER GRB
#define LED_TYPE     WS2812B
#define NUM_LEDS    300


// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Routine specific variables
uint8_t    numdots =   5;                                     // Number of dots in use.
uint8_t   faderate =   255;                                     // How long should the trails be. Very low value = longer trails.
uint8_t     hueinc =  32;                                     // Incremental change in hue between each dot.
uint8_t    thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   basebeat =   0.0001;                                     // Higher = faster movement.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

    LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);    // Use this for WS2812B
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 4000);               // FastLED power management set at 5V, 500mA.
} // setup()


void juggle() {                                               // Several colored dots, weaving in and out of sync with each other
  curhue = thishue;                                           // Reset the hue values.
  thishue=random8();
  fadeToBlackBy(leds, NUM_LEDS, faderate);
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(basebeat+i+numdots,0,NUM_LEDS)] += CHSV(curhue, thissat, thisbright);   //beat16 is a FastLED 3.1 function
    leds[beatsin16(basebeat+i+numdots,0,NUM_LEDS)+1] += CHSV(curhue, thissat, thisbright);   //beat16 is a FastLED 3.1 function
    leds[beatsin16(basebeat+i+numdots,0,NUM_LEDS)+2] += CHSV(curhue, thissat, thisbright);   //beat16 is a FastLED 3.1 function
    leds[beatsin16(basebeat+i+numdots,0,NUM_LEDS)+3] += CHSV(curhue, thissat, thisbright);   //beat16 is a FastLED 3.1 function
    curhue += hueinc;
  }
}

int hue = 0;
void sparkle() {
   fadeToBlackBy(leds, NUM_LEDS, 200);
  for (int i = 0; i < 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue + (pos / 10), 240, 255);
  }
  hue += 1;
  if (hue > 255) {
    hue = hue - 255;
  }
}

bool doSparkle = false;

void change() {
  if (doSparkle) {
    doSparkle = false;
  } else {
    doSparkle = true;
  }
}

void loop() {
  EVERY_N_MILLISECONDS(5000) {
    change();
  }
  EVERY_N_MILLISECONDS(60/1000) {
    if (doSparkle) {
      sparkle();
    } else {
      juggle();
    }
  }
  show_at_max_brightness_for_power();                         // Power managed display of LED's.
}


