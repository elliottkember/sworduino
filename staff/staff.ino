#include "FastLED.h"
#define qsubd(x, b)  ((x>b)?255:0)  // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)         // Analog Unsigned subtraction macro. if result <0, then => 0
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define NUM_LEDS 100         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.
uint8_t max_bright = 128;     // Overall brightness definition. It can be changed on the fly.
unsigned long previousMillis; // Store last time the strip was updated.
int hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = false;

#define BEAUTIFUL_SPARKLES 1
#define DISCO_BARBER_2 2
#define DISCO_BARBER_1 3
#define SINGLE_COLOR_SPARKLES 4
#define WORMS 5
#define PARTY 6
#define DISCO_TWIRL 7
#define NIGHT_SPARKLES 8

int maxPatternId = 8;
int rotationInMillseconds = 2000; // 20 seconds for production

// Either A)
bool holdPattern = true;
int patternId = DISCO_TWIRL;
// OR B)
//bool holdPattern = false;
//int patternId = BEAUTIFUL_SPARKLES;

void setup() {
  delay(1000);
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 3000);
  randomSeed(analogRead(0));
}

int numberOfSparkles = 1;
bool increasing = true;

int upAndDownBy(int value, int difference) {
  if (value < 80 && increasing) {
    value += difference;
  } else if (value > 5) {
    value -= difference;
    increasing = false;
  } else {
    increasing = true;
    value += difference;
  }
  return value;
}

void nightSparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 180);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1);
  for (int i = 0; i < 20; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue, 180, 255);
  }
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    leds[random16(NUM_LEDS)] += CHSV(hue, 200, 20);
  }
  hue = hue + 1;
}

void beautifulSparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1);
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue + (pos / 10), 240, 255);
  }
  hue += 10;
}

void sparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1);
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue, 240, 255);
  }
  hue += 1;
}

uint8_t numberOfWorms = 3;
uint8_t wormsFadeRate = 64; // Very low value = longer trails.
uint8_t wormsHueIncrement = 1; // Incremental change in hue between each dot.
uint8_t wormsStartingHue = 0; // The current hue
uint8_t wormsBaseBeat = 2; // Higher = faster movement.

void worms() {
  fadeToBlackBy(leds, NUM_LEDS, wormsFadeRate);
  for ( int i = 0; i < numberOfWorms; i++) {
    for ( int j = 0; j < 20; j++) {
      leds[beatsin16(wormsBaseBeat + i + numberOfWorms, 0, NUM_LEDS - 20) + j] += CHSV(wormsStartingHue, 240, 255);
    }
    wormsStartingHue += wormsHueIncrement;
  }
}

uint8_t discoBarberFrequency = 3;
int     discoBarberPhase = 0;
uint8_t discoBarberCutoff = 120;
uint8_t discoBarberSaturation = 240;

void discoBarber() {
  if (patternId == DISCO_BARBER_1) {
    discoBarberFrequency = 5;
    discoBarberPhase += 24;
  } else {
    discoBarberFrequency = 3;
    discoBarberPhase += 48;
  }

  hue = hue + 1;

  for (int k = 0; k < NUM_LEDS - 1; k++) {
    // qsub sets a minimum value called discoBarberCutoff. If < discoBarberCutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
    leds[k] = CHSV(0, 0, 0);                                        // First set a background colour, but fully saturated.

    // Sparkles!
    if (random(1000) > 996) {
      leds[k] = CHSV(255, 0, 255);
    }

    if (patternId == DISCO_BARBER_1) {
      // original disco barber 1
      // leds[k] += CHSV(hue+k/5, discoBarberSaturation, _brightness);                             // Then assign a hue to any that are bright enough.
      leds[k] += CHSV(hue * 10 + k / 2, discoBarberSaturation, _brightness);                             // Then assign a hue to any that are bright enough.
    } else {
      leds[k] += CHSV(hue * -20 + k / 4, discoBarberSaturation, _brightness);                       // Then assign a hue to any that are bright enough.
    }
  }
}

int partySeed;
int partySeedDirection = true;

void party() {
  int partySeedLength = 200;
  if (firstTimeRunningThroughPattern) {
    partySeed = 0;
  } else {
    partySeed += partySeedDirection ? 20 : -20;
    if (partySeed > NUM_LEDS - partySeedLength) {
      partySeedDirection = false;
    } else if (partySeed < 0) {
      partySeedDirection = true;
    }
  }
  EVERY_N_MILLISECONDS(500) {
    hue += 33;
  }
  fadeToBlackBy(leds, NUM_LEDS, 120);
  int brightness = 255;
  int saturation = 200;
  int pos = random(partySeed - partySeedLength, partySeed + partySeedLength);
  
  for (int i = pos; i <= pos + 50; i++) {
    brightness *= 0.97;
    if (i < NUM_LEDS && i > 0) {
      leds[i] = CHSV(hue, saturation, brightness);
    }
  }
  brightness = 255;
  saturation = 255;
  for (int j = pos; j >= pos - 50; j--) {
    brightness *= 0.97;
    if (j < NUM_LEDS && j > 0) {
      leds[j] = CHSV(hue, saturation, brightness);
    }
  }
}

uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // You can change direction.
int8_t thisspeed = 32;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.

void discoTwirl() {

  if (firstTimeRunningThroughPattern) {
    allfreq = 32;
    thiscutoff -= 240;
  } else {
    EVERY_N_MILLISECONDS(1000) {
//      allfreq += 1;
//      if (allfreq == 255) {
//        allfreq = 0;
//      }
    }
  }
  
  //  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  thisphase += thisspeed;
//  thishue += thisrot;                                                         // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(0, 255, 0);
    leds[k] += CHSV(thishue + k + thisphase / 5, allsat, thisbright);                               // Assigning hues and brightness to the led array.
  }
}

void loop () {
  if (!holdPattern) {
    EVERY_N_MILLISECONDS(rotationInMillseconds) {
      firstTimeRunningThroughPattern = true;
      patternId += 1;
      if (patternId > maxPatternId) {
        patternId = 1;
      }
    }
  }

  if (patternId == NIGHT_SPARKLES) {
    nightSparkles();
  } else if (patternId == BEAUTIFUL_SPARKLES) {
    beautifulSparkles();
  } else if (patternId == SINGLE_COLOR_SPARKLES) {
    sparkles();
  } else if (patternId == DISCO_BARBER_1 || patternId == DISCO_BARBER_2) {
    discoBarber();
  } else if (patternId == WORMS) {
    worms();
  } else if (patternId == PARTY) {
    party();
  } else if (patternId == DISCO_TWIRL) {
    discoTwirl();
  }

  delay(30);

  firstTimeRunningThroughPattern = false;

  show_at_max_brightness_for_power();
}
