#include <ArduinoJson.h>

DynamicJsonBuffer jsonBuffer(500);
void setupLanternPattern() {
  // Set up Serial for lantern pattern
//  Serial1.begin(9600);
//  Serial1.setTimeout(10);
}



#define CIRCUMFERENCE 22
#define SNAKES_COUNT 50
#define READ_SERIAL false

uint8_t hue = 50;
uint16_t snakes[SNAKES_COUNT];
int numberOfSparkles = 0;
int newBrightness = 128;
int newHue = hue;
int snake = 0;
int speed = 1;

void setSettingsFromSerial() {
//    if(Serial1.available()) {
//      JsonObject& root = jsonBuffer.parseObject(Serial1);
//      root.prettyPrintTo(Serial);
//  
//      float hueDecimal = root["hue"];
//      if (hueDecimal) {
//        newHue = hueDecimal * 255;
//      }
//  
//      float brightnessDecimal = root["brightness"];
//      if (brightnessDecimal) {
//        newBrightness = brightnessDecimal * 255.0;
//        if (newBrightness < 3.0) newBrightness = 0;
//      }
//  
//      float speedDecimal = root["speed"];
//      if (speedDecimal) {
//        speed = (speedDecimal * 50);
//      }
//    }

  EVERY_N_MILLISECONDS(10) {
    if (newBrightness > FastLED.getBrightness()) {
      FastLED.setBrightness(FastLED.getBrightness() + 1);
    } else if (newBrightness < FastLED.getBrightness()) {
      FastLED.setBrightness(FastLED.getBrightness() - 1);
    }
  }

  EVERY_N_MILLISECONDS(10) {
    if (newHue > hue) {
      hue++;
    } else if (newHue < hue) {
      hue--;
    }
  }
}

// The constants here have to change for APA102/WS2812B.
// The timing depends on how many LEDs there are, especially if you're using WS2812B.
#define FADE_MS 20
#define SPARKLE_MS 60
#define CURVE_MS 200
#define SEED_SNAKES_MS 50
#define MAX_SPARKLES_AS_A_PERCENTAGE_OF_NUM_LEDS 0.02
#define MOVE_SNAKES_MS 40

void lanternPattern() {
  if (READ_SERIAL) {
    setSettingsFromSerial();
  } else {
    EVERY_N_MILLISECONDS(100) {
      hue -= 1;
    }
  }

  EVERY_N_MILLISECONDS(MOVE_SNAKES_MS) {
    for (int snakeIndex = 0; snakeIndex < SNAKES_COUNT; snakeIndex++) {
      uint16_t snakeHead = snakes[snakeIndex];
      if (snakeHead > 0) {
        // Snake hue
        leds[snakeHead] = CHSV(hue + snakeHead / 10, 240, 255);
        // Snake movement
        snakes[snakeIndex] += CIRCUMFERENCE;
        // Reset snake
        if (snakes[snakeIndex] >= NUM_LEDS) snakes[snakeIndex] = 0;
      }
    }
  }

  EVERY_N_MILLISECONDS(SEED_SNAKES_MS) {
    float randomness = 0.5;
    // Snake seeding - randomly start a snake based on numberOfSparkles
    if (random16(NUM_LEDS * randomness) < numberOfSparkles) {
      snakes[snake] = random(CIRCUMFERENCE);
      snake++;
      if (snake >= SNAKES_COUNT) snake = 0;
    }
  }

  EVERY_N_MILLISECONDS(FADE_MS) {
    nscale8(leds, NUM_LEDS, 255 - speed);
  }

  EVERY_N_MILLISECONDS(SPARKLE_MS) {
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      if (!leds[pos]) leds[pos] = CHSV(hue + (pos / 15), 255, 128);
    }
  }

  EVERY_N_MILLISECONDS(CURVE_MS) {
    numberOfSparkles = curve(numberOfSparkles, 4, NUM_LEDS * MAX_SPARKLES_AS_A_PERCENTAGE_OF_NUM_LEDS);
  }
}
