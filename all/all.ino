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
uint8_t  thisfade = 120;                                      // How quickly does it fade? Lower = slower fade rate.
int       thishue = 50;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 240;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t thisdelay = 0;                                        // We don't need much delay (if any)

#define RAINBOW 1
#define RAINBOW_2 2
#define RAINBOW_3 3
#define DISCO_BARBER_1 4
#define DISCO_BARBER_2 5
#define JUGGLE 6
#define SNAKES 7
#define PIROUETTE 8
int maxPatternId = 8;

const bool holdPattern = false;
int patternId; // = SNAKES;

const int ledPin = 13;
const int led = 13;


void setup() {
  delay(2000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 4000);
  randomSeed(analogRead(0));
  if (!holdPattern) {
    patternId = random(maxPatternId);
  }
}

int numberOfSparkles = 1;
int increasing = true;

void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  if (numberOfSparkles < 20 && increasing) {
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
    for (int i = 0; i < 5; i++) {
      pos = random16(NUM_LEDS);
      leds[pos] += CHSV(thishue, 180, 255);
    }
  }

  for (int i = 0; i < numberOfSparkles * 8; i++) {
    pos = random16(NUM_LEDS);
    if (patternId == RAINBOW) {
      leds[pos] += CHSV(thishue, 100, 20);
    } else if (patternId == RAINBOW_2) {
      leds[pos] = CHSV(thishue + (pos / 10), thissat, thisbri);
    } else if (patternId == RAINBOW_3) {
      leds[pos] = CHSV(thishue, thissat, thisbri);
    }
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
  numdots = 4; basebeat = 48; hueinc = 2; faderate = 48; thishue = 128;
  for ( int i = 0; i < numdots; i++) {
    leds[beatsin16(basebeat + i + numdots, 0, NUM_LEDS)] += CHSV(curhue, thissat, thisbright); //beat16 is a FastLED 3.1 function
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

int chainStart;
int chainColor;

int currentChain = 0;
#define MAX_CHAINS 3
int heads[MAX_CHAINS];
int colors[MAX_CHAINS];
int fadeRate = 100;
#define MAX_LENGTH 400

void newSnake() {
  chainColor += 20;
  currentChain += 1;
  if (currentChain > MAX_CHAINS) {
    currentChain = 0;
  }
  chainStart = 0;
  heads[currentChain] = chainStart;
}

void snakes() {
  EVERY_N_MILLISECONDS(1200) {
    newSnake();
  }

  int numberOfSnakes = MAX_CHAINS;
  fadeToBlackBy(leds, NUM_LEDS, 400);
  for (int i = 0; i < 40; i++) {
    for (int h = 0; h < numberOfSnakes; h++) {
      int chainStart = heads[h];
      chainStart += 1;
      int offset = 0; random(millis()) * 55;
      if (chainStart > NUM_LEDS) {
        newSnake();
      }
      if (chainStart < NUM_LEDS) {
        leds[chainStart] = CHSV(chainColor + i, (255 - 55) + offset - random(50), 255);
        heads[h] = chainStart;
      } else {
        heads[h] = 0;
      }
    }
  }
}

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
      leds[k] += CHSV(thishue + k / 5, allsat, thisbright);                         // Then assign a hue to any that are bright enough.
    } else {
      leds[k] += CHSV(thishue * 5 + k / 4, allsat, thisbright);                       // Then assign a hue to any that are bright enough.
    }
  }
  bgclr++;
}



void newPirouette() {
  chainColor = random(255);
  currentChain += 1;
  if (currentChain > MAX_CHAINS) {
    currentChain = 0;
  }
  //    chainStart = random(1, 5);
  if (chainStart == 1) {
    chainStart = 0;
  } else {
    chainStart = 1;
  }
  heads[currentChain] = chainStart;
  colors[currentChain] = chainColor; // CHSV(chainColor, 255, 255);
}

int pirouetteFadeRate = random(200) + 100;

void pirouette () {
  EVERY_N_MILLISECONDS(1200) {
    newPirouette();
  }
    EVERY_N_MILLISECONDS(870) {
    newPirouette();
  }
    EVERY_N_MILLISECONDS(330) {
    newPirouette();
  }

  EVERY_N_MILLISECONDS(100) {
    pirouetteFadeRate += 1;
  }

  int numberOfSnakes = MAX_CHAINS;
  fadeToBlackBy(leds, NUM_LEDS, pirouetteFadeRate);
  for (int i = 0; i < 20; i++) {
    for (int h = 0; h < numberOfSnakes; h++) {
      int chainStart = heads[h];
      chainStart += 2;
//      if (chainStart > NUM_LEDS) {
//        newPirouette();
//      }
      if (chainStart < NUM_LEDS) {
        leds[chainStart] = CHSV(colors[h], 255, 255);
        heads[h] = chainStart;
      }
    }
  }
}

void loop () {
  if (!holdPattern) {
    EVERY_N_MILLISECONDS(7000) {
      patternId = random(maxPatternId);
      Serial.println(patternId);
    }
  }

  if (patternId == RAINBOW || patternId == RAINBOW_2 || patternId == RAINBOW_3) {
    confetti();
  } else if (patternId == DISCO_BARBER_1 || patternId == DISCO_BARBER_2) {
    discoBarber();
  } else if (patternId == JUGGLE) {
    juggle();
  } else if (patternId == SNAKES) {
    snakes();
  } else if (patternId == PIROUETTE) {
    pirouette();
  }
  show_at_max_brightness_for_power();
}


