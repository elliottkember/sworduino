#include <FastLED.h>

#define qsubd(x, b)  ((x>b)?255:0)  // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x,   b)  ((x>b)?x-b:0)  // Analog Unsigned subtraction macro. if result <0, then => 0
#define LED_DT 7              // Data pin to connect to the strip.
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
unsigned long previousMillis; // Store last time the strip was updated.
int hue = 50;                 // Starting hue.
bool firstTimeRunningThroughPattern = true;

#define NUM_LEDS 130         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

// PATTERNS
#define BEAUTIFUL_SPARKLES 1
#define DAVE 2
#define NIGHT_SPARKLES 3
#define WORMS 4 
#define DISCO_BARBER_1 5
#define DISCO_TWIRL 6
#define DISCO_BARBER_2 7
#define DISCO_TWIRL_2 8
#define RAIN 9

uint8_t max_bright = 255;     // Overall brightness definition. It can be changed on the fly.
uint16_t frameDelay = 30;

int maxPatternId = 9;
int rotationInMillseconds = 600000;

// If we're testing one pattern, use holdPattern as true and the patternId as the starting pattern.
bool holdPattern = false;
int patternId = BEAUTIFUL_SPARKLES;

// Set up LEDs, fade them all to black.
void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 2000);
  randomSeed(analogRead(0));
  delay(50);
}

uint8_t offset = 0;
int ms = 0;

void dave() {

  for (int k=0; k<NUM_LEDS-1; k++) {
    uint8_t k8 = k;
    uint8_t hue = (k/15 * 255) + (offset*5);
    uint8_t saturation = rand() % 255 > 252 ? 0 : 255; //NUM_LEDS - (k+offset*10);
    uint8_t value = k8 % 14 == offset % 14 ? 200 : 40; //NUM_LEDS- (k+offset*10);
    if(saturation == 0){
      value = 255;
    }

    leds[k] = CHSV(hue, saturation, value);
  }

  EVERY_N_MILLISECONDS(1000/24){
    ms++;
    if(ms % 3 == 0){
      offset++;
    }
  }
}


void verticalLines() {
  struct CRGB colors[8];

//  for (int i = 0; i < numberOfSparkles * 4; i++) {
  for (int i = 0; i < 8; i++) {
    colors[i] = CHSV(offset, ((256/8) * i) + offset, ((256/8) * i) + offset);
  }

  for (int k=0; k<NUM_LEDS-1; k++) {
    leds[k] = colors[k % 8];
  }

  offset += 20;
  // @todo progress colours in some fashion.
}

int     discoBarberPhase = 0;
uint8_t discoBarberFrequency = 10;
uint8_t discoBarberCutoff = 30;
uint8_t discoBarberSaturation = 240;

void discoBarber() {
  if (patternId == DISCO_BARBER_1) {
    discoBarberFrequency = 5;
    discoBarberPhase += 24;
  } else {
    discoBarberFrequency = 4;
    discoBarberPhase += 10;
  }

  hue = hue + 1;

  for (int k = 0; k < NUM_LEDS - 1; k++) {
    // qsub sets a minimum value called discoBarberCutoff. If < discoBarberCutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
    leds[k] = CHSV(0, 0, 0);                                        // First set a background colour, but fully saturated.

    // Sparkles!
    if (random(1000) > 990) {
      leds[k] = CHSV(255, 0, 255);
    }

    if (patternId == DISCO_BARBER_1) {
      // original disco barber 1
      // leds[k] += CHSV(hue+k/5, discoBarberSaturation, _brightness);                             // Then assign a hue to any that are bright enough.
      leds[k] += CHSV(hue * 10 + k / 2, discoBarberSaturation, _brightness);                             // Then assign a hue to any that are bright enough.
    } else {
      leds[k] += CHSV(hue * -20 + k / 2, discoBarberSaturation, _brightness);                       // Then assign a hue to any that are bright enough.
    }
  }
}

void barbershop() {
//  hue = hue + 1;
  hue = 0;

  discoBarberFrequency = 8;
  discoBarberPhase -= 10;

  for (int k = 0; k < NUM_LEDS - 1; k++) {
    // qsub sets a minimum value called discoBarberCutoff. If < discoBarberCutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int _brightness = qsubd(cubicwave8((k * discoBarberFrequency) + discoBarberPhase), discoBarberCutoff);
    leds[k] = CHSV(0, 0, 0);
    leds[k] = CHSV(hue * -20, discoBarberSaturation, _brightness);                       // Then assign a hue to any that are bright enough.
  }
}

int numberOfSparkles = 1;
bool increasing = true;

int upAndDownBy(int value, int difference) {
  if (value < 20 && increasing) {
    value += difference;
  } else if (value > 1) {
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
  for (int i = 0; i < numberOfSparkles * 4; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue, 180, 255);
  }
  for (int i = 0; i < numberOfSparkles; i++) {
    leds[random16(NUM_LEDS)] = CHSV(hue, 200, 20);
  }
  hue += 1;
  delay(6);
}

void beautifulSparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 150);
  numberOfSparkles = upAndDownBy(numberOfSparkles, 1);
  for (int i = 0; i < numberOfSparkles * 3; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CHSV(hue + (pos / 7), 240, 255);
  }
  hue += 10;
  delay(10);
}

uint8_t numberOfWorms = 4;
uint8_t wormsFadeRate = 80; // Very low value = longer trails.
uint8_t wormsHueIncrement = 2; // Incremental change in hue between each dot.
uint8_t wormsStartingHue = 180; // The current hue
uint8_t wormsBaseBeat = 10; // Higher = faster movement.

void worms() {
  fadeToBlackBy(leds, NUM_LEDS, wormsFadeRate);
  for ( int i = 0; i < numberOfWorms; i++) {
    for ( int j = 0; j < 20; j++) {
      leds[beatsin16(wormsBaseBeat + i + numberOfWorms, 0, NUM_LEDS - 20) + j] += CHSV(wormsStartingHue, 180, 255);
    }
    wormsStartingHue += wormsHueIncrement;
    if (wormsStartingHue > 270) {
      wormsStartingHue = 180;
    }
  }
}


int thisphase = 0;                                            // Phase change value gets calculated.
bool fadeUp = 0;
bool thisdir = 0;                                             // You can change direction.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 18;                                         // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
int8_t thisspeed = 16;                                        // You can change the speed, and use negative values.
uint8_t allfreq = 1;                                          // You can change the frequency, thus overall width of bars.
uint8_t thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t fade = 200;

void discoTwirl() {

  if (firstTimeRunningThroughPattern) {
    thishue = 0;                                           // You can change the starting hue value for the first wave.
    thisrot = 18;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
    allsat = 180;                                          // I like 'em fully saturated with colour.
    thisdir = 0;                                           // You can change direction.
    thisspeed = 8;                                         // You can change the speed, and use negative values.
    allfreq = 4;                                           // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                         // Phase change value gets calculated.
    thiscutoff = 200;                                      // You can change the cutoff value to display this wave. Lower value = longer wave.
    fade = 200;
    fadeUp = 1;
    fadeToBlackBy(leds, NUM_LEDS, 255);
  } else {
    EVERY_N_MILLISECONDS(2) {
      if (fadeUp == 1) fade += 10; else fade -= 10;
      if (fade < 50) fadeUp = 1; else if (fade > 245) fadeUp = 0;
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
    allsat = 255;                                         // I like 'em fully saturated with colour.
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
    thisspeed = 6;                                         // You can change the speed, and use negative values.
    allfreq = 64;                                         // You can change the frequency, thus overall width of bars.
    thisphase = 0;                                            // Phase change value gets calculated.
    thiscutoff = 200;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
  }

  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;          // You can change direction and speed individually.
  fadeToBlackBy(leds, NUM_LEDS, fade);
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = cubicwave8((k*-allfreq)+thisphase);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] += CHSV(thishue + k + thisphase / 5, allsat, thisbright);                               // Assigning hues and brightness to the led array.  }
  }
}

int counter = 0;
int frameSize = 1;
void rain() {
  if (firstTimeRunningThroughPattern) {
    for (int i = NUM_LEDS; i > 0; i--) {
      int on = random8(100) > 80 ? 255 : 0;
      leds[i] = CHSV(hue, 255, on);
      counter++;
      if (counter == 20) {
        hue++;
        counter = 0;
      }
    }
  } else {
    for (int i = NUM_LEDS; i > frameSize; i--) {
      leds[i] = leds[i-frameSize];
    }
    for (int i = 0; i <= frameSize + 1; i++) {
      int on = random8(100) > 80 ? 255 : 0;
      leds[i] = CHSV(hue, 255, on);
      counter++;
      if (counter == 20) {
        hue--;
        counter = 0;
      }
    }
  }
}

void draw() {
  if (patternId == NIGHT_SPARKLES) {
    nightSparkles();
  } else if (patternId == RAIN) {
    rain();
  } else if (patternId == BEAUTIFUL_SPARKLES) {
    beautifulSparkles();
  } else if (patternId == DISCO_BARBER_1 || patternId == DISCO_BARBER_2) {
    discoBarber();
  } else if (patternId == WORMS) {
    worms();
  } else if (patternId == DISCO_TWIRL) {
    discoTwirl();
  } else if (patternId == DISCO_TWIRL_2) {
    discoTwirl2();
  } else if (patternId == DAVE) {
    dave();
  }
    
  firstTimeRunningThroughPattern = false;
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

    bool printIt = millis() > previousMillis + frameDelay;
    if (printIt) {
      draw();
      previousMillis = millis();
    }

  FastLED.setBrightness(max_bright);
  FastLED.show();
}
