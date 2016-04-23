uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
#define qsubd(x, b)  ((x>b)?wavebright:0)                     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"
#define LED_DT 7                                             // Data pin to connect to the strip.
#define COLOR_ORDER GRB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B                                       // Don't forget to change LEDS.addLeds
#define NUM_LEDS 1500                                           // Number of LED's.
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t max_bright = 255;                                      // Overall brightness definition. It can be changed on the fly.
unsigned long previousMillis;                                 // Store last time the strip was updated.

int hue = 50;                                       // Starting hue.

// This variable is set the first run-through of every pattern. Use it for setup!
bool firstRun = false;

// Patterns
#define RAINBOW 1
#define RAINBOW_2 2
#define RAINBOW_3 3
#define DISCO_BARBER_1 4
#define DISCO_BARBER_2 5
#define JUGGLE 6
#define NEW 7

int maxPatternId = 6;
int rotationInMillseconds = 20000; // 20 seconds for production


//int patternId = 3;
//bool holdPattern = false;
bool holdPattern = true;
int patternId = RAINBOW;

const int ledPin = 13;
const int led = 13;

void setup() {
  delay(3000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
}

int numberOfSparkles = 1;
int increasing = true;

void confetti() {

  if (patternId == RAINBOW) {
    fadeToBlackBy(leds, NUM_LEDS, 180);
  } else {
    fadeToBlackBy(leds, NUM_LEDS, 200);
  }
  
  if (numberOfSparkles < 80 && increasing) {
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
    for (int i = 0; i < 20; i++) {
      pos = random16(NUM_LEDS);
      leds[pos] = CHSV(hue, 180, 255);
    }
  }

  for (int i = 0; i < numberOfSparkles * 4; i++) {
    pos = random16(NUM_LEDS);
    if (patternId == RAINBOW) {
      leds[pos] += CHSV(hue, 200, 20);
    } else if (patternId == RAINBOW_2) {
      leds[pos] = CHSV(hue + (pos / 10), 240, 255);
    } else if (patternId == RAINBOW_3) {
      leds[pos] = CHSV(hue, 240, 255);
    }
  }

  if (patternId == RAINBOW_2) {
    hue += 10;
  } else {
    hue = hue + 1;
  }
}

uint8_t    numdots = 3;    // Number of dots in use.
uint8_t   faderate = 64;  // How long should the trails be. Very low value = longer trails.
uint8_t     hueinc = 1;   // Incremental change in hue between each dot.
uint8_t     curhue = 0;    // The current hue
uint8_t   basebeat = 2;   // Higher = faster movement.

void juggle() {
  hueinc = 1;
  faderate = 64;
  if (faderate == 100 || faderate == 200) {
    numdots += 1;
  }
  
  fadeToBlackBy(leds, NUM_LEDS, faderate);
  
  for ( int i = 0; i < numdots; i++) {
    for ( int j = 0; j < 20; j++) {
      leds[beatsin16(basebeat + i + numdots, 0, NUM_LEDS - 20) + j] += CHSV(curhue, 240, 255); //beat16 is a FastLED 3.1 function
    }
    // leds[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, 240, 255); //beat16 is a FastLED 3.1 function
    curhue += hueinc;
  }
}

uint8_t allfreq = 3;
int     thisphase = 0;
uint8_t thiscutoff = 120;
uint8_t thisrot = 1;
int8_t  thisspeed = 48;
uint8_t allsat = 240;

void discoBarber() {
  if (patternId == DISCO_BARBER_1) {
    allfreq = 5;
    thisphase += 24;
  } else {
    allfreq = 3;
    thisphase += 48;
  }
  
  hue = hue + 1;                                                // Hue rotation is fun for thiswave.

  for (int k = 0; k < NUM_LEDS - 1; k++) {                                    // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int _brightness = qsubd(cubicwave8((k * allfreq) + thisphase), thiscutoff); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(0, 0, 0);                                        // First set a background colour, but fully saturated.

    // Sparkles!
    if (random(1000) > 996) {
      leds[k] = CHSV(255, 0, 255);
    }

    if (patternId == DISCO_BARBER_1) {
      // original disco barber 1
      // leds[k] += CHSV(hue+k/5, allsat, _brightness);                             // Then assign a hue to any that are bright enough.
      leds[k] += CHSV(hue * 10 + k / 2, allsat, _brightness);                             // Then assign a hue to any that are bright enough.
    } else {
      leds[k] += CHSV(hue * -20 + k / 4, allsat, _brightness);                       // Then assign a hue to any that are bright enough.
    }
  }
 }

void loop () {
  if (!holdPattern) {
    EVERY_N_MILLISECONDS(rotationInMillseconds) {
      firstRun = true;
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

  firstRun = false;

  show_at_max_brightness_for_power();
}
