#include "FastLED.h"
#define qsubd(x, b)  ((x>b)?255:0)  // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)  // Analog Unsigned subtraction macro. if result <0, then => 0

#define LED_DT 2
#define COLOR_ORDER GRB       // Are they RGB, GRB or what??
#define LED_TYPE WS2812B      // Don't forget to change LEDS.addLeds
#define MAX_POWER 4000
#define VOLTS 5

uint8_t max_brightness = 255;
int hue = 50;
bool firstTimeRunningThroughPattern = true;
int rotationInMillseconds = 8000;

#define NUM_LEDS 100;         // Number of LED's.
struct CRGB leds[NUM_LEDS];   // Initialize our LED array.

void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_brightness);
  randomSeed(analogRead(0));
  set_max_power_in_volts_and_milliamps(VOLTS, MAX_POWER);
  delay(500);
}

bool direction = true;
int snakeStart = 0;
int bandLength = 100;
int borderLength = 20;
CHSV pattern(int pos, uint8_t counter, bool firstTime) {
  CHSV color;

  if (pos > snakeStart && pos < snakeStart + bandLength) {
    color = CHSV(0, 0, 0);
  } else if (pos > snakeStart - borderLength && pos < snakeStart + bandLength + borderLength) {
    color = CHSV(hue + pos + counter, 240, 255);  
  } else {
    color = CHSV(120, 255, random(20));
  }

  if (counter > NUM_LEDS * 2) counter = 0;
  EVERY_N_MILLISECONDS(15) hue += 1;

  EVERY_N_MILLISECONDS(1000/800) {
    snakeStart += direction ? 1 : -1;
    if (snakeStart + bandLength + borderLength * 2 > NUM_LEDS || snakeStart <= 0) {
      direction = !direction;
      snakeStart = max(snakeStart, 0);
      snakeStart = min(snakeStart, NUM_LEDS - bandLength - borderLength * 2);
    }
  }
  
  return color;
}

uint8_t counter = -1;
bool firstTime = true;
void loop () {
  for (int k = 0; k < NUM_LEDS - 1; k++) {
    leds[k] = pattern(k, counter, firstTime);
  }
  firstTime = false;
  counter++;
  show_at_max_brightness_for_power();
}

