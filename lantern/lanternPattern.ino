
int numberOfSparkles = 1;
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

#define SNAKES_COUNT 10
uint16_t snakes[SNAKES_COUNT];
int snake = 0;

void lanternPattern() {

  EVERY_N_MILLISECONDS(10) {
    for (int i = 0; i < SNAKES_COUNT; i++) {
      uint16_t j = snakes[i];
      if (j > 0) {
        leds[j] = CHSV(hue + ((NUM_LEDS - j) / 15), 240, 255);
  
        snakes[i] += 8;
        if (snakes[i] >= NUM_LEDS) snakes[i] = 0;
      }
    }
  }

  EVERY_N_MILLISECONDS(2000) {
    snakes[snake] = random(7);
    snake++;
    if (snake >= SNAKES_COUNT) snake = 0;
    numberOfSparkles = curve(numberOfSparkles, 1, NUM_LEDS * 0.01);
  }

  fadeToBlackBy(leds, NUM_LEDS, 8);
  
  for (int i = 0; i < numberOfSparkles; i++) {
    int pos = random16(NUM_LEDS);
    if (!leds[pos]) {
      leds[pos] = CHSV(hue + (pos / 15), 240, 255);
    }
  }
  
  EVERY_N_MILLISECONDS(100) {
    hue += 1;
  }
  
//  delay(8);
}
