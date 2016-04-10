#define qsubd(x, b)  ((x>b)?wavebright:0)                     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"
#define LED_DT 7                                             // Data pin to connect to the strip.
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

#define RAINBOW 1
#define RAINBOW_2 2
#define RAINBOW_3 2
#define DISCO_BARBER_1 3
#define DISCO_BARBER_2 4
#define JUGGLE 5
#define NEW 6

int maxPatternId = 5;
int patternId = random(maxPatternId - 1);
bool holdPattern = false;
int rotationInMillseconds = 20000;

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

  if (patternId == RAINBOW) {
    for (int i = 0; i < 2; i++) {
      pos = random16(NUM_LEDS);
      leds[pos] += CHSV(thishue, 255, 255);
    }
  }

  for (int i = 0; i < numberOfSparkles * 8; i++) {
    pos = random16(NUM_LEDS);
    if (patternId == RAINBOW) {
      leds[pos] += CHSV(thishue, 255, 20);
    } else if (patternId == RAINBOW_2) {
      leds[pos] = CHSV(thishue + (pos / 10), thissat, thisbri);
    } else if (patternId == RAINBOW_3) {
      leds[pos] = CHSV(thishue, thissat, thisbri);
    }
  }

  if (patternId == RAINBOW_2) {
    thishue += thisinc * 10;
  }
  
  thishue = thishue + thisinc;
}


// Routine specific variables
uint8_t    numdots =   4;                                     // Number of dots in use.
uint8_t   faderate =   128;                                     // How long should the trails be. Very low value = longer trails.
uint8_t     hueinc =  16;                                     // Incremental change in hue between each dot.
//  uint8_t    thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
// uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   basebeat =   48;                                     // Higher = faster movement.
// curhue = thishue;                                           // Reset the hue values.

void juggle() {
  fadeToBlackBy(leds, NUM_LEDS, faderate);
  numdots = 3; basebeat = 2; hueinc = 1; faderate = 48; thishue = 128;
  for ( int i = 0; i < numdots; i++) {
    for ( int j = 0; j < 20; j++) {
      leds[beatsin16(basebeat + i + numdots, 0, NUM_LEDS - 20) + j] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function  
    }
//    leds[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
    curhue += hueinc;
  }
}

uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
//uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 240;                                         // I like 'em fully saturated with colour.
int8_t thisspeed = 48;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 3;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 120;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
// int thisdelay = 0;                                           // You can change the delay. Also you can change the allspeed variable above.
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbri = 0;

void discoBarber() {
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k = 0; k < NUM_LEDS - 1; k++) {                                    // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k * allfreq) + thisphase), thiscutoff); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbri);                                        // First set a background colour, but fully saturated.

    // Sparkles!
    if (random(1000) > 997) {
      leds[k] = CHSV(255, 0, 255);
    }

    if (patternId == DISCO_BARBER_1) {
      leds[k] += CHSV(thishue+k/5, allsat, thisbright);                             // Then assign a hue to any that are bright enough.
    } else {
      leds[k] += CHSV(- (thishue * 20) + k / 4, allsat, thisbright);                       // Then assign a hue to any that are bright enough.
    }
  }
  bgclr++;
}

void newPattern() {
  int i = 0;
  for (int k = 0; k < NUM_LEDS; k++) {
    int colour = k; //  + i;
    leds[k + i] = CHSV(colour, 255, 255);
  }
  i++;
  if (i > NUM_LEDS) {
    i = 0;
  }
}

void loop () {
  if (!holdPattern) {
    EVERY_N_MILLISECONDS(rotationInMillseconds) {
      patternId += 1;
      if (patternId > maxPatternId) {
        patternId = 1;
      }
    }
  }

  if (patternId == RAINBOW || patternId == RAINBOW_2 || patternId == RAINBOW_3) {
    confetti();
  } else if (patternId == DISCO_BARBER_1 || patternId == DISCO_BARBER_2) {
    discoBarber();
  } else if (patternId == JUGGLE) {
    juggle();
  }
//  else if (patternId == NEW) {
//    newPattern();
//  }
  show_at_max_brightness_for_power();
}


