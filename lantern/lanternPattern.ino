int numberOfSparkles = 100;
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

int upAndDownBy(int value, int difference) {
  return curve(value, difference, 20);
}

#define SNAKES_COUNT 20
uint16_t snakes[SNAKES_COUNT];
int snake = 0;

void lanternPattern() {

  EVERY_N_MILLISECONDS(10) {
    for (int i = 0; i < SNAKES_COUNT; i++) {
      uint16_t j = snakes[i];
      if (j > 0 && j < NUM_LEDS) {
        leds[j] = CHSV(hue + ((NUM_LEDS - j) / 15), 240, 255);
  
        snakes[i] += 8;
        if (snakes[i] >= NUM_LEDS) snakes[i] = 0;
      }
    }
    numberOfSparkles = curve(numberOfSparkles, 2, NUM_LEDS * 0.05);
  }

  EVERY_N_MILLISECONDS(2000) {
    snakes[snake] = random(7);
    snake++;
    if (snake >= SNAKES_COUNT) snake = 0;
  }

  fadeToBlackBy(leds, NUM_LEDS, 8);
  
  for (int i = 0; i < numberOfSparkles; i++) {
    int pos = random16(NUM_LEDS);
    if (!leds[pos]) {
      leds[pos] = CHSV(hue + (pos / 15), 240, 128);
    }
  }

  EVERY_N_MILLISECONDS(100) {
    hue += 1;
  }
}
