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

#define NUM_LEDS 1130         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

// Elliott's dreadful upAndDownBy function.
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

void setup() {
  // put your setup code here, to run once:
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

bool showingSnake = false;
int snakeStart = 0;
void snake(int every) {
  if (showingSnake) {
    leds[snakeStart] = CRGB(255, 0, 0);
    leds[snakeStart+1] = CRGB(255, 0, 0);
    snakeStart += 2;
  }
  if (snakeStart > NUM_LEDS) {
    snakeStart = 0;
    showingSnake = false;
  }
  EVERY_N_MILLISECONDS(every) {
    showingSnake = true;
  }
}

void beautifulSparkles() {
  fadeToBlackBy(leds, NUM_LEDS, 50);
  int numberOfSparkles = 12;
  for (int i = 0; i < numberOfSparkles / 3; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CRGB(255, 0, 0);
  }
  for (int i = 0; i < numberOfSparkles / 3; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CRGB(0, 255, 0);
  }
  for (int i = 0; i < numberOfSparkles / 3; i++) {
    int pos = random16(NUM_LEDS);
    leds[pos] = CRGB(0, 0, 255);
  }
}

void loop() {
  EVERY_N_MILLISECONDS(100) {
    beautifulSparkles();
  }
//  snake(1000);
  show_at_max_brightness_for_power();
}
