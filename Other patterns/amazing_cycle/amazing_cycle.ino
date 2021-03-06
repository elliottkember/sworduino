
/* rainbow_march_demo

  By: Andrew Tuline
  `
  Date: March, 2015

  Rainbow marching up the strand. Pretty basic, but oh so popular, and we get a few options as well. We don't need to add a 'wheel' routine here.

*/


#include "FastLED.h"                                          // FastLED library. 

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 7                                             // Data pin to connect to the strip.
//#define LED_CK 11
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE WS2812B                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 1500                                           // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Initialize global variables for sequences
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
  //  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5.5, 7000);                // FastLED 2.1 Power management set at 5V, 500mA
}

bool allWhite = false;

void rainbow_march() {                                         // The fill_rainbow call doesn't support brightness levels
  if (thisdir == 0) thishue += thisrot; else thishue -= thisrot; // I could use signed math, but 'thisdir' works with other routines.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);             // I don't change deltahue on the fly as it's too fast near the end of the strip.
}



void ChangeMe() {

  // fun ones
  //  deltahue += 1; // thisrot += 1;

  // A great spiral
  //  deltahue = 1;
  //  thisrot = 30;

  // I tried to do some slower twirling tricks (spin)
  //uint8_t looper = (millis() / 1000) % 1;
  //static uint8_t lastLooper = 99;
  //if (lastLooper != looper) {
  //  deltahue += 1;
  //}

  // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;

    //    allWhite = false;
    //    switch (secondHand % 4) {
    //      case 0: allWhite = true; break;
    //    }
    //
    //    deltahue = 30;
    //    thisrot = 5;
    //    thisdir = -1;

    if (secondHand >= 40) {
      spin = true;
    } else {
      spin = false;
    }

    switch (secondHand) {
      case  0: deltahue = 1; thisrot = 30; break;
      case  5: thisdir = -1; deltahue = 32; thisrot = 10; break;
      case 10: thisrot = 10; deltahue = 15; break;
      case 15: thisrot = 5; thisdir = -1; deltahue = 20; break;
      case 20: deltahue = 30; break;
      case 25: deltahue = 2; thisrot = 5; break;
      case 30: deltahue = 1; thisrot = 30; break;
      case 35: thisdir = -1; deltahue = 32; thisrot = 10; break;
      case 40: thisrot = 5; break;
      case 45: thisrot = 5; thisdir = -1; deltahue = 20; break;
      case 50: deltahue = 30; break;
      case 55: deltahue = 2; thisrot = 5; break;
      case 60: deltahue = 10; thisrot = 10; break;
    }
  }
}

void loop () {
  ChangeMe();
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbow_march();
  }
  if (spin) {
    thisrot += 1;
    // deltahue += 1;
  }
  //  if (allWhite) {
  //    fill_solid(0, NUM_LEDS, CHSV(255, 0, 255));
  //    FastLED.show();
  //    delay(random8(4, 10));
  //    fill_solid(0, NUM_LEDS, CHSV(255, 0, 0));
  //    //    allWhite = false;
  //  }
  show_at_max_brightness_for_power();
}


