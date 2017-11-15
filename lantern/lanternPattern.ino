int numberOfSparkles = 0;
bool increasing = true;

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

#define SNAKES_COUNT 50
uint16_t snakes[SNAKES_COUNT];
int snake = 0;


#define CIRCUMFERENCE 9;

void lanternPattern() {
  EVERY_N_MILLISECONDS(40) {
    for (int i = 0; i < SNAKES_COUNT; i++) {
      uint16_t j = snakes[i];
      if (j > 0) {
        // Different hue for shooting stars
        leds[j] = CHSV(hue + j / 10, 240, 255);

        snakes[i] += CIRCUMFERENCE;
        if (snakes[i] >= NUM_LEDS) snakes[i] = 0;
      }
    }
  }

  EVERY_N_MILLISECONDS(50) {
    if (random16(NUM_LEDS * 0.5) < numberOfSparkles) {
      snakes[snake] = random(7);
      snake++;
      if (snake >= SNAKES_COUNT) snake = 0;
    }
  }

  EVERY_N_MILLISECONDS(20) {
    fadeToBlackBy(leds, NUM_LEDS, speed);
//    nscale8_video(leds, NUM_LEDS, 255-speed);
  }

  EVERY_N_MILLISECONDS(60) {
    for (int i = 0; i < numberOfSparkles; i++) {
      int pos = random16(NUM_LEDS);
      if (!leds[pos]) {
        leds[pos] = CHSV(hue + (pos / 15), 255, 128);
      }
    }
  }

  EVERY_N_MILLISECONDS(200) {
    numberOfSparkles = curve(numberOfSparkles, 4, NUM_LEDS * 0.03);
  }

//  EVERY_N_MILLISECONDS(100) {
//    hue -= 1;
//  }
}
