#include "FastLED.h"
#define qsubd(x, b)  ((x>b)?255:0)  // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)  // Analog Unsigned subtraction macro. if result <0, then => 0
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
uint8_t max_bright = 255;     // Overall brightness definition. It can be changed on the fly.
unsigned long previousMillis; // Store last time the strip was updated.
int hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;

#define NUM_LEDS 1500         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

#define BEAUTIFUL_SPARKLES 1
#define NIGHT_SPARKLES 2
#define SINGLE_COLOR_SPARKLES 3
#define DAVE 4
#define SHOOTING_GRADIENTS 5
#define DISCO_BARBER_1 6
#define DISCO_TWIRL 7
#define DISCO_BARBER_2 8
#define DISCO_TWIRL_2 9
#define NEWPATTERN 10

int maxPatternId = 9;
int rotationInMillseconds = 30000; // 20 seconds for production

// If we're testing one pattern, use holdPattern as true and the patternId as the starting pattern.
//bool holdPattern = false;
//int patternId = BEAUTIFUL_SPARKLES;

bool holdPattern = true;
int patternId = NEWPATTERN;


// Set up LEDs, fade them all to black.
void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 10000);
  randomSeed(analogRead(0));
  delay(50);
  // Make the whole stick black on startup (helps with restarts).
  // TODO: Test this (untested code)
  for (int i=0; i<NUM_LEDS-1; i++) {
    leds[i] = CHSV(0, 0, 0);
  }
}

uint8_t daveOffset = 0;
int daveMilliseconds = 0;

// Dave's pattern. By Dave Grijalva with love.
// A gradient background, with rotating bright points and white sparkles.
void dave() {
  for (int k=0; k<NUM_LEDS-1; k++) {
    uint8_t k8 = k;
    uint8_t hue = (k/15 * 255) + (daveOffset*5);
    uint8_t saturation = rand() % 255 > 252 ? 0 : 255; //NUM_LEDS - (k+daveOffset*10);
    uint8_t value = k8 % 14 == daveOffset % 14 ? 200 : 40; //NUM_LEDS- (k+daveOffset*10);
    if(saturation == 0){
      value = 255;
    }
    leds[k] = CHSV(hue, saturation, value);
  }
  EVERY_N_MILLISECONDS(1000/24){
    daveMilliseconds++;
    if(daveMilliseconds % 3 == 0){
      daveOffset++;
    }
  }
}

int numberOfSparkles = 1;
bool increasing = true;
// TODO increasing and numberOfSparkles should probably be passed by reference
int upAndDownBy(int value, int difference, int max, int min) {
  if (value < max && increasing) {
    value += difference;
  } else if (value > min) {
    value -= difference;
    increasing = false;
  } else {
    increasing = true;
    value += difference;
  }
  return value;
}

void nightSparkles() {
  if (firstTimeRunningThroughPattern) {
    hue = 32;
    numberOfSparkles = 40;
  }
  fadeToBlackBy(leds, NUM_LEDS, 180);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 5);
  for (int i = 0; i < numberOfSparkles * 2; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue, 180, 255);
  }
  for (int i = 0; i < numberOfSparkles * 8; i++) {
    leds[random16(NUM_LEDS)] = CHSV(hue, 200, 20);
  }
  hue += 1;
}

void beautifulSparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 5);
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue + (pos / 10), 240, 255);
  }
  hue += 10;
}

void sparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 80, 5);
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue, 240, 255);
  }
  hue += 1;
}

uint8_t numberOfWorms = 4;
uint8_t wormsFadeRate = 80; // Very low value = longer trails.
uint8_t wormsHueIncrement = 2; // Incremental change in hue between each dot.
uint8_t wormsStartingHue = 180; // The current hue
uint8_t wormsBaseBeat = 10; // Higher = faster movement.

void shootingGradients() {
  fadeToBlackBy(leds, NUM_LEDS, wormsFadeRate);
  for ( int i = 0; i < numberOfWorms; i++) {
    for ( int j = 0; j < 80; j++) {
      leds[beatsin16(wormsBaseBeat + i + numberOfWorms, 0, NUM_LEDS - 80) + j] += CHSV(wormsStartingHue, 180, 255);
    }
    wormsStartingHue += wormsHueIncrement;
    if (wormsStartingHue > 270) {
      wormsStartingHue = 180;
    }
  }
}

uint8_t discoBarberFrequency = 3;
int     discoBarberPhase = 0;
uint8_t discoBarberCutoff = 120;
uint8_t discoBarberSaturation = 240;

// This pattern is called for both Disco Barber and Disco Barber 2.
void discoBarbers() {
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
      leds[k] += CHSV(hue * 10 + k / 2, discoBarberSaturation, _brightness);
    } else {
      leds[k] += CHSV(hue * -20 + k / 4, discoBarberSaturation, _brightness);
    }
  }
}

int thisphase = 0;                // Phase change value gets calculated.
bool fadeUp = 0;
bool thisdir = 0;                 // You can change direction.
int8_t thisspeed = 16;            // You can change the speed, and use negative values.
uint8_t thishue = 0;              // You can change the starting hue value for the first wave.
uint8_t thisrot = 18;             // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;             // I like 'em fully saturated with colour.
uint8_t allfreq = 1;              // You can change the frequency, thus overall width of bars.
uint8_t thiscutoff = 200;         // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t fade = 200;

void discoTwirl() {

  if (firstTimeRunningThroughPattern) {
    thishue = 0;           // You can change the starting hue value for the first wave.
    thisrot = 18;          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 180;          // I like 'em fully saturated with colour.
    thisdir = 0;           // You can change direction.
    thisspeed = 8;         // You can change the speed, and use negative values.
    allfreq = 4;           // You can change the frequency, thus overall width of bars.
    thisphase = 0;         // Phase change value gets calculated.
    thiscutoff = 200;      // You can change the cutoff value to display this wave. Lower value = longer wave.
    fade = 200;
    fadeUp = 1;
    fadeToBlackBy(leds, NUM_LEDS, 255);
  } else {
    EVERY_N_MILLISECONDS(2) {
      if (fadeUp) fade += 10; else fade -= 10;
      if (fade < 50) fadeUp = 1; else if (fade > 250) fadeUp = 0;
    }
  }

  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  fadeToBlackBy(leds, NUM_LEDS, fade);
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = cubicwave8((k*-allfreq)+thisphase);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] += CHSV(thishue + k + thisphase / 5, 200, thisbright);                               // Assigning hues and brightness to the led array.
  }
}

void discoTwirl2() {

  if (firstTimeRunningThroughPattern) {
    thishue = 0;                                          // You can change the starting hue value for the first wave.
    thisrot = 18;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 255;                                         // I like 'em fully saturated with colour.u¨
    thisdir = 0;                                             // You can change direction.
    thisspeed = 16;                                         // You can change the speed, and use negative values.
    allfreq = 1;                                         // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                            // Phase change value gets calculated.
    thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
    fade = 200;
    fadeUp = 0;
    thisrot = 0;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 255;                                         // I like 'em fully saturated with colour.
    thisdir = 0;                                             // You can change direction.
    thisspeed = 32;                                         // You can change the speed, and use negative values.
    allfreq = 32;                                         // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                            // Phase change value gets calculated.
    thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
  }

  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  fadeToBlackBy(leds, NUM_LEDS, fade);
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsubd(cubicwave8((k*-allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] += CHSV(thishue + k + thisphase / 5, allsat, thisbright);                               // Assigning hues and brightness to the led array.  }
  }
}

int newPatternI = 0;
int newPatternDiagonal = float(2.0);
void newPattern() {
  newPatternI+=0.72;

  if (newPatternI >= newPatternDiagonal) {
    newPatternI = 2.0;
  }

  for (int newPatternIterator=0; newPatternIterator<NUM_LEDS; newPatternIterator++) {
    if (newPatternIterator % newPatternDiagonal == newPatternI) {
      leds[newPatternIterator] = CRGB(0, 255, 0);
    } else {
      leds[newPatternIterator] = CRGB(0, 0, 0);
    }
  }

  EVERY_N_MILLISECONDS(1) {
    newPatternDiagonal++;
    if (newPatternDiagonal > 2000) {
      newPatternDiagonal = 0;
    }
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
    discoBarbers();
  } else if (patternId == SHOOTING_GRADIENTS) {
    shootingGradients();
  } else if (patternId == DISCO_TWIRL) {
    discoTwirl();
  } else if (patternId == DISCO_TWIRL_2) {
    discoTwirl2();
  } else if (patternId == DAVE) {
    dave();
  } else if (patternId == NEWPATTERN) {
    newPattern();
  }

  firstTimeRunningThroughPattern = false;

  show_at_max_brightness_for_power();
}
