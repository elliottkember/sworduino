#include <ArduinoJson.h>
#include <WiFiUdp.h>

int numberOfSparkles = 0;
bool increasing = true;
#define CIRCUMFERENCE 8;
#define SNAKES_COUNT 40
uint8_t hue = 50;
uint16_t snakes[SNAKES_COUNT];
int snake = 0;
int speed = 1;
bool readSerial = false;

int curve(int value, int difference, int maximum) {
  if (value < maximum && increasing) {
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

void setupLanternPattern() {
  // Set up Serial for lantern pattern
  Serial1.begin(9600);
  Serial1.setTimeout(20);
}

void lanternPattern() {
  if (readSerial) {
    if(Serial1.available()) {
      StaticJsonBuffer<200> jsonBuffer; 
      JsonObject& root = jsonBuffer.parseObject(Serial1);
      root.prettyPrintTo(Serial);
      
      float hueDecimal = root["hue"];
      if (hueDecimal) {
        hue = hueDecimal * 255;
      }
      
      float brightnessDecimal = root["brightness"];
      if (brightnessDecimal) {
       brightnessDecimal = brightnessDecimal * 255.0;
       if (brightnessDecimal < 3.0) brightnessDecimal = 0;
       FastLED.setBrightness(brightnessDecimal);
      }
      
      float speedDecimal = root["speed"];
      if (speedDecimal) {
        speed = (speedDecimal * 50);
      }

      jsonBuffer.clear();
    }
  } else {
    EVERY_N_MILLISECONDS(100) {
      hue -= 1;
    }
  }

  EVERY_N_MILLISECONDS(40) {
    for (int snakeIndex = 0; snakeIndex < SNAKES_COUNT; snakeIndex++) {
      uint16_t snakeHead = snakes[snakeIndex];
      if (snakeHead > 0) {
        // Snake hue
        leds[snakeHead] = CHSV(hue + snakeHead / 15 - 20, 240, 200);
        // Snake movement
        snakes[snakeIndex] += CIRCUMFERENCE;
        // Reset snake
        if (snakes[snakeIndex] >= NUM_LEDS) snakes[snakeIndex] = 0;
      }
    }
  }

  EVERY_N_MILLISECONDS(50) {
    // Snake seeding - randomly start a snake based on numberOfSparkles
    if (random16(NUM_LEDS * 0.3) < numberOfSparkles) {
      snakes[snake] = random(7);
      snake++;
      if (snake >= SNAKES_COUNT) snake = 0;
    }
  }

//  EVERY_N_MILLISECONDS(20) {
    // todo nscale8_video
    fadeToBlackBy(leds, NUM_LEDS, speed);
//  }

//  EVERY_N_MILLISECONDS(60) {
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      if (!leds[pos]) {
        leds[pos] = CHSV(hue + (pos / 15), 255, 128);
      }
    }
//  }

  EVERY_N_MILLISECONDS(200) {
    numberOfSparkles = curve(numberOfSparkles, 4, NUM_LEDS * 0.02);
  }
}
