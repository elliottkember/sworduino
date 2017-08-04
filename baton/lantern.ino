int numberOfSparkles = 0;

int snakeDelay = 500;

namespace Lantern {

  #define SNAKES_COUNT 20
  uint16_t snakes[SNAKES_COUNT];
  int snake = 0;
  int hue = 0;

  void draw() {
    EVERY_N_MILLISECONDS(10) {
      for (int i = 0; i < SNAKES_COUNT; i++) {
        uint16_t j = snakes[i];
        if (j > 0 && j < NUM_LEDS) {
          Global::leds[j] = CHSV(hue + ((NUM_LEDS - j) / 15), 240, 255);

          snakes[i] += 8;
          if (snakes[i] >= NUM_LEDS) snakes[i] = 0;
        }
      }
      numberOfSparkles = upAndDownBy(numberOfSparkles, 1, NUM_LEDS * 0.01, 0);
    }

    EVERY_N_MILLISECONDS(snakeDelay) {
      snakes[snake] = random(7);
      snake++;
      if (snake >= SNAKES_COUNT) snake = 0;
    }

    fadeToBlackBy(Global::leds, NUM_LEDS, 8);

    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      if (!Global::leds[pos]) {
        Global::leds[pos] = CHSV(hue + (pos / 15), 240, 128);
      }
    }

    EVERY_N_MILLISECONDS(100) {
      hue += 1;
    }
  }
}

void lanternPattern() {
  Lantern::draw();
}
