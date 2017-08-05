int numberOfSparkles = 0;
int snakeDelay = 2000;

namespace Lantern {

  #define SNAKES_COUNT 20
  uint16_t snakes[SNAKES_COUNT];
  int snake = 0;
  int hue = 0;
  int snakeHueOffset = 20;

  void draw() {

    // Draw snakes
    EVERY_N_MILLISECONDS(10) {
      for (int i = 0; i < SNAKES_COUNT; i++) {
        uint16_t j = snakes[i];
        if (j > 0 && j < NUM_LEDS) {
          int h = hue + ((NUM_LEDS - j) / 15) + snakeHueOffset;
          Global::leds[j] = CHSV(h, 240, 255);
          snakes[i] += 8;
          if (snakes[i] >= NUM_LEDS) snakes[i] = 0;
        }
      }
    }

    EVERY_N_MILLISECONDS(80) {
      numberOfSparkles = upAndDownBy(numberOfSparkles, 1, NUM_LEDS * 0.05, 0);
    }

    // Make a new snake
    EVERY_N_MILLISECONDS(snakeDelay) {
      snakes[snake] = random(7);
      snake++;
      if (snake >= SNAKES_COUNT) snake = 0;
    }

    // General fade
    fadeToBlackBy(Global::leds, NUM_LEDS, 18);

    // Draw sparkles
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      // if (!Global::leds[pos]) {
      int brightness = 140 + (numberOfSparkles / NUM_LEDS * 115);
        Global::leds[pos] = CHSV(hue + (pos / 15), 240, brightness) + random(10);
      // }
    }

    // Change colour
    EVERY_N_MILLISECONDS(100) {
      hue += 5;
    }
  }
}

void lantern() {
  Lantern::draw();
}
