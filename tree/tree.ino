#include "FastLED.h"
#define LED_DATA 7
#define NUM_LEDS 200

struct CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, LED_DATA, RGB>(leds, NUM_LEDS);
  Serial.begin(57600);
  set_max_power_in_volts_and_milliamps(5, 3000);
  randomSeed(analogRead(0));
  FastLED.setDither(0);
  FastLED.delay(50);
}

namespace UpAndDownBy {
  bool increasing = true;
  // TODO increasing and numberOfSparkles should probably be passed by reference here!
  int go(int value, int difference, int max, int min) {
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
}

int upAndDownBy(int value, int difference, int max, int min) {
  return UpAndDownBy::go(value, difference, max, min);
}

namespace BeautifulSparkles {
  int numberOfSparkles = 0;
  int hue = 0;

  void changeNumberOfSparkles() {
    numberOfSparkles = upAndDownBy(numberOfSparkles, 1, 20, 1);
  }

  void changeColor() {
    hue += 1;
  }

  CRGB color(int pos) {
    return CHSV(hue + (pos / 2), 240, 255);
  }

  void drawSparkles() {
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      leds[pos] = color(pos);
    }
  }

  void draw() {
    EVERY_N_MILLISECONDS(50) {
      changeNumberOfSparkles();
      drawSparkles();
    }
    EVERY_N_MILLISECONDS(10) {
      changeColor();
    }
    fadeToBlackBy(leds, NUM_LEDS, 7);
  }
}

void beautifulSparkles() {
  BeautifulSparkles::draw();
}

void loop() {
  // put your main code here, to run repeatedly:
  beautifulSparkles();
  FastLED.show();
}


